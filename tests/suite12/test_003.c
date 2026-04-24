int printf(const char *fmt, ...);

// AST builder and evaluator
// Builds an explicit AST in a node pool, then walks it to evaluate

int ND_NUM = 1;
int ND_ADD = 2;
int ND_SUB = 3;
int ND_MUL = 4;
int ND_DIV = 5;
int ND_NEG = 6;
int ND_VAR = 7;
int ND_ASSIGN = 8;
int ND_STMTS = 9;

struct ASTNode {
    int kind;
    int value;
    int left;
    int right;
    int var_idx;
};

struct ASTNode pool[256];
int pool_count;

int vars[26];

int new_node(int kind, int value, int left, int right) {
    int idx = pool_count;
    pool[idx].kind = kind;
    pool[idx].value = value;
    pool[idx].left = left;
    pool[idx].right = right;
    pool[idx].var_idx = 0;
    pool_count = pool_count + 1;
    return idx;
}

int new_num(int val) {
    return new_node(ND_NUM, val, -1, -1);
}

int new_binop(int kind, int left, int right) {
    return new_node(kind, 0, left, right);
}

int new_neg(int child) {
    return new_node(ND_NEG, 0, child, -1);
}

int new_var(int idx) {
    int n = new_node(ND_VAR, 0, -1, -1);
    pool[n].var_idx = idx;
    return n;
}

int new_assign(int var_idx, int expr) {
    int n = new_node(ND_ASSIGN, 0, expr, -1);
    pool[n].var_idx = var_idx;
    return n;
}

int new_stmts(int s1, int s2) {
    return new_node(ND_STMTS, 0, s1, s2);
}

int eval_ast(int idx) {
    if (idx < 0) return 0;
    struct ASTNode node = pool[idx];

    if (node.kind == ND_NUM) return node.value;
    if (node.kind == ND_VAR) return vars[node.var_idx];
    if (node.kind == ND_NEG) return -eval_ast(node.left);

    if (node.kind == ND_ASSIGN) {
        int val = eval_ast(node.left);
        vars[node.var_idx] = val;
        return val;
    }

    if (node.kind == ND_STMTS) {
        eval_ast(node.left);
        return eval_ast(node.right);
    }

    int lv = eval_ast(node.left);
    int rv = eval_ast(node.right);

    if (node.kind == ND_ADD) return lv + rv;
    if (node.kind == ND_SUB) return lv - rv;
    if (node.kind == ND_MUL) return lv * rv;
    if (node.kind == ND_DIV) {
        if (rv == 0) return 0;
        return lv / rv;
    }
    return 0;
}

int count_nodes(int idx) {
    if (idx < 0) return 0;
    struct ASTNode node = pool[idx];
    return 1 + count_nodes(node.left) + count_nodes(node.right);
}

int tree_depth(int idx) {
    if (idx < 0) return 0;
    struct ASTNode node = pool[idx];
    int ld = count_nodes(node.left);
    int rd = count_nodes(node.right);
    int dl = tree_depth(node.left);
    int dr = tree_depth(node.right);
    int mx = dl;
    if (dr > mx) mx = dr;
    return 1 + mx;
}

char *kind_name(int kind) {
    if (kind == ND_NUM) return "NUM";
    if (kind == ND_ADD) return "ADD";
    if (kind == ND_SUB) return "SUB";
    if (kind == ND_MUL) return "MUL";
    if (kind == ND_DIV) return "DIV";
    if (kind == ND_NEG) return "NEG";
    if (kind == ND_VAR) return "VAR";
    if (kind == ND_ASSIGN) return "ASSIGN";
    if (kind == ND_STMTS) return "STMTS";
    return "UNKNOWN";
}

int main() {
    pool_count = 0;
    int i = 0;
    while (i < 26) { vars[i] = 0; i = i + 1; }

    // Build: (3 + 4) * 2
    int n3 = new_num(3);
    int n4 = new_num(4);
    int add = new_binop(ND_ADD, n3, n4);
    int n2 = new_num(2);
    int mul = new_binop(ND_MUL, add, n2);

    int result = eval_ast(mul);
    printf("(3+4)*2 = %d\n", result);
    // EXPECT: (3+4)*2 = 14

    int nc = count_nodes(mul);
    printf("node count: %d\n", nc);
    // EXPECT: node count: 5

    int d = tree_depth(mul);
    printf("depth: %d\n", d);
    // EXPECT: depth: 3

    printf("root kind: %s\n", kind_name(pool[mul].kind));
    // EXPECT: root kind: MUL

    // Build: -(10 - 3)
    int n10 = new_num(10);
    int n3b = new_num(3);
    int sub = new_binop(ND_SUB, n10, n3b);
    int neg = new_neg(sub);
    result = eval_ast(neg);
    printf("-(10-3) = %d\n", result);
    // EXPECT: -(10-3) = -7

    // Build: x = 5; y = x + 10; result = y * 2
    int assign_x = new_assign(0, new_num(5));
    int var_x = new_var(0);
    int add_xy = new_binop(ND_ADD, var_x, new_num(10));
    int assign_y = new_assign(1, add_xy);
    int var_y = new_var(1);
    int mul_y2 = new_binop(ND_MUL, var_y, new_num(2));
    int assign_r = new_assign(2, mul_y2);

    int prog = new_stmts(assign_x, new_stmts(assign_y, assign_r));
    eval_ast(prog);

    printf("x = %d\n", vars[0]);
    // EXPECT: x = 5
    printf("y = %d\n", vars[1]);
    // EXPECT: y = 15
    printf("r = %d\n", vars[2]);
    // EXPECT: r = 30

    // 100 / (5 * 2)
    int div_expr = new_binop(ND_DIV, new_num(100), new_binop(ND_MUL, new_num(5), new_num(2)));
    result = eval_ast(div_expr);
    printf("100/(5*2) = %d\n", result);
    // EXPECT: 100/(5*2) = 10

    printf("total pool: %d\n", pool_count);
    // EXPECT: total pool: 26

    printf("ast done\n");
    // EXPECT: ast done

    return 0;
}
