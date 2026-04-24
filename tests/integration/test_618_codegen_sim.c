int printf(const char *fmt, ...);
// EXPECT: === Program 1: Simple Add ===\nExecuting 5 instructions\nOP_LOAD_IMM: r0 = 10\nOP_LOAD_IMM: r1 = 20\nOP_ADD: r2 = r0 + r1 = 30\nOP_PRINT: r2 = 30\nOP_HALT: stopping\nResult: 30\n=== Program 2: Multiply ===\nExecuting 4 instructions\nOP_LOAD_IMM: r0 = 100\nOP_LOAD_IMM: r1 = 3\nOP_MUL: r2 = r0 * r1 = 300\nOP_PRINT: r2 = 300\nResult: 300\n=== Program 3: (5+3)*(10-2) ===\nExecuting 8 instructions\nOP_LOAD_IMM: r0 = 5\nOP_LOAD_IMM: r1 = 3\nOP_ADD: r2 = r0 + r1 = 8\nOP_LOAD_IMM: r3 = 10\nOP_LOAD_IMM: r4 = 2\nOP_SUB: r5 = r3 - r4 = 8\nOP_MUL: r6 = r2 * r5 = 64\nOP_PRINT: r6 = 64\nResult: 64\n=== Program 4: Factorial 5! ===\nExecuting 10 instructions\nOP_LOAD_IMM: r0 = 1\nOP_LOAD_IMM: r1 = 2\nOP_LOAD_IMM: r2 = 3\nOP_LOAD_IMM: r3 = 4\nOP_LOAD_IMM: r4 = 5\nOP_MUL: r0 = r0 * r1 = 2\nOP_MUL: r0 = r0 * r2 = 6\nOP_MUL: r0 = r0 * r3 = 24\nOP_MUL: r0 = r0 * r4 = 120\nOP_PRINT: r0 = 120\nResult: 120\n=== Program 5: Division and Move ===\nExecuting 7 instructions\nOP_LOAD_IMM: r0 = 144\nOP_LOAD_IMM: r1 = 12\nOP_DIV: r2 = r0 / r1 = 12\nOP_MOV: r3 = r2 = 12\nOP_LOAD_IMM: r4 = 3\nOP_MUL: r5 = r3 * r4 = 36\nOP_PRINT: r5 = 36\nResult: 36\n=== Program 6: Multi-step chain ===\nExecuting 12 instructions\nOP_LOAD_IMM: r0 = 7\nOP_LOAD_IMM: r1 = 3\nOP_ADD: r2 = r0 + r1 = 10\nOP_LOAD_IMM: r3 = 2\nOP_MUL: r4 = r2 * r3 = 20\nOP_LOAD_IMM: r5 = 4\nOP_SUB: r6 = r4 - r5 = 16\nOP_MOV: r7 = r6 = 16\nOP_LOAD_IMM: r0 = 2\nOP_DIV: r1 = r7 / r0 = 8\nOP_PRINT: r1 = 8\nOP_HALT: stopping\nResult: 8\n=== Register dump after program 6 ===\nr0=2 r1=8 r2=10 r3=2 r4=20 r5=4 r6=16 r7=16\n=== All programs passed ===

// Test 618: Code generator simulator stress test
// Tests enum opcodes, instruction struct arrays, register simulation,
// switch dispatch, and multi-step computation chains.

enum Opcode {
    OP_LOAD_IMM,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOV,
    OP_PRINT,
    OP_HALT
};

struct Instr {
    int op;
    int dst;
    int src1;
    int src2;
    int imm;
};

// Helper to create instructions
struct Instr make_load_imm(int dst, int imm) {
    struct Instr i;
    i.op = OP_LOAD_IMM;
    i.dst = dst;
    i.src1 = 0;
    i.src2 = 0;
    i.imm = imm;
    return i;
}

struct Instr make_alu(int op, int dst, int src1, int src2) {
    struct Instr i;
    i.op = op;
    i.dst = dst;
    i.src1 = src1;
    i.src2 = src2;
    i.imm = 0;
    return i;
}

struct Instr make_mov(int dst, int src) {
    struct Instr i;
    i.op = OP_MOV;
    i.dst = dst;
    i.src1 = src;
    i.src2 = 0;
    i.imm = 0;
    return i;
}

struct Instr make_print(int reg) {
    struct Instr i;
    i.op = OP_PRINT;
    i.dst = reg;
    i.src1 = 0;
    i.src2 = 0;
    i.imm = 0;
    return i;
}

struct Instr make_halt() {
    struct Instr i;
    i.op = OP_HALT;
    i.dst = 0;
    i.src1 = 0;
    i.src2 = 0;
    i.imm = 0;
    return i;
}

