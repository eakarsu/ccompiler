int printf(const char *fmt, ...);

// Mini Stack Machine with bytecode instructions
// Instructions: PUSH, ADD, SUB, MUL, DIV, DUP, SWAP, POP, PRINT

// Opcodes
int OP_PUSH;
int OP_ADD;
int OP_SUB;
int OP_MUL;
int OP_DIV;
int OP_DUP;
int OP_SWAP;
int OP_POP;
int OP_PRINT;
int OP_HALT;

void init_opcodes(void) {
    OP_PUSH  = 1;
    OP_ADD   = 2;
    OP_SUB   = 3;
    OP_MUL   = 4;
    OP_DIV   = 5;
    OP_DUP   = 6;
    OP_SWAP  = 7;
    OP_POP   = 8;
    OP_PRINT = 9;
    OP_HALT  = 0;
}

int stack[64];
int sp;

void vm_init(void) {
    sp = 0;
}

void vm_push(int val) {
    stack[sp] = val;
    sp = sp + 1;
}

int vm_pop(void) {
    sp = sp - 1;
    return stack[sp];
}

// Execute a bytecode program
// Returns number of values printed
int vm_exec(int *program, int len) {
    int pc;
    int op;
    int a;
    int b;
    int tmp;
    int prints;

    vm_init();
    pc = 0;
    prints = 0;

    while (pc < len) {
        op = program[pc];
        pc = pc + 1;

        if (op == OP_PUSH) {
            vm_push(program[pc]);
            pc = pc + 1;
        } else if (op == OP_ADD) {
            b = vm_pop();
            a = vm_pop();
            vm_push(a + b);
        } else if (op == OP_SUB) {
            b = vm_pop();
            a = vm_pop();
            vm_push(a - b);
        } else if (op == OP_MUL) {
            b = vm_pop();
            a = vm_pop();
            vm_push(a * b);
        } else if (op == OP_DIV) {
            b = vm_pop();
            a = vm_pop();
            vm_push(a / b);
        } else if (op == OP_DUP) {
            a = vm_pop();
            vm_push(a);
            vm_push(a);
        } else if (op == OP_SWAP) {
            b = vm_pop();
            a = vm_pop();
            vm_push(b);
            vm_push(a);
        } else if (op == OP_POP) {
            vm_pop();
        } else if (op == OP_PRINT) {
            a = vm_pop();
            printf("out: %d\n", a);
            prints = prints + 1;
        } else if (op == OP_HALT) {
            return prints;
        }
    }
    return prints;
}

int main(void) {
    int prog1[16];
    int prog2[32];
    int prog3[32];
    int prog4[32];
    int prog5[48];
    int n;

    init_opcodes();

    // Program 1: push 3, push 4, add, print -> 7
    prog1[0] = OP_PUSH; prog1[1] = 3;
    prog1[2] = OP_PUSH; prog1[3] = 4;
    prog1[4] = OP_ADD;
    prog1[5] = OP_PRINT;
    prog1[6] = OP_HALT;
    n = vm_exec(prog1, 7);
    printf("prints: %d\n", n); // EXPECT: out: 7
    // EXPECT: prints: 1

    // Program 2: push 10, push 3, sub, dup, print, print -> 7, 7
    prog2[0] = OP_PUSH; prog2[1] = 10;
    prog2[2] = OP_PUSH; prog2[3] = 3;
    prog2[4] = OP_SUB;
    prog2[5] = OP_DUP;
    prog2[6] = OP_PRINT;
    prog2[7] = OP_PRINT;
    prog2[8] = OP_HALT;
    n = vm_exec(prog2, 9);
    printf("prints: %d\n", n); // EXPECT: out: 7
    // EXPECT: out: 7
    // EXPECT: prints: 2

    // Program 3: push 5, push 6, mul, print -> 30
    prog3[0] = OP_PUSH; prog3[1] = 5;
    prog3[2] = OP_PUSH; prog3[3] = 6;
    prog3[4] = OP_MUL;
    prog3[5] = OP_PRINT;
    prog3[6] = OP_HALT;
    n = vm_exec(prog3, 7);
    printf("prints: %d\n", n); // EXPECT: out: 30
    // EXPECT: prints: 1

    // Program 4: push 20, push 4, div, push 2, swap, sub, print -> 2 - 5 = -3
    // stack: 20 -> 20,4 -> 5 -> 5,2 -> swap: 2,5 -> sub: 2-5 = -3
    prog4[0] = OP_PUSH; prog4[1] = 20;
    prog4[2] = OP_PUSH; prog4[3] = 4;
    prog4[4] = OP_DIV;
    prog4[5] = OP_PUSH; prog4[6] = 2;
    prog4[7] = OP_SWAP;
    prog4[8] = OP_SUB;
    prog4[9] = OP_PRINT;
    prog4[10] = OP_HALT;
    n = vm_exec(prog4, 11);
    printf("prints: %d\n", n); // EXPECT: out: -3
    // EXPECT: prints: 1

    // Program 5: compute (3+4)*(5-2) = 7*3 = 21
    prog5[0]  = OP_PUSH; prog5[1]  = 3;
    prog5[2]  = OP_PUSH; prog5[3]  = 4;
    prog5[4]  = OP_ADD;
    prog5[5]  = OP_PUSH; prog5[6]  = 5;
    prog5[7]  = OP_PUSH; prog5[8]  = 2;
    prog5[9]  = OP_SUB;
    prog5[10] = OP_MUL;
    prog5[11] = OP_PRINT;
    prog5[12] = OP_HALT;
    n = vm_exec(prog5, 13);
    printf("prints: %d\n", n); // EXPECT: out: 21
    // EXPECT: prints: 1

    printf("all stack machine tests passed\n"); // EXPECT: all stack machine tests passed
    return 0;
}
