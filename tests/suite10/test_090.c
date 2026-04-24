int printf(const char *fmt, ...);

// Visitor Pattern: dispatch table for operations on different node types

enum NodeType { NODE_INT, NODE_ADD, NODE_MUL, NODE_NEG };

struct ASTNode {
    enum NodeType type;
    int int_val;
    int left_idx;
    int right_idx;
};

struct ASTPool {
    struct ASTNode nodes[32];
    int count;
};

void pool_init(struct ASTPool *p) {
    p->count = 0;
}

int make_int(struct ASTPool *p, int val) {
    int idx;
    idx = p->count;
    p->nodes[idx].type = NODE_INT;
    p->nodes[idx].int_val = val;
    p->nodes[idx].left_idx = -1;
    p->nodes[idx].right_idx = -1;
    p->count = p->count + 1;
    return idx;
}

int make_add(struct ASTPool *p, int left, int right) {
    int idx;
    idx = p->count;
    p->nodes[idx].type = NODE_ADD;
    p->nodes[idx].int_val = 0;
    p->nodes[idx].left_idx = left;
    p->nodes[idx].right_idx = right;
    p->count = p->count + 1;
    return idx;
}

int make_mul(struct ASTPool *p, int left, int right) {
    int idx;
    idx = p->count;
    p->nodes[idx].type = NODE_MUL;
    p->nodes[idx].int_val = 0;
    p->nodes[idx].left_idx = left;
    p->nodes[idx].right_idx = right;
    p->count = p->count + 1;
    return idx;
}

int make_neg(struct ASTPool *p, int child) {
    int idx;
    idx = p->count;
    p->nodes[idx].type = NODE_NEG;
    p->nodes[idx].int_val = 0;
    p->nodes[idx].left_idx = child;
    p->nodes[idx].right_idx = -1;
    p->count = p->count + 1;
    return idx;
}

typedef int (*VisitorFunc)(struct ASTPool *, int);

int visit_eval(struct ASTPool *p, int idx);
int visit_count(struct ASTPool *p, int idx);
int visit_depth(struct ASTPool *p, int idx);

int visit_eval(struct ASTPool *p, int idx) {
    struct ASTNode *n;
    n = &p->nodes[idx];
    if (n->type == NODE_INT) {
        return n->int_val;
    } else if (n->type == NODE_ADD) {
        return visit_eval(p, n->left_idx) + visit_eval(p, n->right_idx);
    } else if (n->type == NODE_MUL) {
        return visit_eval(p, n->left_idx) * visit_eval(p, n->right_idx);
    } else if (n->type == NODE_NEG) {
        return -visit_eval(p, n->left_idx);
    }
    return 0;
}

int visit_count(struct ASTPool *p, int idx) {
    struct ASTNode *n;
    n = &p->nodes[idx];
    if (n->type == NODE_INT) {
        return 1;
    } else if (n->type == NODE_NEG) {
        return 1 + visit_count(p, n->left_idx);
    } else {
        return 1 + visit_count(p, n->left_idx) + visit_count(p, n->right_idx);
    }
}

int visit_depth(struct ASTPool *p, int idx) {
    struct ASTNode *n;
    n = &p->nodes[idx];
    if (n->type == NODE_INT) {
        return 1;
    } else if (n->type == NODE_NEG) {
        return 1 + visit_depth(p, n->left_idx);
    } else {
        int ld;
        int rd;
        ld = visit_depth(p, n->left_idx);
        rd = visit_depth(p, n->right_idx);
        if (ld > rd) return 1 + ld;
        return 1 + rd;
    }
}

char *type_name(enum NodeType t) {
    if (t == NODE_INT) return "INT";
    if (t == NODE_ADD) return "ADD";
    if (t == NODE_MUL) return "MUL";
    return "NEG";
}

void apply_visitor(struct ASTPool *p, int root, VisitorFunc visitor, char *name) {
    int result;
    result = visitor(p, root);
    printf("Visitor %s: %d\n", name, result);
}

int main() {
    struct ASTPool pool;
    pool_init(&pool);

    // Build expression: (3 + 4) * 2
    int n3;
    n3 = make_int(&pool, 3);
    int n4;
    n4 = make_int(&pool, 4);
    int add1;
    add1 = make_add(&pool, n3, n4);
    int n2;
    n2 = make_int(&pool, 2);
    int mul1;
    mul1 = make_mul(&pool, add1, n2);

    printf("Tree nodes: %d\n", pool.count); // EXPECT: Tree nodes: 5
    printf("Root type: %s\n", type_name(pool.nodes[mul1].type)); // EXPECT: Root type: MUL

    apply_visitor(&pool, mul1, visit_eval, "eval"); // EXPECT: Visitor eval: 14
    apply_visitor(&pool, mul1, visit_count, "count"); // EXPECT: Visitor count: 5
    apply_visitor(&pool, mul1, visit_depth, "depth"); // EXPECT: Visitor depth: 3

    // Build: -(5 + 3)
    int n5;
    n5 = make_int(&pool, 5);
    int n3b;
    n3b = make_int(&pool, 3);
    int add2;
    add2 = make_add(&pool, n5, n3b);
    int neg1;
    neg1 = make_neg(&pool, add2);

    apply_visitor(&pool, neg1, visit_eval, "eval"); // EXPECT: Visitor eval: -8
    apply_visitor(&pool, neg1, visit_count, "count"); // EXPECT: Visitor count: 4
    apply_visitor(&pool, neg1, visit_depth, "depth"); // EXPECT: Visitor depth: 3

    // Build complex: (2 * 3) + (4 * 5) => 26
    int na;
    na = make_int(&pool, 2);
    int nb;
    nb = make_int(&pool, 3);
    int mul_a;
    mul_a = make_mul(&pool, na, nb);
    int nc;
    nc = make_int(&pool, 4);
    int nd;
    nd = make_int(&pool, 5);
    int mul_b;
    mul_b = make_mul(&pool, nc, nd);
    int add3;
    add3 = make_add(&pool, mul_a, mul_b);

    apply_visitor(&pool, add3, visit_eval, "eval"); // EXPECT: Visitor eval: 26
    apply_visitor(&pool, add3, visit_count, "count"); // EXPECT: Visitor count: 7
    apply_visitor(&pool, add3, visit_depth, "depth"); // EXPECT: Visitor depth: 3

    printf("Total pool nodes: %d\n", pool.count); // EXPECT: Total pool nodes: 16

    // Dispatch table approach
    VisitorFunc visitors[3];
    char *vnames[3];
    visitors[0] = visit_eval;
    visitors[1] = visit_count;
    visitors[2] = visit_depth;
    vnames[0] = "eval";
    vnames[1] = "count";
    vnames[2] = "depth";

    printf("Dispatch all on mul1:\n"); // EXPECT: Dispatch all on mul1:
    int i;
    for (i = 0; i < 3; i = i + 1) {
        apply_visitor(&pool, mul1, visitors[i], vnames[i]);
    }
    // EXPECT: Visitor eval: 14
    // EXPECT: Visitor count: 5
    // EXPECT: Visitor depth: 3

    printf("Done\n"); // EXPECT: Done
    return 0;
}
