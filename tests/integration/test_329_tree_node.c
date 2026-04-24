int printf(const char *fmt, ...);
// EXPECT: inorder: 20 30 40 50 60 70 80 \ncount: 7\nmin: 20\nmax: 80\nheight: 3\nfind 40: 1\nfind 55: 0\nfind 80: 1
// Test: tree node with structs

struct TreeNode {
    int value;
    int left;
    int right;
    int used;
};

struct Tree {
    struct TreeNode nodes[15];
    int count;
    int root;
};

void tree_init(struct Tree *t) {
    t->count = 0;
    t->root = -1;
    int i;
    for (i = 0; i < 15; i++) {
        t->nodes[i].used = 0;
        t->nodes[i].left = -1;
        t->nodes[i].right = -1;
    }
}

int tree_alloc(struct Tree *t, int value) {
    if (t->count >= 15) return -1;
    int idx = t->count;
    t->nodes[idx].value = value;
    t->nodes[idx].left = -1;
    t->nodes[idx].right = -1;
    t->nodes[idx].used = 1;
    t->count = t->count + 1;
    return idx;
}

void tree_insert(struct Tree *t, int value) {
    int newIdx = tree_alloc(t, value);
    if (newIdx < 0) return;
    if (t->root == -1) {
        t->root = newIdx;
        return;
    }
    int curr = t->root;
    while (1) {
        if (value < t->nodes[curr].value) {
            if (t->nodes[curr].left == -1) {
                t->nodes[curr].left = newIdx;
                return;
            }
            curr = t->nodes[curr].left;
        } else {
            if (t->nodes[curr].right == -1) {
                t->nodes[curr].right = newIdx;
                return;
            }
            curr = t->nodes[curr].right;
        }
    }
}

int tree_find(struct Tree *t, int value) {
    int curr = t->root;
    while (curr != -1) {
        if (value == t->nodes[curr].value) return 1;
        if (value < t->nodes[curr].value)
            curr = t->nodes[curr].left;
        else
            curr = t->nodes[curr].right;
    }
    return 0;
}

int tree_min(struct Tree *t) {
    int curr = t->root;
    while (t->nodes[curr].left != -1) {
        curr = t->nodes[curr].left;
    }
    return t->nodes[curr].value;
}

int tree_max(struct Tree *t) {
    int curr = t->root;
    while (t->nodes[curr].right != -1) {
        curr = t->nodes[curr].right;
    }
    return t->nodes[curr].value;
}

void tree_inorder_helper(struct Tree *t, int idx) {
    if (idx == -1) return;
    tree_inorder_helper(t, t->nodes[idx].left);
    printf("%d ", t->nodes[idx].value);
    tree_inorder_helper(t, t->nodes[idx].right);
}

void tree_inorder(struct Tree *t) {
    tree_inorder_helper(t, t->root);
    printf("\n");
}

int tree_height_helper(struct Tree *t, int idx) {
    if (idx == -1) return 0;
    int lh = tree_height_helper(t, t->nodes[idx].left);
    int rh = tree_height_helper(t, t->nodes[idx].right);
    int max = lh > rh ? lh : rh;
    return max + 1;
}

int tree_height(struct Tree *t) {
    return tree_height_helper(t, t->root);
}

int main(void) {
    struct Tree t;
    tree_init(&t);
    tree_insert(&t, 50);
    tree_insert(&t, 30);
    tree_insert(&t, 70);
    tree_insert(&t, 20);
    tree_insert(&t, 40);
    tree_insert(&t, 60);
    tree_insert(&t, 80);

    printf("inorder: "); tree_inorder(&t);
    printf("count: %d\n", t.count);
    printf("min: %d\n", tree_min(&t));
    printf("max: %d\n", tree_max(&t));
    printf("height: %d\n", tree_height(&t));
    printf("find 40: %d\n", tree_find(&t, 40));
    printf("find 55: %d\n", tree_find(&t, 55));
    printf("find 80: %d\n", tree_find(&t, 80));

    return 0;
}
