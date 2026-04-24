int printf(const char *fmt, ...);

/* Stack implementation via array */

struct Stack {
    int data[64];
    int top;
    int capacity;
};

void stack_init(struct Stack *s) {
    s->top = -1;
    s->capacity = 64;
    int i;
    for (i = 0; i < 64; i++) {
        s->data[i] = 0;
    }
}

int stack_is_empty(struct Stack *s) {
    return s->top == -1;
}

int stack_is_full(struct Stack *s) {
    return s->top == s->capacity - 1;
}

int stack_push(struct Stack *s, int val) {
    if (stack_is_full(s)) return 0;
    s->top = s->top + 1;
    s->data[s->top] = val;
    return 1;
}

int stack_pop(struct Stack *s) {
    int val;
    if (stack_is_empty(s)) return -999;
    val = s->data[s->top];
    s->top = s->top - 1;
    return val;
}

int stack_peek(struct Stack *s) {
    if (stack_is_empty(s)) return -999;
    return s->data[s->top];
}

int stack_size(struct Stack *s) {
    return s->top + 1;
}

void stack_print(struct Stack *s) {
    int i;
    printf("stack[%d]:", stack_size(s));
    for (i = 0; i <= s->top; i++) {
        printf(" %d", s->data[i]);
    }
    printf("\n");
}

/* Check if parentheses are balanced using stack */
int check_balanced(char *str) {
    struct Stack s;
    int i;
    char ch;
    stack_init(&s);

    for (i = 0; str[i] != 0; i++) {
        ch = str[i];
        if (ch == '(' || ch == '[' || ch == '{') {
            stack_push(&s, ch);
        } else if (ch == ')') {
            if (stack_is_empty(&s) || stack_pop(&s) != '(') return 0;
        } else if (ch == ']') {
            if (stack_is_empty(&s) || stack_pop(&s) != '[') return 0;
        } else if (ch == '}') {
            if (stack_is_empty(&s) || stack_pop(&s) != '{') return 0;
        }
    }
    return stack_is_empty(&s);
}

/* Reverse an array using a stack */
void reverse_array(int *arr, int n) {
    struct Stack s;
    int i;
    stack_init(&s);
    for (i = 0; i < n; i++) {
        stack_push(&s, arr[i]);
    }
    for (i = 0; i < n; i++) {
        arr[i] = stack_pop(&s);
    }
}

int main() {
    struct Stack s;
    int val;
    int arr[6];
    int i;
    int result;

    stack_init(&s);
    printf("empty=%d\n", stack_is_empty(&s));
    // EXPECT: empty=1

    stack_push(&s, 10);
    stack_push(&s, 20);
    stack_push(&s, 30);
    stack_push(&s, 40);

    // EXPECT: stack[4]: 10 20 30 40
    stack_print(&s);

    val = stack_peek(&s);
    printf("peek=%d\n", val);
    // EXPECT: peek=40

    val = stack_pop(&s);
    printf("pop=%d\n", val);
    // EXPECT: pop=40

    val = stack_pop(&s);
    printf("pop=%d\n", val);
    // EXPECT: pop=30

    // EXPECT: stack[2]: 10 20
    stack_print(&s);

    printf("empty=%d\n", stack_is_empty(&s));
    // EXPECT: empty=0

    /* Test balanced parentheses */
    result = check_balanced("({[]})");
    printf("balanced ({[]})=%d\n", result);
    // EXPECT: balanced ({[]})=1

    result = check_balanced("([)]");
    printf("balanced ([)]=%d\n", result);
    // EXPECT: balanced ([)]=0

    result = check_balanced("((()))");
    printf("balanced ((()))=%d\n", result);
    // EXPECT: balanced ((()))=1

    /* Test reverse array */
    arr[0] = 1; arr[1] = 2; arr[2] = 3;
    arr[3] = 4; arr[4] = 5; arr[5] = 6;
    reverse_array(arr, 6);
    printf("reversed:");
    for (i = 0; i < 6; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");
    // EXPECT: reversed: 6 5 4 3 2 1

    return 0;
}
