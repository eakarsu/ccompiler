int printf(const char *fmt, ...);
// EXPECT: === Quicksort Tests ===
// EXPECT: --- Basic sort ---
// EXPECT: input: 10 7 8 9 1 5
// EXPECT: sorted: 1 5 7 8 9 10
// EXPECT: --- Already sorted ---
// EXPECT: input: 1 2 3 4 5 6 7
// EXPECT: sorted: 1 2 3 4 5 6 7
// EXPECT: --- Reverse sorted ---
// EXPECT: input: 9 8 7 6 5 4 3 2 1
// EXPECT: sorted: 1 2 3 4 5 6 7 8 9
// EXPECT: --- Single element ---
// EXPECT: sorted: 55
// EXPECT: --- Two elements ---
// EXPECT: input: 9 1
// EXPECT: sorted: 1 9
// EXPECT: --- All same ---
// EXPECT: input: 3 3 3 3 3
// EXPECT: sorted: 3 3 3 3 3
// EXPECT: --- Many duplicates ---
// EXPECT: input: 4 2 1 3 2 4 1 3
// EXPECT: sorted: 1 1 2 2 3 3 4 4
// EXPECT: --- Negative values ---
// EXPECT: input: -7 3 -2 9 -5 0 4
// EXPECT: sorted: -7 -5 -2 0 3 4 9
// EXPECT: --- Partition demo ---
// EXPECT: before: 5 3 8 1 9 2 7
// EXPECT: pivot_pos: 3
// EXPECT: after: 1 3 2 5 9 8 7
// EXPECT: --- Three-way partition (Dutch flag) ---
// EXPECT: input: 2 0 1 2 0 1 0 2 1 0
// EXPECT: sorted: 0 0 0 0 1 1 1 2 2 2
// EXPECT: --- Quicksort with median-of-three ---
// EXPECT: input: 15 3 9 1 12 7 6 2 14 8
// EXPECT: sorted: 1 2 3 6 7 8 9 12 14 15
// EXPECT: --- Large array ---
// EXPECT: first: 0 last: 39
// EXPECT: === Quicksort Done ===
// --- Basic sort ---
// input: 10 7 8 9 1 5
// sorted: 1 5 7 8 9 10
// --- Already sorted ---
// input: 1 2 3 4 5 6 7
// sorted: 1 2 3 4 5 6 7
// --- Reverse sorted ---
// input: 9 8 7 6 5 4 3 2 1
// sorted: 1 2 3 4 5 6 7 8 9
// --- Single element ---
// sorted: 55
// --- Two elements ---
// input: 9 1
// sorted: 1 9
// --- All same ---
// input: 3 3 3 3 3
// sorted: 3 3 3 3 3
// --- Many duplicates ---
// input: 4 2 1 3 2 4 1 3
// sorted: 1 1 2 2 3 3 4 4
// --- Negative values ---
// input: -7 3 -2 9 -5 0 4
// sorted: -7 -5 -2 0 3 4 9
// --- Partition demo ---
// before: 5 3 8 1 9 2 7
// pivot_pos: 3
// after: 1 3 2 5 9 8 7
// --- Three-way partition (Dutch flag) ---
// input: 2 0 1 2 0 1 0 2 1 0
// sorted: 0 0 0 0 1 1 1 2 2 2
// --- Quicksort with median-of-three ---
// input: 15 3 9 1 12 7 6 2 14 8
// sorted: 1 2 3 6 7 8 9 12 14 15
// --- Large array ---
// first: 0 last: 39
// === Quicksort Done ===

