int printf(const char *fmt, ...);

/* Binary search variants on a sorted integer array */

/* Standard binary search: returns index of val, or -1 if not found */
int binary_search(int *arr, int n, int val) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == val) return mid;
        if (arr[mid] < val)  lo = mid + 1;
        else                 hi = mid - 1;
    }
    return -1;
}

/* Lower bound: first index where arr[i] >= val (0..n if none) */
int lower_bound(int *arr, int n, int val) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] < val) lo = mid + 1;
        else                hi = mid;
    }
    return lo;
}

/* Upper bound: first index where arr[i] > val (0..n if none) */
int upper_bound(int *arr, int n, int val) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] <= val) lo = mid + 1;
        else                 hi = mid;
    }
    return lo;
}

/* Count occurrences of val in sorted array */
int count_occurrences(int *arr, int n, int val) {
    return upper_bound(arr, n, val) - lower_bound(arr, n, val);
}

/* Search for insertion point of a missing element */
int insertion_point(int *arr, int n, int val) {
    return lower_bound(arr, n, val);
}

/* Print array */
void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

int main(void) {
    /* Sorted array: 1 3 5 7 9 11 13 15 17 19 */
    int a[10] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int n = 10;

    print_arr(a, n);
    // EXPECT: 1 3 5 7 9 11 13 15 17 19

    /* Search for present elements */
    printf("%d\n", binary_search(a, n, 1));   /* 0 */
    // EXPECT: 0
    printf("%d\n", binary_search(a, n, 9));   /* 4 */
    // EXPECT: 4
    printf("%d\n", binary_search(a, n, 19));  /* 9 */
    // EXPECT: 9
    printf("%d\n", binary_search(a, n, 13));  /* 6 */
    // EXPECT: 6

    /* Search for missing elements */
    printf("%d\n", binary_search(a, n, 2));   /* -1 */
    // EXPECT: -1
    printf("%d\n", binary_search(a, n, 0));   /* -1 */
    // EXPECT: -1
    printf("%d\n", binary_search(a, n, 20));  /* -1 */
    // EXPECT: -1

    /* Insertion points for missing elements */
    printf("%d\n", insertion_point(a, n, 0));  /* 0 (before index 0) */
    // EXPECT: 0
    printf("%d\n", insertion_point(a, n, 2));  /* 1 (between 1 and 3) */
    // EXPECT: 1
    printf("%d\n", insertion_point(a, n, 10)); /* 5 (between 9 and 11) */
    // EXPECT: 5
    printf("%d\n", insertion_point(a, n, 20)); /* 10 (after end) */
    // EXPECT: 10

    /* Array with duplicates: 2 2 4 4 4 6 8 8 10 */
    int b[9] = {2, 2, 4, 4, 4, 6, 8, 8, 10};
    int m = 9;

    print_arr(b, m);
    // EXPECT: 2 2 4 4 4 6 8 8 10

    printf("%d\n", count_occurrences(b, m, 2));  /* 2 */
    // EXPECT: 2
    printf("%d\n", count_occurrences(b, m, 4));  /* 3 */
    // EXPECT: 3
    printf("%d\n", count_occurrences(b, m, 6));  /* 1 */
    // EXPECT: 1
    printf("%d\n", count_occurrences(b, m, 8));  /* 2 */
    // EXPECT: 2
    printf("%d\n", count_occurrences(b, m, 10)); /* 1 */
    // EXPECT: 1
    printf("%d\n", count_occurrences(b, m, 5));  /* 0 (missing) */
    // EXPECT: 0

    /* Lower and upper bounds on b */
    printf("%d\n", lower_bound(b, m, 4)); /* 2 */
    // EXPECT: 2
    printf("%d\n", upper_bound(b, m, 4)); /* 5 */
    // EXPECT: 5
    printf("%d\n", lower_bound(b, m, 7)); /* 6 (between 6 and 8, first >=7 is index 6) */
    // EXPECT: 6
    printf("%d\n", upper_bound(b, m, 7)); /* 6 (no element <=7 after index 5) */
    // EXPECT: 6

    /* Single-element array */
    int c[1] = {42};
    printf("%d\n", binary_search(c, 1, 42));  /* 0 */
    // EXPECT: 0
    printf("%d\n", binary_search(c, 1, 41));  /* -1 */
    // EXPECT: -1
    printf("%d\n", lower_bound(c, 1, 42));    /* 0 */
    // EXPECT: 0
    printf("%d\n", upper_bound(c, 1, 42));    /* 1 */
    // EXPECT: 1

    /* All same elements: 5 5 5 5 5 */
    int d[5] = {5, 5, 5, 5, 5};
    printf("%d\n", count_occurrences(d, 5, 5)); /* 5 */
    // EXPECT: 5
    printf("%d\n", lower_bound(d, 5, 5));        /* 0 */
    // EXPECT: 0
    printf("%d\n", upper_bound(d, 5, 5));        /* 5 */
    // EXPECT: 5

    /* Larger array search */
    int e[16];
    int i;
    for (i = 0; i < 16; i++) e[i] = i * 4; /* 0 4 8 ... 60 */
    printf("%d\n", binary_search(e, 16, 32)); /* 8 */
    // EXPECT: 8
    printf("%d\n", binary_search(e, 16, 60)); /* 15 */
    // EXPECT: 15
    printf("%d\n", binary_search(e, 16, 0));  /* 0 */
    // EXPECT: 0
    printf("%d\n", binary_search(e, 16, 6));  /* -1 */
    // EXPECT: -1

    return 0;
}
