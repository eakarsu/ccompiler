int printf(const char *fmt, ...);

// Segment Tree for range sum queries and point updates
// Array-based: for array of size n, segment tree needs 4*n nodes
// Root at index 1. Left child = 2*i, Right child = 2*i+1

struct SegTree {
    int tree[256];  // segment tree array (4 * max_n)
    int n;          // number of elements
};

void seg_build(struct SegTree *st, int *arr, int node, int start, int end) {
    int mid;
    if (start == end) {
        st->tree[node] = arr[start];
        return;
    }
    mid = (start + end) / 2;
    seg_build(st, arr, 2 * node, start, mid);
    seg_build(st, arr, 2 * node + 1, mid + 1, end);
    st->tree[node] = st->tree[2 * node] + st->tree[2 * node + 1];
}

void seg_init(struct SegTree *st, int *arr, int n) {
    int i;
    for (i = 0; i < 256; i++) {
        st->tree[i] = 0;
    }
    st->n = n;
    seg_build(st, arr, 1, 0, n - 1);
}

// Point update: set arr[idx] += diff
void seg_update(struct SegTree *st, int node, int start, int end, int idx, int diff) {
    int mid;
    if (start == end) {
        st->tree[node] = st->tree[node] + diff;
        return;
    }
    mid = (start + end) / 2;
    if (idx <= mid) {
        seg_update(st, 2 * node, start, mid, idx, diff);
    } else {
        seg_update(st, 2 * node + 1, mid + 1, end, idx, diff);
    }
    st->tree[node] = st->tree[2 * node] + st->tree[2 * node + 1];
}

// Range sum query [l, r]
int seg_query(struct SegTree *st, int node, int start, int end, int l, int r) {
    int mid;
    int left_sum;
    int right_sum;
    if (r < start || end < l) {
        return 0; // out of range
    }
    if (l <= start && end <= r) {
        return st->tree[node]; // completely within range
    }
    mid = (start + end) / 2;
    left_sum = seg_query(st, 2 * node, start, mid, l, r);
    right_sum = seg_query(st, 2 * node + 1, mid + 1, end, l, r);
    return left_sum + right_sum;
}

// Convenience wrappers
int query(struct SegTree *st, int l, int r) {
    return seg_query(st, 1, 0, st->n - 1, l, r);
}

void update(struct SegTree *st, int idx, int diff) {
    seg_update(st, 1, 0, st->n - 1, idx, diff);
}

// Range minimum query segment tree
struct SegTreeMin {
    int tree[256];
    int n;
};

void seg_min_build(struct SegTreeMin *st, int *arr, int node, int start, int end) {
    int mid;
    int lv;
    int rv;
    if (start == end) {
        st->tree[node] = arr[start];
        return;
    }
    mid = (start + end) / 2;
    seg_min_build(st, arr, 2 * node, start, mid);
    seg_min_build(st, arr, 2 * node + 1, mid + 1, end);
    lv = st->tree[2 * node];
    rv = st->tree[2 * node + 1];
    if (lv < rv) {
        st->tree[node] = lv;
    } else {
        st->tree[node] = rv;
    }
}

void seg_min_init(struct SegTreeMin *st, int *arr, int n) {
    int i;
    for (i = 0; i < 256; i++) {
        st->tree[i] = 999999;
    }
    st->n = n;
    seg_min_build(st, arr, 1, 0, n - 1);
}

int seg_min_query(struct SegTreeMin *st, int node, int start, int end, int l, int r) {
    int mid;
    int lv;
    int rv;
    if (r < start || end < l) {
        return 999999;
    }
    if (l <= start && end <= r) {
        return st->tree[node];
    }
    mid = (start + end) / 2;
    lv = seg_min_query(st, 2 * node, start, mid, l, r);
    rv = seg_min_query(st, 2 * node + 1, mid + 1, end, l, r);
    if (lv < rv) return lv;
    return rv;
}

int query_min(struct SegTreeMin *st, int l, int r) {
    return seg_min_query(st, 1, 0, st->n - 1, l, r);
}

