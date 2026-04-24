int printf(const char *fmt, ...);

// Peephole optimization patterns
// Applies local rewriting rules to instruction sequences

enum PeepOp {
    P_MOV,       // r = a
    P_ADD,       // r = a + b
    P_SUB,       // r = a - b
    P_MUL,       // r = a * b
    P_DIV,       // r = a / b
    P_SHL,       // r = a << b
    P_NOP        // deleted instruction
};

struct PeepInst {
    int op;
    int dest;
    int src1;       // register or immediate
    int src2;       // register or immediate
    int imm1;       // 1 if src1 is immediate
    int imm2;       // 1 if src2 is immediate
};

struct PeepWindow {
    struct PeepInst insts[64];
    int count;
};

void pw_init(struct PeepWindow *pw) {
    pw->count = 0;
}

void pw_emit(struct PeepWindow *pw, int op, int dest, int s1, int im1, int s2, int im2) {
    int idx = pw->count;
    pw->insts[idx].op = op;
    pw->insts[idx].dest = dest;
    pw->insts[idx].src1 = s1;
    pw->insts[idx].imm1 = im1;
    pw->insts[idx].src2 = s2;
    pw->insts[idx].imm2 = im2;
    pw->count = pw->count + 1;
}

// Is power of 2? Returns log2, or -1 if not
int log2_of(int n) {
    if (n <= 0) return -1;
    int p = 0;
    int v = 1;
    while (v < n && p < 31) {
        v = v * 2;
        p = p + 1;
    }
    if (v == n) return p;
    return -1;
}

// Peephole: x + 0 => MOV x, x - 0 => MOV x, x * 1 => MOV x
int pattern_identity(struct PeepWindow *pw) {
    int changes = 0;
    int i;
    for (i = 0; i < pw->count; i++) {
        struct PeepInst *inst = &pw->insts[i];
        if (inst->op == P_NOP) continue;

        // x + 0 => MOV x
        if (inst->op == P_ADD && inst->imm2 && inst->src2 == 0) {
            inst->op = P_MOV;
            inst->src2 = 0;
            inst->imm2 = 0;
            changes = changes + 1;
        }
        // x - 0 => MOV x
        else if (inst->op == P_SUB && inst->imm2 && inst->src2 == 0) {
            inst->op = P_MOV;
            inst->src2 = 0;
            inst->imm2 = 0;
            changes = changes + 1;
        }
        // x * 1 => MOV x
        else if (inst->op == P_MUL && inst->imm2 && inst->src2 == 1) {
            inst->op = P_MOV;
            inst->src2 = 0;
            inst->imm2 = 0;
            changes = changes + 1;
        }
        // x / 1 => MOV x
        else if (inst->op == P_DIV && inst->imm2 && inst->src2 == 1) {
            inst->op = P_MOV;
            inst->src2 = 0;
            inst->imm2 = 0;
            changes = changes + 1;
        }
    }
    return changes;
}

// Peephole: x * 0 => MOV 0
int pattern_zero_mul(struct PeepWindow *pw) {
    int changes = 0;
    int i;
    for (i = 0; i < pw->count; i++) {
        struct PeepInst *inst = &pw->insts[i];
        if (inst->op == P_NOP) continue;

        if (inst->op == P_MUL && inst->imm2 && inst->src2 == 0) {
            inst->op = P_MOV;
            inst->src1 = 0;
            inst->imm1 = 1;
            inst->src2 = 0;
            inst->imm2 = 0;
            changes = changes + 1;
        }
    }
    return changes;
}

// Peephole: x * (power of 2) => x << log2
int pattern_strength_reduce(struct PeepWindow *pw) {
    int changes = 0;
    int i;
    for (i = 0; i < pw->count; i++) {
        struct PeepInst *inst = &pw->insts[i];
        if (inst->op == P_NOP) continue;

        if (inst->op == P_MUL && inst->imm2) {
            int lg = log2_of(inst->src2);
            if (lg > 0) {  // lg==0 means *1, handled by identity
                inst->op = P_SHL;
                inst->src2 = lg;
                changes = changes + 1;
            }
        }
    }
    return changes;
}

// Peephole: x - x => MOV 0 (when both src are same register, not immediate)
int pattern_self_sub(struct PeepWindow *pw) {
    int changes = 0;
    int i;
    for (i = 0; i < pw->count; i++) {
        struct PeepInst *inst = &pw->insts[i];
        if (inst->op == P_NOP) continue;

        if (inst->op == P_SUB && !inst->imm1 && !inst->imm2 && inst->src1 == inst->src2) {
            inst->op = P_MOV;
            inst->src1 = 0;
            inst->imm1 = 1;
            inst->src2 = 0;
            inst->imm2 = 0;
            changes = changes + 1;
        }
    }
    return changes;
}

