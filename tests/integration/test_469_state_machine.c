int printf(const char *fmt, ...);
// EXPECT: Input: 3 + 4 * 5\nTokens: NUM(3) PLUS NUM(4) STAR NUM(5) END\nCount: 6\nInput: (10 - 2) / 4\nTokens: LPAREN NUM(10) MINUS NUM(2) RPAREN SLASH NUM(4) END\nInput: 42\nTokens: NUM(42) END\nInput: 100 + 200 - 50 * 3\nTokens: NUM(100) PLUS NUM(200) MINUS NUM(50) STAR NUM(3) END\nCount: 8
// State machine for parsing: tokenizes simple expressions

enum State {
    S_START = 0,
    S_NUMBER = 1,
    S_OP = 2,
    S_SPACE = 3,
    S_ERROR = 4,
    S_END = 5
};

enum TokenType {
    T_NUM = 0,
    T_PLUS = 1,
    T_MINUS = 2,
    T_STAR = 3,
    T_SLASH = 4,
    T_LPAREN = 5,
    T_RPAREN = 6,
    T_END = 7,
    T_ERROR = 8
};

struct Token {
    int type;
    int value;
};

struct Lexer {
    char input[64];
    int pos;
    struct Token tokens[32];
    int num_tokens;
};

int is_digit(char c) { return c >= '0' && c <= '9'; }
int is_space(char c) { return c == ' '; }

void lex_init(struct Lexer *lex, char *input) {
    int i = 0;
    while (input[i] != 0 && i < 63) {
        lex->input[i] = input[i];
        i++;
    }
    lex->input[i] = 0;
    lex->pos = 0;
    lex->num_tokens = 0;
}

void lex_add_token(struct Lexer *lex, int type, int value) {
    lex->tokens[lex->num_tokens].type = type;
    lex->tokens[lex->num_tokens].value = value;
    lex->num_tokens++;
}

void lex_run(struct Lexer *lex) {
    int state = S_START;
    char c;
    int num;
    while (state != S_END && state != S_ERROR) {
        c = lex->input[lex->pos];
        if (c == 0) {
            lex_add_token(lex, T_END, 0);
            state = S_END;
        } else if (is_space(c)) {
            lex->pos++;
        } else if (is_digit(c)) {
            num = 0;
            while (is_digit(lex->input[lex->pos])) {
                num = num * 10 + (lex->input[lex->pos] - '0');
                lex->pos++;
            }
            lex_add_token(lex, T_NUM, num);
        } else if (c == '+') {
            lex_add_token(lex, T_PLUS, 0);
            lex->pos++;
        } else if (c == '-') {
            lex_add_token(lex, T_MINUS, 0);
            lex->pos++;
        } else if (c == '*') {
            lex_add_token(lex, T_STAR, 0);
            lex->pos++;
        } else if (c == '/') {
            lex_add_token(lex, T_SLASH, 0);
            lex->pos++;
        } else if (c == '(') {
            lex_add_token(lex, T_LPAREN, 0);
            lex->pos++;
        } else if (c == ')') {
            lex_add_token(lex, T_RPAREN, 0);
            lex->pos++;
        } else {
            lex_add_token(lex, T_ERROR, 0);
            state = S_ERROR;
        }
    }
}

void print_token(struct Token *t) {
    if (t->type == T_NUM) printf("NUM(%d)", t->value);
    else if (t->type == T_PLUS) printf("PLUS");
    else if (t->type == T_MINUS) printf("MINUS");
    else if (t->type == T_STAR) printf("STAR");
    else if (t->type == T_SLASH) printf("SLASH");
    else if (t->type == T_LPAREN) printf("LPAREN");
    else if (t->type == T_RPAREN) printf("RPAREN");
    else if (t->type == T_END) printf("END");
    else printf("ERROR");
}

void lex_print_tokens(struct Lexer *lex) {
    int i;
    for (i = 0; i < lex->num_tokens; i++) {
        print_token(&lex->tokens[i]);
        if (i < lex->num_tokens - 1) printf(" ");
    }
    printf("\n");
}

int main(void) {
    struct Lexer lex;

    lex_init(&lex, "3 + 4 * 5");
    lex_run(&lex);
    printf("Input: 3 + 4 * 5\n");
    printf("Tokens: ");
    lex_print_tokens(&lex);
    printf("Count: %d\n", lex.num_tokens);

    lex_init(&lex, "(10 - 2) / 4");
    lex_run(&lex);
    printf("Input: (10 - 2) / 4\n");
    printf("Tokens: ");
    lex_print_tokens(&lex);

    lex_init(&lex, "42");
    lex_run(&lex);
    printf("Input: 42\n");
    printf("Tokens: ");
    lex_print_tokens(&lex);

    lex_init(&lex, "100 + 200 - 50 * 3");
    lex_run(&lex);
    printf("Input: 100 + 200 - 50 * 3\n");
    printf("Tokens: ");
    lex_print_tokens(&lex);
    printf("Count: %d\n", lex.num_tokens);

    return 0;
}
