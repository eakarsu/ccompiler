int printf(const char *fmt, ...);

enum RegOp {
    REG_LOAD_IMM,
    REG_ADD,
    REG_SUB,
    REG_MUL,
    REG_MOD,
    REG_COPY,
    REG_PRINT,
    REG_BRANCH_ZERO,
    REG_BRANCH_POS,
    REG_JUMP,
    REG_HALT
};

struct RegInst {
    int op;
    int dst;
    int src1;
    int src2;
};

struct RegMachine {
    int regs[16];
    int output[32];
    int out_count;
    int pc;
    int halted;
    int steps;
};

void rm_init(struct RegMachine *m) {
    int i;
    for (i = 0; i < 16; i++) m->regs[i] = 0;
    for (i = 0; i < 32; i++) m->output[i] = 0;
    m->out_count = 0;
    m->pc = 0;
    m->halted = 0;
    m->steps = 0;
}

void rm_run(struct RegMachine *m, struct RegInst *prog, int plen) {
    while (!m->halted && m->pc < plen && m->steps < 5000) {
        struct RegInst *inst = &prog[m->pc];
        m->steps = m->steps + 1;

        switch (inst->op) {
            case REG_LOAD_IMM:
                m->regs[inst->dst] = inst->src1;
                m->pc = m->pc + 1;
                break;
            case REG_ADD:
                m->regs[inst->dst] = m->regs[inst->src1] + m->regs[inst->src2];
                m->pc = m->pc + 1;
                break;
            case REG_SUB:
                m->regs[inst->dst] = m->regs[inst->src1] - m->regs[inst->src2];
                m->pc = m->pc + 1;
                break;
            case REG_MUL:
                m->regs[inst->dst] = m->regs[inst->src1] * m->regs[inst->src2];
                m->pc = m->pc + 1;
                break;
            case REG_MOD:
                if (m->regs[inst->src2] != 0)
                    m->regs[inst->dst] = m->regs[inst->src1] % m->regs[inst->src2];
                m->pc = m->pc + 1;
                break;
            case REG_COPY:
                m->regs[inst->dst] = m->regs[inst->src1];
                m->pc = m->pc + 1;
                break;
            case REG_PRINT:
                if (m->out_count < 32) {
                    m->output[m->out_count] = m->regs[inst->dst];
                    m->out_count = m->out_count + 1;
                }
                m->pc = m->pc + 1;
                break;
            case REG_BRANCH_ZERO:
                if (m->regs[inst->dst] == 0)
                    m->pc = inst->src1;
                else
                    m->pc = m->pc + 1;
                break;
            case REG_BRANCH_POS:
                if (m->regs[inst->dst] > 0)
                    m->pc = inst->src1;
                else
                    m->pc = m->pc + 1;
                break;
            case REG_JUMP:
                m->pc = inst->dst;
                break;
            case REG_HALT:
                m->halted = 1;
                break;
        }
    }
}

void set_inst(struct RegInst *inst, int op, int d, int s1, int s2) {
    inst->op = op;
    inst->dst = d;
    inst->src1 = s1;
    inst->src2 = s2;
}

