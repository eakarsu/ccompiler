int printf(const char *fmt, ...);
// EXPECT: Inorder: 10 20 25 30 40 50 60 70 80 \nSearch 40: 1\nSearch 55: 0\nMin: 10\nMax: 80\nCount: 9\nHeight: 4
// Binary search tree operations with array pool

struct BSTNode {
    int key;
    int left;
    int right;
    int active;
};

struct BST {
    struct BSTNode nodes[30];
    int root;
    int next_free;
};

void bst_init(struct BST *t) {
    int i;
    for (i = 0; i < 30; i++) {
        t->nodes[i].key = 0;
        t->nodes[i].left = -1;
        t->nodes[i].right = -1;
        t->nodes[i].active = 0;
    }
    t->root = -1;
    t->next_free = 0;
}

int bst_alloc(struct BST *t, int key) {
    int idx;
    if (t->next_free >= 30) return -1;
    idx = t->next_free;
    t->next_free++;
    t->nodes[idx].key = key;
    t->nodes[idx].left = -1;
    t->nodes[idx].right = -1;
    t->nodes[idx].active = 1;
    return idx;
}

int bst_insert(struct BST *t, int key) {
    int idx;
    int cur;
    if (t->root == -1) {
        t->root = bst_alloc(t, key);
        return t->root;
    }
    cur = t->root;
    while (1) {
        if (key < t->nodes[cur].key) {
            if (t->nodes[cur].left == -1) {
                idx = bst_alloc(t, key);
                t->nodes[cur].left = idx;
                return idx;
            }
            cur = t->nodes[cur].left;
        } else if (key > t->nodes[cur].key) {
            if (t->nodes[cur].right == -1) {
                idx = bst_alloc(t, key);
                t->nodes[cur].right = idx;
                return idx;
            }
            cur = t->nodes[cur].right;
        } else {
            return cur;
        }
    }
}

int bst_find_min(struct BST *t, int node) {
    while (t->nodes[node].left != -1) {
        node = t->nodes[node].left;
    }
    return node;
}

int bst_find_max(struct BST *t, int node) {
    while (t->nodes[node].right != -1) {
        node = t->nodes[node].right;
    }
    return node;
}

void bst_inorder(struct BST *t, int node) {
    if (node == -1 || !t->nodes[node].active) return;
    bst_inorder(t, t->nodes[node].left);
    printf("%d ", t->nodes[node].key);
    bst_inorder(t, t->nodes[node].right);
}

int bst_search(struct BST *t, int key) {
    int cur = t->root;
    while (cur != -1 && t->nodes[cur].active) {
        if (key == t->nodes[cur].key) return 1;
        if (key < t->nodes[cur].key) cur = t->nodes[cur].left;
        else cur = t->nodes[cur].right;
    }
    return 0;
}

int bst_count(struct BST *t, int node) {
    if (node == -1 || !t->nodes[node].active) return 0;
    return 1 + bst_count(t, t->nodes[node].left) + bst_count(t, t->nodes[node].right);
}

int bst_height(struct BST *t, int node) {
    int lh;
    int rh;
    if (node == -1 || !t->nodes[node].active) return 0;
    lh = bst_height(t, t->nodes[node].left);
    rh = bst_height(t, t->nodes[node].right);
    return 1 + (lh > rh ? lh : rh);
}

int main(void) {
    struct BST t;
    int minN;
    int maxN;
    bst_init(&t);

    bst_insert(&t, 50);
    bst_insert(&t, 30);
    bst_insert(&t, 70);
    bst_insert(&t, 20);
    bst_insert(&t, 40);
    bst_insert(&t, 60);
    bst_insert(&t, 80);
    bst_insert(&t, 10);
    bst_insert(&t, 25);

    printf("Inorder: ");
    bst_inorder(&t, t.root);
    printf("\n");

    printf("Search 40: %d\n", bst_search(&t, 40));
    printf("Search 55: %d\n", bst_search(&t, 55));

    minN = bst_find_min(&t, t.root);
    maxN = bst_find_max(&t, t.root);
    printf("Min: %d\n", t.nodes[minN].key);
    printf("Max: %d\n", t.nodes[maxN].key);
    printf("Count: %d\n", bst_count(&t, t.root));
    printf("Height: %d\n", bst_height(&t, t.root));

    return 0;
}
