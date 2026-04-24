int printf(const char *fmt, ...);

// AVL tree with rotation counting

struct AVLNode {
    int key;
    int height;
    int left;
    int right;
};

struct AVLTree {
    struct AVLNode nodes[64];
    int size;
    int root;
    int left_rotations;
    int right_rotations;
};

void avl_init(struct AVLTree *t) {
    t->size = 0;
    t->root = -1;
    t->left_rotations = 0;
    t->right_rotations = 0;
}

int avl_height(struct AVLTree *t, int idx) {
    if (idx == -1) return 0;
    return t->nodes[idx].height;
}

int avl_max(int a, int b) {
    if (a > b) return a;
    return b;
}

int avl_balance(struct AVLTree *t, int idx) {
    if (idx == -1) return 0;
    return avl_height(t, t->nodes[idx].left) - avl_height(t, t->nodes[idx].right);
}

void avl_update_height(struct AVLTree *t, int idx) {
    int lh = avl_height(t, t->nodes[idx].left);
    int rh = avl_height(t, t->nodes[idx].right);
    t->nodes[idx].height = 1 + avl_max(lh, rh);
}

int avl_rotate_right(struct AVLTree *t, int y) {
    int x = t->nodes[y].left;
    int t2 = t->nodes[x].right;
    t->nodes[x].right = y;
    t->nodes[y].left = t2;
    avl_update_height(t, y);
    avl_update_height(t, x);
    t->right_rotations = t->right_rotations + 1;
    return x;
}

int avl_rotate_left(struct AVLTree *t, int x) {
    int y = t->nodes[x].right;
    int t2 = t->nodes[y].left;
    t->nodes[y].left = x;
    t->nodes[x].right = t2;
    avl_update_height(t, x);
    avl_update_height(t, y);
    t->left_rotations = t->left_rotations + 1;
    return y;
}

int avl_insert_node(struct AVLTree *t, int idx, int key) {
    int bal;
    if (idx == -1) {
        int n = t->size;
        t->size = t->size + 1;
        t->nodes[n].key = key;
        t->nodes[n].height = 1;
        t->nodes[n].left = -1;
        t->nodes[n].right = -1;
        return n;
    }
    if (key < t->nodes[idx].key) {
        t->nodes[idx].left = avl_insert_node(t, t->nodes[idx].left, key);
    } else if (key > t->nodes[idx].key) {
        t->nodes[idx].right = avl_insert_node(t, t->nodes[idx].right, key);
    } else {
        return idx;
    }
    avl_update_height(t, idx);
    bal = avl_balance(t, idx);

    if (bal > 1 && key < t->nodes[t->nodes[idx].left].key) {
        return avl_rotate_right(t, idx);
    }
    if (bal < -1 && key > t->nodes[t->nodes[idx].right].key) {
        return avl_rotate_left(t, idx);
    }
    if (bal > 1 && key > t->nodes[t->nodes[idx].left].key) {
        t->nodes[idx].left = avl_rotate_left(t, t->nodes[idx].left);
        return avl_rotate_right(t, idx);
    }
    if (bal < -1 && key < t->nodes[t->nodes[idx].right].key) {
        t->nodes[idx].right = avl_rotate_right(t, t->nodes[idx].right);
        return avl_rotate_left(t, idx);
    }
    return idx;
}

void avl_insert(struct AVLTree *t, int key) {
    t->root = avl_insert_node(t, t->root, key);
}

int avl_inorder_count;

void avl_inorder(struct AVLTree *t, int idx) {
    if (idx == -1) return;
    avl_inorder(t, t->nodes[idx].left);
    if (avl_inorder_count > 0) printf(" ");
    printf("%d", t->nodes[idx].key);
    avl_inorder_count = avl_inorder_count + 1;
    avl_inorder(t, t->nodes[idx].right);
}

int main() {
    struct AVLTree tree;
    avl_init(&tree);

    avl_insert(&tree, 10);
    avl_insert(&tree, 20);
    avl_insert(&tree, 30);
    printf("after 10,20,30 rotL: %d rotR: %d\n", tree.left_rotations, tree.right_rotations);
    // EXPECT: after 10,20,30 rotL: 1 rotR: 0

    avl_insert(&tree, 40);
    avl_insert(&tree, 50);
    printf("after 40,50 rotL: %d rotR: %d\n", tree.left_rotations, tree.right_rotations);
    // EXPECT: after 40,50 rotL: 2 rotR: 0

    avl_insert(&tree, 25);
    printf("after 25 rotL: %d rotR: %d\n", tree.left_rotations, tree.right_rotations);
    // EXPECT: after 25 rotL: 3 rotR: 1

    printf("inorder: ");
    avl_inorder_count = 0;
    avl_inorder(&tree, tree.root);
    printf("\n");
    // EXPECT: inorder: 10 20 25 30 40 50

    printf("root key: %d\n", tree.nodes[tree.root].key);
    // EXPECT: root key: 30
    printf("tree height: %d\n", avl_height(&tree, tree.root));
    // EXPECT: tree height: 3
    printf("node count: %d\n", tree.size);
    // EXPECT: node count: 6

    avl_insert(&tree, 5);
    avl_insert(&tree, 3);
    avl_insert(&tree, 1);
    printf("after 5,3,1 rotL: %d rotR: %d\n", tree.left_rotations, tree.right_rotations);
    // EXPECT: after 5,3,1 rotL: 3 rotR: 3
    printf("final height: %d\n", avl_height(&tree, tree.root));
    // EXPECT: final height: 4

    return 0;
}
