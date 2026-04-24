int printf(const char *fmt, ...);

// Fenwick tree (Binary Indexed Tree) for prefix sums and range queries

struct Fenwick {
    int tree[33];
    int n;
};

void fen_init(struct Fenwick *f, int n) {
    int i;
    f->n = n;
    for (i = 0; i <= n; i++) {
        f->tree[i] = 0;
    }
}

void fen_update(struct Fenwick *f, int i, int delta) {
    while (i <= f->n) {
        f->tree[i] = f->tree[i] + delta;
        i = i + (i & (-i));
    }
}

int fen_query(struct Fenwick *f, int i) {
    int sum = 0;
    while (i > 0) {
        sum = sum + f->tree[i];
        i = i - (i & (-i));
    }
    return sum;
}

int fen_range_query(struct Fenwick *f, int l, int r) {
    if (l <= 1) return fen_query(f, r);
    return fen_query(f, r) - fen_query(f, l - 1);
}

void fen_build(struct Fenwick *f, int *arr, int n) {
    int i;
    fen_init(f, n);
    for (i = 1; i <= n; i++) {
        fen_update(f, i, arr[i]);
    }
}

int fen_find_kth(struct Fenwick *f, int k) {
    int pos = 0;
    int bitMask;
    int t;
    bitMask = 1;
    while (bitMask <= f->n) {
        bitMask = bitMask * 2;
    }
    bitMask = bitMask / 2;
    while (bitMask > 0) {
        t = pos + bitMask;
        if (t <= f->n && f->tree[t] < k) {
            k = k - f->tree[t];
            pos = t;
        }
        bitMask = bitMask / 2;
    }
    return pos + 1;
}

int count_inversions(int *arr, int n) {
    struct Fenwick fen;
    int inversions = 0;
    int i;
    int val;
    fen_init(&fen, 20);
    for (i = 0; i < n; i++) {
        val = arr[i];
        inversions = inversions + (i - fen_query(&fen, val));
        fen_update(&fen, val, 1);
    }
    return inversions;
}

int main() {
    struct Fenwick fen;
    int arr[17];
    int i;
    int inv_arr[6];

    arr[0] = 0;
    arr[1] = 3; arr[2] = 2; arr[3] = 7; arr[4] = 1;
    arr[5] = 5; arr[6] = 4; arr[7] = 8; arr[8] = 6;
    arr[9] = 10; arr[10] = 9; arr[11] = 12; arr[12] = 11;
    arr[13] = 14; arr[14] = 13; arr[15] = 16; arr[16] = 15;

    fen_build(&fen, arr, 16);

    printf("prefix[4]: %d\n", fen_query(&fen, 4));    // EXPECT: prefix[4]: 13
    printf("prefix[8]: %d\n", fen_query(&fen, 8));    // EXPECT: prefix[8]: 36
    printf("prefix[16]: %d\n", fen_query(&fen, 16));  // EXPECT: prefix[16]: 136

    printf("range[3,6]: %d\n", fen_range_query(&fen, 3, 6));  // EXPECT: range[3,6]: 17
    printf("range[1,1]: %d\n", fen_range_query(&fen, 1, 1));  // EXPECT: range[1,1]: 3
    printf("range[5,8]: %d\n", fen_range_query(&fen, 5, 8));  // EXPECT: range[5,8]: 23

    fen_update(&fen, 3, 3);
    printf("after +3 at 3, prefix[4]: %d\n", fen_query(&fen, 4));  // EXPECT: after +3 at 3, prefix[4]: 16
    printf("after +3 at 3, range[2,5]: %d\n", fen_range_query(&fen, 2, 5));
    // EXPECT: after +3 at 3, range[2,5]: 18

    fen_update(&fen, 1, -1);
    printf("after -1 at 1, prefix[4]: %d\n", fen_query(&fen, 4));  // EXPECT: after -1 at 1, prefix[4]: 15

    inv_arr[0] = 5; inv_arr[1] = 3; inv_arr[2] = 4;
    inv_arr[3] = 1; inv_arr[4] = 2; inv_arr[5] = 6;
    printf("inversions: %d\n", count_inversions(inv_arr, 6));  // EXPECT: inversions: 8

    inv_arr[0] = 1; inv_arr[1] = 2; inv_arr[2] = 3;
    printf("sorted inversions: %d\n", count_inversions(inv_arr, 3));  // EXPECT: sorted inversions: 0

    inv_arr[0] = 3; inv_arr[1] = 2; inv_arr[2] = 1;
    printf("reversed inversions: %d\n", count_inversions(inv_arr, 3));  // EXPECT: reversed inversions: 3

    return 0;
}
