int printf(const char *fmt, ...);

// Constant folding optimization
// Simulates an IR with constant folding pass that simplifies expressions

enum IROpcode {
    IR_CONST,
    IR_ADD,
    IR_SUB,
    IR_MUL,
    IR_DIV,
    IR_NEG,
    IR_LOAD,
    IR_NOP
};

struct IRInst {
    int opcode;
    int dest;       // virtual register
    int op1;        // source reg or constant value
    int op2;        // source reg (for binops)
    int is_const;   // has this been folded to a constant?
    int const_val;  // folded value
};

struct IRProgram {
    struct IRInst insts[64];
    int count;
};

void ir_init(struct IRProgram *prog) {
    prog->count = 0;
}

int emit_const(struct IRProgram *prog, int dest, int val) {
    int idx = prog->count;
    prog->insts[idx].opcode = IR_CONST;
    prog->insts[idx].dest = dest;
    prog->insts[idx].op1 = val;
    prog->insts[idx].op2 = 0;
    prog->insts[idx].is_const = 1;
    prog->insts[idx].const_val = val;
    prog->count = prog->count + 1;
    return idx;
}

int emit_binop(struct IRProgram *prog, int opcode, int dest, int src1, int src2) {
    int idx = prog->count;
    prog->insts[idx].opcode = opcode;
    prog->insts[idx].dest = dest;
    prog->insts[idx].op1 = src1;
    prog->insts[idx].op2 = src2;
    prog->insts[idx].is_const = 0;
    prog->insts[idx].const_val = 0;
    prog->count = prog->count + 1;
    return idx;
}

int emit_load(struct IRProgram *prog, int dest, int addr_reg) {
    int idx = prog->count;
    prog->insts[idx].opcode = IR_LOAD;
    prog->insts[idx].dest = dest;
    prog->insts[idx].op1 = addr_reg;
    prog->insts[idx].op2 = 0;
    prog->insts[idx].is_const = 0;
    prog->insts[idx].const_val = 0;
    prog->count = prog->count + 1;
    return idx;
}

// Find instruction that defines a given register
int find_def(struct IRProgram *prog, int reg) {
    int i;
    for (i = 0; i < prog->count; i++) {
        if (prog->insts[i].dest == reg && prog->insts[i].opcode != IR_NOP) {
            return i;
        }
    }
    return -1;
}

// Constant folding pass
int fold_constants(struct IRProgram *prog) {
    int changes = 0;
    int i;

    for (i = 0; i < prog->count; i++) {
        struct IRInst *inst = &prog->insts[i];
        if (inst->opcode == IR_NOP || inst->opcode == IR_CONST || inst->opcode == IR_LOAD) {
            continue;
        }

        int def1 = find_def(prog, inst->op1);
        int def2 = find_def(prog, inst->op2);

        if (def1 < 0 || def2 < 0) continue;
        if (!prog->insts[def1].is_const || !prog->insts[def2].is_const) continue;

        int v1 = prog->insts[def1].const_val;
        int v2 = prog->insts[def2].const_val;
        int result = 0;

        if (inst->opcode == IR_ADD) result = v1 + v2;
        else if (inst->opcode == IR_SUB) result = v1 - v2;
        else if (inst->opcode == IR_MUL) result = v1 * v2;
        else if (inst->opcode == IR_DIV) {
            if (v2 == 0) continue;
            result = v1 / v2;
        } else continue;

        // Replace with constant
        inst->opcode = IR_CONST;
        inst->op1 = result;
        inst->op2 = 0;
        inst->is_const = 1;
        inst->const_val = result;
        changes = changes + 1;
    }
    return changes;
}

// Count non-NOP instructions
int count_active(struct IRProgram *prog) {
    int c = 0;
    int i;
    for (i = 0; i < prog->count; i++) {
        if (prog->insts[i].opcode != IR_NOP) c = c + 1;
    }
    return c;
}

// Count constant instructions
int count_consts(struct IRProgram *prog) {
    int c = 0;
    int i;
    for (i = 0; i < prog->count; i++) {
        if (prog->insts[i].is_const) c = c + 1;
    }
    return c;
}

char *opcode_name(int op) {
    if (op == IR_CONST) return "CONST";
    if (op == IR_ADD) return "ADD";
    if (op == IR_SUB) return "SUB";
    if (op == IR_MUL) return "MUL";
    if (op == IR_DIV) return "DIV";
    if (op == IR_NEG) return "NEG";
    if (op == IR_LOAD) return "LOAD";
    if (op == IR_NOP) return "NOP";
    return "???";
}

