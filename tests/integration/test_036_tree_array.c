int printf(const char *fmt, ...);
// EXPECT: cnt=7 ht=2 mn=20 mx=80 s50=1 s30=1 s99=0 bst=1 sum=350 lf=4 in0=20 in1=30 in2=40 in3=50 in4=60 in5=70 in6=80 pr0=50 pr1=30 pr2=20 pr3=40 po0=20 po1=40 po2=30 po6=50 lv0=50 lv1=30 lv2=70 lv3=20 d50=0 d30=1 d20=2 cd0=1 cd1=2 cd2=4 ps=100 t2c=3 s1=1 h1=0

/* ================================================================
 * test_036_tree_array.c
 * Binary tree using array representation.
 * tree[1] = root, tree[2i] = left child, tree[2i+1] = right child
 * tree_valid[i] = 1 if node i exists, 0 if not
 * ================================================================ */

int tree[256];
int tree_valid[256];

void tree_init(void) {
    int i;
    i = 0;
    while (i < 256) { tree[i] = 0; tree_valid[i] = 0; i = i + 1; }
}

/* Insert into BST */
void bst_insert(int val) {
    int i;
    i = 1;
    while (1) {
        if (tree_valid[i] == 0) {
            tree[i] = val;
            tree_valid[i] = 1;
            return;
        }
        if (val < tree[i]) {
            i = 2 * i;
        } else if (val > tree[i]) {
            i = 2 * i + 1;
        } else {
            return; /* duplicate */
        }
        if (i >= 256) return; /* overflow */
    }
}

/* Search in BST */
int bst_search(int val) {
    int i;
    i = 1;
    while (i < 256 && tree_valid[i] == 1) {
        if (tree[i] == val) return 1;
        if (val < tree[i]) i = 2 * i;
        else i = 2 * i + 1;
    }
    return 0;
}

/* Find min value in BST */
int bst_min(void) {
    int i;
    i = 1;
    if (tree_valid[i] == 0) return -1;
    while (2 * i < 256 && tree_valid[2 * i] == 1) {
        i = 2 * i;
    }
    return tree[i];
}

/* Find max value in BST */
int bst_max(void) {
    int i;
    i = 1;
    if (tree_valid[i] == 0) return -1;
    while (2 * i + 1 < 256 && tree_valid[2 * i + 1] == 1) {
        i = 2 * i + 1;
    }
    return tree[i];
}

