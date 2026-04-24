int printf(const char *fmt, ...);
// EXPECT: inorder: 10 20 25 30 35 40 45 50 55 60 65 70 75 80 85 \npreorder: 50 30 20 10 25 40 35 45 70 60 55 65 80 75 85 \npostorder: 10 25 20 35 45 40 30 55 65 60 75 85 80 70 50 \nsum=745\nheight=4\ncount=15\nfind(40)=1\nfind(99)=0\nmin=10
// Test 210: Recursive tree traversal (array-based binary tree)

// Array-based binary tree: node i has children at 2i+1 and 2i+2
// tree[i] = value, -1 means empty node
int tree[15];

void init_tree(void) {
    tree[0] = 50;
    tree[1] = 30;
    tree[2] = 70;
    tree[3] = 20;
    tree[4] = 40;
    tree[5] = 60;
    tree[6] = 80;
    tree[7] = 10;
    tree[8] = 25;
    tree[9] = 35;
    tree[10] = 45;
    tree[11] = 55;
    tree[12] = 65;
    tree[13] = 75;
    tree[14] = 85;
}

void inorder(int idx) {
    if (idx >= 15) return;
    if (tree[idx] == -1) return;
    inorder(2 * idx + 1);
    printf("%d ", tree[idx]);
    inorder(2 * idx + 2);
}

void preorder(int idx) {
    if (idx >= 15) return;
    if (tree[idx] == -1) return;
    printf("%d ", tree[idx]);
    preorder(2 * idx + 1);
    preorder(2 * idx + 2);
}

void postorder(int idx) {
    if (idx >= 15) return;
    if (tree[idx] == -1) return;
    postorder(2 * idx + 1);
    postorder(2 * idx + 2);
    printf("%d ", tree[idx]);
}

int tree_sum(int idx) {
    if (idx >= 15) return 0;
    if (tree[idx] == -1) return 0;
    return tree[idx] + tree_sum(2 * idx + 1) + tree_sum(2 * idx + 2);
}

int tree_height(int idx) {
    if (idx >= 15) return 0;
    if (tree[idx] == -1) return 0;
    int lh = tree_height(2 * idx + 1);
    int rh = tree_height(2 * idx + 2);
    int max_h = lh;
    if (rh > max_h) max_h = rh;
    return 1 + max_h;
}

int tree_count(int idx) {
    if (idx >= 15) return 0;
    if (tree[idx] == -1) return 0;
    return 1 + tree_count(2 * idx + 1) + tree_count(2 * idx + 2);
}

int tree_find(int idx, int val) {
    if (idx >= 15) return 0;
    if (tree[idx] == -1) return 0;
    if (tree[idx] == val) return 1;
    if (tree_find(2 * idx + 1, val)) return 1;
    return tree_find(2 * idx + 2, val);
}

int tree_min(int idx) {
    if (idx >= 15) return 99999;
    if (tree[idx] == -1) return 99999;
    int l = tree_min(2 * idx + 1);
    int r = tree_min(2 * idx + 2);
    int m = tree[idx];
    if (l < m) m = l;
    if (r < m) m = r;
    return m;
}

int main(void) {
    init_tree();

    printf("inorder: ");
    inorder(0);
    printf("\n");

    printf("preorder: ");
    preorder(0);
    printf("\n");

    printf("postorder: ");
    postorder(0);
    printf("\n");

    printf("sum=%d\n", tree_sum(0));
    printf("height=%d\n", tree_height(0));
    printf("count=%d\n", tree_count(0));
    printf("find(40)=%d\n", tree_find(0, 40));
    printf("find(99)=%d\n", tree_find(0, 99));
    printf("min=%d\n", tree_min(0));

    return 0;
}