int main() {
    struct RegMachine m;
    struct RegInst prog[32];
    int i;

    /* Test 1: Fibonacci first 8 numbers */
    rm_init(&m);
    /* R0=a(0), R1=b(1), R2=temp, R3=count(8), R4=1 */
    set_inst(&prog[0], REG_LOAD_IMM, 0, 0, 0);     /* R0 = 0 */
    set_inst(&prog[1], REG_LOAD_IMM, 1, 1, 0);     /* R1 = 1 */
    set_inst(&prog[2], REG_LOAD_IMM, 3, 8, 0);     /* R3 = 8 (count) */
    set_inst(&prog[3], REG_LOAD_IMM, 4, 1, 0);     /* R4 = 1 */
    /* loop: */
    set_inst(&prog[4], REG_BRANCH_ZERO, 3, 10, 0);  /* if R3==0 goto end */
    set_inst(&prog[5], REG_PRINT, 0, 0, 0);         /* print R0 */
    set_inst(&prog[6], REG_ADD, 2, 0, 1);           /* R2 = R0 + R1 */
    set_inst(&prog[7], REG_COPY, 0, 1, 0);          /* R0 = R1 */
    set_inst(&prog[8], REG_COPY, 1, 2, 0);          /* R1 = R2 */
    set_inst(&prog[9], REG_SUB, 3, 3, 4);           /* R3 -= 1 */
    set_inst(&prog[10], REG_JUMP, 4, 0, 0);         /* goto loop */
    set_inst(&prog[11], REG_HALT, 0, 0, 0);
    rm_run(&m, prog, 12);

    // EXPECT: Fibonacci:
    printf("Fibonacci:\n");
    for (i = 0; i < m.out_count; i++) {
        // EXPECT: fib[0]=0
        // EXPECT: fib[1]=1
        // EXPECT: fib[2]=1
        // EXPECT: fib[3]=2
        // EXPECT: fib[4]=3
        // EXPECT: fib[5]=5
        // EXPECT: fib[6]=8
        // EXPECT: fib[7]=13
        printf("fib[%d]=%d\n", i, m.output[i]);
    }

    /* Test 2: Factorial of 6 */
    rm_init(&m);
    /* R0=n(6), R1=result(1), R2=1 */
    set_inst(&prog[0], REG_LOAD_IMM, 0, 6, 0);      /* R0 = 6 */
    set_inst(&prog[1], REG_LOAD_IMM, 1, 1, 0);      /* R1 = 1 */
    set_inst(&prog[2], REG_LOAD_IMM, 2, 1, 0);      /* R2 = 1 */
    /* loop: */
    set_inst(&prog[3], REG_BRANCH_ZERO, 0, 8, 0);   /* if R0==0 goto end */
    set_inst(&prog[4], REG_MUL, 1, 1, 0);           /* R1 = R1 * R0 */
    set_inst(&prog[5], REG_SUB, 0, 0, 2);           /* R0 -= 1 */
    set_inst(&prog[6], REG_JUMP, 3, 0, 0);          /* goto loop */
    set_inst(&prog[7], REG_HALT, 0, 0, 0);
    set_inst(&prog[8], REG_PRINT, 1, 0, 0);         /* print result */
    set_inst(&prog[9], REG_HALT, 0, 0, 0);
    rm_run(&m, prog, 10);
    // EXPECT: Factorial 6 = 720
    printf("Factorial 6 = %d\n", m.output[0]);

    /* Test 3: GCD of 48 and 18 using Euclidean algorithm */
    rm_init(&m);
    /* R0=a(48), R1=b(18), R2=temp (a mod b), R3=zero check */
    set_inst(&prog[0], REG_LOAD_IMM, 0, 48, 0);
    set_inst(&prog[1], REG_LOAD_IMM, 1, 18, 0);
    /* loop: */
    set_inst(&prog[2], REG_BRANCH_ZERO, 1, 7, 0);   /* if R1==0 goto done */
    set_inst(&prog[3], REG_MOD, 2, 0, 1);           /* R2 = R0 % R1 */
    set_inst(&prog[4], REG_COPY, 0, 1, 0);          /* R0 = R1 */
    set_inst(&prog[5], REG_COPY, 1, 2, 0);          /* R1 = R2 */
    set_inst(&prog[6], REG_JUMP, 2, 0, 0);          /* goto loop */
    set_inst(&prog[7], REG_PRINT, 0, 0, 0);         /* print GCD */
    set_inst(&prog[8], REG_HALT, 0, 0, 0);
    rm_run(&m, prog, 9);
    // EXPECT: GCD(48,18) = 6
    printf("GCD(48,18) = %d\n", m.output[0]);
    // EXPECT: Steps: 20
    printf("Steps: %d\n", m.steps);

    // EXPECT: All register machine tests done
    printf("All register machine tests done\n");
    return 0;
}