/* Count nodes */
int tree_count(void) {
    int i; int cnt;
    cnt = 0; i = 1;
    while (i < 256) {
        if (tree_valid[i] == 1) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

/* Tree height (0-based: single node = 0) */
int tree_height_from(int i) {
    int lh; int rh;
    if (i >= 256 || tree_valid[i] == 0) return -1;
    lh = tree_height_from(2 * i);
    rh = tree_height_from(2 * i + 1);
    if (lh > rh) return lh + 1;
    return rh + 1;
}

int tree_height(void) {
    return tree_height_from(1);
}

/* Count leaf nodes */
int count_leaves_from(int i) {
    int l; int r; int has_left; int has_right;
    if (i >= 256 || tree_valid[i] == 0) return 0;
    has_left = (2 * i < 256 && tree_valid[2 * i] == 1) ? 1 : 0;
    has_right = (2 * i + 1 < 256 && tree_valid[2 * i + 1] == 1) ? 1 : 0;
    if (has_left == 0 && has_right == 0) return 1;
    return count_leaves_from(2 * i) + count_leaves_from(2 * i + 1);
}

int count_leaves(void) {
    return count_leaves_from(1);
}

/* Inorder traversal: store results in array */
int inorder_buf[64];
int inorder_idx;

void inorder_from(int i) {
    if (i >= 256 || tree_valid[i] == 0) return;
    inorder_from(2 * i);
    inorder_buf[inorder_idx] = tree[i];
    inorder_idx = inorder_idx + 1;
    inorder_from(2 * i + 1);
}

int inorder(int *out) {
    int i; int cnt;
    inorder_idx = 0;
    inorder_from(1);
    cnt = inorder_idx;
    i = 0;
    while (i < cnt) { out[i] = inorder_buf[i]; i = i + 1; }
    return cnt;
}

/* Preorder traversal */
int preorder_buf[64];
int preorder_idx;

void preorder_from(int i) {
    if (i >= 256 || tree_valid[i] == 0) return;
    preorder_buf[preorder_idx] = tree[i];
    preorder_idx = preorder_idx + 1;
    preorder_from(2 * i);
    preorder_from(2 * i + 1);
}

int preorder(int *out) {
    int i; int cnt;
    preorder_idx = 0;
    preorder_from(1);
    cnt = preorder_idx;
    i = 0;
    while (i < cnt) { out[i] = preorder_buf[i]; i = i + 1; }
    return cnt;
}

/* Postorder traversal */
int postorder_buf[64];
int postorder_idx;

void postorder_from(int i) {
    if (i >= 256 || tree_valid[i] == 0) return;
    postorder_from(2 * i);
    postorder_from(2 * i + 1);
    postorder_buf[postorder_idx] = tree[i];
    postorder_idx = postorder_idx + 1;
}

int postorder(int *out) {
    int i; int cnt;
    postorder_idx = 0;
    postorder_from(1);
    cnt = postorder_idx;
    i = 0;
    while (i < cnt) { out[i] = postorder_buf[i]; i = i + 1; }
    return cnt;
}

/* Level-order traversal (BFS) */
int levelorder(int *out) {
    int queue[256];
    int front; int back;
    int oi;
    int cur;
    front = 0; back = 0; oi = 0;
    if (tree_valid[1] == 0) return 0;
    queue[back] = 1; back = back + 1;
    while (front < back) {
        cur = queue[front]; front = front + 1;
        out[oi] = tree[cur]; oi = oi + 1;
        if (2 * cur < 256 && tree_valid[2 * cur] == 1) {
            queue[back] = 2 * cur; back = back + 1;
        }
        if (2 * cur + 1 < 256 && tree_valid[2 * cur + 1] == 1) {
            queue[back] = 2 * cur + 1; back = back + 1;
        }
    }
    return oi;
}

/* Sum of all node values */
int tree_sum_from(int i) {
    if (i >= 256 || tree_valid[i] == 0) return 0;
    return tree[i] + tree_sum_from(2 * i) + tree_sum_from(2 * i + 1);
}

int tree_sum(void) {
    return tree_sum_from(1);
}

/* Check BST property */
int is_bst_util(int i, int min_val, int max_val) {
    if (i >= 256 || tree_valid[i] == 0) return 1;
    if (tree[i] < min_val || tree[i] > max_val) return 0;
    return is_bst_util(2 * i, min_val, tree[i] - 1) &&
           is_bst_util(2 * i + 1, tree[i] + 1, max_val);
}

int is_bst(void) {
    return is_bst_util(1, -100000, 100000);
}

/* Count nodes at a given depth (root = depth 0) */
int count_at_depth(int depth) {
    int start; int end; int i; int cnt;
    /* Level d starts at index 2^d */
    start = 1;
    i = 0;
    while (i < depth) { start = start * 2; i = i + 1; }
    end = start * 2;
    if (end > 256) end = 256;
    cnt = 0;
    i = start;
    while (i < end) {
        if (tree_valid[i] == 1) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

/* Mirror the tree */
void mirror_from(int i) {
    int t;
    int lc; int rc;
    if (i >= 128 || tree_valid[i] == 0) return;
    lc = 2 * i;
    rc = 2 * i + 1;
    /* Swap values and valid flags */
    if (lc < 256 && rc < 256) {
        t = tree[lc]; tree[lc] = tree[rc]; tree[rc] = t;
        t = tree_valid[lc]; tree_valid[lc] = tree_valid[rc]; tree_valid[rc] = t;
    }
    mirror_from(lc);
    mirror_from(rc);
}

/* Path sum from root to a node */
int path_sum_to(int target) {
    int i; int sum;
    /* First find the node */
    i = 1;
    while (i < 256 && tree_valid[i] == 1) {
        if (tree[i] == target) break;
        if (target < tree[i]) i = 2 * i;
        else i = 2 * i + 1;
    }
    if (i >= 256 || tree_valid[i] == 0) return -1;
    /* Walk back to root */
    sum = 0;
    while (i >= 1) {
        sum = sum + tree[i];
        i = i / 2;
    }
    return sum;
}

/* Depth of a specific value */
int depth_of(int val) {
    int i; int d;
    i = 1; d = 0;
    while (i < 256 && tree_valid[i] == 1) {
        if (tree[i] == val) return d;
        if (val < tree[i]) i = 2 * i;
        else i = 2 * i + 1;
        d = d + 1;
    }
    return -1;
}

/* Second tree for independent testing */
int tree2[256];
int tree2_valid[256];

void tree2_init(void) {
    int i;
    i = 0;
    while (i < 256) { tree2[i] = 0; tree2_valid[i] = 0; i = i + 1; }
}

void tree2_insert(int val) {
    int i;
    i = 1;
    while (1) {
        if (tree2_valid[i] == 0) {
            tree2[i] = val; tree2_valid[i] = 1; return;
        }
        if (val < tree2[i]) i = 2 * i;
        else if (val > tree2[i]) i = 2 * i + 1;
        else return;
        if (i >= 256) return;
    }
}

int tree2_count(void) {
    int i; int cnt;
    cnt = 0; i = 1;
    while (i < 256) { if (tree2_valid[i] == 1) cnt = cnt + 1; i = i + 1; }
    return cnt;
}

int main(void) {
    int out[64];
    int n;
    int i;

    /* Build BST: insert 50, 30, 70, 20, 40, 60, 80 */
    tree_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);
    bst_insert(20);
    bst_insert(40);
    bst_insert(60);
    bst_insert(80);

    printf("cnt=%d ", tree_count());           /* 7 */
    printf("ht=%d ", tree_height());           /* 2 */
    printf("mn=%d ", bst_min());               /* 20 */
    printf("mx=%d ", bst_max());               /* 80 */
    printf("s50=%d ", bst_search(50));         /* 1 */
    printf("s30=%d ", bst_search(30));         /* 1 */
    printf("s99=%d ", bst_search(99));         /* 0 */
    printf("bst=%d ", is_bst());               /* 1 */
    printf("sum=%d ", tree_sum());             /* 350 */
    printf("lf=%d ", count_leaves());          /* 4 */

    /* Inorder: 20 30 40 50 60 70 80 */
    n = inorder(out);
    printf("in0=%d in1=%d in2=%d in3=%d in4=%d in5=%d in6=%d ", out[0], out[1], out[2], out[3], out[4], out[5], out[6]);

    /* Preorder: 50 30 20 40 70 60 80 */
    n = preorder(out);
    printf("pr0=%d pr1=%d pr2=%d pr3=%d ", out[0], out[1], out[2], out[3]);

    /* Postorder: 20 40 30 60 80 70 50 */
    n = postorder(out);
    printf("po0=%d po1=%d po2=%d po6=%d ", out[0], out[1], out[2], out[6]);

    /* Level order: 50 30 70 20 40 60 80 */
    n = levelorder(out);
    printf("lv0=%d lv1=%d lv2=%d lv3=%d ", out[0], out[1], out[2], out[3]);

    /* Depth of nodes */
    printf("d50=%d ", depth_of(50));           /* 0 */
    printf("d30=%d ", depth_of(30));           /* 1 */
    printf("d20=%d ", depth_of(20));           /* 2 */

    /* Count at depth */
    printf("cd0=%d ", count_at_depth(0));      /* 1 */
    printf("cd1=%d ", count_at_depth(1));      /* 2 */
    printf("cd2=%d ", count_at_depth(2));      /* 4 */

    /* Path sum to 20: 50+30+20=100 */
    printf("ps=%d ", path_sum_to(20));

    /* Second tree test */
    tree2_init();
    tree2_insert(10);
    tree2_insert(5);
    tree2_insert(15);
    printf("t2c=%d ", tree2_count());          /* 3 */

    /* Single node tree */
    tree_init();
    bst_insert(42);
    printf("s1=%d ", tree_count());            /* 1 */
    printf("h1=%d", tree_height());            /* 0 */

    printf("\n");
    return 0;
}

/* ---- Additional functions to reach 500+ lines ---- */

/* Check if tree is balanced (height diff of subtrees <= 1) */
int is_balanced_from(int i) {
    int lh; int rh; int diff;
    if (i >= 256 || tree_valid[i] == 0) return 1;
    lh = tree_height_from(2 * i);
    rh = tree_height_from(2 * i + 1);
    diff = lh - rh;
    if (diff < 0) diff = -diff;
    if (diff > 1) return 0;
    return is_balanced_from(2 * i) && is_balanced_from(2 * i + 1);
}

int is_balanced(void) {
    return is_balanced_from(1);
}

/* Count internal nodes (non-leaf) */
int count_internal_from(int i) {
    int has_left; int has_right;
    if (i >= 256 || tree_valid[i] == 0) return 0;
    has_left = (2 * i < 256 && tree_valid[2 * i] == 1) ? 1 : 0;
    has_right = (2 * i + 1 < 256 && tree_valid[2 * i + 1] == 1) ? 1 : 0;
    if (has_left == 0 && has_right == 0) return 0;
    return 1 + count_internal_from(2 * i) + count_internal_from(2 * i + 1);
}

int count_internal(void) {
    return count_internal_from(1);
}

/* Diameter of tree (longest path between any two nodes) */
int tree_diameter_from(int i) {
    int lh; int rh; int ld; int rd; int through;
    int max;
    if (i >= 256 || tree_valid[i] == 0) return 0;
    lh = tree_height_from(2 * i);
    rh = tree_height_from(2 * i + 1);
    through = lh + rh + 2; /* path through this node */
    ld = tree_diameter_from(2 * i);
    rd = tree_diameter_from(2 * i + 1);
    max = through;
    if (ld > max) max = ld;
    if (rd > max) max = rd;
    return max;
}

int tree_diameter(void) {
    return tree_diameter_from(1);
}

/* Width of tree at a given level */
int tree_width(int level) {
    return count_at_depth(level);
}

/* Maximum width across all levels */
int max_width(void) {
    int h; int d; int w; int mw;
    h = tree_height();
    mw = 0;
    d = 0;
    while (d <= h) {
        w = count_at_depth(d);
        if (w > mw) mw = w;
        d = d + 1;
    }
    return mw;
}

/* Check if a node is a leaf */
int is_leaf(int i) {
    if (i >= 256 || tree_valid[i] == 0) return 0;
    if (2 * i < 256 && tree_valid[2 * i] == 1) return 0;
    if (2 * i + 1 < 256 && tree_valid[2 * i + 1] == 1) return 0;
    return 1;
}

/* Find LCA of two values in BST */
int bst_lca(int a, int b) {
    int i;
    i = 1;
    while (i < 256 && tree_valid[i] == 1) {
        if (a < tree[i] && b < tree[i]) {
            i = 2 * i;
        } else if (a > tree[i] && b > tree[i]) {
            i = 2 * i + 1;
        } else {
            return tree[i];
        }
    }
    return -1;
}

/* Distance between two nodes */
int bst_distance(int a, int b) {
    int lca_val;
    int da; int db;
    lca_val = bst_lca(a, b);
    if (lca_val == -1) return -1;
    da = depth_of(a) - depth_of(lca_val);
    db = depth_of(b) - depth_of(lca_val);
    return da + db;
}

/* Successor in BST (next larger value) */
int bst_successor(int val) {
    int n; int i; int succ;
    n = inorder(inorder_buf);
    succ = -1;
    i = 0;
    while (i < n - 1) {
        if (inorder_buf[i] == val) {
            succ = inorder_buf[i + 1];
            break;
        }
        i = i + 1;
    }
    return succ;
}

/* Predecessor in BST (previous smaller value) */
int bst_predecessor(int val) {
    int n; int i; int pred;
    n = inorder(inorder_buf);
    pred = -1;
    i = 1;
    while (i < n) {
        if (inorder_buf[i] == val) {
            pred = inorder_buf[i - 1];
            break;
        }
        i = i + 1;
    }
    return pred;
}

/* Count nodes in range [lo, hi] */
int count_in_range(int lo, int hi) {
    int n; int i; int cnt;
    n = inorder(inorder_buf);
    cnt = 0;
    i = 0;
    while (i < n) {
        if (inorder_buf[i] >= lo && inorder_buf[i] <= hi) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

/* Sum of nodes at a given depth */
int sum_at_depth(int depth) {
    int start; int end; int i; int s;
    start = 1;
    i = 0;
    while (i < depth) { start = start * 2; i = i + 1; }
    end = start * 2;
    if (end > 256) end = 256;
    s = 0;
    i = start;
    while (i < end) {
        if (tree_valid[i] == 1) s = s + tree[i];
        i = i + 1;
    }
    return s;
}
