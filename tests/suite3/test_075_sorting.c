int printf(const char *fmt, ...);

/* Bubble sort, selection sort, insertion sort */

/* Check if array is sorted ascending */
int is_sorted(int *arr, int n) {
    int i;
    for (i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
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

/* Copy array */
void copy_arr(int *dst, int *src, int n) {
    int i;
    for (i = 0; i < n; i++) dst[i] = src[i];
}

/* Bubble sort (ascending); returns number of swaps */
int bubble_sort(int *arr, int n) {
    int swaps = 0;
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j]     = arr[j + 1];
                arr[j + 1] = tmp;
                swaps++;
            }
        }
    }
    return swaps;
}

/* Selection sort; returns number of swaps */
int selection_sort(int *arr, int n) {
    int swaps = 0;
    int i, j;
    for (i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) min_idx = j;
        }
        if (min_idx != i) {
            int tmp = arr[i];
            arr[i]       = arr[min_idx];
            arr[min_idx] = tmp;
            swaps++;
        }
    }
    return swaps;
}

/* Insertion sort; returns number of shifts */
int insertion_sort(int *arr, int n) {
    int shifts = 0;
    int i, j;
    for (i = 1; i < n; i++) {
        int key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
            shifts++;
        }
        arr[j + 1] = key;
    }
    return shifts;
}

int main(void) {
    int orig[8] = {5, 2, 8, 1, 9, 3, 7, 4};
    int arr[8];

    /* Bubble sort */
    copy_arr(arr, orig, 8);
    int bswaps = bubble_sort(arr, 8);
    print_arr(arr, 8);
    // EXPECT: 1 2 3 4 5 7 8 9
    printf("%d\n", is_sorted(arr, 8));  /* 1 */
    // EXPECT: 1
    printf("%d\n", bswaps);             /* 13 */
    // EXPECT: 13

    /* Selection sort */
    copy_arr(arr, orig, 8);
    int sswaps = selection_sort(arr, 8);
    print_arr(arr, 8);
    // EXPECT: 1 2 3 4 5 7 8 9
    printf("%d\n", is_sorted(arr, 8));  /* 1 */
    // EXPECT: 1
    printf("%d\n", sswaps);             /* 5 */
    // EXPECT: 5

    /* Insertion sort */
    copy_arr(arr, orig, 8);
    int ishifts = insertion_sort(arr, 8);
    print_arr(arr, 8);
    // EXPECT: 1 2 3 4 5 7 8 9
    printf("%d\n", is_sorted(arr, 8));  /* 1 */
    // EXPECT: 1
    printf("%d\n", ishifts);            /* 13 */
    // EXPECT: 13

    /* Already sorted: 0 swaps/shifts */
    int sorted[5] = {1, 2, 3, 4, 5};
    int b2 = bubble_sort(sorted, 5);
    printf("%d\n", b2);                 /* 0 */
    // EXPECT: 0

    int sorted2[5] = {1, 2, 3, 4, 5};
    int s2 = selection_sort(sorted2, 5);
    printf("%d\n", s2);                 /* 0 */
    // EXPECT: 0

    int sorted3[5] = {1, 2, 3, 4, 5};
    int i2 = insertion_sort(sorted3, 5);
    printf("%d\n", i2);                 /* 0 */
    // EXPECT: 0

    /* Reverse sorted: worst case for bubble/insertion */
    int rev[5] = {5, 4, 3, 2, 1};
    int brev = bubble_sort(rev, 5);
    print_arr(rev, 5);
    // EXPECT: 1 2 3 4 5
    printf("%d\n", brev);               /* 10 */
    // EXPECT: 10

    int rev2[5] = {5, 4, 3, 2, 1};
    int irev = insertion_sort(rev2, 5);
    printf("%d\n", irev);               /* 10 */
    // EXPECT: 10

    /* Single element */
    int one[1] = {42};
    printf("%d\n", bubble_sort(one, 1));    /* 0 */
    // EXPECT: 0
    printf("%d\n", selection_sort(one, 1)); /* 0 */
    // EXPECT: 0
    printf("%d\n", insertion_sort(one, 1)); /* 0 */
    // EXPECT: 0

    /* Negative numbers */
    int neg[6] = {-3, 7, -1, 0, 5, -9};
    bubble_sort(neg, 6);
    print_arr(neg, 6);
    // EXPECT: -9 -3 -1 0 5 7
    printf("%d\n", is_sorted(neg, 6));  /* 1 */
    // EXPECT: 1

    /* Duplicates: selection sort */
    int dup[7] = {4, 2, 4, 1, 3, 4, 2};
    selection_sort(dup, 7);
    print_arr(dup, 7);
    // EXPECT: 1 2 2 3 4 4 4
    printf("%d\n", is_sorted(dup, 7));  /* 1 */
    // EXPECT: 1

    /* Insertion sort on nearly sorted (one element out of place) */
    int near[6] = {1, 2, 6, 3, 4, 5};
    int nshifts = insertion_sort(near, 6);
    print_arr(near, 6);
    // EXPECT: 1 2 3 4 5 6
    printf("%d\n", nshifts);            /* 3 */
    // EXPECT: 3

    return 0;
}
