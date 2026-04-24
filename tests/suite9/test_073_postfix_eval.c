int printf(const char *fmt, ...);

struct IntStack {
    int data[64];
    int top;
};

void stack_init(struct IntStack *s) {
    s->top = -1;
}

void stack_push(struct IntStack *s, int val) {
    s->top = s->top + 1;
    s->data[s->top] = val;
}

int stack_pop(struct IntStack *s) {
    int val = s->data[s->top];
    s->top = s->top - 1;
    return val;
}

int stack_peek(struct IntStack *s) {
    return s->data[s->top];
}

int stack_size(struct IntStack *s) {
    return s->top + 1;
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int eval_postfix(char *expr) {
    struct IntStack stk;
    stack_init(&stk);
    int i = 0;

    while (expr[i] != 0) {
        char c = expr[i];

        if (c == ' ') {
            i = i + 1;
            continue;
        }

        if (is_digit(c)) {
            int num = 0;
            while (expr[i] != 0 && is_digit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i = i + 1;
            }
            stack_push(&stk, num);
            continue;
        }

        if (c == '+' || c == '-' || c == '*' || c == '/') {
            int b = stack_pop(&stk);
            int a = stack_pop(&stk);
            int result = 0;
            if (c == '+') result = a + b;
            else if (c == '-') result = a - b;
            else if (c == '*') result = a * b;
            else if (c == '/') {
                if (b != 0) result = a / b;
                else result = 0;
            }
            stack_push(&stk, result);
        }

        i = i + 1;
    }

    return stack_pop(&stk);
}

void test_eval(char *expr, int expected) {
    int result = eval_postfix(expr);
    printf("Postfix: %s\n", expr);
    printf("Result: %d\n", result);
    if (result == expected) {
        printf("PASS\n");
    } else {
        printf("FAIL expected %d\n", expected);
    }
}

int main() {
    // EXPECT: Postfix: 3 4 +
    // EXPECT: Result: 7
    // EXPECT: PASS
    test_eval("3 4 +", 7);

    // EXPECT: Postfix: 3 4 5 * +
    // EXPECT: Result: 23
    // EXPECT: PASS
    test_eval("3 4 5 * +", 23);

    // EXPECT: Postfix: 3 4 + 5 *
    // EXPECT: Result: 35
    // EXPECT: PASS
    test_eval("3 4 + 5 *", 35);

    // EXPECT: Postfix: 10 3 -
    // EXPECT: Result: 7
    // EXPECT: PASS
    test_eval("10 3 -", 7);

    // EXPECT: Postfix: 15 3 /
    // EXPECT: Result: 5
    // EXPECT: PASS
    test_eval("15 3 /", 5);

    // EXPECT: Postfix: 2 3 4 + * 1 -
    // EXPECT: Result: 13
    // EXPECT: PASS
    test_eval("2 3 4 + * 1 -", 13);

    // EXPECT: Postfix: 1 2 + 3 4 + *
    // EXPECT: Result: 21
    // EXPECT: PASS
    test_eval("1 2 + 3 4 + *", 21);

    // EXPECT: Postfix: 8 4 / 3 2 * +
    // EXPECT: Result: 8
    // EXPECT: PASS
    test_eval("8 4 / 3 2 * +", 8);

    // EXPECT: Postfix: 100 50 25 + -
    // EXPECT: Result: 25
    // EXPECT: PASS
    test_eval("100 50 25 + -", 25);

    // EXPECT: All postfix evaluations done
    printf("All postfix evaluations done\n");
    return 0;
}
