int printf(const char *fmt, ...);

struct TreeNode {
    int key;
    int value;
    int left;
    int right;
    int used;
};

struct BST {
    struct TreeNode nodes[32];
    int root;
    int next_free;
    int size;
};

void bst_init(struct BST *t) {
    t->root = -1;
    t->next_free = 0;
    t->size = 0;
    int i = 0;
    while (i < 32) {
        t->nodes[i].key = 0;
        t->nodes[i].value = 0;
        t->nodes[i].left = -1;
        t->nodes[i].right = -1;
        t->nodes[i].used = 0;
        i = i + 1;
    }
}

int bst_new_node(struct BST *t, int key, int value) {
    if (t->next_free >= 32) return -1;
    int idx = t->next_free;
    t->next_free = t->next_free + 1;
    t->nodes[idx].key = key;
    t->nodes[idx].value = value;
    t->nodes[idx].left = -1;
    t->nodes[idx].right = -1;
    t->nodes[idx].used = 1;
    return idx;
}

void bst_insert(struct BST *t, int key, int value) {
    int node = bst_new_node(t, key, value);
    if (node == -1) return;
    t->size = t->size + 1;

    if (t->root == -1) {
        t->root = node;
        return;
    }

    int cur = t->root;
    while (1) {
        if (key < t->nodes[cur].key) {
            if (t->nodes[cur].left == -1) {
                t->nodes[cur].left = node;
                return;
            }
            cur = t->nodes[cur].left;
        } else if (key > t->nodes[cur].key) {
            if (t->nodes[cur].right == -1) {
                t->nodes[cur].right = node;
                return;
            }
            cur = t->nodes[cur].right;
        } else {
            t->nodes[cur].value = value;
            t->size = t->size - 1;
            return;
        }
    }
}

int bst_search(struct BST *t, int key) {
    int cur = t->root;
    while (cur != -1) {
        if (key == t->nodes[cur].key) {
            return t->nodes[cur].value;
        } else if (key < t->nodes[cur].key) {
            cur = t->nodes[cur].left;
        } else {
            cur = t->nodes[cur].right;
        }
    }
    return -1;
}

int inorder_buf[32];
int inorder_count;

void bst_inorder(struct BST *t, int node) {
    if (node == -1) return;
    bst_inorder(t, t->nodes[node].left);
    inorder_buf[inorder_count] = t->nodes[node].key;
    inorder_count = inorder_count + 1;
    bst_inorder(t, t->nodes[node].right);
}

int bst_height(struct BST *t, int node) {
    if (node == -1) return 0;
    int lh = bst_height(t, t->nodes[node].left);
    int rh = bst_height(t, t->nodes[node].right);
    if (lh > rh) return lh + 1;
    return rh + 1;
}

int bst_min(struct BST *t) {
    if (t->root == -1) return -1;
    int cur = t->root;
    while (t->nodes[cur].left != -1) {
        cur = t->nodes[cur].left;
    }
    return t->nodes[cur].key;
}

int bst_max(struct BST *t) {
    if (t->root == -1) return -1;
    int cur = t->root;
    while (t->nodes[cur].right != -1) {
        cur = t->nodes[cur].right;
    }
    return t->nodes[cur].key;
}

int main(void) {
    struct BST tree;
    bst_init(&tree);

    bst_insert(&tree, 50, 500);
    bst_insert(&tree, 30, 300);
    bst_insert(&tree, 70, 700);
    bst_insert(&tree, 20, 200);
    bst_insert(&tree, 40, 400);
    bst_insert(&tree, 60, 600);
    bst_insert(&tree, 80, 800);

    // EXPECT: size=7
    printf("size=%d\n", tree.size);

    // EXPECT: search50=500
    printf("search50=%d\n", bst_search(&tree, 50));
    // EXPECT: search30=300
    printf("search30=%d\n", bst_search(&tree, 30));
    // EXPECT: search80=800
    printf("search80=%d\n", bst_search(&tree, 80));
    // EXPECT: search99=-1
    printf("search99=%d\n", bst_search(&tree, 99));

    inorder_count = 0;
    bst_inorder(&tree, tree.root);
    // EXPECT: inorder_count=7
    printf("inorder_count=%d\n", inorder_count);
    // EXPECT: inorder0=20
    printf("inorder0=%d\n", inorder_buf[0]);
    // EXPECT: inorder3=50
    printf("inorder3=%d\n", inorder_buf[3]);
    // EXPECT: inorder6=80
    printf("inorder6=%d\n", inorder_buf[6]);

    int sorted = 1;
    int i = 0;
    while (i < inorder_count - 1) {
        if (inorder_buf[i] >= inorder_buf[i + 1]) {
            sorted = 0;
        }
        i = i + 1;
    }
    // EXPECT: sorted=1
    printf("sorted=%d\n", sorted);

    // EXPECT: height=3
    printf("height=%d\n", bst_height(&tree, tree.root));
    // EXPECT: min=20
    printf("min=%d\n", bst_min(&tree));
    // EXPECT: max=80
    printf("max=%d\n", bst_max(&tree));

    bst_insert(&tree, 10, 100);
    bst_insert(&tree, 25, 250);
    // EXPECT: size2=9
    printf("size2=%d\n", tree.size);
    // EXPECT: search25=250
    printf("search25=%d\n", bst_search(&tree, 25));
    // EXPECT: min2=10
    printf("min2=%d\n", bst_min(&tree));

    bst_insert(&tree, 50, 999);
    // EXPECT: updated50=999
    printf("updated50=%d\n", bst_search(&tree, 50));
    // EXPECT: size_no_dup=9
    printf("size_no_dup=%d\n", tree.size);

    return 0;
}
