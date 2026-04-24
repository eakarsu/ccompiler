int printf(const char *fmt, ...);

// JSON Tokenizer
// Recognizes: strings, numbers, true, false, null, braces, brackets, colons, commas

enum json_tok_type {
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_LBRACKET,
    TOK_RBRACKET,
    TOK_COLON,
    TOK_COMMA,
    TOK_STRING,
    TOK_NUMBER,
    TOK_TRUE,
    TOK_FALSE,
    TOK_NULL_TOK,
    TOK_ERROR,
    TOK_EOF
};

enum json_lex_state {
    LEX_START,
    LEX_IN_STRING,
    LEX_IN_STRING_ESC,
    LEX_IN_NUMBER,
    LEX_IN_KEYWORD
};

struct json_token {
    int type;
    char value[32];
    int value_len;
};

struct json_lexer {
    char *input;
    int pos;
    int len;
    int state;
    struct json_token tokens[32];
    int token_count;
    char buf[32];
    int buf_len;
};

void lexer_init(struct json_lexer *lex, char *inp, int len) {
    lex->input = inp;
    lex->pos = 0;
    lex->len = len;
    lex->state = LEX_START;
    lex->token_count = 0;
    lex->buf_len = 0;
    int i;
    for (i = 0; i < 32; i++) {
        lex->buf[i] = 0;
    }
}

void add_token(struct json_lexer *lex, int type) {
    int idx = lex->token_count;
    if (idx >= 32) return;
    lex->tokens[idx].type = type;
    int i;
    for (i = 0; i < 32; i++) {
        lex->tokens[idx].value[i] = 0;
    }
    for (i = 0; i < lex->buf_len && i < 31; i++) {
        lex->tokens[idx].value[i] = lex->buf[i];
    }
    lex->tokens[idx].value_len = lex->buf_len;
    lex->token_count = lex->token_count + 1;
    lex->buf_len = 0;
    int j;
    for (j = 0; j < 32; j++) {
        lex->buf[j] = 0;
    }
}

