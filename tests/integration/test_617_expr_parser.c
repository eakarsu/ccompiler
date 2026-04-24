int printf(const char *fmt, ...);
// EXPECT: 3+4 = 7\n3+4*5 = 23\n10-2-3 = 5\n(2+3)*4 = 20\n100/(5*2) = 10\n1+2*3+4 = 11\nAll 6 expression tests passed!\n

// Test 617: Recursive descent expression parser with AST construction and evaluation
// Stress test for: structs, arrays, globals, recursion, function pointers (none here),
// switch/case, loops, pointer arithmetic, and complex control flow.

// ========== Token types ==========
int TOK_NUM    = 0;
int TOK_PLUS   = 1;
int TOK_MINUS  = 2;
int TOK_STAR   = 3;
int TOK_SLASH  = 4;
int TOK_LPAREN = 5;
int TOK_RPAREN = 6;
int TOK_EOF    = 7;

// ========== AST node types ==========
int NODE_NUM   = 0;
int NODE_ADD   = 1;
int NODE_SUB   = 2;
int NODE_MUL   = 3;
int NODE_DIV   = 4;

// ========== Token struct ==========
struct Token {
    int type;
    int value;
};

// ========== AST Node struct ==========
// left and right are indices into the node pool (-1 means none)
struct Node {
    int type;
    int value;
    int left;
    int right;
};

// ========== Global state ==========
// Token storage
struct Token tokens[64];
int token_count;

// Node pool for AST
struct Node nodes[128];
int node_count;

// Parser state: current position in token array
int pos;

// ========== Tokenizer ==========
// Returns 1 if c is a digit character
int is_digit(int c) {
    return c >= 48 && c <= 57;  // '0' = 48, '9' = 57
}

// Returns 1 if c is a whitespace character
int is_space(int c) {
    return c == 32 || c == 9 || c == 10 || c == 13;
}

// Tokenize the input string into the global tokens array
// Returns number of tokens produced
int tokenize(char *input) {
    int i;
    int len;
    int tc;
    char c;

    // Compute string length
    len = 0;
    while (input[len] != 0) {
        len = len + 1;
    }

    tc = 0;
    i = 0;
    while (i < len) {
        c = input[i];

        // Skip whitespace
        if (is_space(c)) {
            i = i + 1;
            continue;
        }

        // Number: accumulate digits
        if (is_digit(c)) {
            int val;
            val = 0;
            while (i < len && is_digit(input[i])) {
                val = val * 10 + (input[i] - 48);
                i = i + 1;
            }
            tokens[tc].type = TOK_NUM;
            tokens[tc].value = val;
            tc = tc + 1;
            continue;
        }

        // Single-character tokens
        if (c == 43) {  // '+'
            tokens[tc].type = TOK_PLUS;
            tokens[tc].value = 0;
            tc = tc + 1;
        } else if (c == 45) {  // '-'
            tokens[tc].type = TOK_MINUS;
            tokens[tc].value = 0;
            tc = tc + 1;
        } else if (c == 42) {  // '*'
            tokens[tc].type = TOK_STAR;
            tokens[tc].value = 0;
            tc = tc + 1;
        } else if (c == 47) {  // '/'
            tokens[tc].type = TOK_SLASH;
            tokens[tc].value = 0;
            tc = tc + 1;
        } else if (c == 40) {  // '('
            tokens[tc].type = TOK_LPAREN;
            tokens[tc].value = 0;
            tc = tc + 1;
        } else if (c == 41) {  // ')'
            tokens[tc].type = TOK_RPAREN;
            tokens[tc].value = 0;
            tc = tc + 1;
        }

        i = i + 1;
    }

    // Append EOF token
    tokens[tc].type = TOK_EOF;
    tokens[tc].value = 0;
    tc = tc + 1;

    token_count = tc;
    return tc;
}

// ========== Token access helpers ==========

// Return the type of the current token
int current_token_type() {
    return tokens[pos].type;
}

// Return the value of the current token
int current_token_value() {
    return tokens[pos].value;
}

// Advance to the next token
void advance() {
    if (pos < token_count - 1) {
        pos = pos + 1;
    }
}

// ========== AST construction helpers ==========

// Create a number node, return its index in the pool
int make_num_node(int value) {
    int idx;
    idx = node_count;
    nodes[idx].type = NODE_NUM;
    nodes[idx].value = value;
    nodes[idx].left = -1;
    nodes[idx].right = -1;
    node_count = node_count + 1;
    return idx;
}

// Create a binary operator node, return its index
int make_binop_node(int op_type, int left_idx, int right_idx) {
    int idx;
    idx = node_count;
    nodes[idx].type = op_type;
    nodes[idx].value = 0;
    nodes[idx].left = left_idx;
    nodes[idx].right = right_idx;
    node_count = node_count + 1;
    return idx;
}

