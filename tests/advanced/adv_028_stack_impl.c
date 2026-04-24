// EXPECT: === Push & Pop ===
// EXPECT: pushed: 10 20 30
// EXPECT: pop: 30
// EXPECT: pop: 20
// EXPECT: pop: 10
// EXPECT: === Peek ===
// EXPECT: peek: 50
// EXPECT: peek again: 50
// EXPECT: pop: 50
// EXPECT: peek: 40
// EXPECT: === isEmpty ===
// EXPECT: empty at start: yes
// EXPECT: after push: no
// EXPECT: after pop: yes
// EXPECT: === Stack Size ===
// EXPECT: size after 5 pushes: 5
// EXPECT: size after 2 pops: 3
// EXPECT: === Reverse Array ===
// EXPECT: original: 1 2 3 4 5
// EXPECT: reversed: 5 4 3 2 1
// EXPECT: === Balanced Parens ===
// EXPECT: "(())" balanced: yes
// EXPECT: "(()" balanced: no
// EXPECT: "" balanced: yes
// EXPECT: "()()" balanced: yes
// EXPECT: ")(" balanced: no
// EXPECT: === Expression Eval ===
// EXPECT: postfix "3 4 + 2 *" = 14
// EXPECT: postfix "5 1 2 + 4 * + 3 -" = 14

int printf(const char *fmt, ...);

struct Stack {
    int data[64];
    int top;
};

void stack_init(struct Stack *s) {
    s->top = -1;
}

int stack_empty(struct Stack *s) {
    return s->top == -1;
}

int stack_size(struct Stack *s) {
    return s->top + 1;
}

void stack_push(struct Stack *s, int val) {
    s->top = s->top + 1;
    s->data[s->top] = val;
}

int stack_pop(struct Stack *s) {
    int val = s->data[s->top];
    s->top = s->top - 1;
    return val;
}

int stack_peek(struct Stack *s) {
    return s->data[s->top];
}

void test_push_pop(void) {
    printf("=== Push & Pop ===\n");
    struct Stack s;
    stack_init(&s);
    stack_push(&s, 10);
    stack_push(&s, 20);
    stack_push(&s, 30);
    printf("pushed: 10 20 30\n");
    printf("pop: %d\n", stack_pop(&s));
    printf("pop: %d\n", stack_pop(&s));
    printf("pop: %d\n", stack_pop(&s));
}

void test_peek(void) {
    printf("=== Peek ===\n");
    struct Stack s;
    stack_init(&s);
    stack_push(&s, 40);
    stack_push(&s, 50);
    printf("peek: %d\n", stack_peek(&s));
    printf("peek again: %d\n", stack_peek(&s));
    printf("pop: %d\n", stack_pop(&s));
    printf("peek: %d\n", stack_peek(&s));
}

void test_empty(void) {
    printf("=== isEmpty ===\n");
    struct Stack s;
    stack_init(&s);
    if (stack_empty(&s)) printf("empty at start: yes\n");
    else printf("empty at start: no\n");
    stack_push(&s, 1);
    if (stack_empty(&s)) printf("after push: yes\n");
    else printf("after push: no\n");
    stack_pop(&s);
    if (stack_empty(&s)) printf("after pop: yes\n");
    else printf("after pop: no\n");
}

void test_size(void) {
    printf("=== Stack Size ===\n");
    struct Stack s;
    stack_init(&s);
    int i;
    for (i = 0; i < 5; i++) {
        stack_push(&s, i * 10);
    }
    printf("size after 5 pushes: %d\n", stack_size(&s));
    stack_pop(&s);
    stack_pop(&s);
    printf("size after 2 pops: %d\n", stack_size(&s));
}

void test_reverse_array(void) {
    printf("=== Reverse Array ===\n");
    int arr[5];
    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    printf("original:");
    int i;
    for (i = 0; i < 5; i++) printf(" %d", arr[i]);
    printf("\n");

    struct Stack s;
    stack_init(&s);
    for (i = 0; i < 5; i++) stack_push(&s, arr[i]);
    for (i = 0; i < 5; i++) arr[i] = stack_pop(&s);
    printf("reversed:");
    for (i = 0; i < 5; i++) printf(" %d", arr[i]);
    printf("\n");
}

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

int check_balanced(char *expr) {
    struct Stack s;
    stack_init(&s);
    int i;
    int len = str_len(expr);
    for (i = 0; i < len; i++) {
        if (expr[i] == '(') {
            stack_push(&s, 1);
        } else if (expr[i] == ')') {
            if (stack_empty(&s)) return 0;
            stack_pop(&s);
        }
    }
    return stack_empty(&s);
}

void test_balanced(void) {
    printf("=== Balanced Parens ===\n");
    if (check_balanced("(())")) printf("\"(())\" balanced: yes\n");
    else printf("\"(())\" balanced: no\n");
    if (check_balanced("(()")) printf("\"(()\" balanced: yes\n");
    else printf("\"(()\" balanced: no\n");
    if (check_balanced("")) printf("\"\" balanced: yes\n");
    else printf("\"\" balanced: no\n");
    if (check_balanced("()()")) printf("\"()()\" balanced: yes\n");
    else printf("\"()()\" balanced: no\n");
    if (check_balanced(")(")) printf("\")(\" balanced: yes\n");
    else printf("\")(\" balanced: no\n");
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int eval_postfix(char *expr) {
    struct Stack s;
    stack_init(&s);
    int i = 0;
    int len = str_len(expr);
    while (i < len) {
        if (expr[i] == ' ') {
            i = i + 1;
            continue;
        }
        if (is_digit(expr[i])) {
            int num = 0;
            while (i < len && is_digit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i = i + 1;
            }
            stack_push(&s, num);
        } else {
            int b = stack_pop(&s);
            int a = stack_pop(&s);
            if (expr[i] == '+') stack_push(&s, a + b);
            else if (expr[i] == '-') stack_push(&s, a - b);
            else if (expr[i] == '*') stack_push(&s, a * b);
            i = i + 1;
        }
    }
    return stack_pop(&s);
}

void test_eval(void) {
    printf("=== Expression Eval ===\n");
    // 3 4 + 2 * = (3+4)*2 = 14
    int r1 = eval_postfix("3 4 + 2 *");
    printf("postfix \"3 4 + 2 *\" = %d\n", r1);
    // 5 1 2 + 4 * + 3 - = 5 + (1+2)*4 - 3 = 5+12-3 = 14
    int r2 = eval_postfix("5 1 2 + 4 * + 3 -");
    printf("postfix \"5 1 2 + 4 * + 3 -\" = %d\n", r2);
}

int main(void) {
    test_push_pop();
    test_peek();
    test_empty();
    test_size();
    test_reverse_array();
    test_balanced();
    test_eval();
    return 0;
}
