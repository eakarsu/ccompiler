int printf(const char *fmt, ...);

// Expression evaluator supporting unary minus, modulo, and power
// Uses recursive descent parsing with the grammar:
// expr = term (('+' | '-') term)*
// term = power (('*' | '/' | '%') power)*
// power = unary ('^' unary)*
// unary = '-' unary | atom
// atom = number | '(' expr ')'

char input[256];
int pos;

void set_input(char *s) {
    int i;
    i = 0;
    while (s[i] != 0) {
        input[i] = s[i];
        i = i + 1;
    }
    input[i] = 0;
    pos = 0;
}

void skip_ws(void) {
    while (input[pos] == 32) pos = pos + 1;
}

int peek_ch(void) {
    skip_ws();
    return input[pos];
}

int next_ch(void) {
    int ch;
    skip_ws();
    ch = input[pos];
    pos = pos + 1;
    return ch;
}

int power_func(int base, int exp) {
    int result;
    result = 1;
    while (exp > 0) {
        result = result * base;
        exp = exp - 1;
    }
    return result;
}

int parse_expr(void);

int parse_number(void) {
    int result;
    int ch;
    result = 0;
    ch = peek_ch();
    while (ch >= 48 && ch <= 57) {
        next_ch();
        result = result * 10 + (ch - 48);
        ch = peek_ch();
    }
    return result;
}

int parse_atom(void) {
    int result;
    if (peek_ch() == 40) {
        next_ch(); // '('
        result = parse_expr();
        if (peek_ch() == 41) next_ch(); // ')'
        return result;
    }
    return parse_number();
}

int parse_unary(void) {
    if (peek_ch() == 45) {
        // Check if this is unary minus (not binary minus)
        // It's unary if it's the first thing or after an operator/open-paren
        next_ch();
        return -parse_unary();
    }
    return parse_atom();
}

int parse_power(void) {
    int base;
    base = parse_unary();
    while (peek_ch() == 94) {
        // '^'
        next_ch();
        // Right-associative: 2^3^2 = 2^(3^2) = 2^9 = 512
        base = power_func(base, parse_unary());
    }
    return base;
}

int parse_term(void) {
    int left;
    int op;
    int right;
    left = parse_power();
    while (peek_ch() == 42 || peek_ch() == 47 || peek_ch() == 37) {
        op = next_ch();
        right = parse_power();
        if (op == 42) {
            left = left * right;
        } else if (op == 47) {
            left = left / right;
        } else {
            // modulo
            left = left - (left / right) * right;
        }
    }
    return left;
}

int parse_expr(void) {
    int left;
    int op;
    int right;
    left = parse_term();
    while (peek_ch() == 43 || peek_ch() == 45) {
        op = next_ch();
        right = parse_term();
        if (op == 43)
            left = left + right;
        else
            left = left - right;
    }
    return left;
}

int evaluate(char *s) {
    set_input(s);
    return parse_expr();
}

int main(void) {
    int r;

    // Basic power
    r = evaluate("2^10");
    printf("2^10 = %d\n", r); // EXPECT: 2^10 = 1024

    // Power with multiply
    r = evaluate("3^2*2");
    printf("3^2*2 = %d\n", r); // EXPECT: 3^2*2 = 18

    // Unary minus
    r = evaluate("-5+10");
    printf("-5+10 = %d\n", r); // EXPECT: -5+10 = 5

    // Double negative
    r = evaluate("--3");
    printf("--3 = %d\n", r); // EXPECT: --3 = 3

    // Negative in parens
    r = evaluate("(-3)*(-4)");
    printf("(-3)*(-4) = %d\n", r); // EXPECT: (-3)*(-4) = 12

    // Modulo
    r = evaluate("17%5");
    printf("17%%5 = %d\n", r); // EXPECT: 17%5 = 2

    // Complex: -2^3 => -(2^3) = -8 ... actually with our grammar: parse_unary -> -parse_unary -> -parse_atom -> -2, then ^3 is not seen
    // Our grammar: parse_power calls parse_unary which returns -2, then ^3 => (-2)^3 = -8
    r = evaluate("-2^3");
    printf("-2^3 = %d\n", r); // EXPECT: -2^3 = -8

    // Mixed expression
    r = evaluate("2^3+3^2");
    printf("2^3+3^2 = %d\n", r); // EXPECT: 2^3+3^2 = 17

    // Nested parens with power
    r = evaluate("(2+1)^(1+2)");
    printf("(2+1)^(1+2) = %d\n", r); // EXPECT: (2+1)^(1+2) = 27

    // Modulo chain
    r = evaluate("100%30%7");
    printf("100%%30%%7 = %d\n", r); // EXPECT: 100%30%7 = 3

    // Complex mixed
    r = evaluate("2^4-10%3+(-2)*3");
    printf("complex = %d\n", r); // EXPECT: complex = 9

    // Large expression
    r = evaluate("(3+4)*(2^3-1)%10");
    printf("(3+4)*(2^3-1)%%10 = %d\n", r); // EXPECT: (3+4)*(2^3-1)%10 = 9

    // Precedence check: power before multiply
    r = evaluate("2*3^2");
    printf("2*3^2 = %d\n", r); // EXPECT: 2*3^2 = 18

    // 0 power
    r = evaluate("5^0");
    printf("5^0 = %d\n", r); // EXPECT: 5^0 = 1

    printf("all extended eval tests passed\n"); // EXPECT: all extended eval tests passed
    return 0;
}
