int printf(const char *fmt, ...);
// EXPECT: === Sum 1 to 10 ===\nR0 = 55\n=== Multiply 6 * 7 ===\nR0 = 42\n=== Power 2^8 ===\nR0 = 256
// Simple assembler: parse and execute instructions

enum Inst {
    I_MOV = 0,
    I_ADD = 1,
    I_SUB = 2,
    I_MUL = 3,
    I_CMP = 4,
    I_JMP = 5,
    I_JEQ = 6,
    I_JNE = 7,
    I_JLT = 8,
    I_PRT = 9,
    I_HLT = 10,
    I_INC = 11,
    I_DEC = 12
};

struct Instruction {
    int opcode;
    int arg1;
    int arg2;
};

struct CPU {
    int regs[8];
    int flags;
    int pc;
    int running;
    struct Instruction program[32];
    int prog_len;
};

void cpu_init(struct CPU *cpu) {
    int i;
    for (i = 0; i < 8; i++) cpu->regs[i] = 0;
    cpu->flags = 0;
    cpu->pc = 0;
    cpu->running = 1;
    cpu->prog_len = 0;
}

void cpu_emit(struct CPU *cpu, int op, int a1, int a2) {
    int idx = cpu->prog_len;
    cpu->program[idx].opcode = op;
    cpu->program[idx].arg1 = a1;
    cpu->program[idx].arg2 = a2;
    cpu->prog_len++;
}

void cpu_run(struct CPU *cpu) {
    struct Instruction *inst;
    int limit = 1000;
    while (cpu->running && cpu->pc < cpu->prog_len && limit > 0) {
        inst = &cpu->program[cpu->pc];
        cpu->pc++;
        limit--;
        if (inst->opcode == I_MOV) {
            if (inst->arg2 >= 100) {
                cpu->regs[inst->arg1] = inst->arg2 - 100;
            } else {
                cpu->regs[inst->arg1] = cpu->regs[inst->arg2];
            }
        } else if (inst->opcode == I_ADD) {
            cpu->regs[inst->arg1] += cpu->regs[inst->arg2];
        } else if (inst->opcode == I_SUB) {
            cpu->regs[inst->arg1] -= cpu->regs[inst->arg2];
        } else if (inst->opcode == I_MUL) {
            cpu->regs[inst->arg1] *= cpu->regs[inst->arg2];
        } else if (inst->opcode == I_CMP) {
            cpu->flags = cpu->regs[inst->arg1] - cpu->regs[inst->arg2];
        } else if (inst->opcode == I_JMP) {
            cpu->pc = inst->arg1;
        } else if (inst->opcode == I_JEQ) {
            if (cpu->flags == 0) cpu->pc = inst->arg1;
        } else if (inst->opcode == I_JNE) {
            if (cpu->flags != 0) cpu->pc = inst->arg1;
        } else if (inst->opcode == I_JLT) {
            if (cpu->flags < 0) cpu->pc = inst->arg1;
        } else if (inst->opcode == I_PRT) {
            printf("R%d = %d\n", inst->arg1, cpu->regs[inst->arg1]);
        } else if (inst->opcode == I_INC) {
            cpu->regs[inst->arg1]++;
        } else if (inst->opcode == I_DEC) {
            cpu->regs[inst->arg1]--;
        } else if (inst->opcode == I_HLT) {
            cpu->running = 0;
        }
    }
}

int main(void) {
    struct CPU cpu;

    /* Sum 1 to 10: R0=sum, R1=counter(1..10), R2=limit(11) */
    printf("=== Sum 1 to 10 ===\n");
    cpu_init(&cpu);
    cpu_emit(&cpu, I_MOV, 0, 100);     /* 0: R0 = 0 (sum) */
    cpu_emit(&cpu, I_MOV, 1, 101);     /* 1: R1 = 1 (counter) */
    cpu_emit(&cpu, I_MOV, 2, 111);     /* 2: R2 = 11 (limit) */
    /* loop at 3: */
    cpu_emit(&cpu, I_CMP, 1, 2);       /* 3: cmp R1, R2 */
    cpu_emit(&cpu, I_JEQ, 8, 0);       /* 4: if R1==11 goto 8 (done) */
    cpu_emit(&cpu, I_ADD, 0, 1);       /* 5: R0 += R1 */
    cpu_emit(&cpu, I_INC, 1, 0);       /* 6: R1++ */
    cpu_emit(&cpu, I_JMP, 3, 0);       /* 7: goto 3 */
    cpu_emit(&cpu, I_PRT, 0, 0);       /* 8: print sum */
    cpu_emit(&cpu, I_HLT, 0, 0);       /* 9: halt */
    cpu_run(&cpu);

    /* Multiply 6 * 7 */
    printf("=== Multiply 6 * 7 ===\n");
    cpu_init(&cpu);
    cpu_emit(&cpu, I_MOV, 0, 100);     /* 0: R0 = 0 (result) */
    cpu_emit(&cpu, I_MOV, 1, 107);     /* 1: R1 = 7 (mul) */
    cpu_emit(&cpu, I_MOV, 2, 106);     /* 2: R2 = 6 (counter) */
    cpu_emit(&cpu, I_MOV, 3, 100);     /* 3: R3 = 0 */
    /* loop at 4: */
    cpu_emit(&cpu, I_CMP, 2, 3);       /* 4: cmp R2, R3 */
    cpu_emit(&cpu, I_JEQ, 9, 0);       /* 5: if == goto 9 */
    cpu_emit(&cpu, I_ADD, 0, 1);       /* 6: R0 += R1 */
    cpu_emit(&cpu, I_DEC, 2, 0);       /* 7: R2-- */
    cpu_emit(&cpu, I_JMP, 4, 0);       /* 8: goto 4 */
    cpu_emit(&cpu, I_PRT, 0, 0);       /* 9: print result */
    cpu_emit(&cpu, I_HLT, 0, 0);       /* 10: halt */
    cpu_run(&cpu);

    /* Power 2^8 */
    printf("=== Power 2^8 ===\n");
    cpu_init(&cpu);
    cpu_emit(&cpu, I_MOV, 0, 101);     /* 0: R0 = 1 (result) */
    cpu_emit(&cpu, I_MOV, 1, 102);     /* 1: R1 = 2 (base) */
    cpu_emit(&cpu, I_MOV, 2, 108);     /* 2: R2 = 8 (exp) */
    cpu_emit(&cpu, I_MOV, 3, 100);     /* 3: R3 = 0 */
    /* loop at 4: */
    cpu_emit(&cpu, I_CMP, 2, 3);       /* 4: cmp R2, 0 */
    cpu_emit(&cpu, I_JEQ, 9, 0);       /* 5: if == goto 9 */
    cpu_emit(&cpu, I_MUL, 0, 1);       /* 6: R0 *= R1 */
    cpu_emit(&cpu, I_DEC, 2, 0);       /* 7: R2-- */
    cpu_emit(&cpu, I_JMP, 4, 0);       /* 8: goto 4 */
    cpu_emit(&cpu, I_PRT, 0, 0);       /* 9: print */
    cpu_emit(&cpu, I_HLT, 0, 0);       /* 10: halt */
    cpu_run(&cpu);

    return 0;
}
