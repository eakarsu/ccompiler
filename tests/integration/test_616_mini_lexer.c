int printf(const char *fmt, ...);
// EXPECT: === Mini Lexer Test ===\n--- Test 1: single number ---\nToken: NUM val=42\nToken: EOF val=0\nCount: 2\n--- Test 2: simple addition ---\nToken: NUM val=1\nToken: PLUS val=0\nToken: NUM val=2\nToken: EOF val=0\nCount: 4\n--- Test 3: complex expression ---\nToken: NUM val=123\nToken: PLUS val=0\nToken: NUM val=456\nToken: STAR val=0\nToken: LPAREN val=0\nToken: NUM val=78\nToken: MINUS val=0\nToken: NUM val=9\nToken: RPAREN val=0\nToken: EOF val=0\nCount: 10\n--- Test 4: extra whitespace ---\nToken: NUM val=100\nToken: SLASH val=0\nToken: NUM val=25\nToken: EOF val=0\nCount: 4\n--- Test 5: nested parens ---\nToken: LPAREN val=0\nToken: LPAREN val=0\nToken: LPAREN val=0\nToken: NUM val=3\nToken: RPAREN val=0\nToken: RPAREN val=0\nToken: RPAREN val=0\nToken: EOF val=0\nCount: 8\n--- Test 6: all operators ---\nToken: NUM val=10\nToken: PLUS val=0\nToken: NUM val=20\nToken: MINUS val=0\nToken: NUM val=30\nToken: STAR val=0\nToken: NUM val=40\nToken: SLASH val=0\nToken: NUM val=50\nToken: EOF val=0\nCount: 10\n--- Test 7: empty string ---\nToken: EOF val=0\nCount: 1\n--- Test 8: just whitespace ---\nToken: EOF val=0\nCount: 1\n--- Token type values ---\nTOK_NUM=0\nTOK_PLUS=1\nTOK_MINUS=2\nTOK_STAR=3\nTOK_SLASH=4\nTOK_LPAREN=5\nTOK_RPAREN=6\nTOK_EOF=7\n--- Token size check ---\nToken has type and value\n--- Reverse lookup ---\nType 0 -> NUM\nType 1 -> PLUS\nType 2 -> MINUS\nType 3 -> STAR\nType 4 -> SLASH\nType 5 -> LPAREN\nType 6 -> RPAREN\nType 7 -> EOF\n--- Test 9: multi-digit numbers ---\nToken: NUM val=0\nToken: PLUS val=0\nToken: NUM val=9\nToken: PLUS val=0\nToken: NUM val=99\nToken: PLUS val=0\nToken: NUM val=999\nToken: PLUS val=0\nToken: NUM val=9999\nToken: EOF val=0\nCount: 10\n--- Test 10: no spaces ---\nToken: NUM val=1\nToken: PLUS val=0\nToken: NUM val=2\nToken: STAR val=0\nToken: NUM val=3\nToken: MINUS val=0\nToken: NUM val=4\nToken: SLASH val=0\nToken: NUM val=5\nToken: EOF val=0\nCount: 10\n--- Test 11: expression evaluation check ---\nTokenized: 2 + 3 * 4\nFound 6 tokens\nToken 0: NUM(2)\nToken 1: PLUS\nToken 2: NUM(3)\nToken 3: STAR\nToken 4: NUM(4)\nToken 5: EOF\n--- Test 12: counting token types ---\nInput: (1+2)*(3-4)/(5+6)\nNumbers: 6\nOperators: 5\nParens: 6\nTotal: 18\n--- Test 13: largest number ---\nToken: NUM val=32767\nToken: PLUS val=0\nToken: NUM val=1\nToken: EOF val=0\nCount: 4\n--- Test 14: token array walk ---\nForward: NUM PLUS NUM STAR LPAREN NUM MINUS NUM RPAREN EOF\nBackward: EOF RPAREN NUM MINUS NUM LPAREN STAR NUM PLUS NUM\n=== All mini lexer tests passed ===\n

// -------------------------------------------------------
// Token type enumeration
// -------------------------------------------------------
enum {
    TOK_NUM    = 0,
    TOK_PLUS   = 1,
    TOK_MINUS  = 2,
    TOK_STAR   = 3,
    TOK_SLASH  = 4,
    TOK_LPAREN = 5,
    TOK_RPAREN = 6,
    TOK_EOF    = 7
};

// -------------------------------------------------------
// Token structure
// -------------------------------------------------------
struct Token {
    int type;
    int value;
};

