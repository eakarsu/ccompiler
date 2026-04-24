int printf(const char *fmt, ...);

// Stack-based calculator VM with multiple operations
// Supports: numbers, +, -, *, /, dup, swap, over, rot, drop

enum CalcOp {
    C_NUM   = 0,
    C_ADD   = 1,
    C_SUB   = 2,
    C_MUL   = 3,
    C_DIV   = 4,
    C_DUP   = 5,
    C_SWAP  = 6,
    C_OVER  = 7,
    C_ROT   = 8,
    C_DROP  = 9,
    C_PRINT = 10,
    C_END   = 11
};

struct CalcVM {
    int stack[32];
    int sp;
};

void calc_init(struct CalcVM *c) {
    c->sp = 0;
    int i;
    for (i = 0; i < 32; i++) {
        c->stack[i] = 0;
    }
}

void calc_push(struct CalcVM *c, int v) {
    c->stack[c->sp] = v;
    c->sp = c->sp + 1;
}

int calc_pop(struct CalcVM *c) {
    c->sp = c->sp - 1;
    return c->stack[c->sp];
}

int calc_peek(struct CalcVM *c, int offset) {
    return c->stack[c->sp - 1 - offset];
}

void calc_exec(struct CalcVM *c, int *prog) {
    int pc = 0;
    int running = 1;
    while (running) {
        int op = prog[pc];
        pc = pc + 1;
        int a;
        int b;
        int tmp;

        if (op == C_NUM) {
            calc_push(c, prog[pc]);
            pc = pc + 1;
        } else if (op == C_ADD) {
            b = calc_pop(c);
            a = calc_pop(c);
            calc_push(c, a + b);
        } else if (op == C_SUB) {
            b = calc_pop(c);
            a = calc_pop(c);
            calc_push(c, a - b);
        } else if (op == C_MUL) {
            b = calc_pop(c);
            a = calc_pop(c);
            calc_push(c, a * b);
        } else if (op == C_DIV) {
            b = calc_pop(c);
            a = calc_pop(c);
            calc_push(c, a / b);
        } else if (op == C_DUP) {
            a = calc_peek(c, 0);
            calc_push(c, a);
        } else if (op == C_SWAP) {
            b = calc_pop(c);
            a = calc_pop(c);
            calc_push(c, b);
            calc_push(c, a);
        } else if (op == C_OVER) {
            a = calc_peek(c, 1);
            calc_push(c, a);
        } else if (op == C_ROT) {
            int c3 = calc_pop(c);
            int c2 = calc_pop(c);
            int c1 = calc_pop(c);
            calc_push(c, c2);
            calc_push(c, c3);
            calc_push(c, c1);
        } else if (op == C_DROP) {
            calc_pop(c);
        } else if (op == C_PRINT) {
            printf("= %d\n", calc_peek(c, 0));
        } else if (op == C_END) {
            running = 0;
        }
    }
}

int main() {
    struct CalcVM calc;
    calc_init(&calc);

    // Test 1: 5 3 + => 8
    int prog1[16];
    prog1[0] = C_NUM; prog1[1] = 5;
    prog1[2] = C_NUM; prog1[3] = 3;
    prog1[4] = C_ADD;
    prog1[5] = C_PRINT;
    prog1[6] = C_END;
    calc_exec(&calc, prog1);
    printf("stack depth: %d\n", calc.sp);
    // EXPECT: = 8
    // EXPECT: stack depth: 1

    // Test 2: dup and mul (square)
    calc_init(&calc);
    int prog2[16];
    prog2[0] = C_NUM; prog2[1] = 7;
    prog2[2] = C_DUP;
    prog2[3] = C_MUL;
    prog2[4] = C_PRINT;
    prog2[5] = C_END;
    calc_exec(&calc, prog2);
    // EXPECT: = 49

    // Test 3: swap
    calc_init(&calc);
    int prog3[16];
    prog3[0] = C_NUM; prog3[1] = 10;
    prog3[2] = C_NUM; prog3[3] = 20;
    prog3[4] = C_SWAP;
    prog3[5] = C_SUB;
    prog3[6] = C_PRINT;
    prog3[7] = C_END;
    calc_exec(&calc, prog3);
    // EXPECT: = 10

    // Test 4: over
    calc_init(&calc);
    int prog4[16];
    prog4[0] = C_NUM; prog4[1] = 3;
    prog4[2] = C_NUM; prog4[3] = 4;
    prog4[4] = C_OVER;
    prog4[5] = C_PRINT;
    prog4[6] = C_ADD;
    prog4[7] = C_PRINT;
    prog4[8] = C_END;
    calc_exec(&calc, prog4);
    // EXPECT: = 3
    // EXPECT: = 7

    // Test 5: complex expression (2+3)*4 = 20
    calc_init(&calc);
    int prog5[16];
    prog5[0] = C_NUM;  prog5[1] = 2;
    prog5[2] = C_NUM;  prog5[3] = 3;
    prog5[4] = C_ADD;
    prog5[5] = C_NUM;  prog5[6] = 4;
    prog5[7] = C_MUL;
    prog5[8] = C_PRINT;
    prog5[9] = C_END;
    calc_exec(&calc, prog5);
    // EXPECT: = 20

    printf("done\n");
    // EXPECT: done
    return 0;
}
