int printf(const char *fmt, ...);

// Lexer for a Mini-Language
// Recognizes: identifiers, integers, operators (+, -, *, /, =, ==, !=, <, >),
// parentheses, semicolons, keywords (if, else, while, return)

enum mini_tok {
    MT_IDENT,
    MT_INT,
    MT_PLUS,
    MT_MINUS,
    MT_STAR,
    MT_SLASH,
    MT_ASSIGN,
    MT_EQ,
    MT_NEQ,
    MT_LT,
    MT_GT,
    MT_LPAREN,
    MT_RPAREN,
    MT_SEMI,
    MT_IF,
    MT_ELSE,
    MT_WHILE,
    MT_RETURN,
    MT_EOF,
    MT_ERROR
};

struct token {
    int type;
    char text[32];
    int text_len;
    int int_val;
};

struct lexer {
    char *src;
    int pos;
    int len;
    struct token tokens[64];
    int count;
};

int is_alpha(int c) {
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95;
}

int is_digit(int c) {
    return c >= 48 && c <= 57;
}

int is_alnum(int c) {
    return is_alpha(c) || is_digit(c);
}

int str_match(char *a, int alen, char *b, int blen) {
    if (alen != blen) return 0;
    int i;
    for (i = 0; i < alen; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void emit_tok(struct lexer *lex, int type, char *text, int tlen, int val) {
    int idx = lex->count;
    if (idx >= 64) return;
    lex->tokens[idx].type = type;
    lex->tokens[idx].int_val = val;
    lex->tokens[idx].text_len = tlen;
    int i;
    for (i = 0; i < 32; i++) lex->tokens[idx].text[i] = 0;
    for (i = 0; i < tlen && i < 31; i++) lex->tokens[idx].text[i] = text[i];
    lex->count = lex->count + 1;
}

int classify_keyword(char *text, int len) {
    // "if"
    if (len == 2 && text[0] == 'i' && text[1] == 'f') return MT_IF;
    // "else"
    if (len == 4 && text[0] == 'e' && text[1] == 'l' && text[2] == 's' && text[3] == 'e') return MT_ELSE;
    // "while"
    if (len == 5 && text[0] == 'w' && text[1] == 'h' && text[2] == 'i' && text[3] == 'l' && text[4] == 'e') return MT_WHILE;
    // "return"
    if (len == 6 && text[0] == 'r' && text[1] == 'e' && text[2] == 't' && text[3] == 'u' && text[4] == 'r' && text[5] == 'n') return MT_RETURN;
    return MT_IDENT;
}

void lex_all(struct lexer *lex) {
    lex->count = 0;
    while (lex->pos < lex->len) {
        char c = lex->src[lex->pos];

        // Skip whitespace
        if (c == 32 || c == 9 || c == 10 || c == 13) {
            lex->pos = lex->pos + 1;
            continue;
        }

        // Identifier or keyword
        if (is_alpha(c)) {
            int start = lex->pos;
            while (lex->pos < lex->len && is_alnum(lex->src[lex->pos])) {
                lex->pos = lex->pos + 1;
            }
            int tlen = lex->pos - start;
            int kw = classify_keyword(lex->src + start, tlen);
            emit_tok(lex, kw, lex->src + start, tlen, 0);
            continue;
        }

        // Number
        if (is_digit(c)) {
            int start = lex->pos;
            int val = 0;
            while (lex->pos < lex->len && is_digit(lex->src[lex->pos])) {
                val = val * 10 + (lex->src[lex->pos] - 48);
                lex->pos = lex->pos + 1;
            }
            int tlen = lex->pos - start;
            emit_tok(lex, MT_INT, lex->src + start, tlen, val);
            continue;
        }

        // Two-char operators
        if (c == 61 && lex->pos + 1 < lex->len && lex->src[lex->pos + 1] == 61) {
            emit_tok(lex, MT_EQ, lex->src + lex->pos, 2, 0);
            lex->pos = lex->pos + 2;
            continue;
        }
        if (c == 33 && lex->pos + 1 < lex->len && lex->src[lex->pos + 1] == 61) {
            emit_tok(lex, MT_NEQ, lex->src + lex->pos, 2, 0);
            lex->pos = lex->pos + 2;
            continue;
        }

        // Single-char operators
        if (c == 43) { emit_tok(lex, MT_PLUS, lex->src + lex->pos, 1, 0); lex->pos = lex->pos + 1; continue; }
        if (c == 45) { emit_tok(lex, MT_MINUS, lex->src + lex->pos, 1, 0); lex->pos = lex->pos + 1; continue; }
        if (c == 42) { emit_tok(lex, MT_STAR, lex->src + lex->pos, 1, 0); lex->pos = lex->pos + 1; continue; }
        if (c == 47) { emit_tok(lex, MT_SLASH, lex->src + lex->pos, 1, 0); lex->pos = lex->pos + 1; continue; }
        if (c == 61) { emit_tok(lex, MT_ASSIGN, lex->src + lex->pos, 1, 0); lex->pos = lex->pos + 1; continue; }
        if (c == 60) { emit_tok(lex, MT_LT, lex->src + lex->pos, 1, 0); lex->pos = lex->pos + 1; continue; }
        if (c == 62) { emit_tok(lex, MT_GT, lex->src + lex->pos, 1, 0); lex->pos = lex->pos + 1; continue; }
        if (c == 40) { emit_tok(lex, MT_LPAREN, lex->src + lex->pos, 1, 0); lex->pos = lex->pos + 1; continue; }
        if (c == 41) { emit_tok(lex, MT_RPAREN, lex->src + lex->pos, 1, 0); lex->pos = lex->pos + 1; continue; }
        if (c == 59) { emit_tok(lex, MT_SEMI, lex->src + lex->pos, 1, 0); lex->pos = lex->pos + 1; continue; }

        emit_tok(lex, MT_ERROR, lex->src + lex->pos, 1, 0);
        lex->pos = lex->pos + 1;
    }
}

char *type_name(int t) {
    if (t == MT_IDENT) return "IDENT";
    if (t == MT_INT) return "INT";
    if (t == MT_PLUS) return "PLUS";
    if (t == MT_MINUS) return "MINUS";
    if (t == MT_STAR) return "STAR";
    if (t == MT_SLASH) return "SLASH";
    if (t == MT_ASSIGN) return "ASSIGN";
    if (t == MT_EQ) return "EQ";
    if (t == MT_NEQ) return "NEQ";
    if (t == MT_LT) return "LT";
    if (t == MT_GT) return "GT";
    if (t == MT_LPAREN) return "LPAREN";
    if (t == MT_RPAREN) return "RPAREN";
    if (t == MT_SEMI) return "SEMI";
    if (t == MT_IF) return "IF";
    if (t == MT_ELSE) return "ELSE";
    if (t == MT_WHILE) return "WHILE";
    if (t == MT_RETURN) return "RETURN";
    if (t == MT_ERROR) return "ERROR";
    return "EOF";
}

int main() {
    struct lexer lex;

    printf("=== Mini Lexer ===\n"); // EXPECT: === Mini Lexer ===

    // Test 1: "x = 42;"
    char s1[16];
    s1[0]='x'; s1[1]=' '; s1[2]='='; s1[3]=' '; s1[4]='4'; s1[5]='2'; s1[6]=';'; s1[7]=0;
    lex.src = s1; lex.pos = 0; lex.len = 7;
    lex_all(&lex);
    printf("Count: %d\n", lex.count);                      // EXPECT: Count: 4
    printf("T0: %s %s\n", type_name(lex.tokens[0].type), lex.tokens[0].text); // EXPECT: T0: IDENT x
    printf("T1: %s\n", type_name(lex.tokens[1].type));     // EXPECT: T1: ASSIGN
    printf("T2: %s %d\n", type_name(lex.tokens[2].type), lex.tokens[2].int_val); // EXPECT: T2: INT 42
    printf("T3: %s\n", type_name(lex.tokens[3].type));     // EXPECT: T3: SEMI

    // Test 2: "if (a == b)"
    char s2[16];
    s2[0]='i'; s2[1]='f'; s2[2]=' '; s2[3]='('; s2[4]='a';
    s2[5]=' '; s2[6]='='; s2[7]='='; s2[8]=' '; s2[9]='b'; s2[10]=')'; s2[11]=0;
    lex.src = s2; lex.pos = 0; lex.len = 11;
    lex_all(&lex);
    printf("Count: %d\n", lex.count);                      // EXPECT: Count: 6
    printf("T0: %s\n", type_name(lex.tokens[0].type));     // EXPECT: T0: IF
    printf("T1: %s\n", type_name(lex.tokens[1].type));     // EXPECT: T1: LPAREN
    printf("T2: %s %s\n", type_name(lex.tokens[2].type), lex.tokens[2].text); // EXPECT: T2: IDENT a
    printf("T3: %s\n", type_name(lex.tokens[3].type));     // EXPECT: T3: EQ
    printf("T4: %s %s\n", type_name(lex.tokens[4].type), lex.tokens[4].text); // EXPECT: T4: IDENT b
    printf("T5: %s\n", type_name(lex.tokens[5].type));     // EXPECT: T5: RPAREN

    // Test 3: "return a + b * c;"
    char s3[24];
    s3[0]='r'; s3[1]='e'; s3[2]='t'; s3[3]='u'; s3[4]='r'; s3[5]='n';
    s3[6]=' '; s3[7]='a'; s3[8]=' '; s3[9]='+'; s3[10]=' ';
    s3[11]='b'; s3[12]=' '; s3[13]='*'; s3[14]=' '; s3[15]='c'; s3[16]=';'; s3[17]=0;
    lex.src = s3; lex.pos = 0; lex.len = 17;
    lex_all(&lex);
    printf("Count: %d\n", lex.count);                      // EXPECT: Count: 7
    printf("T0: %s\n", type_name(lex.tokens[0].type));     // EXPECT: T0: RETURN
    printf("T2: %s\n", type_name(lex.tokens[2].type));     // EXPECT: T2: PLUS
    printf("T4: %s\n", type_name(lex.tokens[4].type));     // EXPECT: T4: STAR

    // Test 4: "a != 0"
    char s4[8];
    s4[0]='a'; s4[1]=' '; s4[2]='!'; s4[3]='='; s4[4]=' '; s4[5]='0'; s4[6]=0;
    lex.src = s4; lex.pos = 0; lex.len = 6;
    lex_all(&lex);
    printf("Count: %d\n", lex.count);                      // EXPECT: Count: 3
    printf("T1: %s\n", type_name(lex.tokens[1].type));     // EXPECT: T1: NEQ

    printf("Done\n"); // EXPECT: Done
    return 0;
}
