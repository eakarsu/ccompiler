int printf(const char *fmt, ...);

// Peephole optimizer
// Applies local pattern-matching optimizations to instruction sequences

int OP_MOV = 1;
int OP_ADD = 2;
int OP_SUB = 3;
int OP_MUL = 4;
int OP_LOAD = 5;
int OP_STORE = 6;
int OP_NOP = 7;
int OP_CMP = 8;
int OP_JMP = 9;
int OP_LOADI = 10;
int OP_LSL = 11;

struct Inst {
    int op;
    int rd;
    int rs1;
    int rs2;
    int imm;
    int dead;
};

struct Inst code[128];
int code_len;

void emit(int op, int rd, int rs1, int rs2, int imm) {
    code[code_len].op = op;
    code[code_len].rd = rd;
    code[code_len].rs1 = rs1;
    code[code_len].rs2 = rs2;
    code[code_len].imm = imm;
    code[code_len].dead = 0;
    code_len = code_len + 1;
}

// Pattern: MOV rx, rx -> NOP
int elim_self_mov() {
    int count = 0;
    int i = 0;
    while (i < code_len) {
        if (!code[i].dead && code[i].op == OP_MOV) {
            if (code[i].rd == code[i].rs1) {
                code[i].dead = 1;
                count = count + 1;
            }
        }
        i = i + 1;
    }
    return count;
}

// Pattern: ADD rx, ry, #0 -> MOV rx, ry
int elim_add_zero() {
    int count = 0;
    int i = 0;
    while (i < code_len) {
        if (!code[i].dead && code[i].op == OP_ADD) {
            if (code[i].rs2 == -1 && code[i].imm == 0) {
                code[i].op = OP_MOV;
                code[i].rs2 = -1;
                count = count + 1;
            }
        }
        i = i + 1;
    }
    return count;
}

// Pattern: MUL rx, ry, #1 -> MOV rx, ry
int elim_mul_one() {
    int count = 0;
    int i = 0;
    while (i < code_len) {
        if (!code[i].dead && code[i].op == OP_MUL) {
            if (code[i].rs2 == -1 && code[i].imm == 1) {
                code[i].op = OP_MOV;
                code[i].rs2 = -1;
                count = count + 1;
            }
        }
        i = i + 1;
    }
    return count;
}

// Pattern: MUL rx, ry, #2 -> LSL rx, ry, #1 (shift left)
int strength_reduce_mul_pow2() {
    int count = 0;
    int i = 0;
    while (i < code_len) {
        if (!code[i].dead && code[i].op == OP_MUL && code[i].rs2 == -1) {
            int v = code[i].imm;
            if (v > 1 && (v & (v - 1)) == 0) {
                int shift = 0;
                int tmp = v;
                while (tmp > 1) { tmp = tmp / 2; shift = shift + 1; }
                code[i].op = OP_LSL;
                code[i].imm = shift;
                count = count + 1;
            }
        }
        i = i + 1;
    }
    return count;
}

// Pattern: SUB rx, ry, #0 -> MOV rx, ry
int elim_sub_zero() {
    int count = 0;
    int i = 0;
    while (i < code_len) {
        if (!code[i].dead && code[i].op == OP_SUB) {
            if (code[i].rs2 == -1 && code[i].imm == 0) {
                code[i].op = OP_MOV;
                count = count + 1;
            }
        }
        i = i + 1;
    }
    return count;
}

// Pattern: consecutive STORE then LOAD same addr -> eliminate LOAD
int elim_store_load() {
    int count = 0;
    int i = 0;
    while (i + 1 < code_len) {
        if (!code[i].dead && !code[i + 1].dead) {
            if (code[i].op == OP_STORE && code[i + 1].op == OP_LOAD) {
                if (code[i].imm == code[i + 1].imm && code[i].rs1 == code[i + 1].rs1) {
                    // Replace load with mov
                    code[i + 1].op = OP_MOV;
                    code[i + 1].rs1 = code[i].rd;
                    code[i + 1].imm = 0;
                    count = count + 1;
                }
            }
        }
        i = i + 1;
    }
    return count;
}

int run_peephole() {
    int total = 0;
    total = total + elim_self_mov();
    total = total + elim_add_zero();
    total = total + elim_mul_one();
    total = total + strength_reduce_mul_pow2();
    total = total + elim_sub_zero();
    total = total + elim_store_load();
    return total;
}

int count_live() {
    int c = 0;
    int i = 0;
    while (i < code_len) {
        if (!code[i].dead) c = c + 1;
        i = i + 1;
    }
    return c;
}

char *op_name(int op) {
    if (op == OP_MOV) return "MOV";
    if (op == OP_ADD) return "ADD";
    if (op == OP_SUB) return "SUB";
    if (op == OP_MUL) return "MUL";
    if (op == OP_LOAD) return "LOAD";
    if (op == OP_STORE) return "STORE";
    if (op == OP_NOP) return "NOP";
    if (op == OP_CMP) return "CMP";
    if (op == OP_JMP) return "JMP";
    if (op == OP_LOADI) return "LOADI";
    if (op == OP_LSL) return "LSL";
    return "???";
}

int main() {
    code_len = 0;

    // Self-move: MOV r0, r0
    emit(OP_MOV, 0, 0, -1, 0);
    // Add zero: ADD r1, r2, #0
    emit(OP_ADD, 1, 2, -1, 0);
    // Mul by 1: MUL r3, r4, #1
    emit(OP_MUL, 3, 4, -1, 1);
    // Mul by 4 (power of 2): MUL r5, r6, #4
    emit(OP_MUL, 5, 6, -1, 4);
    // Sub zero: SUB r7, r8, #0
    emit(OP_SUB, 7, 8, -1, 0);
    // Store then load same location
    emit(OP_STORE, 9, 10, -1, 100);
    emit(OP_LOAD, 11, 10, -1, 100);
    // Normal add (should not be optimized)
    emit(OP_ADD, 12, 13, 14, 0);
    // Mul by 8
    emit(OP_MUL, 15, 16, -1, 8);

    printf("before peephole: %d insts\n", code_len);
    // EXPECT: before peephole: 9 insts

    int opts = run_peephole();
    printf("optimizations: %d\n", opts);
    // EXPECT: optimizations: 7

    printf("live after: %d\n", count_live());
    // EXPECT: live after: 8

    // Check individual results
    printf("self-mov dead: %d\n", code[0].dead);
    // EXPECT: self-mov dead: 1

    printf("add-zero became: %s\n", op_name(code[1].op));
    // EXPECT: add-zero became: MOV

    printf("mul-one became: %s\n", op_name(code[2].op));
    // EXPECT: mul-one became: MOV

    printf("mul-4 became: %s imm=%d\n", op_name(code[3].op), code[3].imm);
    // EXPECT: mul-4 became: LSL imm=2

    printf("sub-zero became: %s\n", op_name(code[4].op));
    // EXPECT: sub-zero became: MOV

    printf("store-load: store=%s load_became=%s\n", op_name(code[5].op), op_name(code[6].op));
    // EXPECT: store-load: store=STORE load_became=MOV

    printf("normal add: %s\n", op_name(code[7].op));
    // EXPECT: normal add: ADD

    printf("mul-8 became: %s imm=%d\n", op_name(code[8].op), code[8].imm);
    // EXPECT: mul-8 became: LSL imm=3

    // Second pass test - verify idempotence
    int opts2 = run_peephole();
    printf("second pass opts: %d\n", opts2);
    // EXPECT: second pass opts: 0

    printf("peephole done\n");
    // EXPECT: peephole done

    return 0;
}