// Peephole: MOV r, r => NOP (redundant move)
int pattern_redundant_mov(struct PeepWindow *pw) {
    int changes = 0;
    int i;
    for (i = 0; i < pw->count; i++) {
        struct PeepInst *inst = &pw->insts[i];
        if (inst->op == P_MOV && !inst->imm1 && inst->dest == inst->src1) {
            inst->op = P_NOP;
            changes = changes + 1;
        }
    }
    return changes;
}

// Run all peephole patterns until fixpoint
int run_all_peepholes(struct PeepWindow *pw) {
    int total = 0;
    int pass;
    for (pass = 0; pass < 10; pass++) {
        int ch = 0;
        ch = ch + pattern_identity(pw);
        ch = ch + pattern_zero_mul(pw);
        ch = ch + pattern_strength_reduce(pw);
        ch = ch + pattern_self_sub(pw);
        ch = ch + pattern_redundant_mov(pw);
        total = total + ch;
        if (ch == 0) break;
    }
    return total;
}

int count_active(struct PeepWindow *pw) {
    int c = 0;
    int i;
    for (i = 0; i < pw->count; i++) {
        if (pw->insts[i].op != P_NOP) c = c + 1;
    }
    return c;
}

char *op_name(int op) {
    if (op == P_MOV) return "MOV";
    if (op == P_ADD) return "ADD";
    if (op == P_SUB) return "SUB";
    if (op == P_MUL) return "MUL";
    if (op == P_DIV) return "DIV";
    if (op == P_SHL) return "SHL";
    if (op == P_NOP) return "NOP";
    return "???";
}

void dump_window(struct PeepWindow *pw) {
    int i;
    for (i = 0; i < pw->count; i++) {
        struct PeepInst *inst = &pw->insts[i];
        if (inst->op == P_NOP) continue;
        if (inst->op == P_MOV) {
            if (inst->imm1) {
                printf("  r%d = MOV %d\n", inst->dest, inst->src1);
            } else {
                printf("  r%d = MOV r%d\n", inst->dest, inst->src1);
            }
        } else {
            char *op = op_name(inst->op);
            if (inst->imm2) {
                printf("  r%d = %s r%d, %d\n", inst->dest, op, inst->src1, inst->src2);
            } else {
                printf("  r%d = %s r%d, r%d\n", inst->dest, op, inst->src1, inst->src2);
            }
        }
    }
}