void dump_ir(struct IRProgram *prog) {
    int i;
    for (i = 0; i < prog->count; i++) {
        struct IRInst *inst = &prog->insts[i];
        if (inst->opcode == IR_CONST) {
            printf("  r%d = CONST %d\n", inst->dest, inst->op1);
        } else if (inst->opcode == IR_LOAD) {
            printf("  r%d = LOAD r%d\n", inst->dest, inst->op1);
        } else if (inst->opcode != IR_NOP) {
            printf("  r%d = %s r%d, r%d\n", inst->dest, opcode_name(inst->opcode), inst->op1, inst->op2);
        }
    }
}

int main() {
    struct IRProgram prog;

    // Test 1: 3 + 4 => fold to 7
    printf("=== Fold Test 1: 3+4 ===\n");
    // EXPECT: === Fold Test 1: 3+4 ===
    ir_init(&prog);
    emit_const(&prog, 0, 3);
    emit_const(&prog, 1, 4);
    emit_binop(&prog, IR_ADD, 2, 0, 1);

    printf("before:\n");
    // EXPECT: before:
    dump_ir(&prog);
    // EXPECT:   r0 = CONST 3
    // EXPECT:   r1 = CONST 4
    // EXPECT:   r2 = ADD r0, r1

    int ch1 = fold_constants(&prog);
    printf("after (%d folds):\n", ch1);
    // EXPECT: after (1 folds):
    dump_ir(&prog);
    // EXPECT:   r0 = CONST 3
    // EXPECT:   r1 = CONST 4
    // EXPECT:   r2 = CONST 7
    printf("r2 val: %d\n", prog.insts[2].const_val);
    // EXPECT: r2 val: 7

    // Test 2: (2 * 5) + (10 / 2) => multi-step folding
    printf("=== Fold Test 2: (2*5)+(10/2) ===\n");
    // EXPECT: === Fold Test 2: (2*5)+(10/2) ===
    ir_init(&prog);
    emit_const(&prog, 0, 2);   // r0 = 2
    emit_const(&prog, 1, 5);   // r1 = 5
    emit_binop(&prog, IR_MUL, 2, 0, 1); // r2 = r0 * r1
    emit_const(&prog, 3, 10);  // r3 = 10
    emit_const(&prog, 4, 2);   // r4 = 2
    emit_binop(&prog, IR_DIV, 5, 3, 4); // r5 = r3 / r4
    emit_binop(&prog, IR_ADD, 6, 2, 5); // r6 = r2 + r5

    int pass1 = fold_constants(&prog);
    printf("pass 1: %d folds\n", pass1);
    // EXPECT: pass 1: 3 folds
    int pass2 = fold_constants(&prog);
    printf("pass 2: %d folds\n", pass2);
    // EXPECT: pass 2: 0 folds
    printf("r6 val: %d\n", prog.insts[6].const_val);
    // EXPECT: r6 val: 15
    printf("all const: %d\n", count_consts(&prog));
    // EXPECT: all const: 7

    // Test 3: Non-foldable (uses LOAD)
    printf("=== Fold Test 3: Non-foldable ===\n");
    // EXPECT: === Fold Test 3: Non-foldable ===
    ir_init(&prog);
    emit_const(&prog, 0, 100);
    emit_load(&prog, 1, 0);   // r1 = LOAD r0 (not const)
    emit_const(&prog, 2, 5);
    emit_binop(&prog, IR_ADD, 3, 1, 2); // can't fold: r1 is not const

    int ch3 = fold_constants(&prog);
    printf("folds: %d\n", ch3);
    // EXPECT: folds: 0
    printf("r3 is_const: %d\n", prog.insts[3].is_const);
    // EXPECT: r3 is_const: 0

    // Test 4: Chain: (3+4) * (5-2) - 1
    printf("=== Fold Test 4: Chain ===\n");
    // EXPECT: === Fold Test 4: Chain ===
    ir_init(&prog);
    emit_const(&prog, 0, 3);
    emit_const(&prog, 1, 4);
    emit_binop(&prog, IR_ADD, 2, 0, 1);  // 7
    emit_const(&prog, 3, 5);
    emit_const(&prog, 4, 2);
    emit_binop(&prog, IR_SUB, 5, 3, 4);  // 3
    emit_binop(&prog, IR_MUL, 6, 2, 5);  // 21
    emit_const(&prog, 7, 1);
    emit_binop(&prog, IR_SUB, 8, 6, 7);  // 20

    int total = 0;
    int p;
    for (p = 0; p < 5; p++) {
        int f = fold_constants(&prog);
        total = total + f;
        if (f == 0) break;
    }
    printf("total folds: %d\n", total);
    // EXPECT: total folds: 4
    printf("r8 val: %d\n", prog.insts[8].const_val);
    // EXPECT: r8 val: 20

    printf("done\n");
    // EXPECT: done
    return 0;
}
