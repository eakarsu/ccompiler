int printf(const char *fmt, ...);

// Quick sort with partition tracking
// Non-recursive implementation using explicit stack

struct StackFrame {
    int lo;
    int hi;
};

int partition(int *arr, int lo, int hi, int *comps, int *swaps) {
    int pivot = arr[hi];
    int i = lo - 1;
    int j;
    for (j = lo; j < hi; j = j + 1) {
        *comps = *comps + 1;
        if (arr[j] <= pivot) {
            i = i + 1;
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            *swaps = *swaps + 1;
        }
    }
    // Place pivot in correct position
    int tmp = arr[i + 1];
    arr[i + 1] = arr[hi];
    arr[hi] = tmp;
    *swaps = *swaps + 1;
    return i + 1;
}

void quicksort(int *arr, int n, int *total_comps, int *total_swaps, int *partitions) {
    *total_comps = 0;
    *total_swaps = 0;
    *partitions = 0;

    if (n <= 1) return;

    struct StackFrame stack[32];
    int top = 0;
    stack[top].lo = 0;
    stack[top].hi = n - 1;
    top = top + 1;

    while (top > 0) {
        top = top - 1;
        int lo = stack[top].lo;
        int hi = stack[top].hi;

        if (lo < hi) {
            int p = partition(arr, lo, hi, total_comps, total_swaps);
            *partitions = *partitions + 1;

            // Push right subarray
            if (p + 1 < hi) {
                stack[top].lo = p + 1;
                stack[top].hi = hi;
                top = top + 1;
            }
            // Push left subarray
            if (lo < p - 1) {
                stack[top].lo = lo;
                stack[top].hi = p - 1;
                top = top + 1;
            }
        }
    }
}

// Partition that returns pivot position for median-of-three
int median_of_three(int a, int b, int c) {
    if (a <= b) {
        if (b <= c) return b;
        if (a <= c) return c;
        return a;
    }
    if (a <= c) return a;
    if (b <= c) return c;
    return b;
}

int partition_mo3(int *arr, int lo, int hi) {
    int mid = (lo + hi) / 2;
    int pivot = median_of_three(arr[lo], arr[mid], arr[hi]);
    // Find and move pivot to hi
    if (arr[lo] == pivot) {
        int tmp = arr[lo]; arr[lo] = arr[hi]; arr[hi] = tmp;
    } else if (arr[mid] == pivot) {
        int tmp = arr[mid]; arr[mid] = arr[hi]; arr[hi] = tmp;
    }
    int i = lo - 1;
    int j;
    for (j = lo; j < hi; j = j + 1) {
        if (arr[j] <= pivot) {
            i = i + 1;
            int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
        }
    }
    int tmp = arr[i + 1]; arr[i + 1] = arr[hi]; arr[hi] = tmp;
    return i + 1;
}

int is_sorted(int *arr, int n) {
    int i;
    for (i = 0; i < n - 1; i = i + 1) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

int main() {
    int comps;
    int swaps;
    int parts;

    // Test 1: Basic sort
    int a1[8];
    a1[0] = 10; a1[1] = 80; a1[2] = 30; a1[3] = 90;
    a1[4] = 40; a1[5] = 50; a1[6] = 70; a1[7] = 20;
    quicksort(a1, 8, &comps, &swaps, &parts);
    printf("sorted: %d %d %d %d %d %d %d %d\n",
           a1[0], a1[1], a1[2], a1[3], a1[4], a1[5], a1[6], a1[7]);
    // EXPECT: sorted: 10 20 30 40 50 70 80 90
    printf("comps: %d swaps: %d parts: %d\n", comps, swaps, parts);
    // EXPECT: comps: 18 swaps: 16 parts: 5

    // Test 2: Already sorted
    int a2[5];
    a2[0] = 1; a2[1] = 2; a2[2] = 3; a2[3] = 4; a2[4] = 5;
    quicksort(a2, 5, &comps, &swaps, &parts);
    printf("presorted: %d %d %d %d %d\n", a2[0], a2[1], a2[2], a2[3], a2[4]);
    // EXPECT: presorted: 1 2 3 4 5
    printf("pre comps: %d parts: %d\n", comps, parts);
    // EXPECT: pre comps: 10 parts: 4

    // Test 3: Reverse sorted
    int a3[6];
    a3[0] = 6; a3[1] = 5; a3[2] = 4; a3[3] = 3; a3[4] = 2; a3[5] = 1;
    quicksort(a3, 6, &comps, &swaps, &parts);
    printf("reverse: %d %d %d %d %d %d\n", a3[0], a3[1], a3[2], a3[3], a3[4], a3[5]);
    // EXPECT: reverse: 1 2 3 4 5 6
    printf("rev comps: %d swaps: %d\n", comps, swaps);
    // EXPECT: rev comps: 15 swaps: 11

    // Test 4: Duplicates
    int a4[6];
    a4[0] = 3; a4[1] = 1; a4[2] = 2; a4[3] = 3; a4[4] = 1; a4[5] = 2;
    quicksort(a4, 6, &comps, &swaps, &parts);
    printf("dupes: %d %d %d %d %d %d\n", a4[0], a4[1], a4[2], a4[3], a4[4], a4[5]);
    // EXPECT: dupes: 1 1 2 2 3 3
    printf("dupes parts: %d\n", parts);
    // EXPECT: dupes parts: 3

    // Test 5: Single element
    int a5[1];
    a5[0] = 77;
    quicksort(a5, 1, &comps, &swaps, &parts);
    printf("single: %d parts: %d\n", a5[0], parts);
    // EXPECT: single: 77 parts: 0

    // Test 6: Median of three test
    int m1 = median_of_three(1, 2, 3);
    int m2 = median_of_three(3, 1, 2);
    int m3 = median_of_three(5, 5, 5);
    int m4 = median_of_three(10, 1, 5);
    printf("medians: %d %d %d %d\n", m1, m2, m3, m4);
    // EXPECT: medians: 2 2 5 5

    // Test 7: Partition of small array
    int a7[5];
    a7[0] = 4; a7[1] = 2; a7[2] = 5; a7[3] = 1; a7[4] = 3;
    int pc = 0;
    int ps = 0;
    int pidx = partition(a7, 0, 4, &pc, &ps);
    printf("pivot at: %d val: %d\n", pidx, a7[pidx]);
    // EXPECT: pivot at: 2 val: 3
    printf("is_sorted after qsort: %d\n", is_sorted(a1, 8));
    // EXPECT: is_sorted after qsort: 1

    printf("done\n");
    // EXPECT: done

    return 0;
}
