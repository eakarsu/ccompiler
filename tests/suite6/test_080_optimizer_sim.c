int printf(const char *fmt, ...);

int OP_CONST = 0;
int OP_ADD = 1;
int OP_SUB = 2;
int OP_MUL = 3;
int OP_MOV = 4;
int OP_PRINT = 5;
int OP_NOP = 6;
int OP_LOAD = 7;
int OP_STORE = 8;

int inst_op[128];
int inst_dst[128];
int inst_src1[128];
int inst_src2[128];
int inst_count;

int regs[16];

void reset(void) {
    inst_count = 0;
    int i = 0;
    while (i < 16) { regs[i] = 0; i = i + 1; }
}

void emit(int op, int dst, int s1, int s2) {
    inst_op[inst_count] = op;
    inst_dst[inst_count] = dst;
    inst_src1[inst_count] = s1;
    inst_src2[inst_count] = s2;
    inst_count = inst_count + 1;
}

char *op_name(int op) {
    if (op == 0) return "const";
    if (op == 1) return "add";
    if (op == 2) return "sub";
    if (op == 3) return "mul";
    if (op == 4) return "mov";
    if (op == 5) return "print";
    if (op == 6) return "nop";
    if (op == 7) return "load";
    if (op == 8) return "store";
    return "???";
}

int constant_fold(void) {
    int folded = 0;
    int i = 0;
    while (i < inst_count) {
        if (inst_op[i] == OP_ADD || inst_op[i] == OP_SUB || inst_op[i] == OP_MUL) {
            int s1_const = -1;
            int s2_const = -1;
            int j = i - 1;
            while (j >= 0) {
                if (inst_op[j] == OP_CONST && inst_dst[j] == inst_src1[i]) {
                    s1_const = inst_src1[j];
                    break;
                }
                if (inst_op[j] != OP_NOP && inst_dst[j] == inst_src1[i]) break;
                j = j - 1;
            }
            j = i - 1;
            while (j >= 0) {
                if (inst_op[j] == OP_CONST && inst_dst[j] == inst_src2[i]) {
                    s2_const = inst_src1[j];
                    break;
                }
                if (inst_op[j] != OP_NOP && inst_dst[j] == inst_src2[i]) break;
                j = j - 1;
            }
            if (s1_const >= 0 && s2_const >= 0) {
                int result = 0;
                if (inst_op[i] == OP_ADD) result = s1_const + s2_const;
                if (inst_op[i] == OP_SUB) result = s1_const - s2_const;
                if (inst_op[i] == OP_MUL) result = s1_const * s2_const;
                inst_op[i] = OP_CONST;
                inst_src1[i] = result;
                inst_src2[i] = 0;
                folded = folded + 1;
            }
        }
        i = i + 1;
    }
    return folded;
}

int is_reg_used_after(int reg, int from) {
    int i = from;
    while (i < inst_count) {
        if (inst_op[i] == OP_NOP) { i = i + 1; continue; }
        if (inst_op[i] == OP_PRINT && inst_src1[i] == reg) return 1;
        if (inst_src1[i] == reg || inst_src2[i] == reg) return 1;
        if (inst_dst[i] == reg) return 0;
        i = i + 1;
    }
    return 0;
}

int dead_code_elim(void) {
    int removed = 0;
    int i = 0;
    while (i < inst_count) {
        if (inst_op[i] == OP_NOP || inst_op[i] == OP_PRINT || inst_op[i] == OP_STORE) {
            i = i + 1;
            continue;
        }
        if (inst_op[i] == OP_CONST || inst_op[i] == OP_ADD || inst_op[i] == OP_SUB ||
            inst_op[i] == OP_MUL || inst_op[i] == OP_MOV || inst_op[i] == OP_LOAD) {
            if (is_reg_used_after(inst_dst[i], i + 1) == 0) {
                inst_op[i] = OP_NOP;
                removed = removed + 1;
            }
        }
        i = i + 1;
    }
    return removed;
}

int count_real_insts(void) {
    int c = 0;
    int i = 0;
    while (i < inst_count) {
        if (inst_op[i] != OP_NOP) c = c + 1;
        i = i + 1;
    }
    return c;
}