// -------------------------------------------------------
// Return a printable name for a token type
// -------------------------------------------------------
const char *tok_name(int type) {
    if (type == TOK_NUM)    return "NUM";
    if (type == TOK_PLUS)   return "PLUS";
    if (type == TOK_MINUS)  return "MINUS";
    if (type == TOK_STAR)   return "STAR";
    if (type == TOK_SLASH)  return "SLASH";
    if (type == TOK_LPAREN) return "LPAREN";
    if (type == TOK_RPAREN) return "RPAREN";
    if (type == TOK_EOF)    return "EOF";
    return "UNKNOWN";
}

// -------------------------------------------------------
// Check if a character is a digit
// -------------------------------------------------------
int is_digit(char c) {
    return c >= '0' && c <= '9';
}

// -------------------------------------------------------
// Check if a character is whitespace
// -------------------------------------------------------
int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

// -------------------------------------------------------
// Lexer: tokenize an input string into the token array.
// Returns the number of tokens produced (including EOF).
// -------------------------------------------------------
int lex(const char *input, struct Token *tokens, int max_tokens) {
    const char *p = input;
    int count = 0;

    while (*p != '\0' && count < max_tokens - 1) {
        // Skip whitespace using pointer traversal
        while (*p != '\0' && is_space(*p)) {
            p++;
        }
        if (*p == '\0') {
            break;
        }

        // Parse multi-digit numbers
        if (is_digit(*p)) {
            int val = 0;
            while (*p >= '0' && *p <= '9') {
                val = val * 10 + (*p - '0');
                p++;
            }
            tokens[count].type = TOK_NUM;
            tokens[count].value = val;
            count++;
            continue;
        }

        // Single-character operators via switch dispatch
        switch (*p) {
            case '+':
                tokens[count].type = TOK_PLUS;
                tokens[count].value = 0;
                count++;
                p++;
                break;
            case '-':
                tokens[count].type = TOK_MINUS;
                tokens[count].value = 0;
                count++;
                p++;
                break;
            case '*':
                tokens[count].type = TOK_STAR;
                tokens[count].value = 0;
                count++;
                p++;
                break;
            case '/':
                tokens[count].type = TOK_SLASH;
                tokens[count].value = 0;
                count++;
                p++;
                break;
            case '(':
                tokens[count].type = TOK_LPAREN;
                tokens[count].value = 0;
                count++;
                p++;
                break;
            case ')':
                tokens[count].type = TOK_RPAREN;
                tokens[count].value = 0;
                count++;
                p++;
                break;
            default:
                // Skip unknown characters
                p++;
                break;
        }
    }

    // Append EOF token
    tokens[count].type = TOK_EOF;
    tokens[count].value = 0;
    count++;

    return count;
}

// -------------------------------------------------------
// Print all tokens in the array
// -------------------------------------------------------
void print_tokens(struct Token *tokens, int count) {
    int i;
    for (i = 0; i < count; i++) {
        printf("Token: %s val=%d\n", tok_name(tokens[i].type), tokens[i].value);
    }
}

// -------------------------------------------------------
// Count tokens of a specific category
// -------------------------------------------------------
int count_type(struct Token *tokens, int count, int type) {
    int n = 0;
    int i;
    for (i = 0; i < count; i++) {
        if (tokens[i].type == type) {
            n++;
        }
    }
    return n;
}

// -------------------------------------------------------
// Count operator tokens (PLUS, MINUS, STAR, SLASH)
// -------------------------------------------------------
int count_operators(struct Token *tokens, int count) {
    int n = 0;
    int i;
    for (i = 0; i < count; i++) {
        int t = tokens[i].type;
        if (t == TOK_PLUS || t == TOK_MINUS || t == TOK_STAR || t == TOK_SLASH) {
            n++;
        }
    }
    return n;
}

// -------------------------------------------------------
// Count paren tokens (LPAREN, RPAREN)
// -------------------------------------------------------
int count_parens(struct Token *tokens, int count) {
    int n = 0;
    int i;
    for (i = 0; i < count; i++) {
        int t = tokens[i].type;
        if (t == TOK_LPAREN || t == TOK_RPAREN) {
            n++;
        }
    }
    return n;
}

