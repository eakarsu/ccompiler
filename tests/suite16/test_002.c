int printf(const char *fmt, ...);

// Infix expression parser with recursive descent
// Supports: +, -, *, /, parentheses, single-digit numbers

char expr_buf[256];
int expr_pos;

void expr_set(char *s) {
    int i;
    i = 0;
    while (s[i] != 0) {
        expr_buf[i] = s[i];
        i = i + 1;
    }
    expr_buf[i] = 0;
    expr_pos = 0;
}

int expr_peek(void) {
    return expr_buf[expr_pos];
}

int expr_next(void) {
    int ch;
    ch = expr_buf[expr_pos];
    expr_pos = expr_pos + 1;
    return ch;
}

void expr_skip_spaces(void) {
    while (expr_buf[expr_pos] == 32) {
        expr_pos = expr_pos + 1;
    }
}

int parse_expr(void);

int parse_number(void) {
    int result;
    int ch;
    result = 0;
    expr_skip_spaces();
    ch = expr_peek();
    while (ch >= 48 && ch <= 57) {
        result = result * 10 + (ch - 48);
        expr_next();
        ch = expr_peek();
    }
    return result;
}

// factor = number | '(' expr ')'
int parse_factor(void) {
    int result;
    expr_skip_spaces();
    if (expr_peek() == 40) {
        // '('
        expr_next();
        result = parse_expr();
        expr_skip_spaces();
        if (expr_peek() == 41) {
            // ')'
            expr_next();
        }
        return result;
    }
    return parse_number();
}

// term = factor (('*' | '/') factor)*
int parse_term(void) {
    int left;
    int op;
    int right;
    left = parse_factor();
    expr_skip_spaces();
    while (expr_peek() == 42 || expr_peek() == 47) {
        op = expr_next();
        right = parse_factor();
        if (op == 42) {
            left = left * right;
        } else {
            left = left / right;
        }
        expr_skip_spaces();
    }
    return left;
}

// expr = term (('+' | '-') term)*
int parse_expr(void) {
    int left;
    int op;
    int right;
    left = parse_term();
    expr_skip_spaces();
    while (expr_peek() == 43 || expr_peek() == 45) {
        op = expr_next();
        right = parse_term();
        if (op == 43) {
            left = left + right;
        } else {
            left = left - right;
        }
        expr_skip_spaces();
    }
    return left;
}

int evaluate(char *s) {
    expr_set(s);
    return parse_expr();
}

int main(void) {
    int r;

    r = evaluate("3+4");
    printf("3+4 = %d\n", r); // EXPECT: 3+4 = 7

    r = evaluate("10-3");
    printf("10-3 = %d\n", r); // EXPECT: 10-3 = 7

    r = evaluate("2*6");
    printf("2*6 = %d\n", r); // EXPECT: 2*6 = 12

    r = evaluate("20/4");
    printf("20/4 = %d\n", r); // EXPECT: 20/4 = 5

    // Precedence: 2+3*4 = 14
    r = evaluate("2+3*4");
    printf("2+3*4 = %d\n", r); // EXPECT: 2+3*4 = 14

    // Parentheses: (2+3)*4 = 20
    r = evaluate("(2+3)*4");
    printf("(2+3)*4 = %d\n", r); // EXPECT: (2+3)*4 = 20

    // Nested parens: ((1+2)*(3+4)) = 21
    r = evaluate("((1+2)*(3+4))");
    printf("((1+2)*(3+4)) = %d\n", r); // EXPECT: ((1+2)*(3+4)) = 21

    // Complex: 10+20*3-50/5 = 10+60-10 = 60
    r = evaluate("10+20*3-50/5");
    printf("10+20*3-50/5 = %d\n", r); // EXPECT: 10+20*3-50/5 = 60

    // More complex: (10+20)*(3-1)/(6-1) = 30*2/5 = 12
    r = evaluate("(10+20)*(3-1)/(6-1)");
    printf("(10+20)*(3-1)/(6-1) = %d\n", r); // EXPECT: (10+20)*(3-1)/(6-1) = 12

    // Deeply nested: (((5))) = 5
    r = evaluate("(((5)))");
    printf("(((5))) = %d\n", r); // EXPECT: (((5))) = 5

    // Long expression: 1+2+3+4+5+6+7+8+9 = 45
    r = evaluate("1+2+3+4+5+6+7+8+9");
    printf("1+2+...+9 = %d\n", r); // EXPECT: 1+2+...+9 = 45

    // Mixed: 100/(2*(3+2)) = 100/10 = 10
    r = evaluate("100/(2*(3+2))");
    printf("100/(2*(3+2)) = %d\n", r); // EXPECT: 100/(2*(3+2)) = 10

    printf("all infix tests passed\n"); // EXPECT: all infix tests passed
    return 0;
}