int str_eq(char *a, char *b, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void finish_keyword(struct json_lexer *lex) {
    // check true, false, null
    if (lex->buf_len == 4 && lex->buf[0]=='t' && lex->buf[1]=='r' && lex->buf[2]=='u' && lex->buf[3]=='e') {
        add_token(lex, TOK_TRUE);
    } else if (lex->buf_len == 5 && lex->buf[0]=='f' && lex->buf[1]=='a' && lex->buf[2]=='l' && lex->buf[3]=='s' && lex->buf[4]=='e') {
        add_token(lex, TOK_FALSE);
    } else if (lex->buf_len == 4 && lex->buf[0]=='n' && lex->buf[1]=='u' && lex->buf[2]=='l' && lex->buf[3]=='l') {
        add_token(lex, TOK_NULL_TOK);
    } else {
        add_token(lex, TOK_ERROR);
    }
}

int is_digit(int c) {
    return c >= 48 && c <= 57;
}

int is_alpha(int c) {
    return (c >= 97 && c <= 122) || (c >= 65 && c <= 90);
}

void json_tokenize(struct json_lexer *lex) {
    while (lex->pos < lex->len) {
        char c = lex->input[lex->pos];
        int st = lex->state;

        if (st == LEX_START) {
            if (c == 32 || c == 9 || c == 10 || c == 13) {
                lex->pos = lex->pos + 1;
            } else if (c == 123) {
                add_token(lex, TOK_LBRACE);
                lex->pos = lex->pos + 1;
            } else if (c == 125) {
                add_token(lex, TOK_RBRACE);
                lex->pos = lex->pos + 1;
            } else if (c == 91) {
                add_token(lex, TOK_LBRACKET);
                lex->pos = lex->pos + 1;
            } else if (c == 93) {
                add_token(lex, TOK_RBRACKET);
                lex->pos = lex->pos + 1;
            } else if (c == 58) {
                add_token(lex, TOK_COLON);
                lex->pos = lex->pos + 1;
            } else if (c == 44) {
                add_token(lex, TOK_COMMA);
                lex->pos = lex->pos + 1;
            } else if (c == 34) {
                lex->state = LEX_IN_STRING;
                lex->pos = lex->pos + 1;
            } else if (is_digit(c) || c == 45) {
                lex->buf[lex->buf_len] = c;
                lex->buf_len = lex->buf_len + 1;
                lex->state = LEX_IN_NUMBER;
                lex->pos = lex->pos + 1;
            } else if (is_alpha(c)) {
                lex->buf[lex->buf_len] = c;
                lex->buf_len = lex->buf_len + 1;
                lex->state = LEX_IN_KEYWORD;
                lex->pos = lex->pos + 1;
            } else {
                add_token(lex, TOK_ERROR);
                lex->pos = lex->pos + 1;
            }
        } else if (st == LEX_IN_STRING) {
            if (c == 34) {
                add_token(lex, TOK_STRING);
                lex->state = LEX_START;
                lex->pos = lex->pos + 1;
            } else if (c == 92) {
                lex->state = LEX_IN_STRING_ESC;
                lex->pos = lex->pos + 1;
            } else {
                if (lex->buf_len < 31) {
                    lex->buf[lex->buf_len] = c;
                    lex->buf_len = lex->buf_len + 1;
                }
                lex->pos = lex->pos + 1;
            }
        } else if (st == LEX_IN_STRING_ESC) {
            if (lex->buf_len < 31) {
                lex->buf[lex->buf_len] = c;
                lex->buf_len = lex->buf_len + 1;
            }
            lex->state = LEX_IN_STRING;
            lex->pos = lex->pos + 1;
        } else if (st == LEX_IN_NUMBER) {
            if (is_digit(c) || c == 46) {
                if (lex->buf_len < 31) {
                    lex->buf[lex->buf_len] = c;
                    lex->buf_len = lex->buf_len + 1;
                }
                lex->pos = lex->pos + 1;
            } else {
                add_token(lex, TOK_NUMBER);
                lex->state = LEX_START;
            }
        } else if (st == LEX_IN_KEYWORD) {
            if (is_alpha(c)) {
                if (lex->buf_len < 31) {
                    lex->buf[lex->buf_len] = c;
                    lex->buf_len = lex->buf_len + 1;
                }
                lex->pos = lex->pos + 1;
            } else {
                finish_keyword(lex);
                lex->state = LEX_START;
            }
        }
    }
    // Finish any pending token
    if (lex->state == LEX_IN_NUMBER) {
        add_token(lex, TOK_NUMBER);
    } else if (lex->state == LEX_IN_KEYWORD) {
        finish_keyword(lex);
    }
}

char *tok_name(int t) {
    if (t == TOK_LBRACE) return "LBRACE";
    if (t == TOK_RBRACE) return "RBRACE";
    if (t == TOK_LBRACKET) return "LBRACKET";
    if (t == TOK_RBRACKET) return "RBRACKET";
    if (t == TOK_COLON) return "COLON";
    if (t == TOK_COMMA) return "COMMA";
    if (t == TOK_STRING) return "STRING";
    if (t == TOK_NUMBER) return "NUMBER";
    if (t == TOK_TRUE) return "TRUE";
    if (t == TOK_FALSE) return "FALSE";
    if (t == TOK_NULL_TOK) return "NULL";
    if (t == TOK_ERROR) return "ERROR";
    return "EOF";
}

int main() {
    struct json_lexer lex;

    printf("=== JSON Tokenizer ===\n"); // EXPECT: === JSON Tokenizer ===

    // Test 1: {"a":42}
    char j1[16];
    j1[0]=123; j1[1]=34; j1[2]='a'; j1[3]=34;
    j1[4]=58; j1[5]='4'; j1[6]='2'; j1[7]=125;
    j1[8]=0;
    lexer_init(&lex, j1, 8);
    json_tokenize(&lex);
    printf("Count: %d\n", lex.token_count);         // EXPECT: Count: 5
    printf("T0: %s\n", tok_name(lex.tokens[0].type)); // EXPECT: T0: LBRACE
    printf("T1: %s\n", tok_name(lex.tokens[1].type)); // EXPECT: T1: STRING
    printf("V1: %s\n", lex.tokens[1].value);          // EXPECT: V1: a
    printf("T2: %s\n", tok_name(lex.tokens[2].type)); // EXPECT: T2: COLON
    printf("T3: %s\n", tok_name(lex.tokens[3].type)); // EXPECT: T3: NUMBER
    printf("V3: %s\n", lex.tokens[3].value);          // EXPECT: V3: 42
    printf("T4: %s\n", tok_name(lex.tokens[4].type)); // EXPECT: T4: RBRACE

    // Test 2: [true,false,null]
    char j2[32];
    j2[0]=91; // [
    j2[1]='t'; j2[2]='r'; j2[3]='u'; j2[4]='e';
    j2[5]=44; // ,
    j2[6]='f'; j2[7]='a'; j2[8]='l'; j2[9]='s'; j2[10]='e';
    j2[11]=44;
    j2[12]='n'; j2[13]='u'; j2[14]='l'; j2[15]='l';
    j2[16]=93; // ]
    j2[17]=0;
    lexer_init(&lex, j2, 17);
    json_tokenize(&lex);
    printf("Count: %d\n", lex.token_count);            // EXPECT: Count: 7
    printf("T0: %s\n", tok_name(lex.tokens[0].type));  // EXPECT: T0: LBRACKET
    printf("T1: %s\n", tok_name(lex.tokens[1].type));  // EXPECT: T1: TRUE
    printf("T3: %s\n", tok_name(lex.tokens[3].type));  // EXPECT: T3: FALSE
    printf("T5: %s\n", tok_name(lex.tokens[5].type));  // EXPECT: T5: NULL
    printf("T6: %s\n", tok_name(lex.tokens[6].type));  // EXPECT: T6: RBRACKET

    // Test 3: Negative number
    char j3[8];
    j3[0]=45; j3[1]='7'; j3[2]=0;
    lexer_init(&lex, j3, 2);
    json_tokenize(&lex);
    printf("Count: %d\n", lex.token_count);            // EXPECT: Count: 1
    printf("V0: %s\n", lex.tokens[0].value);           // EXPECT: V0: -7

    printf("Done\n"); // EXPECT: Done
    return 0;
}