int main() {
    int arr[8];
    struct SegTree st;

    // Initialize array: [1, 3, 5, 7, 9, 11, 13, 15]
    arr[0] = 1;
    arr[1] = 3;
    arr[2] = 5;
    arr[3] = 7;
    arr[4] = 9;
    arr[5] = 11;
    arr[6] = 13;
    arr[7] = 15;

    seg_init(&st, arr, 8);

    // Total sum
    printf("Sum [0,7]: %d\n", query(&st, 0, 7));
    // EXPECT: Sum [0,7]: 64

    // Partial sums
    printf("Sum [0,3]: %d\n", query(&st, 0, 3));
    // EXPECT: Sum [0,3]: 16
    printf("Sum [4,7]: %d\n", query(&st, 4, 7));
    // EXPECT: Sum [4,7]: 48
    printf("Sum [2,5]: %d\n", query(&st, 2, 5));
    // EXPECT: Sum [2,5]: 32
    printf("Sum [3,3]: %d\n", query(&st, 3, 3));
    // EXPECT: Sum [3,3]: 7
    printf("Sum [0,0]: %d\n", query(&st, 0, 0));
    // EXPECT: Sum [0,0]: 1
    printf("Sum [1,6]: %d\n", query(&st, 1, 6));
    // EXPECT: Sum [1,6]: 48

    // Point update: add 5 to index 3 (7 -> 12)
    update(&st, 3, 5);
    printf("After +5 at [3], Sum [0,7]: %d\n", query(&st, 0, 7));
    // EXPECT: After +5 at [3], Sum [0,7]: 69
    printf("After +5 at [3], Sum [3,3]: %d\n", query(&st, 3, 3));
    // EXPECT: After +5 at [3], Sum [3,3]: 12
    printf("After +5 at [3], Sum [0,3]: %d\n", query(&st, 0, 3));
    // EXPECT: After +5 at [3], Sum [0,3]: 21

    // Another update: subtract 3 from index 0 (1 -> -2)
    update(&st, 0, -3);
    printf("After -3 at [0], Sum [0,7]: %d\n", query(&st, 0, 7));
    // EXPECT: After -3 at [0], Sum [0,7]: 66
    printf("After -3 at [0], Sum [0,0]: %d\n", query(&st, 0, 0));
    // EXPECT: After -3 at [0], Sum [0,0]: -2

    // Multiple updates
    update(&st, 7, -10); // 15 -> 5
    update(&st, 4, 1);   // 9 -> 10
    printf("After more updates, Sum [0,7]: %d\n", query(&st, 0, 7));
    // EXPECT: After more updates, Sum [0,7]: 57
    printf("Sum [4,7]: %d\n", query(&st, 4, 7));
    // EXPECT: Sum [4,7]: 39

    // Range minimum query tree
    struct SegTreeMin stmin;
    int arr2[8];
    arr2[0] = 5;
    arr2[1] = 2;
    arr2[2] = 8;
    arr2[3] = 1;
    arr2[4] = 9;
    arr2[5] = 3;
    arr2[6] = 7;
    arr2[7] = 4;

    seg_min_init(&stmin, arr2, 8);

    printf("Min [0,7]: %d\n", query_min(&stmin, 0, 7));
    // EXPECT: Min [0,7]: 1
    printf("Min [0,3]: %d\n", query_min(&stmin, 0, 3));
    // EXPECT: Min [0,3]: 1
    printf("Min [4,7]: %d\n", query_min(&stmin, 4, 7));
    // EXPECT: Min [4,7]: 3
    printf("Min [1,2]: %d\n", query_min(&stmin, 1, 2));
    // EXPECT: Min [1,2]: 2
    printf("Min [5,6]: %d\n", query_min(&stmin, 5, 6));
    // EXPECT: Min [5,6]: 3
    printf("Min [3,5]: %d\n", query_min(&stmin, 3, 5));
    // EXPECT: Min [3,5]: 1
    printf("Min [6,7]: %d\n", query_min(&stmin, 6, 7));
    // EXPECT: Min [6,7]: 4

    return 0;
}
