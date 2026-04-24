int printf(const char *fmt, ...);
// EXPECT: === Linear Search Tests ===
// EXPECT: --- Basic linear search ---
// EXPECT: array: 4 2 7 1 9 3 8 5 6 0
// EXPECT: search 7: index=2
// EXPECT: search 0: index=9
// EXPECT: search 4: index=0
// EXPECT: search 10: index=-1
// EXPECT: --- Search returning all occurrences ---
// EXPECT: array: 3 1 4 1 5 9 2 6 5 3
// EXPECT: occurrences of 1: 1 3
// EXPECT: occurrences of 5: 4 8
// EXPECT: occurrences of 3: 0 9
// EXPECT: occurrences of 7: (none)
// EXPECT: --- Count occurrences ---
// EXPECT: count of 1: 2
// EXPECT: count of 5: 2
// EXPECT: count of 9: 1
// EXPECT: count of 7: 0
// EXPECT: --- Search in 2D array ---
// EXPECT: matrix:
// EXPECT: 1 2 3 4
// EXPECT: 5 6 7 8
// EXPECT: 9 10 11 12
// EXPECT: find 7: row=1 col=2
// EXPECT: find 12: row=2 col=3
// EXPECT: find 1: row=0 col=0
// EXPECT: find 13: not found
// EXPECT: --- Sentinel search ---
// EXPECT: array: 8 3 5 2 9 1 7
// EXPECT: sentinel search 5: index=2
// EXPECT: sentinel search 1: index=5
// EXPECT: sentinel search 4: index=-1
// EXPECT: --- Find min and max ---
// EXPECT: array: 15 3 9 27 1 8 42 6
// EXPECT: min=1 at index=4
// EXPECT: max=42 at index=6
// EXPECT: --- Find second largest ---
// EXPECT: array: 15 3 9 27 1 8 42 6
// EXPECT: second_largest: 27
// EXPECT: --- Search with predicate (find first even) ---
// EXPECT: array: 7 3 9 4 1 8 5
// EXPECT: first even: index=3 value=4
// EXPECT: --- Search with predicate (find first > 10) ---
// EXPECT: array: 3 7 2 15 9 12 1
// EXPECT: first > 10: index=3 value=15
// EXPECT: --- Reverse linear search ---
// EXPECT: array: 4 2 7 1 9 3 7 5
// EXPECT: last 7: index=6
// EXPECT: last 4: index=0
// EXPECT: last 5: index=7
// EXPECT: last 8: index=-1
// EXPECT: --- Search in sorted+rotated array ---
// EXPECT: array: 5 6 7 8 1 2 3 4
// EXPECT: search 7: index=2
// EXPECT: search 1: index=4
// EXPECT: search 4: index=7
// EXPECT: search 9: index=-1
// EXPECT: === Linear Search Done ===
// --- Basic linear search ---
// array: 4 2 7 1 9 3 8 5 6 0
// search 7: index=2
// search 0: index=9
// search 4: index=0
// search 10: index=-1
// --- Search returning all occurrences ---
// array: 3 1 4 1 5 9 2 6 5 3
// occurrences of 1: 1 3
// occurrences of 5: 4 8
// occurrences of 3: 0 9
// occurrences of 7: (none)
// --- Count occurrences ---
// count of 1: 2
// count of 5: 2
// count of 9: 1
// count of 7: 0
// --- Search in 2D array ---
// matrix:
// 1 2 3 4
// 5 6 7 8
// 9 10 11 12
// find 7: row=1 col=2
// find 12: row=2 col=3
// find 1: row=0 col=0
// find 13: not found
// --- Sentinel search ---
// array: 8 3 5 2 9 1 7
// sentinel search 5: index=2
// sentinel search 1: index=5
// sentinel search 4: index=-1
// --- Find min and max ---
// array: 15 3 9 27 1 8 42 6
// min=1 at index=4
// max=42 at index=6
// --- Find second largest ---
// array: 15 3 9 27 1 8 42 6
// second_largest: 27
// --- Search with predicate (find first even) ---
// array: 7 3 9 4 1 8 5
// first even: index=3 value=4
// --- Search with predicate (find first > 10) ---
// array: 3 7 2 15 9 12 1
// first > 10: index=3 value=15
// --- Reverse linear search ---
// array: 4 2 7 1 9 3 7 5
// last 7: index=6
// last 4: index=0
// last 5: index=7
// last 8: index=-1
// --- Search in sorted+rotated array ---
// array: 5 6 7 8 1 2 3 4
// search 7: index=2
// search 1: index=4
// search 4: index=7
// search 9: index=-1
// === Linear Search Done ===

