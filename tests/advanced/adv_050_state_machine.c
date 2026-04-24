// EXPECT: === Finite State Machine Lexer ===
// EXPECT: --- Tokenize "123+456" ---
// EXPECT: NUM(123)
// EXPECT: OP(+)
// EXPECT: NUM(456)
// EXPECT: END
// EXPECT: Token count: 3
// EXPECT: --- Tokenize "1+2*3" ---
// EXPECT: NUM(1)
// EXPECT: OP(+)
// EXPECT: NUM(2)
// EXPECT: OP(*)
// EXPECT: NUM(3)
// EXPECT: END
// EXPECT: Token count: 5
// EXPECT: --- Tokenize "(10+20)*3" ---
// EXPECT: LPAREN
// EXPECT: NUM(10)
// EXPECT: OP(+)
// EXPECT: NUM(20)
// EXPECT: RPAREN
// EXPECT: OP(*)
// EXPECT: NUM(3)
// EXPECT: END
// EXPECT: Token count: 7
// EXPECT: --- Tokenize "  42  " ---
// EXPECT: NUM(42)
// EXPECT: END
// EXPECT: Token count: 1
// EXPECT: --- Tokenize "" ---
// EXPECT: END
// EXPECT: Token count: 0
// EXPECT: --- Tokenize "100" ---
// EXPECT: NUM(100)
// EXPECT: END
// EXPECT: Token count: 1
// EXPECT: --- Tokenize "((1+2))" ---
// EXPECT: LPAREN
// EXPECT: LPAREN
// EXPECT: NUM(1)
// EXPECT: OP(+)
// EXPECT: NUM(2)
// EXPECT: RPAREN
// EXPECT: RPAREN
// EXPECT: END
// EXPECT: Token count: 7
// EXPECT: --- Tokenize "5-3/2+1" ---
// EXPECT: NUM(5)
// EXPECT: OP(-)
// EXPECT: NUM(3)
// EXPECT: OP(/)
// EXPECT: NUM(2)
// EXPECT: OP(+)
// EXPECT: NUM(1)
// EXPECT: END
// EXPECT: Token count: 7
// EXPECT: --- Tokenize "9999" ---
// EXPECT: NUM(9999)
// EXPECT: END
// EXPECT: Token count: 1
// EXPECT: --- Tokenize "1+2+3+4+5" ---
// EXPECT: NUM(1)
// EXPECT: OP(+)
// EXPECT: NUM(2)
// EXPECT: OP(+)
// EXPECT: NUM(3)
// EXPECT: OP(+)
// EXPECT: NUM(4)
// EXPECT: OP(+)
// EXPECT: NUM(5)
// EXPECT: END
// EXPECT: Token count: 9
// EXPECT: --- Eval "1+2*3" = 7 ---
// EXPECT: --- Eval "10+20" = 30 ---
// EXPECT: --- Eval "100-50" = 50 ---
// EXPECT: --- Eval "5+3-2" = 6 ---
// EXPECT: === Done ===

int printf(const char *fmt, ...);

// Token types
// 0=END, 1=NUM, 2=OP, 3=LPAREN, 4=RPAREN, 5=ERROR

struct Token {
    int type;
    int value;    // for NUM: numeric value; for OP: char code
};

// FSM states
// 0=START, 1=IN_NUMBER, 2=DONE

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

