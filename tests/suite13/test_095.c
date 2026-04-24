int printf(const char *fmt, ...);

// Lowest Common Ancestor (LCA) in a binary tree
// Uses array-based tree with parent tracking

struct TreeNode {
    int val;
    int left;
    int right;
    int parent;
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
        t->nodes[i].parent = 0;
    }
    t->count = 1;
}

int tree_add(struct Tree *t, int val) {
    int idx = t->count;
    t->count = t->count + 1;
    t->nodes[idx].val = val;
    t->nodes[idx].left = 0;
    t->nodes[idx].right = 0;
    t->nodes[idx].parent = 0;
    return idx;
}

void tree_link(struct Tree *t, int parent, int left, int right) {
    t->nodes[parent].left = left;
    t->nodes[parent].right = right;
    if (left != 0) t->nodes[left].parent = parent;
    if (right != 0) t->nodes[right].parent = parent;
}

// Get depth of a node
int node_depth(struct Tree *t, int node) {
    int d = 0;
    int cur = node;
    while (t->nodes[cur].parent != 0) {
        d = d + 1;
        cur = t->nodes[cur].parent;
    }
    return d;
}

// LCA using parent pointers: bring both to same depth, then walk up together
int lca(struct Tree *t, int a, int b) {
    int da = node_depth(t, a);
    int db = node_depth(t, b);

    // Bring deeper node up
    while (da > db) {
        a = t->nodes[a].parent;
        da = da - 1;
    }
    while (db > da) {
        b = t->nodes[b].parent;
        db = db - 1;
    }

    // Walk up together until we meet
    while (a != b) {
        a = t->nodes[a].parent;
        b = t->nodes[b].parent;
    }
    return a;
}

// Alternative: recursive LCA without parent pointers
// Returns the index of LCA node, or 0 if not found in subtree
int lca_recursive(struct Tree *t, int node, int a, int b) {
    int left_res;
    int right_res;

    if (node == 0) return 0;
    if (node == a || node == b) return node;

    left_res = lca_recursive(t, t->nodes[node].left, a, b);
    right_res = lca_recursive(t, t->nodes[node].right, a, b);

    if (left_res != 0 && right_res != 0) return node;
    if (left_res != 0) return left_res;
    return right_res;
}

// Distance between two nodes through their LCA
int distance(struct Tree *t, int a, int b) {
    int l = lca(t, a, b);
    int da = node_depth(t, a) - node_depth(t, l);
    int db = node_depth(t, b) - node_depth(t, l);
    return da + db;
}

// Check if node is ancestor of target
int is_ancestor(struct Tree *t, int ancestor, int target) {
    int cur = target;
    while (cur != 0) {
        if (cur == ancestor) return 1;
        cur = t->nodes[cur].parent;
    }
    return 0;
}

// Path from node to root (store values)
int path_to_root(struct Tree *t, int node, int *path) {
    int len = 0;
    int cur = node;
    while (cur != 0) {
        path[len] = t->nodes[cur].val;
        len = len + 1;
        cur = t->nodes[cur].parent;
    }
    return len;
}

int main() {
    struct Tree t;
    tree_init(&t);

    //            1
    //          /   \
    //         2     3
    //        / \   / \
    //       4   5 6   7
    //      / \     \
    //     8   9    10

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
    tree_link(&t, n3, n6, n7);
    tree_link(&t, n4, n8, n9);
    tree_link(&t, n6, 0, n10);

    // LCA tests using parent pointer method
    int result;

    result = lca(&t, n8, n9);
    printf("LCA(8,9) = %d\n", t.nodes[result].val);
    // EXPECT: LCA(8,9) = 4

    result = lca(&t, n8, n5);
    printf("LCA(8,5) = %d\n", t.nodes[result].val);
    // EXPECT: LCA(8,5) = 2

    result = lca(&t, n8, n10);
    printf("LCA(8,10) = %d\n", t.nodes[result].val);
    // EXPECT: LCA(8,10) = 1

    result = lca(&t, n4, n5);
    printf("LCA(4,5) = %d\n", t.nodes[result].val);
    // EXPECT: LCA(4,5) = 2

    result = lca(&t, n6, n7);
    printf("LCA(6,7) = %d\n", t.nodes[result].val);
    // EXPECT: LCA(6,7) = 3

    result = lca(&t, n2, n3);
    printf("LCA(2,3) = %d\n", t.nodes[result].val);
    // EXPECT: LCA(2,3) = 1

    result = lca(&t, n2, n8);
    printf("LCA(2,8) = %d\n", t.nodes[result].val);
    // EXPECT: LCA(2,8) = 2

    // Recursive LCA
    result = lca_recursive(&t, n1, n8, n10);
    printf("Recursive LCA(8,10) = %d\n", t.nodes[result].val);
    // EXPECT: Recursive LCA(8,10) = 1

    result = lca_recursive(&t, n1, n8, n9);
    printf("Recursive LCA(8,9) = %d\n", t.nodes[result].val);
    // EXPECT: Recursive LCA(8,9) = 4

    // Distance tests
    printf("Dist(8,9) = %d\n", distance(&t, n8, n9));
    // EXPECT: Dist(8,9) = 2
    printf("Dist(8,10) = %d\n", distance(&t, n8, n10));
    // EXPECT: Dist(8,10) = 6
    printf("Dist(4,5) = %d\n", distance(&t, n4, n5));
    // EXPECT: Dist(4,5) = 2
    printf("Dist(8,7) = %d\n", distance(&t, n8, n7));
    // EXPECT: Dist(8,7) = 5

    // Ancestor checks
    printf("IsAnc(1,8) = %d\n", is_ancestor(&t, n1, n8));
    // EXPECT: IsAnc(1,8) = 1
    printf("IsAnc(3,8) = %d\n", is_ancestor(&t, n3, n8));
    // EXPECT: IsAnc(3,8) = 0
    printf("IsAnc(2,9) = %d\n", is_ancestor(&t, n2, n9));
    // EXPECT: IsAnc(2,9) = 1

    // Path to root
    int path[16];
    int plen;
    int i;

    plen = path_to_root(&t, n8, path);
    printf("Path 8->root: %d %d %d %d\n", path[0], path[1], path[2], path[3]);
    // EXPECT: Path 8->root: 8 4 2 1

    plen = path_to_root(&t, n10, path);
    printf("Path 10->root: %d %d %d %d\n", path[0], path[1], path[2], path[3]);
    // EXPECT: Path 10->root: 10 6 3 1

    return 0;
}
