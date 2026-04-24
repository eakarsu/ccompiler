int printf(const char *fmt, ...);

// Selection sort with swap tracking
// Finds minimum element and swaps it into position

struct SortStats {
    int comparisons;
    int swaps;
    int min_idx_changes;  // how many times we updated the min index
};

void selection_sort(int *arr, int n, struct SortStats *stats) {
    stats->comparisons = 0;
    stats->swaps = 0;
    stats->min_idx_changes = 0;

    int i;
    int j;
    for (i = 0; i < n - 1; i = i + 1) {
        int min_idx = i;
        for (j = i + 1; j < n; j = j + 1) {
            stats->comparisons = stats->comparisons + 1;
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
                stats->min_idx_changes = stats->min_idx_changes + 1;
            }
        }
        if (min_idx != i) {
            int tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
            stats->swaps = stats->swaps + 1;
        }
    }
}

// Double selection sort: finds both min and max in each pass
void double_selection_sort(int *arr, int n, int *passes) {
    *passes = 0;
    int lo = 0;
    int hi = n - 1;
    while (lo < hi) {
        int min_idx = lo;
        int max_idx = lo;
        int j;
        for (j = lo; j <= hi; j = j + 1) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
            if (arr[j] > arr[max_idx]) {
                max_idx = j;
            }
        }
        // Swap min into lo position
        int tmp = arr[lo];
        arr[lo] = arr[min_idx];
        arr[min_idx] = tmp;
        // If max was at lo, it has been moved to min_idx
        if (max_idx == lo) {
            max_idx = min_idx;
        }
        // Swap max into hi position
        tmp = arr[hi];
        arr[hi] = arr[max_idx];
        arr[max_idx] = tmp;
        lo = lo + 1;
        hi = hi - 1;
        *passes = *passes + 1;
    }
}

int find_min(int *arr, int n) {
    int min = arr[0];
    int i;
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

int find_max(int *arr, int n) {
    int max = arr[0];
    int i;
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

int main() {
    struct SortStats stats;

    // Test 1: Basic sort
    int a1[6];
    a1[0] = 64; a1[1] = 25; a1[2] = 12; a1[3] = 22; a1[4] = 11; a1[5] = 90;
    selection_sort(a1, 6, &stats);
    printf("sorted: %d %d %d %d %d %d\n", a1[0], a1[1], a1[2], a1[3], a1[4], a1[5]);
    // EXPECT: sorted: 11 12 22 25 64 90
    printf("comps: %d swaps: %d min_changes: %d\n", stats.comparisons, stats.swaps, stats.min_idx_changes);
    // EXPECT: comps: 15 swaps: 3 min_changes: 5

    // Test 2: Already sorted
    int a2[4];
    a2[0] = 10; a2[1] = 20; a2[2] = 30; a2[3] = 40;
    selection_sort(a2, 4, &stats);
    printf("presorted swaps: %d\n", stats.swaps);
    // EXPECT: presorted swaps: 0
    printf("presorted comps: %d\n", stats.comparisons);
    // EXPECT: presorted comps: 6

    // Test 3: Reverse sorted
    int a3[5];
    a3[0] = 50; a3[1] = 40; a3[2] = 30; a3[3] = 20; a3[4] = 10;
    selection_sort(a3, 5, &stats);
    printf("reverse: %d %d %d %d %d\n", a3[0], a3[1], a3[2], a3[3], a3[4]);
    // EXPECT: reverse: 10 20 30 40 50
    printf("rev comps: %d swaps: %d\n", stats.comparisons, stats.swaps);
    // EXPECT: rev comps: 10 swaps: 2

    // Test 4: Double selection sort
    int a4[8];
    a4[0] = 8; a4[1] = 3; a4[2] = 7; a4[3] = 1;
    a4[4] = 5; a4[5] = 2; a4[6] = 6; a4[7] = 4;
    int passes;
    double_selection_sort(a4, 8, &passes);
    printf("double: %d %d %d %d %d %d %d %d\n",
           a4[0], a4[1], a4[2], a4[3], a4[4], a4[5], a4[6], a4[7]);
    // EXPECT: double: 1 2 3 4 5 6 7 8
    printf("double passes: %d\n", passes);
    // EXPECT: double passes: 4

    // Test 5: Selection sort with all same
    int a5[4];
    a5[0] = 7; a5[1] = 7; a5[2] = 7; a5[3] = 7;
    selection_sort(a5, 4, &stats);
    printf("same swaps: %d min_changes: %d\n", stats.swaps, stats.min_idx_changes);
    // EXPECT: same swaps: 0 min_changes: 0

    // Test 6: Two elements
    int a6[2];
    a6[0] = 20; a6[1] = 10;
    selection_sort(a6, 2, &stats);
    printf("two: %d %d swaps: %d\n", a6[0], a6[1], stats.swaps);
    // EXPECT: two: 10 20 swaps: 1

    // Test 7: Double selection on odd-length array
    int a7[7];
    a7[0] = 44; a7[1] = 11; a7[2] = 77; a7[3] = 33;
    a7[4] = 66; a7[5] = 22; a7[6] = 55;
    double_selection_sort(a7, 7, &passes);
    printf("odd double: %d %d %d %d %d %d %d\n",
           a7[0], a7[1], a7[2], a7[3], a7[4], a7[5], a7[6]);
    // EXPECT: odd double: 11 22 33 44 55 66 77
    printf("odd passes: %d\n", passes);
    // EXPECT: odd passes: 3

    // Test 8: Selection sort comparison count is always n*(n-1)/2
    int a8[6];
    a8[0] = 99; a8[1] = 1; a8[2] = 50; a8[3] = 25; a8[4] = 75; a8[5] = 10;
    selection_sort(a8, 6, &stats);
    printf("always comps: %d expected: %d\n", stats.comparisons, 6 * 5 / 2);
    // EXPECT: always comps: 15 expected: 15
    printf("result: %d %d %d %d %d %d\n", a8[0], a8[1], a8[2], a8[3], a8[4], a8[5]);
    // EXPECT: result: 1 10 25 50 75 99

    printf("done\n");
    // EXPECT: done

    return 0;
}
