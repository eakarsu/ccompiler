int printf(const char *fmt, ...);

enum Opcode {
    OP_NOP,
    OP_LOAD,
    OP_STORE,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOV,
    OP_CMP,
    OP_JMP,
    OP_JEQ,
    OP_JNE,
    OP_JLT,
    OP_HALT
};

struct Instruction {
    int opcode;
    int arg1;
    int arg2;
    int arg3;
};

struct CPU {
    int regs[8];
    int memory[64];
    int pc;
    int zero_flag;
    int neg_flag;
    int halted;
    int cycle_count;
};

void cpu_init(struct CPU *cpu) {
    int i;
    for (i = 0; i < 8; i++) cpu->regs[i] = 0;
    for (i = 0; i < 64; i++) cpu->memory[i] = 0;
    cpu->pc = 0;
    cpu->zero_flag = 0;
    cpu->neg_flag = 0;
    cpu->halted = 0;
    cpu->cycle_count = 0;
}

void cpu_execute(struct CPU *cpu, struct Instruction *prog, int prog_len) {
    while (!cpu->halted && cpu->pc < prog_len && cpu->cycle_count < 1000) {
        struct Instruction *inst = &prog[cpu->pc];
        cpu->cycle_count = cpu->cycle_count + 1;

        switch (inst->opcode) {
            case OP_NOP:
                cpu->pc = cpu->pc + 1;
                break;
            case OP_LOAD:
                cpu->regs[inst->arg1] = cpu->memory[inst->arg2];
                cpu->pc = cpu->pc + 1;
                break;
            case OP_STORE:
                cpu->memory[inst->arg2] = cpu->regs[inst->arg1];
                cpu->pc = cpu->pc + 1;
                break;
            case OP_ADD:
                cpu->regs[inst->arg1] = cpu->regs[inst->arg2] + cpu->regs[inst->arg3];
                cpu->pc = cpu->pc + 1;
                break;
            case OP_SUB:
                cpu->regs[inst->arg1] = cpu->regs[inst->arg2] - cpu->regs[inst->arg3];
                cpu->pc = cpu->pc + 1;
                break;
            case OP_MUL:
                cpu->regs[inst->arg1] = cpu->regs[inst->arg2] * cpu->regs[inst->arg3];
                cpu->pc = cpu->pc + 1;
                break;
            case OP_DIV:
                if (cpu->regs[inst->arg3] != 0)
                    cpu->regs[inst->arg1] = cpu->regs[inst->arg2] / cpu->regs[inst->arg3];
                cpu->pc = cpu->pc + 1;
                break;
            case OP_MOV:
                cpu->regs[inst->arg1] = inst->arg2;
                cpu->pc = cpu->pc + 1;
                break;
            case OP_CMP: {
                int diff = cpu->regs[inst->arg1] - cpu->regs[inst->arg2];
                cpu->zero_flag = (diff == 0);
                cpu->neg_flag = (diff < 0);
                cpu->pc = cpu->pc + 1;
                break;
            }
            case OP_JMP:
                cpu->pc = inst->arg1;
                break;
            case OP_JEQ:
                if (cpu->zero_flag) cpu->pc = inst->arg1;
                else cpu->pc = cpu->pc + 1;
                break;
            case OP_JNE:
                if (!cpu->zero_flag) cpu->pc = inst->arg1;
                else cpu->pc = cpu->pc + 1;
                break;
            case OP_JLT:
                if (cpu->neg_flag) cpu->pc = inst->arg1;
                else cpu->pc = cpu->pc + 1;
                break;
            case OP_HALT:
                cpu->halted = 1;
                break;
        }
    }
}

void make_inst(struct Instruction *inst, int op, int a1, int a2, int a3) {
    inst->opcode = op;
    inst->arg1 = a1;
    inst->arg2 = a2;
    inst->arg3 = a3;
}

