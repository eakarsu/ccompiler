int printf(const char *fmt, ...);
// EXPECT: Sum [0,7]: 42\nSum [0,3]: 16\nSum [4,7]: 26\nSum [2,5]: 32\nSum [3,3]: 7\nMin [0,7]: 1\nMin [0,3]: 1\nMin [4,7]: 2\nMin [2,5]: 5\nAfter update [2]=0:\nSum [0,7]: 37\nMin [0,3]: 0\nMin [0,7]: 0\nDone

// Segment tree for range sum and range min queries

struct SegTree {
    int sum_tree[64];
    int min_tree[64];
    int n;
};

void st_build(struct SegTree *st, int *arr, int node, int start, int end) {
    if (start == end) {
        st->sum_tree[node] = arr[start];
        st->min_tree[node] = arr[start];
    } else {
        int mid = (start + end) / 2;
        st_build(st, arr, 2 * node, start, mid);
        st_build(st, arr, 2 * node + 1, mid + 1, end);
        st->sum_tree[node] = st->sum_tree[2 * node] + st->sum_tree[2 * node + 1];
        int lm = st->min_tree[2 * node];
        int rm = st->min_tree[2 * node + 1];
        st->min_tree[node] = lm < rm ? lm : rm;
    }
}

void st_init(struct SegTree *st, int *arr, int n) {
    st->n = n;
    int i = 0;
    while (i < 64) {
        st->sum_tree[i] = 0;
        st->min_tree[i] = 999999;
        i = i + 1;
    }
    st_build(st, arr, 1, 0, n - 1);
}

int st_query_sum(struct SegTree *st, int node, int start, int end, int l, int r) {
    if (r < start || end < l) {
        return 0;
    }
    if (l <= start && end <= r) {
        return st->sum_tree[node];
    }
    int mid = (start + end) / 2;
    int left_sum = st_query_sum(st, 2 * node, start, mid, l, r);
    int right_sum = st_query_sum(st, 2 * node + 1, mid + 1, end, l, r);
    return left_sum + right_sum;
}

int st_query_min(struct SegTree *st, int node, int start, int end, int l, int r) {
    if (r < start || end < l) {
        return 999999;
    }
    if (l <= start && end <= r) {
        return st->min_tree[node];
    }
    int mid = (start + end) / 2;
    int lm = st_query_min(st, 2 * node, start, mid, l, r);
    int rm = st_query_min(st, 2 * node + 1, mid + 1, end, l, r);
    return lm < rm ? lm : rm;
}

void st_update(struct SegTree *st, int node, int start, int end, int idx, int val) {
    if (start == end) {
        st->sum_tree[node] = val;
        st->min_tree[node] = val;
    } else {
        int mid = (start + end) / 2;
        if (idx <= mid) {
            st_update(st, 2 * node, start, mid, idx, val);
        } else {
            st_update(st, 2 * node + 1, mid + 1, end, idx, val);
        }
        st->sum_tree[node] = st->sum_tree[2 * node] + st->sum_tree[2 * node + 1];
        int lm = st->min_tree[2 * node];
        int rm = st->min_tree[2 * node + 1];
        st->min_tree[node] = lm < rm ? lm : rm;
    }
}

int main(void) {
    int arr[8];
    arr[0] = 1; arr[1] = 3; arr[2] = 5; arr[3] = 7;
    arr[4] = 9; arr[5] = 11; arr[6] = 2; arr[7] = 4;

    struct SegTree st;
    st_init(&st, arr, 8);

    printf("Sum [0,7]: %d\n", st_query_sum(&st, 1, 0, 7, 0, 7));
    printf("Sum [0,3]: %d\n", st_query_sum(&st, 1, 0, 7, 0, 3));
    printf("Sum [4,7]: %d\n", st_query_sum(&st, 1, 0, 7, 4, 7));
    printf("Sum [2,5]: %d\n", st_query_sum(&st, 1, 0, 7, 2, 5));
    printf("Sum [3,3]: %d\n", st_query_sum(&st, 1, 0, 7, 3, 3));

    printf("Min [0,7]: %d\n", st_query_min(&st, 1, 0, 7, 0, 7));
    printf("Min [0,3]: %d\n", st_query_min(&st, 1, 0, 7, 0, 3));
    printf("Min [4,7]: %d\n", st_query_min(&st, 1, 0, 7, 4, 7));
    printf("Min [2,5]: %d\n", st_query_min(&st, 1, 0, 7, 2, 5));

    // Update index 2 from 5 to 0
    st_update(&st, 1, 0, 7, 2, 0);
    printf("After update [2]=0:\n");
    printf("Sum [0,7]: %d\n", st_query_sum(&st, 1, 0, 7, 0, 7));
    printf("Min [0,3]: %d\n", st_query_min(&st, 1, 0, 7, 0, 3));
    printf("Min [0,7]: %d\n", st_query_min(&st, 1, 0, 7, 0, 7));

    printf("Done\n");
    return 0;
}
