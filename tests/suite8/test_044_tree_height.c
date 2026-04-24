int printf(const char *fmt, ...);

/* Various tree shapes and height/depth/balance computations */

struct TNode {
    int val;
    int left;
    int right;
};

struct TNode nodes[64];
int ncnt;

int mk(int val) {
    int idx;
    idx = ncnt;
    nodes[idx].val = val;
    nodes[idx].left = -1;
    nodes[idx].right = -1;
    ncnt = ncnt + 1;
    return idx;
}

void link_left(int p, int c) {
    nodes[p].left = c;
}

void link_right(int p, int c) {
    nodes[p].right = c;
}

int height(int idx) {
    int lh; int rh;
    if (idx == -1) return 0;
    lh = height(nodes[idx].left);
    rh = height(nodes[idx].right);
    if (lh > rh) return lh + 1;
    return rh + 1;
}

int depth(int root, int target, int d) {
    if (root == -1) return -1;
    if (nodes[root].val == target) return d;
    int ld;
    ld = depth(nodes[root].left, target, d + 1);
    if (ld != -1) return ld;
    return depth(nodes[root].right, target, d + 1);
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int is_balanced(int idx) {
    int lh; int rh;
    if (idx == -1) return 1;
    lh = height(nodes[idx].left);
    rh = height(nodes[idx].right);
    if (abs_val(lh - rh) > 1) return 0;
    if (is_balanced(nodes[idx].left) == 0) return 0;
    if (is_balanced(nodes[idx].right) == 0) return 0;
    return 1;
}

int count_nodes(int idx) {
    if (idx == -1) return 0;
    return 1 + count_nodes(nodes[idx].left) + count_nodes(nodes[idx].right);
}

int sum_nodes(int idx) {
    if (idx == -1) return 0;
    return nodes[idx].val + sum_nodes(nodes[idx].left) + sum_nodes(nodes[idx].right);
}

int max_val(int idx) {
    int lmax; int rmax; int best;
    if (idx == -1) return -999;
    best = nodes[idx].val;
    lmax = max_val(nodes[idx].left);
    rmax = max_val(nodes[idx].right);
    if (lmax > best) best = lmax;
    if (rmax > best) best = rmax;
    return best;
}

int min_val(int idx) {
    int lmin; int rmin; int best;
    if (idx == -1) return 999;
    best = nodes[idx].val;
    lmin = min_val(nodes[idx].left);
    rmin = min_val(nodes[idx].right);
    if (lmin < best) best = lmin;
    if (rmin < best) best = rmin;
    return best;
}

int main(void) {
    int root1; int root2; int root3;
    int a; int b; int c; int d; int e; int f; int g;

    ncnt = 0;

    /* Balanced tree:      10
                          /  \
                         5    15
                        / \   / \
                       3   7 12  20
    */
    root1 = mk(10);
    a = mk(5); b = mk(15);
    c = mk(3); d = mk(7); e = mk(12); f = mk(20);
    link_left(root1, a); link_right(root1, b);
    link_left(a, c); link_right(a, d);
    link_left(b, e); link_right(b, f);

    // EXPECT: tree1 height=3
    printf("tree1 height=%d\n", height(root1));
    // EXPECT: tree1 balanced=1
    printf("tree1 balanced=%d\n", is_balanced(root1));
    // EXPECT: tree1 count=7
    printf("tree1 count=%d\n", count_nodes(root1));
    // EXPECT: tree1 sum=72
    printf("tree1 sum=%d\n", sum_nodes(root1));
    // EXPECT: tree1 max=20
    printf("tree1 max=%d\n", max_val(root1));
    // EXPECT: tree1 min=3
    printf("tree1 min=%d\n", min_val(root1));

    /* Skewed tree: 1 -> 2 -> 3 -> 4 -> 5 (all right) */
    root2 = mk(1);
    a = mk(2); b = mk(3); c = mk(4); d = mk(5);
    link_right(root2, a);
    link_right(a, b);
    link_right(b, c);
    link_right(c, d);

    // EXPECT: tree2 height=5
    printf("tree2 height=%d\n", height(root2));
    // EXPECT: tree2 balanced=0
    printf("tree2 balanced=%d\n", is_balanced(root2));

    /* Depth queries on tree1 */
    // EXPECT: depth of 10=0
    printf("depth of 10=%d\n", depth(root1, 10, 0));
    // EXPECT: depth of 5=1
    printf("depth of 5=%d\n", depth(root1, 5, 0));
    // EXPECT: depth of 7=2
    printf("depth of 7=%d\n", depth(root1, 7, 0));
    // EXPECT: depth of 20=2
    printf("depth of 20=%d\n", depth(root1, 20, 0));
    // EXPECT: depth of 99=-1
    printf("depth of 99=%d\n", depth(root1, 99, 0));

    /* Single node tree */
    root3 = mk(42);
    // EXPECT: single height=1
    printf("single height=%d\n", height(root3));
    // EXPECT: single balanced=1
    printf("single balanced=%d\n", is_balanced(root3));

    return 0;
}
