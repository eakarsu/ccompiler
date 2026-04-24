int printf(const char *fmt, ...);

// Balanced BST check
// A BST is balanced if for every node, the height difference of left and
// right subtrees is at most 1 (AVL property)

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

// Returns height if balanced, -1 if unbalanced
int check_balanced(struct Tree *t, int node) {
    int lh;
    int rh;
    int diff;
    if (node == 0) return 0;
    lh = check_balanced(t, t->nodes[node].left);
    if (lh == -1) return -1;
    rh = check_balanced(t, t->nodes[node].right);
    if (rh == -1) return -1;
    diff = lh - rh;
    if (diff < 0) diff = -diff;
    if (diff > 1) return -1;
    if (lh > rh) return lh + 1;
    return rh + 1;
}

int is_balanced(struct Tree *t, int root) {
    return check_balanced(t, root) != -1;
}

// Check if tree is a valid BST (inorder values should be strictly increasing)
int is_bst_helper(struct Tree *t, int node, int min_val, int max_val) {
    if (node == 0) return 1;
    if (t->nodes[node].val <= min_val || t->nodes[node].val >= max_val) {
        return 0;
    }
    return is_bst_helper(t, t->nodes[node].left, min_val, t->nodes[node].val) &&
           is_bst_helper(t, t->nodes[node].right, t->nodes[node].val, max_val);
}

int is_bst(struct Tree *t, int root) {
    return is_bst_helper(t, root, -999999, 999999);
}

// BST insert
void bst_insert(struct Tree *t, int *root, int val) {
    int cur;
    int idx;
    if (*root == 0) {
        *root = tree_add(t, val);
        return;
    }
    cur = *root;
    while (1) {
        if (val < t->nodes[cur].val) {
            if (t->nodes[cur].left == 0) {
                idx = tree_add(t, val);
                t->nodes[cur].left = idx;
                return;
            }
            cur = t->nodes[cur].left;
        } else if (val > t->nodes[cur].val) {
            if (t->nodes[cur].right == 0) {
                idx = tree_add(t, val);
                t->nodes[cur].right = idx;
                return;
            }
            cur = t->nodes[cur].right;
        } else {
            return;
        }
    }
}

// Build balanced BST from sorted array
int build_balanced(struct Tree *t, int *arr, int lo, int hi) {
    int mid;
    int node;
    int left_child;
    int right_child;
    if (lo > hi) return 0;
    mid = (lo + hi) / 2;
    node = tree_add(t, arr[mid]);
    left_child = build_balanced(t, arr, lo, mid - 1);
    right_child = build_balanced(t, arr, mid + 1, hi);
    tree_link(t, node, left_child, right_child);
    return node;
}

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
    // Test 1: Balanced tree
    struct Tree t1;
    tree_init(&t1);
    //       4
    //      / \
    //     2   6
    //    / \ / \
    //   1  3 5  7
    int n4 = tree_add(&t1, 4);
    int n2 = tree_add(&t1, 2);
    int n6 = tree_add(&t1, 6);
    int n1 = tree_add(&t1, 1);
    int n3 = tree_add(&t1, 3);
    int n5 = tree_add(&t1, 5);
    int n7 = tree_add(&t1, 7);
    tree_link(&t1, n4, n2, n6);
    tree_link(&t1, n2, n1, n3);
    tree_link(&t1, n6, n5, n7);

    printf("Tree1 balanced: %d\n", is_balanced(&t1, n4));
    // EXPECT: Tree1 balanced: 1
    printf("Tree1 is BST: %d\n", is_bst(&t1, n4));
    // EXPECT: Tree1 is BST: 1
    printf("Tree1 height: %d\n", tree_height(&t1, n4));
    // EXPECT: Tree1 height: 3

    // Test 2: Unbalanced tree (right-skewed)
    struct Tree t2;
    tree_init(&t2);
    int a1 = tree_add(&t2, 1);
    int a2 = tree_add(&t2, 2);
    int a3 = tree_add(&t2, 3);
    int a4 = tree_add(&t2, 4);
    tree_link(&t2, a1, 0, a2);
    tree_link(&t2, a2, 0, a3);
    tree_link(&t2, a3, 0, a4);

    printf("Tree2 balanced: %d\n", is_balanced(&t2, a1));
    // EXPECT: Tree2 balanced: 0
    printf("Tree2 is BST: %d\n", is_bst(&t2, a1));
    // EXPECT: Tree2 is BST: 1

    // Test 3: BST built by sequential insertion (unbalanced)
    struct Tree t3;
    tree_init(&t3);
    int root3 = 0;
    bst_insert(&t3, &root3, 10);
    bst_insert(&t3, &root3, 20);
    bst_insert(&t3, &root3, 30);
    bst_insert(&t3, &root3, 40);
    bst_insert(&t3, &root3, 50);

    printf("Tree3 balanced: %d\n", is_balanced(&t3, root3));
    // EXPECT: Tree3 balanced: 0
    printf("Tree3 is BST: %d\n", is_bst(&t3, root3));
    // EXPECT: Tree3 is BST: 1
    printf("Tree3 height: %d\n", tree_height(&t3, root3));
    // EXPECT: Tree3 height: 5

    // Test 4: Build balanced BST from sorted array
    struct Tree t4;
    tree_init(&t4);
    int sorted[7];
    sorted[0] = 10;
    sorted[1] = 20;
    sorted[2] = 30;
    sorted[3] = 40;
    sorted[4] = 50;
    sorted[5] = 60;
    sorted[6] = 70;
    int root4 = build_balanced(&t4, sorted, 0, 6);

    printf("Tree4 balanced: %d\n", is_balanced(&t4, root4));
    // EXPECT: Tree4 balanced: 1
    printf("Tree4 is BST: %d\n", is_bst(&t4, root4));
    // EXPECT: Tree4 is BST: 1
    printf("Tree4 height: %d\n", tree_height(&t4, root4));
    // EXPECT: Tree4 height: 3

    print_inorder(&t4, root4, "Tree4 inorder: ");
    // EXPECT: Tree4 inorder: 10 20 30 40 50 60 70

    // Test 5: Not a BST
    struct Tree t5;
    tree_init(&t5);
    //      5
    //     / \
    //    3   8
    //   / \
    //  1   7   <-- 7 > 5, violates BST property
    int b5 = tree_add(&t5, 5);
    int b3 = tree_add(&t5, 3);
    int b8 = tree_add(&t5, 8);
    int b1 = tree_add(&t5, 1);
    int b7 = tree_add(&t5, 7);
    tree_link(&t5, b5, b3, b8);
    tree_link(&t5, b3, b1, b7);

    printf("Tree5 balanced: %d\n", is_balanced(&t5, b5));
    // EXPECT: Tree5 balanced: 1
    printf("Tree5 is BST: %d\n", is_bst(&t5, b5));
    // EXPECT: Tree5 is BST: 0

    // Test 6: Single node
    struct Tree t6;
    tree_init(&t6);
    int single = tree_add(&t6, 99);
    printf("Single balanced: %d\n", is_balanced(&t6, single));
    // EXPECT: Single balanced: 1
    printf("Single is BST: %d\n", is_bst(&t6, single));
    // EXPECT: Single is BST: 1

    return 0;
}
