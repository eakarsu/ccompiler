int printf(const char *fmt, ...);
// EXPECT: === Selection Sort Tests ===
// EXPECT: --- Basic sort ---
// EXPECT: input: 64 25 12 22 11
// EXPECT: sorted: 11 12 22 25 64
// EXPECT: --- Already sorted ---
// EXPECT: input: 1 2 3 4 5
// EXPECT: sorted: 1 2 3 4 5
// EXPECT: --- Reverse sorted ---
// EXPECT: input: 10 9 8 7 6 5 4 3 2 1
// EXPECT: sorted: 1 2 3 4 5 6 7 8 9 10
// EXPECT: --- Single element ---
// EXPECT: sorted: 77
// EXPECT: --- All duplicates ---
// EXPECT: sorted: 4 4 4 4
// EXPECT: --- Mixed duplicates ---
// EXPECT: input: 3 1 4 1 5 9 2 6
// EXPECT: sorted: 1 1 2 3 4 5 6 9
// EXPECT: --- Negative values ---
// EXPECT: input: -10 5 -3 0 8 -1
// EXPECT: sorted: -10 -3 -1 0 5 8
// EXPECT: --- Count comparisons (n=5) ---
// EXPECT: comparisons: 10
// EXPECT: --- Count swaps (sorted input) ---
// EXPECT: swaps_sorted: 0
// EXPECT: --- Count swaps (reverse input) ---
// EXPECT: swaps_reverse: 2
// EXPECT: --- Find minimum index ---
// EXPECT: min_index of {7,2,9,1,5}: 3
// EXPECT: --- Double selection sort (min and max) ---
// EXPECT: input: 8 3 6 1 9 2 7 4 5 0
// EXPECT: double_sorted: 0 1 2 3 4 5 6 7 8 9
// EXPECT: --- Large array ---
// EXPECT: first: 0 last: 29
// EXPECT: === Selection Sort Done ===
// --- Basic sort ---
// input: 64 25 12 22 11
// sorted: 11 12 22 25 64
// --- Already sorted ---
// input: 1 2 3 4 5
// sorted: 1 2 3 4 5
// --- Reverse sorted ---
// input: 10 9 8 7 6 5 4 3 2 1
// sorted: 1 2 3 4 5 6 7 8 9 10
// --- Single element ---
// sorted: 77
// --- All duplicates ---
// sorted: 4 4 4 4
// --- Mixed duplicates ---
// input: 3 1 4 1 5 9 2 6
// sorted: 1 1 2 3 4 5 6 9
// --- Negative values ---
// input: -10 5 -3 0 8 -1
// sorted: -10 -3 -1 0 5 8
// --- Count comparisons (n=5) ---
// comparisons: 10
// --- Count swaps (sorted input) ---
// swaps_sorted: 0
// --- Count swaps (reverse input) ---
// swaps_reverse: 2
// --- Find minimum index ---
// min_index of {7,2,9,1,5}: 3
// --- Double selection sort (min and max) ---
// input: 8 3 6 1 9 2 7 4 5 0
// double_sorted: 0 1 2 3 4 5 6 7 8 9
// --- Large array ---
// first: 0 last: 29
// === Selection Sort Done ===

void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

void selection_sort(int *arr, int n) {
    int i;
    int j;
    int min_idx;
    int tmp;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
        }
    }
}

int selection_sort_count_cmp(int *arr, int n) {
    int i;
    int j;
    int min_idx;
    int tmp;
    int cmp = 0;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            cmp = cmp + 1;
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
        }
    }
    return cmp;
}

int selection_sort_count_swaps(int *arr, int n) {
    int i;
    int j;
    int min_idx;
    int tmp;
    int swaps = 0;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
            swaps = swaps + 1;
        }
    }
    return swaps;
}

int find_min_index(int *arr, int n) {
    int i;
    int min_idx = 0;
    for (i = 1; i < n; i++) {
        if (arr[i] < arr[min_idx]) {
            min_idx = i;
        }
    }
    return min_idx;
}

