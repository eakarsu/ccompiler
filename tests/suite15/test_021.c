int printf(const char *fmt, ...);

// Lexer for arithmetic expressions (tokenization)
// Tokenizes strings like "123+45*6" into token streams

enum TokenType {
    TOK_NUM,
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_EOF,
    TOK_ERROR
};

struct Token {
    int type;
    int value;
    int pos;
};

struct Lexer {
    char input[128];
    int pos;
    int len;
};

int is_digit(int c) {
    return c >= '0' && c <= '9';
}

int is_space(int c) {
    return c == ' ' || c == '\t';
}

void lexer_init(struct Lexer *lex, char *src) {
    int i = 0;
    while (src[i] != 0) {
        lex->input[i] = src[i];
        i = i + 1;
    }
    lex->input[i] = 0;
    lex->len = i;
    lex->pos = 0;
}

void skip_whitespace(struct Lexer *lex) {
    while (lex->pos < lex->len && is_space(lex->input[lex->pos])) {
        lex->pos = lex->pos + 1;
    }
}

struct Token next_token(struct Lexer *lex) {
    struct Token tok;
    skip_whitespace(lex);

    if (lex->pos >= lex->len) {
        tok.type = TOK_EOF;
        tok.value = 0;
        tok.pos = lex->pos;
        return tok;
    }

    char c = lex->input[lex->pos];
    tok.pos = lex->pos;

    if (is_digit(c)) {
        int num = 0;
        while (lex->pos < lex->len && is_digit(lex->input[lex->pos])) {
            num = num * 10 + (lex->input[lex->pos] - '0');
            lex->pos = lex->pos + 1;
        }
        tok.type = TOK_NUM;
        tok.value = num;
        return tok;
    }

    lex->pos = lex->pos + 1;

    if (c == '+') { tok.type = TOK_PLUS; tok.value = 0; return tok; }
    if (c == '-') { tok.type = TOK_MINUS; tok.value = 0; return tok; }
    if (c == '*') { tok.type = TOK_STAR; tok.value = 0; return tok; }
    if (c == '/') { tok.type = TOK_SLASH; tok.value = 0; return tok; }
    if (c == '(') { tok.type = TOK_LPAREN; tok.value = 0; return tok; }
    if (c == ')') { tok.type = TOK_RPAREN; tok.value = 0; return tok; }

    tok.type = TOK_ERROR;
    tok.value = c;
    return tok;
}

char *token_name(int type) {
    if (type == TOK_NUM) return "NUM";
    if (type == TOK_PLUS) return "PLUS";
    if (type == TOK_MINUS) return "MINUS";
    if (type == TOK_STAR) return "STAR";
    if (type == TOK_SLASH) return "SLASH";
    if (type == TOK_LPAREN) return "LPAREN";
    if (type == TOK_RPAREN) return "RPAREN";
    if (type == TOK_EOF) return "EOF";
    return "ERROR";
}

void tokenize_and_print(char *src) {
    struct Lexer lex;
    lexer_init(&lex, src);
    struct Token tok;
    int count = 0;

    while (1) {
        tok = next_token(&lex);
        if (tok.type == TOK_EOF) break;
        if (tok.type == TOK_NUM) {
            printf("  [%d] %s(%d)\n", tok.pos, token_name(tok.type), tok.value);
        } else {
            printf("  [%d] %s\n", tok.pos, token_name(tok.type));
        }
        count = count + 1;
    }
    printf("  total: %d tokens\n", count);
}

int count_tokens(char *src) {
    struct Lexer lex;
    lexer_init(&lex, src);
    int count = 0;
    struct Token tok;
    while (1) {
        tok = next_token(&lex);
        if (tok.type == TOK_EOF) break;
        count = count + 1;
    }
    return count;
}

int evaluate_simple(char *src) {
    // Evaluate left-to-right, no precedence
    struct Lexer lex;
    lexer_init(&lex, src);
    struct Token tok = next_token(&lex);
    if (tok.type != TOK_NUM) return -1;
    int result = tok.value;

    while (1) {
        struct Token op = next_token(&lex);
        if (op.type == TOK_EOF) break;
        struct Token num = next_token(&lex);
        if (num.type != TOK_NUM) return -1;

        if (op.type == TOK_PLUS) result = result + num.value;
        else if (op.type == TOK_MINUS) result = result - num.value;
        else if (op.type == TOK_STAR) result = result * num.value;
        else if (op.type == TOK_SLASH) result = result / num.value;
    }
    return result;
}

int main() {
    printf("=== Lexer Test 1: Simple addition ===\n");
    // EXPECT: === Lexer Test 1: Simple addition ===
    tokenize_and_print("3+4");
    // EXPECT:   [0] NUM(3)
    // EXPECT:   [1] PLUS
    // EXPECT:   [2] NUM(4)
    // EXPECT:   total: 3 tokens

    printf("=== Lexer Test 2: Complex expression ===\n");
    // EXPECT: === Lexer Test 2: Complex expression ===
    tokenize_and_print("12+34*56");
    // EXPECT:   [0] NUM(12)
    // EXPECT:   [2] PLUS
    // EXPECT:   [3] NUM(34)
    // EXPECT:   [5] STAR
    // EXPECT:   [6] NUM(56)
    // EXPECT:   total: 5 tokens

    printf("=== Lexer Test 3: With parens ===\n");
    // EXPECT: === Lexer Test 3: With parens ===
    tokenize_and_print("(1+2)*3");
    // EXPECT:   [0] LPAREN
    // EXPECT:   [1] NUM(1)
    // EXPECT:   [2] PLUS
    // EXPECT:   [3] NUM(2)
    // EXPECT:   [4] RPAREN
    // EXPECT:   [5] STAR
    // EXPECT:   [6] NUM(3)
    // EXPECT:   total: 7 tokens

    printf("=== Lexer Test 4: With spaces ===\n");
    // EXPECT: === Lexer Test 4: With spaces ===
    int c1 = count_tokens("10 + 20 - 5");
    printf("token count: %d\n", c1);
    // EXPECT: token count: 5

    printf("=== Lexer Test 5: Single number ===\n");
    // EXPECT: === Lexer Test 5: Single number ===
    int c2 = count_tokens("999");
    printf("token count: %d\n", c2);
    // EXPECT: token count: 1

    printf("=== Lexer Test 6: Evaluate simple ===\n");
    // EXPECT: === Lexer Test 6: Evaluate simple ===
    int r1 = evaluate_simple("10+20+30");
    printf("result: %d\n", r1);
    // EXPECT: result: 60

    int r2 = evaluate_simple("100-25-10");
    printf("result: %d\n", r2);
    // EXPECT: result: 65

    int r3 = evaluate_simple("6*7");
    printf("result: %d\n", r3);
    // EXPECT: result: 42

    int r4 = evaluate_simple("100/5/4");
    printf("result: %d\n", r4);
    // EXPECT: result: 5

    printf("=== Lexer Test 7: All operators ===\n");
    // EXPECT: === Lexer Test 7: All operators ===
    tokenize_and_print("8-3/2");
    // EXPECT:   [0] NUM(8)
    // EXPECT:   [1] MINUS
    // EXPECT:   [2] NUM(3)
    // EXPECT:   [3] SLASH
    // EXPECT:   [4] NUM(2)
    // EXPECT:   total: 5 tokens

    printf("done\n");
    // EXPECT: done
    return 0;
}
