int printf(const char *fmt, ...);

/* Binary tree stored in a flat array.
   For node at index i:
     left child  = 2*i + 1
     right child = 2*i + 2
     parent      = (i - 1) / 2
   We use -1 as "empty" sentinel.
*/

int tree[31];   /* max 5 levels (indices 0..30) */
int tree_size;

void tree_init(void) {
    int i;
    for (i = 0; i < 31; i++) {
        tree[i] = -1;
    }
    tree_size = 0;
}

void tree_set_root(int val) {
    tree[0] = val;
    tree_size = 1;
}

void tree_set_left(int parent_idx, int val) {
    int idx;
    idx = 2 * parent_idx + 1;
    if (idx < 31) {
        tree[idx] = val;
        if (idx >= tree_size) {
            tree_size = idx + 1;
        }
    }
}

void tree_set_right(int parent_idx, int val) {
    int idx;
    idx = 2 * parent_idx + 2;
    if (idx < 31) {
        tree[idx] = val;
        if (idx >= tree_size) {
            tree_size = idx + 1;
        }
    }
}

int tree_get_left(int parent_idx) {
    int idx;
    idx = 2 * parent_idx + 1;
    if (idx < 31) return tree[idx];
    return -1;
}

int tree_get_right(int parent_idx) {
    int idx;
    idx = 2 * parent_idx + 2;
    if (idx < 31) return tree[idx];
    return -1;
}

int tree_get_parent(int child_idx) {
    int idx;
    if (child_idx <= 0) return -1;
    idx = (child_idx - 1) / 2;
    return tree[idx];
}

int tree_is_leaf(int idx) {
    int left_idx;
    left_idx = 2 * idx + 1;
    if (left_idx >= 31) return 1;
    if (tree[left_idx] == -1 && tree[left_idx + 1] == -1) return 1;
    return 0;
}

int tree_height_helper(int idx) {
    int lh;
    int rh;
    int left_idx;
    int right_idx;
    if (idx >= 31 || tree[idx] == -1) return 0;
    left_idx = 2 * idx + 1;
    right_idx = 2 * idx + 2;
    lh = tree_height_helper(left_idx);
    rh = tree_height_helper(right_idx);
    if (lh > rh) return lh + 1;
    return rh + 1;
}

int tree_count(int idx) {
    int left_idx;
    int right_idx;
    if (idx >= 31 || tree[idx] == -1) return 0;
    left_idx = 2 * idx + 1;
    right_idx = 2 * idx + 2;
    return 1 + tree_count(left_idx) + tree_count(right_idx);
}

int main(void) {
    int h;
    int cnt;
    int i;

    tree_init();
    tree_set_root(10);
    tree_set_left(0, 20);
    tree_set_right(0, 30);
    tree_set_left(1, 40);
    tree_set_right(1, 50);
    tree_set_left(2, 60);
    tree_set_right(2, 70);

    // EXPECT: root=10
    printf("root=%d\n", tree[0]);
    // EXPECT: left_of_root=20
    printf("left_of_root=%d\n", tree_get_left(0));
    // EXPECT: right_of_root=30
    printf("right_of_root=%d\n", tree_get_right(0));
    // EXPECT: parent_of_3=20
    printf("parent_of_3=%d\n", tree_get_parent(3));
    // EXPECT: parent_of_4=20
    printf("parent_of_4=%d\n", tree_get_parent(4));
    // EXPECT: parent_of_5=30
    printf("parent_of_5=%d\n", tree_get_parent(5));

    // EXPECT: leaf_0=0
    printf("leaf_0=%d\n", tree_is_leaf(0));
    // EXPECT: leaf_3=1
    printf("leaf_3=%d\n", tree_is_leaf(3));
    // EXPECT: leaf_6=1
    printf("leaf_6=%d\n", tree_is_leaf(6));

    h = tree_height_helper(0);
    // EXPECT: height=3
    printf("height=%d\n", h);

    cnt = tree_count(0);
    // EXPECT: count=7
    printf("count=%d\n", cnt);

    /* Print all values level by level */
    // EXPECT: level: 10 20 30 40 50 60 70
    printf("level:");
    for (i = 0; i < 31; i++) {
        if (tree[i] != -1) {
            printf(" %d", tree[i]);
        }
    }
    printf("\n");

    return 0;
}