int main() {
    struct PeepWindow pw;

    // Test 1: Identity patterns
    printf("=== Peephole Test 1: Identity ===\n");
    // EXPECT: === Peephole Test 1: Identity ===
    pw_init(&pw);
    pw_emit(&pw, P_ADD, 1, 0, 0, 0, 1);  // r1 = r0 + 0
    pw_emit(&pw, P_SUB, 2, 1, 0, 0, 1);  // r2 = r1 - 0
    pw_emit(&pw, P_MUL, 3, 2, 0, 1, 1);  // r3 = r2 * 1
    pw_emit(&pw, P_DIV, 4, 3, 0, 1, 1);  // r4 = r3 / 1

    int ch = run_all_peepholes(&pw);
    printf("optimizations: %d\n", ch);
    // EXPECT: optimizations: 4
    dump_window(&pw);
    // EXPECT:   r1 = MOV r0
    // EXPECT:   r2 = MOV r1
    // EXPECT:   r3 = MOV r2
    // EXPECT:   r4 = MOV r3

    // Test 2: Multiply by zero
    printf("=== Peephole Test 2: Zero multiply ===\n");
    // EXPECT: === Peephole Test 2: Zero multiply ===
    pw_init(&pw);
    pw_emit(&pw, P_MUL, 1, 0, 0, 0, 1);  // r1 = r0 * 0

    ch = run_all_peepholes(&pw);
    printf("optimizations: %d\n", ch);
    // EXPECT: optimizations: 1
    dump_window(&pw);
    // EXPECT:   r1 = MOV 0

    // Test 3: Strength reduction (multiply by power of 2)
    printf("=== Peephole Test 3: Strength reduction ===\n");
    // EXPECT: === Peephole Test 3: Strength reduction ===
    pw_init(&pw);
    pw_emit(&pw, P_MUL, 1, 0, 0, 2, 1);  // r1 = r0 * 2 -> SHL 1
    pw_emit(&pw, P_MUL, 2, 0, 0, 4, 1);  // r2 = r0 * 4 -> SHL 2
    pw_emit(&pw, P_MUL, 3, 0, 0, 8, 1);  // r3 = r0 * 8 -> SHL 3
    pw_emit(&pw, P_MUL, 4, 0, 0, 16, 1); // r4 = r0 * 16 -> SHL 4

    ch = run_all_peepholes(&pw);
    printf("optimizations: %d\n", ch);
    // EXPECT: optimizations: 4
    dump_window(&pw);
    // EXPECT:   r1 = SHL r0, 1
    // EXPECT:   r2 = SHL r0, 2
    // EXPECT:   r3 = SHL r0, 3
    // EXPECT:   r4 = SHL r0, 4

    // Test 4: Self subtraction
    printf("=== Peephole Test 4: Self sub ===\n");
    // EXPECT: === Peephole Test 4: Self sub ===
    pw_init(&pw);
    pw_emit(&pw, P_SUB, 1, 5, 0, 5, 0);  // r1 = r5 - r5

    ch = run_all_peepholes(&pw);
    printf("optimizations: %d\n", ch);
    // EXPECT: optimizations: 1
    dump_window(&pw);
    // EXPECT:   r1 = MOV 0

    // Test 5: Redundant move
    printf("=== Peephole Test 5: Redundant MOV ===\n");
    // EXPECT: === Peephole Test 5: Redundant MOV ===
    pw_init(&pw);
    pw_emit(&pw, P_MOV, 3, 3, 0, 0, 0);  // r3 = MOV r3 (redundant)
    pw_emit(&pw, P_MOV, 5, 5, 0, 0, 0);  // r5 = MOV r5 (redundant)
    pw_emit(&pw, P_MOV, 2, 7, 0, 0, 0);  // r2 = MOV r7 (NOT redundant)

    ch = run_all_peepholes(&pw);
    printf("optimizations: %d\n", ch);
    // EXPECT: optimizations: 2
    printf("active: %d\n", count_active(&pw));
    // EXPECT: active: 1
    dump_window(&pw);
    // EXPECT:   r2 = MOV r7

    // Test 6: Combined patterns
    printf("=== Peephole Test 6: Combined ===\n");
    // EXPECT: === Peephole Test 6: Combined ===
    pw_init(&pw);
    pw_emit(&pw, P_ADD, 1, 0, 0, 0, 1);   // r1 = r0 + 0 -> MOV r0
    pw_emit(&pw, P_MUL, 2, 1, 0, 8, 1);   // r2 = r1 * 8 -> SHL r1, 3
    pw_emit(&pw, P_SUB, 3, 2, 0, 0, 1);   // r3 = r2 - 0 -> MOV r2
    pw_emit(&pw, P_MUL, 4, 3, 0, 0, 1);   // r4 = r3 * 0 -> MOV 0
    pw_emit(&pw, P_SUB, 5, 7, 0, 7, 0);   // r5 = r7 - r7 -> MOV 0

    ch = run_all_peepholes(&pw);
    printf("optimizations: %d\n", ch);
    // EXPECT: optimizations: 5
    dump_window(&pw);
    // EXPECT:   r1 = MOV r0
    // EXPECT:   r2 = SHL r1, 3
    // EXPECT:   r3 = MOV r2
    // EXPECT:   r4 = MOV 0
    // EXPECT:   r5 = MOV 0

    // Test 7: Non-power-of-2 not reduced
    printf("=== Peephole Test 7: No reduce ===\n");
    // EXPECT: === Peephole Test 7: No reduce ===
    pw_init(&pw);
    pw_emit(&pw, P_MUL, 1, 0, 0, 3, 1);  // r1 = r0 * 3 (not power of 2)
    pw_emit(&pw, P_MUL, 2, 0, 0, 5, 1);  // r2 = r0 * 5 (not power of 2)
    pw_emit(&pw, P_MUL, 3, 0, 0, 7, 1);  // r3 = r0 * 7 (not power of 2)

    ch = run_all_peepholes(&pw);
    printf("optimizations: %d\n", ch);
    // EXPECT: optimizations: 0
    printf("active: %d\n", count_active(&pw));
    // EXPECT: active: 3

    // Test 8: log2 verification
    printf("=== Peephole Test 8: log2 ===\n");
    // EXPECT: === Peephole Test 8: log2 ===
    printf("log2(1) = %d\n", log2_of(1));
    // EXPECT: log2(1) = 0
    printf("log2(2) = %d\n", log2_of(2));
    // EXPECT: log2(2) = 1
    printf("log2(4) = %d\n", log2_of(4));
    // EXPECT: log2(4) = 2
    printf("log2(1024) = %d\n", log2_of(1024));
    // EXPECT: log2(1024) = 10
    printf("log2(6) = %d\n", log2_of(6));
    // EXPECT: log2(6) = -1

    printf("done\n");
    // EXPECT: done
    return 0;
}
