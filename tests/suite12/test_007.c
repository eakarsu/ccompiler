int printf(const char *fmt, ...);

// Dead code eliminator
// Marks instructions as live/dead based on usage analysis

int OP_CONST = 1;
int OP_ADD = 2;
int OP_SUB = 3;
int OP_MUL = 4;
int OP_LOAD = 5;
int OP_STORE = 6;
int OP_RET = 7;
int OP_BRANCH = 8;
int OP_CALL = 9;
int OP_PHI = 10;

struct Instruction {
    int opcode;
    int dest;
    int src1;
    int src2;
    int value;
    int live;
    int has_side_effect;
};

struct Instruction code[64];
int code_count;
int used[128];

int emit(int op, int dest, int s1, int s2, int val) {
    int idx = code_count;
    code[idx].opcode = op;
    code[idx].dest = dest;
    code[idx].src1 = s1;
    code[idx].src2 = s2;
    code[idx].value = val;
    code[idx].live = 1;
    code[idx].has_side_effect = 0;
    if (op == OP_STORE || op == OP_RET || op == OP_BRANCH || op == OP_CALL) {
        code[idx].has_side_effect = 1;
    }
    code_count = code_count + 1;
    return idx;
}

void mark_used(int reg) {
    if (reg >= 0 && reg < 128) used[reg] = 1;
}

void clear_used() {
    int i = 0;
    while (i < 128) { used[i] = 0; i = i + 1; }
}

int eliminate_dead_code() {
    int eliminated = 0;
    int changed = 1;

    while (changed) {
        changed = 0;
        clear_used();

        // Mark all registers used by side-effecting instructions or live instructions
        int i = 0;
        while (i < code_count) {
            if (code[i].live && (code[i].has_side_effect || used[code[i].dest])) {
                mark_used(code[i].src1);
                mark_used(code[i].src2);
            }
            i = i + 1;
        }

        // Also mark regs used by live instructions whose dest is used
        // Need multiple passes
        int pass = 0;
        while (pass < 5) {
            i = 0;
            while (i < code_count) {
                if (code[i].live && used[code[i].dest]) {
                    if (!used[code[i].src1] && code[i].src1 >= 0) {
                        used[code[i].src1] = 1;
                        changed = 1;
                    }
                    if (!used[code[i].src2] && code[i].src2 >= 0) {
                        used[code[i].src2] = 1;
                        changed = 1;
                    }
                }
                i = i + 1;
            }
            pass = pass + 1;
        }

        // Kill non-side-effecting instructions whose dest is not used
        changed = 0;
        i = 0;
        while (i < code_count) {
            if (code[i].live && !code[i].has_side_effect) {
                if (!used[code[i].dest]) {
                    code[i].live = 0;
                    eliminated = eliminated + 1;
                    changed = 1;
                }
            }
            i = i + 1;
        }
    }

    return eliminated;
}

int count_live() {
    int c = 0;
    int i = 0;
    while (i < code_count) {
        if (code[i].live) c = c + 1;
        i = i + 1;
    }
    return c;
}

char *op_name(int op) {
    if (op == OP_CONST) return "CONST";
    if (op == OP_ADD) return "ADD";
    if (op == OP_SUB) return "SUB";
    if (op == OP_MUL) return "MUL";
    if (op == OP_LOAD) return "LOAD";
    if (op == OP_STORE) return "STORE";
    if (op == OP_RET) return "RET";
    if (op == OP_BRANCH) return "BR";
    if (op == OP_CALL) return "CALL";
    if (op == OP_PHI) return "PHI";
    return "?";
}

void print_live() {
    int i = 0;
    while (i < code_count) {
        if (code[i].live) {
            printf("  [%d] %s r%d\n", i, op_name(code[i].opcode), code[i].dest);
        }
        i = i + 1;
    }
}

int main() {
    code_count = 0;

    // Program:
    //   r0 = CONST 10
    //   r1 = CONST 20
    //   r2 = ADD r0, r1       (used by RET)
    //   r3 = CONST 99         (dead - not used)
    //   r4 = MUL r3, r0       (dead - not used)
    //   r5 = CONST 5          (dead - not used)
    //   RET r2

    emit(OP_CONST, 0, -1, -1, 10);
    emit(OP_CONST, 1, -1, -1, 20);
    emit(OP_ADD, 2, 0, 1, 0);
    emit(OP_CONST, 3, -1, -1, 99);
    emit(OP_MUL, 4, 3, 0, 0);
    emit(OP_CONST, 5, -1, -1, 5);
    emit(OP_RET, -1, 2, -1, 0);

    printf("before DCE: %d live\n", count_live());
    // EXPECT: before DCE: 7 live

    int elim = eliminate_dead_code();
    printf("eliminated: %d\n", elim);
    // EXPECT: eliminated: 3

    printf("after DCE: %d live\n", count_live());
    // EXPECT: after DCE: 4 live

    printf("r3 live: %d\n", code[3].live);
    // EXPECT: r3 live: 0
    printf("r4 live: %d\n", code[4].live);
    // EXPECT: r4 live: 0
    printf("r5 live: %d\n", code[5].live);
    // EXPECT: r5 live: 0

    // Second test: store keeps things alive
    code_count = 0;
    emit(OP_CONST, 0, -1, -1, 42);
    emit(OP_CONST, 1, -1, -1, 100);
    emit(OP_ADD, 2, 0, 1, 0);
    emit(OP_STORE, -1, 2, -1, 0);
    emit(OP_CONST, 3, -1, -1, 77);
    emit(OP_RET, -1, -1, -1, 0);

    elim = eliminate_dead_code();
    printf("store test elim: %d\n", elim);
    // EXPECT: store test elim: 1

    printf("store test live: %d\n", count_live());
    // EXPECT: store test live: 5

    printf("dead r3: %d\n", code[4].live);
    // EXPECT: dead r3: 0

    // Third test: call has side effects, kept alive
    code_count = 0;
    emit(OP_CONST, 0, -1, -1, 1);
    emit(OP_CALL, 10, 0, -1, 0);
    emit(OP_CONST, 1, -1, -1, 2);
    emit(OP_RET, -1, -1, -1, 0);

    elim = eliminate_dead_code();
    printf("call test elim: %d\n", elim);
    // EXPECT: call test elim: 1

    printf("call kept: %d\n", code[1].live);
    // EXPECT: call kept: 1
    printf("dead const: %d\n", code[2].live);
    // EXPECT: dead const: 0

    printf("dce done\n");
    // EXPECT: dce done

    return 0;
}
