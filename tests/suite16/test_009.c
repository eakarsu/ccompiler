int printf(const char *fmt, ...);

// Expression tree evaluator using array-based binary tree
// Each node has: type (0=num, 1=op), value (number or operator), left, right
// Tree stored in arrays with indices

int node_type[128];   // 0 = number, 1 = operator
int node_val[128];    // number value or operator char
int node_left[128];   // left child index (-1 if none)
int node_right[128];  // right child index (-1 if none)
int node_count;

void tree_init(void) {
    node_count = 0;
}

int make_num(int val) {
    int idx;
    idx = node_count;
    node_count = node_count + 1;
    node_type[idx] = 0;
    node_val[idx] = val;
    node_left[idx] = -1;
    node_right[idx] = -1;
    return idx;
}

int make_op(int op, int left, int right) {
    int idx;
    idx = node_count;
    node_count = node_count + 1;
    node_type[idx] = 1;
    node_val[idx] = op;
    node_left[idx] = left;
    node_right[idx] = right;
    return idx;
}

int eval_tree(int idx) {
    int left_val;
    int right_val;
    int op;

    if (idx == -1) return 0;

    if (node_type[idx] == 0) {
        return node_val[idx];
    }

    left_val = eval_tree(node_left[idx]);
    right_val = eval_tree(node_right[idx]);
    op = node_val[idx];

    if (op == 43) return left_val + right_val;      // '+'
    if (op == 45) return left_val - right_val;      // '-'
    if (op == 42) return left_val * right_val;      // '*'
    if (op == 47) return left_val / right_val;      // '/'
    return 0;
}

// Count nodes in tree
int count_nodes(int idx) {
    if (idx == -1) return 0;
    return 1 + count_nodes(node_left[idx]) + count_nodes(node_right[idx]);
}

// Compute depth of tree
int tree_depth(int idx) {
    int ld;
    int rd;
    if (idx == -1) return 0;
    ld = tree_depth(node_left[idx]);
    rd = tree_depth(node_right[idx]);
    if (ld > rd)
        return 1 + ld;
    return 1 + rd;
}

// Print tree in-order (infix)
void print_inorder(int idx) {
    if (idx == -1) return;
    if (node_type[idx] == 0) {
        printf("%d", node_val[idx]);
        return;
    }
    printf("(");
    print_inorder(node_left[idx]);
    if (node_val[idx] == 43) printf("+");
    else if (node_val[idx] == 45) printf("-");
    else if (node_val[idx] == 42) printf("*");
    else if (node_val[idx] == 47) printf("/");
    print_inorder(node_right[idx]);
    printf(")");
}

// Print tree post-order (postfix/RPN)
void print_postorder(int idx) {
    if (idx == -1) return;
    if (node_type[idx] == 0) {
        printf("%d ", node_val[idx]);
        return;
    }
    print_postorder(node_left[idx]);
    print_postorder(node_right[idx]);
    if (node_val[idx] == 43) printf("+ ");
    else if (node_val[idx] == 45) printf("- ");
    else if (node_val[idx] == 42) printf("* ");
    else if (node_val[idx] == 47) printf("/ ");
}

int main(void) {
    int root;
    int r;
    int n1;
    int n2;
    int n3;
    int n4;
    int n5;
    int op1;
    int op2;
    int op3;

    // Tree 1: 3 + 4
    tree_init();
    n1 = make_num(3);
    n2 = make_num(4);
    root = make_op(43, n1, n2);
    r = eval_tree(root);
    printf("3+4 = %d\n", r); // EXPECT: 3+4 = 7
    printf("nodes: %d\n", count_nodes(root)); // EXPECT: nodes: 3
    printf("depth: %d\n", tree_depth(root)); // EXPECT: depth: 2

    // Tree 2: (3 + 4) * 5
    tree_init();
    n1 = make_num(3);
    n2 = make_num(4);
    op1 = make_op(43, n1, n2);  // 3+4
    n3 = make_num(5);
    root = make_op(42, op1, n3);  // (3+4)*5
    r = eval_tree(root);
    printf("(3+4)*5 = %d\n", r); // EXPECT: (3+4)*5 = 35
    printf("nodes: %d\n", count_nodes(root)); // EXPECT: nodes: 5
    printf("depth: %d\n", tree_depth(root)); // EXPECT: depth: 3

    // Tree 3: (2 + 3) * (4 - 1)
    tree_init();
    n1 = make_num(2);
    n2 = make_num(3);
    op1 = make_op(43, n1, n2);  // 2+3
    n3 = make_num(4);
    n4 = make_num(1);
    op2 = make_op(45, n3, n4);  // 4-1
    root = make_op(42, op1, op2); // (2+3)*(4-1)
    r = eval_tree(root);
    printf("(2+3)*(4-1) = %d\n", r); // EXPECT: (2+3)*(4-1) = 15
    printf("nodes: %d\n", count_nodes(root)); // EXPECT: nodes: 7

    // Tree 4: ((1 + 2) * 3) - (10 / 5)
    tree_init();
    n1 = make_num(1);
    n2 = make_num(2);
    op1 = make_op(43, n1, n2);  // 1+2
    n3 = make_num(3);
    op2 = make_op(42, op1, n3); // (1+2)*3 = 9
    n4 = make_num(10);
    n5 = make_num(5);
    op3 = make_op(47, n4, n5);  // 10/5 = 2
    root = make_op(45, op2, op3); // 9 - 2 = 7
    r = eval_tree(root);
    printf("((1+2)*3)-(10/5) = %d\n", r); // EXPECT: ((1+2)*3)-(10/5) = 7
    printf("depth: %d\n", tree_depth(root)); // EXPECT: depth: 4

    // Print infix of tree 4
    printf("infix: ");
    print_inorder(root);
    printf("\n"); // EXPECT: infix: (((1+2)*3)-(10/5))

    // Print postfix of tree 4
    printf("postfix: ");
    print_postorder(root);
    printf("\n"); // EXPECT: postfix: 1 2 + 3 * 10 5 / - 

    // Tree 5: deep left chain: ((((5-1)-1)-1)-1) = 1
    tree_init();
    n1 = make_num(5);
    n2 = make_num(1);
    op1 = make_op(45, n1, n2); // 5-1=4
    n3 = make_num(1);
    op2 = make_op(45, op1, n3); // 4-1=3
    n4 = make_num(1);
    op3 = make_op(45, op2, n4); // 3-1=2
    n5 = make_num(1);
    root = make_op(45, op3, n5); // 2-1=1
    r = eval_tree(root);
    printf("deep chain = %d\n", r); // EXPECT: deep chain = 1
    printf("depth: %d\n", tree_depth(root)); // EXPECT: depth: 5
    printf("nodes: %d\n", count_nodes(root)); // EXPECT: nodes: 9

    printf("all tree eval tests passed\n"); // EXPECT: all tree eval tests passed
    return 0;
}