// Opcode names for tracing
char *opcode_name(int op) {
    if (op == OP_LOAD_IMM) return "OP_LOAD_IMM";
    if (op == OP_ADD) return "OP_ADD";
    if (op == OP_SUB) return "OP_SUB";
    if (op == OP_MUL) return "OP_MUL";
    if (op == OP_DIV) return "OP_DIV";
    if (op == OP_MOV) return "OP_MOV";
    if (op == OP_PRINT) return "OP_PRINT";
    if (op == OP_HALT) return "OP_HALT";
    return "UNKNOWN";
}

// ALU op symbol
char alu_sym(int op) {
    if (op == OP_ADD) return '+';
    if (op == OP_SUB) return '-';
    if (op == OP_MUL) return '*';
    if (op == OP_DIV) return '/';
    return '?';
}

// Execute a program. Returns the last printed value.
int execute(struct Instr program[], int num_instr, int regs[]) {
    int i;
    int last_printed;
    int result;

    last_printed = 0;

    // Clear registers
    for (i = 0; i < 8; i++) {
        regs[i] = 0;
    }

    printf("Executing %d instructions\n", num_instr);

    for (i = 0; i < num_instr; i++) {
        struct Instr inst;
        inst = program[i];

        switch (inst.op) {
        case OP_LOAD_IMM:
            regs[inst.dst] = inst.imm;
            printf("%s: r%d = %d\n", opcode_name(inst.op), inst.dst, inst.imm);
            break;

        case OP_ADD:
        case OP_SUB:
        case OP_MUL:
        case OP_DIV:
            if (inst.op == OP_ADD) {
                result = regs[inst.src1] + regs[inst.src2];
            } else if (inst.op == OP_SUB) {
                result = regs[inst.src1] - regs[inst.src2];
            } else if (inst.op == OP_MUL) {
                result = regs[inst.src1] * regs[inst.src2];
            } else {
                result = regs[inst.src1] / regs[inst.src2];
            }
            regs[inst.dst] = result;
            printf("%s: r%d = r%d %c r%d = %d\n",
                   opcode_name(inst.op), inst.dst,
                   inst.src1, alu_sym(inst.op), inst.src2, result);
            break;

        case OP_MOV:
            regs[inst.dst] = regs[inst.src1];
            printf("%s: r%d = r%d = %d\n",
                   opcode_name(inst.op), inst.dst, inst.src1, regs[inst.dst]);
            break;

        case OP_PRINT:
            printf("%s: r%d = %d\n",
                   opcode_name(inst.op), inst.dst, regs[inst.dst]);
            last_printed = regs[inst.dst];
            break;

        case OP_HALT:
            printf("%s: stopping\n", opcode_name(inst.op));
            return last_printed;
        }
    }

    return last_printed;
}

// Build program 1: 10 + 20 = 30
int build_prog1(struct Instr prog[]) {
    int pc;
    pc = 0;
    prog[pc] = make_load_imm(0, 10); pc++;
    prog[pc] = make_load_imm(1, 20); pc++;
    prog[pc] = make_alu(OP_ADD, 2, 0, 1); pc++;
    prog[pc] = make_print(2); pc++;
    prog[pc] = make_halt(); pc++;
    return pc;
}

// Build program 2: 100 * 3 = 300
int build_prog2(struct Instr prog[]) {
    int pc;
    pc = 0;
    prog[pc] = make_load_imm(0, 100); pc++;
    prog[pc] = make_load_imm(1, 3); pc++;
    prog[pc] = make_alu(OP_MUL, 2, 0, 1); pc++;
    prog[pc] = make_print(2); pc++;
    return pc;
}

// Build program 3: (5+3) * (10-2) = 8 * 8 = 64
int build_prog3(struct Instr prog[]) {
    int pc;
    pc = 0;
    prog[pc] = make_load_imm(0, 5); pc++;
    prog[pc] = make_load_imm(1, 3); pc++;
    prog[pc] = make_alu(OP_ADD, 2, 0, 1); pc++;      // r2 = 8
    prog[pc] = make_load_imm(3, 10); pc++;
    prog[pc] = make_load_imm(4, 2); pc++;
    prog[pc] = make_alu(OP_SUB, 5, 3, 4); pc++;      // r5 = 8
    prog[pc] = make_alu(OP_MUL, 6, 2, 5); pc++;      // r6 = 64
    prog[pc] = make_print(6); pc++;
    return pc;
}

