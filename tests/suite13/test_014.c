int printf(const char *fmt, ...);

// Iterative bottom-up merge sort
// Uses a temporary buffer and merges subarrays of increasing size

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

void merge(int *arr, int *tmp, int left, int mid, int right, int *merge_ops) {
    int i = left;
    int j = mid;
    int k = left;

    while (i < mid && j < right) {
        *merge_ops = *merge_ops + 1;
        if (arr[i] <= arr[j]) {
            tmp[k] = arr[i];
            i = i + 1;
        } else {
            tmp[k] = arr[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i < mid) {
        tmp[k] = arr[i];
        i = i + 1;
        k = k + 1;
    }
    while (j < right) {
        tmp[k] = arr[j];
        j = j + 1;
        k = k + 1;
    }
    // Copy back
    i = left;
    while (i < right) {
        arr[i] = tmp[i];
        i = i + 1;
    }
}

void merge_sort_iterative(int *arr, int n, int *tmp, int *total_merges, int *total_ops) {
    *total_merges = 0;
    *total_ops = 0;
    int width = 1;
    while (width < n) {
        int i = 0;
        while (i < n) {
            int left = i;
            int mid = min(i + width, n);
            int right = min(i + 2 * width, n);
            if (mid < right) {
                merge(arr, tmp, left, mid, right, total_ops);
                *total_merges = *total_merges + 1;
            }
            i = i + 2 * width;
        }
        width = width * 2;
    }
}

int is_sorted(int *arr, int n) {
    int i;
    for (i = 0; i < n - 1; i = i + 1) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

// Count inversions using merge sort approach
int count_inversions_merge(int *arr, int n, int *tmp) {
    int inversions = 0;
    int width = 1;
    while (width < n) {
        int pos = 0;
        while (pos < n) {
            int left = pos;
            int mid = min(pos + width, n);
            int right = min(pos + 2 * width, n);
            if (mid < right) {
                int i = left;
                int j = mid;
                int k = left;
                while (i < mid && j < right) {
                    if (arr[i] <= arr[j]) {
                        tmp[k] = arr[i];
                        i = i + 1;
                    } else {
                        tmp[k] = arr[j];
                        j = j + 1;
                        inversions = inversions + (mid - i);
                    }
                    k = k + 1;
                }
                while (i < mid) {
                    tmp[k] = arr[i]; i = i + 1; k = k + 1;
                }
                while (j < right) {
                    tmp[k] = arr[j]; j = j + 1; k = k + 1;
                }
                i = left;
                while (i < right) {
                    arr[i] = tmp[i]; i = i + 1;
                }
            }
            pos = pos + 2 * width;
        }
        width = width * 2;
    }
    return inversions;
}

int main() {
    int tmp[20];
    int merges;
    int ops;

    // Test 1: Basic sort
    int a1[8];
    a1[0] = 38; a1[1] = 27; a1[2] = 43; a1[3] = 3;
    a1[4] = 9; a1[5] = 82; a1[6] = 10; a1[7] = 76;
    merge_sort_iterative(a1, 8, tmp, &merges, &ops);
    printf("sorted: %d %d %d %d %d %d %d %d\n",
           a1[0], a1[1], a1[2], a1[3], a1[4], a1[5], a1[6], a1[7]);
    // EXPECT: sorted: 3 9 10 27 38 43 76 82
    printf("merges: %d ops: %d\n", merges, ops);
    // EXPECT: merges: 7 ops: 16
    printf("is_sorted: %d\n", is_sorted(a1, 8));
    // EXPECT: is_sorted: 1

    // Test 2: Already sorted
    int a2[4];
    a2[0] = 1; a2[1] = 2; a2[2] = 3; a2[3] = 4;
    merge_sort_iterative(a2, 4, tmp, &merges, &ops);
    printf("presorted: %d %d %d %d merges: %d ops: %d\n",
           a2[0], a2[1], a2[2], a2[3], merges, ops);
    // EXPECT: presorted: 1 2 3 4 merges: 3 ops: 4

    // Test 3: Reverse sorted
    int a3[6];
    a3[0] = 6; a3[1] = 5; a3[2] = 4; a3[3] = 3; a3[4] = 2; a3[5] = 1;
    merge_sort_iterative(a3, 6, tmp, &merges, &ops);
    printf("reverse: %d %d %d %d %d %d\n",
           a3[0], a3[1], a3[2], a3[3], a3[4], a3[5]);
    // EXPECT: reverse: 1 2 3 4 5 6
    printf("rev merges: %d ops: %d\n", merges, ops);
    // EXPECT: rev merges: 5 ops: 7

    // Test 4: Odd length
    int a4[5];
    a4[0] = 5; a4[1] = 3; a4[2] = 1; a4[3] = 4; a4[4] = 2;
    merge_sort_iterative(a4, 5, tmp, &merges, &ops);
    printf("odd: %d %d %d %d %d\n", a4[0], a4[1], a4[2], a4[3], a4[4]);
    // EXPECT: odd: 1 2 3 4 5
    printf("odd merges: %d\n", merges);
    // EXPECT: odd merges: 4

    // Test 5: Count inversions
    int a5[5];
    a5[0] = 5; a5[1] = 4; a5[2] = 3; a5[3] = 2; a5[4] = 1;
    int inv = count_inversions_merge(a5, 5, tmp);
    printf("inversions in reverse: %d\n", inv);
    // EXPECT: inversions in reverse: 10
    printf("after inv count: %d %d %d %d %d\n", a5[0], a5[1], a5[2], a5[3], a5[4]);
    // EXPECT: after inv count: 1 2 3 4 5

    // Test 6: Inversions in partially sorted
    int a6[4];
    a6[0] = 2; a6[1] = 4; a6[2] = 1; a6[3] = 3;
    inv = count_inversions_merge(a6, 4, tmp);
    printf("partial inversions: %d\n", inv);
    // EXPECT: partial inversions: 3

    // Test 7: Single and two element
    int a7[1];
    a7[0] = 42;
    merge_sort_iterative(a7, 1, tmp, &merges, &ops);
    printf("single: %d merges: %d\n", a7[0], merges);
    // EXPECT: single: 42 merges: 0

    int a8[2];
    a8[0] = 20; a8[1] = 10;
    merge_sort_iterative(a8, 2, tmp, &merges, &ops);
    printf("two: %d %d merges: %d\n", a8[0], a8[1], merges);
    // EXPECT: two: 10 20 merges: 1

    printf("done\n");
    // EXPECT: done

    return 0;
}
