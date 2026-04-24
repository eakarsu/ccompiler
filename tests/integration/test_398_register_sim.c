int printf(const char *fmt, ...);
// EXPECT: Program: 3 + 4\n  PC=3 FLAGS=[]\n  R0=7 R1=4 R2=0 R3=0\nProgram: 10 - 10\n  PC=3 FLAGS=[Z]\n  R0=0 R1=10 R2=0 R3=0\nProgram: 0xFF AND 0x0F\n  PC=3 FLAGS=[]\n  R0=15 R1=15 R2=0 R3=0\nProgram: 1 << 8\n  PC=2 FLAGS=[]\n  R0=256 R1=0 R2=0 R3=0\nProgram: 5!\n  5! = 120
// Test: register simulation

typedef struct {
    int regs[8];       // 8 general-purpose registers
    int flags;         // status flags
    int pc;            // program counter
} CPU;

enum {
    FLAG_ZERO    = 1,
    FLAG_CARRY   = 2,
    FLAG_NEG     = 4,
    FLAG_OVERFLOW = 8
};

void cpu_init(CPU *cpu) {
    int i;
    for (i = 0; i < 8; i++) cpu->regs[i] = 0;
    cpu->flags = 0;
    cpu->pc = 0;
}

void update_flags(CPU *cpu, int result) {
    cpu->flags = 0;
    if (result == 0) cpu->flags = cpu->flags | FLAG_ZERO;
    if (result < 0)  cpu->flags = cpu->flags | FLAG_NEG;
}

void cpu_mov(CPU *cpu, int dst, int val) {
    cpu->regs[dst] = val;
    cpu->pc++;
}

void cpu_add(CPU *cpu, int dst, int src) {
    cpu->regs[dst] = cpu->regs[dst] + cpu->regs[src];
    update_flags(cpu, cpu->regs[dst]);
    cpu->pc++;
}

void cpu_sub(CPU *cpu, int dst, int src) {
    cpu->regs[dst] = cpu->regs[dst] - cpu->regs[src];
    update_flags(cpu, cpu->regs[dst]);
    cpu->pc++;
}

void cpu_and(CPU *cpu, int dst, int src) {
    cpu->regs[dst] = cpu->regs[dst] & cpu->regs[src];
    update_flags(cpu, cpu->regs[dst]);
    cpu->pc++;
}

void cpu_or(CPU *cpu, int dst, int src) {
    cpu->regs[dst] = cpu->regs[dst] | cpu->regs[src];
    update_flags(cpu, cpu->regs[dst]);
    cpu->pc++;
}

void cpu_xor(CPU *cpu, int dst, int src) {
    cpu->regs[dst] = cpu->regs[dst] ^ cpu->regs[src];
    update_flags(cpu, cpu->regs[dst]);
    cpu->pc++;
}

void cpu_shl(CPU *cpu, int dst, int amount) {
    cpu->regs[dst] = cpu->regs[dst] << amount;
    update_flags(cpu, cpu->regs[dst]);
    cpu->pc++;
}

void cpu_shr(CPU *cpu, int dst, int amount) {
    cpu->regs[dst] = cpu->regs[dst] >> amount;
    update_flags(cpu, cpu->regs[dst]);
    cpu->pc++;
}

void print_state(CPU *cpu) {
    printf("  PC=%d FLAGS=[", cpu->pc);
    if (cpu->flags & FLAG_ZERO) printf("Z");
    if (cpu->flags & FLAG_NEG)  printf("N");
    if (cpu->flags & FLAG_CARRY) printf("C");
    if (cpu->flags & FLAG_OVERFLOW) printf("V");
    printf("]\n");
    printf("  R0=%d R1=%d R2=%d R3=%d\n",
           cpu->regs[0], cpu->regs[1], cpu->regs[2], cpu->regs[3]);
}

int main(void) {
    CPU cpu;
    cpu_init(&cpu);

    // Program: compute 3 + 4
    printf("Program: 3 + 4\n");
    cpu_mov(&cpu, 0, 3);
    cpu_mov(&cpu, 1, 4);
    cpu_add(&cpu, 0, 1);
    print_state(&cpu);

    // Program: compute 10 - 10 (zero flag)
    printf("Program: 10 - 10\n");
    cpu_init(&cpu);
    cpu_mov(&cpu, 0, 10);
    cpu_mov(&cpu, 1, 10);
    cpu_sub(&cpu, 0, 1);
    print_state(&cpu);

    // Program: bitwise operations
    printf("Program: 0xFF AND 0x0F\n");
    cpu_init(&cpu);
    cpu_mov(&cpu, 0, 0xFF);
    cpu_mov(&cpu, 1, 0x0F);
    cpu_and(&cpu, 0, 1);
    print_state(&cpu);

    // Program: shift left
    printf("Program: 1 << 8\n");
    cpu_init(&cpu);
    cpu_mov(&cpu, 0, 1);
    cpu_shl(&cpu, 0, 8);
    print_state(&cpu);

    // Program: compute factorial of 5 using registers
    printf("Program: 5!\n");
    cpu_init(&cpu);
    cpu_mov(&cpu, 0, 1);   // result = 1
    cpu_mov(&cpu, 1, 5);   // counter = 5
    cpu_mov(&cpu, 2, 1);   // decrement value
    int i;
    for (i = 0; i < 5; i++) {
        // result *= counter  (simulate with repeated addition)
        int mult = cpu.regs[1];
        int res = 0;
        int j;
        for (j = 0; j < mult; j++) {
            res = res + cpu.regs[0];
        }
        cpu.regs[0] = res;
        cpu_sub(&cpu, 1, 2);  // counter--
    }
    printf("  5! = %d\n", cpu.regs[0]);

    return 0;
}