int main() {
    struct CPU cpu;
    struct Instruction prog[32];
    int i;

    /* Test 1: Simple arithmetic: R0=10, R1=20, R2=R0+R1 */
    cpu_init(&cpu);
    make_inst(&prog[0], OP_MOV, 0, 10, 0);
    make_inst(&prog[1], OP_MOV, 1, 20, 0);
    make_inst(&prog[2], OP_ADD, 2, 0, 1);
    make_inst(&prog[3], OP_HALT, 0, 0, 0);
    cpu_execute(&cpu, prog, 4);
    // EXPECT: Test 1: R0=10 R1=20 R2=30
    printf("Test 1: R0=%d R1=%d R2=%d\n", cpu.regs[0], cpu.regs[1], cpu.regs[2]);

    /* Test 2: Store and load from memory */
    cpu_init(&cpu);
    make_inst(&prog[0], OP_MOV, 0, 42, 0);
    make_inst(&prog[1], OP_STORE, 0, 5, 0);
    make_inst(&prog[2], OP_MOV, 0, 0, 0);
    make_inst(&prog[3], OP_LOAD, 1, 5, 0);
    make_inst(&prog[4], OP_HALT, 0, 0, 0);
    cpu_execute(&cpu, prog, 5);
    // EXPECT: Test 2: R0=0 R1=42 mem[5]=42
    printf("Test 2: R0=%d R1=%d mem[5]=%d\n", cpu.regs[0], cpu.regs[1], cpu.memory[5]);

    /* Test 3: Loop - sum 1 to 5 */
    /* R0=counter(1), R1=limit(6), R2=sum(0), R3=1 */
    cpu_init(&cpu);
    make_inst(&prog[0], OP_MOV, 0, 1, 0);    /* R0 = 1 */
    make_inst(&prog[1], OP_MOV, 1, 6, 0);    /* R1 = 6 */
    make_inst(&prog[2], OP_MOV, 2, 0, 0);    /* R2 = 0 */
    make_inst(&prog[3], OP_MOV, 3, 1, 0);    /* R3 = 1 */
    /* loop: */
    make_inst(&prog[4], OP_CMP, 0, 1, 0);    /* CMP R0, R1 */
    make_inst(&prog[5], OP_JEQ, 9, 0, 0);    /* if R0==R1, goto end */
    make_inst(&prog[6], OP_ADD, 2, 2, 0);    /* R2 = R2 + R0 */
    make_inst(&prog[7], OP_ADD, 0, 0, 3);    /* R0 = R0 + 1 */
    make_inst(&prog[8], OP_JMP, 4, 0, 0);    /* goto loop */
    make_inst(&prog[9], OP_HALT, 0, 0, 0);
    cpu_execute(&cpu, prog, 10);
    // EXPECT: Test 3: Sum 1..5 = 15
    printf("Test 3: Sum 1..5 = %d\n", cpu.regs[2]);
    // EXPECT: Cycles: 32
    printf("Cycles: %d\n", cpu.cycle_count);

    /* Test 4: Multiply via repeated addition: 7*6 */
    cpu_init(&cpu);
    make_inst(&prog[0], OP_MOV, 0, 7, 0);    /* R0 = 7 (multiplicand) */
    make_inst(&prog[1], OP_MOV, 1, 6, 0);    /* R1 = 6 (count) */
    make_inst(&prog[2], OP_MOV, 2, 0, 0);    /* R2 = result */
    make_inst(&prog[3], OP_MOV, 3, 1, 0);    /* R3 = 1 */
    make_inst(&prog[4], OP_MOV, 4, 0, 0);    /* R4 = 0 */
    /* loop: */
    make_inst(&prog[5], OP_CMP, 1, 4, 0);    /* CMP R1, R4 */
    make_inst(&prog[6], OP_JEQ, 10, 0, 0);   /* if R1==0, goto end */
    make_inst(&prog[7], OP_ADD, 2, 2, 0);    /* R2 += R0 */
    make_inst(&prog[8], OP_SUB, 1, 1, 3);    /* R1 -= 1 */
    make_inst(&prog[9], OP_JMP, 5, 0, 0);    /* goto loop */
    make_inst(&prog[10], OP_HALT, 0, 0, 0);
    cpu_execute(&cpu, prog, 11);
    // EXPECT: Test 4: 7*6 = 42
    printf("Test 4: 7*6 = %d\n", cpu.regs[2]);

    // EXPECT: All assembler sim tests done
    printf("All assembler sim tests done\n");
    return 0;
}
