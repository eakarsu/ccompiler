int printf(const char *fmt, ...);
// EXPECT: Test 1: bubble_sorted=1\nTest 1: select_sorted=1\nTest 1: insert_sorted=1\nTest 1: shell_sorted=1\nTest 1: merge_sorted=1\nTest 1: all_equal=1\nTest 1: first=6 last=94\nTest 2: bubble_sorted=1\nTest 2: select_sorted=1\nTest 2: insert_sorted=1\nTest 2: shell_sorted=1\nTest 2: merge_sorted=1\nTest 2: all_equal=1\nTest 2: first=-93 last=77\nTest 3: bubble_sorted=1\nTest 3: select_sorted=1\nTest 3: insert_sorted=1\nTest 3: shell_sorted=1\nTest 3: merge_sorted=1\nTest 3: all_equal=1\nTest 3: first=-83 last=92\nTest 4: nearly_sorted_first=1 last=34\nTest 4: insertion_shifts=8\nTest 5: reverse_bubble_swaps=45\nTest 5: reverse_inversions=45\nTest 5: reverse_sorted_first=10 last=100\nTest 6: dup_original: 0 2 4 1 3 0 2 4 1 3 0 2 4 1 3\nTest 6: dup_sorted: 0 0 0 1 1 1 2 2 2 3 3 3 4 4 4\nTest 6: dup_first=0 dup_last=4\nTest 7: single=42\nTest 7: two_sorted=5 10\nTest 7: two_rev=5 10\nTest 8: all_same_sorted=1 first=7 last=7\nTest 9: select_comparisons=45\nTest 9: shell_passes=3\nTest 9: merge_ops=40\nStats 10: sum=-140 min=-97 max=95 inversions=330\nTest 10: large_first=-97 large_last=95\nTest 10: large_mid=-1\nTest 10: sum_preserved=1\nTest 11: neg_first=-15 neg_last=-1\nTest 11: neg_sorted=1\nTest 12: mixed_first=-12 mixed_last=15\nTest 12: mixed_sorted=1\nTest 12: mixed_sum=10
// ============================================================
// test_081_sorting_bench.c
// Sort the same array with 5 different algorithms:
//   bubble sort, selection sort, insertion sort, shell sort, merge sort
// Verify all produce the same sorted result.
// ============================================================

// ---- Utility: copy array ----
void copy_array(int *src, int *dst, int n) {
    int i;
    for (i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

// ---- Utility: print array ----
void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

// ---- Utility: check if sorted ascending ----
int is_sorted(int *arr, int n) {
    int i;
    for (i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

// ---- Utility: arrays equal ----
int arrays_equal(int *a, int *b, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

// ---- Utility: swap two ints ----
void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// ---- Utility: sum of array ----
int array_sum(int *arr, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) {
        s = s + arr[i];
    }
    return s;
}

// ---- Utility: min of array ----
int array_min(int *arr, int n) {
    int m = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] < m) {
            m = arr[i];
        }
    }
    return m;
}

// ---- Utility: max of array ----
int array_max(int *arr, int n) {
    int m = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] > m) {
            m = arr[i];
        }
    }
    return m;
}

// ============================================================
// Bubble Sort
// ============================================================
void bubble_sort(int *arr, int n) {
    int i;
    int j;
    // Repeatedly swap adjacent elements if out of order
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

// Count the number of swaps bubble sort would make
int bubble_sort_count_swaps(int *arr, int n) {
    int count = 0;
    int i;
    int j;
    int tmp[50];
    copy_array(arr, tmp, n);
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (tmp[j] > tmp[j + 1]) {
                swap(&tmp[j], &tmp[j + 1]);
                count++;
            }
        }
    }
    return count;
}

// ============================================================
// Selection Sort
// ============================================================
void selection_sort(int *arr, int n) {
    int i;
    int j;
    int min_idx;
    // Find the minimum element in unsorted part, place at beginning
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(&arr[i], &arr[min_idx]);
        }
    }
}

// Count the number of comparisons selection sort would make
int selection_sort_count_comparisons(int *arr, int n) {
    // Selection sort always does n*(n-1)/2 comparisons
    return n * (n - 1) / 2;
}

