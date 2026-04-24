int printf(const char *fmt, ...);

// Tree traversals: inorder, preorder, postorder
// Using array-based binary tree with explicit child indices

struct TreeNode {
    int val;
    int left;   // index, 0 = none
    int right;  // index, 0 = none
    int active;
};

struct Tree {
    struct TreeNode nodes[32];
    int count; // next index (0 is unused sentinel)
};

void tree_init(struct Tree *t) {
    int i;
    for (i = 0; i < 32; i++) {
        t->nodes[i].val = 0;
        t->nodes[i].left = 0;
        t->nodes[i].right = 0;
        t->nodes[i].active = 0;
    }
    t->count = 1;
}

int tree_add(struct Tree *t, int val) {
    int idx = t->count;
    t->count = t->count + 1;
    t->nodes[idx].val = val;
    t->nodes[idx].left = 0;
    t->nodes[idx].right = 0;
    t->nodes[idx].active = 1;
    return idx;
}

void tree_set_children(struct Tree *t, int parent, int left, int right) {
    t->nodes[parent].left = left;
    t->nodes[parent].right = right;
}

// Inorder: left, root, right
void inorder(struct Tree *t, int node, int *out, int *pos) {
    if (node == 0) return;
    inorder(t, t->nodes[node].left, out, pos);
    out[*pos] = t->nodes[node].val;
    *pos = *pos + 1;
    inorder(t, t->nodes[node].right, out, pos);
}

// Preorder: root, left, right
void preorder(struct Tree *t, int node, int *out, int *pos) {
    if (node == 0) return;
    out[*pos] = t->nodes[node].val;
    *pos = *pos + 1;
    preorder(t, t->nodes[node].left, out, pos);
    preorder(t, t->nodes[node].right, out, pos);
}

// Postorder: left, right, root
void postorder(struct Tree *t, int node, int *out, int *pos) {
    if (node == 0) return;
    postorder(t, t->nodes[node].left, out, pos);
    postorder(t, t->nodes[node].right, out, pos);
    out[*pos] = t->nodes[node].val;
    *pos = *pos + 1;
}

void print_labeled_array(char *label, int *arr, int n) {
    int i;
    printf("%s", label);
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

// Count nodes in subtree
int count_nodes(struct Tree *t, int node) {
    if (node == 0) return 0;
    return 1 + count_nodes(t, t->nodes[node].left) + count_nodes(t, t->nodes[node].right);
}

// Height of tree
int tree_height(struct Tree *t, int node) {
    int lh;
    int rh;
    if (node == 0) return 0;
    lh = tree_height(t, t->nodes[node].left);
    rh = tree_height(t, t->nodes[node].right);
    if (lh > rh) return lh + 1;
    return rh + 1;
}

// Sum of all node values
int tree_sum(struct Tree *t, int node) {
    if (node == 0) return 0;
    return t->nodes[node].val + tree_sum(t, t->nodes[node].left) + tree_sum(t, t->nodes[node].right);
}

// Check if two trees are structurally identical
int trees_equal(struct Tree *t1, int n1, struct Tree *t2, int n2) {
    if (n1 == 0 && n2 == 0) return 1;
    if (n1 == 0 || n2 == 0) return 0;
    if (t1->nodes[n1].val != t2->nodes[n2].val) return 0;
    return trees_equal(t1, t1->nodes[n1].left, t2, t2->nodes[n2].left) &&
           trees_equal(t1, t1->nodes[n1].right, t2, t2->nodes[n2].right);
}

int main() {
    struct Tree t;
    int result[32];
    int pos;
    int root;

    tree_init(&t);

    //        1
    //       / \
    //      2   3
    //     / \   \
    //    4   5   6
    //   /
    //  7

    int n1 = tree_add(&t, 1);
    int n2 = tree_add(&t, 2);
    int n3 = tree_add(&t, 3);
    int n4 = tree_add(&t, 4);
    int n5 = tree_add(&t, 5);
    int n6 = tree_add(&t, 6);
    int n7 = tree_add(&t, 7);

    tree_set_children(&t, n1, n2, n3);
    tree_set_children(&t, n2, n4, n5);
    tree_set_children(&t, n3, 0, n6);
    tree_set_children(&t, n4, n7, 0);

    root = n1;

    // Inorder traversal
    pos = 0;
    inorder(&t, root, result, &pos);
    print_labeled_array("Inorder: ", result, pos);
    // EXPECT: Inorder: 7 4 2 5 1 3 6

    // Preorder traversal
    pos = 0;
    preorder(&t, root, result, &pos);
    print_labeled_array("Preorder: ", result, pos);
    // EXPECT: Preorder: 1 2 4 7 5 3 6

    // Postorder traversal
    pos = 0;
    postorder(&t, root, result, &pos);
    print_labeled_array("Postorder: ", result, pos);
    // EXPECT: Postorder: 7 4 5 2 6 3 1

    printf("Node count: %d\n", count_nodes(&t, root));
    // EXPECT: Node count: 7
    printf("Height: %d\n", tree_height(&t, root));
    // EXPECT: Height: 4
    printf("Sum: %d\n", tree_sum(&t, root));
    // EXPECT: Sum: 28

    // Build a second tree: simple right-skewed
    //  10 -> 20 -> 30 -> 40
    struct Tree t2;
    tree_init(&t2);
    int a = tree_add(&t2, 10);
    int b = tree_add(&t2, 20);
    int c = tree_add(&t2, 30);
    int d = tree_add(&t2, 40);
    tree_set_children(&t2, a, 0, b);
    tree_set_children(&t2, b, 0, c);
    tree_set_children(&t2, c, 0, d);

    pos = 0;
    inorder(&t2, a, result, &pos);
    print_labeled_array("Skew in: ", result, pos);
    // EXPECT: Skew in: 10 20 30 40

    pos = 0;
    preorder(&t2, a, result, &pos);
    print_labeled_array("Skew pre: ", result, pos);
    // EXPECT: Skew pre: 10 20 30 40

    pos = 0;
    postorder(&t2, a, result, &pos);
    print_labeled_array("Skew post: ", result, pos);
    // EXPECT: Skew post: 40 30 20 10

    printf("Skew height: %d\n", tree_height(&t2, a));
    // EXPECT: Skew height: 4
    printf("Skew sum: %d\n", tree_sum(&t2, a));
    // EXPECT: Skew sum: 100

    // Self-equality check
    printf("Equal self: %d\n", trees_equal(&t, root, &t, root));
    // EXPECT: Equal self: 1
    printf("Equal diff: %d\n", trees_equal(&t, root, &t2, a));
    // EXPECT: Equal diff: 0

    return 0;
}
