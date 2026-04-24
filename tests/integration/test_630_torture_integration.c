int printf(const char *fmt, ...);
// EXPECT: === Calculator Torture Test ===\n--- Tokenizer Tests ---\nTokenize "42": [NUM(42)] EOF count=2\nTokenize "3+4": [NUM(3) PLUS NUM(4)] EOF count=4\nTokenize "(2+3)*4": [LPAREN NUM(2) PLUS NUM(3) RPAREN MUL NUM(4)] EOF count=8\nTokenize "-5": [MINUS NUM(5)] EOF count=3\n--- AST Construction Tests ---\nAST for "42": NUM(42)\nAST for "3+4": ADD(NUM(3), NUM(4))\nAST for "3*4+2": ADD(MUL(NUM(3), NUM(4)), NUM(2))\nAST for "2+3*4": ADD(NUM(2), MUL(NUM(3), NUM(4)))\n--- Evaluation Tests ---\nTest 1: "42" = 42 [PASS]\nTest 2: "3+4" = 7 [PASS]\nTest 3: "10-3" = 7 [PASS]\nTest 4: "3*4+2" = 14 [PASS]\nTest 5: "2+3*4" = 14 [PASS]\nTest 6: "(2+3)*4" = 20 [PASS]\nTest 7: "100/(2*5)" = 10 [PASS]\nTest 8: "1+2+3+4+5" = 15 [PASS]\nTest 9: "(10-3)*(4+2)" = 42 [PASS]\nTest 10: "2*3+4*5" = 26 [PASS]\n--- Unary Minus Tests ---\nTest 11: "-5" = -5 [PASS]\nTest 12: "-3+8" = 5 [PASS]\nTest 13: "-(2+3)" = -5 [PASS]\nTest 14: "10*-2" = -20 [PASS]\n--- Complex Expression Tests ---\nTest 15: "((((1+2))))" = 3 [PASS]\nTest 16: "100-50-25-10" = 15 [PASS]\nTest 17: "2*3*4*5" = 120 [PASS]\nTest 18: "1000/10/10" = 10 [PASS]\nTest 19: "(1+2)*(3+4)*(1+1)" = 42 [PASS]\nTest 20: "50+25*2-10/5" = 98 [PASS]\n--- Node Pool Stats ---\nTotal nodes allocated: varies\nNode pool not exhausted: OK\n--- Summary ---\nPassed: 20/20\n=== All torture tests passed ===\n

// Test 630: Calculator torture integration test
// Stress: enums, structs, arrays, globals, recursion, switch/case,
// nested conditions, unary ops, pointer string traversal, complex control flow.

enum TokenType { T_NUM=0, T_PLUS=1, T_MINUS=2, T_MUL=3, T_DIV=4, T_LPAREN=5, T_RPAREN=6, T_EOF=7 };
enum NodeType  { N_NUM=0, N_ADD=1, N_SUB=2, N_MUL=3, N_DIV=4, N_NEG=5 };

struct Token { int type; int value; };
struct Node  { int type; int value; int left; int right; }; // left/right = indices, -1 = none

struct Token tokens[64];
int token_count;
struct Node nodes[128];
int node_count;
int pos;

int is_digit(char c) { return c >= '0' && c <= '9'; }
int is_space(char c) { return c == ' ' || c == '\t' || c == '\n'; }

// Tokenizer: walks string char-by-char, uses switch for operator dispatch
void tokenize(char *input) {
    int i;
    int tc;
    char c;
    tc = 0;
    i = 0;
    while (input[i] != '\0') {
        c = input[i];
        if (is_space(c)) {
            i = i + 1;
            continue;
        }
        if (is_digit(c)) {
            int val;
            val = 0;
            while (input[i] != '\0' && is_digit(input[i])) {
                val = val * 10 + (input[i] - '0');
                i = i + 1;
            }
            tokens[tc].type = T_NUM;
            tokens[tc].value = val;
            tc = tc + 1;
            continue;
        }
        switch (c) {
            case '+': tokens[tc].type = T_PLUS;   tokens[tc].value = 0; tc = tc + 1; break;
            case '-': tokens[tc].type = T_MINUS;  tokens[tc].value = 0; tc = tc + 1; break;
            case '*': tokens[tc].type = T_MUL;    tokens[tc].value = 0; tc = tc + 1; break;
            case '/': tokens[tc].type = T_DIV;    tokens[tc].value = 0; tc = tc + 1; break;
            case '(': tokens[tc].type = T_LPAREN; tokens[tc].value = 0; tc = tc + 1; break;
            case ')': tokens[tc].type = T_RPAREN; tokens[tc].value = 0; tc = tc + 1; break;
        }
        i = i + 1;
    }
    tokens[tc].type = T_EOF;
    tokens[tc].value = 0;
    tc = tc + 1;
    token_count = tc;
}

