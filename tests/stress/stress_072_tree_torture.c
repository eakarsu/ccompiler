// stress_072_tree_torture.c - Binary tree torture test
// EXPECT: insert_count=30
// EXPECT: bst_valid=1
// EXPECT: height=5
// EXPECT: leaves=15
// EXPECT: inorder=1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
// EXPECT: preorder_root=16
// EXPECT: postorder_first=1
// EXPECT: delete_leaf_ok=1
// EXPECT: delete_one_child_ok=1
// EXPECT: delete_two_child_ok=1
// EXPECT: after_delete_count=27
// EXPECT: still_bst=1
// EXPECT: min_val=3
// EXPECT: max_val=30

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

typedef struct TNode {
    int val;
    struct TNode *left;
    struct TNode *right;
} TNode;

TNode *new_tnode(int v) {
    TNode *n = (TNode *)malloc(sizeof(TNode));
    n->val = v;
    n->left = 0;
    n->right = 0;
    return n;
}

TNode *bst_insert(TNode *root, int v) {
    if (!root) return new_tnode(v);
    if (v < root->val)
        root->left = bst_insert(root->left, v);
    else if (v > root->val)
        root->right = bst_insert(root->right, v);
    return root;
}

int bst_count(TNode *root) {
    if (!root) return 0;
    return 1 + bst_count(root->left) + bst_count(root->right);
}

int bst_valid_helper(TNode *root, int lo, int hi) {
    if (!root) return 1;
    if (root->val <= lo || root->val >= hi) return 0;
    return bst_valid_helper(root->left, lo, root->val) &&
           bst_valid_helper(root->right, root->val, hi);
}

int bst_valid(TNode *root) {
    return bst_valid_helper(root, -999999, 999999);
}

int tree_height(TNode *root) {
    if (!root) return 0;
    int lh = tree_height(root->left);
    int rh = tree_height(root->right);
    return 1 + (lh > rh ? lh : rh);
}

int count_leaves(TNode *root) {
    if (!root) return 0;
    if (!root->left && !root->right) return 1;
    return count_leaves(root->left) + count_leaves(root->right);
}

void inorder(TNode *root, int *arr, int *idx) {
    if (!root) return;
    inorder(root->left, arr, idx);
    arr[*idx] = root->val;
    *idx = *idx + 1;
    inorder(root->right, arr, idx);
}

void preorder(TNode *root, int *arr, int *idx) {
    if (!root) return;
    arr[*idx] = root->val;
    *idx = *idx + 1;
    preorder(root->left, arr, idx);
    preorder(root->right, arr, idx);
}

void postorder(TNode *root, int *arr, int *idx) {
    if (!root) return;
    postorder(root->left, arr, idx);
    postorder(root->right, arr, idx);
    arr[*idx] = root->val;
    *idx = *idx + 1;
}

TNode *bst_min_node(TNode *root) {
    while (root && root->left) root = root->left;
    return root;
}

TNode *bst_max_node(TNode *root) {
    while (root && root->right) root = root->right;
    return root;
}

TNode *bst_delete(TNode *root, int v) {
    if (!root) return 0;
    if (v < root->val) {
        root->left = bst_delete(root->left, v);
    } else if (v > root->val) {
        root->right = bst_delete(root->right, v);
    } else {
        // Found node to delete
        if (!root->left && !root->right) {
            free(root);
            return 0;
        }
        if (!root->left) {
            TNode *tmp = root->right;
            free(root);
            return tmp;
        }
        if (!root->right) {
            TNode *tmp = root->left;
            free(root);
            return tmp;
        }
        // Two children: replace with inorder successor
        TNode *succ = bst_min_node(root->right);
        root->val = succ->val;
        root->right = bst_delete(root->right, succ->val);
    }
    return root;
}

void free_tree(TNode *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(void) {
    TNode *root = 0;
    // Insert values in a specific order to create a known tree shape
    // Insert: 16, 8, 24, 4, 12, 20, 28, 2, 6, 10, 14, 18, 22, 26, 30
    // Then: 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29
    int ins1[] = {16, 8, 24, 4, 12, 20, 28, 2, 6, 10, 14, 18, 22, 26, 30};
    int ins2[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29};
    int i;

    for (i = 0; i < 15; i++) root = bst_insert(root, ins1[i]);
    for (i = 0; i < 15; i++) root = bst_insert(root, ins2[i]);

    printf("insert_count=%d\n", bst_count(root));
    printf("bst_valid=%d\n", bst_valid(root));
    printf("height=%d\n", tree_height(root));
    printf("leaves=%d\n", count_leaves(root));

    // Inorder traversal
    int arr[30];
    int idx = 0;
    inorder(root, arr, &idx);
    printf("inorder=");
    for (i = 0; i < idx; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");

    // Preorder: first element is root
    idx = 0;
    preorder(root, arr, &idx);
    printf("preorder_root=%d\n", arr[0]);

    // Postorder: first element should be leftmost leaf
    idx = 0;
    postorder(root, arr, &idx);
    printf("postorder_first=%d\n", arr[0]);

    // Delete a leaf node (1)
    root = bst_delete(root, 1);
    printf("delete_leaf_ok=%d\n", bst_valid(root));

    // Delete node with one child (2 now has no left child, only right=3)
    root = bst_delete(root, 2);
    printf("delete_one_child_ok=%d\n", bst_valid(root));

    // Delete node with two children (24 has left=20, right=28)
    root = bst_delete(root, 24);
    printf("delete_two_child_ok=%d\n", bst_valid(root));

    printf("after_delete_count=%d\n", bst_count(root));
    printf("still_bst=%d\n", bst_valid(root));

    TNode *mn = bst_min_node(root);
    TNode *mx = bst_max_node(root);
    printf("min_val=%d\n", mn ? mn->val : -1);
    printf("max_val=%d\n", mx ? mx->val : -1);

    free_tree(root);
    return 0;
}
