int printf(const char *fmt, ...);
// EXPECT: === Insertion Sort Tests ===
// EXPECT: --- Basic sort ---
// EXPECT: input: 12 11 13 5 6
// EXPECT: sorted: 5 6 11 12 13
// EXPECT: --- Already sorted ---
// EXPECT: input: 1 2 3 4 5 6 7 8
// EXPECT: sorted: 1 2 3 4 5 6 7 8
// EXPECT: --- Reverse sorted ---
// EXPECT: input: 8 7 6 5 4 3 2 1
// EXPECT: sorted: 1 2 3 4 5 6 7 8
// EXPECT: --- Single element ---
// EXPECT: sorted: 99
// EXPECT: --- Duplicates ---
// EXPECT: input: 3 1 4 1 5 9 2 6 5 3
// EXPECT: sorted: 1 1 2 3 3 4 5 5 6 9
// EXPECT: --- Negative values ---
// EXPECT: input: -5 3 -2 8 -1 0
// EXPECT: sorted: -5 -2 -1 0 3 8
// EXPECT: --- Stability test ---
// EXPECT: pair(1,1) pair(2,1) pair(0,3) pair(3,4) pair(4,5) pair(5,9)
// EXPECT: stable: key=1 first_id=1 second_id=2
// EXPECT: --- Shift count ---
// EXPECT: array: 5 4 3 2 1
// EXPECT: shifts: 10
// EXPECT: --- Nearly sorted (one swap) ---
// EXPECT: input: 1 3 2 4 5 6 7 8
// EXPECT: sorted: 1 2 3 4 5 6 7 8
// EXPECT: shifts_nearly: 1
// EXPECT: --- Large array ---
// EXPECT: first: 0 last: 24
// EXPECT: --- Binary insertion sort ---
// EXPECT: input: 7 2 9 1 6 3
// EXPECT: bin_sorted: 1 2 3 6 7 9
// EXPECT: === Insertion Sort Done ===
// --- Basic sort ---
// input: 12 11 13 5 6
// sorted: 5 6 11 12 13
// --- Already sorted ---
// input: 1 2 3 4 5 6 7 8
// sorted: 1 2 3 4 5 6 7 8
// --- Reverse sorted ---
// input: 8 7 6 5 4 3 2 1
// sorted: 1 2 3 4 5 6 7 8
// --- Single element ---
// sorted: 99
// --- Duplicates ---
// input: 3 1 4 1 5 9 2 6 5 3
// sorted: 1 1 2 3 3 4 5 5 6 9
// --- Negative values ---
// input: -5 3 -2 8 -1 0
// sorted: -5 -2 -1 0 3 8
// --- Stability test ---
// pair(1,1) pair(2,1) pair(0,3) pair(3,4) pair(4,5) pair(5,9)
// stable: key=1 first_id=1 second_id=2
// --- Shift count ---
// array: 5 4 3 2 1
// shifts: 10
// --- Nearly sorted (one swap) ---
// input: 1 3 2 4 5 6 7 8
// sorted: 1 2 3 4 5 6 7 8
// shifts_nearly: 1
// --- Large array ---
// first: 0 last: 24
// --- Binary insertion sort ---
// input: 7 2 9 1 6 3
// bin_sorted: 1 2 3 6 7 9
// === Insertion Sort Done ===

void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

void insertion_sort(int *arr, int n) {
    int i;
    int j;
    int key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int insertion_sort_count_shifts(int *arr, int n) {
    int i;
    int j;
    int key;
    int shifts = 0;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
            shifts = shifts + 1;
        }
        arr[j + 1] = key;
    }
    return shifts;
}

// Binary insertion sort: use binary search to find insertion position
void binary_insertion_sort(int *arr, int n) {
    int i;
    int j;
    int key;
    int lo;
    int hi;
    int mid;
    int pos;
    for (i = 1; i < n; i++) {
        key = arr[i];
        lo = 0;
        hi = i - 1;
        // Binary search for insertion position
        while (lo <= hi) {
            mid = (lo + hi) / 2;
            if (arr[mid] > key)
                hi = mid - 1;
            else
                lo = mid + 1;
        }
        pos = lo;
        // Shift elements
        for (j = i - 1; j >= pos; j--) {
            arr[j + 1] = arr[j];
        }
        arr[pos] = key;
    }
}