// -------------------------------------------------------
// Main test driver
// -------------------------------------------------------
int main() {
    struct Token tokens[64];
    int count;
    int i;

    printf("=== Mini Lexer Test ===\n");

    // ----- Test 1: single number -----
    printf("--- Test 1: single number ---\n");
    count = lex("42", tokens, 64);
    print_tokens(tokens, count);
    printf("Count: %d\n", count);

    // ----- Test 2: simple addition -----
    printf("--- Test 2: simple addition ---\n");
    count = lex("1+2", tokens, 64);
    print_tokens(tokens, count);
    printf("Count: %d\n", count);

    // ----- Test 3: complex expression -----
    printf("--- Test 3: complex expression ---\n");
    count = lex("123 + 456 * (78 - 9)", tokens, 64);
    print_tokens(tokens, count);
    printf("Count: %d\n", count);

    // ----- Test 4: extra whitespace -----
    printf("--- Test 4: extra whitespace ---\n");
    count = lex(" 100  /  25 ", tokens, 64);
    print_tokens(tokens, count);
    printf("Count: %d\n", count);

    // ----- Test 5: nested parens -----
    printf("--- Test 5: nested parens ---\n");
    count = lex("(((3)))", tokens, 64);
    print_tokens(tokens, count);
    printf("Count: %d\n", count);

    // ----- Test 6: all operators -----
    printf("--- Test 6: all operators ---\n");
    count = lex("10+20-30*40/50", tokens, 64);
    print_tokens(tokens, count);
    printf("Count: %d\n", count);

    // ----- Test 7: empty string -----
    printf("--- Test 7: empty string ---\n");
    count = lex("", tokens, 64);
    print_tokens(tokens, count);
    printf("Count: %d\n", count);

    // ----- Test 8: just whitespace -----
    printf("--- Test 8: just whitespace ---\n");
    count = lex("   ", tokens, 64);
    print_tokens(tokens, count);
    printf("Count: %d\n", count);

    // ----- Token type values -----
    printf("--- Token type values ---\n");
    printf("TOK_NUM=%d\n", TOK_NUM);
    printf("TOK_PLUS=%d\n", TOK_PLUS);
    printf("TOK_MINUS=%d\n", TOK_MINUS);
    printf("TOK_STAR=%d\n", TOK_STAR);
    printf("TOK_SLASH=%d\n", TOK_SLASH);
    printf("TOK_LPAREN=%d\n", TOK_LPAREN);
    printf("TOK_RPAREN=%d\n", TOK_RPAREN);
    printf("TOK_EOF=%d\n", TOK_EOF);

    // ----- Token size check -----
    printf("--- Token size check ---\n");
    {
        struct Token t;
        t.type = TOK_NUM;
        t.value = 42;
        if (t.type == TOK_NUM && t.value == 42) {
            printf("Token has type and value\n");
        }
    }

    // ----- Reverse lookup -----
    printf("--- Reverse lookup ---\n");
    for (i = 0; i <= 7; i++) {
        printf("Type %d -> %s\n", i, tok_name(i));
    }

    // ----- Test 9: multi-digit numbers -----
    printf("--- Test 9: multi-digit numbers ---\n");
    count = lex("0+9+99+999+9999", tokens, 64);
    print_tokens(tokens, count);
    printf("Count: %d\n", count);

    // ----- Test 10: no spaces -----
    printf("--- Test 10: no spaces ---\n");
    count = lex("1+2*3-4/5", tokens, 64);
    print_tokens(tokens, count);
    printf("Count: %d\n", count);

    // ----- Test 11: expression evaluation check -----
    printf("--- Test 11: expression evaluation check ---\n");
    {
        char expr[] = "2 + 3 * 4";
        int n = lex(expr, tokens, 64);
        printf("Tokenized: %s\n", expr);
        printf("Found %d tokens\n", n);
        for (i = 0; i < n; i++) {
            if (tokens[i].type == TOK_NUM) {
                printf("Token %d: NUM(%d)\n", i, tokens[i].value);
            } else {
                printf("Token %d: %s\n", i, tok_name(tokens[i].type));
            }
        }
    }

    // ----- Test 12: counting token types -----
    printf("--- Test 12: counting token types ---\n");
    {
        char input[] = "(1+2)*(3-4)/(5+6)";
        int n = lex(input, tokens, 64);
        int nums = count_type(tokens, n, TOK_NUM);
        int ops = count_operators(tokens, n);
        int parens = count_parens(tokens, n);
        printf("Input: %s\n", input);
        printf("Numbers: %d\n", nums);
        printf("Operators: %d\n", ops);
        printf("Parens: %d\n", parens);
        // Total = nums + ops + parens + EOF
        printf("Total: %d\n", n);
    }

    // ----- Test 13: largest number -----
    printf("--- Test 13: largest number ---\n");
    count = lex("32767+1", tokens, 64);
    print_tokens(tokens, count);
    printf("Count: %d\n", count);

    // ----- Test 14: token array walk -----
    printf("--- Test 14: token array walk ---\n");
    {
        int n = lex("123 + 456 * (78 - 9)", tokens, 64);
        // Forward walk
        printf("Forward: ");
        for (i = 0; i < n; i++) {
            if (i > 0) printf(" ");
            printf("%s", tok_name(tokens[i].type));
        }
        printf("\n");
        // Backward walk
        printf("Backward:");
        for (i = n - 1; i >= 0; i--) {
            printf(" %s", tok_name(tokens[i].type));
        }
        printf("\n");
    }

    printf("=== All mini lexer tests passed ===\n");

    return 0;
}
