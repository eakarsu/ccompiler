int printf(const char *fmt, ...);
// EXPECT: 1 + 2 = 3\n3 * 4 + 5 = 17\n3 + 4 * 5 = 23\n(3 + 4) * 5 = 35\n10 / 2 + 3 = 8\n(10 + 20) / (2 + 3) = 6\n2 * (3 + 4) * (5 - 2) = 42\n100 - 50 * 2 = 0\n((2 + 3) * (4 + 5)) = 45\n1 + 2 + 3 + 4 + 5 = 15\n2 * 3 * 4 = 24\n100 / 10 / 2 = 5
// Test: Expression parser (recursive descent)

char expr_buf[256];
int expr_pos;

int parse_expr(void);
int parse_term(void);
int parse_factor(void);

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

char peek(void) {
    return expr_buf[expr_pos];
}

char advance(void) {
    char c = expr_buf[expr_pos];
    expr_pos++;
    return c;
}

void skip_spaces(void) {
    while (expr_buf[expr_pos] == ' ') expr_pos++;
}

int parse_number(void) {
    skip_spaces();
    int neg = 0;
    if (peek() == '-') {
        neg = 1;
        advance();
    }
    int num = 0;
    while (is_digit(peek())) {
        num = num * 10 + (advance() - '0');
    }
    skip_spaces();
    if (neg) return -num;
    return num;
}

int parse_factor(void) {
    skip_spaces();
    if (peek() == '(') {
        advance();
        int val = parse_expr();
        skip_spaces();
        if (peek() == ')') advance();
        return val;
    }
    return parse_number();
}

int parse_term(void) {
    int val = parse_factor();
    skip_spaces();
    while (peek() == '*' || peek() == '/') {
        char op = advance();
        int right = parse_factor();
        if (op == '*') val = val * right;
        else val = val / right;
        skip_spaces();
    }
    return val;
}

int parse_expr(void) {
    int val = parse_term();
    skip_spaces();
    while (peek() == '+' || peek() == '-') {
        char op = advance();
        int right = parse_term();
        if (op == '+') val = val + right;
        else val = val - right;
        skip_spaces();
    }
    return val;
}

int evaluate(char *s) {
    int i = 0;
    while (s[i] != 0) {
        expr_buf[i] = s[i];
        i++;
    }
    expr_buf[i] = 0;
    expr_pos = 0;
    return parse_expr();
}

int main(void) {
    printf("1 + 2 = %d\n", evaluate("1 + 2"));
    printf("3 * 4 + 5 = %d\n", evaluate("3 * 4 + 5"));
    printf("3 + 4 * 5 = %d\n", evaluate("3 + 4 * 5"));
    printf("(3 + 4) * 5 = %d\n", evaluate("(3 + 4) * 5"));
    printf("10 / 2 + 3 = %d\n", evaluate("10 / 2 + 3"));
    printf("(10 + 20) / (2 + 3) = %d\n", evaluate("(10 + 20) / (2 + 3)"));
    printf("2 * (3 + 4) * (5 - 2) = %d\n", evaluate("2 * (3 + 4) * (5 - 2)"));
    printf("100 - 50 * 2 = %d\n", evaluate("100 - 50 * 2"));
    printf("((2 + 3) * (4 + 5)) = %d\n", evaluate("((2 + 3) * (4 + 5))"));
    printf("1 + 2 + 3 + 4 + 5 = %d\n", evaluate("1 + 2 + 3 + 4 + 5"));
    printf("2 * 3 * 4 = %d\n", evaluate("2 * 3 * 4"));
    printf("100 / 10 / 2 = %d\n", evaluate("100 / 10 / 2"));

    return 0;
}