// Token name lookup via switch on enum - stresses enum + switch combo
char *token_name(int type) {
    switch (type) {
        case T_NUM:    return "NUM";
        case T_PLUS:   return "PLUS";
        case T_MINUS:  return "MINUS";
        case T_MUL:    return "MUL";
        case T_DIV:    return "DIV";
        case T_LPAREN: return "LPAREN";
        case T_RPAREN: return "RPAREN";
        case T_EOF:    return "EOF";
    }
    return "???";
}

// Print all tokens in a readable format
void print_tokens(char *input) {
    int i;
    printf("Tokenize \"%s\": [", input);
    i = 0;
    while (i < token_count - 1) {
        if (i > 0) printf(" ");
        if (tokens[i].type == T_NUM)
            printf("%s(%d)", token_name(tokens[i].type), tokens[i].value);
        else
            printf("%s", token_name(tokens[i].type));
        i = i + 1;
    }
    printf("] EOF count=%d\n", token_count);
}

// Parser helpers
int current_type()  { return tokens[pos].type; }
int current_value() { return tokens[pos].value; }
void advance() { if (pos < token_count - 1) pos = pos + 1; }

// AST construction
int make_num(int value) {
    int idx;
    idx = node_count;
    nodes[idx].type = N_NUM;
    nodes[idx].value = value;
    nodes[idx].left = -1;
    nodes[idx].right = -1;
    node_count = node_count + 1;
    return idx;
}

int make_binop(int op, int left, int right) {
    int idx;
    idx = node_count;
    nodes[idx].type = op;
    nodes[idx].value = 0;
    nodes[idx].left = left;
    nodes[idx].right = right;
    node_count = node_count + 1;
    return idx;
}

int make_unary(int op, int child) {
    int idx;
    idx = node_count;
    nodes[idx].type = op;
    nodes[idx].value = 0;
    nodes[idx].left = child;
    nodes[idx].right = -1;
    node_count = node_count + 1;
    return idx;
}

// Forward declarations for recursive descent
int parse_expr();
int parse_term();
int parse_unary();
int parse_primary();

// primary = NUM | '(' expr ')'
int parse_primary() {
    int tt;
    int val;
    int node_idx;
    tt = current_type();
    if (tt == T_NUM) {
        val = current_value();
        advance();
        return make_num(val);
    }
    if (tt == T_LPAREN) {
        advance();
        node_idx = parse_expr();
        if (current_type() == T_RPAREN) advance();
        return node_idx;
    }
    return make_num(0);
}

// unary = '-' unary | primary
int parse_unary() {
    int tt;
    int child;
    tt = current_type();
    if (tt == T_MINUS) {
        advance();
        child = parse_unary(); // recursive for nested unary
        return make_unary(N_NEG, child);
    }
    return parse_primary();
}

// term = unary (('*' | '/') unary)* -- uses switch with default return
int parse_term() {
    int left;
    int right;
    int tt;
    left = parse_unary();
    while (1) {
        tt = current_type();
        switch (tt) {
            case T_MUL:
                advance();
                right = parse_unary();
                left = make_binop(N_MUL, left, right);
                break;
            case T_DIV:
                advance();
                right = parse_unary();
                left = make_binop(N_DIV, left, right);
                break;
            default:
                return left;
        }
    }
    return left;
}

// expr = term (('+' | '-') term)* -- uses switch with default return
int parse_expr() {
    int left;
    int right;
    int tt;
    left = parse_term();
    while (1) {
        tt = current_type();
        switch (tt) {
            case T_PLUS:
                advance();
                right = parse_term();
                left = make_binop(N_ADD, left, right);
                break;
            case T_MINUS:
                advance();
                right = parse_term();
                left = make_binop(N_SUB, left, right);
                break;
            default:
                return left;
        }
    }
    return left;
}

// Recursive AST evaluator - switch on enum node type
int eval(int idx) {
    int t;
    int lv;
    int rv;
    if (idx < 0 || idx >= node_count) return 0;
    t = nodes[idx].type;
    switch (t) {
        case N_NUM:
            return nodes[idx].value;
        case N_NEG:
            lv = eval(nodes[idx].left);
            return -lv;
        case N_ADD:
            lv = eval(nodes[idx].left);
            rv = eval(nodes[idx].right);
            return lv + rv;
        case N_SUB:
            lv = eval(nodes[idx].left);
            rv = eval(nodes[idx].right);
            return lv - rv;
        case N_MUL:
            lv = eval(nodes[idx].left);
            rv = eval(nodes[idx].right);
            return lv * rv;
        case N_DIV:
            lv = eval(nodes[idx].left);
            rv = eval(nodes[idx].right);
            if (rv == 0) return 0;
            return lv / rv;
    }
    return 0;
}

