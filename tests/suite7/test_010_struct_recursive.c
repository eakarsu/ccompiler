int printf(const char *fmt, ...);

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode pool[15];
int pool_next;

void pool_init(void) {
    pool_next = 0;
    int i;
    for (i = 0; i < 15; i = i + 1) {
        pool[i].val = 0;
        pool[i].left = 0;
        pool[i].right = 0;
    }
}

struct TreeNode *pool_alloc(int val) {
    if (pool_next >= 15) return 0;
    struct TreeNode *n = &pool[pool_next];
    pool_next = pool_next + 1;
    n->val = val;
    n->left = 0;
    n->right = 0;
    return n;
}

void bst_insert(struct TreeNode **root, int val) {
    if (*root == 0) {
        *root = pool_alloc(val);
        return;
    }
    if (val < (*root)->val) {
        bst_insert(&(*root)->left, val);
    } else if (val > (*root)->val) {
        bst_insert(&(*root)->right, val);
    }
}

int bst_find(struct TreeNode *root, int val) {
    if (root == 0) return 0;
    if (val == root->val) return 1;
    if (val < root->val) return bst_find(root->left, val);
    return bst_find(root->right, val);
}

int tree_sum(struct TreeNode *root) {
    if (root == 0) return 0;
    return root->val + tree_sum(root->left) + tree_sum(root->right);
}

int tree_count(struct TreeNode *root) {
    if (root == 0) return 0;
    return 1 + tree_count(root->left) + tree_count(root->right);
}

int tree_height(struct TreeNode *root) {
    if (root == 0) return 0;
    int lh = tree_height(root->left);
    int rh = tree_height(root->right);
    if (lh > rh) return 1 + lh;
    return 1 + rh;
}

int tree_min(struct TreeNode *root) {
    if (root->left == 0) return root->val;
    return tree_min(root->left);
}

int tree_max(struct TreeNode *root) {
    if (root->right == 0) return root->val;
    return tree_max(root->right);
}

int tree_leaf_count(struct TreeNode *root) {
    if (root == 0) return 0;
    if (root->left == 0 && root->right == 0) return 1;
    return tree_leaf_count(root->left) + tree_leaf_count(root->right);
}

/* In-order traversal printing */
void tree_inorder_helper(struct TreeNode *root, int *first) {
    if (root == 0) return;
    tree_inorder_helper(root->left, first);
    if (*first) {
        *first = 0;
    } else {
        printf(" ");
    }
    printf("%d", root->val);
    tree_inorder_helper(root->right, first);
}

void tree_inorder(struct TreeNode *root) {
    int first = 1;
    tree_inorder_helper(root, &first);
}

/* Mirror the tree */
void tree_mirror(struct TreeNode *root) {
    if (root == 0) return;
    struct TreeNode *tmp = root->left;
    root->left = root->right;
    root->right = tmp;
    tree_mirror(root->left);
    tree_mirror(root->right);
}

int main(void) {
    pool_init();

    struct TreeNode *root = 0;
    bst_insert(&root, 50);
    bst_insert(&root, 30);
    bst_insert(&root, 70);
    bst_insert(&root, 20);
    bst_insert(&root, 40);
    bst_insert(&root, 60);
    bst_insert(&root, 80);

    // EXPECT: root=50
    printf("root=%d\n", root->val);
    // EXPECT: count=7
    printf("count=%d\n", tree_count(root));
    // EXPECT: sum=350
    printf("sum=%d\n", tree_sum(root));
    // EXPECT: height=3
    printf("height=%d\n", tree_height(root));

    // EXPECT: min=20
    printf("min=%d\n", tree_min(root));
    // EXPECT: max=80
    printf("max=%d\n", tree_max(root));

    // EXPECT: find 40=1
    printf("find 40=%d\n", bst_find(root, 40));
    // EXPECT: find 99=0
    printf("find 99=%d\n", bst_find(root, 99));
    // EXPECT: find 50=1
    printf("find 50=%d\n", bst_find(root, 50));

    // EXPECT: leaves=4
    printf("leaves=%d\n", tree_leaf_count(root));

    // EXPECT: inorder=20 30 40 50 60 70 80
    printf("inorder=");
    tree_inorder(root);
    printf("\n");

    /* Add more nodes */
    bst_insert(&root, 25);
    bst_insert(&root, 35);
    // EXPECT: count after=9
    printf("count after=%d\n", tree_count(root));
    // EXPECT: sum after=410
    printf("sum after=%d\n", tree_sum(root));
    // EXPECT: height after=4
    printf("height after=%d\n", tree_height(root));

    /* Mirror tree and check */
    tree_mirror(root);
    // EXPECT: mirror root left=70
    printf("mirror root left=%d\n", root->left->val);
    // EXPECT: mirror root right=30
    printf("mirror root right=%d\n", root->right->val);
    // EXPECT: mirror max now left=80
    printf("mirror max now left=%d\n", root->left->left->val);

    return 0;
}
