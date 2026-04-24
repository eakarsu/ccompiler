int printf(const char *fmt, ...);

// Simple debugger with breakpoints
// Simulates setting breakpoints, single-stepping, inspecting state

struct Breakpoint {
    int addr;
    int enabled;
    int hit_count;
};

struct DebugVM {
    int regs[4];
    int code[32];
    int pc;
    int halted;
    struct Breakpoint bps[8];
    int num_bps;
    int stepping;
    int step_count;
};

void dbg_init(struct DebugVM *d) {
    int i;
    for (i = 0; i < 4; i++) d->regs[i] = 0;
    for (i = 0; i < 32; i++) d->code[i] = 0;
    d->pc = 0;
    d->halted = 0;
    d->num_bps = 0;
    d->stepping = 0;
    d->step_count = 0;
    for (i = 0; i < 8; i++) {
        d->bps[i].addr = -1;
        d->bps[i].enabled = 0;
        d->bps[i].hit_count = 0;
    }
}

int dbg_add_bp(struct DebugVM *d, int addr) {
    int idx = d->num_bps;
    d->bps[idx].addr = addr;
    d->bps[idx].enabled = 1;
    d->bps[idx].hit_count = 0;
    d->num_bps = d->num_bps + 1;
    return idx;
}

void dbg_disable_bp(struct DebugVM *d, int idx) {
    d->bps[idx].enabled = 0;
}

void dbg_enable_bp(struct DebugVM *d, int idx) {
    d->bps[idx].enabled = 1;
}

int dbg_check_bp(struct DebugVM *d) {
    int i;
    for (i = 0; i < d->num_bps; i++) {
        if (d->bps[i].enabled && d->bps[i].addr == d->pc) {
            d->bps[i].hit_count = d->bps[i].hit_count + 1;
            return i;
        }
    }
    return -1;
}

// Simple instruction set for debugging:
// 0=NOP, 1=LOAD_IMM(reg,val), 2=ADD(dst,src), 3=PRINT(reg), 4=JMP(addr), 5=HALT
// Each instruction is 3 words: [op, arg1, arg2]
void dbg_step(struct DebugVM *d) {
    int op = d->code[d->pc];
    int a1 = d->code[d->pc + 1];
    int a2 = d->code[d->pc + 2];

    if (op == 0) {
        // NOP
    } else if (op == 1) {
        d->regs[a1] = a2;
    } else if (op == 2) {
        d->regs[a1] = d->regs[a1] + d->regs[a2];
    } else if (op == 3) {
        printf("dbg print r%d=%d\n", a1, d->regs[a1]);
    } else if (op == 4) {
        d->pc = a1;
        d->step_count = d->step_count + 1;
        return;
    } else if (op == 5) {
        d->halted = 1;
        d->step_count = d->step_count + 1;
        return;
    }

    d->pc = d->pc + 3;
    d->step_count = d->step_count + 1;
}

void dbg_run_until_bp_or_halt(struct DebugVM *d) {
    while (!d->halted) {
        int bp = dbg_check_bp(d);
        if (bp >= 0) {
            printf("hit bp %d at pc=%d\n", bp, d->pc);
            return;
        }
        dbg_step(d);
    }
}

void dbg_print_state(struct DebugVM *d) {
    printf("pc=%d r0=%d r1=%d r2=%d r3=%d\n",
           d->pc, d->regs[0], d->regs[1], d->regs[2], d->regs[3]);
}