// ============================================================
// Insertion Sort
// ============================================================
void insertion_sort(int *arr, int n) {
    int i;
    int j;
    int key;
    // Insert each element into its correct position
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Count shifts in insertion sort
int insertion_sort_count_shifts(int *arr, int n) {
    int count = 0;
    int i;
    int j;
    int key;
    int tmp[50];
    copy_array(arr, tmp, n);
    for (i = 1; i < n; i++) {
        key = tmp[i];
        j = i - 1;
        while (j >= 0 && tmp[j] > key) {
            tmp[j + 1] = tmp[j];
            j--;
            count++;
        }
        tmp[j + 1] = key;
    }
    return count;
}

// ============================================================
// Shell Sort
// ============================================================
void shell_sort(int *arr, int n) {
    int gap;
    int i;
    int j;
    int temp;
    // Start with a large gap, reduce by half each iteration
    for (gap = n / 2; gap > 0; gap = gap / 2) {
        // Perform gapped insertion sort
        for (i = gap; i < n; i++) {
            temp = arr[i];
            j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j = j - gap;
            }
            arr[j] = temp;
        }
    }
}

// Count total gap passes in shell sort
int shell_sort_count_passes(int n) {
    int count = 0;
    int gap;
    for (gap = n / 2; gap > 0; gap = gap / 2) {
        count++;
    }
    return count;
}

// ============================================================
// Merge Sort
// ============================================================

