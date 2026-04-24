int printf(const char *fmt, ...);

// Simulate compiler phases: tokenize, parse (build tree), type check, evaluate

enum {
    TOK_NUM = 1,
    TOK_PLUS = 2,
    TOK_MINUS = 3,
    TOK_MUL = 4,
    TOK_DIV = 5,
    TOK_LPAREN = 6,
    TOK_RPAREN = 7,
    TOK_EOF = 8
};

struct Token {
    int kind;
    int value;
};

struct TokenStream {
    struct Token tokens[32];
    int count;
    int pos;
};

// AST nodes
enum {
    NODE_NUM = 1,
    NODE_ADD = 2,
    NODE_SUB = 3,
    NODE_MUL = 4,
    NODE_DIV = 5,
    NODE_NEG = 6
};

struct ASTNode {
    int kind;
    int value;
    int left;    // index into node pool
    int right;   // index into node pool
    int type_ok; // type check result
};

struct NodePool {
    struct ASTNode nodes[32];
    int count;
};

int alloc_node(struct NodePool *pool, int kind, int value, int left, int right) {
    int idx = pool->count;
    pool->nodes[idx].kind = kind;
    pool->nodes[idx].value = value;
    pool->nodes[idx].left = left;
    pool->nodes[idx].right = right;
    pool->nodes[idx].type_ok = 0;
    pool->count = pool->count + 1;
    return idx;
}

// Tokenizer: tokenize a simple expression encoded as int array
// Convention: positive numbers = literal, -1='+', -2='-', -3='*', -4='/', -5='(', -6=')', -7=EOF
void tokenize(int *input, int len, struct TokenStream *ts) {
    ts->count = 0;
    ts->pos = 0;
    int i;
    for (i = 0; i < len; i++) {
        int v = input[i];
        if (v >= 0) {
            ts->tokens[ts->count].kind = TOK_NUM;
            ts->tokens[ts->count].value = v;
        } else if (v == -1) {
            ts->tokens[ts->count].kind = TOK_PLUS;
            ts->tokens[ts->count].value = 0;
        } else if (v == -2) {
            ts->tokens[ts->count].kind = TOK_MINUS;
            ts->tokens[ts->count].value = 0;
        } else if (v == -3) {
            ts->tokens[ts->count].kind = TOK_MUL;
            ts->tokens[ts->count].value = 0;
        } else if (v == -4) {
            ts->tokens[ts->count].kind = TOK_DIV;
            ts->tokens[ts->count].value = 0;
        } else if (v == -5) {
            ts->tokens[ts->count].kind = TOK_LPAREN;
            ts->tokens[ts->count].value = 0;
        } else if (v == -6) {
            ts->tokens[ts->count].kind = TOK_RPAREN;
            ts->tokens[ts->count].value = 0;
        } else {
            ts->tokens[ts->count].kind = TOK_EOF;
            ts->tokens[ts->count].value = 0;
        }
        ts->count = ts->count + 1;
    }
}

struct Token peek(struct TokenStream *ts) {
    return ts->tokens[ts->pos];
}

struct Token advance(struct TokenStream *ts) {
    struct Token t = ts->tokens[ts->pos];
    if (ts->pos < ts->count - 1) ts->pos = ts->pos + 1;
    return t;
}

// Recursive descent parser
int parse_expr(struct TokenStream *ts, struct NodePool *pool);
int parse_term(struct TokenStream *ts, struct NodePool *pool);
int parse_factor(struct TokenStream *ts, struct NodePool *pool);

int parse_factor(struct TokenStream *ts, struct NodePool *pool) {
    struct Token t = peek(ts);
    if (t.kind == TOK_NUM) {
        advance(ts);
        return alloc_node(pool, NODE_NUM, t.value, -1, -1);
    }
    if (t.kind == TOK_MINUS) {
        advance(ts);
        int child = parse_factor(ts, pool);
        return alloc_node(pool, NODE_NEG, 0, child, -1);
    }
    if (t.kind == TOK_LPAREN) {
        advance(ts);
        int node = parse_expr(ts, pool);
        advance(ts); // consume RPAREN
        return node;
    }
    return alloc_node(pool, NODE_NUM, 0, -1, -1);
}

int parse_term(struct TokenStream *ts, struct NodePool *pool) {
    int left = parse_factor(ts, pool);
    while (peek(ts).kind == TOK_MUL || peek(ts).kind == TOK_DIV) {
        struct Token op = advance(ts);
        int right = parse_factor(ts, pool);
        if (op.kind == TOK_MUL) {
            left = alloc_node(pool, NODE_MUL, 0, left, right);
        } else {
            left = alloc_node(pool, NODE_DIV, 0, left, right);
        }
    }
    return left;
}

int parse_expr(struct TokenStream *ts, struct NodePool *pool) {
    int left = parse_term(ts, pool);
    while (peek(ts).kind == TOK_PLUS || peek(ts).kind == TOK_MINUS) {
        struct Token op = advance(ts);
        int right = parse_term(ts, pool);
        if (op.kind == TOK_PLUS) {
            left = alloc_node(pool, NODE_ADD, 0, left, right);
        } else {
            left = alloc_node(pool, NODE_SUB, 0, left, right);
        }
    }
    return left;
}

// Type checker (simple: all nodes type-check to int)
int type_check(struct NodePool *pool, int idx) {
    if (idx < 0) return 1;
    struct ASTNode *n = &pool->nodes[idx];
    int lok = type_check(pool, n->left);
    int rok = type_check(pool, n->right);
    if (n->kind == NODE_DIV && n->right >= 0) {
        // Check for division by zero at compile time
        if (pool->nodes[n->right].kind == NODE_NUM && pool->nodes[n->right].value == 0) {
            n->type_ok = 0;
            return 0;
        }
    }
    n->type_ok = lok && rok;
    return n->type_ok;
}