void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// Lomuto partition: pivot = last element
int partition(int *arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    int j;
    for (j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i = i + 1;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quicksort_helper(int *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort_helper(arr, low, pi - 1);
        quicksort_helper(arr, pi + 1, high);
    }
}

void quicksort(int *arr, int n) {
    if (n > 1)
        quicksort_helper(arr, 0, n - 1);
}

// Partition returning pivot position (for demo)
int partition_demo(int *arr, int low, int high) {
    int pivot = arr[low];
    int i = low + 1;
    int j = high;
    while (1) {
        while (i <= high && arr[i] <= pivot) i = i + 1;
        while (j > low && arr[j] >= pivot) j = j - 1;
        if (i >= j) break;
        swap(&arr[i], &arr[j]);
    }
    swap(&arr[low], &arr[j]);
    return j;
}

// Dutch National Flag / 3-way partition sort (for arrays with few distinct values)
void three_way_sort(int *arr, int n) {
    int lo = 0;
    int mid = 0;
    int hi = n - 1;
    // Sort values 0, 1, 2
    while (mid <= hi) {
        if (arr[mid] == 0) {
            swap(&arr[lo], &arr[mid]);
            lo = lo + 1;
            mid = mid + 1;
        } else if (arr[mid] == 1) {
            mid = mid + 1;
        } else {
            swap(&arr[mid], &arr[hi]);
            hi = hi - 1;
        }
    }
}

// Median of three: pick median of first, mid, last as pivot
int median_of_three(int *arr, int low, int high) {
    int mid = low + (high - low) / 2;
    if (arr[low] > arr[mid]) swap(&arr[low], &arr[mid]);
    if (arr[low] > arr[high]) swap(&arr[low], &arr[high]);
    if (arr[mid] > arr[high]) swap(&arr[mid], &arr[high]);
    // Place median at high-1
    swap(&arr[mid], &arr[high]);
    return arr[high];
}

int partition_median(int *arr, int low, int high) {
    int pivot = median_of_three(arr, low, high);
    int i = low - 1;
    int j;
    for (j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i = i + 1;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quicksort_median_helper(int *arr, int low, int high) {
    if (low < high) {
        int pi;
        if (high - low < 3) {
            // Use simple partition for small subarrays
            pi = partition(arr, low, high);
        } else {
            pi = partition_median(arr, low, high);
        }
        quicksort_median_helper(arr, low, pi - 1);
        quicksort_median_helper(arr, pi + 1, high);
    }
}

void quicksort_median(int *arr, int n) {
    if (n > 1)
        quicksort_median_helper(arr, 0, n - 1);
}

int main() {
    printf("=== Quicksort Tests ===\n");

    // Basic sort
    printf("--- Basic sort ---\n");
    int a1[6] = {10, 7, 8, 9, 1, 5};
    printf("input: ");
    print_array(a1, 6);
    quicksort(a1, 6);
    printf("sorted: ");
    print_array(a1, 6);

    // Already sorted
    printf("--- Already sorted ---\n");
    int a2[7] = {1, 2, 3, 4, 5, 6, 7};
    printf("input: ");
    print_array(a2, 7);
    quicksort(a2, 7);
    printf("sorted: ");
    print_array(a2, 7);

    // Reverse sorted
    printf("--- Reverse sorted ---\n");
    int a3[9] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    printf("input: ");
    print_array(a3, 9);
    quicksort(a3, 9);
    printf("sorted: ");
    print_array(a3, 9);

    // Single element
    printf("--- Single element ---\n");
    int a4[1] = {55};
    quicksort(a4, 1);
    printf("sorted: ");
    print_array(a4, 1);

    // Two elements
    printf("--- Two elements ---\n");
    int a5[2] = {9, 1};
    printf("input: ");
    print_array(a5, 2);
    quicksort(a5, 2);
    printf("sorted: ");
    print_array(a5, 2);

    // All same
    printf("--- All same ---\n");
    int a6[5] = {3, 3, 3, 3, 3};
    printf("input: ");
    print_array(a6, 5);
    quicksort(a6, 5);
    printf("sorted: ");
    print_array(a6, 5);

    // Many duplicates
    printf("--- Many duplicates ---\n");
    int a7[8] = {4, 2, 1, 3, 2, 4, 1, 3};
    printf("input: ");
    print_array(a7, 8);
    quicksort(a7, 8);
    printf("sorted: ");
    print_array(a7, 8);

    // Negative values
    printf("--- Negative values ---\n");
    int a8[7] = {-7, 3, -2, 9, -5, 0, 4};
    printf("input: ");
    print_array(a8, 7);
    quicksort(a8, 7);
    printf("sorted: ");
    print_array(a8, 7);

    // Partition demo
    printf("--- Partition demo ---\n");
    int a9[7] = {5, 3, 8, 1, 9, 2, 7};
    printf("before: ");
    print_array(a9, 7);
    int pp = partition_demo(a9, 0, 6);
    printf("pivot_pos: %d\n", pp);
    printf("after: ");
    print_array(a9, 7);

    // Three-way partition (Dutch flag)
    printf("--- Three-way partition (Dutch flag) ---\n");
    int a10[10] = {2, 0, 1, 2, 0, 1, 0, 2, 1, 0};
    printf("input: ");
    print_array(a10, 10);
    three_way_sort(a10, 10);
    printf("sorted: ");
    print_array(a10, 10);

    // Quicksort with median-of-three
    printf("--- Quicksort with median-of-three ---\n");
    int a11[10] = {15, 3, 9, 1, 12, 7, 6, 2, 14, 8};
    printf("input: ");
    print_array(a11, 10);
    quicksort_median(a11, 10);
    printf("sorted: ");
    print_array(a11, 10);

    // Large array
    printf("--- Large array ---\n");
    int a12[40];
    int i;
    for (i = 0; i < 40; i++) {
        a12[i] = 39 - i;
    }
    quicksort(a12, 40);
    printf("first: %d last: %d\n", a12[0], a12[39]);

    printf("=== Quicksort Done ===\n");
    return 0;
}
