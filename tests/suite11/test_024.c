int printf(const char *fmt, ...);

// Segment tree for range sum and range min queries

struct SegTree {
    int sum_tree[256];
    int min_tree[256];
    int n;
};

void seg_build_sum(struct SegTree *st, int *arr, int node, int start, int end) {
    int mid;
    if (start == end) {
        st->sum_tree[node] = arr[start];
        return;
    }
    mid = (start + end) / 2;
    seg_build_sum(st, arr, 2 * node, start, mid);
    seg_build_sum(st, arr, 2 * node + 1, mid + 1, end);
    st->sum_tree[node] = st->sum_tree[2 * node] + st->sum_tree[2 * node + 1];
}

void seg_build_min(struct SegTree *st, int *arr, int node, int start, int end) {
    int mid;
    int lv;
    int rv;
    if (start == end) {
        st->min_tree[node] = arr[start];
        return;
    }
    mid = (start + end) / 2;
    seg_build_min(st, arr, 2 * node, start, mid);
    seg_build_min(st, arr, 2 * node + 1, mid + 1, end);
    lv = st->min_tree[2 * node];
    rv = st->min_tree[2 * node + 1];
    st->min_tree[node] = (lv < rv) ? lv : rv;
}

void seg_init(struct SegTree *st, int *arr, int n) {
    int i;
    st->n = n;
    for (i = 0; i < 256; i++) {
        st->sum_tree[i] = 0;
        st->min_tree[i] = 999999;
    }
    seg_build_sum(st, arr, 1, 0, n - 1);
    seg_build_min(st, arr, 1, 0, n - 1);
}

int seg_query_sum(struct SegTree *st, int node, int start, int end, int l, int r) {
    int mid;
    int left_sum;
    int right_sum;
    if (r < start || end < l) return 0;
    if (l <= start && end <= r) return st->sum_tree[node];
    mid = (start + end) / 2;
    left_sum = seg_query_sum(st, 2 * node, start, mid, l, r);
    right_sum = seg_query_sum(st, 2 * node + 1, mid + 1, end, l, r);
    return left_sum + right_sum;
}

int seg_query_min(struct SegTree *st, int node, int start, int end, int l, int r) {
    int mid;
    int left_min;
    int right_min;
    if (r < start || end < l) return 999999;
    if (l <= start && end <= r) return st->min_tree[node];
    mid = (start + end) / 2;
    left_min = seg_query_min(st, 2 * node, start, mid, l, r);
    right_min = seg_query_min(st, 2 * node + 1, mid + 1, end, l, r);
    return (left_min < right_min) ? left_min : right_min;
}

void seg_update(struct SegTree *st, int node, int start, int end, int idx, int val) {
    int mid;
    int lv;
    int rv;
    if (start == end) {
        st->sum_tree[node] = val;
        st->min_tree[node] = val;
        return;
    }
    mid = (start + end) / 2;
    if (idx <= mid) {
        seg_update(st, 2 * node, start, mid, idx, val);
    } else {
        seg_update(st, 2 * node + 1, mid + 1, end, idx, val);
    }
    st->sum_tree[node] = st->sum_tree[2 * node] + st->sum_tree[2 * node + 1];
    lv = st->min_tree[2 * node];
    rv = st->min_tree[2 * node + 1];
    st->min_tree[node] = (lv < rv) ? lv : rv;
}

int main() {
    struct SegTree seg;
    int arr[8];
    int s;
    int m;

    arr[0] = 1; arr[1] = 3; arr[2] = 5; arr[3] = 7;
    arr[4] = 9; arr[5] = 11; arr[6] = 13; arr[7] = 15;

    seg_init(&seg, arr, 8);

    s = seg_query_sum(&seg, 1, 0, 7, 0, 7);
    printf("sum [0,7]: %d\n", s);          // EXPECT: sum [0,7]: 64

    s = seg_query_sum(&seg, 1, 0, 7, 1, 4);
    printf("sum [1,4]: %d\n", s);          // EXPECT: sum [1,4]: 24

    s = seg_query_sum(&seg, 1, 0, 7, 3, 3);
    printf("sum [3,3]: %d\n", s);          // EXPECT: sum [3,3]: 7

    m = seg_query_min(&seg, 1, 0, 7, 0, 7);
    printf("min [0,7]: %d\n", m);          // EXPECT: min [0,7]: 1

    m = seg_query_min(&seg, 1, 0, 7, 2, 5);
    printf("min [2,5]: %d\n", m);          // EXPECT: min [2,5]: 5

    m = seg_query_min(&seg, 1, 0, 7, 6, 7);
    printf("min [6,7]: %d\n", m);          // EXPECT: min [6,7]: 13

    seg_update(&seg, 1, 0, 7, 0, 100);
    s = seg_query_sum(&seg, 1, 0, 7, 0, 7);
    printf("after update sum [0,7]: %d\n", s);   // EXPECT: after update sum [0,7]: 163

    m = seg_query_min(&seg, 1, 0, 7, 0, 7);
    printf("after update min [0,7]: %d\n", m);   // EXPECT: after update min [0,7]: 3

    seg_update(&seg, 1, 0, 7, 4, -5);
    s = seg_query_sum(&seg, 1, 0, 7, 3, 5);
    printf("after neg update sum [3,5]: %d\n", s);  // EXPECT: after neg update sum [3,5]: 13

    m = seg_query_min(&seg, 1, 0, 7, 0, 7);
    printf("after neg update min [0,7]: %d\n", m);  // EXPECT: after neg update min [0,7]: -5

    seg_update(&seg, 1, 0, 7, 7, 2);
    s = seg_query_sum(&seg, 1, 0, 7, 5, 7);
    printf("sum [5,7]: %d\n", s);          // EXPECT: sum [5,7]: 26

    return 0;
}
