int printf(const char *fmt, ...);

enum ParseState {
    PS_START, PS_IN_NUMBER, PS_IN_WORD, PS_IN_STRING,
    PS_OPERATOR, PS_WHITESPACE, PS_ERROR, PS_DONE
};

struct Token {
    int type;
    char text[32];
    int len;
};

struct Parser {
    int state;
    char *input;
    int pos;
    int token_count;
    int number_count;
    int word_count;
    int op_count;
    int string_count;
    int error_count;
};

void init_parser(struct Parser *p, char *input) {
    p->state = PS_START;
    p->input = input;
    p->pos = 0;
    p->token_count = 0;
    p->number_count = 0;
    p->word_count = 0;
    p->op_count = 0;
    p->string_count = 0;
    p->error_count = 0;
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

void emit_token(struct Parser *p, char *type_str) {
    p->token_count = p->token_count + 1;
    printf("Token[%d]: %s\n", p->token_count, type_str);
}

void parse_step(struct Parser *p) {
    char c = p->input[p->pos];

    if (c == 0) {
        if (p->state == PS_IN_NUMBER) {
            emit_token(p, "NUMBER");
            p->number_count = p->number_count + 1;
        } else if (p->state == PS_IN_WORD) {
            emit_token(p, "WORD");
            p->word_count = p->word_count + 1;
        }
        p->state = PS_DONE;
        return;
    }

    switch (p->state) {
    case PS_START:
    case PS_WHITESPACE:
        if (is_digit(c)) {
            p->state = PS_IN_NUMBER;
        } else if (is_alpha(c)) {
            p->state = PS_IN_WORD;
        } else if (is_operator(c)) {
            p->state = PS_OPERATOR;
            emit_token(p, "OP");
            p->op_count = p->op_count + 1;
            p->state = PS_START;
        } else if (c == '"') {
            p->state = PS_IN_STRING;
        } else if (is_whitespace(c)) {
            p->state = PS_WHITESPACE;
        } else {
            p->state = PS_ERROR;
            p->error_count = p->error_count + 1;
            p->state = PS_START;
        }
        break;

    case PS_IN_NUMBER:
        if (is_digit(c)) {
            /* stay in number */
        } else {
            emit_token(p, "NUMBER");
            p->number_count = p->number_count + 1;
            p->state = PS_START;
            p->pos = p->pos - 1;
        }
        break;

    case PS_IN_WORD:
        if (is_alpha(c) || is_digit(c)) {
            /* stay in word */
        } else {
            emit_token(p, "WORD");
            p->word_count = p->word_count + 1;
            p->state = PS_START;
            p->pos = p->pos - 1;
        }
        break;

    case PS_IN_STRING:
        if (c == '"') {
            emit_token(p, "STRING");
            p->string_count = p->string_count + 1;
            p->state = PS_START;
        }
        break;

    default:
        p->state = PS_ERROR;
        break;
    }

    p->pos = p->pos + 1;
}

void run_parser(struct Parser *p) {
    while (p->state != PS_DONE && p->state != PS_ERROR) {
        parse_step(p);
    }
}

int main() {
    struct Parser p1;
    init_parser(&p1, "123+abc");
    printf("--- Parse: 123+abc ---\n");
    // EXPECT: --- Parse: 123+abc ---
    run_parser(&p1);
    // EXPECT: Token[1]: NUMBER
    // EXPECT: Token[2]: OP
    // EXPECT: Token[3]: WORD
    printf("Tokens: %d nums=%d words=%d ops=%d\n",
           p1.token_count, p1.number_count, p1.word_count, p1.op_count);
    // EXPECT: Tokens: 3 nums=1 words=1 ops=1

    struct Parser p2;
    init_parser(&p2, "x = 42 + y * 7");
    printf("--- Parse: x = 42 + y * 7 ---\n");
    // EXPECT: --- Parse: x = 42 + y * 7 ---
    run_parser(&p2);
    // EXPECT: Token[1]: WORD
    // EXPECT: Token[2]: OP
    // EXPECT: Token[3]: NUMBER
    // EXPECT: Token[4]: OP
    // EXPECT: Token[5]: WORD
    // EXPECT: Token[6]: OP
    // EXPECT: Token[7]: NUMBER
    printf("Tokens: %d nums=%d words=%d ops=%d\n",
           p2.token_count, p2.number_count, p2.word_count, p2.op_count);
    // EXPECT: Tokens: 7 nums=2 words=2 ops=3

    struct Parser p3;
    init_parser(&p3, "foo<bar>99");
    printf("--- Parse: foo<bar>99 ---\n");
    // EXPECT: --- Parse: foo<bar>99 ---
    run_parser(&p3);
    // EXPECT: Token[1]: WORD
    // EXPECT: Token[2]: OP
    // EXPECT: Token[3]: WORD
    // EXPECT: Token[4]: OP
    // EXPECT: Token[5]: NUMBER
    printf("Tokens: %d\n", p3.token_count);
    // EXPECT: Tokens: 5

    struct Parser p4;
    init_parser(&p4, "a+b-c*d/e");
    printf("--- Parse: a+b-c*d/e ---\n");
    // EXPECT: --- Parse: a+b-c*d/e ---
    run_parser(&p4);
    // EXPECT: Token[1]: WORD
    // EXPECT: Token[2]: OP
    // EXPECT: Token[3]: WORD
    // EXPECT: Token[4]: OP
    // EXPECT: Token[5]: WORD
    // EXPECT: Token[6]: OP
    // EXPECT: Token[7]: WORD
    // EXPECT: Token[8]: OP
    // EXPECT: Token[9]: WORD
    printf("Ops: %d, Words: %d\n", p4.op_count, p4.word_count);
    // EXPECT: Ops: 4, Words: 5
    printf("Errors: %d\n", p4.error_count);
    // EXPECT: Errors: 0

    return 0;
}
