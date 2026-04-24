int printf(const char *fmt, ...);
// EXPECT: Prefix sum [1..1]: 3\nPrefix sum [1..3]: 4\nPrefix sum [1..5]: 15\nPrefix sum [1..10]: 28\nRange [2..5]: 12\nRange [3..7]: 11\nRange [1..10]: 28\nRange [4..4]: 6\nAfter update [3]+=5:\nPrefix sum [1..3]: 9\nRange [2..5]: 17\nAfter update [7]+=10:\nPrefix sum [1..7]: 31\nRange [6..8]: 14\nDone

// Binary Indexed Tree (Fenwick Tree) for prefix sums

struct FenwickTree {
    int tree[17];
    int n;
};

void ft_init(struct FenwickTree *ft, int n) {
    ft->n = n;
    int i = 0;
    while (i <= 16) {
        ft->tree[i] = 0;
        i = i + 1;
    }
}

void ft_update(struct FenwickTree *ft, int idx, int delta) {
    while (idx <= ft->n) {
        ft->tree[idx] = ft->tree[idx] + delta;
        idx = idx + (idx & (-idx));
    }
}

int ft_query(struct FenwickTree *ft, int idx) {
    int sum = 0;
    while (idx > 0) {
        sum = sum + ft->tree[idx];
        idx = idx - (idx & (-idx));
    }
    return sum;
}

int ft_range_query(struct FenwickTree *ft, int l, int r) {
    if (l <= 1) return ft_query(ft, r);
    return ft_query(ft, r) - ft_query(ft, l - 1);
}

void ft_build(struct FenwickTree *ft, int *arr, int n) {
    ft_init(ft, n);
    int i = 0;
    while (i < n) {
        ft_update(ft, i + 1, arr[i]);
        i = i + 1;
    }
}

// Find position of k-th element (1-indexed)
int ft_find_kth(struct FenwickTree *ft, int k) {
    int pos = 0;
    int bitMask = 8; // largest power of 2 <= n
    while (bitMask > 0) {
        int next = pos + bitMask;
        if (next <= ft->n && ft->tree[next] < k) {
            k = k - ft->tree[next];
            pos = next;
        }
        bitMask = bitMask / 2;
    }
    return pos + 1;
}

int main(void) {
    int arr[10];
    arr[0] = 3; arr[1] = 2; arr[2] = -1; arr[3] = 6;
    arr[4] = 5; arr[5] = 4; arr[6] = -3; arr[7] = 3;
    arr[8] = 7; arr[9] = 2;

    struct FenwickTree ft;
    ft_build(&ft, arr, 10);

    // Prefix sums
    printf("Prefix sum [1..1]: %d\n", ft_query(&ft, 1));
    printf("Prefix sum [1..3]: %d\n", ft_query(&ft, 3));
    printf("Prefix sum [1..5]: %d\n", ft_query(&ft, 5));
    printf("Prefix sum [1..10]: %d\n", ft_query(&ft, 10));

    // Range queries
    printf("Range [2..5]: %d\n", ft_range_query(&ft, 2, 5));
    printf("Range [3..7]: %d\n", ft_range_query(&ft, 3, 7));
    printf("Range [1..10]: %d\n", ft_range_query(&ft, 1, 10));
    printf("Range [4..4]: %d\n", ft_range_query(&ft, 4, 4));

    // Point update
    ft_update(&ft, 3, 5); // arr[2] becomes -1+5=4
    printf("After update [3]+=5:\n");
    printf("Prefix sum [1..3]: %d\n", ft_query(&ft, 3));
    printf("Range [2..5]: %d\n", ft_range_query(&ft, 2, 5));

    // Another update
    ft_update(&ft, 7, 10); // arr[6] becomes -3+10=7
    printf("After update [7]+=10:\n");
    printf("Prefix sum [1..7]: %d\n", ft_query(&ft, 7));
    printf("Range [6..8]: %d\n", ft_range_query(&ft, 6, 8));

    printf("Done\n");
    return 0;
}
