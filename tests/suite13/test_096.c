int printf(const char *fmt, ...);

// Tree diameter calculation
// Diameter = longest path between any two nodes (in edges)
// Uses array-based tree

struct TreeNode {
    int val;
    int left;
    int right;
};

struct Tree {
    struct TreeNode nodes[32];
    int count;
};

void tree_init(struct Tree *t) {
    int i;
    for (i = 0; i < 32; i++) {
        t->nodes[i].val = 0;
        t->nodes[i].left = 0;
        t->nodes[i].right = 0;
    }
    t->count = 1;
}

int tree_add(struct Tree *t, int val) {
    int idx = t->count;
    t->count = t->count + 1;
    t->nodes[idx].val = val;
    t->nodes[idx].left = 0;
    t->nodes[idx].right = 0;
    return idx;
}

void tree_link(struct Tree *t, int parent, int left, int right) {
    t->nodes[parent].left = left;
    t->nodes[parent].right = right;
}

int tree_height(struct Tree *t, int node) {
    int lh;
    int rh;
    if (node == 0) return 0;
    lh = tree_height(t, t->nodes[node].left);
    rh = tree_height(t, t->nodes[node].right);
    if (lh > rh) return lh + 1;
    return rh + 1;
}

// Global variable to track maximum diameter found
int global_diameter;

// Compute height and update diameter simultaneously
int height_and_diameter(struct Tree *t, int node) {
    int lh;
    int rh;
    int through_here;
    if (node == 0) return 0;
    lh = height_and_diameter(t, t->nodes[node].left);
    rh = height_and_diameter(t, t->nodes[node].right);
    through_here = lh + rh;
    if (through_here > global_diameter) {
        global_diameter = through_here;
    }
    if (lh > rh) return lh + 1;
    return rh + 1;
}

int diameter(struct Tree *t, int root) {
    global_diameter = 0;
    height_and_diameter(t, root);
    return global_diameter;
}

// Count leaf nodes
int count_leaves(struct Tree *t, int node) {
    if (node == 0) return 0;
    if (t->nodes[node].left == 0 && t->nodes[node].right == 0) return 1;
    return count_leaves(t, t->nodes[node].left) + count_leaves(t, t->nodes[node].right);
}

// Sum of all depths of all nodes
int sum_of_depths(struct Tree *t, int node, int depth) {
    if (node == 0) return 0;
    return depth + sum_of_depths(t, t->nodes[node].left, depth + 1)
                 + sum_of_depths(t, t->nodes[node].right, depth + 1);
}

// Count internal nodes (non-leaf, non-null)
int count_internal(struct Tree *t, int node) {
    if (node == 0) return 0;
    if (t->nodes[node].left == 0 && t->nodes[node].right == 0) return 0;
    return 1 + count_internal(t, t->nodes[node].left) + count_internal(t, t->nodes[node].right);
}

// Mirror the tree
void tree_mirror(struct Tree *t, int node) {
    int tmp;
    if (node == 0) return;
    tmp = t->nodes[node].left;
    t->nodes[node].left = t->nodes[node].right;
    t->nodes[node].right = tmp;
    tree_mirror(t, t->nodes[node].left);
    tree_mirror(t, t->nodes[node].right);
}

// Collect inorder into array
void inorder_collect(struct Tree *t, int node, int *out, int *pos) {
    if (node == 0) return;
    inorder_collect(t, t->nodes[node].left, out, pos);
    out[*pos] = t->nodes[node].val;
    *pos = *pos + 1;
    inorder_collect(t, t->nodes[node].right, out, pos);
}

void print_inorder(struct Tree *t, int root, char *label) {
    int buf[32];
    int n = 0;
    int i;
    inorder_collect(t, root, buf, &n);
    printf("%s", label);
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", buf[i]);
    }
    printf("\n");
}

