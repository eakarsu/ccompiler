int printf(const char *fmt, ...);

/* Stack implementation using a fixed-size array */

#define STACK_MAX 16

typedef struct {
    int data[STACK_MAX];
    int top;
} Stack;

void stack_init(Stack *s) {
    s->top = -1;
}

int stack_is_empty(Stack *s) {
    return s->top == -1;
}

int stack_is_full(Stack *s) {
    return s->top == STACK_MAX - 1;
}

int stack_push(Stack *s, int val) {
    if (stack_is_full(s)) return 0;
    s->top++;
    s->data[s->top] = val;
    return 1;
}

int stack_pop(Stack *s, int *out) {
    if (stack_is_empty(s)) return 0;
    *out = s->data[s->top];
    s->top--;
    return 1;
}

int stack_peek(Stack *s, int *out) {
    if (stack_is_empty(s)) return 0;
    *out = s->data[s->top];
    return 1;
}

int stack_size(Stack *s) {
    return s->top + 1;
}

/* Bracket matching using the stack */
int brackets_matched(const char *str, int len) {
    Stack s;
    stack_init(&s);
    int i;
    for (i = 0; i < len; i++) {
        char c = str[i];
        if (c == '(' || c == '[' || c == '{') {
            stack_push(&s, (int)c);
        } else if (c == ')' || c == ']' || c == '}') {
            int top_val;
            if (!stack_peek(&s, &top_val)) return 0;
            if (c == ')' && top_val != '(') return 0;
            if (c == ']' && top_val != '[') return 0;
            if (c == '}' && top_val != '{') return 0;
            stack_pop(&s, &top_val);
        }
    }
    return stack_is_empty(&s);
}

/* Evaluate a simple RPN (Reverse Polish Notation) expression */
/* Tokens: positive single digits and operators +, -, * */
int rpn_eval(const char *tokens, int len) {
    Stack s;
    stack_init(&s);
    int i;
    for (i = 0; i < len; i++) {
        char c = tokens[i];
        if (c >= '0' && c <= '9') {
            stack_push(&s, c - '0');
        } else if (c == '+' || c == '-' || c == '*') {
            int b, a;
            stack_pop(&s, &b);
            stack_pop(&s, &a);
            int result;
            if (c == '+') result = a + b;
            else if (c == '-') result = a - b;
            else result = a * b;
            stack_push(&s, result);
        }
    }
    int out;
    stack_pop(&s, &out);
    return out;
}

int main(void) {
    Stack s;
    stack_init(&s);

    /* Basic push/pop/peek */
    printf("%d\n", stack_is_empty(&s));   /* 1 */
    // EXPECT: 1

    stack_push(&s, 10);
    stack_push(&s, 20);
    stack_push(&s, 30);

    printf("%d\n", stack_size(&s));       /* 3 */
    // EXPECT: 3

    int val;
    stack_peek(&s, &val);
    printf("%d\n", val);                  /* 30 */
    // EXPECT: 30

    stack_pop(&s, &val);
    printf("%d\n", val);                  /* 30 */
    // EXPECT: 30

    stack_pop(&s, &val);
    printf("%d\n", val);                  /* 20 */
    // EXPECT: 20

    printf("%d\n", stack_size(&s));       /* 1 */
    // EXPECT: 1

    stack_pop(&s, &val);
    printf("%d\n", stack_is_empty(&s));   /* 1 */
    // EXPECT: 1

    /* Push until full */
    int i;
    for (i = 0; i < STACK_MAX; i++) {
        stack_push(&s, i * 3);
    }
    printf("%d\n", stack_is_full(&s));    /* 1 */
    // EXPECT: 1

    /* Push onto full stack returns 0 */
    int pushed = stack_push(&s, 999);
    printf("%d\n", pushed);               /* 0 */
    // EXPECT: 0

    /* Peek at top = (STACK_MAX-1)*3 = 15*3 = 45 */
    stack_peek(&s, &val);
    printf("%d\n", val);                  /* 45 */
    // EXPECT: 45

    /* Pop all and sum */
    int sum = 0;
    while (!stack_is_empty(&s)) {
        stack_pop(&s, &val);
        sum += val;
    }
    /* sum = 0+3+6+9+12+15+18+21+24+27+30+33+36+39+42+45 = 360 */
    printf("%d\n", sum);                  /* 360 */
    // EXPECT: 360

    /* Bracket matching */
    /* "(())" -> matched */
    const char *b1 = "(())";
    printf("%d\n", brackets_matched(b1, 4)); /* 1 */
    // EXPECT: 1

    /* "([{}])" -> matched */
    const char *b2 = "([{}])";
    printf("%d\n", brackets_matched(b2, 6)); /* 1 */
    // EXPECT: 1

    /* "([)]" -> not matched */
    const char *b3 = "([)]";
    printf("%d\n", brackets_matched(b3, 4)); /* 0 */
    // EXPECT: 0

    /* "((" -> not matched (unclosed) */
    const char *b4 = "((";
    printf("%d\n", brackets_matched(b4, 2)); /* 0 */
    // EXPECT: 0

    /* RPN: "3 4 + 2 *" = (3+4)*2 = 14; tokens = "34+2*" */
    int r1 = rpn_eval("34+2*", 5);
    printf("%d\n", r1);                      /* 14 */
    // EXPECT: 14

    /* RPN: "52-3*" = (5-2)*3 = 9 */
    int r2 = rpn_eval("52-3*", 5);
    printf("%d\n", r2);                      /* 9 */
    // EXPECT: 9

    /* RPN: "23*45*+" = (2*3)+(4*5) = 6+20 = 26 */
    int r3 = rpn_eval("23*45*+", 7);
    printf("%d\n", r3);                      /* 26 */
    // EXPECT: 26

    return 0;
}
