int printf(const char *fmt, ...);

// Recursive descent expression parser
// Parses arithmetic expressions with +, -, *, /, parens
// Returns evaluated result directly during parsing

int TOK_NUM = 1;
int TOK_PLUS = 2;
int TOK_MINUS = 3;
int TOK_STAR = 4;
int TOK_SLASH = 5;
int TOK_LPAREN = 6;
int TOK_RPAREN = 7;
int TOK_EOF = 8;

struct Token {
    int type;
    int value;
};

struct Token tokens[128];
int token_count;
int current_pos;

int is_digit(int ch) {
    return ch >= 48 && ch <= 57;
}

void tokenize(char *input) {
    int pos = 0;
    int len = 0;
    char *p = input;
    while (p[len] != 0) len = len + 1;
    token_count = 0;

    while (pos < len) {
        int ch = input[pos];
        if (ch == 32 || ch == 9) { pos = pos + 1; continue; }
        if (is_digit(ch)) {
            int val = 0;
            while (pos < len && is_digit(input[pos])) {
                val = val * 10 + (input[pos] - 48);
                pos = pos + 1;
            }
            tokens[token_count].type = TOK_NUM;
            tokens[token_count].value = val;
            token_count = token_count + 1;
            continue;
        }
        if (ch == 43) { tokens[token_count].type = TOK_PLUS; tokens[token_count].value = 0; token_count = token_count + 1; }
        else if (ch == 45) { tokens[token_count].type = TOK_MINUS; tokens[token_count].value = 0; token_count = token_count + 1; }
        else if (ch == 42) { tokens[token_count].type = TOK_STAR; tokens[token_count].value = 0; token_count = token_count + 1; }
        else if (ch == 47) { tokens[token_count].type = TOK_SLASH; tokens[token_count].value = 0; token_count = token_count + 1; }
        else if (ch == 40) { tokens[token_count].type = TOK_LPAREN; tokens[token_count].value = 0; token_count = token_count + 1; }
        else if (ch == 41) { tokens[token_count].type = TOK_RPAREN; tokens[token_count].value = 0; token_count = token_count + 1; }
        pos = pos + 1;
    }
    tokens[token_count].type = TOK_EOF;
    tokens[token_count].value = 0;
}

struct Token peek() {
    return tokens[current_pos];
}

struct Token advance() {
    struct Token t = tokens[current_pos];
    if (t.type != TOK_EOF) current_pos = current_pos + 1;
    return t;
}

int expect(int type) {
    struct Token t = advance();
    if (t.type != type) {
        printf("parse error: expected %d got %d\n", type, t.type);
        return 0;
    }
    return 1;
}

int parse_expr();

int parse_primary() {
    struct Token t = peek();
    if (t.type == TOK_NUM) {
        advance();
        return t.value;
    }
    if (t.type == TOK_LPAREN) {
        advance();
        int val = parse_expr();
        expect(TOK_RPAREN);
        return val;
    }
    if (t.type == TOK_MINUS) {
        advance();
        return -parse_primary();
    }
    printf("unexpected token type %d\n", t.type);
    return 0;
}

int parse_mul() {
    int left = parse_primary();
    while (1) {
        struct Token t = peek();
        if (t.type == TOK_STAR) {
            advance();
            int right = parse_primary();
            left = left * right;
        } else if (t.type == TOK_SLASH) {
            advance();
            int right = parse_primary();
            if (right != 0) left = left / right;
        } else {
            break;
        }
    }
    return left;
}

int parse_expr() {
    int left = parse_mul();
    while (1) {
        struct Token t = peek();
        if (t.type == TOK_PLUS) {
            advance();
            int right = parse_mul();
            left = left + right;
        } else if (t.type == TOK_MINUS) {
            advance();
            int right = parse_mul();
            left = left - right;
        } else {
            break;
        }
    }
    return left;
}

int eval(char *expr) {
    tokenize(expr);
    current_pos = 0;
    return parse_expr();
}

int main() {
    int r;
    r = eval("42");
    printf("eval 42 = %d\n", r);
    // EXPECT: eval 42 = 42

    r = eval("2 + 3");
    printf("eval 2+3 = %d\n", r);
    // EXPECT: eval 2+3 = 5

    r = eval("10 - 4");
    printf("eval 10-4 = %d\n", r);
    // EXPECT: eval 10-4 = 6

    r = eval("3 * 7");
    printf("eval 3*7 = %d\n", r);
    // EXPECT: eval 3*7 = 21

    r = eval("20 / 4");
    printf("eval 20/4 = %d\n", r);
    // EXPECT: eval 20/4 = 5

    r = eval("2 + 3 * 4");
    printf("eval 2+3*4 = %d\n", r);
    // EXPECT: eval 2+3*4 = 14

    r = eval("(2 + 3) * 4");
    printf("eval (2+3)*4 = %d\n", r);
    // EXPECT: eval (2+3)*4 = 20

    r = eval("100 / (5 + 5)");
    printf("eval 100/(5+5) = %d\n", r);
    // EXPECT: eval 100/(5+5) = 10

    r = eval("(1 + 2) * (3 + 4)");
    printf("eval (1+2)*(3+4) = %d\n", r);
    // EXPECT: eval (1+2)*(3+4) = 21

    r = eval("10 - 2 - 3");
    printf("eval 10-2-3 = %d\n", r);
    // EXPECT: eval 10-2-3 = 5

    r = eval("2 * 3 + 4 * 5");
    printf("eval 2*3+4*5 = %d\n", r);
    // EXPECT: eval 2*3+4*5 = 26

    r = eval("-5");
    printf("eval -5 = %d\n", r);
    // EXPECT: eval -5 = -5

    r = eval("-(3 + 4)");
    printf("eval -(3+4) = %d\n", r);
    // EXPECT: eval -(3+4) = -7

    r = eval("((2))");
    printf("eval ((2)) = %d\n", r);
    // EXPECT: eval ((2)) = 2

    printf("parser done\n");
    // EXPECT: parser done

    return 0;
}