void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

int linear_search(int *arr, int n, int target) {
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}

// Find all occurrences and store in result array, return count
int find_all(int *arr, int n, int target, int *result) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == target) {
            result[count] = i;
            count = count + 1;
        }
    }
    return count;
}

int count_value(int *arr, int n, int target) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == target) count = count + 1;
    }
    return count;
}

// Search in 2D array (flattened), returns row and col via pointers
int search_2d(int *mat, int rows, int cols, int target, int *out_row, int *out_col) {
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (mat[i * cols + j] == target) {
                *out_row = i;
                *out_col = j;
                return 1;
            }
        }
    }
    return 0;
}

// Sentinel search: place target at end to avoid bounds check in loop
int sentinel_search(int *arr, int n, int target) {
    int last = arr[n - 1];
    arr[n - 1] = target;
    int i = 0;
    while (arr[i] != target) {
        i = i + 1;
    }
    arr[n - 1] = last;
    if (i < n - 1) return i;
    if (last == target) return n - 1;
    return -1;
}

// Find min value and its index
void find_min(int *arr, int n, int *out_val, int *out_idx) {
    int i;
    *out_val = arr[0];
    *out_idx = 0;
    for (i = 1; i < n; i++) {
        if (arr[i] < *out_val) {
            *out_val = arr[i];
            *out_idx = i;
        }
    }
}

// Find max value and its index
void find_max(int *arr, int n, int *out_val, int *out_idx) {
    int i;
    *out_val = arr[0];
    *out_idx = 0;
    for (i = 1; i < n; i++) {
        if (arr[i] > *out_val) {
            *out_val = arr[i];
            *out_idx = i;
        }
    }
}

// Find second largest
int find_second_largest(int *arr, int n) {
    int first = -2147483647;
    int second = -2147483647;
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] > first) {
            second = first;
            first = arr[i];
        } else if (arr[i] > second && arr[i] != first) {
            second = arr[i];
        }
    }
    return second;
}

// Find first even number
int find_first_even(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] % 2 == 0) return i;
    }
    return -1;
}

// Find first element > threshold
int find_first_gt(int *arr, int n, int threshold) {
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] > threshold) return i;
    }
    return -1;
}

// Reverse linear search (find last occurrence)
int linear_search_last(int *arr, int n, int target) {
    int i;
    for (i = n - 1; i >= 0; i--) {
        if (arr[i] == target) return i;
    }
    return -1;
}

// Search in a sorted+rotated array (linear for simplicity)
int search_rotated(int *arr, int n, int target) {
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}