int is_op(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

struct Token tokens[64];
int token_count;

int tokenize(char *input) {
    token_count = 0;
    int pos = 0;
    int state = 0;  // START
    int num_val = 0;

    while (1) {
        char c = input[pos];

        if (state == 0) {
            // START state
            if (c == 0) {
                // End of input
                break;
            } else if (is_space(c)) {
                pos = pos + 1;
            } else if (is_digit(c)) {
                state = 1;
                num_val = c - '0';
                pos = pos + 1;
            } else if (is_op(c)) {
                tokens[token_count].type = 2;
                tokens[token_count].value = c;
                token_count = token_count + 1;
                pos = pos + 1;
            } else if (c == '(') {
                tokens[token_count].type = 3;
                tokens[token_count].value = 0;
                token_count = token_count + 1;
                pos = pos + 1;
            } else if (c == ')') {
                tokens[token_count].type = 4;
                tokens[token_count].value = 0;
                token_count = token_count + 1;
                pos = pos + 1;
            } else {
                tokens[token_count].type = 5;
                tokens[token_count].value = c;
                token_count = token_count + 1;
                pos = pos + 1;
            }
        } else if (state == 1) {
            // IN_NUMBER state
            if (is_digit(c)) {
                num_val = num_val * 10 + (c - '0');
                pos = pos + 1;
            } else {
                // Emit number token, don't advance pos
                tokens[token_count].type = 1;
                tokens[token_count].value = num_val;
                token_count = token_count + 1;
                state = 0;
            }
        }
    }

    // If we were in the middle of a number, emit it
    if (state == 1) {
        tokens[token_count].type = 1;
        tokens[token_count].value = num_val;
        token_count = token_count + 1;
    }

    return token_count;
}

void print_tokens(void) {
    int i = 0;
    while (i < token_count) {
        if (tokens[i].type == 1) {
            printf("NUM(%d)\n", tokens[i].value);
        } else if (tokens[i].type == 2) {
            printf("OP(%c)\n", tokens[i].value);
        } else if (tokens[i].type == 3) {
            printf("LPAREN\n");
        } else if (tokens[i].type == 4) {
            printf("RPAREN\n");
        } else if (tokens[i].type == 5) {
            printf("ERROR(%c)\n", tokens[i].value);
        }
        i = i + 1;
    }
    printf("END\n");
    printf("Token count: %d\n", token_count);
}

void test_tokenize(char *input) {
    printf("--- Tokenize \"%s\" ---\n", input);
    tokenize(input);
    print_tokens();
}

// Simple expression evaluator (left-to-right, respects * / precedence)
// Only handles: num op num op num ... (no parens in eval)
int eval_simple(char *input) {
    tokenize(input);
    if (token_count == 0) return 0;

    // Two-pass: first handle * and /, then + and -
    int vals[32];
    int ops[32];
    int nval = 0;
    int nop = 0;

    int i = 0;
    while (i < token_count) {
        if (tokens[i].type == 1) {
            vals[nval] = tokens[i].value;
            nval = nval + 1;
        } else if (tokens[i].type == 2) {
            ops[nop] = tokens[i].value;
            nop = nop + 1;
        }
        i = i + 1;
    }

    // First pass: handle * and /
    int vals2[32];
    int ops2[32];
    int nval2 = 0;
    int nop2 = 0;

    vals2[0] = vals[0];
    nval2 = 1;

    i = 0;
    while (i < nop) {
        if (ops[i] == '*') {
            vals2[nval2 - 1] = vals2[nval2 - 1] * vals[i + 1];
        } else if (ops[i] == '/') {
            vals2[nval2 - 1] = vals2[nval2 - 1] / vals[i + 1];
        } else {
            ops2[nop2] = ops[i];
            nop2 = nop2 + 1;
            vals2[nval2] = vals[i + 1];
            nval2 = nval2 + 1;
        }
        i = i + 1;
    }

    // Second pass: handle + and -
    int result = vals2[0];
    i = 0;
    while (i < nop2) {
        if (ops2[i] == '+') {
            result = result + vals2[i + 1];
        } else if (ops2[i] == '-') {
            result = result - vals2[i + 1];
        }
        i = i + 1;
    }

    return result;
}

int main(void) {
    printf("=== Finite State Machine Lexer ===\n");

    test_tokenize("123+456");
    test_tokenize("1+2*3");
    test_tokenize("(10+20)*3");
    test_tokenize("  42  ");
    test_tokenize("");
    test_tokenize("100");
    test_tokenize("((1+2))");
    test_tokenize("5-3/2+1");
    test_tokenize("9999");
    test_tokenize("1+2+3+4+5");

    // Evaluation tests (simple, no parens)
    printf("--- Eval \"1+2*3\" = %d ---\n", eval_simple("1+2*3"));
    printf("--- Eval \"10+20\" = %d ---\n", eval_simple("10+20"));
    printf("--- Eval \"100-50\" = %d ---\n", eval_simple("100-50"));
    printf("--- Eval \"5+3-2\" = %d ---\n", eval_simple("5+3-2"));

    printf("=== Done ===\n");
    return 0;
}