// Stability test structures emulated with parallel arrays
void insertion_sort_stable(int *keys, int *ids, int n) {
    int i;
    int j;
    int kkey;
    int kid;
    for (i = 1; i < n; i++) {
        kkey = keys[i];
        kid = ids[i];
        j = i - 1;
        while (j >= 0 && keys[j] > kkey) {
            keys[j + 1] = keys[j];
            ids[j + 1] = ids[j];
            j = j - 1;
        }
        keys[j + 1] = kkey;
        ids[j + 1] = kid;
    }
}

int main() {
    printf("=== Insertion Sort Tests ===\n");

    // Basic sort
    printf("--- Basic sort ---\n");
    int a1[5] = {12, 11, 13, 5, 6};
    printf("input: ");
    print_array(a1, 5);
    insertion_sort(a1, 5);
    printf("sorted: ");
    print_array(a1, 5);

    // Already sorted
    printf("--- Already sorted ---\n");
    int a2[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    printf("input: ");
    print_array(a2, 8);
    insertion_sort(a2, 8);
    printf("sorted: ");
    print_array(a2, 8);

    // Reverse sorted
    printf("--- Reverse sorted ---\n");
    int a3[8] = {8, 7, 6, 5, 4, 3, 2, 1};
    printf("input: ");
    print_array(a3, 8);
    insertion_sort(a3, 8);
    printf("sorted: ");
    print_array(a3, 8);

    // Single element
    printf("--- Single element ---\n");
    int a4[1] = {99};
    insertion_sort(a4, 1);
    printf("sorted: ");
    print_array(a4, 1);

    // Duplicates
    printf("--- Duplicates ---\n");
    int a5[10] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    printf("input: ");
    print_array(a5, 10);
    insertion_sort(a5, 10);
    printf("sorted: ");
    print_array(a5, 10);

    // Negative values
    printf("--- Negative values ---\n");
    int a6[6] = {-5, 3, -2, 8, -1, 0};
    printf("input: ");
    print_array(a6, 6);
    insertion_sort(a6, 6);
    printf("sorted: ");
    print_array(a6, 6);

    // Stability test
    printf("--- Stability test ---\n");
    int keys[6] = {3, 1, 1, 4, 5, 9};
    int ids[6]  = {0, 1, 2, 3, 4, 5};
    insertion_sort_stable(keys, ids, 6);
    int i;
    for (i = 0; i < 6; i++) {
        if (i > 0) printf(" ");
        printf("pair(%d,%d)", ids[i], keys[i]);
    }
    printf("\n");
    // Check stability: the two 1s should keep original order (id=1 before id=2)
    int first_one_id = -1;
    int second_one_id = -1;
    for (i = 0; i < 6; i++) {
        if (keys[i] == 1) {
            if (first_one_id == -1) first_one_id = ids[i];
            else second_one_id = ids[i];
        }
    }
    printf("stable: key=1 first_id=%d second_id=%d\n", first_one_id, second_one_id);

    // Shift count
    printf("--- Shift count ---\n");
    int a7[5] = {5, 4, 3, 2, 1};
    printf("array: ");
    print_array(a7, 5);
    int shifts = insertion_sort_count_shifts(a7, 5);
    printf("shifts: %d\n", shifts);

    // Nearly sorted
    printf("--- Nearly sorted (one swap) ---\n");
    int a8[8] = {1, 3, 2, 4, 5, 6, 7, 8};
    printf("input: ");
    print_array(a8, 8);
    int s2 = insertion_sort_count_shifts(a8, 8);
    printf("sorted: ");
    print_array(a8, 8);
    printf("shifts_nearly: %d\n", s2);

    // Large array
    printf("--- Large array ---\n");
    int a9[25];
    for (i = 0; i < 25; i++) {
        a9[i] = 24 - i;
    }
    insertion_sort(a9, 25);
    printf("first: %d last: %d\n", a9[0], a9[24]);

    // Binary insertion sort
    printf("--- Binary insertion sort ---\n");
    int a10[6] = {7, 2, 9, 1, 6, 3};
    printf("input: ");
    print_array(a10, 6);
    binary_insertion_sort(a10, 6);
    printf("bin_sorted: ");
    print_array(a10, 6);

    printf("=== Insertion Sort Done ===\n");
    return 0;
}
