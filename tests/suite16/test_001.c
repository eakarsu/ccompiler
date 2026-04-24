int printf(const char *fmt, ...);

// RPN (Reverse Polish Notation) Calculator
// Evaluates postfix expressions using a stack

int stack[64];
int sp;

void stack_init(void) {
    sp = 0;
}

void stack_push(int val) {
    stack[sp] = val;
    sp = sp + 1;
}

int stack_pop(void) {
    sp = sp - 1;
    return stack[sp];
}

int stack_peek(void) {
    return stack[sp - 1];
}

int is_digit(int ch) {
    return ch >= 48 && ch <= 57;
}

// Evaluate an RPN expression string
// Supports: digits 0-9, +, -, *, /
int rpn_eval(char *expr) {
    int i;
    int a;
    int b;
    int ch;

    stack_init();
    i = 0;
    while (expr[i] != 0) {
        ch = expr[i];
        if (is_digit(ch)) {
            stack_push(ch - 48);
        } else if (ch == 43) {
            // '+'
            b = stack_pop();
            a = stack_pop();
            stack_push(a + b);
        } else if (ch == 45) {
            // '-'
            b = stack_pop();
            a = stack_pop();
            stack_push(a - b);
        } else if (ch == 42) {
            // '*'
            b = stack_pop();
            a = stack_pop();
            stack_push(a * b);
        } else if (ch == 47) {
            // '/'
            b = stack_pop();
            a = stack_pop();
            stack_push(a / b);
        }
        // skip spaces and unknown chars
        i = i + 1;
    }
    return stack_pop();
}

// Evaluate RPN with multi-digit number support (space-separated)
int rpn_eval_multi(char *expr) {
    int i;
    int a;
    int b;
    int ch;
    int num;
    int in_num;

    stack_init();
    i = 0;
    in_num = 0;
    num = 0;
    while (expr[i] != 0) {
        ch = expr[i];
        if (is_digit(ch)) {
            num = num * 10 + (ch - 48);
            in_num = 1;
        } else {
            if (in_num) {
                stack_push(num);
                num = 0;
                in_num = 0;
            }
            if (ch == 43) {
                b = stack_pop();
                a = stack_pop();
                stack_push(a + b);
            } else if (ch == 45) {
                b = stack_pop();
                a = stack_pop();
                stack_push(a - b);
            } else if (ch == 42) {
                b = stack_pop();
                a = stack_pop();
                stack_push(a * b);
            } else if (ch == 47) {
                b = stack_pop();
                a = stack_pop();
                stack_push(a / b);
            }
        }
        i = i + 1;
    }
    if (in_num) {
        stack_push(num);
    }
    return stack_pop();
}

int main(void) {
    int r;

    // Simple: 3 + 4 = 7
    r = rpn_eval("34+");
    printf("3 4 + = %d\n", r); // EXPECT: 3 4 + = 7

    // 5 - 2 = 3
    r = rpn_eval("52-");
    printf("5 2 - = %d\n", r); // EXPECT: 5 2 - = 3

    // 6 * 7 = 42
    r = rpn_eval("67*");
    printf("6 7 * = %d\n", r); // EXPECT: 6 7 * = 42

    // 8 / 2 = 4
    r = rpn_eval("82/");
    printf("8 2 / = %d\n", r); // EXPECT: 8 2 / = 4

    // (3 + 4) * 2 = 14 => 3 4 + 2 *
    r = rpn_eval("34+2*");
    printf("3 4 + 2 * = %d\n", r); // EXPECT: 3 4 + 2 * = 14

    // 5 1 2 + 4 * + 3 - = 5 + (1+2)*4 - 3 = 5+12-3 = 14
    r = rpn_eval("512+4*+3-");
    printf("5 1 2 + 4 * + 3 - = %d\n", r); // EXPECT: 5 1 2 + 4 * + 3 - = 14

    // 9 3 / 2 + = 3+2 = 5
    r = rpn_eval("93/2+");
    printf("9 3 / 2 + = %d\n", r); // EXPECT: 9 3 / 2 + = 5

    // Multi-digit: 10 20 + = 30
    r = rpn_eval_multi("10 20 +");
    printf("10 20 + = %d\n", r); // EXPECT: 10 20 + = 30

    // Multi-digit: 100 3 * = 300
    r = rpn_eval_multi("100 3 *");
    printf("100 3 * = %d\n", r); // EXPECT: 100 3 * = 300

    // Multi-digit: 50 25 - 10 * = 250
    r = rpn_eval_multi("50 25 - 10 *");
    printf("50 25 - 10 * = %d\n", r); // EXPECT: 50 25 - 10 * = 250

    // Complex: 15 7 1 1 + - / 3 * 2 1 1 + + -
    // = 15 / (7 - (1+1)) * 3 - (2 + (1+1))
    // = 15/5*3 - 4 = 9 - 4 = 5
    r = rpn_eval_multi("15 7 1 1 + - / 3 * 2 1 1 + + -");
    printf("complex = %d\n", r); // EXPECT: complex = 5

    printf("all rpn tests passed\n"); // EXPECT: all rpn tests passed
    return 0;
}
