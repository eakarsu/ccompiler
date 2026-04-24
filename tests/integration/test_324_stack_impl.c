int printf(const char *fmt, ...);
// EXPECT: empty: 1\nstack[3]: 10 20 30\npeek: 30\nsize: 3\npopped: 30\nstack[2]: 10 20\nstack[4]: 10 20 40 50\npop: 50\npop: 40\npop: 20\npop: 10\nempty: 1\nbalanced: 1
// Test: struct for stack implementation

struct Stack {
    int data[20];
    int top;
};

void stack_init(struct Stack *s) {
    s->top = -1;
}

int stack_empty(struct Stack *s) {
    return s->top == -1;
}

int stack_full(struct Stack *s) {
    return s->top == 19;
}

int stack_size(struct Stack *s) {
    return s->top + 1;
}

void stack_push(struct Stack *s, int val) {
    if (!stack_full(s)) {
        s->top = s->top + 1;
        s->data[s->top] = val;
    }
}

int stack_pop(struct Stack *s) {
    if (!stack_empty(s)) {
        int val = s->data[s->top];
        s->top = s->top - 1;
        return val;
    }
    return -1;
}

int stack_peek(struct Stack *s) {
    if (!stack_empty(s)) {
        return s->data[s->top];
    }
    return -1;
}

void stack_print(struct Stack *s) {
    printf("stack[%d]: ", stack_size(s));
    int i;
    for (i = 0; i <= s->top; i++) {
        if (i > 0) printf(" ");
        printf("%d", s->data[i]);
    }
    printf("\n");
}

int check_balanced(int opens[], int closes[], int n) {
    struct Stack s;
    stack_init(&s);
    int i;
    for (i = 0; i < n; i++) {
        if (opens[i]) {
            stack_push(&s, i);
        } else if (closes[i]) {
            if (stack_empty(&s)) return 0;
            stack_pop(&s);
        }
    }
    return stack_empty(&s);
}

int main(void) {
    struct Stack s;
    stack_init(&s);

    printf("empty: %d\n", stack_empty(&s));
    stack_push(&s, 10);
    stack_push(&s, 20);
    stack_push(&s, 30);
    stack_print(&s);
    printf("peek: %d\n", stack_peek(&s));
    printf("size: %d\n", stack_size(&s));

    int val = stack_pop(&s);
    printf("popped: %d\n", val);
    stack_print(&s);

    stack_push(&s, 40);
    stack_push(&s, 50);
    stack_print(&s);

    while (!stack_empty(&s)) {
        printf("pop: %d\n", stack_pop(&s));
    }
    printf("empty: %d\n", stack_empty(&s));

    int opens[6];
    int closes[6];
    opens[0]=1; closes[0]=0;
    opens[1]=1; closes[1]=0;
    opens[2]=0; closes[2]=1;
    opens[3]=0; closes[3]=1;
    opens[4]=1; closes[4]=0;
    opens[5]=0; closes[5]=1;
    printf("balanced: %d\n", check_balanced(opens, closes, 6));

    return 0;
}