void merge(int *arr, int left, int mid, int right) {
    int i;
    int j;
    int k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Copy data to temp arrays
    int left_arr[30];
    int right_arr[30];
    for (i = 0; i < n1; i++) {
        left_arr[i] = arr[left + i];
    }
    for (j = 0; j < n2; j++) {
        right_arr[j] = arr[mid + 1 + j];
    }

    // Merge the temp arrays back into arr
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (left_arr[i] <= right_arr[j]) {
            arr[k] = left_arr[i];
            i++;
        } else {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of left_arr
    while (i < n1) {
        arr[k] = left_arr[i];
        i++;
        k++;
    }

    // Copy remaining elements of right_arr
    while (j < n2) {
        arr[k] = right_arr[j];
        j++;
        k++;
    }
}

void merge_sort_helper(int *arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_helper(arr, left, mid);
        merge_sort_helper(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void merge_sort(int *arr, int n) {
    if (n > 1) {
        merge_sort_helper(arr, 0, n - 1);
    }
}

// Count merge operations
int count_merge_ops(int n) {
    // Approximate: n * log2(n), compute iteratively
    if (n <= 1) return 0;
    int ops = 0;
    int level_size = 1;
    while (level_size < n) {
        ops = ops + n;
        level_size = level_size * 2;
    }
    return ops;
}

// ============================================================
// Additional utility: reverse array
// ============================================================
void reverse_array(int *arr, int n) {
    int i = 0;
    int j = n - 1;
    while (i < j) {
        swap(&arr[i], &arr[j]);
        i++;
        j--;
    }
}

// ============================================================
// Additional utility: count inversions (brute force)
// ============================================================
int count_inversions(int *arr, int n) {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (arr[i] > arr[j]) {
                count++;
            }
        }
    }
    return count;
}

// ============================================================
// Test runner: sort with all 5 algorithms and compare
// ============================================================
void run_sorting_test(int *original, int n, int test_num) {
    int arr_bubble[50];
    int arr_select[50];
    int arr_insert[50];
    int arr_shell[50];
    int arr_merge[50];

    // Copy original into each working array
    copy_array(original, arr_bubble, n);
    copy_array(original, arr_select, n);
    copy_array(original, arr_insert, n);
    copy_array(original, arr_shell, n);
    copy_array(original, arr_merge, n);

    // Sort using each algorithm
    bubble_sort(arr_bubble, n);
    selection_sort(arr_select, n);
    insertion_sort(arr_insert, n);
    shell_sort(arr_shell, n);
    merge_sort(arr_merge, n);

    // Check all are sorted
    int b_ok = is_sorted(arr_bubble, n);
    int s_ok = is_sorted(arr_select, n);
    int i_ok = is_sorted(arr_insert, n);
    int h_ok = is_sorted(arr_shell, n);
    int m_ok = is_sorted(arr_merge, n);

    printf("Test %d: bubble_sorted=%d\n", test_num, b_ok);
    printf("Test %d: select_sorted=%d\n", test_num, s_ok);
    printf("Test %d: insert_sorted=%d\n", test_num, i_ok);
    printf("Test %d: shell_sorted=%d\n", test_num, h_ok);
    printf("Test %d: merge_sorted=%d\n", test_num, m_ok);

    // Check all produce same result
    int eq_bs = arrays_equal(arr_bubble, arr_select, n);
    int eq_si = arrays_equal(arr_select, arr_insert, n);
    int eq_ih = arrays_equal(arr_insert, arr_shell, n);
    int eq_hm = arrays_equal(arr_shell, arr_merge, n);

    printf("Test %d: all_equal=%d\n", test_num, eq_bs && eq_si && eq_ih && eq_hm);

    // Print first and last element of sorted result
    printf("Test %d: first=%d last=%d\n", test_num, arr_bubble[0], arr_bubble[n - 1]);
}

// ============================================================
// Generate a pseudo-random-looking array deterministically
// ============================================================
void generate_array(int *arr, int n, int seed) {
    int i;
    // Simple linear congruential-like generator
    int val = seed;
    for (i = 0; i < n; i++) {
        val = (val * 37 + 13) % 1000;
        arr[i] = val % 200 - 100; // range [-100, 99]
    }
}

// ============================================================
// Print statistics about an array
// ============================================================
void print_stats(int *arr, int n, int label) {
    int s = array_sum(arr, n);
    int mn = array_min(arr, n);
    int mx = array_max(arr, n);
    int inv = count_inversions(arr, n);
    printf("Stats %d: sum=%d min=%d max=%d inversions=%d\n", label, s, mn, mx, inv);
}

// ============================================================
// Test sorting a nearly sorted array
// ============================================================
void test_nearly_sorted(int test_num) {
    int arr[12];
    int i;
    // Create a sorted array then swap a couple elements
    for (i = 0; i < 12; i++) {
        arr[i] = i * 3 + 1;
    }
    // Swap elements at positions 2 and 5
    swap(&arr[2], &arr[5]);
    // Swap elements at positions 8 and 10
    swap(&arr[8], &arr[10]);

    int sorted_arr[12];
    copy_array(arr, sorted_arr, 12);
    insertion_sort(sorted_arr, 12);
    printf("Test %d: nearly_sorted_first=%d last=%d\n", test_num, sorted_arr[0], sorted_arr[11]);

    int shifts = insertion_sort_count_shifts(arr, 12);
    printf("Test %d: insertion_shifts=%d\n", test_num, shifts);
}

// ============================================================
// Test sorting a reverse-sorted array
// ============================================================
void test_reverse_sorted(int test_num) {
    int arr[10];
    int i;
    for (i = 0; i < 10; i++) {
        arr[i] = 100 - i * 10;
    }

    int swaps = bubble_sort_count_swaps(arr, 10);
    printf("Test %d: reverse_bubble_swaps=%d\n", test_num, swaps);

    int inv = count_inversions(arr, 10);
    printf("Test %d: reverse_inversions=%d\n", test_num, inv);

    int sorted_arr[10];
    copy_array(arr, sorted_arr, 10);
    bubble_sort(sorted_arr, 10);
    printf("Test %d: reverse_sorted_first=%d last=%d\n", test_num, sorted_arr[0], sorted_arr[9]);
}

// ============================================================
// Test sorting array with duplicates
// ============================================================
void test_duplicates(int test_num) {
    int arr[15];
    int i;
    // Create array with many duplicates
    for (i = 0; i < 15; i++) {
        arr[i] = (i * 7) % 5;
    }

    printf("Test %d: dup_original: ", test_num);
    print_array(arr, 15);

    int sorted_arr[15];
    copy_array(arr, sorted_arr, 15);
    merge_sort(sorted_arr, 15);
    printf("Test %d: dup_sorted: ", test_num);
    print_array(sorted_arr, 15);

    // Check stability hint: first element should be 0
    printf("Test %d: dup_first=%d dup_last=%d\n", test_num, sorted_arr[0], sorted_arr[14]);
}

// ============================================================
// Test sorting single element and two elements
// ============================================================
void test_edge_cases(int test_num) {
    // Single element
    int single[1];
    single[0] = 42;
    bubble_sort(single, 1);
    printf("Test %d: single=%d\n", test_num, single[0]);

    // Two elements already sorted
    int two_sorted[2];
    two_sorted[0] = 5;
    two_sorted[1] = 10;
    selection_sort(two_sorted, 2);
    printf("Test %d: two_sorted=%d %d\n", test_num, two_sorted[0], two_sorted[1]);

    // Two elements reversed
    int two_rev[2];
    two_rev[0] = 10;
    two_rev[1] = 5;
    insertion_sort(two_rev, 2);
    printf("Test %d: two_rev=%d %d\n", test_num, two_rev[0], two_rev[1]);
}

// ============================================================
// Test sorting all-same elements
// ============================================================
void test_all_same(int test_num) {
    int arr[8];
    int i;
    for (i = 0; i < 8; i++) {
        arr[i] = 7;
    }
    shell_sort(arr, 8);
    int ok = is_sorted(arr, 8);
    printf("Test %d: all_same_sorted=%d first=%d last=%d\n", test_num, ok, arr[0], arr[7]);
}

// ============================================================
// Verify sort stability by counting final positions
// ============================================================
void test_sort_properties(int test_num) {
    int arr[10];
    arr[0] = 50; arr[1] = 20; arr[2] = 40; arr[3] = 10; arr[4] = 30;
    arr[5] = 60; arr[6] = 80; arr[7] = 70; arr[8] = 90; arr[9] = 0;

    int comparisons = selection_sort_count_comparisons(arr, 10);
    printf("Test %d: select_comparisons=%d\n", test_num, comparisons);

    int shell_passes = shell_sort_count_passes(10);
    printf("Test %d: shell_passes=%d\n", test_num, shell_passes);

    int merge_ops = count_merge_ops(10);
    printf("Test %d: merge_ops=%d\n", test_num, merge_ops);
}

// ============================================================
// Large array sorting test
// ============================================================
void test_large_array(int test_num) {
    int arr[40];
    int i;
    // Fill with a deterministic pattern
    for (i = 0; i < 40; i++) {
        arr[i] = (i * 31 + 17) % 200 - 100;
    }

    print_stats(arr, 40, test_num);

    int sorted_arr[40];
    copy_array(arr, sorted_arr, 40);
    merge_sort(sorted_arr, 40);

    printf("Test %d: large_first=%d large_last=%d\n", test_num, sorted_arr[0], sorted_arr[39]);
    printf("Test %d: large_mid=%d\n", test_num, sorted_arr[20]);

    // Verify sum is preserved
    int orig_sum = array_sum(arr, 40);
    int sort_sum = array_sum(sorted_arr, 40);
    printf("Test %d: sum_preserved=%d\n", test_num, orig_sum == sort_sum);
}

// ============================================================
// Test negative numbers sorting
// ============================================================
void test_negative_numbers(int test_num) {
    int arr[8];
    arr[0] = -5; arr[1] = -10; arr[2] = -3; arr[3] = -8;
    arr[4] = -1; arr[5] = -15; arr[6] = -7; arr[7] = -2;

    int sorted_arr[8];
    copy_array(arr, sorted_arr, 8);
    bubble_sort(sorted_arr, 8);

    printf("Test %d: neg_first=%d neg_last=%d\n", test_num, sorted_arr[0], sorted_arr[7]);
    printf("Test %d: neg_sorted=%d\n", test_num, is_sorted(sorted_arr, 8));
}

// ============================================================
// Test mixed positive and negative
// ============================================================
void test_mixed_signs(int test_num) {
    int arr[10];
    arr[0] = 3; arr[1] = -7; arr[2] = 0; arr[3] = 15; arr[4] = -3;
    arr[5] = 8; arr[6] = -12; arr[7] = 1; arr[8] = -1; arr[9] = 6;

    int sorted_arr[10];
    copy_array(arr, sorted_arr, 10);
    shell_sort(sorted_arr, 10);

    printf("Test %d: mixed_first=%d mixed_last=%d\n", test_num, sorted_arr[0], sorted_arr[9]);
    printf("Test %d: mixed_sorted=%d\n", test_num, is_sorted(sorted_arr, 10));
    printf("Test %d: mixed_sum=%d\n", test_num, array_sum(sorted_arr, 10));
}


int main() {
    // ---- Test 1: Sort array {64, 34, 25, 12, 22, 11, 90, 94, 6, 55} ----
    int arr1[10];
    arr1[0] = 64; arr1[1] = 34; arr1[2] = 25; arr1[3] = 12; arr1[4] = 22;
    arr1[5] = 11; arr1[6] = 90; arr1[7] = 94; arr1[8] = 6; arr1[9] = 55;
    run_sorting_test(arr1, 10, 1);

    // ---- Test 2: Sort generated array with seed=7 ----
    int arr2[15];
    generate_array(arr2, 15, 7);
    run_sorting_test(arr2, 15, 2);

    // ---- Test 3: Sort generated array with seed=42 ----
    int arr3[20];
    generate_array(arr3, 20, 42);
    run_sorting_test(arr3, 20, 3);

    // ---- Test 4: Nearly sorted array ----
    test_nearly_sorted(4);

    // ---- Test 5: Reverse sorted array ----
    test_reverse_sorted(5);

    // ---- Test 6: Array with duplicates ----
    test_duplicates(6);

    // ---- Test 7: Edge cases ----
    test_edge_cases(7);

    // ---- Test 8: All same elements ----
    test_all_same(8);

    // ---- Test 9: Sort properties ----
    test_sort_properties(9);

    // ---- Test 10: Large array ----
    test_large_array(10);

    // ---- Test 11: Negative numbers ----
    test_negative_numbers(11);

    // ---- Test 12: Mixed signs ----
    test_mixed_signs(12);

    return 0;
}
