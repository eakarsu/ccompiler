int printf(const char *fmt, ...);

// Fenwick Tree (Binary Indexed Tree)
// Supports point update and prefix sum queries in O(log n)

struct FenwickTree {
    int tree[17]; // 1-indexed, size up to 16
    int n;
};

void fenwick_init(struct FenwickTree *ft, int n) {
    int i;
    ft->n = n;
    for (i = 0; i <= n; i++) {
        ft->tree[i] = 0;
    }
}

void fenwick_update(struct FenwickTree *ft, int i, int delta) {
    while (i <= ft->n) {
        ft->tree[i] = ft->tree[i] + delta;
        i = i + (i & (-i));
    }
}

int fenwick_query(struct FenwickTree *ft, int i) {
    int sum;
    sum = 0;
    while (i > 0) {
        sum = sum + ft->tree[i];
        i = i - (i & (-i));
    }
    return sum;
}

int fenwick_range_query(struct FenwickTree *ft, int l, int r) {
    int right_sum;
    int left_sum;
    right_sum = fenwick_query(ft, r);
    left_sum = fenwick_query(ft, l - 1);
    return right_sum - left_sum;
}

// Build from array: arr[0..n-1] maps to tree positions 1..n
void fenwick_build(struct FenwickTree *ft, int *arr, int n) {
    int i;
    fenwick_init(ft, n);
    for (i = 0; i < n; i++) {
        fenwick_update(ft, i + 1, arr[i]);
    }
}

// Find index with given cumulative frequency (1-indexed)
int fenwick_find(struct FenwickTree *ft, int cum) {
    int pos;
    int bitMask;
    int tsum;
    pos = 0;
    bitMask = 8; // largest power of 2 <= 16
    tsum = 0;
    while (bitMask > 0) {
        int next;
        next = pos + bitMask;
        if (next <= ft->n && tsum + ft->tree[next] < cum) {
            pos = next;
            tsum = tsum + ft->tree[next];
        }
        bitMask = bitMask / 2;
    }
    return pos + 1;
}

int main() {
    struct FenwickTree ft;
    int arr[8];
    int i;
    int sum;

    // Test 1: Simple operations
    fenwick_init(&ft, 8);

    fenwick_update(&ft, 1, 3);
    fenwick_update(&ft, 2, 5);
    fenwick_update(&ft, 3, 7);
    fenwick_update(&ft, 4, 2);
    fenwick_update(&ft, 5, 8);
    fenwick_update(&ft, 6, 1);
    fenwick_update(&ft, 7, 4);
    fenwick_update(&ft, 8, 6);

    printf("Prefix sum [1..4]: %d\n", fenwick_query(&ft, 4));
    // EXPECT: Prefix sum [1..4]: 17

    printf("Prefix sum [1..8]: %d\n", fenwick_query(&ft, 8));
    // EXPECT: Prefix sum [1..8]: 36

    printf("Range sum [3..6]: %d\n", fenwick_range_query(&ft, 3, 6));
    // EXPECT: Range sum [3..6]: 18

    printf("Range sum [1..1]: %d\n", fenwick_range_query(&ft, 1, 1));
    // EXPECT: Range sum [1..1]: 3

    printf("Range sum [5..8]: %d\n", fenwick_range_query(&ft, 5, 8));
    // EXPECT: Range sum [5..8]: 19

    // Update: add 10 to position 3
    fenwick_update(&ft, 3, 10);
    printf("After update, prefix [1..4]: %d\n", fenwick_query(&ft, 4));
    // EXPECT: After update, prefix [1..4]: 27

    printf("After update, range [3..3]: %d\n", fenwick_range_query(&ft, 3, 3));
    // EXPECT: After update, range [3..3]: 17

    // Test 2: Build from array
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    arr[3] = 4;
    arr[4] = 5;
    arr[5] = 6;
    arr[6] = 7;
    arr[7] = 8;
    fenwick_build(&ft, arr, 8);

    printf("Built prefix [1..8]: %d\n", fenwick_query(&ft, 8));
    // EXPECT: Built prefix [1..8]: 36

    printf("Built prefix [1..4]: %d\n", fenwick_query(&ft, 4));
    // EXPECT: Built prefix [1..4]: 10

    printf("Built range [2..5]: %d\n", fenwick_range_query(&ft, 2, 5));
    // EXPECT: Built range [2..5]: 14

    printf("Built range [4..7]: %d\n", fenwick_range_query(&ft, 4, 7));
    // EXPECT: Built range [4..7]: 22

    // Test find: find position where cumulative sum reaches target
    printf("Find cum 10: %d\n", fenwick_find(&ft, 10));
    // EXPECT: Find cum 10: 4

    printf("Find cum 15: %d\n", fenwick_find(&ft, 15));
    // EXPECT: Find cum 15: 5

    printf("Find cum 1: %d\n", fenwick_find(&ft, 1));
    // EXPECT: Find cum 1: 1

    // Subtract values to test negative updates
    fenwick_update(&ft, 3, -2);
    printf("After subtract, range [1..3]: %d\n", fenwick_range_query(&ft, 1, 3));
    // EXPECT: After subtract, range [1..3]: 4

    printf("After subtract, prefix [1..8]: %d\n", fenwick_query(&ft, 8));
    // EXPECT: After subtract, prefix [1..8]: 34

    return 0;
}
