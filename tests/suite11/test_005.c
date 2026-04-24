int printf(const char *fmt, ...);

// Opcode dispatch loop using function pointer jump table

typedef void (*handler_fn)(int *regs, int *stack, int *sp, int arg);

void op_push(int *regs, int *stack, int *sp, int arg) {
    stack[*sp] = arg;
    *sp = *sp + 1;
}

void op_pop_to_reg(int *regs, int *stack, int *sp, int arg) {
    *sp = *sp - 1;
    regs[arg] = stack[*sp];
}

void op_push_reg(int *regs, int *stack, int *sp, int arg) {
    stack[*sp] = regs[arg];
    *sp = *sp + 1;
}

void op_add(int *regs, int *stack, int *sp, int arg) {
    int b = stack[*sp - 1];
    int a = stack[*sp - 2];
    *sp = *sp - 2;
    stack[*sp] = a + b;
    *sp = *sp + 1;
}

void op_sub(int *regs, int *stack, int *sp, int arg) {
    int b = stack[*sp - 1];
    int a = stack[*sp - 2];
    *sp = *sp - 2;
    stack[*sp] = a - b;
    *sp = *sp + 1;
}

void op_mul(int *regs, int *stack, int *sp, int arg) {
    int b = stack[*sp - 1];
    int a = stack[*sp - 2];
    *sp = *sp - 2;
    stack[*sp] = a * b;
    *sp = *sp + 1;
}

void op_print_top(int *regs, int *stack, int *sp, int arg) {
    // EXPECT: top: 15
    // EXPECT: top: 42
    // EXPECT: top: 6
    // EXPECT: top: 120
    printf("top: %d\n", stack[*sp - 1]);
}

void op_nop(int *regs, int *stack, int *sp, int arg) {
    // do nothing
}

struct Bytecode {
    int op;
    int arg;
};

int main() {
    // Jump table of 8 handlers
    handler_fn table[8];
    table[0] = op_nop;
    table[1] = op_push;
    table[2] = op_pop_to_reg;
    table[3] = op_push_reg;
    table[4] = op_add;
    table[5] = op_sub;
    table[6] = op_mul;
    table[7] = op_print_top;

    int regs[8];
    int stack[32];
    int sp;
    int i;

    for (i = 0; i < 8; i++) regs[i] = 0;
    for (i = 0; i < 32; i++) stack[i] = 0;
    sp = 0;

    // Program 1: push 7, push 8, add, print => 15
    struct Bytecode prog1[8];
    prog1[0].op = 1; prog1[0].arg = 7;    // push 7
    prog1[1].op = 1; prog1[1].arg = 8;    // push 8
    prog1[2].op = 4; prog1[2].arg = 0;    // add
    prog1[3].op = 7; prog1[3].arg = 0;    // print

    for (i = 0; i < 4; i++) {
        table[prog1[i].op](regs, stack, &sp, prog1[i].arg);
    }

    // Program 2: push 50, push 8, sub, print => 42
    sp = 0;
    struct Bytecode prog2[8];
    prog2[0].op = 1; prog2[0].arg = 50;
    prog2[1].op = 1; prog2[1].arg = 8;
    prog2[2].op = 5; prog2[2].arg = 0;    // sub
    prog2[3].op = 7; prog2[3].arg = 0;    // print

    for (i = 0; i < 4; i++) {
        table[prog2[i].op](regs, stack, &sp, prog2[i].arg);
    }

    // Program 3: push 2, push 3, mul, print => 6
    sp = 0;
    struct Bytecode prog3[8];
    prog3[0].op = 1; prog3[0].arg = 2;
    prog3[1].op = 1; prog3[1].arg = 3;
    prog3[2].op = 6; prog3[2].arg = 0;    // mul
    prog3[3].op = 7; prog3[3].arg = 0;

    for (i = 0; i < 4; i++) {
        table[prog3[i].op](regs, stack, &sp, prog3[i].arg);
    }

    // Program 4: compute 5! = 120 using the stack
    // 1*2=2, *3=6, *4=24, *5=120
    sp = 0;
    struct Bytecode prog4[16];
    int pi = 0;
    prog4[pi].op = 1; prog4[pi].arg = 1; pi = pi + 1;
    prog4[pi].op = 1; prog4[pi].arg = 2; pi = pi + 1;
    prog4[pi].op = 6; prog4[pi].arg = 0; pi = pi + 1;  // 1*2=2
    prog4[pi].op = 1; prog4[pi].arg = 3; pi = pi + 1;
    prog4[pi].op = 6; prog4[pi].arg = 0; pi = pi + 1;  // 2*3=6
    prog4[pi].op = 1; prog4[pi].arg = 4; pi = pi + 1;
    prog4[pi].op = 6; prog4[pi].arg = 0; pi = pi + 1;  // 6*4=24
    prog4[pi].op = 1; prog4[pi].arg = 5; pi = pi + 1;
    prog4[pi].op = 6; prog4[pi].arg = 0; pi = pi + 1;  // 24*5=120
    prog4[pi].op = 7; prog4[pi].arg = 0; pi = pi + 1;  // print

    for (i = 0; i < pi; i++) {
        table[prog4[i].op](regs, stack, &sp, prog4[i].arg);
    }

    // Test pop_to_reg and push_reg
    sp = 0;
    stack[0] = 0;
    regs[0] = 0;
    regs[3] = 0;

    struct Bytecode prog5[8];
    prog5[0].op = 1; prog5[0].arg = 77;   // push 77
    prog5[1].op = 2; prog5[1].arg = 3;    // pop to r3
    prog5[2].op = 3; prog5[2].arg = 3;    // push r3

    for (i = 0; i < 3; i++) {
        table[prog5[i].op](regs, stack, &sp, prog5[i].arg);
    }

    printf("r3=%d sp=%d top=%d\n", regs[3], sp, stack[sp - 1]);
    // EXPECT: r3=77 sp=1 top=77

    printf("dispatch done\n");
    // EXPECT: dispatch done

    return 0;
}
