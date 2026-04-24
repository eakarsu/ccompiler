int printf(const char *fmt, ...);
// EXPECT: === Bubble Sort Tests ===
// EXPECT: --- Basic ascending sort ---
// EXPECT: input: 5 3 8 1 9 2 7 4 6 0
// EXPECT: sorted: 0 1 2 3 4 5 6 7 8 9
// EXPECT: --- Already sorted input ---
// EXPECT: input: 1 2 3 4 5
// EXPECT: sorted: 1 2 3 4 5
// EXPECT: --- Reverse sorted input ---
// EXPECT: input: 9 8 7 6 5 4 3 2 1 0
// EXPECT: sorted: 0 1 2 3 4 5 6 7 8 9
// EXPECT: --- Single element ---
// EXPECT: input: 42
// EXPECT: sorted: 42
// EXPECT: --- Two elements unsorted ---
// EXPECT: input: 7 3
// EXPECT: sorted: 3 7
// EXPECT: --- Two elements sorted ---
// EXPECT: input: 3 7
// EXPECT: sorted: 3 7
// EXPECT: --- All same values ---
// EXPECT: input: 5 5 5 5 5
// EXPECT: sorted: 5 5 5 5 5
// EXPECT: --- Negative numbers ---
// EXPECT: input: -3 5 -1 8 -7 2
// EXPECT: sorted: -7 -3 -1 2 5 8
// EXPECT: --- Descending bubble sort ---
// EXPECT: input: 5 3 8 1 9 2
// EXPECT: desc_sorted: 9 8 5 3 2 1
// EXPECT: --- Swap count test ---
// EXPECT: array: 4 3 2 1
// EXPECT: swaps: 6
// EXPECT: --- Optimized early exit ---
// EXPECT: sorted_passes: 1
// EXPECT: --- Large array sort ---
// EXPECT: first: 0 last: 19
// EXPECT: === Bubble Sort Done ===
// --- Basic ascending sort ---
// input: 5 3 8 1 9 2 7 4 6 0
// sorted: 0 1 2 3 4 5 6 7 8 9
// --- Already sorted input ---
// input: 1 2 3 4 5
// sorted: 1 2 3 4 5
// --- Reverse sorted input ---
// input: 9 8 7 6 5 4 3 2 1 0
// sorted: 0 1 2 3 4 5 6 7 8 9
// --- Single element ---
// input: 42
// sorted: 42
// --- Two elements unsorted ---
// input: 7 3
// sorted: 3 7
// --- Two elements sorted ---
// input: 3 7
// sorted: 3 7
// --- All same values ---
// input: 5 5 5 5 5
// sorted: 5 5 5 5 5
// --- Negative numbers ---
// input: -3 5 -1 8 -7 2
// sorted: -7 -3 -1 2 5 8
// --- Descending bubble sort ---
// input: 5 3 8 1 9 2
// desc_sorted: 9 8 5 3 2 1
// --- Swap count test ---
// array: 4 3 2 1
// swaps: 6
// --- Optimized early exit ---
// sorted_passes: 1
// --- Large array sort ---
// first: 0 last: 19
// === Bubble Sort Done ===

void bubble_sort_asc(int *arr, int n) {
    int i;
    int j;
    int tmp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void bubble_sort_desc(int *arr, int n) {
    int i;
    int j;
    int tmp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] < arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int bubble_sort_count_swaps(int *arr, int n) {
    int i;
    int j;
    int tmp;
    int swaps = 0;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
                swaps = swaps + 1;
            }
        }
    }
    return swaps;
}

int bubble_sort_optimized(int *arr, int n) {
    int i;
    int j;
    int tmp;
    int swapped;
    int passes = 0;
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        passes = passes + 1;
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
    return passes;
}

void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

int main() {
    printf("=== Bubble Sort Tests ===\n");

    // Basic ascending sort
    printf("--- Basic ascending sort ---\n");
    int a1[10] = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0};
    printf("input: ");
    print_array(a1, 10);
    bubble_sort_asc(a1, 10);
    printf("sorted: ");
    print_array(a1, 10);

    // Already sorted
    printf("--- Already sorted input ---\n");
    int a2[5] = {1, 2, 3, 4, 5};
    printf("input: ");
    print_array(a2, 5);
    bubble_sort_asc(a2, 5);
    printf("sorted: ");
    print_array(a2, 5);

    // Reverse sorted
    printf("--- Reverse sorted input ---\n");
    int a3[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    printf("input: ");
    print_array(a3, 10);
    bubble_sort_asc(a3, 10);
    printf("sorted: ");
    print_array(a3, 10);

    // Single element
    printf("--- Single element ---\n");
    int a4[1] = {42};
    printf("input: ");
    print_array(a4, 1);
    bubble_sort_asc(a4, 1);
    printf("sorted: ");
    print_array(a4, 1);

    // Two elements unsorted
    printf("--- Two elements unsorted ---\n");
    int a5[2] = {7, 3};
    printf("input: ");
    print_array(a5, 2);
    bubble_sort_asc(a5, 2);
    printf("sorted: ");
    print_array(a5, 2);

    // Two elements sorted
    printf("--- Two elements sorted ---\n");
    int a6[2] = {3, 7};
    printf("input: ");
    print_array(a6, 2);
    bubble_sort_asc(a6, 2);
    printf("sorted: ");
    print_array(a6, 2);

    // All same values
    printf("--- All same values ---\n");
    int a7[5] = {5, 5, 5, 5, 5};
    printf("input: ");
    print_array(a7, 5);
    bubble_sort_asc(a7, 5);
    printf("sorted: ");
    print_array(a7, 5);

    // Negative numbers
    printf("--- Negative numbers ---\n");
    int a8[6] = {-3, 5, -1, 8, -7, 2};
    printf("input: ");
    print_array(a8, 6);
    bubble_sort_asc(a8, 6);
    printf("sorted: ");
    print_array(a8, 6);

    // Descending sort
    printf("--- Descending bubble sort ---\n");
    int a9[6] = {5, 3, 8, 1, 9, 2};
    printf("input: ");
    print_array(a9, 6);
    bubble_sort_desc(a9, 6);
    printf("desc_sorted: ");
    print_array(a9, 6);

    // Swap count
    printf("--- Swap count test ---\n");
    int a10[4] = {4, 3, 2, 1};
    printf("array: ");
    print_array(a10, 4);
    int swaps = bubble_sort_count_swaps(a10, 4);
    printf("swaps: %d\n", swaps);

    // Optimized early exit on already sorted
    printf("--- Optimized early exit ---\n");
    int a11[5] = {1, 2, 3, 4, 5};
    int passes = bubble_sort_optimized(a11, 5);
    printf("sorted_passes: %d\n", passes);

    // Large array
    printf("--- Large array sort ---\n");
    int a12[20];
    int i;
    for (i = 0; i < 20; i++) {
        a12[i] = 19 - i;
    }
    bubble_sort_asc(a12, 20);
    printf("first: %d last: %d\n", a12[0], a12[19]);

    printf("=== Bubble Sort Done ===\n");
    return 0;
}