int main() {
    struct DebugVM dbg;
    dbg_init(&dbg);

    // Program: r0=10, r1=5, r2=r0+r1, print r2, halt
    int i = 0;
    dbg.code[i] = 1; dbg.code[i+1] = 0; dbg.code[i+2] = 10;  i = i + 3;  // LOAD r0, 10
    dbg.code[i] = 1; dbg.code[i+1] = 1; dbg.code[i+2] = 5;   i = i + 3;  // LOAD r1, 5
    dbg.code[i] = 2; dbg.code[i+1] = 0; dbg.code[i+2] = 1;   i = i + 3;  // ADD r0, r1
    dbg.code[i] = 3; dbg.code[i+1] = 0; dbg.code[i+2] = 0;   i = i + 3;  // PRINT r0
    dbg.code[i] = 5; dbg.code[i+1] = 0; dbg.code[i+2] = 0;               // HALT

    // Set breakpoint at instruction 2 (ADD), which is at pc=6
    int bp0 = dbg_add_bp(&dbg, 6);
    printf("bp %d set at addr %d\n", bp0, 6);
    // EXPECT: bp 0 set at addr 6

    // Run until breakpoint
    dbg_run_until_bp_or_halt(&dbg);
    // EXPECT: hit bp 0 at pc=6

    dbg_print_state(&dbg);
    // EXPECT: pc=6 r0=10 r1=5 r2=0 r3=0

    // Single step past the breakpoint
    dbg_disable_bp(&dbg, bp0);
    dbg_step(&dbg);
    dbg_print_state(&dbg);
    // EXPECT: pc=9 r0=15 r1=5 r2=0 r3=0

    // Continue to halt
    dbg_run_until_bp_or_halt(&dbg);
    // EXPECT: dbg print r0=15

    printf("halted: %d steps: %d\n", dbg.halted, dbg.step_count);
    // EXPECT: halted: 1 steps: 5

    // Test 2: multiple breakpoints and stepping
    dbg_init(&dbg);
    i = 0;
    dbg.code[i] = 1; dbg.code[i+1] = 0; dbg.code[i+2] = 10;  i = i + 3;  // pc=0: r0=10
    dbg.code[i] = 1; dbg.code[i+1] = 1; dbg.code[i+2] = 20;  i = i + 3;  // pc=3: r1=20
    dbg.code[i] = 1; dbg.code[i+1] = 2; dbg.code[i+2] = 30;  i = i + 3;  // pc=6: r2=30
    dbg.code[i] = 2; dbg.code[i+1] = 0; dbg.code[i+2] = 1;   i = i + 3;  // pc=9: r0+=r1
    dbg.code[i] = 2; dbg.code[i+1] = 0; dbg.code[i+2] = 2;   i = i + 3;  // pc=12: r0+=r2
    dbg.code[i] = 3; dbg.code[i+1] = 0; dbg.code[i+2] = 0;   i = i + 3;  // pc=15: print r0
    dbg.code[i] = 5; dbg.code[i+1] = 0; dbg.code[i+2] = 0;               // pc=18: HALT

    // Set breakpoints at first and second ADD
    int bpA = dbg_add_bp(&dbg, 9);
    int bpB = dbg_add_bp(&dbg, 12);

    // Run to first bp
    dbg_run_until_bp_or_halt(&dbg);
    printf("bp%d at pc=%d r0=%d\n", bpA, dbg.pc, dbg.regs[0]);
    // EXPECT: hit bp 0 at pc=9
    // EXPECT: bp0 at pc=9 r0=10

    // Step past first bp
    dbg_disable_bp(&dbg, bpA);
    dbg_step(&dbg);
    dbg_enable_bp(&dbg, bpA);

    // Run to second bp
    dbg_run_until_bp_or_halt(&dbg);
    printf("bp%d at pc=%d r0=%d\n", bpB, dbg.pc, dbg.regs[0]);
    // EXPECT: hit bp 1 at pc=12
    // EXPECT: bp1 at pc=12 r0=30

    // Step past second bp and continue to halt
    dbg_disable_bp(&dbg, bpB);
    dbg_run_until_bp_or_halt(&dbg);
    // EXPECT: dbg print r0=60

    printf("bp0 hits: %d bp1 hits: %d\n", dbg.bps[bpA].hit_count, dbg.bps[bpB].hit_count);
    // EXPECT: bp0 hits: 1 bp1 hits: 1

    printf("debugger done\n");
    // EXPECT: debugger done

    return 0;
}
