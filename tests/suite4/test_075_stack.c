int printf(const char *fmt, ...);

struct Stack {
    int data[32];
    int top;
};

void stack_init(struct Stack *s) {
    s->top = -1;
}

int stack_is_empty(struct Stack *s) {
    return s->top == -1;
}

int stack_is_full(struct Stack *s) {
    return s->top == 31;
}

int stack_size(struct Stack *s) {
    return s->top + 1;
}

void stack_push(struct Stack *s, int val) {
    if (s->top < 31) {
        s->top = s->top + 1;
        s->data[s->top] = val;
    }
}

int stack_pop(struct Stack *s) {
    int val;
    if (s->top >= 0) {
        val = s->data[s->top];
        s->top = s->top - 1;
        return val;
    }
    return -1;
}

int stack_peek(struct Stack *s) {
    if (s->top >= 0) {
        return s->data[s->top];
    }
    return -1;
}

void stack_print(struct Stack *s) {
    int i;
    for (i = 0; i <= s->top; i = i + 1) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", s->data[i]);
    }
}

int check_balanced(int *parens, int n) {
    struct Stack s;
    int i;
    int val;
    stack_init(&s);
    for (i = 0; i < n; i = i + 1) {
        if (parens[i] == 1) {
            stack_push(&s, 1);
        } else {
            if (stack_is_empty(&s)) {
                return 0;
            }
            stack_pop(&s);
        }
    }
    return stack_is_empty(&s);
}

void reverse_array(int *arr, int n) {
    struct Stack s;
    int i;
    stack_init(&s);
    for (i = 0; i < n; i = i + 1) {
        stack_push(&s, arr[i]);
    }
    for (i = 0; i < n; i = i + 1) {
        arr[i] = stack_pop(&s);
    }
}

int main(void) {
    struct Stack s;
    int val;
    int arr[5];
    int parens[6];
    int i;

    stack_init(&s);

    // EXPECT: empty: 1
    printf("empty: %d\n", stack_is_empty(&s));
    // EXPECT: size: 0
    printf("size: %d\n", stack_size(&s));

    stack_push(&s, 10);
    stack_push(&s, 20);
    stack_push(&s, 30);

    // EXPECT: empty: 0
    printf("empty: %d\n", stack_is_empty(&s));
    // EXPECT: size: 3
    printf("size: %d\n", stack_size(&s));
    // EXPECT: peek: 30
    printf("peek: %d\n", stack_peek(&s));

    // EXPECT: stack: 10 20 30
    printf("stack: ");
    stack_print(&s);
    printf("\n");

    val = stack_pop(&s);
    // EXPECT: popped: 30
    printf("popped: %d\n", val);
    // EXPECT: peek: 20
    printf("peek: %d\n", stack_peek(&s));
    // EXPECT: size: 2
    printf("size: %d\n", stack_size(&s));

    val = stack_pop(&s);
    // EXPECT: popped: 20
    printf("popped: %d\n", val);

    val = stack_pop(&s);
    // EXPECT: popped: 10
    printf("popped: %d\n", val);

    // EXPECT: empty: 1
    printf("empty: %d\n", stack_is_empty(&s));

    stack_push(&s, 1);
    stack_push(&s, 2);
    stack_push(&s, 3);
    stack_push(&s, 4);
    stack_push(&s, 5);
    // EXPECT: five: 1 2 3 4 5
    printf("five: ");
    stack_print(&s);
    printf("\n");

    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50;
    reverse_array(arr, 5);
    // EXPECT: reversed: 50 40 30 20 10
    printf("reversed: ");
    for (i = 0; i < 5; i = i + 1) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", arr[i]);
    }
    printf("\n");

    parens[0] = 1; parens[1] = 1; parens[2] = 0; parens[3] = 0;
    // EXPECT: balanced (()): 1
    printf("balanced (()): %d\n", check_balanced(parens, 4));

    parens[0] = 1; parens[1] = 0; parens[2] = 0;
    // EXPECT: balanced ()): 0
    printf("balanced ()): %d\n", check_balanced(parens, 3));

    parens[0] = 1; parens[1] = 1; parens[2] = 0;
    // EXPECT: balanced ((: 0
    printf("balanced ((: %d\n", check_balanced(parens, 3));

    // EXPECT: done
    printf("done\n");

    return 0;
}
