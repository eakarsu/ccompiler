int printf(const char *fmt, ...);

int TOK_NUM = 0;
int TOK_PLUS = 1;
int TOK_MINUS = 2;
int TOK_MUL = 3;
int TOK_DIV = 4;
int TOK_LPAREN = 5;
int TOK_RPAREN = 6;
int TOK_END = 7;

int tok_types[64];
int tok_values[64];
int tok_count;

int is_digit(int c) {
    return c >= 48 && c <= 57;
}

int is_space(int c) {
    return c == 32 || c == 9;
}

void add_token(int type, int value) {
    tok_types[tok_count] = type;
    tok_values[tok_count] = value;
    tok_count = tok_count + 1;
}

int tokenize(char *input) {
    tok_count = 0;
    int i = 0;
    while (input[i] != 0) {
        if (is_space(input[i])) {
            i = i + 1;
            continue;
        }
        if (is_digit(input[i])) {
            int num = 0;
            while (input[i] != 0 && is_digit(input[i])) {
                num = num * 10 + (input[i] - 48);
                i = i + 1;
            }
            add_token(TOK_NUM, num);
            continue;
        }
        if (input[i] == 43) { add_token(TOK_PLUS, 0); i = i + 1; continue; }
        if (input[i] == 45) { add_token(TOK_MINUS, 0); i = i + 1; continue; }
        if (input[i] == 42) { add_token(TOK_MUL, 0); i = i + 1; continue; }
        if (input[i] == 47) { add_token(TOK_DIV, 0); i = i + 1; continue; }
        if (input[i] == 40) { add_token(TOK_LPAREN, 0); i = i + 1; continue; }
        if (input[i] == 41) { add_token(TOK_RPAREN, 0); i = i + 1; continue; }
        return -1;
    }
    add_token(TOK_END, 0);
    return tok_count;
}

char *tok_name(int type) {
    if (type == 0) return "NUM";
    if (type == 1) return "PLUS";
    if (type == 2) return "MINUS";
    if (type == 3) return "MUL";
    if (type == 4) return "DIV";
    if (type == 5) return "LPAREN";
    if (type == 6) return "RPAREN";
    if (type == 7) return "END";
    return "UNKNOWN";
}

void print_tokens(void) {
    int i = 0;
    while (i < tok_count) {
        if (tok_types[i] == TOK_NUM) {
            printf("%s(%d)", tok_name(tok_types[i]), tok_values[i]);
        } else {
            printf("%s", tok_name(tok_types[i]));
        }
        if (i < tok_count - 1) printf(" ");
        i = i + 1;
    }
    printf("\n");
}

int count_type(int type) {
    int c = 0;
    int i = 0;
    while (i < tok_count) {
        if (tok_types[i] == type) c = c + 1;
        i = i + 1;
    }
    return c;
}

int check_balanced_parens(void) {
    int depth = 0;
    int i = 0;
    while (i < tok_count) {
        if (tok_types[i] == TOK_LPAREN) depth = depth + 1;
        if (tok_types[i] == TOK_RPAREN) depth = depth - 1;
        if (depth < 0) return 0;
        i = i + 1;
    }
    return depth == 0;
}

int max_number(void) {
    int mx = -1;
    int i = 0;
    while (i < tok_count) {
        if (tok_types[i] == TOK_NUM && tok_values[i] > mx) {
            mx = tok_values[i];
        }
        i = i + 1;
    }
    return mx;
}

int main(void) {
    int n;

    n = tokenize("3 + 4");
    // EXPECT: tokens=4
    printf("tokens=%d\n", n);
    // EXPECT: NUM(3) PLUS NUM(4) END
    print_tokens();

    n = tokenize("12 * 34 + 56");
    // EXPECT: tokens=6
    printf("tokens=%d\n", n);
    // EXPECT: NUM(12) MUL NUM(34) PLUS NUM(56) END
    print_tokens();

    n = tokenize("(1 + 2) * 3");
    // EXPECT: tokens=8
    printf("tokens=%d\n", n);
    // EXPECT: LPAREN NUM(1) PLUS NUM(2) RPAREN MUL NUM(3) END
    print_tokens();

    n = tokenize("100 - 50 / 5");
    // EXPECT: tokens=6
    printf("tokens=%d\n", n);
    // EXPECT: nums=3
    printf("nums=%d\n", count_type(TOK_NUM));

    // EXPECT: balanced=1
    tokenize("(1 + (2 * 3))");
    printf("balanced=%d\n", check_balanced_parens());

    // EXPECT: balanced=1
    tokenize("((1 + 2) * (3 - 4))");
    printf("balanced=%d\n", check_balanced_parens());

    tokenize("42 + 100 - 7");
    // EXPECT: max=100
    printf("max=%d\n", max_number());

    tokenize("(5)");
    // EXPECT: tokens=4
    printf("tokens=%d\n", tok_count);
    // EXPECT: parens=2
    printf("parens=%d\n", count_type(TOK_LPAREN) + count_type(TOK_RPAREN));

    n = tokenize("1+2+3+4+5");
    // EXPECT: tokens=10
    printf("tokens=%d\n", n);
    // EXPECT: plus_count=4
    printf("plus_count=%d\n", count_type(TOK_PLUS));

    tokenize("999");
    // EXPECT: single_num=999
    printf("single_num=%d\n", tok_values[0]);

    return 0;
}
