int printf(const char *fmt, ...);

// B-tree insert and search (array-based, order 3)
// Each node has up to 2 keys, 3 children

struct BNode {
    int keys[4];
    int nkeys;
    int children[5];
    int is_leaf;
};

struct BTree {
    struct BNode nodes[64];
    int nnodes;
    int root;
};

void btree_init(struct BTree *t) {
    int i;
    for (i = 0; i < 64; i++) {
        t->nodes[i].nkeys = 0;
        t->nodes[i].is_leaf = 1;
        int j;
        for (j = 0; j < 5; j++) t->nodes[i].children[j] = -1;
        for (j = 0; j < 4; j++) t->nodes[i].keys[j] = 0;
    }
    t->nnodes = 1;
    t->root = 0;
}

int btree_search(struct BTree *t, int node, int key) {
    if (node < 0) return 0;
    struct BNode *n = &t->nodes[node];
    int i = 0;
    while (i < n->nkeys && key > n->keys[i]) i++;
    if (i < n->nkeys && key == n->keys[i]) return 1;
    if (n->is_leaf) return 0;
    return btree_search(t, n->children[i], key);
}

// Simple insert into non-full leaf (simplified B-tree)
void btree_insert_nonfull(struct BTree *t, int node, int key) {
    struct BNode *n = &t->nodes[node];
    if (n->is_leaf) {
        int i = n->nkeys - 1;
        while (i >= 0 && n->keys[i] > key) {
            n->keys[i + 1] = n->keys[i];
            i--;
        }
        n->keys[i + 1] = key;
        n->nkeys++;
    } else {
        int i = n->nkeys - 1;
        while (i >= 0 && n->keys[i] > key) i--;
        i++;
        if (t->nodes[n->children[i]].nkeys == 3) {
            // Split child
            int child = n->children[i];
            int newnode = t->nnodes;
            t->nnodes++;
            t->nodes[newnode].is_leaf = t->nodes[child].is_leaf;
            t->nodes[newnode].nkeys = 1;
            t->nodes[newnode].keys[0] = t->nodes[child].keys[2];
            int midkey = t->nodes[child].keys[1];
            t->nodes[child].nkeys = 1;
            if (!t->nodes[child].is_leaf) {
                t->nodes[newnode].children[0] = t->nodes[child].children[2];
                t->nodes[newnode].children[1] = t->nodes[child].children[3];
                t->nodes[child].children[2] = -1;
                t->nodes[child].children[3] = -1;
            }
            int j = n->nkeys;
            while (j > i) {
                n->children[j + 1] = n->children[j];
                j--;
            }
            n->children[i + 1] = newnode;
            j = n->nkeys - 1;
            while (j >= i) {
                n->keys[j + 1] = n->keys[j];
                j--;
            }
            n->keys[i] = midkey;
            n->nkeys++;
            if (key > midkey) i++;
        }
        btree_insert_nonfull(t, n->children[i], key);
    }
}

void btree_insert(struct BTree *t, int key) {
    struct BNode *r = &t->nodes[t->root];
    if (r->nkeys == 3) {
        int newroot = t->nnodes;
        t->nnodes++;
        t->nodes[newroot].is_leaf = 0;
        t->nodes[newroot].nkeys = 0;
        t->nodes[newroot].children[0] = t->root;
        t->root = newroot;
        // Split old root
        int child = t->nodes[newroot].children[0];
        int newnode = t->nnodes;
        t->nnodes++;
        t->nodes[newnode].is_leaf = t->nodes[child].is_leaf;
        t->nodes[newnode].nkeys = 1;
        t->nodes[newnode].keys[0] = t->nodes[child].keys[2];
        int midkey = t->nodes[child].keys[1];
        t->nodes[child].nkeys = 1;
        t->nodes[newroot].keys[0] = midkey;
        t->nodes[newroot].nkeys = 1;
        t->nodes[newroot].children[1] = newnode;
        if (key > midkey) {
            btree_insert_nonfull(t, newnode, key);
        } else {
            btree_insert_nonfull(t, child, key);
        }
    } else {
        btree_insert_nonfull(t, t->root, key);
    }
}

int main() {
    struct BTree tree;
    btree_init(&tree);

    int vals[10];
    vals[0] = 10; vals[1] = 20; vals[2] = 5;
    vals[3] = 15; vals[4] = 25; vals[5] = 30;
    vals[6] = 3; vals[7] = 8; vals[8] = 12;
    vals[9] = 22;

    int i;
    for (i = 0; i < 10; i++) {
        btree_insert(&tree, vals[i]);
    }
    printf("Inserted 10 keys into B-tree\n"); // EXPECT: Inserted 10 keys into B-tree

    // Search for existing keys
    int found_count = 0;
    for (i = 0; i < 10; i++) {
        if (btree_search(&tree, tree.root, vals[i])) found_count++;
    }
    printf("Found %d of 10 keys\n", found_count); // EXPECT: Found 10 of 10 keys

    // Search for non-existing keys
    int nf = 0;
    int missing[5];
    missing[0] = 1; missing[1] = 7; missing[2] = 13; missing[3] = 50; missing[4] = 99;
    for (i = 0; i < 5; i++) {
        if (!btree_search(&tree, tree.root, missing[i])) nf++;
    }
    printf("Correctly missed %d of 5 absent keys\n", nf); // EXPECT: Correctly missed 5 of 5 absent keys

    printf("Tree nodes used: %d\n", tree.nnodes); // EXPECT: Tree nodes used: 4
    printf("Root node keys: %d\n", tree.nodes[tree.root].nkeys); // EXPECT: Root node keys: 2

    // Verify ordering via in-order traversal count
    int search_hits = 0;
    for (i = 1; i <= 30; i++) {
        if (btree_search(&tree, tree.root, i)) search_hits++;
    }
    printf("Total keys in range 1..30: %d\n", search_hits); // EXPECT: Total keys in range 1..30: 10

    printf("B-tree test passed\n"); // EXPECT: B-tree test passed
    return 0;
}