// Double selection: find both min and max each pass
void double_selection_sort(int *arr, int n) {
    int lo = 0;
    int hi = n - 1;
    int i;
    int min_idx;
    int max_idx;
    int tmp;
    while (lo < hi) {
        min_idx = lo;
        max_idx = lo;
        for (i = lo; i <= hi; i++) {
            if (arr[i] < arr[min_idx]) min_idx = i;
            if (arr[i] > arr[max_idx]) max_idx = i;
        }
        // Place minimum at lo
        tmp = arr[lo];
        arr[lo] = arr[min_idx];
        arr[min_idx] = tmp;
        // If max was at lo, it got swapped to min_idx
        if (max_idx == lo) max_idx = min_idx;
        // Place maximum at hi
        tmp = arr[hi];
        arr[hi] = arr[max_idx];
        arr[max_idx] = tmp;
        lo = lo + 1;
        hi = hi - 1;
    }
}

int main() {
    printf("=== Selection Sort Tests ===\n");

    // Basic sort
    printf("--- Basic sort ---\n");
    int a1[5] = {64, 25, 12, 22, 11};
    printf("input: ");
    print_array(a1, 5);
    selection_sort(a1, 5);
    printf("sorted: ");
    print_array(a1, 5);

    // Already sorted
    printf("--- Already sorted ---\n");
    int a2[5] = {1, 2, 3, 4, 5};
    printf("input: ");
    print_array(a2, 5);
    selection_sort(a2, 5);
    printf("sorted: ");
    print_array(a2, 5);

    // Reverse sorted
    printf("--- Reverse sorted ---\n");
    int a3[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    printf("input: ");
    print_array(a3, 10);
    selection_sort(a3, 10);
    printf("sorted: ");
    print_array(a3, 10);

    // Single element
    printf("--- Single element ---\n");
    int a4[1] = {77};
    selection_sort(a4, 1);
    printf("sorted: ");
    print_array(a4, 1);

    // All duplicates
    printf("--- All duplicates ---\n");
    int a5[4] = {4, 4, 4, 4};
    selection_sort(a5, 4);
    printf("sorted: ");
    print_array(a5, 4);

    // Mixed duplicates
    printf("--- Mixed duplicates ---\n");
    int a6[8] = {3, 1, 4, 1, 5, 9, 2, 6};
    printf("input: ");
    print_array(a6, 8);
    selection_sort(a6, 8);
    printf("sorted: ");
    print_array(a6, 8);

    // Negative values
    printf("--- Negative values ---\n");
    int a7[6] = {-10, 5, -3, 0, 8, -1};
    printf("input: ");
    print_array(a7, 6);
    selection_sort(a7, 6);
    printf("sorted: ");
    print_array(a7, 6);

    // Count comparisons
    printf("--- Count comparisons (n=5) ---\n");
    int a8[5] = {5, 3, 1, 4, 2};
    int cmp = selection_sort_count_cmp(a8, 5);
    printf("comparisons: %d\n", cmp);

    // Count swaps on sorted input
    printf("--- Count swaps (sorted input) ---\n");
    int a9[5] = {1, 2, 3, 4, 5};
    int sw1 = selection_sort_count_swaps(a9, 5);
    printf("swaps_sorted: %d\n", sw1);

    // Count swaps on reverse input
    printf("--- Count swaps (reverse input) ---\n");
    int a10[5] = {5, 4, 3, 2, 1};
    int sw2 = selection_sort_count_swaps(a10, 5);
    printf("swaps_reverse: %d\n", sw2);

    // Find minimum index
    printf("--- Find minimum index ---\n");
    int a11[5] = {7, 2, 9, 1, 5};
    int mi = find_min_index(a11, 5);
    printf("min_index of {7,2,9,1,5}: %d\n", mi);

    // Double selection sort
    printf("--- Double selection sort (min and max) ---\n");
    int a12[10] = {8, 3, 6, 1, 9, 2, 7, 4, 5, 0};
    printf("input: ");
    print_array(a12, 10);
    double_selection_sort(a12, 10);
    printf("double_sorted: ");
    print_array(a12, 10);

    // Large array
    printf("--- Large array ---\n");
    int a13[30];
    int i;
    for (i = 0; i < 30; i++) {
        a13[i] = 29 - i;
    }
    selection_sort(a13, 30);
    printf("first: %d last: %d\n", a13[0], a13[29]);

    printf("=== Selection Sort Done ===\n");
    return 0;
}