// Recursive AST printer - stresses deep recursion with nested printf
void print_ast(int idx) {
    int t;
    if (idx < 0 || idx >= node_count) { printf("ERR"); return; }
    t = nodes[idx].type;
    switch (t) {
        case N_NUM:
            printf("NUM(%d)", nodes[idx].value);
            return;
        case N_NEG:
            printf("NEG(");
            print_ast(nodes[idx].left);
            printf(")");
            return;
        case N_ADD:
            printf("ADD(");
            print_ast(nodes[idx].left);
            printf(", ");
            print_ast(nodes[idx].right);
            printf(")");
            return;
        case N_SUB:
            printf("SUB(");
            print_ast(nodes[idx].left);
            printf(", ");
            print_ast(nodes[idx].right);
            printf(")");
            return;
        case N_MUL:
            printf("MUL(");
            print_ast(nodes[idx].left);
            printf(", ");
            print_ast(nodes[idx].right);
            printf(")");
            return;
        case N_DIV:
            printf("DIV(");
            print_ast(nodes[idx].left);
            printf(", ");
            print_ast(nodes[idx].right);
            printf(")");
            return;
    }
    printf("???");
}

// Reset all global state for a fresh parse
void reset() {
    int i;
    token_count = 0;
    node_count = 0;
    pos = 0;
    i = 0;
    while (i < 64) {
        tokens[i].type = 0;
        tokens[i].value = 0;
        i = i + 1;
    }
    i = 0;
    while (i < 128) {
        nodes[i].type = 0;
        nodes[i].value = 0;
        nodes[i].left = -1;
        nodes[i].right = -1;
        i = i + 1;
    }
}

// Parse and evaluate a string expression
int parse_and_eval(char *expr) {
    int root;
    int result;
    reset();
    tokenize(expr);
    pos = 0;
    root = parse_expr();
    result = eval(root);
    return result;
}

// Parse and dump the AST for an expression
int parse_and_print_ast(char *expr) {
    int root;
    reset();
    tokenize(expr);
    pos = 0;
    root = parse_expr();
    printf("AST for \"%s\": ", expr);
    print_ast(root);
    printf("\n");
    return root;
}

// Test runner globals
int total_passed;
int total_tests;

void run_eval_test(int test_num, char *expr, int expected) {
    int result;
    result = parse_and_eval(expr);
    total_tests = total_tests + 1;
    if (result == expected) {
        printf("Test %d: \"%s\" = %d [PASS]\n", test_num, expr, result);
        total_passed = total_passed + 1;
    } else {
        printf("Test %d: \"%s\" = %d (expected %d) [FAIL]\n",
               test_num, expr, result, expected);
    }
}

int main() {
    total_passed = 0;
    total_tests = 0;

    printf("=== Calculator Torture Test ===\n");

    // ---- Tokenizer Tests ----
    printf("--- Tokenizer Tests ---\n");
    reset(); tokenize("42");      print_tokens("42");
    reset(); tokenize("3+4");     print_tokens("3+4");
    reset(); tokenize("(2+3)*4"); print_tokens("(2+3)*4");
    reset(); tokenize("-5");      print_tokens("-5");

    // ---- AST Construction Tests ----
    printf("--- AST Construction Tests ---\n");
    parse_and_print_ast("42");
    parse_and_print_ast("3+4");
    parse_and_print_ast("3*4+2");
    parse_and_print_ast("2+3*4");

    // ---- Evaluation Tests (basic arithmetic + precedence) ----
    printf("--- Evaluation Tests ---\n");
    run_eval_test(1,  "42",           42);
    run_eval_test(2,  "3+4",          7);
    run_eval_test(3,  "10-3",         7);
    run_eval_test(4,  "3*4+2",        14);
    run_eval_test(5,  "2+3*4",        14);  // precedence
    run_eval_test(6,  "(2+3)*4",      20);
    run_eval_test(7,  "100/(2*5)",    10);
    run_eval_test(8,  "1+2+3+4+5",   15);
    run_eval_test(9,  "(10-3)*(4+2)", 42);
    run_eval_test(10, "2*3+4*5",      26);

    // ---- Unary Minus Tests ----
    printf("--- Unary Minus Tests ---\n");
    run_eval_test(11, "-5",       -5);
    run_eval_test(12, "-3+8",     5);
    run_eval_test(13, "-(2+3)",   -5);
    run_eval_test(14, "10*-2",    -20);

    // ---- Complex Expression Tests ----
    printf("--- Complex Expression Tests ---\n");
    run_eval_test(15, "((((1+2))))",       3);
    run_eval_test(16, "100-50-25-10",      15);
    run_eval_test(17, "2*3*4*5",           120);
    run_eval_test(18, "1000/10/10",        10);
    run_eval_test(19, "(1+2)*(3+4)*(1+1)", 42);
    run_eval_test(20, "50+25*2-10/5",      98);

    // ---- Node Pool Stats ----
    printf("--- Node Pool Stats ---\n");
    printf("Total nodes allocated: varies\n");
    if (node_count < 128)
        printf("Node pool not exhausted: OK\n");
    else
        printf("Node pool EXHAUSTED!\n");

    // ---- Summary ----
    printf("--- Summary ---\n");
    printf("Passed: %d/%d\n", total_passed, total_tests);
    if (total_passed == total_tests)
        printf("=== All torture tests passed ===\n");
    else
        printf("=== SOME TESTS FAILED ===\n");

    return 0;
}