// ========== Recursive descent parser ==========

// Forward declarations
int parse_expr();
int parse_term();
int parse_factor();

// parse_factor: NUM | '(' expr ')'
int parse_factor() {
    int tt;
    int val;
    int node_idx;

    tt = current_token_type();

    if (tt == TOK_NUM) {
        val = current_token_value();
        advance();
        return make_num_node(val);
    }

    if (tt == TOK_LPAREN) {
        advance();  // consume '('
        node_idx = parse_expr();
        // expect ')'
        if (current_token_type() == TOK_RPAREN) {
            advance();  // consume ')'
        }
        return node_idx;
    }

    // Error case: unexpected token, return a zero node
    return make_num_node(0);
}

// parse_term: factor (('*' | '/') factor)*
// Left-associative
int parse_term() {
    int left;
    int right;
    int tt;

    left = parse_factor();

    while (1) {
        tt = current_token_type();
        if (tt == TOK_STAR) {
            advance();
            right = parse_factor();
            left = make_binop_node(NODE_MUL, left, right);
        } else if (tt == TOK_SLASH) {
            advance();
            right = parse_factor();
            left = make_binop_node(NODE_DIV, left, right);
        } else {
            break;
        }
    }

    return left;
}

// parse_expr: term (('+' | '-') term)*
// Left-associative
int parse_expr() {
    int left;
    int right;
    int tt;

    left = parse_term();

    while (1) {
        tt = current_token_type();
        if (tt == TOK_PLUS) {
            advance();
            right = parse_term();
            left = make_binop_node(NODE_ADD, left, right);
        } else if (tt == TOK_MINUS) {
            advance();
            right = parse_term();
            left = make_binop_node(NODE_SUB, left, right);
        } else {
            break;
        }
    }

    return left;
}

// ========== AST evaluator ==========

// Recursively evaluate the AST rooted at nodes[idx]
int eval(int idx) {
    int t;
    int lv;
    int rv;

    if (idx < 0 || idx >= node_count) {
        return 0;
    }

    t = nodes[idx].type;

    if (t == NODE_NUM) {
        return nodes[idx].value;
    }

    lv = eval(nodes[idx].left);
    rv = eval(nodes[idx].right);

    if (t == NODE_ADD) {
        return lv + rv;
    }
    if (t == NODE_SUB) {
        return lv - rv;
    }
    if (t == NODE_MUL) {
        return lv * rv;
    }
    if (t == NODE_DIV) {
        if (rv == 0) {
            return 0;  // avoid division by zero
        }
        return lv / rv;
    }

    return 0;
}

// ========== Reset state for a new expression ==========

void reset_state() {
    int i;
    token_count = 0;
    node_count = 0;
    pos = 0;
    // Clear token array
    i = 0;
    while (i < 64) {
        tokens[i].type = 0;
        tokens[i].value = 0;
        i = i + 1;
    }
    // Clear node pool
    i = 0;
    while (i < 128) {
        nodes[i].type = 0;
        nodes[i].value = 0;
        nodes[i].left = -1;
        nodes[i].right = -1;
        i = i + 1;
    }
}

// ========== Test harness ==========

// Parse and evaluate an expression string, return the result
int parse_and_eval(char *expr) {
    int root;
    int result;

    reset_state();
    tokenize(expr);
    pos = 0;
    root = parse_expr();
    result = eval(root);
    return result;
}

// Run a single test case: parse, eval, check result
// Returns 1 if passed, 0 if failed
int run_test(char *expr, int expected) {
    int result;
    result = parse_and_eval(expr);
    printf("%s = %d\n", expr, result);
    if (result == expected) {
        return 1;
    }
    return 0;
}

// ========== Main ==========

int main() {
    int passed;
    int total;

    passed = 0;
    total = 6;

    // Test 1: Simple addition
    passed = passed + run_test("3+4", 7);

    // Test 2: Precedence - multiplication before addition
    passed = passed + run_test("3+4*5", 23);

    // Test 3: Left-associativity of subtraction
    passed = passed + run_test("10-2-3", 5);

    // Test 4: Parenthesized expression
    passed = passed + run_test("(2+3)*4", 20);

    // Test 5: Division with parenthesized sub-expression
    passed = passed + run_test("100/(5*2)", 10);

    // Test 6: Mixed operators
    passed = passed + run_test("1+2*3+4", 11);

    if (passed == total) {
        printf("All %d expression tests passed!\n", total);
    } else {
        printf("FAILED: %d/%d tests passed\n", passed, total);
    }

    return 0;
}
