int printf(const char *fmt, ...);

// Stack-based postfix expression evaluation

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

int stack_size(struct Stack *s) {
    return s->top + 1;
}

// Tokens for postfix expression
// Positive values are operand numbers
// Negative values are operators:
int OP_ADD = -1;
int OP_SUB = -2;
int OP_MUL = -3;
int OP_DIV = -4;
int OP_DUP = -5;    // duplicate top
int OP_SWAP = -6;   // swap top two
int OP_OVER = -7;   // copy second from top

int eval_postfix(int *tokens, int n) {
    struct Stack s;
    stack_init(&s);
    int i = 0;
    while (i < n) {
        int tok = tokens[i];
        if (tok >= 0) {
            stack_push(&s, tok);
        } else if (tok == -1) {
            int b = stack_pop(&s);
            int a = stack_pop(&s);
            stack_push(&s, a + b);
        } else if (tok == -2) {
            int b = stack_pop(&s);
            int a = stack_pop(&s);
            stack_push(&s, a - b);
        } else if (tok == -3) {
            int b = stack_pop(&s);
            int a = stack_pop(&s);
            stack_push(&s, a * b);
        } else if (tok == -4) {
            int b = stack_pop(&s);
            int a = stack_pop(&s);
            stack_push(&s, a / b);
        } else if (tok == -5) {
            int a = stack_peek(&s);
            stack_push(&s, a);
        } else if (tok == -6) {
            int b = stack_pop(&s);
            int a = stack_pop(&s);
            stack_push(&s, b);
            stack_push(&s, a);
        } else if (tok == -7) {
            int b = stack_pop(&s);
            int a = stack_peek(&s);
            stack_push(&s, b);
            stack_push(&s, a);
        }
        i = i + 1;
    }
    return stack_pop(&s);
}

// Convert infix (fully parenthesized, single digits) to postfix
// Input: string like "(3+(4*5))"
// Output: postfix token array, returns count
int infix_to_postfix(char *expr, int *output) {
    struct Stack ops;
    stack_init(&ops);
    int out_count = 0;
    int i = 0;
    while (expr[i] != 0) {
        char c = expr[i];
        if (c >= 48 && c <= 57) {
            // digit
            output[out_count] = c - 48;
            out_count = out_count + 1;
        } else if (c == 40) {
            // '('
            stack_push(&ops, c);
        } else if (c == 43) {
            // '+'
            stack_push(&ops, -1);
        } else if (c == 45) {
            // '-'
            stack_push(&ops, -2);
        } else if (c == 42) {
            // '*'
            stack_push(&ops, -3);
        } else if (c == 47) {
            // '/'
            stack_push(&ops, -4);
        } else if (c == 41) {
            // ')'
            while (!stack_empty(&ops) && stack_peek(&ops) != 40) {
                output[out_count] = stack_pop(&ops);
                out_count = out_count + 1;
            }
            if (!stack_empty(&ops)) stack_pop(&ops); // remove '('
        }
        i = i + 1;
    }
    while (!stack_empty(&ops)) {
        output[out_count] = stack_pop(&ops);
        out_count = out_count + 1;
    }
    return out_count;
}

int main() {
    // Test 1: 3 4 + => 7
    int t1[3];
    t1[0] = 3; t1[1] = 4; t1[2] = -1;
    printf("%d\n", eval_postfix(t1, 3));
    // EXPECT: 7

    // Test 2: 5 3 - => 2
    int t2[3];
    t2[0] = 5; t2[1] = 3; t2[2] = -2;
    printf("%d\n", eval_postfix(t2, 3));
    // EXPECT: 2

    // Test 3: 3 4 * 2 + => 14
    int t3[5];
    t3[0] = 3; t3[1] = 4; t3[2] = -3; t3[3] = 2; t3[4] = -1;
    printf("%d\n", eval_postfix(t3, 5));
    // EXPECT: 14

    // Test 4: 10 2 / 3 * => 15
    int t4[5];
    t4[0] = 10; t4[1] = 2; t4[2] = -4; t4[3] = 3; t4[4] = -3;
    printf("%d\n", eval_postfix(t4, 5));
    // EXPECT: 15

    // Test 5: 5 DUP * => 25 (5*5)
    int t5[3];
    t5[0] = 5; t5[1] = -5; t5[2] = -3;
    printf("%d\n", eval_postfix(t5, 3));
    // EXPECT: 25

    // Test 6: 3 7 SWAP - => 7-3 = 4
    int t6[4];
    t6[0] = 3; t6[1] = 7; t6[2] = -6; t6[3] = -2;
    printf("%d\n", eval_postfix(t6, 4));
    // EXPECT: 4

    // Test 7: 2 3 OVER => stack has [2,3,2], then + + => 7
    int t7[5];
    t7[0] = 2; t7[1] = 3; t7[2] = -7; t7[3] = -1; t7[4] = -1;
    printf("%d\n", eval_postfix(t7, 5));
    // EXPECT: 7

    // Test 8: Complex: 2 3 + 4 * 6 - => (2+3)*4-6 = 14
    int t8[7];
    t8[0] = 2; t8[1] = 3; t8[2] = -1; t8[3] = 4; t8[4] = -3; t8[5] = 6; t8[6] = -2;
    printf("%d\n", eval_postfix(t8, 7));
    // EXPECT: 14

    // Test infix to postfix conversion: "(3+(4*5))" => 3 4 5 * +
    char expr1[16];
    expr1[0] = '('; expr1[1] = '3'; expr1[2] = '+';
    expr1[3] = '('; expr1[4] = '4'; expr1[5] = '*';
    expr1[6] = '5'; expr1[7] = ')'; expr1[8] = ')';
    expr1[9] = 0;
    int postfix[32];
    int pcount = infix_to_postfix(expr1, postfix);
    printf("%d\n", pcount);
    // EXPECT: 5
    int result = eval_postfix(postfix, pcount);
    printf("%d\n", result);
    // EXPECT: 23

    // Infix: "((2+3)*(4-1))" => 2 3 + 4 1 - * => 15
    char expr2[20];
    expr2[0] = '('; expr2[1] = '('; expr2[2] = '2'; expr2[3] = '+';
    expr2[4] = '3'; expr2[5] = ')'; expr2[6] = '*'; expr2[7] = '(';
    expr2[8] = '4'; expr2[9] = '-'; expr2[10] = '1'; expr2[11] = ')';
    expr2[12] = ')'; expr2[13] = 0;
    pcount = infix_to_postfix(expr2, postfix);
    result = eval_postfix(postfix, pcount);
    printf("%d\n", result);
    // EXPECT: 15

    // Infix: "((9-3)/(3-1))" => 9 3 - 3 1 - / => 3
    char expr3[20];
    expr3[0] = '('; expr3[1] = '('; expr3[2] = '9'; expr3[3] = '-';
    expr3[4] = '3'; expr3[5] = ')'; expr3[6] = '/'; expr3[7] = '(';
    expr3[8] = '3'; expr3[9] = '-'; expr3[10] = '1'; expr3[11] = ')';
    expr3[12] = ')'; expr3[13] = 0;
    pcount = infix_to_postfix(expr3, postfix);
    result = eval_postfix(postfix, pcount);
    printf("%d\n", result);
    // EXPECT: 3

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
