int printf(const char *fmt, ...);
// EXPECT: Expr: ((3 + 4) * (10 - 2))\nPostfix: 3 4 + 10 2 - * \nResult: 56\nHeight: 3\nNodes: 7\nExpr: (((5 + 3) * 2) / (8 - 4))\nResult: 4
// Expression tree builder and evaluator

enum NodeType {
    NODE_NUM = 0,
    NODE_ADD = 1,
    NODE_SUB = 2,
    NODE_MUL = 3,
    NODE_DIV = 4
};

struct ExprNode {
    int type;
    int value;
    int left;
    int right;
};

struct ExprTree {
    struct ExprNode nodes[32];
    int count;
};

void et_init(struct ExprTree *t) {
    t->count = 0;
}

int et_num(struct ExprTree *t, int val) {
    int idx = t->count;
    t->nodes[idx].type = NODE_NUM;
    t->nodes[idx].value = val;
    t->nodes[idx].left = -1;
    t->nodes[idx].right = -1;
    t->count++;
    return idx;
}

int et_op(struct ExprTree *t, int type, int left, int right) {
    int idx = t->count;
    t->nodes[idx].type = type;
    t->nodes[idx].value = 0;
    t->nodes[idx].left = left;
    t->nodes[idx].right = right;
    t->count++;
    return idx;
}

int et_eval(struct ExprTree *t, int node) {
    int l;
    int r;
    if (node == -1) return 0;
    if (t->nodes[node].type == NODE_NUM) return t->nodes[node].value;
    l = et_eval(t, t->nodes[node].left);
    r = et_eval(t, t->nodes[node].right);
    if (t->nodes[node].type == NODE_ADD) return l + r;
    if (t->nodes[node].type == NODE_SUB) return l - r;
    if (t->nodes[node].type == NODE_MUL) return l * r;
    if (t->nodes[node].type == NODE_DIV) return l / r;
    return 0;
}

void et_print_infix(struct ExprTree *t, int node) {
    if (node == -1) return;
    if (t->nodes[node].type == NODE_NUM) {
        printf("%d", t->nodes[node].value);
        return;
    }
    printf("(");
    et_print_infix(t, t->nodes[node].left);
    if (t->nodes[node].type == NODE_ADD) printf(" + ");
    else if (t->nodes[node].type == NODE_SUB) printf(" - ");
    else if (t->nodes[node].type == NODE_MUL) printf(" * ");
    else if (t->nodes[node].type == NODE_DIV) printf(" / ");
    et_print_infix(t, t->nodes[node].right);
    printf(")");
}

void et_print_postfix(struct ExprTree *t, int node) {
    if (node == -1) return;
    if (t->nodes[node].type == NODE_NUM) {
        printf("%d ", t->nodes[node].value);
        return;
    }
    et_print_postfix(t, t->nodes[node].left);
    et_print_postfix(t, t->nodes[node].right);
    if (t->nodes[node].type == NODE_ADD) printf("+ ");
    else if (t->nodes[node].type == NODE_SUB) printf("- ");
    else if (t->nodes[node].type == NODE_MUL) printf("* ");
    else if (t->nodes[node].type == NODE_DIV) printf("/ ");
}

int et_height(struct ExprTree *t, int node) {
    int lh;
    int rh;
    if (node == -1) return 0;
    lh = et_height(t, t->nodes[node].left);
    rh = et_height(t, t->nodes[node].right);
    return 1 + (lh > rh ? lh : rh);
}

int et_count_nodes(struct ExprTree *t, int node) {
    if (node == -1) return 0;
    return 1 + et_count_nodes(t, t->nodes[node].left) + et_count_nodes(t, t->nodes[node].right);
}

int main(void) {
    struct ExprTree t;
    int root;
    int a;
    int b;
    int c;
    int d;
    int e;

    /* (3 + 4) * (10 - 2) */
    et_init(&t);
    a = et_num(&t, 3);
    b = et_num(&t, 4);
    c = et_op(&t, NODE_ADD, a, b);
    d = et_num(&t, 10);
    e = et_num(&t, 2);
    root = et_op(&t, NODE_MUL, c, et_op(&t, NODE_SUB, d, e));
    printf("Expr: ");
    et_print_infix(&t, root);
    printf("\n");
    printf("Postfix: ");
    et_print_postfix(&t, root);
    printf("\n");
    printf("Result: %d\n", et_eval(&t, root));
    printf("Height: %d\n", et_height(&t, root));
    printf("Nodes: %d\n", et_count_nodes(&t, root));

    /* ((5 + 3) * 2) / (8 - 4) */
    et_init(&t);
    a = et_num(&t, 5);
    b = et_num(&t, 3);
    c = et_op(&t, NODE_ADD, a, b);
    d = et_num(&t, 2);
    e = et_op(&t, NODE_MUL, c, d);
    a = et_num(&t, 8);
    b = et_num(&t, 4);
    c = et_op(&t, NODE_SUB, a, b);
    root = et_op(&t, NODE_DIV, e, c);
    printf("Expr: ");
    et_print_infix(&t, root);
    printf("\n");
    printf("Result: %d\n", et_eval(&t, root));

    return 0;
}
