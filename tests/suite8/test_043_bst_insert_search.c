int printf(const char *fmt, ...);

/* BST using array-based node pool */

struct BSTNode {
    int key;
    int left;
    int right;
};

struct BSTNode pool[32];
int pool_count;
int bst_root;

int bst_new_node(int key) {
    int idx;
    idx = pool_count;
    pool[idx].key = key;
    pool[idx].left = -1;
    pool[idx].right = -1;
    pool_count = pool_count + 1;
    return idx;
}

int bst_insert(int root, int key) {
    int cur;
    int parent;
    int child;
    int go_left;

    child = bst_new_node(key);
    if (root == -1) return child;

    cur = root;
    parent = -1;
    go_left = 0;

    while (cur != -1) {
        parent = cur;
        if (key < pool[cur].key) {
            go_left = 1;
            cur = pool[cur].left;
        } else {
            go_left = 0;
            cur = pool[cur].right;
        }
    }

    if (go_left) {
        pool[parent].left = child;
    } else {
        pool[parent].right = child;
    }

    return root;
}

int bst_search(int root, int key) {
    int cur;
    cur = root;
    while (cur != -1) {
        if (key == pool[cur].key) return 1;
        if (key < pool[cur].key) {
            cur = pool[cur].left;
        } else {
            cur = pool[cur].right;
        }
    }
    return 0;
}

int bst_min(int root) {
    int cur;
    cur = root;
    while (pool[cur].left != -1) {
        cur = pool[cur].left;
    }
    return pool[cur].key;
}

int bst_max(int root) {
    int cur;
    cur = root;
    while (pool[cur].right != -1) {
        cur = pool[cur].right;
    }
    return pool[cur].key;
}

void bst_inorder(int idx) {
    if (idx == -1) return;
    bst_inorder(pool[idx].left);
    // EXPECT: 5
    // EXPECT: 10
    // EXPECT: 15
    // EXPECT: 20
    // EXPECT: 25
    // EXPECT: 30
    // EXPECT: 35
    // EXPECT: 40
    // EXPECT: 50
    printf("%d\n", pool[idx].key);
    bst_inorder(pool[idx].right);
}

int bst_count(int idx) {
    if (idx == -1) return 0;
    return 1 + bst_count(pool[idx].left) + bst_count(pool[idx].right);
}

int bst_height(int idx) {
    int lh;
    int rh;
    if (idx == -1) return 0;
    lh = bst_height(pool[idx].left);
    rh = bst_height(pool[idx].right);
    if (lh > rh) return lh + 1;
    return rh + 1;
}

int main(void) {
    int found;

    pool_count = 0;
    bst_root = -1;

    /* Insert: 20, 10, 30, 5, 15, 25, 35, 50, 40 */
    bst_root = bst_insert(bst_root, 20);
    bst_root = bst_insert(bst_root, 10);
    bst_root = bst_insert(bst_root, 30);
    bst_root = bst_insert(bst_root, 5);
    bst_root = bst_insert(bst_root, 15);
    bst_root = bst_insert(bst_root, 25);
    bst_root = bst_insert(bst_root, 35);
    bst_root = bst_insert(bst_root, 50);
    bst_root = bst_insert(bst_root, 40);

    bst_inorder(bst_root);

    // EXPECT: search 20: 1
    printf("search 20: %d\n", bst_search(bst_root, 20));
    // EXPECT: search 15: 1
    printf("search 15: %d\n", bst_search(bst_root, 15));
    // EXPECT: search 99: 0
    printf("search 99: %d\n", bst_search(bst_root, 99));
    // EXPECT: search 1: 0
    printf("search 1: %d\n", bst_search(bst_root, 1));

    // EXPECT: min=5
    printf("min=%d\n", bst_min(bst_root));
    // EXPECT: max=50
    printf("max=%d\n", bst_max(bst_root));

    // EXPECT: count=9
    printf("count=%d\n", bst_count(bst_root));
    // EXPECT: height=5
    printf("height=%d\n", bst_height(bst_root));

    return 0;
}
