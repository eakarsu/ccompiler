int printf(const char *fmt, ...);

// Red-black tree property checking
// Encode a tree in arrays, verify RB properties

// Colors: 0 = RED, 1 = BLACK, -1 = NIL (always black)

struct RBTree {
    int key[16];
    int color[16];    // 0=red, 1=black
    int left[16];     // index or -1 for nil
    int right[16];    // index or -1 for nil
    int parent[16];   // index or -1 for root
    int size;
    int root;
};

void rb_init(struct RBTree *t) {
    int i;
    t->size = 0;
    t->root = -1;
    for (i = 0; i < 16; i++) {
        t->key[i] = 0;
        t->color[i] = 1;
        t->left[i] = -1;
        t->right[i] = -1;
        t->parent[i] = -1;
    }
}

int rb_add_node(struct RBTree *t, int k, int c, int l, int r, int p) {
    int idx;
    idx = t->size;
    t->key[idx] = k;
    t->color[idx] = c;
    t->left[idx] = l;
    t->right[idx] = r;
    t->parent[idx] = p;
    t->size = t->size + 1;
    return idx;
}

// Property 1: Root is black
int check_root_black(struct RBTree *t) {
    if (t->root == -1) return 1;
    return t->color[t->root] == 1;
}

// Property 2: Red nodes have black children
int check_red_children(struct RBTree *t, int node) {
    int left;
    int right;
    if (node == -1) return 1;
    left = t->left[node];
    right = t->right[node];
    if (t->color[node] == 0) {
        // Red node: children must be black
        if (left != -1 && t->color[left] == 0) return 0;
        if (right != -1 && t->color[right] == 0) return 0;
    }
    if (!check_red_children(t, left)) return 0;
    if (!check_red_children(t, right)) return 0;
    return 1;
}

// Property 3: All paths from a node to leaves have same black count
// Returns black height or -1 if invalid
int check_black_height(struct RBTree *t, int node) {
    int left_bh;
    int right_bh;
    int add;
    if (node == -1) return 1; // nil counts as 1 black
    left_bh = check_black_height(t, t->left[node]);
    right_bh = check_black_height(t, t->right[node]);
    if (left_bh == -1 || right_bh == -1) return -1;
    if (left_bh != right_bh) return -1;
    add = 0;
    if (t->color[node] == 1) add = 1;
    return left_bh + add;
}

// Check BST property
int check_bst(struct RBTree *t, int node, int lo, int hi) {
    if (node == -1) return 1;
    if (t->key[node] <= lo || t->key[node] >= hi) return 0;
    if (!check_bst(t, t->left[node], lo, t->key[node])) return 0;
    if (!check_bst(t, t->right[node], t->key[node], hi)) return 0;
    return 1;
}

int count_nodes(struct RBTree *t, int node) {
    if (node == -1) return 0;
    return 1 + count_nodes(t, t->left[node]) + count_nodes(t, t->right[node]);
}

int count_red_nodes(struct RBTree *t, int node) {
    int c;
    if (node == -1) return 0;
    c = 0;
    if (t->color[node] == 0) c = 1;
    return c + count_red_nodes(t, t->left[node]) + count_red_nodes(t, t->right[node]);
}

int main() {
    struct RBTree t;

    // Build a valid red-black tree:
    //          8(B)
    //        /      \
    //     4(R)      12(R)
    //    /   \     /    \
    //  2(B) 6(B) 10(B) 14(B)

    rb_init(&t);
    rb_add_node(&t, 8, 1, 1, 2, -1);   // 0: root
    rb_add_node(&t, 4, 0, 3, 4, 0);    // 1: left child of root
    rb_add_node(&t, 12, 0, 5, 6, 0);   // 2: right child of root
    rb_add_node(&t, 2, 1, -1, -1, 1);  // 3
    rb_add_node(&t, 6, 1, -1, -1, 1);  // 4
    rb_add_node(&t, 10, 1, -1, -1, 2); // 5
    rb_add_node(&t, 14, 1, -1, -1, 2); // 6
    t.root = 0;

    printf("Node count: %d\n", count_nodes(&t, t.root));
    // EXPECT: Node count: 7

    printf("Red nodes: %d\n", count_red_nodes(&t, t.root));
    // EXPECT: Red nodes: 2

    printf("Root is black: %d\n", check_root_black(&t));
    // EXPECT: Root is black: 1

    printf("Red children ok: %d\n", check_red_children(&t, t.root));
    // EXPECT: Red children ok: 1

    printf("Black height: %d\n", check_black_height(&t, t.root));
    // EXPECT: Black height: 3

    printf("BST valid: %d\n", check_bst(&t, t.root, -9999, 9999));
    // EXPECT: BST valid: 1

    // Now break the red property: make node 3 (key=2) red
    // So we have red 4 -> red 2 (violation)
    t.color[3] = 0;
    printf("After break: red children ok: %d\n", check_red_children(&t, t.root));
    // EXPECT: After break: red children ok: 0

    // Restore and break black height: make node 5 (key=10) red
    t.color[3] = 1;
    t.color[5] = 0;
    printf("After color change: black height: %d\n", check_black_height(&t, t.root));
    // EXPECT: After color change: black height: -1

    // Restore
    t.color[5] = 1;
    printf("Restored black height: %d\n", check_black_height(&t, t.root));
    // EXPECT: Restored black height: 3

    // Break BST: swap keys of nodes 3 and 4 (2 and 6)
    t.key[3] = 6;
    t.key[4] = 2;
    printf("After swap: BST valid: %d\n", check_bst(&t, t.root, -9999, 9999));
    // EXPECT: After swap: BST valid: 0

    // Restore
    t.key[3] = 2;
    t.key[4] = 6;
    printf("Restored BST: %d\n", check_bst(&t, t.root, -9999, 9999));
    // EXPECT: Restored BST: 1

    // Make root red - violates property
    t.color[0] = 0;
    printf("Red root: root is black: %d\n", check_root_black(&t));
    // EXPECT: Red root: root is black: 0

    printf("Red nodes now: %d\n", count_red_nodes(&t, t.root));
    // EXPECT: Red nodes now: 3

    return 0;
}
