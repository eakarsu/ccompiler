int printf(const char *fmt, ...);
// EXPECT: 3+4=7\n5-3=2\n2*3+4=10\n10/2*3=15\n5+(1+2)*4-3=14\n=== power ===\n2^0=1\n2^1=2\n2^10=1024\n3^5=243\n5^3=125\n2*3+4=10
// Test 219: Recursive expression evaluator

// Simple stack-based expression evaluator for postfix (RPN)
int stack[100];
int sp;

void push(int val) {
    stack[sp] = val;
    sp = sp + 1;
}

int pop(void) {
    sp = sp - 1;
    return stack[sp];
}

int peek(void) {
    return stack[sp - 1];
}

int eval_rpn(int *tokens, int n) {
    // tokens: positive = number, -1=add, -2=sub, -3=mul, -4=div
    sp = 0;
    int i = 0;
    while (i < n) {
        if (tokens[i] >= 0) {
            push(tokens[i]);
        } else {
            int b = pop();
            int a = pop();
            if (tokens[i] == -1) push(a + b);
            else if (tokens[i] == -2) push(a - b);
            else if (tokens[i] == -3) push(a * b);
            else if (tokens[i] == -4) {
                if (b != 0) push(a / b);
                else push(0);
            }
        }
        i = i + 1;
    }
    return pop();
}

// Recursive descent parser for simple expressions: num (+|-|*) num ...
int expr_str[100];
int expr_len;
int pos;

int parse_number(void) {
    int result = 0;
    while (pos < expr_len && expr_str[pos] >= 0) {
        result = result * 10 + expr_str[pos];
        pos = pos + 1;
    }
    return result;
}

int parse_expr(void);

int parse_factor(void) {
    return parse_number();
}

int parse_term(void) {
    int left = parse_factor();
    while (pos < expr_len && (expr_str[pos] == -3 || expr_str[pos] == -4)) {
        int op = expr_str[pos];
        pos = pos + 1;
        int right = parse_factor();
        if (op == -3) left = left * right;
        else if (right != 0) left = left / right;
    }
    return left;
}

int parse_expr(void) {
    int left = parse_term();
    while (pos < expr_len && (expr_str[pos] == -1 || expr_str[pos] == -2)) {
        int op = expr_str[pos];
        pos = pos + 1;
        int right = parse_term();
        if (op == -1) left = left + right;
        else left = left - right;
    }
    return left;
}

int power(int base, int exp) {
    if (exp == 0) return 1;
    if (exp == 1) return base;
    int half = power(base, exp / 2);
    if ((exp / 2) * 2 == exp)
        return half * half;
    return half * half * base;
}

int main(void) {
    // 3 4 + => 7
    int t1[3]; t1[0] = 3; t1[1] = 4; t1[2] = -1;
    printf("3+4=%d\n", eval_rpn(t1, 3));

    // 5 3 - => 2
    int t2[3]; t2[0] = 5; t2[1] = 3; t2[2] = -2;
    printf("5-3=%d\n", eval_rpn(t2, 3));

    // 2 3 * 4 + => 10
    int t3[5]; t3[0] = 2; t3[1] = 3; t3[2] = -3; t3[3] = 4; t3[4] = -1;
    printf("2*3+4=%d\n", eval_rpn(t3, 5));

    // 10 2 / 3 * => 15
    int t4[5]; t4[0] = 10; t4[1] = 2; t4[2] = -4; t4[3] = 3; t4[4] = -3;
    printf("10/2*3=%d\n", eval_rpn(t4, 5));

    // 5 1 2 + 4 * + 3 - => 5 + (1+2)*4 - 3 = 14
    int t5[9]; t5[0]=5; t5[1]=1; t5[2]=2; t5[3]=-1; t5[4]=4; t5[5]=-3; t5[6]=-1; t5[7]=3; t5[8]=-2;
    printf("5+(1+2)*4-3=%d\n", eval_rpn(t5, 9));

    printf("=== power ===\n");
    printf("2^0=%d\n", power(2, 0));
    printf("2^1=%d\n", power(2, 1));
    printf("2^10=%d\n", power(2, 10));
    printf("3^5=%d\n", power(3, 5));
    printf("5^3=%d\n", power(5, 3));

    // Test recursive descent: 2*3+4 encoded as [2,-3,3,-1,4]
    expr_str[0] = 2; expr_str[1] = -3; expr_str[2] = 3;
    expr_str[3] = -1; expr_str[4] = 4;
    expr_len = 5;
    pos = 0;
    printf("2*3+4=%d\n", parse_expr());

    return 0;
}
