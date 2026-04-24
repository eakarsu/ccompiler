int printf(const char *fmt, ...);

/* Lexer/tokenizer state machine.
   Tokenizes a simple expression language with:
   - Integers (sequences of digits)
   - Identifiers (letter followed by letters/digits)
   - Operators: + - * / = ( )
   - Whitespace (skipped) */

enum {
    TOK_NUM = 0,
    TOK_IDENT = 1,
    TOK_PLUS = 2,
    TOK_MINUS = 3,
    TOK_STAR = 4,
    TOK_SLASH = 5,
    TOK_EQ = 6,
    TOK_LPAREN = 7,
    TOK_RPAREN = 8,
    TOK_EOF = 9,
    TOK_ERROR = 10
};

typedef struct {
    int type;
    int int_val;
    char str_val[32];
    int str_len;
} Token;

typedef struct {
    char *input;
    int pos;
} Lexer;

void lexer_init(Lexer *lex, char *input) {
    lex->input = input;
    lex->pos = 0;
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

void token_init(Token *t) {
    t->type = TOK_ERROR;
    t->int_val = 0;
    t->str_len = 0;
    t->str_val[0] = 0;
}

void next_token(Lexer *lex, Token *tok) {
    char c;
    token_init(tok);

    while (lex->input[lex->pos] != 0 && is_space(lex->input[lex->pos])) {
        lex->pos++;
    }

    c = lex->input[lex->pos];

    if (c == 0) {
        tok->type = TOK_EOF;
        return;
    }

    if (is_digit(c)) {
        tok->type = TOK_NUM;
        tok->int_val = 0;
        while (lex->input[lex->pos] != 0 && is_digit(lex->input[lex->pos])) {
            tok->int_val = tok->int_val * 10 + (lex->input[lex->pos] - '0');
            lex->pos++;
        }
        return;
    }

    if (is_alpha(c)) {
        tok->type = TOK_IDENT;
        tok->str_len = 0;
        while (lex->input[lex->pos] != 0 &&
               (is_alpha(lex->input[lex->pos]) || is_digit(lex->input[lex->pos]))) {
            if (tok->str_len < 31) {
                tok->str_val[tok->str_len] = lex->input[lex->pos];
                tok->str_len++;
            }
            lex->pos++;
        }
        tok->str_val[tok->str_len] = 0;
        return;
    }

    lex->pos++;
    if (c == '+') { tok->type = TOK_PLUS; return; }
    if (c == '-') { tok->type = TOK_MINUS; return; }
    if (c == '*') { tok->type = TOK_STAR; return; }
    if (c == '/') { tok->type = TOK_SLASH; return; }
    if (c == '=') { tok->type = TOK_EQ; return; }
    if (c == '(') { tok->type = TOK_LPAREN; return; }
    if (c == ')') { tok->type = TOK_RPAREN; return; }

    tok->type = TOK_ERROR;
}

char *tok_name(int type) {
    if (type == TOK_NUM) return "NUM";
    if (type == TOK_IDENT) return "IDENT";
    if (type == TOK_PLUS) return "PLUS";
    if (type == TOK_MINUS) return "MINUS";
    if (type == TOK_STAR) return "STAR";
    if (type == TOK_SLASH) return "SLASH";
    if (type == TOK_EQ) return "EQ";
    if (type == TOK_LPAREN) return "LPAREN";
    if (type == TOK_RPAREN) return "RPAREN";
    if (type == TOK_EOF) return "EOF";
    return "ERROR";
}

int count_tokens(char *input) {
    Lexer lex;
    Token tok;
    int count = 0;

    lexer_init(&lex, input);
    do {
        next_token(&lex, &tok);
        if (tok.type != TOK_EOF) count++;
    } while (tok.type != TOK_EOF);

    return count;
}

void test_simple_expr(void) {
    Lexer lex;
    Token tok;

    printf("=== Simple Expression ===\n");
    // EXPECT: === Simple Expression ===

    lexer_init(&lex, "42 + 7");

    next_token(&lex, &tok);
    printf("tok1: %s %d\n", tok_name(tok.type), tok.int_val);
    // EXPECT: tok1: NUM 42

    next_token(&lex, &tok);
    printf("tok2: %s\n", tok_name(tok.type));
    // EXPECT: tok2: PLUS

    next_token(&lex, &tok);
    printf("tok3: %s %d\n", tok_name(tok.type), tok.int_val);
    // EXPECT: tok3: NUM 7

    next_token(&lex, &tok);
    printf("tok4: %s\n", tok_name(tok.type));
    // EXPECT: tok4: EOF
}

void test_identifier(void) {
    Lexer lex;
    Token tok;

    printf("=== Identifiers ===\n");
    // EXPECT: === Identifiers ===

    lexer_init(&lex, "x = foo123");

    next_token(&lex, &tok);
    printf("id1: %s\n", tok.str_val);
    // EXPECT: id1: x

    next_token(&lex, &tok);
    printf("op: %s\n", tok_name(tok.type));
    // EXPECT: op: EQ

    next_token(&lex, &tok);
    printf("id2: %s\n", tok.str_val);
    // EXPECT: id2: foo123
}

void test_complex_expr(void) {
    Lexer lex;
    Token tok;
    int i;
    int types[16];
    int n = 0;

    printf("=== Complex Expression ===\n");
    // EXPECT: === Complex Expression ===

    /* (a + b) * 100 - c / 2
       = LPAREN IDENT PLUS IDENT RPAREN STAR NUM MINUS IDENT SLASH NUM
       = 11 tokens */
    lexer_init(&lex, "(a + b) * 100 - c / 2");

    while (n < 16) {
        next_token(&lex, &tok);
        if (tok.type == TOK_EOF) break;
        types[n] = tok.type;
        n++;
    }

    printf("token count: %d\n", n);
    // EXPECT: token count: 11

    printf("types:");
    for (i = 0; i < n; i++) {
        printf(" %d", types[i]);
    }
    printf("\n");
    // EXPECT: types: 7 1 2 1 8 4 0 3 1 5 0
}

void test_no_spaces(void) {
    Lexer lex;
    Token tok;
    int c;

    printf("=== No Spaces ===\n");
    // EXPECT: === No Spaces ===

    c = count_tokens("1+2*3");
    printf("1+2*3 tokens: %d\n", c);
    // EXPECT: 1+2*3 tokens: 5

    lexer_init(&lex, "1+2");
    next_token(&lex, &tok);
    printf("v1=%d\n", tok.int_val);
    // EXPECT: v1=1
    next_token(&lex, &tok);
    printf("op=%d\n", tok.type);
    // EXPECT: op=2
    next_token(&lex, &tok);
    printf("v2=%d\n", tok.int_val);
    // EXPECT: v2=2
}

void test_multi_digit(void) {
    Lexer lex;
    Token tok;

    printf("=== Multi-Digit Numbers ===\n");
    // EXPECT: === Multi-Digit Numbers ===

    lexer_init(&lex, "12345 + 67890");

    next_token(&lex, &tok);
    printf("n1=%d\n", tok.int_val);
    // EXPECT: n1=12345

    next_token(&lex, &tok); /* skip + */
    next_token(&lex, &tok);
    printf("n2=%d\n", tok.int_val);
    // EXPECT: n2=67890
}

void test_underscore_ident(void) {
    Lexer lex;
    Token tok;

    printf("=== Underscore Ident ===\n");
    // EXPECT: === Underscore Ident ===

    lexer_init(&lex, "_start + _end99");

    next_token(&lex, &tok);
    printf("id=%s\n", tok.str_val);
    // EXPECT: id=_start

    next_token(&lex, &tok); /* skip + */
    next_token(&lex, &tok);
    printf("id=%s\n", tok.str_val);
    // EXPECT: id=_end99
}

void test_all_operators(void) {
    Lexer lex;
    Token tok;
    int i;
    int n = 0;

    printf("=== All Operators ===\n");
    // EXPECT: === All Operators ===

    lexer_init(&lex, "+-*/=()");

    for (i = 0; i < 7; i++) {
        next_token(&lex, &tok);
        printf("op%d: %s\n", i, tok_name(tok.type));
    }
    // EXPECT: op0: PLUS
    // EXPECT: op1: MINUS
    // EXPECT: op2: STAR
    // EXPECT: op3: SLASH
    // EXPECT: op4: EQ
    // EXPECT: op5: LPAREN
    // EXPECT: op6: RPAREN
}

void test_mixed_whitespace(void) {
    int c;

    printf("=== Mixed Whitespace ===\n");
    // EXPECT: === Mixed Whitespace ===

    c = count_tokens("  1   +   2  ");
    printf("spaced: %d\n", c);
    // EXPECT: spaced: 3

    c = count_tokens("");
    printf("empty: %d\n", c);
    // EXPECT: empty: 0

    c = count_tokens("   ");
    printf("blanks: %d\n", c);
    // EXPECT: blanks: 0
}

int main(void) {
    test_simple_expr();
    test_identifier();
    test_complex_expr();
    test_no_spaces();
    test_multi_digit();
    test_underscore_ident();
    test_all_operators();
    test_mixed_whitespace();

    printf("Lexer done.\n");
    // EXPECT: Lexer done.
    return 0;
}
