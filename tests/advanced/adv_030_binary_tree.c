// EXPECT: === Insert & Inorder ===
// EXPECT: inorder: 2 3 5 7 8 10 12
// EXPECT: === Search ===
// EXPECT: search 5: found
// EXPECT: search 7: found
// EXPECT: search 1: not found
// EXPECT: search 11: not found
// EXPECT: === Node Count ===
// EXPECT: count: 7
// EXPECT: === Tree Height ===
// EXPECT: height: 3
// EXPECT: === Min & Max ===
// EXPECT: min: 2
// EXPECT: max: 12
// EXPECT: === Single Node ===
// EXPECT: inorder: 42
// EXPECT: height: 1
// EXPECT: count: 1
// EXPECT: === Left-Skewed ===
// EXPECT: inorder: 1 2 3 4 5
// EXPECT: height: 5
// EXPECT: === Right-Skewed ===
// EXPECT: inorder: 1 2 3 4 5
// EXPECT: height: 5
// EXPECT: === Duplicate Insert ===
// EXPECT: inorder: 5 10 15
// EXPECT: count: 3

int printf(const char *fmt, ...);
void *malloc(long size);
void free(void *ptr);

struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode *create_node(int data) {
    struct TreeNode *n = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    n->data = data;
    n->left = 0;
    n->right = 0;
    return n;
}

struct TreeNode *insert(struct TreeNode *root, int data) {
    if (root == 0) {
        return create_node(data);
    }
    if (data < root->data) {
        root->left = insert(root->left, data);
    } else if (data > root->data) {
        root->right = insert(root->right, data);
    }
    // duplicate: do nothing
    return root;
}

int search(struct TreeNode *root, int data) {
    if (root == 0) return 0;
    if (data == root->data) return 1;
    if (data < root->data) return search(root->left, data);
    return search(root->right, data);
}

int first_printed;

void inorder_helper(struct TreeNode *root) {
    if (root == 0) return;
    inorder_helper(root->left);
    if (first_printed) {
        printf(" %d", root->data);
    } else {
        printf("%d", root->data);
        first_printed = 1;
    }
    inorder_helper(root->right);
}

void print_inorder(struct TreeNode *root) {
    first_printed = 0;
    printf("inorder: ");
    inorder_helper(root);
    printf("\n");
}

int tree_height(struct TreeNode *root) {
    if (root == 0) return 0;
    int lh = tree_height(root->left);
    int rh = tree_height(root->right);
    if (lh > rh) return lh + 1;
    return rh + 1;
}

int node_count(struct TreeNode *root) {
    if (root == 0) return 0;
    return 1 + node_count(root->left) + node_count(root->right);
}

int tree_min(struct TreeNode *root) {
    struct TreeNode *cur = root;
    while (cur->left != 0) {
        cur = cur->left;
    }
    return cur->data;
}

int tree_max(struct TreeNode *root) {
    struct TreeNode *cur = root;
    while (cur->right != 0) {
        cur = cur->right;
    }
    return cur->data;
}

void free_tree(struct TreeNode *root) {
    if (root == 0) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

void test_insert_inorder(void) {
    printf("=== Insert & Inorder ===\n");
    struct TreeNode *root = 0;
    root = insert(root, 7);
    root = insert(root, 3);
    root = insert(root, 10);
    root = insert(root, 5);
    root = insert(root, 2);
    root = insert(root, 8);
    root = insert(root, 12);
    print_inorder(root);
    free_tree(root);
}

void test_search(void) {
    printf("=== Search ===\n");
    struct TreeNode *root = 0;
    root = insert(root, 7);
    root = insert(root, 3);
    root = insert(root, 10);
    root = insert(root, 5);
    root = insert(root, 2);
    root = insert(root, 8);
    root = insert(root, 12);
    if (search(root, 5)) printf("search 5: found\n");
    else printf("search 5: not found\n");
    if (search(root, 7)) printf("search 7: found\n");
    else printf("search 7: not found\n");
    if (search(root, 1)) printf("search 1: found\n");
    else printf("search 1: not found\n");
    if (search(root, 11)) printf("search 11: found\n");
    else printf("search 11: not found\n");
    free_tree(root);
}

void test_count(void) {
    printf("=== Node Count ===\n");
    struct TreeNode *root = 0;
    root = insert(root, 7);
    root = insert(root, 3);
    root = insert(root, 10);
    root = insert(root, 5);
    root = insert(root, 2);
    root = insert(root, 8);
    root = insert(root, 12);
    printf("count: %d\n", node_count(root));
    free_tree(root);
}

void test_height(void) {
    printf("=== Tree Height ===\n");
    struct TreeNode *root = 0;
    root = insert(root, 7);
    root = insert(root, 3);
    root = insert(root, 10);
    root = insert(root, 5);
    root = insert(root, 2);
    root = insert(root, 8);
    root = insert(root, 12);
    printf("height: %d\n", tree_height(root));
    free_tree(root);
}

void test_min_max(void) {
    printf("=== Min & Max ===\n");
    struct TreeNode *root = 0;
    root = insert(root, 7);
    root = insert(root, 3);
    root = insert(root, 10);
    root = insert(root, 5);
    root = insert(root, 2);
    root = insert(root, 8);
    root = insert(root, 12);
    printf("min: %d\n", tree_min(root));
    printf("max: %d\n", tree_max(root));
    free_tree(root);
}

void test_single(void) {
    printf("=== Single Node ===\n");
    struct TreeNode *root = 0;
    root = insert(root, 42);
    print_inorder(root);
    printf("height: %d\n", tree_height(root));
    printf("count: %d\n", node_count(root));
    free_tree(root);
}

void test_left_skew(void) {
    printf("=== Left-Skewed ===\n");
    struct TreeNode *root = 0;
    root = insert(root, 5);
    root = insert(root, 4);
    root = insert(root, 3);
    root = insert(root, 2);
    root = insert(root, 1);
    print_inorder(root);
    printf("height: %d\n", tree_height(root));
    free_tree(root);
}

void test_right_skew(void) {
    printf("=== Right-Skewed ===\n");
    struct TreeNode *root = 0;
    root = insert(root, 1);
    root = insert(root, 2);
    root = insert(root, 3);
    root = insert(root, 4);
    root = insert(root, 5);
    print_inorder(root);
    printf("height: %d\n", tree_height(root));
    free_tree(root);
}

void test_duplicate(void) {
    printf("=== Duplicate Insert ===\n");
    struct TreeNode *root = 0;
    root = insert(root, 10);
    root = insert(root, 5);
    root = insert(root, 15);
    root = insert(root, 10);
    root = insert(root, 5);
    root = insert(root, 15);
    print_inorder(root);
    printf("count: %d\n", node_count(root));
    free_tree(root);
}

int main(void) {
    test_insert_inorder();
    test_search();
    test_count();
    test_height();
    test_min_max();
    test_single();
    test_left_skew();
    test_right_skew();
    test_duplicate();
    return 0;
}
