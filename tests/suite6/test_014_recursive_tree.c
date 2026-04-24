int printf(const char *fmt, ...);

struct TreeNode {
    int value;
    int left;
    int right;
};

struct TreeNode nodes[64];
int node_count;

int new_node(int val) {
    int idx = node_count;
    nodes[idx].value = val;
    nodes[idx].left = -1;
    nodes[idx].right = -1;
    node_count = node_count + 1;
    return idx;
}

int tree_insert(int root, int val) {
    if (root == -1) return new_node(val);
    if (val < nodes[root].value) {
        nodes[root].left = tree_insert(nodes[root].left, val);
    } else if (val > nodes[root].value) {
        nodes[root].right = tree_insert(nodes[root].right, val);
    }
    return root;
}

int tree_search(int root, int val) {
    if (root == -1) return 0;
    if (nodes[root].value == val) return 1;
    if (val < nodes[root].value) return tree_search(nodes[root].left, val);
    return tree_search(nodes[root].right, val);
}

int tree_height(int root) {
    if (root == -1) return 0;
    int lh = tree_height(nodes[root].left);
    int rh = tree_height(nodes[root].right);
    if (lh > rh) return lh + 1;
    return rh + 1;
}

int tree_count(int root) {
    if (root == -1) return 0;
    return 1 + tree_count(nodes[root].left) + tree_count(nodes[root].right);
}

int tree_min(int root) {
    if (nodes[root].left == -1) return nodes[root].value;
    return tree_min(nodes[root].left);
}

int tree_max(int root) {
    if (nodes[root].right == -1) return nodes[root].value;
    return tree_max(nodes[root].right);
}

int tree_sum(int root) {
    if (root == -1) return 0;
    return nodes[root].value + tree_sum(nodes[root].left) + tree_sum(nodes[root].right);
}

int inorder_buf[64];
int inorder_len;

void tree_inorder_collect(int root) {
    if (root == -1) return;
    tree_inorder_collect(nodes[root].left);
    inorder_buf[inorder_len] = nodes[root].value;
    inorder_len = inorder_len + 1;
    tree_inorder_collect(nodes[root].right);
}

void tree_inorder(int root) {
    inorder_len = 0;
    tree_inorder_collect(root);
    int i = 0;
    while (i < inorder_len) {
        if (i > 0) printf(" ");
        printf("%d", inorder_buf[i]);
        i = i + 1;
    }
}

int preorder_buf[64];
int preorder_len;

void tree_preorder_collect(int root) {
    if (root == -1) return;
    preorder_buf[preorder_len] = nodes[root].value;
    preorder_len = preorder_len + 1;
    tree_preorder_collect(nodes[root].left);
    tree_preorder_collect(nodes[root].right);
}

void tree_preorder(int root) {
    preorder_len = 0;
    tree_preorder_collect(root);
    int i = 0;
    while (i < preorder_len) {
        if (i > 0) printf(" ");
        printf("%d", preorder_buf[i]);
        i = i + 1;
    }
}

int postorder_buf[64];
int postorder_len;

void tree_postorder_collect(int root) {
    if (root == -1) return;
    tree_postorder_collect(nodes[root].left);
    tree_postorder_collect(nodes[root].right);
    postorder_buf[postorder_len] = nodes[root].value;
    postorder_len = postorder_len + 1;
}

void tree_postorder(int root) {
    postorder_len = 0;
    tree_postorder_collect(root);
    int i = 0;
    while (i < postorder_len) {
        if (i > 0) printf(" ");
        printf("%d", postorder_buf[i]);
        i = i + 1;
    }
}

int tree_count_leaves(int root) {
    if (root == -1) return 0;
    if (nodes[root].left == -1 && nodes[root].right == -1) return 1;
    return tree_count_leaves(nodes[root].left) + tree_count_leaves(nodes[root].right);
}

int tree_is_bst_helper(int root, int min_val, int max_val) {
    if (root == -1) return 1;
    if (nodes[root].value <= min_val) return 0;
    if (nodes[root].value >= max_val) return 0;
    return tree_is_bst_helper(nodes[root].left, min_val, nodes[root].value) &&
           tree_is_bst_helper(nodes[root].right, nodes[root].value, max_val);
}

int tree_is_bst(int root) {
    return tree_is_bst_helper(root, -100000, 100000);
}

int main(void) {
    node_count = 0;
    int root = -1;
    root = tree_insert(root, 50);
    root = tree_insert(root, 30);
    root = tree_insert(root, 70);
    root = tree_insert(root, 20);
    root = tree_insert(root, 40);
    root = tree_insert(root, 60);
    root = tree_insert(root, 80);
    root = tree_insert(root, 10);
    root = tree_insert(root, 25);
    root = tree_insert(root, 90);

    // EXPECT: count=10
    printf("count=%d\n", tree_count(root));
    // EXPECT: height=4
    printf("height=%d\n", tree_height(root));
    // EXPECT: search(40)=1
    printf("search(40)=%d\n", tree_search(root, 40));
    // EXPECT: search(55)=0
    printf("search(55)=%d\n", tree_search(root, 55));
    // EXPECT: min=10
    printf("min=%d\n", tree_min(root));
    // EXPECT: max=90
    printf("max=%d\n", tree_max(root));
    // EXPECT: sum=475
    printf("sum=%d\n", tree_sum(root));

    // EXPECT: inorder=10 20 25 30 40 50 60 70 80 90
    printf("inorder=");
    tree_inorder(root);
    printf("\n");

    // EXPECT: preorder=50 30 20 10 25 40 70 60 80 90
    printf("preorder=");
    tree_preorder(root);
    printf("\n");

    // EXPECT: postorder=10 25 20 40 30 60 90 80 70 50
    printf("postorder=");
    tree_postorder(root);
    printf("\n");

    // EXPECT: leaves=5
    printf("leaves=%d\n", tree_count_leaves(root));
    // EXPECT: is_bst=1
    printf("is_bst=%d\n", tree_is_bst(root));

    root = tree_insert(root, 35);
    root = tree_insert(root, 65);
    root = tree_insert(root, 15);
    // EXPECT: count2=13
    printf("count2=%d\n", tree_count(root));
    // EXPECT: height2=5
    printf("height2=%d\n", tree_height(root));
    // EXPECT: sum2=590
    printf("sum2=%d\n", tree_sum(root));

    return 0;
}
