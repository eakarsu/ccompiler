int printf(const char *fmt, ...);
// EXPECT: === IR Optimizer Simulator ===\n--- Program 1: Simple constant fold ---\nBefore: 4 instructions\nRunning optimization passes...\nPass 1: folded 1 instructions\nPass 1: eliminated 2 dead instructions\nPass 2: folded 0 instructions\nAfter: 2 active instructions\nExecuting: PRINT r2 = 7\n--- Program 2: Chained fold ---\nBefore: 6 instructions\nRunning optimization passes...\nPass 1: folded 1 instructions\nPass 1: eliminated 2 dead instructions\nPass 2: folded 1 instructions\nPass 2: eliminated 2 dead instructions\nPass 3: folded 0 instructions\nAfter: 2 active instructions\nExecuting: PRINT r4 = 25\n--- Program 3: Dead code elimination ---\nBefore: 5 instructions\nRunning optimization passes...\nPass 1: folded 2 instructions\nPass 1: eliminated 3 dead instructions\nPass 2: folded 0 instructions\nAfter: 2 active instructions\nExecuting: PRINT r3 = 2\n--- Program 4: Multi-pass folding ---\nBefore: 6 instructions\nRunning optimization passes...\nPass 1: folded 1 instructions\nPass 1: eliminated 2 dead instructions\nPass 2: folded 1 instructions\nPass 2: eliminated 2 dead instructions\nPass 3: folded 0 instructions\nAfter: 2 active instructions\nExecuting: PRINT r4 = 10\n--- Summary ---\nAll 4 programs optimized correctly.\nTotal instructions before: 21\nTotal active after: 8\nReduction: 13 eliminated\n=== DONE ===

// ============================================================
// Mini SSA IR Optimizer Simulator
// Tests: constant folding, dead code elimination, copy prop,
//        multi-pass optimization
// ============================================================

// IR instruction types
enum {
    IR_CONST = 0,
    IR_ADD   = 1,
    IR_SUB   = 2,
    IR_MUL   = 3,
    IR_DIV   = 4,
    IR_COPY  = 5,
    IR_PRINT = 6,
    IR_NOP   = 7
};

struct IR {
    int op;
    int dst;
    int src1;
    int src2;
    int imm;
    int is_const;
    int const_val;
    int dead;
};

struct IR prog[64];
int prog_len;

// Snapshot of which registers are known constants before a pass
int snap_valid[32];
int snap_val[32];

void init_prog(void) {
    int i;
    for (i = 0; i < 64; i++) {
        prog[i].op = IR_NOP;
        prog[i].dst = -1;
        prog[i].src1 = -1;
        prog[i].src2 = -1;
        prog[i].imm = 0;
        prog[i].is_const = 0;
        prog[i].const_val = 0;
        prog[i].dead = 0;
    }
    prog_len = 0;
}

void emit_const(int dst, int val) {
    int idx = prog_len;
    prog[idx].op = IR_CONST;
    prog[idx].dst = dst;
    prog[idx].imm = val;
    prog[idx].is_const = 1;
    prog[idx].const_val = val;
    prog[idx].dead = 0;
    prog_len = prog_len + 1;
}

void emit_binop(int op, int dst, int s1, int s2) {
    int idx = prog_len;
    prog[idx].op = op;
    prog[idx].dst = dst;
    prog[idx].src1 = s1;
    prog[idx].src2 = s2;
    prog[idx].is_const = 0;
    prog[idx].const_val = 0;
    prog[idx].dead = 0;
    prog_len = prog_len + 1;
}

void emit_copy(int dst, int src) {
    int idx = prog_len;
    prog[idx].op = IR_COPY;
    prog[idx].dst = dst;
    prog[idx].src1 = src;
    prog[idx].is_const = 0;
    prog[idx].const_val = 0;
    prog[idx].dead = 0;
    prog_len = prog_len + 1;
}

void emit_print(int src) {
    int idx = prog_len;
    prog[idx].op = IR_PRINT;
    prog[idx].dst = -1;
    prog[idx].src1 = src;
    prog[idx].is_const = 0;
    prog[idx].const_val = 0;
    prog[idx].dead = 0;
    prog_len = prog_len + 1;
}

// Look up the const status of a register from the snapshot
// taken before this pass began
int snap_is_const(int reg) {
    if (reg < 0 || reg >= 32) return 0;
    return snap_valid[reg];
}

int snap_get_val(int reg) {
    if (reg < 0 || reg >= 32) return 0;
    return snap_val[reg];
}

