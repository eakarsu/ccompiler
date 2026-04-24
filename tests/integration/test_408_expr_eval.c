int printf(const char *fmt, ...);
// EXPECT: 3 4 + = 7\n5 3 - 4 * = 8\n2 3 + 4 5 + * = 45\n10 2 / = 5\n5 1 2 + 4 * + 3 - = 14\n8 3 * 2 + = 26\ncomplex = 5
// Test: Stack-based expression evaluation (postfix)

int stack[64];
int sp;

void stack_init(void) {
    sp = 0;
}

void push(int val) {
    stack[sp] = val;
    sp++;
}

int pop(void) {
    sp--;
    return stack[sp];
}

int peek(void) {
    return stack[sp - 1];
}

int is_empty(void) {
    return sp == 0;
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int eval_postfix(char *expr) {
    stack_init();
    int i = 0;
    while (expr[i] != 0) {
        char c = expr[i];
        if (c == ' ') {
            i++;
            continue;
        }
        if (is_digit(c)) {
            int num = 0;
            while (expr[i] != 0 && is_digit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }
            push(num);
        } else {
            int b = pop();
            int a = pop();
            if (c == '+') push(a + b);
            else if (c == '-') push(a - b);
            else if (c == '*') push(a * b);
            else if (c == '/') push(a / b);
            i++;
        }
    }
    return pop();
}

int main(void) {
    char e1[] = "3 4 +";
    printf("3 4 + = %d\n", eval_postfix(e1));

    char e2[] = "5 3 - 4 *";
    printf("5 3 - 4 * = %d\n", eval_postfix(e2));

    char e3[] = "2 3 + 4 5 + *";
    printf("2 3 + 4 5 + * = %d\n", eval_postfix(e3));

    char e4[] = "10 2 /";
    printf("10 2 / = %d\n", eval_postfix(e4));

    char e5[] = "5 1 2 + 4 * + 3 -";
    printf("5 1 2 + 4 * + 3 - = %d\n", eval_postfix(e5));

    char e6[] = "8 3 * 2 +";
    printf("8 3 * 2 + = %d\n", eval_postfix(e6));

    char e7[] = "15 7 1 1 + - / 3 * 2 1 1 + + -";
    printf("complex = %d\n", eval_postfix(e7));

    return 0;
}
