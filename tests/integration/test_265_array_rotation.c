int printf(const char *fmt, ...);
// EXPECT: left 1: 2 3 4 5 6 7 1\nleft 3: 4 5 6 7 1 2 3\nleft 7: 1 2 3 4 5 6 7\nright 1: 7 1 2 3 4 5 6\nright 3: 5 6 7 1 2 3 4\nrestore: 1 2 3 4 5 6 7\nleft 10: 4 5 6 7 1 2 3
// Test: array rotation (left and right)

void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

void reverse(int *arr, int lo, int hi) {
    while (lo < hi) {
        int tmp = arr[lo];
        arr[lo] = arr[hi];
        arr[hi] = tmp;
        lo = lo + 1;
        hi = hi - 1;
    }
}

void rotate_left(int *arr, int n, int k) {
    k = k % n;
    if (k == 0) return;
    reverse(arr, 0, k - 1);
    reverse(arr, k, n - 1);
    reverse(arr, 0, n - 1);
}

void rotate_right(int *arr, int n, int k) {
    k = k % n;
    if (k == 0) return;
    rotate_left(arr, n, n - k);
}

void copy_arr(int *dst, int *src, int n) {
    int i;
    for (i = 0; i < n; i++) dst[i] = src[i];
}

int main(void) {
    int orig[7];
    orig[0]=1; orig[1]=2; orig[2]=3; orig[3]=4;
    orig[4]=5; orig[5]=6; orig[6]=7;

    int arr[7];

    // Left rotate by 1
    copy_arr(arr, orig, 7);
    rotate_left(arr, 7, 1);
    printf("left 1: ");
    print_arr(arr, 7);

    // Left rotate by 3
    copy_arr(arr, orig, 7);
    rotate_left(arr, 7, 3);
    printf("left 3: ");
    print_arr(arr, 7);

    // Left rotate by 7 (full cycle)
    copy_arr(arr, orig, 7);
    rotate_left(arr, 7, 7);
    printf("left 7: ");
    print_arr(arr, 7);

    // Right rotate by 1
    copy_arr(arr, orig, 7);
    rotate_right(arr, 7, 1);
    printf("right 1: ");
    print_arr(arr, 7);

    // Right rotate by 3
    copy_arr(arr, orig, 7);
    rotate_right(arr, 7, 3);
    printf("right 3: ");
    print_arr(arr, 7);

    // Left then right should restore
    copy_arr(arr, orig, 7);
    rotate_left(arr, 7, 2);
    rotate_right(arr, 7, 2);
    printf("restore: ");
    print_arr(arr, 7);

    // Rotate by more than length
    copy_arr(arr, orig, 7);
    rotate_left(arr, 7, 10);
    printf("left 10: ");
    print_arr(arr, 7);

    return 0;
}
