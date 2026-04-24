int printf(const char *fmt, ...);
// EXPECT: === CPU Pipeline Simulation ===\nProgram: 6 instructions\n  0: ADD r1, r2, r3\n  1: SUB r4, r5, r6\n  2: LOAD r7, r0, r0\n  3: ADD r2, r1, r4\n  4: STORE r0, r3, r1\n  5: NOP r0, r0, r0\nInitial regs: r0=0 r1=10 r2=20 r3=30 r4=40 r5=50 r6=60 r7=70\nCycle  1: completed=0 stalls=0 IF=ADD\nCycle  2: completed=0 stalls=0 IF=SUB\nCycle  3: completed=0 stalls=0 IF=LOAD\nCycle  4: completed=0 stalls=0 IF=ADD\nCycle  5: completed=0 stalls=1 IF=ADD\nCycle  6: completed=1 stalls=1 IF=STORE\nCycle  7: completed=2 stalls=1 IF=NOP\nCycle  8: completed=3 stalls=1\nCycle  9: completed=3 stalls=1\nCycle 10: completed=4 stalls=1\nCycle 11: completed=5 stalls=1\nCycle 12: completed=6 stalls=1\n--- Results ---\nCycles: 12\nInstructions completed: 6\nStalls: 1\nFinal regs: r0=0 r1=50 r2=40 r3=30 r4=-10 r5=50 r6=60 r7=15

enum Stage { FETCH, DECODE, EXECUTE, MEMORY, WRITEBACK, STAGE_EMPTY };
enum OpCode { OP_ADD, OP_SUB, OP_LOAD, OP_STORE, OP_NOP };

struct Instruction {
    enum OpCode op;
    int rd;
    int rs1;
    int rs2;
    int imm;
};

struct PipelineReg {
    struct Instruction inst;
    int valid;
    int result;
    int stall;
};

struct CPU {
    struct PipelineReg stages[5];
    int regs[8];
    int mem[16];
    int cycle;
    int instructions_completed;
    int stalls;
    int pc;
};

const char *op_name(enum OpCode op) {
    if (op == OP_ADD) return "ADD";
    if (op == OP_SUB) return "SUB";
    if (op == OP_LOAD) return "LOAD";
    if (op == OP_STORE) return "STORE";
    return "NOP";
}

void init_cpu(struct CPU *cpu) {
    int i;
    cpu->cycle = 0;
    cpu->instructions_completed = 0;
    cpu->stalls = 0;
    cpu->pc = 0;
    for (i = 0; i < 8; i++) cpu->regs[i] = i * 10;
    for (i = 0; i < 16; i++) cpu->mem[i] = i * 5;
    for (i = 0; i < 5; i++) {
        cpu->stages[i].valid = 0;
        cpu->stages[i].stall = 0;
        cpu->stages[i].result = 0;
    }
}

int check_hazard(struct CPU *cpu, int rs) {
    if (rs == 0) return 0;
    int i;
    for (i = 1; i < 4; i++) {
        if (cpu->stages[i].valid && cpu->stages[i].inst.rd == rs) {
            return 1;
        }
    }
    return 0;
}

