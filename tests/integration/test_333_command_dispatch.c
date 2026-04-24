int printf(const char *fmt, ...);
// EXPECT: R0 = 10\nR1 = 5\nR0 = 15\nR0 = 45\nR0 = 40\nR0=40 R1=5 R2=3 R3=0 PC=12
// Test: enum-based command dispatch

enum Command {
    CMD_ADD,
    CMD_SUB,
    CMD_MUL,
    CMD_DIV,
    CMD_LOAD,
    CMD_STORE,
    CMD_PRINT,
    CMD_HALT
};

struct Instruction {
    enum Command cmd;
    int arg1;
    int arg2;
};

struct CPU {
    int registers[4];
    int running;
    int pc;
};

void cpu_init(struct CPU *cpu) {
    int i;
    for (i = 0; i < 4; i++) {
        cpu->registers[i] = 0;
    }
    cpu->running = 1;
    cpu->pc = 0;
}

void cpu_execute(struct CPU *cpu, struct Instruction *inst) {
    switch (inst->cmd) {
        case CMD_ADD:
            cpu->registers[inst->arg1] = cpu->registers[inst->arg1] + cpu->registers[inst->arg2];
            break;
        case CMD_SUB:
            cpu->registers[inst->arg1] = cpu->registers[inst->arg1] - cpu->registers[inst->arg2];
            break;
        case CMD_MUL:
            cpu->registers[inst->arg1] = cpu->registers[inst->arg1] * cpu->registers[inst->arg2];
            break;
        case CMD_DIV:
            if (cpu->registers[inst->arg2] != 0)
                cpu->registers[inst->arg1] = cpu->registers[inst->arg1] / cpu->registers[inst->arg2];
            break;
        case CMD_LOAD:
            cpu->registers[inst->arg1] = inst->arg2;
            break;
        case CMD_STORE:
            cpu->registers[inst->arg2] = cpu->registers[inst->arg1];
            break;
        case CMD_PRINT:
            printf("R%d = %d\n", inst->arg1, cpu->registers[inst->arg1]);
            break;
        case CMD_HALT:
            cpu->running = 0;
            break;
    }
    cpu->pc = cpu->pc + 1;
}

void cpu_run(struct CPU *cpu, struct Instruction program[], int len) {
    while (cpu->running && cpu->pc < len) {
        cpu_execute(cpu, &program[cpu->pc]);
    }
}

void cpu_dump(struct CPU *cpu) {
    int i;
    for (i = 0; i < 4; i++) {
        printf("R%d=%d ", i, cpu->registers[i]);
    }
    printf("PC=%d\n", cpu->pc);
}

int main(void) {
    struct CPU cpu;
    cpu_init(&cpu);

    struct Instruction prog[12];
    prog[0].cmd = CMD_LOAD;  prog[0].arg1 = 0; prog[0].arg2 = 10;
    prog[1].cmd = CMD_LOAD;  prog[1].arg1 = 1; prog[1].arg2 = 5;
    prog[2].cmd = CMD_PRINT; prog[2].arg1 = 0; prog[2].arg2 = 0;
    prog[3].cmd = CMD_PRINT; prog[3].arg1 = 1; prog[3].arg2 = 0;
    prog[4].cmd = CMD_ADD;   prog[4].arg1 = 0; prog[4].arg2 = 1;
    prog[5].cmd = CMD_PRINT; prog[5].arg1 = 0; prog[5].arg2 = 0;
    prog[6].cmd = CMD_LOAD;  prog[6].arg1 = 2; prog[6].arg2 = 3;
    prog[7].cmd = CMD_MUL;   prog[7].arg1 = 0; prog[7].arg2 = 2;
    prog[8].cmd = CMD_PRINT; prog[8].arg1 = 0; prog[8].arg2 = 0;
    prog[9].cmd = CMD_SUB;   prog[9].arg1 = 0; prog[9].arg2 = 1;
    prog[10].cmd = CMD_PRINT; prog[10].arg1 = 0; prog[10].arg2 = 0;
    prog[11].cmd = CMD_HALT; prog[11].arg1 = 0; prog[11].arg2 = 0;

    cpu_run(&cpu, prog, 12);
    cpu_dump(&cpu);

    return 0;
}
