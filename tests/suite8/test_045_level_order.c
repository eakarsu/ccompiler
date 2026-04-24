int printf(const char *fmt, ...);

/* Level-order (BFS) traversal of a binary tree using a queue */

struct TreeNode {
    int val;
    int left;
    int right;
};

struct TreeNode pool[32];
int pool_cnt;

int make_node(int val) {
    int idx;
    idx = pool_cnt;
    pool[idx].val = val;
    pool[idx].left = -1;
    pool[idx].right = -1;
    pool_cnt = pool_cnt + 1;
    return idx;
}

/* Simple queue for BFS */
int queue[64];
int q_front;
int q_back;

void q_init(void) {
    q_front = 0;
    q_back = 0;
}

void q_push(int val) {
    queue[q_back] = val;
    q_back = q_back + 1;
}

int q_pop(void) {
    int val;
    val = queue[q_front];
    q_front = q_front + 1;
    return val;
}

int q_empty(void) {
    return q_front == q_back;
}

int q_size(void) {
    return q_back - q_front;
}

void level_order(int root) {
    int cur;
    int level_size;
    int i;
    int level;

    if (root == -1) return;
    q_init();
    q_push(root);
    level = 0;

    while (q_empty() == 0) {
        level_size = q_size();
        printf("level %d:", level);
        for (i = 0; i < level_size; i++) {
            cur = q_pop();
            printf(" %d", pool[cur].val);
            if (pool[cur].left != -1) {
                q_push(pool[cur].left);
            }
            if (pool[cur].right != -1) {
                q_push(pool[cur].right);
            }
        }
        printf("\n");
        level = level + 1;
    }
}

int width_at_level(int root, int target_level, int cur_level) {
    if (root == -1) return 0;
    if (cur_level == target_level) return 1;
    return width_at_level(pool[root].left, target_level, cur_level + 1) +
           width_at_level(pool[root].right, target_level, cur_level + 1);
}

int max_width(int root) {
    int h;
    int lv;
    int w;
    int best;
    int lh; int rh;

    /* compute height */
    if (root == -1) return 0;
    best = 0;
    /* get height iteratively via level order */
    h = 0;
    q_init();
    q_push(root);
    while (q_empty() == 0) {
        int sz;
        int j;
        sz = q_size();
        if (sz > best) best = sz;
        for (j = 0; j < sz; j++) {
            int nd;
            nd = q_pop();
            if (pool[nd].left != -1) q_push(pool[nd].left);
            if (pool[nd].right != -1) q_push(pool[nd].right);
        }
    }
    return best;
}

int main(void) {
    int r;
    int n2; int n3; int n4; int n5; int n6; int n7; int n8;
    int w;

    pool_cnt = 0;

    /*         1
              / \
             2   3
            / \   \
           4   5   6
          /       / \
         7       8   9
    */
    r  = make_node(1);
    n2 = make_node(2);
    n3 = make_node(3);
    n4 = make_node(4);
    n5 = make_node(5);
    n6 = make_node(6);
    n7 = make_node(7);
    n8 = make_node(8);
    int n9;
    n9 = make_node(9);

    pool[r].left = n2;  pool[r].right = n3;
    pool[n2].left = n4; pool[n2].right = n5;
    pool[n3].right = n6;
    pool[n4].left = n7;
    pool[n6].left = n8; pool[n6].right = n9;

    // EXPECT: level 0: 1
    // EXPECT: level 1: 2 3
    // EXPECT: level 2: 4 5 6
    // EXPECT: level 3: 7 8 9
    level_order(r);

    w = max_width(r);
    // EXPECT: max_width=3
    printf("max_width=%d\n", w);

    // EXPECT: width_level0=1
    printf("width_level0=%d\n", width_at_level(r, 0, 0));
    // EXPECT: width_level1=2
    printf("width_level1=%d\n", width_at_level(r, 1, 0));
    // EXPECT: width_level2=3
    printf("width_level2=%d\n", width_at_level(r, 2, 0));
    // EXPECT: width_level3=3
    printf("width_level3=%d\n", width_at_level(r, 3, 0));

    return 0;
}
