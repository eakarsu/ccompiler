int printf(const char *fmt, ...);

// Lexer tokenizer for a C-like language
// Tokenizes simple expressions with numbers, operators, identifiers, and parens

int TOK_NUM = 1;
int TOK_PLUS = 2;
int TOK_MINUS = 3;
int TOK_STAR = 4;
int TOK_SLASH = 5;
int TOK_LPAREN = 6;
int TOK_RPAREN = 7;
int TOK_IDENT = 8;
int TOK_SEMI = 9;
int TOK_ASSIGN = 10;
int TOK_EOF = 11;
int TOK_EQ = 12;
int TOK_LT = 13;
int TOK_GT = 14;

struct Token {
    int type;
    int int_val;
    char name[32];
};

int is_digit(int ch) {
    return ch >= 48 && ch <= 57;
}

int is_alpha(int ch) {
    return (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122) || ch == 95;
}

int is_alnum(int ch) {
    return is_digit(ch) || is_alpha(ch);
}

int char_at(char *s, int i) {
    return s[i];
}

struct Token make_token(int type, int val) {
    struct Token t;
    t.type = type;
    t.int_val = val;
    t.name[0] = 0;
    return t;
}

int tokenize(char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int count = 0;
    int len = 0;
    char *p = input;
    while (p[len] != 0) len = len + 1;

    while (pos < len && count < max_tokens) {
        int ch = char_at(input, pos);

        if (ch == 32 || ch == 9 || ch == 10) {
            pos = pos + 1;
            continue;
        }

        if (is_digit(ch)) {
            int val = 0;
            while (pos < len && is_digit(char_at(input, pos))) {
                val = val * 10 + (char_at(input, pos) - 48);
                pos = pos + 1;
            }
            tokens[count] = make_token(TOK_NUM, val);
            count = count + 1;
            continue;
        }

        if (is_alpha(ch)) {
            int start = pos;
            int ni = 0;
            struct Token t;
            t.type = TOK_IDENT;
            t.int_val = 0;
            while (pos < len && is_alnum(char_at(input, pos)) && ni < 31) {
                t.name[ni] = char_at(input, pos);
                ni = ni + 1;
                pos = pos + 1;
            }
            t.name[ni] = 0;
            tokens[count] = t;
            count = count + 1;
            continue;
        }

        if (ch == 43) { tokens[count] = make_token(TOK_PLUS, 0); count = count + 1; pos = pos + 1; }
        else if (ch == 45) { tokens[count] = make_token(TOK_MINUS, 0); count = count + 1; pos = pos + 1; }
        else if (ch == 42) { tokens[count] = make_token(TOK_STAR, 0); count = count + 1; pos = pos + 1; }
        else if (ch == 47) { tokens[count] = make_token(TOK_SLASH, 0); count = count + 1; pos = pos + 1; }
        else if (ch == 40) { tokens[count] = make_token(TOK_LPAREN, 0); count = count + 1; pos = pos + 1; }
        else if (ch == 41) { tokens[count] = make_token(TOK_RPAREN, 0); count = count + 1; pos = pos + 1; }
        else if (ch == 59) { tokens[count] = make_token(TOK_SEMI, 0); count = count + 1; pos = pos + 1; }
        else if (ch == 61) {
            if (pos + 1 < len && char_at(input, pos + 1) == 61) {
                tokens[count] = make_token(TOK_EQ, 0);
                count = count + 1;
                pos = pos + 2;
            } else {
                tokens[count] = make_token(TOK_ASSIGN, 0);
                count = count + 1;
                pos = pos + 1;
            }
        }
        else if (ch == 60) { tokens[count] = make_token(TOK_LT, 0); count = count + 1; pos = pos + 1; }
        else if (ch == 62) { tokens[count] = make_token(TOK_GT, 0); count = count + 1; pos = pos + 1; }
        else { pos = pos + 1; }
    }
    tokens[count] = make_token(TOK_EOF, 0);
    return count;
}

char *tok_name(int type) {
    if (type == 1) return "NUM";
    if (type == 2) return "PLUS";
    if (type == 3) return "MINUS";
    if (type == 4) return "STAR";
    if (type == 5) return "SLASH";
    if (type == 6) return "LPAREN";
    if (type == 7) return "RPAREN";
    if (type == 8) return "IDENT";
    if (type == 9) return "SEMI";
    if (type == 10) return "ASSIGN";
    if (type == 11) return "EOF";
    if (type == 12) return "EQ";
    if (type == 13) return "LT";
    if (type == 14) return "GT";
    return "UNKNOWN";
}

int main() {
    struct Token tokens[64];
    int count;

    count = tokenize("x = 42 + y * 3;", tokens, 64);
    printf("token count: %d\n", count);
    // EXPECT: token count: 8

    int i = 0;
    while (i < count) {
        printf("tok[%d]: %s", i, tok_name(tokens[i].type));
        if (tokens[i].type == TOK_NUM) {
            printf(" val=%d", tokens[i].int_val);
        }
        if (tokens[i].type == TOK_IDENT) {
            printf(" name=%s", tokens[i].name);
        }
        printf("\n");
        i = i + 1;
    }
    // EXPECT: tok[0]: IDENT name=x
    // EXPECT: tok[1]: ASSIGN
    // EXPECT: tok[2]: NUM val=42
    // EXPECT: tok[3]: PLUS
    // EXPECT: tok[4]: IDENT name=y
    // EXPECT: tok[5]: STAR
    // EXPECT: tok[6]: NUM val=3
    // EXPECT: tok[7]: SEMI

    count = tokenize("a == b < c > d", tokens, 64);
    printf("token count2: %d\n", count);
    // EXPECT: token count2: 7

    printf("t0: %s %s\n", tok_name(tokens[0].type), tokens[0].name);
    // EXPECT: t0: IDENT a
    printf("t1: %s\n", tok_name(tokens[1].type));
    // EXPECT: t1: EQ
    printf("t2: %s %s\n", tok_name(tokens[2].type), tokens[2].name);
    // EXPECT: t2: IDENT b
    printf("t3: %s\n", tok_name(tokens[3].type));
    // EXPECT: t3: LT
    printf("t5: %s\n", tok_name(tokens[5].type));
    // EXPECT: t5: GT

    count = tokenize("(10 - 3) / 2", tokens, 64);
    printf("paren count: %d\n", count);
    // EXPECT: paren count: 7
    printf("p0: %s\n", tok_name(tokens[0].type));
    // EXPECT: p0: LPAREN
    printf("p1: %s val=%d\n", tok_name(tokens[1].type), tokens[1].int_val);
    // EXPECT: p1: NUM val=10
    printf("p6: %s\n", tok_name(tokens[6].type));
    // EXPECT: p6: NUM

    printf("lexer done\n");
    // EXPECT: lexer done

    return 0;
}