void pipeline_cycle(struct CPU *cpu, struct Instruction *program, int prog_size) {
    cpu->cycle++;

    if (cpu->stages[4].valid) {
        struct Instruction *inst = &cpu->stages[4].inst;
        if (inst->op == OP_ADD || inst->op == OP_SUB || inst->op == OP_LOAD) {
            cpu->regs[inst->rd] = cpu->stages[4].result;
        }
        cpu->instructions_completed++;
    }

    cpu->stages[4] = cpu->stages[3];

    if (cpu->stages[2].valid) {
        struct Instruction *inst = &cpu->stages[2].inst;
        if (inst->op == OP_LOAD) {
            int addr = cpu->stages[2].result;
            if (addr >= 0 && addr < 16)
                cpu->stages[2].result = cpu->mem[addr];
        } else if (inst->op == OP_STORE) {
            int addr = cpu->regs[inst->rs1];
            if (addr >= 0 && addr < 16)
                cpu->mem[addr] = cpu->regs[inst->rs2];
        }
    }
    cpu->stages[3] = cpu->stages[2];

    if (cpu->stages[1].valid) {
        struct Instruction *inst = &cpu->stages[1].inst;
        if (inst->op == OP_ADD) {
            cpu->stages[1].result = cpu->regs[inst->rs1] + cpu->regs[inst->rs2];
        } else if (inst->op == OP_SUB) {
            cpu->stages[1].result = cpu->regs[inst->rs1] - cpu->regs[inst->rs2];
        } else if (inst->op == OP_LOAD) {
            cpu->stages[1].result = cpu->regs[inst->rs1] + inst->imm;
        }
    }
    cpu->stages[2] = cpu->stages[1];

    int stalled = 0;
    if (cpu->stages[0].valid) {
        struct Instruction *inst = &cpu->stages[0].inst;
        if (check_hazard(cpu, inst->rs1) || check_hazard(cpu, inst->rs2)) {
            stalled = 1;
            cpu->stalls++;
        }
    }

    if (!stalled) {
        cpu->stages[1] = cpu->stages[0];
        if (cpu->pc < prog_size) {
            cpu->stages[0].inst = program[cpu->pc];
            cpu->stages[0].valid = 1;
            cpu->stages[0].result = 0;
            cpu->stages[0].stall = 0;
            cpu->pc++;
        } else {
            cpu->stages[0].valid = 0;
        }
    } else {
        cpu->stages[1].valid = 0;
    }
}

int main(void) {
    struct CPU cpu;
    init_cpu(&cpu);

    struct Instruction program[6];
    program[0].op = OP_ADD; program[0].rd = 1; program[0].rs1 = 2; program[0].rs2 = 3; program[0].imm = 0;
    program[1].op = OP_SUB; program[1].rd = 4; program[1].rs1 = 5; program[1].rs2 = 6; program[1].imm = 0;
    program[2].op = OP_LOAD; program[2].rd = 7; program[2].rs1 = 0; program[2].rs2 = 0; program[2].imm = 3;
    program[3].op = OP_ADD; program[3].rd = 2; program[3].rs1 = 1; program[3].rs2 = 4; program[3].imm = 0;
    program[4].op = OP_STORE; program[4].rd = 0; program[4].rs1 = 3; program[4].rs2 = 1; program[4].imm = 0;
    program[5].op = OP_NOP; program[5].rd = 0; program[5].rs1 = 0; program[5].rs2 = 0; program[5].imm = 0;

    printf("=== CPU Pipeline Simulation ===\n");
    printf("Program: %d instructions\n", 6);

    int i;
    for (i = 0; i < 6; i++) {
        printf("  %d: %s r%d, r%d, r%d\n", i, op_name(program[i].op),
               program[i].rd, program[i].rs1, program[i].rs2);
    }

    printf("Initial regs:");
    for (i = 0; i < 8; i++) printf(" r%d=%d", i, cpu.regs[i]);
    printf("\n");

    int cycle;
    for (cycle = 0; cycle < 12; cycle++) {
        pipeline_cycle(&cpu, program, 6);
        printf("Cycle %2d: completed=%d stalls=%d",
               cpu.cycle, cpu.instructions_completed, cpu.stalls);
        if (cpu.stages[0].valid)
            printf(" IF=%s", op_name(cpu.stages[0].inst.op));
        printf("\n");
    }

    printf("--- Results ---\n");
    printf("Cycles: %d\n", cpu.cycle);
    printf("Instructions completed: %d\n", cpu.instructions_completed);
    printf("Stalls: %d\n", cpu.stalls);
    printf("Final regs:");
    for (i = 0; i < 8; i++) printf(" r%d=%d", i, cpu.regs[i]);
    printf("\n");

    return 0;
}
