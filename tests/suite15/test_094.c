int printf(const char *fmt, ...);

/* B-tree search and range query
   Order 3 B-tree (max 2 keys, max 3 children per node).
   Static array-based, pre-built tree for testing. */

struct BTreeNode {
    int keys[2];
    int nkeys;
    int children[3]; /* indices into node pool, -1 = no child */
    int is_leaf;
};

struct BTreeNode pool[16];
int pool_size;

int new_node(int leaf) {
    int idx = pool_size;
    pool[idx].nkeys = 0;
    pool[idx].children[0] = -1;
    pool[idx].children[1] = -1;
    pool[idx].children[2] = -1;
    pool[idx].is_leaf = leaf;
    pool_size = pool_size + 1;
    return idx;
}

void add_key(int node, int key) {
    int n = pool[node].nkeys;
    /* Insert in sorted order */
    if (n == 0 || key > pool[node].keys[n - 1]) {
        pool[node].keys[n] = key;
    } else {
        /* shift right */
        pool[node].keys[1] = pool[node].keys[0];
        pool[node].keys[0] = key;
    }
    pool[node].nkeys = n + 1;
}

int search(int node, int key) {
    int i;
    if (node < 0) return 0;

    for (i = 0; i < pool[node].nkeys; i++) {
        if (key == pool[node].keys[i]) return 1;
        if (key < pool[node].keys[i]) {
            return search(pool[node].children[i], key);
        }
    }
    return search(pool[node].children[pool[node].nkeys], key);
}

/* Count keys in range [lo, hi] inclusive */
int range_count(int node, int lo, int hi) {
    int count;
    int i;
    if (node < 0) return 0;

    count = 0;
    for (i = 0; i < pool[node].nkeys; i++) {
        if (pool[node].keys[i] >= lo && pool[node].keys[i] <= hi) {
            count = count + 1;
        }
    }

    if (!pool[node].is_leaf) {
        for (i = 0; i < pool[node].nkeys; i++) {
            if (lo <= pool[node].keys[i]) {
                count = count + range_count(pool[node].children[i], lo, hi);
            }
        }
        /* rightmost child */
        if (hi >= pool[node].keys[pool[node].nkeys - 1]) {
            count = count + range_count(pool[node].children[pool[node].nkeys], lo, hi);
        }
    }

    return count;
}

/* Collect all keys in sorted order (in-order traversal) */
int collected[32];
int collect_count;

void collect_inorder(int node) {
    int i;
    if (node < 0) return;

    if (pool[node].is_leaf) {
        for (i = 0; i < pool[node].nkeys; i++) {
            collected[collect_count] = pool[node].keys[i];
            collect_count = collect_count + 1;
        }
        return;
    }

    for (i = 0; i < pool[node].nkeys; i++) {
        collect_inorder(pool[node].children[i]);
        collected[collect_count] = pool[node].keys[i];
        collect_count = collect_count + 1;
    }
    collect_inorder(pool[node].children[pool[node].nkeys]);
}

int tree_height(int node) {
    int h;
    if (node < 0) return 0;
    if (pool[node].is_leaf) return 1;
    h = tree_height(pool[node].children[0]);
    return h + 1;
}

/*
   Build tree:
            [20, 40]
           /    |    \
       [5,10] [25,30] [50,60]

   Contains: 5, 10, 20, 25, 30, 40, 50, 60
*/

int build_tree(void) {
    int root;
    int c0;
    int c1;
    int c2;

    pool_size = 0;

    root = new_node(0);
    add_key(root, 20);
    add_key(root, 40);

    c0 = new_node(1);
    add_key(c0, 5);
    add_key(c0, 10);

    c1 = new_node(1);
    add_key(c1, 25);
    add_key(c1, 30);

    c2 = new_node(1);
    add_key(c2, 50);
    add_key(c2, 60);

    pool[root].children[0] = c0;
    pool[root].children[1] = c1;
    pool[root].children[2] = c2;

    return root;
}

void test_search(int root) {
    printf("=== B-tree Search ===\n");
    // EXPECT: === B-tree Search ===
    printf("Search 5: %d\n", search(root, 5));
    // EXPECT: Search 5: 1
    printf("Search 20: %d\n", search(root, 20));
    // EXPECT: Search 20: 1
    printf("Search 30: %d\n", search(root, 30));
    // EXPECT: Search 30: 1
    printf("Search 60: %d\n", search(root, 60));
    // EXPECT: Search 60: 1
    printf("Search 15: %d\n", search(root, 15));
    // EXPECT: Search 15: 0
    printf("Search 35: %d\n", search(root, 35));
    // EXPECT: Search 35: 0
    printf("Search 0: %d\n", search(root, 0));
    // EXPECT: Search 0: 0
    printf("Search 100: %d\n", search(root, 100));
    // EXPECT: Search 100: 0
}

void test_range(int root) {
    printf("=== B-tree Range Query ===\n");
    // EXPECT: === B-tree Range Query ===
    /* Range [1,100]: all 8 keys */
    printf("Range [1,100]: %d\n", range_count(root, 1, 100));
    // EXPECT: Range [1,100]: 8
    /* Range [20,40]: 20, 25, 30, 40 = 4 */
    printf("Range [20,40]: %d\n", range_count(root, 20, 40));
    // EXPECT: Range [20,40]: 4
    /* Range [5,10]: 5, 10 = 2 */
    printf("Range [5,10]: %d\n", range_count(root, 5, 10));
    // EXPECT: Range [5,10]: 2
    /* Range [11,19]: none */
    printf("Range [11,19]: %d\n", range_count(root, 11, 19));
    // EXPECT: Range [11,19]: 0
    /* Range [50,60]: 50, 60 = 2 */
    printf("Range [50,60]: %d\n", range_count(root, 50, 60));
    // EXPECT: Range [50,60]: 2
}

void test_inorder(int root) {
    int i;
    collect_count = 0;
    collect_inorder(root);

    printf("=== B-tree In-order ===\n");
    // EXPECT: === B-tree In-order ===
    printf("Count: %d\n", collect_count);
    // EXPECT: Count: 8
    /* Print sorted keys */
    printf("Keys:");
    for (i = 0; i < collect_count; i++) {
        printf(" %d", collected[i]);
    }
    printf("\n");
    // EXPECT: Keys: 5 10 20 25 30 40 50 60
}

void test_height(int root) {
    printf("=== B-tree Height ===\n");
    // EXPECT: === B-tree Height ===
    printf("Height: %d\n", tree_height(root));
    // EXPECT: Height: 2
}

int main(void) {
    int root = build_tree();
    test_search(root);
    test_range(root);
    test_inorder(root);
    test_height(root);

    printf("All B-tree tests passed!\n");
    // EXPECT: All B-tree tests passed!
    return 0;
}
