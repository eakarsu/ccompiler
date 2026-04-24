int printf(const char *fmt, ...);

enum OpCode {
    OP_ADD = 0,
    OP_SUB = 1,
    OP_MUL = 2,
    OP_DIV = 3,
    OP_NEG = 4,
    OP_ABS = 5,
    OP_NOP = 6
};

typedef struct {
    enum OpCode op;
    int arg1;
    int arg2;
} Instruction;

int execute(Instruction inst) {
    switch (inst.op) {
        case OP_ADD: return inst.arg1 + inst.arg2;
        case OP_SUB: return inst.arg1 - inst.arg2;
        case OP_MUL: return inst.arg1 * inst.arg2;
        case OP_DIV:
            if (inst.arg2 == 0) return 0;
            return inst.arg1 / inst.arg2;
        case OP_NEG: return -inst.arg1;
        case OP_ABS:
            if (inst.arg1 < 0) return -inst.arg1;
            return inst.arg1;
        case OP_NOP: return 0;
        default: return -1;
    }
}

char *op_name(enum OpCode op) {
    switch (op) {
        case OP_ADD: return "ADD";
        case OP_SUB: return "SUB";
        case OP_MUL: return "MUL";
        case OP_DIV: return "DIV";
        case OP_NEG: return "NEG";
        case OP_ABS: return "ABS";
        case OP_NOP: return "NOP";
        default: return "UNK";
    }
}

void test_basic_dispatch() {
    Instruction inst;

    inst.op = OP_ADD;
    inst.arg1 = 10;
    inst.arg2 = 20;
    // EXPECT: ADD(10,20)=30
    printf("%s(%d,%d)=%d\n", op_name(inst.op), inst.arg1, inst.arg2, execute(inst));

    inst.op = OP_SUB;
    inst.arg1 = 50;
    inst.arg2 = 15;
    // EXPECT: SUB(50,15)=35
    printf("%s(%d,%d)=%d\n", op_name(inst.op), inst.arg1, inst.arg2, execute(inst));

    inst.op = OP_MUL;
    inst.arg1 = 7;
    inst.arg2 = 8;
    // EXPECT: MUL(7,8)=56
    printf("%s(%d,%d)=%d\n", op_name(inst.op), inst.arg1, inst.arg2, execute(inst));

    inst.op = OP_DIV;
    inst.arg1 = 100;
    inst.arg2 = 4;
    // EXPECT: DIV(100,4)=25
    printf("%s(%d,%d)=%d\n", op_name(inst.op), inst.arg1, inst.arg2, execute(inst));
}

void test_unary_dispatch() {
    Instruction inst;

    inst.op = OP_NEG;
    inst.arg1 = 42;
    inst.arg2 = 0;
    // EXPECT: NEG(42)=-42
    printf("%s(%d)=%d\n", op_name(inst.op), inst.arg1, execute(inst));

    inst.op = OP_ABS;
    inst.arg1 = -99;
    inst.arg2 = 0;
    // EXPECT: ABS(-99)=99
    printf("%s(%d)=%d\n", op_name(inst.op), inst.arg1, execute(inst));

    inst.op = OP_ABS;
    inst.arg1 = 77;
    inst.arg2 = 0;
    // EXPECT: ABS(77)=77
    printf("%s(%d)=%d\n", op_name(inst.op), inst.arg1, execute(inst));
}

void test_program() {
    Instruction program[6];
    int regs[4];
    regs[0] = 0;
    regs[1] = 0;
    regs[2] = 0;
    regs[3] = 0;

    program[0].op = OP_ADD;
    program[0].arg1 = 5;
    program[0].arg2 = 3;

    program[1].op = OP_MUL;
    program[1].arg1 = 0;
    program[1].arg2 = 4;

    program[2].op = OP_SUB;
    program[2].arg1 = 0;
    program[2].arg2 = 2;

    program[3].op = OP_NEG;
    program[3].arg1 = 0;
    program[3].arg2 = 0;

    program[4].op = OP_ABS;
    program[4].arg1 = 0;
    program[4].arg2 = 0;

    program[5].op = OP_NOP;
    program[5].arg1 = 0;
    program[5].arg2 = 0;

    int acc = 0;
    int i;
    for (i = 0; i < 6; i++) {
        Instruction inst = program[i];
        if (inst.op == OP_NOP) {
            // EXPECT: NOP acc=30
            printf("NOP acc=%d\n", acc);
        } else if (inst.op == OP_NEG || inst.op == OP_ABS) {
            inst.arg1 = acc;
            acc = execute(inst);
        } else {
            if (i > 0) {
                inst.arg1 = acc;
            }
            acc = execute(inst);
        }
    }

    // EXPECT: final_acc=30
    printf("final_acc=%d\n", acc);
}

void test_dispatch_table() {
    int results[7];
    Instruction inst;
    int op;
    for (op = OP_ADD; op <= OP_NOP; op++) {
        inst.op = op;
        inst.arg1 = 10;
        inst.arg2 = 3;
        results[op] = execute(inst);
    }
    // EXPECT: table: 13 7 30 3 -10 10 0
    printf("table: %d %d %d %d %d %d %d\n",
        results[0], results[1], results[2], results[3],
        results[4], results[5], results[6]);
}

int main() {
    test_basic_dispatch();
    test_unary_dispatch();
    test_program();
    test_dispatch_table();
    // EXPECT: done
    printf("done\n");
    return 0;
}
