int printf(const char *fmt, ...);

// Simplified B-tree of order 3 (max 2 keys, 3 children per node)
// Using array-based storage for nodes

struct BTreeNode {
    int keys[2];
    int children[3];
    int num_keys;
    int is_leaf;
};

struct BTree {
    struct BTreeNode nodes[20];
    int num_nodes;
    int root;
};

void btree_init(struct BTree *t) {
    t->num_nodes = 1;
    t->root = 0;
    t->nodes[0].num_keys = 0;
    t->nodes[0].is_leaf = 1;
    t->nodes[0].children[0] = -1;
    t->nodes[0].children[1] = -1;
    t->nodes[0].children[2] = -1;
}

int btree_alloc_node(struct BTree *t) {
    int idx;
    idx = t->num_nodes;
    t->num_nodes = t->num_nodes + 1;
    t->nodes[idx].num_keys = 0;
    t->nodes[idx].is_leaf = 1;
    t->nodes[idx].children[0] = -1;
    t->nodes[idx].children[1] = -1;
    t->nodes[idx].children[2] = -1;
    return idx;
}

int btree_search(struct BTree *t, int node, int key) {
    int i;
    struct BTreeNode *n;
    if (node < 0) return 0;
    n = &t->nodes[node];
    i = 0;
    while (i < n->num_keys && key > n->keys[i]) {
        i = i + 1;
    }
    if (i < n->num_keys && key == n->keys[i]) {
        return 1;
    }
    if (n->is_leaf) return 0;
    return btree_search(t, n->children[i], key);
}

void btree_split_child(struct BTree *t, int parent, int idx) {
    struct BTreeNode *p;
    struct BTreeNode *child;
    struct BTreeNode *new_node;
    int new_idx;
    int mid_key;
    int j;

    p = &t->nodes[parent];
    child = &t->nodes[p->children[idx]];
    new_idx = btree_alloc_node(t);

    // Re-fetch pointers after alloc (array may not move but be safe)
    p = &t->nodes[parent];
    child = &t->nodes[p->children[idx]];
    new_node = &t->nodes[new_idx];

    mid_key = child->keys[1];
    new_node->is_leaf = child->is_leaf;
    new_node->num_keys = 0;

    // Child keeps only first key
    child->num_keys = 1;

    // If not leaf, new_node gets child's right child
    if (!child->is_leaf) {
        new_node->children[0] = child->children[2];
        child->children[2] = -1;
    }

    // Shift parent keys and children right
    for (j = p->num_keys; j > idx; j = j - 1) {
        p->keys[j] = p->keys[j - 1];
        p->children[j + 1] = p->children[j];
    }
    p->keys[idx] = mid_key;
    p->children[idx + 1] = new_idx;
    p->num_keys = p->num_keys + 1;
}

void btree_insert_nonfull(struct BTree *t, int node, int key) {
    struct BTreeNode *n;
    int i;
    n = &t->nodes[node];

    if (n->is_leaf) {
        i = n->num_keys - 1;
        while (i >= 0 && key < n->keys[i]) {
            n->keys[i + 1] = n->keys[i];
            i = i - 1;
        }
        n->keys[i + 1] = key;
        n->num_keys = n->num_keys + 1;
    } else {
        i = n->num_keys - 1;
        while (i >= 0 && key < n->keys[i]) {
            i = i - 1;
        }
        i = i + 1;
        if (t->nodes[n->children[i]].num_keys == 2) {
            btree_split_child(t, node, i);
            n = &t->nodes[node];
            if (key > n->keys[i]) {
                i = i + 1;
            }
        }
        btree_insert_nonfull(t, n->children[i], key);
    }
}

void btree_insert(struct BTree *t, int key) {
    struct BTreeNode *root;
    int old_root;
    int new_root;

    root = &t->nodes[t->root];
    if (root->num_keys == 2) {
        old_root = t->root;
        new_root = btree_alloc_node(t);
        t->root = new_root;
        t->nodes[new_root].is_leaf = 0;
        t->nodes[new_root].children[0] = old_root;
        btree_split_child(t, new_root, 0);
        btree_insert_nonfull(t, new_root, key);
    } else {
        btree_insert_nonfull(t, t->root, key);
    }
}

int btree_count_nodes(struct BTree *t) {
    return t->num_nodes;
}

int main() {
    struct BTree t;

    btree_init(&t);
    printf("Nodes after init: %d\n", btree_count_nodes(&t));
    // EXPECT: Nodes after init: 1

    btree_insert(&t, 10);
    btree_insert(&t, 20);
    printf("Search 10: %d\n", btree_search(&t, t.root, 10));
    // EXPECT: Search 10: 1
    printf("Search 20: %d\n", btree_search(&t, t.root, 20));
    // EXPECT: Search 20: 1
    printf("Search 15: %d\n", btree_search(&t, t.root, 15));
    // EXPECT: Search 15: 0

    // Insert 30 causes split (root has 2 keys: 10, 20)
    btree_insert(&t, 30);
    printf("Nodes after split: %d\n", btree_count_nodes(&t));
    // EXPECT: Nodes after split: 3
    printf("Search 30: %d\n", btree_search(&t, t.root, 30));
    // EXPECT: Search 30: 1

    btree_insert(&t, 5);
    btree_insert(&t, 15);
    printf("Search 5: %d\n", btree_search(&t, t.root, 5));
    // EXPECT: Search 5: 1
    printf("Search 15: %d\n", btree_search(&t, t.root, 15));
    // EXPECT: Search 15: 1

    btree_insert(&t, 25);
    printf("Search 25: %d\n", btree_search(&t, t.root, 25));
    // EXPECT: Search 25: 1

    btree_insert(&t, 35);
    printf("Search 35: %d\n", btree_search(&t, t.root, 35));
    // EXPECT: Search 35: 1

    printf("Total nodes: %d\n", btree_count_nodes(&t));
    // EXPECT: Total nodes: 7

    printf("Search 99: %d\n", btree_search(&t, t.root, 99));
    // EXPECT: Search 99: 0

    printf("Root keys: %d\n", t.nodes[t.root].num_keys);
    // EXPECT: Root keys: 1

    return 0;
}