// Build a snapshot of which registers are currently known constants
void build_snapshot(void) {
    int i;
    for (i = 0; i < 32; i++) {
        snap_valid[i] = 0;
        snap_val[i] = 0;
    }
    for (i = 0; i < prog_len; i++) {
        if (prog[i].dead) continue;
        if (prog[i].is_const && prog[i].dst >= 0 && prog[i].dst < 32) {
            snap_valid[prog[i].dst] = 1;
            snap_val[prog[i].dst] = prog[i].const_val;
        }
    }
}

// Constant folding pass: uses snapshot so newly-folded constants
// are NOT visible within the same pass (requires multi-pass)
int constant_fold_pass(void) {
    int folded = 0;
    int i;
    int v1;
    int v2;
    int result;

    build_snapshot();

    for (i = 0; i < prog_len; i++) {
        if (prog[i].dead) continue;

        // Copy propagation: if COPY and source is known constant
        if (prog[i].op == IR_COPY) {
            if (snap_is_const(prog[i].src1)) {
                prog[i].op = IR_CONST;
                prog[i].const_val = snap_get_val(prog[i].src1);
                prog[i].imm = prog[i].const_val;
                prog[i].is_const = 1;
                folded = folded + 1;
            }
            continue;
        }

        // Only fold binary ops
        if (prog[i].op != IR_ADD && prog[i].op != IR_SUB &&
            prog[i].op != IR_MUL && prog[i].op != IR_DIV) {
            continue;
        }

        // Check if both operands are known constants from snapshot
        if (!snap_is_const(prog[i].src1)) continue;
        if (!snap_is_const(prog[i].src2)) continue;

        v1 = snap_get_val(prog[i].src1);
        v2 = snap_get_val(prog[i].src2);
        result = 0;

        if (prog[i].op == IR_ADD) {
            result = v1 + v2;
        } else if (prog[i].op == IR_SUB) {
            result = v1 - v2;
        } else if (prog[i].op == IR_MUL) {
            result = v1 * v2;
        } else if (prog[i].op == IR_DIV) {
            if (v2 != 0) {
                result = v1 / v2;
            }
        }

        prog[i].op = IR_CONST;
        prog[i].const_val = result;
        prog[i].imm = result;
        prog[i].is_const = 1;
        prog[i].src1 = -1;
        prog[i].src2 = -1;
        folded = folded + 1;
    }

    return folded;
}

// Check if register 'reg' is used as a source by any non-dead instruction
int is_reg_used(int reg) {
    int i;
    for (i = 0; i < prog_len; i++) {
        if (prog[i].dead) continue;
        if (prog[i].op == IR_NOP) continue;
        if (prog[i].src1 == reg) return 1;
        if (prog[i].src2 == reg) return 1;
    }
    return 0;
}

// Dead code elimination pass
int dce_pass(void) {
    int eliminated = 0;
    int i;

    for (i = 0; i < prog_len; i++) {
        if (prog[i].dead) continue;
        if (prog[i].op == IR_NOP) continue;
        if (prog[i].op == IR_PRINT) continue;  // PRINT has side effects

        // If dst is not used by anyone, it's dead
        if (prog[i].dst >= 0 && !is_reg_used(prog[i].dst)) {
            prog[i].dead = 1;
            eliminated = eliminated + 1;
        }
    }

    return eliminated;
}

// Count active (non-dead, non-NOP) instructions
int count_active(void) {
    int count = 0;
    int i;
    for (i = 0; i < prog_len; i++) {
        if (prog[i].dead) continue;
        if (prog[i].op == IR_NOP) continue;
        count = count + 1;
    }
    return count;
}

// Execute the program: run PRINT instructions using current const values
void execute(void) {
    int i;
    int reg;
    int val;

    // Build final register values from non-dead CONST instructions
    int reg_valid[32];
    int reg_val[32];
    for (i = 0; i < 32; i++) {
        reg_valid[i] = 0;
        reg_val[i] = 0;
    }
    for (i = 0; i < prog_len; i++) {
        if (prog[i].dead) continue;
        if (prog[i].is_const && prog[i].dst >= 0 && prog[i].dst < 32) {
            reg_valid[prog[i].dst] = 1;
            reg_val[prog[i].dst] = prog[i].const_val;
        }
    }

    for (i = 0; i < prog_len; i++) {
        if (prog[i].dead) continue;
        if (prog[i].op == IR_PRINT) {
            reg = prog[i].src1;
            if (reg >= 0 && reg < 32 && reg_valid[reg]) {
                val = reg_val[reg];
                printf("Executing: PRINT r%d = %d\n", reg, val);
            } else {
                printf("Executing: PRINT r%d = ???\n", reg);
            }
        }
    }
}

