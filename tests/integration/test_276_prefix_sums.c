int printf(const char *fmt, ...);
// EXPECT: array: 1 2 3 4 5 6 7 8\nprefix: 1 3 6 10 15 21 28 36\nsum[0,7] = 36\nsum[0,3] = 10\nsum[2,5] = 18\nsum[4,7] = 26\nsum[3,3] = 4\nclosest to 15: sum[0,4] = 15\nreconstructed: 1 2 3 4 5 6 7 8\nrunning avg: 1 1 2 2 3 3 4 4 
// Test: prefix sums (cumulative sums and range queries)

void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

void build_prefix(int *arr, int *prefix, int n) {
    prefix[0] = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        prefix[i] = prefix[i - 1] + arr[i];
    }
}

// Range sum query [l, r] inclusive using prefix sums
int range_sum(int *prefix, int l, int r) {
    if (l == 0) return prefix[r];
    return prefix[r] - prefix[l - 1];
}

int main(void) {
    int arr[8];
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4;
    arr[4]=5; arr[5]=6; arr[6]=7; arr[7]=8;

    printf("array: ");
    print_arr(arr, 8);

    int prefix[8];
    build_prefix(arr, prefix, 8);
    printf("prefix: ");
    print_arr(prefix, 8);

    // Range sum queries
    printf("sum[0,7] = %d\n", range_sum(prefix, 0, 7));
    printf("sum[0,3] = %d\n", range_sum(prefix, 0, 3));
    printf("sum[2,5] = %d\n", range_sum(prefix, 2, 5));
    printf("sum[4,7] = %d\n", range_sum(prefix, 4, 7));
    printf("sum[3,3] = %d\n", range_sum(prefix, 3, 3));

    // Find subarray with sum closest to target
    int target = 15;
    int best_diff = 1000;
    int best_l = 0;
    int best_r = 0;
    int i; int j;
    for (i = 0; i < 8; i++) {
        for (j = i; j < 8; j++) {
            int s = range_sum(prefix, i, j);
            int diff = s - target;
            if (diff < 0) diff = -diff;
            if (diff < best_diff) {
                best_diff = diff;
                best_l = i;
                best_r = j;
            }
        }
    }
    printf("closest to %d: sum[%d,%d] = %d\n", target, best_l, best_r,
           range_sum(prefix, best_l, best_r));

    // Difference array (inverse of prefix)
    int diff_arr[8];
    diff_arr[0] = prefix[0];
    for (i = 1; i < 8; i++) {
        diff_arr[i] = prefix[i] - prefix[i - 1];
    }
    printf("reconstructed: ");
    print_arr(diff_arr, 8);

    // Running average (integer)
    printf("running avg: ");
    for (i = 0; i < 8; i++) {
        printf("%d ", prefix[i] / (i + 1));
    }
    printf("\n");

    return 0;
}