// Build program 4: 5! = 120 (unrolled iterative)
int build_prog4(struct Instr prog[]) {
    int pc;
    pc = 0;
    prog[pc] = make_load_imm(0, 1); pc++;             // r0 = acc = 1
    prog[pc] = make_load_imm(1, 2); pc++;             // r1 = 2
    prog[pc] = make_load_imm(2, 3); pc++;             // r2 = 3
    prog[pc] = make_load_imm(3, 4); pc++;             // r3 = 4
    prog[pc] = make_load_imm(4, 5); pc++;             // r4 = 5
    prog[pc] = make_alu(OP_MUL, 0, 0, 1); pc++;      // r0 = 1*2 = 2
    prog[pc] = make_alu(OP_MUL, 0, 0, 2); pc++;      // r0 = 2*3 = 6
    prog[pc] = make_alu(OP_MUL, 0, 0, 3); pc++;      // r0 = 6*4 = 24
    prog[pc] = make_alu(OP_MUL, 0, 0, 4); pc++;      // r0 = 24*5 = 120
    prog[pc] = make_print(0); pc++;
    return pc;
}

// Build program 5: 144 / 12 = 12, move to r3, * 3 = 36
int build_prog5(struct Instr prog[]) {
    int pc;
    pc = 0;
    prog[pc] = make_load_imm(0, 144); pc++;
    prog[pc] = make_load_imm(1, 12); pc++;
    prog[pc] = make_alu(OP_DIV, 2, 0, 1); pc++;      // r2 = 12
    prog[pc] = make_mov(3, 2); pc++;                  // r3 = 12
    prog[pc] = make_load_imm(4, 3); pc++;
    prog[pc] = make_alu(OP_MUL, 5, 3, 4); pc++;      // r5 = 36
    prog[pc] = make_print(5); pc++;
    return pc;
}

// Build program 6: ((7+3)*2 - 4) / 2 = (10*2-4)/2 = 16/2 = 8
int build_prog6(struct Instr prog[]) {
    int pc;
    pc = 0;
    prog[pc] = make_load_imm(0, 7); pc++;             // r0 = 7
    prog[pc] = make_load_imm(1, 3); pc++;             // r1 = 3
    prog[pc] = make_alu(OP_ADD, 2, 0, 1); pc++;       // r2 = 10
    prog[pc] = make_load_imm(3, 2); pc++;             // r3 = 2
    prog[pc] = make_alu(OP_MUL, 4, 2, 3); pc++;       // r4 = 20
    prog[pc] = make_load_imm(5, 4); pc++;             // r5 = 4
    prog[pc] = make_alu(OP_SUB, 6, 4, 5); pc++;       // r6 = 16
    prog[pc] = make_mov(7, 6); pc++;                   // r7 = 16
    prog[pc] = make_load_imm(0, 2); pc++;             // r0 = 2 (reuse r0)
    prog[pc] = make_alu(OP_DIV, 1, 7, 0); pc++;       // r1 = 16/2 = 8
    prog[pc] = make_print(1); pc++;
    prog[pc] = make_halt(); pc++;
    return pc;
}

void dump_regs(int regs[]) {
    printf("r0=%d r1=%d r2=%d r3=%d r4=%d r5=%d r6=%d r7=%d\n",
           regs[0], regs[1], regs[2], regs[3],
           regs[4], regs[5], regs[6], regs[7]);
}

int main() {
    struct Instr program[64];
    int regs[8];
    int num_instr;
    int result;

    // Program 1: Simple add
    printf("=== Program 1: Simple Add ===\n");
    num_instr = build_prog1(program);
    result = execute(program, num_instr, regs);
    printf("Result: %d\n", result);

    // Program 2: Multiply
    printf("=== Program 2: Multiply ===\n");
    num_instr = build_prog2(program);
    result = execute(program, num_instr, regs);
    printf("Result: %d\n", result);

    // Program 3: (5+3)*(10-2)
    printf("=== Program 3: (5+3)*(10-2) ===\n");
    num_instr = build_prog3(program);
    result = execute(program, num_instr, regs);
    printf("Result: %d\n", result);

    // Program 4: Factorial 5!
    printf("=== Program 4: Factorial 5! ===\n");
    num_instr = build_prog4(program);
    result = execute(program, num_instr, regs);
    printf("Result: %d\n", result);

    // Program 5: Division and Move
    printf("=== Program 5: Division and Move ===\n");
    num_instr = build_prog5(program);
    result = execute(program, num_instr, regs);
    printf("Result: %d\n", result);

    // Program 6: Multi-step chain
    printf("=== Program 6: Multi-step chain ===\n");
    num_instr = build_prog6(program);
    result = execute(program, num_instr, regs);
    printf("Result: %d\n", result);

    // Dump registers after program 6
    printf("=== Register dump after program 6 ===\n");
    dump_regs(regs);

    printf("=== All programs passed ===\n");
    return 0;
}