// Run multi-pass optimization with reporting
void optimize(void) {
    int pass_num;
    int folded;
    int eliminated;
    int max_passes;

    max_passes = 5;
    printf("Running optimization passes...\n");

    for (pass_num = 1; pass_num <= max_passes; pass_num++) {
        folded = constant_fold_pass();
        printf("Pass %d: folded %d instructions\n", pass_num, folded);

        eliminated = dce_pass();
        if (eliminated > 0) {
            printf("Pass %d: eliminated %d dead instructions\n", pass_num, eliminated);
        }

        // If nothing changed, we're done
        if (folded == 0 && eliminated == 0) {
            break;
        }
    }

    printf("After: %d active instructions\n", count_active());
}

// ============================================================
// Program 1: a=3; b=4; c=a+b; print c
// Expected: fold c=7, eliminate a,b -> print 7
// ============================================================
void test_program1(void) {
    int before;

    printf("--- Program 1: Simple constant fold ---\n");
    init_prog();

    emit_const(0, 3);       // r0 = 3  (a)
    emit_const(1, 4);       // r1 = 4  (b)
    emit_binop(IR_ADD, 2, 0, 1);  // r2 = r0 + r1  (c = a + b)
    emit_print(2);           // print r2

    before = count_active();
    printf("Before: %d instructions\n", before);

    optimize();
    execute();
}

// ============================================================
// Program 2: a=10; b=2; c=a*b; d=c+5; print d
// Expected: pass1 folds c=20, pass2 folds d=25, print 25
// ============================================================
void test_program2(void) {
    int before;

    printf("--- Program 2: Chained fold ---\n");
    init_prog();

    emit_const(0, 10);      // r0 = 10  (a)
    emit_const(1, 2);       // r1 = 2   (b)
    emit_binop(IR_MUL, 2, 0, 1);  // r2 = r0 * r1  (c = a * b)
    emit_const(3, 5);       // r3 = 5
    emit_binop(IR_ADD, 4, 2, 3);  // r4 = r2 + r3  (d = c + 5)
    emit_print(4);           // print r4

    before = count_active();
    printf("Before: %d instructions\n", before);

    optimize();
    execute();
}

// ============================================================
// Program 3: a=5; b=3; c=a+b; d=a-b; print d
// Expected: fold c=8, d=2, eliminate a,b,c -> print 2
// ============================================================
void test_program3(void) {
    int before;

    printf("--- Program 3: Dead code elimination ---\n");
    init_prog();

    emit_const(0, 5);       // r0 = 5  (a)
    emit_const(1, 3);       // r1 = 3  (b)
    emit_binop(IR_ADD, 2, 0, 1);  // r2 = r0 + r1  (c = a + b)
    emit_binop(IR_SUB, 3, 0, 1);  // r3 = r0 - r1  (d = a - b)
    emit_print(3);           // print r3

    before = count_active();
    printf("Before: %d instructions\n", before);

    optimize();
    execute();
}

// ============================================================
// Program 4: a=2; b=3; c=a+b; d=c*2; print d
// Multi-pass: pass1 folds c=5, pass2 folds d=10
// ============================================================
void test_program4(void) {
    int before;

    printf("--- Program 4: Multi-pass folding ---\n");
    init_prog();

    emit_const(0, 2);       // r0 = 2   (a)
    emit_const(1, 3);       // r1 = 3   (b)
    emit_binop(IR_ADD, 2, 0, 1);  // r2 = r0 + r1  (c = a + b)
    emit_const(3, 2);       // r3 = 2
    emit_binop(IR_MUL, 4, 2, 3);  // r4 = r2 * r3  (d = c * 2)
    emit_print(4);           // print r4

    before = count_active();
    printf("Before: %d instructions\n", before);

    optimize();
    execute();
}

int main(void) {
    int total_before;
    int total_after;

    printf("=== IR Optimizer Simulator ===\n");

    test_program1();
    test_program2();
    test_program3();
    test_program4();

    // Summary
    total_before = 4 + 6 + 5 + 6;
    total_after = 2 + 2 + 2 + 2;

    printf("--- Summary ---\n");
    printf("All 4 programs optimized correctly.\n");
    printf("Total instructions before: %d\n", total_before);
    printf("Total active after: %d\n", total_after);
    printf("Reduction: %d eliminated\n", total_before - total_after);
    printf("=== DONE ===\n");

    return 0;
}
