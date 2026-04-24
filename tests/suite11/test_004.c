int printf(const char *fmt, ...);

// Instruction decoder: decodes packed 32-bit instructions
// Format: [opcode:8][dst:4][src1:4][src2:4][imm:12]

struct Instruction {
    int opcode;
    int dst;
    int src1;
    int src2;
    int imm;
    int raw;
};

void decode(int raw, struct Instruction *inst) {
    inst->raw = raw;
    inst->opcode = (raw >> 24) & 255;
    inst->dst    = (raw >> 20) & 15;
    inst->src1   = (raw >> 16) & 15;
    inst->src2   = (raw >> 12) & 15;
    inst->imm    = raw & 4095;
}

int encode(int opcode, int dst, int src1, int src2, int imm) {
    int result = 0;
    result = result | ((opcode & 255) << 24);
    result = result | ((dst & 15) << 20);
    result = result | ((src1 & 15) << 16);
    result = result | ((src2 & 15) << 12);
    result = result | (imm & 4095);
    return result;
}

void print_inst(struct Instruction *inst) {
    printf("op=%d dst=%d s1=%d s2=%d imm=%d\n",
           inst->opcode, inst->dst, inst->src1, inst->src2, inst->imm);
}

enum {
    I_NOP  = 0,
    I_ADDI = 1,
    I_ADD  = 2,
    I_SUB  = 3,
    I_MUL  = 4,
    I_LOAD = 5,
    I_STORE = 6,
    I_BEQ  = 7,
    I_JMP  = 8,
    I_HALT = 15
};

struct Processor {
    int regs[16];
    int mem[64];
    int pc;
    int halted;
};

void proc_init(struct Processor *p) {
    int i;
    for (i = 0; i < 16; i++) p->regs[i] = 0;
    for (i = 0; i < 64; i++) p->mem[i] = 0;
    p->pc = 0;
    p->halted = 0;
}

void proc_exec(struct Processor *p, struct Instruction *inst) {
    int op = inst->opcode;
    if (op == I_NOP) {
        // nothing
    } else if (op == I_ADDI) {
        p->regs[inst->dst] = p->regs[inst->src1] + inst->imm;
    } else if (op == I_ADD) {
        p->regs[inst->dst] = p->regs[inst->src1] + p->regs[inst->src2];
    } else if (op == I_SUB) {
        p->regs[inst->dst] = p->regs[inst->src1] - p->regs[inst->src2];
    } else if (op == I_MUL) {
        p->regs[inst->dst] = p->regs[inst->src1] * p->regs[inst->src2];
    } else if (op == I_LOAD) {
        int addr = p->regs[inst->src1] + inst->imm;
        p->regs[inst->dst] = p->mem[addr];
    } else if (op == I_STORE) {
        int addr = p->regs[inst->src1] + inst->imm;
        p->mem[addr] = p->regs[inst->dst];
    } else if (op == I_BEQ) {
        if (p->regs[inst->src1] == p->regs[inst->src2]) {
            p->pc = inst->imm;
            return;
        }
    } else if (op == I_JMP) {
        p->pc = inst->imm;
        return;
    } else if (op == I_HALT) {
        p->halted = 1;
        return;
    }
    p->pc = p->pc + 1;
}

int main() {
    // Test encode/decode roundtrip
    int raw = encode(I_ADDI, 3, 1, 0, 42);
    struct Instruction inst;
    decode(raw, &inst);
    print_inst(&inst);
    // EXPECT: op=1 dst=3 s1=1 s2=0 imm=42

    raw = encode(I_ADD, 5, 2, 3, 0);
    decode(raw, &inst);
    print_inst(&inst);
    // EXPECT: op=2 dst=5 s1=2 s2=3 imm=0

    raw = encode(I_MUL, 7, 4, 6, 0);
    decode(raw, &inst);
    print_inst(&inst);
    // EXPECT: op=4 dst=7 s1=4 s2=6 imm=0

    // Test processor: compute (5+3)*2 = 16
    struct Processor proc;
    proc_init(&proc);

    int program[8];
    program[0] = encode(I_ADDI, 1, 0, 0, 5);   // r1 = 5
    program[1] = encode(I_ADDI, 2, 0, 0, 3);   // r2 = 3
    program[2] = encode(I_ADD,  3, 1, 2, 0);   // r3 = r1+r2 = 8
    program[3] = encode(I_ADDI, 4, 0, 0, 2);   // r4 = 2
    program[4] = encode(I_MUL,  5, 3, 4, 0);   // r5 = r3*r4 = 16
    program[5] = encode(I_HALT, 0, 0, 0, 0);

    while (!proc.halted) {
        decode(program[proc.pc], &inst);
        proc_exec(&proc, &inst);
    }

    printf("r1=%d r2=%d r3=%d\n", proc.regs[1], proc.regs[2], proc.regs[3]);
    // EXPECT: r1=5 r2=3 r3=8
    printf("r4=%d r5=%d\n", proc.regs[4], proc.regs[5]);
    // EXPECT: r4=2 r5=16

    // Test store and load
    proc_init(&proc);
    program[0] = encode(I_ADDI, 1, 0, 0, 99);   // r1 = 99
    program[1] = encode(I_STORE, 1, 0, 0, 10);   // mem[10] = r1
    program[2] = encode(I_LOAD, 2, 0, 0, 10);    // r2 = mem[10]
    program[3] = encode(I_HALT, 0, 0, 0, 0);

    while (!proc.halted) {
        decode(program[proc.pc], &inst);
        proc_exec(&proc, &inst);
    }
    printf("store/load: r2=%d\n", proc.regs[2]);
    // EXPECT: store/load: r2=99
    printf("mem[10]=%d\n", proc.mem[10]);
    // EXPECT: mem[10]=99

    return 0;
}
