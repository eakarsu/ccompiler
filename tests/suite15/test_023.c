int printf(const char *fmt, ...);

// AST construction and evaluation
// Build explicit AST nodes and then walk them to evaluate

enum NodeKind {
    NODE_NUM,
    NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV,
    NODE_NEG
};

struct ASTNode {
    int kind;
    int value;      // for NODE_NUM
    int left_idx;   // index into node pool
    int right_idx;  // index into node pool
};

struct ASTPool {
    struct ASTNode nodes[64];
    int count;
};

int make_num(struct ASTPool *pool, int val) {
    int idx = pool->count;
    pool->nodes[idx].kind = NODE_NUM;
    pool->nodes[idx].value = val;
    pool->nodes[idx].left_idx = -1;
    pool->nodes[idx].right_idx = -1;
    pool->count = pool->count + 1;
    return idx;
}

int make_binop(struct ASTPool *pool, int kind, int left, int right) {
    int idx = pool->count;
    pool->nodes[idx].kind = kind;
    pool->nodes[idx].value = 0;
    pool->nodes[idx].left_idx = left;
    pool->nodes[idx].right_idx = right;
    pool->count = pool->count + 1;
    return idx;
}

int make_neg(struct ASTPool *pool, int child) {
    int idx = pool->count;
    pool->nodes[idx].kind = NODE_NEG;
    pool->nodes[idx].value = 0;
    pool->nodes[idx].left_idx = child;
    pool->nodes[idx].right_idx = -1;
    pool->count = pool->count + 1;
    return idx;
}

int eval_ast(struct ASTPool *pool, int idx) {
    struct ASTNode *n = &pool->nodes[idx];
    if (n->kind == NODE_NUM) return n->value;
    if (n->kind == NODE_NEG) return -eval_ast(pool, n->left_idx);

    int lv = eval_ast(pool, n->left_idx);
    int rv = eval_ast(pool, n->right_idx);

    if (n->kind == NODE_ADD) return lv + rv;
    if (n->kind == NODE_SUB) return lv - rv;
    if (n->kind == NODE_MUL) return lv * rv;
    if (n->kind == NODE_DIV) return lv / rv;
    return 0;
}

int count_nodes(struct ASTPool *pool, int idx) {
    if (idx < 0) return 0;
    struct ASTNode *n = &pool->nodes[idx];
    if (n->kind == NODE_NUM) return 1;
    if (n->kind == NODE_NEG) return 1 + count_nodes(pool, n->left_idx);
    return 1 + count_nodes(pool, n->left_idx) + count_nodes(pool, n->right_idx);
}

int tree_depth(struct ASTPool *pool, int idx) {
    if (idx < 0) return 0;
    struct ASTNode *n = &pool->nodes[idx];
    if (n->kind == NODE_NUM) return 1;
    if (n->kind == NODE_NEG) return 1 + tree_depth(pool, n->left_idx);
    int ld = tree_depth(pool, n->left_idx);
    int rd = tree_depth(pool, n->right_idx);
    if (ld > rd) return 1 + ld;
    return 1 + rd;
}

char *kind_name(int kind) {
    if (kind == NODE_NUM) return "NUM";
    if (kind == NODE_ADD) return "ADD";
    if (kind == NODE_SUB) return "SUB";
    if (kind == NODE_MUL) return "MUL";
    if (kind == NODE_DIV) return "DIV";
    if (kind == NODE_NEG) return "NEG";
    return "???";
}

void print_ast(struct ASTPool *pool, int idx, int depth) {
    if (idx < 0) return;
    struct ASTNode *n = &pool->nodes[idx];

    int i;
    for (i = 0; i < depth; i++) printf("  ");

    if (n->kind == NODE_NUM) {
        printf("%s(%d)\n", kind_name(n->kind), n->value);
    } else {
        printf("%s\n", kind_name(n->kind));
    }

    if (n->left_idx >= 0) print_ast(pool, n->left_idx, depth + 1);
    if (n->right_idx >= 0) print_ast(pool, n->right_idx, depth + 1);
}

