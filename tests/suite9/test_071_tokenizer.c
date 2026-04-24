int printf(const char *fmt, ...);

enum TokenType {
    TOK_NUM,
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_SEMI,
    TOK_IDENT,
    TOK_ASSIGN,
    TOK_EOF,
    TOK_UNKNOWN
};

struct Token {
    int type;
    int value;
    char name[16];
    int name_len;
};

struct Lexer {
    char input[128];
    int pos;
    int len;
    struct Token tokens[32];
    int token_count;
};

int is_digit(int ch) {
    return ch >= '0' && ch <= '9';
}

int is_alpha(int ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}

int is_alnum(int ch) {
    return is_digit(ch) || is_alpha(ch);
}

void skip_whitespace(struct Lexer *lex) {
    while (lex->pos < lex->len) {
        char c = lex->input[lex->pos];
        if (c == ' ' || c == '\t' || c == '\n') {
            lex->pos = lex->pos + 1;
        } else {
            break;
        }
    }
}

int read_number(struct Lexer *lex) {
    int val = 0;
    while (lex->pos < lex->len && is_digit(lex->input[lex->pos])) {
        val = val * 10 + (lex->input[lex->pos] - '0');
        lex->pos = lex->pos + 1;
    }
    return val;
}

void read_ident(struct Lexer *lex, struct Token *tok) {
    int start = lex->pos;
    while (lex->pos < lex->len && is_alnum(lex->input[lex->pos])) {
        lex->pos = lex->pos + 1;
    }
    int length = lex->pos - start;
    if (length > 15) length = 15;
    int i;
    for (i = 0; i < length; i++) {
        tok->name[i] = lex->input[start + i];
    }
    tok->name[length] = 0;
    tok->name_len = length;
}

void tokenize(struct Lexer *lex) {
    lex->token_count = 0;
    while (lex->pos < lex->len) {
        skip_whitespace(lex);
        if (lex->pos >= lex->len) break;

        struct Token *tok = &lex->tokens[lex->token_count];
        tok->value = 0;
        tok->name[0] = 0;
        tok->name_len = 0;
        char c = lex->input[lex->pos];

        if (is_digit(c)) {
            tok->type = TOK_NUM;
            tok->value = read_number(lex);
        } else if (is_alpha(c)) {
            tok->type = TOK_IDENT;
            read_ident(lex, tok);
        } else if (c == '+') { tok->type = TOK_PLUS; lex->pos = lex->pos + 1;
        } else if (c == '-') { tok->type = TOK_MINUS; lex->pos = lex->pos + 1;
        } else if (c == '*') { tok->type = TOK_STAR; lex->pos = lex->pos + 1;
        } else if (c == '/') { tok->type = TOK_SLASH; lex->pos = lex->pos + 1;
        } else if (c == '(') { tok->type = TOK_LPAREN; lex->pos = lex->pos + 1;
        } else if (c == ')') { tok->type = TOK_RPAREN; lex->pos = lex->pos + 1;
        } else if (c == ';') { tok->type = TOK_SEMI; lex->pos = lex->pos + 1;
        } else if (c == '=') { tok->type = TOK_ASSIGN; lex->pos = lex->pos + 1;
        } else {
            tok->type = TOK_UNKNOWN;
            lex->pos = lex->pos + 1;
        }
        lex->token_count = lex->token_count + 1;
    }
}

char *token_type_name(int type) {
    if (type == TOK_NUM) return "NUM";
    if (type == TOK_PLUS) return "PLUS";
    if (type == TOK_MINUS) return "MINUS";
    if (type == TOK_STAR) return "STAR";
    if (type == TOK_SLASH) return "SLASH";
    if (type == TOK_LPAREN) return "LPAREN";
    if (type == TOK_RPAREN) return "RPAREN";
    if (type == TOK_SEMI) return "SEMI";
    if (type == TOK_IDENT) return "IDENT";
    if (type == TOK_ASSIGN) return "ASSIGN";
    if (type == TOK_EOF) return "EOF";
    return "UNKNOWN";
}

void init_lexer(struct Lexer *lex, char *src) {
    int i = 0;
    while (src[i] != 0) {
        lex->input[i] = src[i];
        i = i + 1;
    }
    lex->input[i] = 0;
    lex->len = i;
    lex->pos = 0;
    lex->token_count = 0;
}

int main() {
    struct Lexer lex;

    init_lexer(&lex, "x = 10 + 20;");
    tokenize(&lex);
    // EXPECT: Tokenizing: x = 10 + 20;
    printf("Tokenizing: x = 10 + 20;\n");
    // EXPECT: Token count: 6
    printf("Token count: %d\n", lex.token_count);
    int i;
    for (i = 0; i < lex.token_count; i++) {
        // EXPECT: Token[0]: IDENT
        // EXPECT: Token[1]: ASSIGN
        // EXPECT: Token[2]: NUM val=10
        // EXPECT: Token[3]: PLUS
        // EXPECT: Token[4]: NUM val=20
        // EXPECT: Token[5]: SEMI
        if (lex.tokens[i].type == TOK_NUM) {
            printf("Token[%d]: %s val=%d\n", i, token_type_name(lex.tokens[i].type), lex.tokens[i].value);
        } else {
            printf("Token[%d]: %s\n", i, token_type_name(lex.tokens[i].type));
        }
    }

    init_lexer(&lex, "(3 + 5) * 2");
    tokenize(&lex);
    // EXPECT: Tokenizing: (3 + 5) * 2
    printf("Tokenizing: (3 + 5) * 2\n");
    // EXPECT: Token count: 7
    printf("Token count: %d\n", lex.token_count);
    for (i = 0; i < lex.token_count; i++) {
        // EXPECT: Token[0]: LPAREN
        // EXPECT: Token[1]: NUM val=3
        // EXPECT: Token[2]: PLUS
        // EXPECT: Token[3]: NUM val=5
        // EXPECT: Token[4]: RPAREN
        // EXPECT: Token[5]: STAR
        // EXPECT: Token[6]: NUM val=2
        if (lex.tokens[i].type == TOK_NUM) {
            printf("Token[%d]: %s val=%d\n", i, token_type_name(lex.tokens[i].type), lex.tokens[i].value);
        } else {
            printf("Token[%d]: %s\n", i, token_type_name(lex.tokens[i].type));
        }
    }

    init_lexer(&lex, "a - b / c");
    tokenize(&lex);
    // EXPECT: Tokenizing: a - b / c
    printf("Tokenizing: a - b / c\n");
    // EXPECT: Token count: 5
    printf("Token count: %d\n", lex.token_count);
    for (i = 0; i < lex.token_count; i++) {
        // EXPECT: Token[0]: IDENT
        // EXPECT: Token[1]: MINUS
        // EXPECT: Token[2]: IDENT
        // EXPECT: Token[3]: SLASH
        // EXPECT: Token[4]: IDENT
        printf("Token[%d]: %s\n", i, token_type_name(lex.tokens[i].type));
    }

    // EXPECT: Done
    printf("Done\n");
    return 0;
}
