// stress_083_recursive_descent.c - Recursive descent parser for math expressions
// EXPECT: eval_1=3
// EXPECT: eval_2=7
// EXPECT: eval_3=14
// EXPECT: eval_4=2
// EXPECT: eval_5=10
// EXPECT: eval_6=6
// EXPECT: eval_7=1
// EXPECT: eval_8=42
// EXPECT: eval_9=100
// EXPECT: eval_10=0
// EXPECT: eval_11=25
// EXPECT: eval_12=37
// EXPECT: eval_13=100
// EXPECT: eval_14=3
// EXPECT: eval_15=8
// EXPECT: pass_done=1

int printf(const char *fmt, ...);

// Global parser state
char *input;
int pos;

char peek(void) {
    return input[pos];
}

char advance(void) {
    char c = input[pos];
    if (c != 0) pos++;
    return c;
}

void skip_spaces(void) {
    while (input[pos] == ' ') pos++;
}

// Forward declarations
int parse_expr(void);
int parse_term(void);
int parse_factor(void);

// factor = number | '(' expr ')' | '-' factor
int parse_factor(void) {
    skip_spaces();
    char c = peek();

    if (c == '(') {
        advance(); // skip '('
        int result = parse_expr();
        skip_spaces();
        advance(); // skip ')'
        return result;
    }

    if (c == '-') {
        advance();
        return -parse_factor();
    }

    // Parse number
    int result = 0;
    while (peek() >= '0' && peek() <= '9') {
        result = result * 10 + (advance() - '0');
    }
    return result;
}

// term = factor (('*' | '/') factor)*
int parse_term(void) {
    int result = parse_factor();
    while (1) {
        skip_spaces();
        char c = peek();
        if (c == '*') {
            advance();
            result = result * parse_factor();
        } else if (c == '/') {
            advance();
            int divisor = parse_factor();
            if (divisor != 0) result = result / divisor;
        } else {
            break;
        }
    }
    return result;
}

// expr = term (('+' | '-') term)*
int parse_expr(void) {
    int result = parse_term();
    while (1) {
        skip_spaces();
        char c = peek();
        if (c == '+') {
            advance();
            result = result + parse_term();
        } else if (c == '-') {
            advance();
            result = result - parse_term();
        } else {
            break;
        }
    }
    return result;
}

int eval(char *expr_str) {
    input = expr_str;
    pos = 0;
    return parse_expr();
}

int main(void) {
    // Simple addition
    printf("eval_1=%d\n", eval("1+2"));

    // Addition and multiplication precedence
    printf("eval_2=%d\n", eval("1+2*3"));

    // Multiple operations
    printf("eval_3=%d\n", eval("2+3*4"));

    // Division
    printf("eval_4=%d\n", eval("10/5"));

    // Parentheses
    printf("eval_5=%d\n", eval("(2+3)*2"));

    // Nested parentheses
    printf("eval_6=%d\n", eval("((1+2))*(3-1)"));

    // Subtraction chain
    printf("eval_7=%d\n", eval("10-4-3-2"));

    // Complex expression
    printf("eval_8=%d\n", eval("(5+2)*(8-2)"));

    // Large number
    printf("eval_9=%d\n", eval("100"));

    // Expression equals zero
    printf("eval_10=%d\n", eval("5-3-2"));

    // Nested multiplication
    printf("eval_11=%d\n", eval("5*5"));

    // Long chain
    printf("eval_12=%d\n", eval("1+2+3+4+5+6+7+9"));

    // Mix everything
    printf("eval_13=%d\n", eval("10*(8+3)-2*5"));

    // Division truncation
    printf("eval_14=%d\n", eval("10/3"));

    // Negative number via subtraction and parens
    printf("eval_15=%d\n", eval("10+(-2)"));

    printf("pass_done=1\n");
    return 0;
}
