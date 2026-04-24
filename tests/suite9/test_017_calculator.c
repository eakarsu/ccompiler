int printf(const char *fmt, ...);

// Recursive descent calculator for arithmetic expressions
// Grammar: expr = term (('+' | '-') term)*
//          term = factor (('*' | '/') factor)*
//          factor = number | '(' expr ')' | '-' factor

char input[128];
int pos;

int peek(void) {
    return input[pos];
}

int get_char(void) {
    int ch;
    ch = input[pos];
    if (ch != 0) pos = pos + 1;
    return ch;
}

void skip_spaces(void) {
    while (input[pos] == ' ') {
        pos = pos + 1;
    }
}

int is_digit(int ch) {
    return ch >= 48 && ch <= 57;
}

int parse_number(void) {
    int result;
    int neg;
    result = 0;
    skip_spaces();
    while (is_digit(input[pos])) {
        result = result * 10 + (input[pos] - 48);
        pos = pos + 1;
    }
    return result;
}

int parse_expr(void);

int parse_factor(void) {
    int result;
    skip_spaces();
    if (input[pos] == 45) { // '-'
        pos = pos + 1;
        result = -parse_factor();
        return result;
    }
    if (input[pos] == 40) { // '('
        pos = pos + 1;
        result = parse_expr();
        skip_spaces();
        if (input[pos] == 41) { // ')'
            pos = pos + 1;
        }
        return result;
    }
    return parse_number();
}

int parse_term(void) {
    int result;
    int op;
    int right;
    result = parse_factor();
    while (1) {
        skip_spaces();
        op = input[pos];
        if (op == 42) { // '*'
            pos = pos + 1;
            right = parse_factor();
            result = result * right;
        } else if (op == 47) { // '/'
            pos = pos + 1;
            right = parse_factor();
            if (right != 0) {
                result = result / right;
            }
        } else {
            break;
        }
    }
    return result;
}

int parse_expr(void) {
    int result;
    int op;
    int right;
    result = parse_term();
    while (1) {
        skip_spaces();
        op = input[pos];
        if (op == 43) { // '+'
            pos = pos + 1;
            right = parse_term();
            result = result + right;
        } else if (op == 45) { // '-'
            pos = pos + 1;
            right = parse_term();
            result = result - right;
        } else {
            break;
        }
    }
    return result;
}

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

int eval(char *s) {
    set_input(s);
    return parse_expr();
}

int main() {
    int r;

    r = eval("42");
    // EXPECT: 42 = 42
    printf("42 = %d\n", r);

    r = eval("2 + 3");
    // EXPECT: 2 + 3 = 5
    printf("2 + 3 = %d\n", r);

    r = eval("10 - 7");
    // EXPECT: 10 - 7 = 3
    printf("10 - 7 = %d\n", r);

    r = eval("3 * 4");
    // EXPECT: 3 * 4 = 12
    printf("3 * 4 = %d\n", r);

    r = eval("15 / 3");
    // EXPECT: 15 / 3 = 5
    printf("15 / 3 = %d\n", r);

    r = eval("2 + 3 * 4");
    // EXPECT: 2 + 3 * 4 = 14
    printf("2 + 3 * 4 = %d\n", r);

    r = eval("(2 + 3) * 4");
    // EXPECT: (2 + 3) * 4 = 20
    printf("(2 + 3) * 4 = %d\n", r);

    r = eval("10 - 2 * 3");
    // EXPECT: 10 - 2 * 3 = 4
    printf("10 - 2 * 3 = %d\n", r);

    r = eval("(10 - 2) * 3");
    // EXPECT: (10 - 2) * 3 = 24
    printf("(10 - 2) * 3 = %d\n", r);

    r = eval("100 / 2 / 5");
    // EXPECT: 100 / 2 / 5 = 10
    printf("100 / 2 / 5 = %d\n", r);

    r = eval("((3 + 2) * (8 - 3))");
    // EXPECT: ((3 + 2) * (8 - 3)) = 25
    printf("((3 + 2) * (8 - 3)) = %d\n", r);

    r = eval("-5 + 10");
    // EXPECT: -5 + 10 = 5
    printf("-5 + 10 = %d\n", r);

    r = eval("-(3 + 4)");
    // EXPECT: -(3 + 4) = -7
    printf("-(3 + 4) = %d\n", r);

    r = eval("1 + 2 + 3 + 4 + 5");
    // EXPECT: 1+2+3+4+5 = 15
    printf("1+2+3+4+5 = %d\n", r);

    // EXPECT: Calculator done
    printf("Calculator done\n");

    return 0;
}