int main() {
    struct ASTPool pool;
    pool.count = 0;

    // Build AST for: (3 + 4) * 2
    printf("=== AST Test 1: (3+4)*2 ===\n");
    // EXPECT: === AST Test 1: (3+4)*2 ===
    int n3 = make_num(&pool, 3);
    int n4 = make_num(&pool, 4);
    int add1 = make_binop(&pool, NODE_ADD, n3, n4);
    int n2 = make_num(&pool, 2);
    int mul1 = make_binop(&pool, NODE_MUL, add1, n2);

    printf("result: %d\n", eval_ast(&pool, mul1));
    // EXPECT: result: 14
    printf("nodes: %d\n", count_nodes(&pool, mul1));
    // EXPECT: nodes: 5
    printf("depth: %d\n", tree_depth(&pool, mul1));
    // EXPECT: depth: 3
    print_ast(&pool, mul1, 0);
    // EXPECT: MUL
    // EXPECT:   ADD
    // EXPECT:     NUM(3)
    // EXPECT:     NUM(4)
    // EXPECT:   NUM(2)

    // Build AST for: 10 - (2 * 3)
    printf("=== AST Test 2: 10-(2*3) ===\n");
    // EXPECT: === AST Test 2: 10-(2*3) ===
    int n10 = make_num(&pool, 10);
    int n2b = make_num(&pool, 2);
    int n3b = make_num(&pool, 3);
    int mul2 = make_binop(&pool, NODE_MUL, n2b, n3b);
    int sub1 = make_binop(&pool, NODE_SUB, n10, mul2);

    printf("result: %d\n", eval_ast(&pool, sub1));
    // EXPECT: result: 4
    printf("nodes: %d\n", count_nodes(&pool, sub1));
    // EXPECT: nodes: 5
    printf("depth: %d\n", tree_depth(&pool, sub1));
    // EXPECT: depth: 3

    // Build AST for: -(5 + 3)
    printf("=== AST Test 3: -(5+3) ===\n");
    // EXPECT: === AST Test 3: -(5+3) ===
    int n5 = make_num(&pool, 5);
    int n3c = make_num(&pool, 3);
    int add2 = make_binop(&pool, NODE_ADD, n5, n3c);
    int neg1 = make_neg(&pool, add2);

    printf("result: %d\n", eval_ast(&pool, neg1));
    // EXPECT: result: -8
    printf("nodes: %d\n", count_nodes(&pool, neg1));
    // EXPECT: nodes: 4
    printf("depth: %d\n", tree_depth(&pool, neg1));
    // EXPECT: depth: 3

    // Build AST for: (1 + 2) + (3 + 4)
    printf("=== AST Test 4: (1+2)+(3+4) ===\n");
    // EXPECT: === AST Test 4: (1+2)+(3+4) ===
    int a1 = make_num(&pool, 1);
    int a2 = make_num(&pool, 2);
    int a3 = make_num(&pool, 3);
    int a4 = make_num(&pool, 4);
    int left = make_binop(&pool, NODE_ADD, a1, a2);
    int right = make_binop(&pool, NODE_ADD, a3, a4);
    int root = make_binop(&pool, NODE_ADD, left, right);

    printf("result: %d\n", eval_ast(&pool, root));
    // EXPECT: result: 10
    printf("nodes: %d\n", count_nodes(&pool, root));
    // EXPECT: nodes: 7
    printf("depth: %d\n", tree_depth(&pool, root));
    // EXPECT: depth: 3

    // Build AST for: 100 / (5 * 2) - 3
    printf("=== AST Test 5: 100/(5*2)-3 ===\n");
    // EXPECT: === AST Test 5: 100/(5*2)-3 ===
    int b100 = make_num(&pool, 100);
    int b5 = make_num(&pool, 5);
    int b2 = make_num(&pool, 2);
    int b3 = make_num(&pool, 3);
    int bmul = make_binop(&pool, NODE_MUL, b5, b2);
    int bdiv = make_binop(&pool, NODE_DIV, b100, bmul);
    int bsub = make_binop(&pool, NODE_SUB, bdiv, b3);

    printf("result: %d\n", eval_ast(&pool, bsub));
    // EXPECT: result: 7
    printf("total pool nodes: %d\n", pool.count);
    // EXPECT: total pool nodes: 28

    printf("done\n");
    // EXPECT: done
    return 0;
}