int main() {
    struct Tree t;
    tree_init(&t);

    //          1
    //        /   \
    //       2     3
    //      / \     \
    //     4   5     6
    //    /         / \
    //   7         8   9
    //  /
    // 10

    int n1 = tree_add(&t, 1);
    int n2 = tree_add(&t, 2);
    int n3 = tree_add(&t, 3);
    int n4 = tree_add(&t, 4);
    int n5 = tree_add(&t, 5);
    int n6 = tree_add(&t, 6);
    int n7 = tree_add(&t, 7);
    int n8 = tree_add(&t, 8);
    int n9 = tree_add(&t, 9);
    int n10 = tree_add(&t, 10);

    tree_link(&t, n1, n2, n3);
    tree_link(&t, n2, n4, n5);
    tree_link(&t, n3, 0, n6);
    tree_link(&t, n4, n7, 0);
    tree_link(&t, n6, n8, n9);
    tree_link(&t, n7, n10, 0);

    printf("Height: %d\n", tree_height(&t, n1));
    // EXPECT: Height: 5

    printf("Diameter: %d\n", diameter(&t, n1));
    // EXPECT: Diameter: 7

    printf("Leaves: %d\n", count_leaves(&t, n1));
    // EXPECT: Leaves: 4

    printf("Internal: %d\n", count_internal(&t, n1));
    // EXPECT: Internal: 6

    printf("Sum of depths: %d\n", sum_of_depths(&t, n1, 0));
    // EXPECT: Sum of depths: 21

    print_inorder(&t, n1, "Inorder: ");
    // EXPECT: Inorder: 10 7 4 2 5 1 3 8 6 9

    // Mirror the tree and print inorder again
    tree_mirror(&t, n1);
    print_inorder(&t, n1, "Mirrored: ");
    // EXPECT: Mirrored: 9 6 8 3 1 5 2 4 7 10

    // Diameter should remain the same after mirroring
    printf("Mirrored diameter: %d\n", diameter(&t, n1));
    // EXPECT: Mirrored diameter: 7

    // Test with a linear (degenerate) tree: 1-2-3-4-5
    struct Tree t2;
    tree_init(&t2);
    int a1 = tree_add(&t2, 1);
    int a2 = tree_add(&t2, 2);
    int a3 = tree_add(&t2, 3);
    int a4 = tree_add(&t2, 4);
    int a5 = tree_add(&t2, 5);
    tree_link(&t2, a1, 0, a2);
    tree_link(&t2, a2, 0, a3);
    tree_link(&t2, a3, 0, a4);
    tree_link(&t2, a4, 0, a5);

    printf("Linear height: %d\n", tree_height(&t2, a1));
    // EXPECT: Linear height: 5
    printf("Linear diameter: %d\n", diameter(&t2, a1));
    // EXPECT: Linear diameter: 4
    printf("Linear leaves: %d\n", count_leaves(&t2, a1));
    // EXPECT: Linear leaves: 1

    // Single node tree
    struct Tree t3;
    tree_init(&t3);
    int s = tree_add(&t3, 42);
    printf("Single height: %d\n", tree_height(&t3, s));
    // EXPECT: Single height: 1
    printf("Single diameter: %d\n", diameter(&t3, s));
    // EXPECT: Single diameter: 0
    printf("Single leaves: %d\n", count_leaves(&t3, s));
    // EXPECT: Single leaves: 1

    // Full binary tree test
    //       1
    //      / \
    //     2   3
    //    / \ / \
    //   4  5 6  7
    struct Tree t4;
    tree_init(&t4);
    int f1 = tree_add(&t4, 1);
    int f2 = tree_add(&t4, 2);
    int f3 = tree_add(&t4, 3);
    int f4 = tree_add(&t4, 4);
    int f5 = tree_add(&t4, 5);
    int f6 = tree_add(&t4, 6);
    int f7 = tree_add(&t4, 7);
    tree_link(&t4, f1, f2, f3);
    tree_link(&t4, f2, f4, f5);
    tree_link(&t4, f3, f6, f7);

    printf("Full diameter: %d\n", diameter(&t4, f1));
    // EXPECT: Full diameter: 4
    printf("Full leaves: %d\n", count_leaves(&t4, f1));
    // EXPECT: Full leaves: 4
    printf("Full internal: %d\n", count_internal(&t4, f1));
    // EXPECT: Full internal: 3

    return 0;
}
