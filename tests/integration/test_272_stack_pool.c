int printf(const char *fmt, ...);
// EXPECT: empty? 1\nafter pushes: [10 20 30] (top=30)\nsize = 3\npeek = 30\npop = 30\npop = 20\nafter pops: [10] (top=10)\nreversed: 5 4 3 2 1 \npostfix 3 4 + 2 * = 14\n(()): 1\n((): 0\n(): 1\n)(: 0\n((())): 1
// Test: pointer-based stack using array pool

int stack[20];
int sp = -1;

void push(int val) {
    sp = sp + 1;
    stack[sp] = val;
}

int pop(void) {
    int val = stack[sp];
    sp = sp - 1;
    return val;
}

int peek(void) {
    return stack[sp];
}

int is_empty(void) {
    return sp == -1;
}

int size(void) {
    return sp + 1;
}

void print_stack(void) {
    printf("[");
    int i;
    for (i = 0; i <= sp; i++) {
        if (i > 0) printf(" ");
        printf("%d", stack[i]);
    }
    printf("] (top=%d)\n", sp >= 0 ? stack[sp] : -1);
}

// Check balanced parentheses using stack
int check_parens(char *s) {
    int saved_sp = sp;
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] == '(') {
            push(s[i]);
        } else if (s[i] == ')') {
            if (is_empty()) {
                sp = saved_sp;
                return 0;
            }
            pop();
        }
        i = i + 1;
    }
    int result = is_empty();
    sp = saved_sp;
    return result;
}

int main(void) {
    // Basic stack operations
    printf("empty? %d\n", is_empty());
    push(10);
    push(20);
    push(30);
    printf("after pushes: ");
    print_stack();
    printf("size = %d\n", size());

    printf("peek = %d\n", peek());
    printf("pop = %d\n", pop());
    printf("pop = %d\n", pop());
    printf("after pops: ");
    print_stack();

    // Use stack to reverse a sequence
    sp = -1; // reset
    int i;
    for (i = 1; i <= 5; i++) push(i);
    printf("reversed: ");
    while (!is_empty()) {
        printf("%d ", pop());
    }
    printf("\n");

    // Evaluate postfix: 3 4 + 2 * = 14
    sp = -1;
    push(3);
    push(4);
    int a = pop(); int b = pop();
    push(a + b);
    push(2);
    a = pop(); b = pop();
    push(a * b);
    printf("postfix 3 4 + 2 * = %d\n", pop());

    // Balanced parens
    sp = -1;
    printf("(()): %d\n", check_parens("(())"));
    printf("((): %d\n", check_parens("(()"));
    printf("(): %d\n", check_parens("()"));
    printf(")(: %d\n", check_parens(")("));
    printf("((())): %d\n", check_parens("((()))"));

    return 0;
}