// Evaluator
int evaluate(struct NodePool *pool, int idx) {
    if (idx < 0) return 0;
    struct ASTNode *n = &pool->nodes[idx];
    if (n->kind == NODE_NUM) return n->value;
    if (n->kind == NODE_NEG) return -evaluate(pool, n->left);
    int lv = evaluate(pool, n->left);
    int rv = evaluate(pool, n->right);
    if (n->kind == NODE_ADD) return lv + rv;
    if (n->kind == NODE_SUB) return lv - rv;
    if (n->kind == NODE_MUL) return lv * rv;
    if (n->kind == NODE_DIV) {
        if (rv == 0) return 0;
        return lv / rv;
    }
    return 0;
}

int count_nodes(struct NodePool *pool) {
    return pool->count;
}

int main(void) {
    struct TokenStream ts;
    struct NodePool pool;
    int input[16];
    int root;
    int result;

    // Expression: 3 + 4 * 2    encoded as: 3 + 4 * 2 EOF
    input[0] = 3; input[1] = -1; input[2] = 4; input[3] = -3;
    input[4] = 2; input[5] = -7;
    tokenize(input, 6, &ts);

    // EXPECT: Tokens: 6
    printf("Tokens: %d\n", ts.count);

    // EXPECT: Token 0: kind=1 val=3
    printf("Token 0: kind=%d val=%d\n", ts.tokens[0].kind, ts.tokens[0].value);

    // EXPECT: Token 1: kind=2 val=0
    printf("Token 1: kind=%d val=%d\n", ts.tokens[1].kind, ts.tokens[1].value);

    pool.count = 0;
    root = parse_expr(&ts, &pool);
    // EXPECT: AST nodes: 5
    printf("AST nodes: %d\n", pool.count);

    // EXPECT: Root kind=2
    printf("Root kind=%d\n", pool.nodes[root].kind);

    type_check(&pool, root);
    // EXPECT: Type check ok=1
    printf("Type check ok=%d\n", pool.nodes[root].type_ok);

    result = evaluate(&pool, root);
    // EXPECT: 3+4*2 = 11
    printf("3+4*2 = %d\n", result);

    // Expression: (3 + 4) * 2  encoded as: ( 3 + 4 ) * 2 EOF
    input[0] = -5; input[1] = 3; input[2] = -1; input[3] = 4;
    input[4] = -6; input[5] = -3; input[6] = 2; input[7] = -7;
    tokenize(input, 8, &ts);
    pool.count = 0;
    root = parse_expr(&ts, &pool);
    result = evaluate(&pool, root);
    // EXPECT: (3+4)*2 = 14
    printf("(3+4)*2 = %d\n", result);

    // Expression: 10 - 3 - 2  (left-associative)
    input[0] = 10; input[1] = -2; input[2] = 3; input[3] = -2;
    input[4] = 2; input[5] = -7;
    tokenize(input, 6, &ts);
    pool.count = 0;
    root = parse_expr(&ts, &pool);
    result = evaluate(&pool, root);
    // EXPECT: 10-3-2 = 5
    printf("10-3-2 = %d\n", result);

    // Expression: 100 / 5 / 4
    input[0] = 100; input[1] = -4; input[2] = 5; input[3] = -4;
    input[4] = 4; input[5] = -7;
    tokenize(input, 6, &ts);
    pool.count = 0;
    root = parse_expr(&ts, &pool);
    result = evaluate(&pool, root);
    // EXPECT: 100/5/4 = 5
    printf("100/5/4 = %d\n", result);

    // Expression with negation: -5 + 8
    input[0] = -2; input[1] = 5; input[2] = -1; input[3] = 8;
    input[4] = -7;
    tokenize(input, 5, &ts);
    pool.count = 0;
    root = parse_expr(&ts, &pool);
    result = evaluate(&pool, root);
    // EXPECT: -5+8 = 3
    printf("-5+8 = %d\n", result);

    // Division by zero detection in type checker
    input[0] = 10; input[1] = -4; input[2] = 0; input[3] = -7;
    tokenize(input, 4, &ts);
    pool.count = 0;
    root = parse_expr(&ts, &pool);
    int tc = type_check(&pool, root);
    // EXPECT: Div by zero type_ok=0
    printf("Div by zero type_ok=%d\n", tc);

    // Complex: (2 + 3) * (4 - 1)
    input[0] = -5; input[1] = 2; input[2] = -1; input[3] = 3;
    input[4] = -6; input[5] = -3; input[6] = -5; input[7] = 4;
    input[8] = -2; input[9] = 1; input[10] = -6; input[11] = -7;
    tokenize(input, 12, &ts);
    pool.count = 0;
    root = parse_expr(&ts, &pool);
    result = evaluate(&pool, root);
    // EXPECT: (2+3)*(4-1) = 15
    printf("(2+3)*(4-1) = %d\n", result);

    // EXPECT: AST node count: 7
    printf("AST node count: %d\n", pool.count);

    // Single number
    input[0] = 42; input[1] = -7;
    tokenize(input, 2, &ts);
    pool.count = 0;
    root = parse_expr(&ts, &pool);
    result = evaluate(&pool, root);
    // EXPECT: Single 42 = 42
    printf("Single 42 = %d\n", result);

    // EXPECT: Compiler phases complete
    printf("Compiler phases complete\n");

    return 0;
}