void run(void) {
    int i = 0;
    while (i < inst_count) {
        int op = inst_op[i];
        int dst = inst_dst[i];
        int s1 = inst_src1[i];
        int s2 = inst_src2[i];
        if (op == OP_CONST) {
            regs[dst] = s1;
        } else if (op == OP_ADD) {
            regs[dst] = regs[s1] + regs[s2];
        } else if (op == OP_SUB) {
            regs[dst] = regs[s1] - regs[s2];
        } else if (op == OP_MUL) {
            regs[dst] = regs[s1] * regs[s2];
        } else if (op == OP_MOV) {
            regs[dst] = regs[s1];
        } else if (op == OP_PRINT) {
            printf("r%d=%d\n", s1, regs[s1]);
        }
        i = i + 1;
    }
}

int main(void) {
    /* Test 1: constant folding of 3 + 4 */
    reset();
    emit(OP_CONST, 0, 3, 0);     /* r0 = 3 */
    emit(OP_CONST, 1, 4, 0);     /* r1 = 4 */
    emit(OP_ADD, 2, 0, 1);       /* r2 = r0 + r1 */
    emit(OP_PRINT, 0, 2, 0);     /* print r2 */
    int f = constant_fold();
    // EXPECT: folded=1
    printf("folded=%d\n", f);
    // EXPECT: r2=7
    run();

    /* Test 2: constant folding of 5 * 6 */
    reset();
    emit(OP_CONST, 0, 5, 0);
    emit(OP_CONST, 1, 6, 0);
    emit(OP_MUL, 2, 0, 1);
    emit(OP_PRINT, 0, 2, 0);
    constant_fold();
    // EXPECT: r2=30
    run();

    /* Test 3: dead code elimination */
    reset();
    emit(OP_CONST, 0, 10, 0);    /* r0 = 10, used */
    emit(OP_CONST, 1, 20, 0);    /* r1 = 20, dead */
    emit(OP_CONST, 2, 30, 0);    /* r2 = 30, dead */
    emit(OP_PRINT, 0, 0, 0);     /* print r0 */
    int d = dead_code_elim();
    // EXPECT: dead_removed=2
    printf("dead_removed=%d\n", d);
    // EXPECT: real_insts=2
    printf("real_insts=%d\n", count_real_insts());
    // EXPECT: r0=10
    run();

    /* Test 4: chained folding */
    reset();
    emit(OP_CONST, 0, 2, 0);
    emit(OP_CONST, 1, 3, 0);
    emit(OP_ADD, 2, 0, 1);       /* r2 = 5 after fold */
    emit(OP_CONST, 3, 4, 0);
    emit(OP_MUL, 4, 2, 3);       /* r4 = 5*4 = 20 after fold */
    emit(OP_PRINT, 0, 4, 0);
    f = constant_fold();
    // EXPECT: folded_chain=2
    printf("folded_chain=%d\n", f);
    /* run a second pass to catch any remaining chained folds */
    f = constant_fold();
    // EXPECT: folded_chain2=0
    printf("folded_chain2=%d\n", f);
    // EXPECT: r4=20
    run();

    /* Test 5: no folding when not const */
    reset();
    emit(OP_LOAD, 0, 0, 0);      /* r0 from memory, not const */
    emit(OP_CONST, 1, 5, 0);
    emit(OP_ADD, 2, 0, 1);
    emit(OP_PRINT, 0, 2, 0);
    f = constant_fold();
    // EXPECT: no_fold=0
    printf("no_fold=%d\n", f);

    /* Test 6: combined optimization */
    reset();
    emit(OP_CONST, 0, 10, 0);
    emit(OP_CONST, 1, 20, 0);
    emit(OP_ADD, 2, 0, 1);       /* r2 = 30 */
    emit(OP_CONST, 3, 99, 0);    /* dead */
    emit(OP_CONST, 4, 77, 0);    /* dead */
    emit(OP_PRINT, 0, 2, 0);
    int before = count_real_insts();
    constant_fold();
    dead_code_elim();
    int after = count_real_insts();
    // EXPECT: before=6
    printf("before=%d\n", before);
    // EXPECT: after=3
    printf("after=%d\n", after);
    // EXPECT: r2=30
    run();

    /* Test 7: subtraction fold */
    reset();
    emit(OP_CONST, 0, 100, 0);
    emit(OP_CONST, 1, 42, 0);
    emit(OP_SUB, 2, 0, 1);
    emit(OP_PRINT, 0, 2, 0);
    constant_fold();
    // EXPECT: r2=58
    run();

    /* Test 8: multiple prints kept */
    reset();
    emit(OP_CONST, 0, 1, 0);
    emit(OP_CONST, 1, 2, 0);
    emit(OP_PRINT, 0, 0, 0);
    emit(OP_PRINT, 0, 1, 0);
    dead_code_elim();
    // EXPECT: real=4
    printf("real=%d\n", count_real_insts());

    return 0;
}
