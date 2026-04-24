int printf(const char *fmt, ...);

enum TokenType {
    TOK_NUMBER,
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_SPACE,
    TOK_UNKNOWN,
    TOK_EOF
};

struct Token {
    int type;
    int value;     /* numeric value for TOK_NUMBER */
    int start;     /* start position in input */
    int length;    /* length of token */
};

struct Tokenizer {
    char *input;
    int pos;
    int len;
    int token_count;
};

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_space(char c) {
    return c == ' ';
}

int char_to_digit(char c) {
    return c - '0';
}

void tok_init(struct Tokenizer *t, char *input, int len) {
    t->input = input;
    t->pos = 0;
    t->len = len;
    t->token_count = 0;
}

struct Token tok_next(struct Tokenizer *t) {
    struct Token tok;
    tok.value = 0;
    tok.start = t->pos;
    tok.length = 0;

    if (t->pos >= t->len) {
        tok.type = TOK_EOF;
        return tok;
    }

    char c = t->input[t->pos];

    /* Skip spaces */
    if (is_space(c)) {
        while (t->pos < t->len && is_space(t->input[t->pos])) {
            t->pos = t->pos + 1;
        }
        tok.type = TOK_SPACE;
        tok.length = t->pos - tok.start;
        t->token_count = t->token_count + 1;
        return tok;
    }

    /* Number */
    if (is_digit(c)) {
        int val = 0;
        while (t->pos < t->len && is_digit(t->input[t->pos])) {
            val = val * 10 + char_to_digit(t->input[t->pos]);
            t->pos = t->pos + 1;
        }
        tok.type = TOK_NUMBER;
        tok.value = val;
        tok.length = t->pos - tok.start;
        t->token_count = t->token_count + 1;
        return tok;
    }

    /* Single character tokens */
    t->pos = t->pos + 1;
    tok.length = 1;
    t->token_count = t->token_count + 1;
    switch (c) {
        case '+': tok.type = TOK_PLUS; break;
        case '-': tok.type = TOK_MINUS; break;
        case '*': tok.type = TOK_STAR; break;
        case '/': tok.type = TOK_SLASH; break;
        case '(': tok.type = TOK_LPAREN; break;
        case ')': tok.type = TOK_RPAREN; break;
        default:  tok.type = TOK_UNKNOWN; break;
    }
    return tok;
}

int count_tokens_of_type(struct Token *tokens, int n, int type) {
    int count = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (tokens[i].type == type) {
            count = count + 1;
        }
    }
    return count;
}

int sum_numbers(struct Token *tokens, int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (tokens[i].type == TOK_NUMBER) {
            sum = sum + tokens[i].value;
        }
    }
    return sum;
}

int main(void) {
    /* Tokenize "123 + 45 * 6" */
    char input1[20];
    input1[0] = '1'; input1[1] = '2'; input1[2] = '3';
    input1[3] = ' '; input1[4] = '+'; input1[5] = ' ';
    input1[6] = '4'; input1[7] = '5'; input1[8] = ' ';
    input1[9] = '*'; input1[10] = ' '; input1[11] = '6';
    int len1 = 12;

    struct Tokenizer tok;
    tok_init(&tok, input1, len1);

    struct Token tokens[30];
    int n = 0;
    while (1) {
        struct Token t = tok_next(&tok);
        if (t.type == TOK_EOF) break;
        tokens[n] = t;
        n = n + 1;
    }

    /* "123 + 45 * 6" -> 123, SPACE, +, SPACE, 45, SPACE, *, SPACE, 6 = 9 tokens */
    // EXPECT: Token count: 9
    printf("Token count: %d\n", n);

    /* First token should be NUMBER with value 123 */
    // EXPECT: Token 0 type: 0
    printf("Token 0 type: %d\n", tokens[0].type);
    // EXPECT: Token 0 value: 123
    printf("Token 0 value: %d\n", tokens[0].value);

    /* Token 2 should be PLUS */
    // EXPECT: Token 2 type: 1
    printf("Token 2 type: %d\n", tokens[2].type);

    /* Token 4 should be NUMBER 45 */
    // EXPECT: Token 4 value: 45
    printf("Token 4 value: %d\n", tokens[4].value);

    /* Token 6 should be STAR */
    // EXPECT: Token 6 type: 3
    printf("Token 6 type: %d\n", tokens[6].type);

    /* Token 8 should be NUMBER 6 */
    // EXPECT: Token 8 value: 6
    printf("Token 8 value: %d\n", tokens[8].value);

    int num_count = count_tokens_of_type(tokens, n, TOK_NUMBER);
    // EXPECT: Number tokens: 3
    printf("Number tokens: %d\n", num_count);

    int space_count = count_tokens_of_type(tokens, n, TOK_SPACE);
    // EXPECT: Space tokens: 4
    printf("Space tokens: %d\n", space_count);

    int total = sum_numbers(tokens, n);
    // 123 + 45 + 6 = 174
    // EXPECT: Sum of numbers: 174
    printf("Sum of numbers: %d\n", total);

    /* Tokenize "(10-3)" */
    char input2[10];
    input2[0] = '('; input2[1] = '1'; input2[2] = '0';
    input2[3] = '-'; input2[4] = '3'; input2[5] = ')';
    int len2 = 6;

    tok_init(&tok, input2, len2);
    n = 0;
    while (1) {
        struct Token t = tok_next(&tok);
        if (t.type == TOK_EOF) break;
        tokens[n] = t;
        n = n + 1;
    }

    /* Tokens: ( 10 - 3 ) = 5 tokens */
    // EXPECT: Tokens in (10-3): 5
    printf("Tokens in (10-3): %d\n", n);
    // EXPECT: First is LPAREN: 5
    printf("First is LPAREN: %d\n", tokens[0].type);
    // EXPECT: Number value: 10
    printf("Number value: %d\n", tokens[1].value);
    // EXPECT: Minus type: 2
    printf("Minus type: %d\n", tokens[2].type);
    // EXPECT: Second number: 3
    printf("Second number: %d\n", tokens[3].value);
    // EXPECT: Last is RPAREN: 6
    printf("Last is RPAREN: %d\n", tokens[4].type);

    /* Tokenize a larger expression: "100/25+300" */
    char input3[15];
    input3[0] = '1'; input3[1] = '0'; input3[2] = '0';
    input3[3] = '/'; input3[4] = '2'; input3[5] = '5';
    input3[6] = '+'; input3[7] = '3'; input3[8] = '0';
    input3[9] = '0';
    int len3 = 10;

    tok_init(&tok, input3, len3);
    n = 0;
    while (1) {
        struct Token t = tok_next(&tok);
        if (t.type == TOK_EOF) break;
        tokens[n] = t;
        n = n + 1;
    }
    // EXPECT: Tokens in 100/25+300: 5
    printf("Tokens in 100/25+300: %d\n", n);

    total = sum_numbers(tokens, n);
    // 100 + 25 + 300 = 425
    // EXPECT: Sum: 425
    printf("Sum: %d\n", total);

    /* Check token positions */
    // EXPECT: Token 0 start: 0
    printf("Token 0 start: %d\n", tokens[0].start);
    // EXPECT: Token 0 length: 3
    printf("Token 0 length: %d\n", tokens[0].length);
    // EXPECT: Token 1 start: 3
    printf("Token 1 start: %d\n", tokens[1].start);

    return 0;
}
