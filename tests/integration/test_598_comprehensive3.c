int printf(const char *fmt, ...);
// EXPECT: Expression 1: ((3+4)*(10-2))\nResult: 56\nNodes: 7, Depth: 3\n\nExpression 2: (((5*6)+(8/2))-7)\nResult: 27\nNodes: 9, Depth: 4\n\nExpression 3: (100/(2+3))\nResult: 20\nNodes: 5, Depth: 3
#define MAX_EXPR 20

enum ExprType { EXPR_NUM = 0, EXPR_ADD = 1, EXPR_SUB = 2, EXPR_MUL = 3, EXPR_DIV = 4 };

struct ExprNode {
    int type;
    int value;
    int left;
    int right;
};

struct ExprTree {
    struct ExprNode nodes[MAX_EXPR];
    int count;
};

typedef int (*EvalFn)(struct ExprTree *, int);

int tree_init(struct ExprTree *t) {
    t->count = 0;
    return 0;
}

int tree_add_num(struct ExprTree *t, int val) {
    int i = t->count;
    t->nodes[i].type = EXPR_NUM;
    t->nodes[i].value = val;
    t->nodes[i].left = -1;
    t->nodes[i].right = -1;
    t->count++;
    return i;
}

int tree_add_op(struct ExprTree *t, int type, int left, int right) {
    int i = t->count;
    t->nodes[i].type = type;
    t->nodes[i].value = 0;
    t->nodes[i].left = left;
    t->nodes[i].right = right;
    t->count++;
    return i;
}

int eval(struct ExprTree *t, int node) {
    if (node < 0 || node >= t->count) return 0;
    struct ExprNode *n = &t->nodes[node];
    if (n->type == EXPR_NUM) return n->value;

    int left_val = eval(t, n->left);
    int right_val = eval(t, n->right);

    switch (n->type) {
        case EXPR_ADD: return left_val + right_val;
        case EXPR_SUB: return left_val - right_val;
        case EXPR_MUL: return left_val * right_val;
        case EXPR_DIV: return right_val != 0 ? left_val / right_val : 0;
    }
    return 0;
}

void print_expr(struct ExprTree *t, int node) {
    if (node < 0 || node >= t->count) return;
    struct ExprNode *n = &t->nodes[node];
    if (n->type == EXPR_NUM) {
        printf("%d", n->value);
        return;
    }
    printf("(");
    print_expr(t, n->left);
    switch (n->type) {
        case EXPR_ADD: printf("+"); break;
        case EXPR_SUB: printf("-"); break;
        case EXPR_MUL: printf("*"); break;
        case EXPR_DIV: printf("/"); break;
    }
    print_expr(t, n->right);
    printf(")");
}

int count_nodes(struct ExprTree *t, int node) {
    if (node < 0 || node >= t->count) return 0;
    if (t->nodes[node].type == EXPR_NUM) return 1;
    return 1 + count_nodes(t, t->nodes[node].left) + count_nodes(t, t->nodes[node].right);
}

int tree_depth(struct ExprTree *t, int node) {
    if (node < 0 || node >= t->count) return 0;
    if (t->nodes[node].type == EXPR_NUM) return 1;
    int ld = tree_depth(t, t->nodes[node].left);
    int rd = tree_depth(t, t->nodes[node].right);
    return 1 + (ld > rd ? ld : rd);
}

int main(void) {
    struct ExprTree tree;
    tree_init(&tree);

    /* Build: (3 + 4) * (10 - 2) */
    int n3 = tree_add_num(&tree, 3);
    int n4 = tree_add_num(&tree, 4);
    int add1 = tree_add_op(&tree, EXPR_ADD, n3, n4);
    int n10 = tree_add_num(&tree, 10);
    int n2 = tree_add_num(&tree, 2);
    int sub1 = tree_add_op(&tree, EXPR_SUB, n10, n2);
    int root1 = tree_add_op(&tree, EXPR_MUL, add1, sub1);

    printf("Expression 1: ");
    print_expr(&tree, root1);
    printf("\n");

    EvalFn evaluate = eval;
    printf("Result: %d\n", evaluate(&tree, root1));
    printf("Nodes: %d, Depth: %d\n", count_nodes(&tree, root1), tree_depth(&tree, root1));

    /* Build: ((5 * 6) + (8 / 2)) - 7 */
    struct ExprTree tree2;
    tree_init(&tree2);
    int a5 = tree_add_num(&tree2, 5);
    int a6 = tree_add_num(&tree2, 6);
    int mul1 = tree_add_op(&tree2, EXPR_MUL, a5, a6);
    int a8 = tree_add_num(&tree2, 8);
    int a2 = tree_add_num(&tree2, 2);
    int div1 = tree_add_op(&tree2, EXPR_DIV, a8, a2);
    int add2 = tree_add_op(&tree2, EXPR_ADD, mul1, div1);
    int a7 = tree_add_num(&tree2, 7);
    int root2 = tree_add_op(&tree2, EXPR_SUB, add2, a7);

    printf("\nExpression 2: ");
    print_expr(&tree2, root2);
    printf("\n");
    printf("Result: %d\n", evaluate(&tree2, root2));
    printf("Nodes: %d, Depth: %d\n", count_nodes(&tree2, root2), tree_depth(&tree2, root2));

    /* Build: 100 / (2 + 3) */
    struct ExprTree tree3;
    tree_init(&tree3);
    int b100 = tree_add_num(&tree3, 100);
    int b2 = tree_add_num(&tree3, 2);
    int b3 = tree_add_num(&tree3, 3);
    int badd = tree_add_op(&tree3, EXPR_ADD, b2, b3);
    int root3 = tree_add_op(&tree3, EXPR_DIV, b100, badd);

    printf("\nExpression 3: ");
    print_expr(&tree3, root3);
    printf("\n");
    printf("Result: %d\n", evaluate(&tree3, root3));
    printf("Nodes: %d, Depth: %d\n", count_nodes(&tree3, root3), tree_depth(&tree3, root3));

    return 0;
}
