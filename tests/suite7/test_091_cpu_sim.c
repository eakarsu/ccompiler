int printf(const char *fmt, ...);

// Simple CPU simulator with registers, ALU, flags, memory, and program counter

enum {
    OP_NOP = 0,
    OP_LOAD = 1,
    OP_STORE = 2,
    OP_ADD = 3,
    OP_SUB = 4,
    OP_MUL = 5,
    OP_AND = 6,
    OP_OR = 7,
    OP_XOR = 8,
    OP_CMP = 9,
    OP_JMP = 10,
    OP_JZ = 11,
    OP_HALT = 12
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

struct Instruction {
    int opcode;
    int dst;
    int src1;
    int src2;
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

void update_flags(struct CPU *cpu, int result) {
    cpu->zero_flag = (result == 0) ? 1 : 0;
    cpu->neg_flag = (result < 0) ? 1 : 0;
}

void cpu_execute(struct CPU *cpu, struct Instruction *prog, int prog_len) {
    while (cpu->halted == 0 && cpu->pc < prog_len && cpu->cycle_count < 1000) {
        struct Instruction inst = prog[cpu->pc];
        int result;
        cpu->cycle_count = cpu->cycle_count + 1;

        if (inst.opcode == OP_NOP) {
            cpu->pc = cpu->pc + 1;
        } else if (inst.opcode == OP_LOAD) {
            cpu->regs[inst.dst] = cpu->memory[inst.src1];
            cpu->pc = cpu->pc + 1;
        } else if (inst.opcode == OP_STORE) {
            cpu->memory[inst.dst] = cpu->regs[inst.src1];
            cpu->pc = cpu->pc + 1;
        } else if (inst.opcode == OP_ADD) {
            result = cpu->regs[inst.src1] + cpu->regs[inst.src2];
            cpu->regs[inst.dst] = result;
            update_flags(cpu, result);
            cpu->pc = cpu->pc + 1;
        } else if (inst.opcode == OP_SUB) {
            result = cpu->regs[inst.src1] - cpu->regs[inst.src2];
            cpu->regs[inst.dst] = result;
            update_flags(cpu, result);
            cpu->pc = cpu->pc + 1;
        } else if (inst.opcode == OP_MUL) {
            result = cpu->regs[inst.src1] * cpu->regs[inst.src2];
            cpu->regs[inst.dst] = result;
            update_flags(cpu, result);
            cpu->pc = cpu->pc + 1;
        } else if (inst.opcode == OP_AND) {
            result = cpu->regs[inst.src1] & cpu->regs[inst.src2];
            cpu->regs[inst.dst] = result;
            update_flags(cpu, result);
            cpu->pc = cpu->pc + 1;
        } else if (inst.opcode == OP_OR) {
            result = cpu->regs[inst.src1] | cpu->regs[inst.src2];
            cpu->regs[inst.dst] = result;
            update_flags(cpu, result);
            cpu->pc = cpu->pc + 1;
        } else if (inst.opcode == OP_XOR) {
            result = cpu->regs[inst.src1] ^ cpu->regs[inst.src2];
            cpu->regs[inst.dst] = result;
            update_flags(cpu, result);
            cpu->pc = cpu->pc + 1;
        } else if (inst.opcode == OP_CMP) {
            result = cpu->regs[inst.src1] - cpu->regs[inst.src2];
            update_flags(cpu, result);
            cpu->pc = cpu->pc + 1;
        } else if (inst.opcode == OP_JMP) {
            cpu->pc = inst.dst;
        } else if (inst.opcode == OP_JZ) {
            if (cpu->zero_flag) cpu->pc = inst.dst;
            else cpu->pc = cpu->pc + 1;
        } else if (inst.opcode == OP_HALT) {
            cpu->halted = 1;
        } else {
            cpu->pc = cpu->pc + 1;
        }
    }
}

int main(void) {
    struct CPU cpu;
    struct Instruction prog[20];
    int i;

    // Test 1: Basic register operations
    cpu_init(&cpu);
    cpu.regs[0] = 10;
    cpu.regs[1] = 20;
    // EXPECT: R0=10 R1=20
    printf("R0=%d R1=%d\n", cpu.regs[0], cpu.regs[1]);

    // Test 2: ADD instruction
    prog[0].opcode = OP_ADD; prog[0].dst = 2; prog[0].src1 = 0; prog[0].src2 = 1;
    prog[1].opcode = OP_HALT; prog[1].dst = 0; prog[1].src1 = 0; prog[1].src2 = 0;
    cpu_execute(&cpu, prog, 2);
    // EXPECT: ADD result: R2=30
    printf("ADD result: R2=%d\n", cpu.regs[2]);

    // Test 3: SUB instruction
    cpu_init(&cpu);
    cpu.regs[0] = 50;
    cpu.regs[1] = 30;
    prog[0].opcode = OP_SUB; prog[0].dst = 2; prog[0].src1 = 0; prog[0].src2 = 1;
    prog[1].opcode = OP_HALT; prog[1].dst = 0; prog[1].src1 = 0; prog[1].src2 = 0;
    cpu_execute(&cpu, prog, 2);
    // EXPECT: SUB result: R2=20
    printf("SUB result: R2=%d\n", cpu.regs[2]);

    // Test 4: MUL instruction
    cpu_init(&cpu);
    cpu.regs[0] = 6;
    cpu.regs[1] = 7;
    prog[0].opcode = OP_MUL; prog[0].dst = 2; prog[0].src1 = 0; prog[0].src2 = 1;
    prog[1].opcode = OP_HALT; prog[1].dst = 0; prog[1].src1 = 0; prog[1].src2 = 0;
    cpu_execute(&cpu, prog, 2);
    // EXPECT: MUL result: R2=42
    printf("MUL result: R2=%d\n", cpu.regs[2]);

    // Test 5: Memory store and load
    cpu_init(&cpu);
    cpu.regs[0] = 99;
    prog[0].opcode = OP_STORE; prog[0].dst = 10; prog[0].src1 = 0; prog[0].src2 = 0;
    prog[1].opcode = OP_LOAD; prog[1].dst = 1; prog[1].src1 = 10; prog[1].src2 = 0;
    prog[2].opcode = OP_HALT; prog[2].dst = 0; prog[2].src1 = 0; prog[2].src2 = 0;
    cpu_execute(&cpu, prog, 3);
    // EXPECT: Mem[10]=99 R1=99
    printf("Mem[10]=%d R1=%d\n", cpu.memory[10], cpu.regs[1]);

    // Test 6: Zero flag
    cpu_init(&cpu);
    cpu.regs[0] = 5;
    cpu.regs[1] = 5;
    prog[0].opcode = OP_SUB; prog[0].dst = 2; prog[0].src1 = 0; prog[0].src2 = 1;
    prog[1].opcode = OP_HALT; prog[1].dst = 0; prog[1].src1 = 0; prog[1].src2 = 0;
    cpu_execute(&cpu, prog, 2);
    // EXPECT: Zero flag=1
    printf("Zero flag=%d\n", cpu.zero_flag);

    // Test 7: Negative flag
    cpu_init(&cpu);
    cpu.regs[0] = 3;
    cpu.regs[1] = 10;
    prog[0].opcode = OP_SUB; prog[0].dst = 2; prog[0].src1 = 0; prog[0].src2 = 1;
    prog[1].opcode = OP_HALT; prog[1].dst = 0; prog[1].src1 = 0; prog[1].src2 = 0;
    cpu_execute(&cpu, prog, 2);
    // EXPECT: Neg flag=1 result=-7
    printf("Neg flag=%d result=%d\n", cpu.neg_flag, cpu.regs[2]);

    // Test 8: AND operation
    cpu_init(&cpu);
    cpu.regs[0] = 15;
    cpu.regs[1] = 6;
    prog[0].opcode = OP_AND; prog[0].dst = 2; prog[0].src1 = 0; prog[0].src2 = 1;
    prog[1].opcode = OP_HALT; prog[1].dst = 0; prog[1].src1 = 0; prog[1].src2 = 0;
    cpu_execute(&cpu, prog, 2);
    // EXPECT: AND 15&6=6
    printf("AND 15&6=%d\n", cpu.regs[2]);

    // Test 9: OR operation
    cpu_init(&cpu);
    cpu.regs[0] = 10;
    cpu.regs[1] = 5;
    prog[0].opcode = OP_OR; prog[0].dst = 2; prog[0].src1 = 0; prog[0].src2 = 1;
    prog[1].opcode = OP_HALT; prog[1].dst = 0; prog[1].src1 = 0; prog[1].src2 = 0;
    cpu_execute(&cpu, prog, 2);
    // EXPECT: OR 10|5=15
    printf("OR 10|5=%d\n", cpu.regs[2]);

    // Test 10: XOR operation
    cpu_init(&cpu);
    cpu.regs[0] = 12;
    cpu.regs[1] = 10;
    prog[0].opcode = OP_XOR; prog[0].dst = 2; prog[0].src1 = 0; prog[0].src2 = 1;
    prog[1].opcode = OP_HALT; prog[1].dst = 0; prog[1].src1 = 0; prog[1].src2 = 0;
    cpu_execute(&cpu, prog, 2);
    // EXPECT: XOR 12^10=6
    printf("XOR 12^10=%d\n", cpu.regs[2]);

    // Test 11: Conditional jump (JZ taken)
    cpu_init(&cpu);
    cpu.regs[0] = 5;
    cpu.regs[1] = 5;
    // SUB r0-r1=0, set zero flag, JZ to instruction 3 (HALT), skip ADD
    prog[0].opcode = OP_SUB; prog[0].dst = 2; prog[0].src1 = 0; prog[0].src2 = 1;
    prog[1].opcode = OP_JZ;  prog[1].dst = 3; prog[1].src1 = 0; prog[1].src2 = 0;
    prog[2].opcode = OP_ADD; prog[2].dst = 3; prog[2].src1 = 0; prog[2].src2 = 1;
    prog[3].opcode = OP_HALT; prog[3].dst = 0; prog[3].src1 = 0; prog[3].src2 = 0;
    cpu_execute(&cpu, prog, 4);
    // EXPECT: JZ taken, R3=0
    printf("JZ taken, R3=%d\n", cpu.regs[3]);

    // Test 12: Conditional jump (JZ not taken)
    cpu_init(&cpu);
    cpu.regs[0] = 5;
    cpu.regs[1] = 3;
    prog[0].opcode = OP_SUB; prog[0].dst = 2; prog[0].src1 = 0; prog[0].src2 = 1;
    prog[1].opcode = OP_JZ;  prog[1].dst = 3; prog[1].src1 = 0; prog[1].src2 = 0;
    prog[2].opcode = OP_ADD; prog[2].dst = 3; prog[2].src1 = 0; prog[2].src2 = 1;
    prog[3].opcode = OP_HALT; prog[3].dst = 0; prog[3].src1 = 0; prog[3].src2 = 0;
    cpu_execute(&cpu, prog, 4);
    // EXPECT: JZ not taken, R3=8
    printf("JZ not taken, R3=%d\n", cpu.regs[3]);

    // Test 13: Cycle count
    cpu_init(&cpu);
    prog[0].opcode = OP_NOP;  prog[0].dst = 0; prog[0].src1 = 0; prog[0].src2 = 0;
    prog[1].opcode = OP_NOP;  prog[1].dst = 0; prog[1].src1 = 0; prog[1].src2 = 0;
    prog[2].opcode = OP_NOP;  prog[2].dst = 0; prog[2].src1 = 0; prog[2].src2 = 0;
    prog[3].opcode = OP_HALT; prog[3].dst = 0; prog[3].src1 = 0; prog[3].src2 = 0;
    cpu_execute(&cpu, prog, 4);
    // EXPECT: Cycles=4
    printf("Cycles=%d\n", cpu.cycle_count);

    // Test 14: Unconditional jump loop (count to 3)
    cpu_init(&cpu);
    cpu.regs[0] = 0;
    cpu.regs[1] = 1;
    cpu.regs[2] = 3;
    // 0: ADD r0 += r1
    prog[0].opcode = OP_ADD; prog[0].dst = 0; prog[0].src1 = 0; prog[0].src2 = 1;
    // 1: CMP r0 vs r2
    prog[1].opcode = OP_CMP; prog[1].dst = 0; prog[1].src1 = 0; prog[1].src2 = 2;
    // 2: JZ to 4 (halt)
    prog[2].opcode = OP_JZ;  prog[2].dst = 4; prog[2].src1 = 0; prog[2].src2 = 0;
    // 3: JMP to 0
    prog[3].opcode = OP_JMP; prog[3].dst = 0; prog[3].src1 = 0; prog[3].src2 = 0;
    // 4: HALT
    prog[4].opcode = OP_HALT; prog[4].dst = 0; prog[4].src1 = 0; prog[4].src2 = 0;
    cpu_execute(&cpu, prog, 5);
    // EXPECT: Loop count R0=3
    printf("Loop count R0=%d\n", cpu.regs[0]);

    // Test 15: Halt flag
    // EXPECT: Halted=1
    printf("Halted=%d\n", cpu.halted);

    // Test 16: PC after halt
    // EXPECT: PC after halt=4
    printf("PC after halt=%d\n", cpu.pc);

    return 0;
}
