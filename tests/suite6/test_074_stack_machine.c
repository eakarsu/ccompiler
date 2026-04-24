int printf(const char *fmt, ...);

int stack[256];
int sp;

int OP_PUSH = 1;
int OP_ADD = 2;
int OP_SUB = 3;
int OP_MUL = 4;
int OP_DIV = 5;
int OP_DUP = 6;
int OP_SWAP = 7;
int OP_PRINT = 8;
int OP_OVER = 9;
int OP_DROP = 10;
int OP_HALT = 0;

int prog_ops[128];
int prog_args[128];
int prog_len;

void reset(void) {
    sp = 0;
    prog_len = 0;
}

void emit(int op, int arg) {
    prog_ops[prog_len] = op;
    prog_args[prog_len] = arg;
    prog_len = prog_len + 1;
}

void push(int val) {
    stack[sp] = val;
    sp = sp + 1;
}

int pop(void) {
    sp = sp - 1;
    return stack[sp];
}

int peek(void) {
    return stack[sp - 1];
}

int run(void) {
    int pc = 0;
    int output_count = 0;
    while (pc < prog_len) {
        int op = prog_ops[pc];
        int arg = prog_args[pc];

        if (op == OP_PUSH) {
            push(arg);
        } else if (op == OP_ADD) {
            int b = pop();
            int a = pop();
            push(a + b);
        } else if (op == OP_SUB) {
            int b = pop();
            int a = pop();
            push(a - b);
        } else if (op == OP_MUL) {
            int b = pop();
            int a = pop();
            push(a * b);
        } else if (op == OP_DIV) {
            int b = pop();
            int a = pop();
            push(a / b);
        } else if (op == OP_DUP) {
            int val = peek();
            push(val);
        } else if (op == OP_SWAP) {
            int b = pop();
            int a = pop();
            push(b);
            push(a);
        } else if (op == OP_OVER) {
            int val = stack[sp - 2];
            push(val);
        } else if (op == OP_DROP) {
            pop();
        } else if (op == OP_PRINT) {
            printf("result=%d\n", peek());
            output_count = output_count + 1;
        } else if (op == OP_HALT) {
            break;
        }
        pc = pc + 1;
    }
    return output_count;
}

int main(void) {
    reset();
    emit(OP_PUSH, 3);
    emit(OP_PUSH, 4);
    emit(OP_ADD, 0);
    emit(OP_PRINT, 0);
    emit(OP_HALT, 0);
    // EXPECT: result=7
    run();

    reset();
    emit(OP_PUSH, 10);
    emit(OP_PUSH, 3);
    emit(OP_SUB, 0);
    emit(OP_PRINT, 0);
    emit(OP_HALT, 0);
    // EXPECT: result=7
    run();

    reset();
    emit(OP_PUSH, 6);
    emit(OP_PUSH, 7);
    emit(OP_MUL, 0);
    emit(OP_PRINT, 0);
    emit(OP_HALT, 0);
    // EXPECT: result=42
    run();

    reset();
    emit(OP_PUSH, 100);
    emit(OP_PUSH, 4);
    emit(OP_DIV, 0);
    emit(OP_PRINT, 0);
    emit(OP_HALT, 0);
    // EXPECT: result=25
    run();

    reset();
    emit(OP_PUSH, 5);
    emit(OP_DUP, 0);
    emit(OP_MUL, 0);
    emit(OP_PRINT, 0);
    emit(OP_HALT, 0);
    // EXPECT: result=25
    run();

    reset();
    emit(OP_PUSH, 10);
    emit(OP_PUSH, 20);
    emit(OP_SWAP, 0);
    emit(OP_SUB, 0);
    emit(OP_PRINT, 0);
    emit(OP_HALT, 0);
    // EXPECT: result=10
    run();

    reset();
    emit(OP_PUSH, 3);
    emit(OP_PUSH, 5);
    emit(OP_OVER, 0);
    emit(OP_ADD, 0);
    emit(OP_PRINT, 0);
    emit(OP_HALT, 0);
    // EXPECT: result=8
    run();

    reset();
    emit(OP_PUSH, 2);
    emit(OP_PUSH, 3);
    emit(OP_ADD, 0);
    emit(OP_PUSH, 4);
    emit(OP_MUL, 0);
    emit(OP_PRINT, 0);
    emit(OP_HALT, 0);
    // EXPECT: result=20
    run();

    reset();
    emit(OP_PUSH, 10);
    emit(OP_DUP, 0);
    emit(OP_DUP, 0);
    emit(OP_ADD, 0);
    emit(OP_ADD, 0);
    emit(OP_PRINT, 0);
    emit(OP_HALT, 0);
    // EXPECT: result=30
    run();

    reset();
    emit(OP_PUSH, 100);
    emit(OP_PUSH, 200);
    emit(OP_DROP, 0);
    emit(OP_PRINT, 0);
    emit(OP_HALT, 0);
    // EXPECT: result=100
    run();

    reset();
    emit(OP_PUSH, 2);
    emit(OP_PUSH, 3);
    emit(OP_PUSH, 4);
    emit(OP_MUL, 0);
    emit(OP_ADD, 0);
    emit(OP_PRINT, 0);
    emit(OP_HALT, 0);
    // EXPECT: result=14
    run();

    reset();
    emit(OP_PUSH, 50);
    emit(OP_PUSH, 30);
    emit(OP_SUB, 0);
    emit(OP_PUSH, 2);
    emit(OP_DIV, 0);
    emit(OP_PRINT, 0);
    emit(OP_HALT, 0);
    // EXPECT: result=10
    run();

    reset();
    emit(OP_PUSH, 5);
    emit(OP_DUP, 0);
    emit(OP_ADD, 0);
    emit(OP_DUP, 0);
    emit(OP_ADD, 0);
    emit(OP_PRINT, 0);
    emit(OP_HALT, 0);
    // EXPECT: result=20
    run();

    reset();
    emit(OP_PUSH, 7);
    emit(OP_PUSH, 3);
    emit(OP_OVER, 0);
    emit(OP_MUL, 0);
    emit(OP_SWAP, 0);
    emit(OP_SUB, 0);
    emit(OP_PRINT, 0);
    emit(OP_HALT, 0);
    // EXPECT: result=14
    run();

    // EXPECT: sp=1
    printf("sp=%d\n", sp);

    return 0;
}