int main() {
    printf("=== Linear Search Tests ===\n");

    // Basic linear search
    printf("--- Basic linear search ---\n");
    int a1[10] = {4, 2, 7, 1, 9, 3, 8, 5, 6, 0};
    printf("array: ");
    print_array(a1, 10);
    printf("search 7: index=%d\n", linear_search(a1, 10, 7));
    printf("search 0: index=%d\n", linear_search(a1, 10, 0));
    printf("search 4: index=%d\n", linear_search(a1, 10, 4));
    printf("search 10: index=%d\n", linear_search(a1, 10, 10));

    // Find all occurrences
    printf("--- Search returning all occurrences ---\n");
    int a2[10] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    printf("array: ");
    print_array(a2, 10);

    int results[10];
    int cnt;
    int i;

    cnt = find_all(a2, 10, 1, results);
    printf("occurrences of 1:");
    for (i = 0; i < cnt; i++) printf(" %d", results[i]);
    printf("\n");

    cnt = find_all(a2, 10, 5, results);
    printf("occurrences of 5:");
    for (i = 0; i < cnt; i++) printf(" %d", results[i]);
    printf("\n");

    cnt = find_all(a2, 10, 3, results);
    printf("occurrences of 3:");
    for (i = 0; i < cnt; i++) printf(" %d", results[i]);
    printf("\n");

    cnt = find_all(a2, 10, 7, results);
    printf("occurrences of 7:");
    if (cnt == 0) printf(" (none)");
    for (i = 0; i < cnt; i++) printf(" %d", results[i]);
    printf("\n");

    // Count occurrences
    printf("--- Count occurrences ---\n");
    printf("count of 1: %d\n", count_value(a2, 10, 1));
    printf("count of 5: %d\n", count_value(a2, 10, 5));
    printf("count of 9: %d\n", count_value(a2, 10, 9));
    printf("count of 7: %d\n", count_value(a2, 10, 7));

    // Search in 2D array
    printf("--- Search in 2D array ---\n");
    int mat[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    printf("matrix:\n");
    int r;
    int c;
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 4; c++) {
            if (c > 0) printf(" ");
            printf("%d", mat[r * 4 + c]);
        }
        printf("\n");
    }
    int row;
    int col;
    if (search_2d(mat, 3, 4, 7, &row, &col))
        printf("find 7: row=%d col=%d\n", row, col);
    if (search_2d(mat, 3, 4, 12, &row, &col))
        printf("find 12: row=%d col=%d\n", row, col);
    if (search_2d(mat, 3, 4, 1, &row, &col))
        printf("find 1: row=%d col=%d\n", row, col);
    if (!search_2d(mat, 3, 4, 13, &row, &col))
        printf("find 13: not found\n");

    // Sentinel search
    printf("--- Sentinel search ---\n");
    int a3[7] = {8, 3, 5, 2, 9, 1, 7};
    printf("array: ");
    print_array(a3, 7);
    printf("sentinel search 5: index=%d\n", sentinel_search(a3, 7, 5));
    printf("sentinel search 1: index=%d\n", sentinel_search(a3, 7, 1));
    printf("sentinel search 4: index=%d\n", sentinel_search(a3, 7, 4));

    // Find min and max
    printf("--- Find min and max ---\n");
    int a4[8] = {15, 3, 9, 27, 1, 8, 42, 6};
    printf("array: ");
    print_array(a4, 8);
    int minv;
    int mini;
    int maxv;
    int maxi;
    find_min(a4, 8, &minv, &mini);
    find_max(a4, 8, &maxv, &maxi);
    printf("min=%d at index=%d\n", minv, mini);
    printf("max=%d at index=%d\n", maxv, maxi);

    // Find second largest
    printf("--- Find second largest ---\n");
    printf("array: ");
    print_array(a4, 8);
    printf("second_largest: %d\n", find_second_largest(a4, 8));

    // Search with predicate (first even)
    printf("--- Search with predicate (find first even) ---\n");
    int a5[7] = {7, 3, 9, 4, 1, 8, 5};
    printf("array: ");
    print_array(a5, 7);
    int idx = find_first_even(a5, 7);
    printf("first even: index=%d value=%d\n", idx, a5[idx]);

    // Search with predicate (first > 10)
    printf("--- Search with predicate (find first > 10) ---\n");
    int a6[7] = {3, 7, 2, 15, 9, 12, 1};
    printf("array: ");
    print_array(a6, 7);
    idx = find_first_gt(a6, 7, 10);
    printf("first > 10: index=%d value=%d\n", idx, a6[idx]);

    // Reverse linear search
    printf("--- Reverse linear search ---\n");
    int a7[8] = {4, 2, 7, 1, 9, 3, 7, 5};
    printf("array: ");
    print_array(a7, 8);
    printf("last 7: index=%d\n", linear_search_last(a7, 8, 7));
    printf("last 4: index=%d\n", linear_search_last(a7, 8, 4));
    printf("last 5: index=%d\n", linear_search_last(a7, 8, 5));
    printf("last 8: index=%d\n", linear_search_last(a7, 8, 8));

    // Search in sorted+rotated array
    printf("--- Search in sorted+rotated array ---\n");
    int a8[8] = {5, 6, 7, 8, 1, 2, 3, 4};
    printf("array: ");
    print_array(a8, 8);
    printf("search 7: index=%d\n", search_rotated(a8, 8, 7));
    printf("search 1: index=%d\n", search_rotated(a8, 8, 1));
    printf("search 4: index=%d\n", search_rotated(a8, 8, 4));
    printf("search 9: index=%d\n", search_rotated(a8, 8, 9));

    printf("=== Linear Search Done ===\n");
    return 0;
}
