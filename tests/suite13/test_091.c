int printf(const char *fmt, ...);

// Binary Search Tree using array representation
// Index 0 is unused; root is at index 1
// Uses a flat array with explicit left/right child indices.

struct BSTNode {
    int key;
    int left;   // index of left child, 0 = none
    int right;  // index of right child, 0 = none
    int used;   // 1 if this slot is occupied
};

struct BST {
    struct BSTNode nodes[64];
    int root;       // index of root node, 0 = empty
    int next_free;  // next free slot
};

void bst_init(struct BST *t) {
    int i;
    for (i = 0; i < 64; i++) {
        t->nodes[i].key = 0;
        t->nodes[i].left = 0;
        t->nodes[i].right = 0;
        t->nodes[i].used = 0;
    }
    t->root = 0;
    t->next_free = 1;
}

int bst_alloc(struct BST *t, int key) {
    int idx = t->next_free;
    t->next_free = t->next_free + 1;
    t->nodes[idx].key = key;
    t->nodes[idx].left = 0;
    t->nodes[idx].right = 0;
    t->nodes[idx].used = 1;
    return idx;
}

void bst_insert(struct BST *t, int key) {
    int idx;
    int cur;
    if (t->root == 0) {
        idx = bst_alloc(t, key);
        t->root = idx;
        return;
    }
    cur = t->root;
    while (1) {
        if (key < t->nodes[cur].key) {
            if (t->nodes[cur].left == 0) {
                idx = bst_alloc(t, key);
                t->nodes[cur].left = idx;
                return;
            }
            cur = t->nodes[cur].left;
        } else if (key > t->nodes[cur].key) {
            if (t->nodes[cur].right == 0) {
                idx = bst_alloc(t, key);
                t->nodes[cur].right = idx;
                return;
            }
            cur = t->nodes[cur].right;
        } else {
            return; // duplicate, ignore
        }
    }
}

int bst_search(struct BST *t, int key) {
    int cur = t->root;
    while (cur != 0) {
        if (key == t->nodes[cur].key) {
            return 1;
        } else if (key < t->nodes[cur].key) {
            cur = t->nodes[cur].left;
        } else {
            cur = t->nodes[cur].right;
        }
    }
    return 0;
}

int bst_find_min(struct BST *t, int node) {
    int cur = node;
    while (t->nodes[cur].left != 0) {
        cur = t->nodes[cur].left;
    }
    return cur;
}

// Delete returns the new subtree root index
int bst_delete_node(struct BST *t, int node, int key) {
    int succ;
    if (node == 0) return 0;
    if (key < t->nodes[node].key) {
        t->nodes[node].left = bst_delete_node(t, t->nodes[node].left, key);
    } else if (key > t->nodes[node].key) {
        t->nodes[node].right = bst_delete_node(t, t->nodes[node].right, key);
    } else {
        // Found the node to delete
        if (t->nodes[node].left == 0 && t->nodes[node].right == 0) {
            t->nodes[node].used = 0;
            return 0;
        }
        if (t->nodes[node].left == 0) {
            int r = t->nodes[node].right;
            t->nodes[node].used = 0;
            return r;
        }
        if (t->nodes[node].right == 0) {
            int l = t->nodes[node].left;
            t->nodes[node].used = 0;
            return l;
        }
        // Two children: find inorder successor
        succ = bst_find_min(t, t->nodes[node].right);
        t->nodes[node].key = t->nodes[succ].key;
        t->nodes[node].right = bst_delete_node(t, t->nodes[node].right, t->nodes[succ].key);
    }
    return node;
}

void bst_delete(struct BST *t, int key) {
    t->root = bst_delete_node(t, t->root, key);
}

// Collect inorder traversal into array
void bst_inorder_collect(struct BST *t, int node, int *out, int *pos) {
    if (node == 0) return;
    bst_inorder_collect(t, t->nodes[node].left, out, pos);
    out[*pos] = t->nodes[node].key;
    *pos = *pos + 1;
    bst_inorder_collect(t, t->nodes[node].right, out, pos);
}

void print_inorder(struct BST *t, char *label) {
    int buf[64];
    int n = 0;
    int i;
    bst_inorder_collect(t, t->root, buf, &n);
    printf("%s", label);
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", buf[i]);
    }
    printf("\n");
}

int bst_count(struct BST *t, int node) {
    if (node == 0) return 0;
    return 1 + bst_count(t, t->nodes[node].left) + bst_count(t, t->nodes[node].right);
}

int main() {
    struct BST tree;
    bst_init(&tree);

    // Insert elements
    bst_insert(&tree, 50);
    bst_insert(&tree, 30);
    bst_insert(&tree, 70);
    bst_insert(&tree, 20);
    bst_insert(&tree, 40);
    bst_insert(&tree, 60);
    bst_insert(&tree, 80);

    print_inorder(&tree, "Inorder: ");
    // EXPECT: Inorder: 20 30 40 50 60 70 80

    printf("Count: %d\n", bst_count(&tree, tree.root));
    // EXPECT: Count: 7

    // Search tests
    printf("Search 40: %d\n", bst_search(&tree, 40));
    // EXPECT: Search 40: 1
    printf("Search 25: %d\n", bst_search(&tree, 25));
    // EXPECT: Search 25: 0
    printf("Search 70: %d\n", bst_search(&tree, 70));
    // EXPECT: Search 70: 1

    // Delete leaf node
    bst_delete(&tree, 20);
    print_inorder(&tree, "After del 20: ");
    // EXPECT: After del 20: 30 40 50 60 70 80

    // Delete node with one child
    bst_delete(&tree, 30);
    print_inorder(&tree, "After del 30: ");
    // EXPECT: After del 30: 40 50 60 70 80

    // Delete node with two children (root)
    bst_delete(&tree, 50);
    print_inorder(&tree, "After del 50: ");
    // EXPECT: After del 50: 40 60 70 80

    printf("Count after deletions: %d\n", bst_count(&tree, tree.root));
    // EXPECT: Count after deletions: 4

    // Insert more and verify
    bst_insert(&tree, 55);
    bst_insert(&tree, 65);
    bst_insert(&tree, 10);
    print_inorder(&tree, "Final: ");
    // EXPECT: Final: 10 40 55 60 65 70 80

    printf("Final count: %d\n", bst_count(&tree, tree.root));
    // EXPECT: Final count: 7

    printf("Search 55: %d\n", bst_search(&tree, 55));
    // EXPECT: Search 55: 1
    printf("Search 50: %d\n", bst_search(&tree, 50));
    // EXPECT: Search 50: 0

    // Test: insert duplicates (should be ignored)
    bst_insert(&tree, 55);
    bst_insert(&tree, 10);
    printf("After dup inserts count: %d\n", bst_count(&tree, tree.root));
    // EXPECT: After dup inserts count: 7

    // Delete non-existent key
    bst_delete(&tree, 999);
    printf("After del nonexist count: %d\n", bst_count(&tree, tree.root));
    // EXPECT: After del nonexist count: 7

    return 0;
}
