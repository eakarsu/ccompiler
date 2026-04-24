int printf(const char *fmt, ...);

struct CharStack {
    char data[64];
    int top;
};

void stack_init(struct CharStack *s) {
    s->top = -1;
}

void stack_push(struct CharStack *s, char c) {
    s->top = s->top + 1;
    s->data[s->top] = c;
}

char stack_pop(struct CharStack *s) {
    char c = s->data[s->top];
    s->top = s->top - 1;
    return c;
}

char stack_peek(struct CharStack *s) {
    return s->data[s->top];
}

int stack_empty(struct CharStack *s) {
    return s->top < 0;
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int infix_to_postfix(char *infix, char *postfix) {
    struct CharStack ops;
    stack_init(&ops);
    int i = 0;
    int j = 0;

    while (infix[i] != 0) {
        char c = infix[i];
        if (c == ' ') {
            i = i + 1;
            continue;
        }

        if (is_digit(c)) {
            while (infix[i] != 0 && is_digit(infix[i])) {
                postfix[j] = infix[i];
                j = j + 1;
                i = i + 1;
            }
            postfix[j] = ' ';
            j = j + 1;
            continue;
        }

        if (c == '(') {
            stack_push(&ops, c);
        } else if (c == ')') {
            while (!stack_empty(&ops) && stack_peek(&ops) != '(') {
                postfix[j] = stack_pop(&ops);
                j = j + 1;
                postfix[j] = ' ';
                j = j + 1;
            }
            if (!stack_empty(&ops)) {
                stack_pop(&ops);
            }
        } else if (is_operator(c)) {
            while (!stack_empty(&ops) && stack_peek(&ops) != '(' &&
                   precedence(stack_peek(&ops)) >= precedence(c)) {
                postfix[j] = stack_pop(&ops);
                j = j + 1;
                postfix[j] = ' ';
                j = j + 1;
            }
            stack_push(&ops, c);
        }
        i = i + 1;
    }

    while (!stack_empty(&ops)) {
        postfix[j] = stack_pop(&ops);
        j = j + 1;
        postfix[j] = ' ';
        j = j + 1;
    }

    if (j > 0 && postfix[j - 1] == ' ') {
        j = j - 1;
    }
    postfix[j] = 0;
    return j;
}

void test_conversion(char *expr, char *expected) {
    char postfix[128];
    int i;
    for (i = 0; i < 128; i++) postfix[i] = 0;

    infix_to_postfix(expr, postfix);

    int match = 1;
    i = 0;
    while (expected[i] != 0 || postfix[i] != 0) {
        if (expected[i] != postfix[i]) {
            match = 0;
            break;
        }
        i = i + 1;
    }
    // Prints inside test_conversion
    printf("Infix: %s\n", expr);
    printf("Postfix: %s\n", postfix);
    if (match) {
        printf("PASS\n");
    } else {
        printf("FAIL expected: %s\n", expected);
    }
}

int main() {
    // EXPECT: Infix: 3 + 4
    // EXPECT: Postfix: 3 4 +
    // EXPECT: PASS
    test_conversion("3 + 4", "3 4 +");

    // EXPECT: Infix: 3 + 4 * 5
    // EXPECT: Postfix: 3 4 5 * +
    // EXPECT: PASS
    test_conversion("3 + 4 * 5", "3 4 5 * +");

    // EXPECT: Infix: (3 + 4) * 5
    // EXPECT: Postfix: 3 4 + 5 *
    // EXPECT: PASS
    test_conversion("(3 + 4) * 5", "3 4 + 5 *");

    // EXPECT: Infix: 2 * (3 + 4) - 1
    // EXPECT: Postfix: 2 3 4 + * 1 -
    // EXPECT: PASS
    test_conversion("2 * (3 + 4) - 1", "2 3 4 + * 1 -");

    // EXPECT: Infix: (1 + 2) * (3 + 4)
    // EXPECT: Postfix: 1 2 + 3 4 + *
    // EXPECT: PASS
    test_conversion("(1 + 2) * (3 + 4)", "1 2 + 3 4 + *");

    // EXPECT: Infix: 8 / 4 + 3 * 2
    // EXPECT: Postfix: 8 4 / 3 2 * +
    // EXPECT: PASS
    test_conversion("8 / 4 + 3 * 2", "8 4 / 3 2 * +");

    // EXPECT: Infix: 1 + 2 + 3
    // EXPECT: Postfix: 1 2 + 3 +
    // EXPECT: PASS
    test_conversion("1 + 2 + 3", "1 2 + 3 +");

    // EXPECT: All infix-to-postfix tests done
    printf("All infix-to-postfix tests done\n");
    return 0;
}
