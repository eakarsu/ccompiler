int printf(const char *fmt, ...);

// Insertion sort with shift counting
// Tracks how many shifts (moves) each element requires

void insertion_sort(int *arr, int n, int *total_shifts) {
    *total_shifts = 0;
    int i;
    for (i = 1; i < n; i = i + 1) {
        int key = arr[i];
        int j = i - 1;
        int shifts = 0;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
            shifts = shifts + 1;
        }
        arr[j + 1] = key;
        *total_shifts = *total_shifts + shifts;
    }
}

// Sort and return the index where each pass inserted
void insertion_sort_verbose(int *arr, int n, int *shift_per_pass) {
    int i;
    for (i = 1; i < n; i = i + 1) {
        int key = arr[i];
        int j = i - 1;
        int shifts = 0;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
            shifts = shifts + 1;
        }
        arr[j + 1] = key;
        shift_per_pass[i - 1] = shifts;
    }
}

int is_sorted(int *arr, int n) {
    int i;
    for (i = 0; i < n - 1; i = i + 1) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

// Binary insertion sort - uses binary search to find position
void binary_insertion_sort(int *arr, int n, int *total_comps) {
    *total_comps = 0;
    int i;
    for (i = 1; i < n; i = i + 1) {
        int key = arr[i];
        int lo = 0;
        int hi = i;
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            *total_comps = *total_comps + 1;
            if (arr[mid] > key) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }
        int j = i - 1;
        while (j >= lo) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[lo] = key;
    }
}

int main() {
    // Test 1: Basic array
    int a1[6];
    a1[0] = 12; a1[1] = 11; a1[2] = 13; a1[3] = 5; a1[4] = 6; a1[5] = 7;
    int total_shifts;
    insertion_sort(a1, 6, &total_shifts);
    printf("sorted: %d %d %d %d %d %d\n", a1[0], a1[1], a1[2], a1[3], a1[4], a1[5]);
    // EXPECT: sorted: 5 6 7 11 12 13
    printf("total shifts: %d\n", total_shifts);
    // EXPECT: total shifts: 10

    // Test 2: Already sorted
    int a2[5];
    a2[0] = 1; a2[1] = 2; a2[2] = 3; a2[3] = 4; a2[4] = 5;
    insertion_sort(a2, 5, &total_shifts);
    printf("presorted shifts: %d\n", total_shifts);
    // EXPECT: presorted shifts: 0

    // Test 3: Reverse sorted
    int a3[5];
    a3[0] = 5; a3[1] = 4; a3[2] = 3; a3[3] = 2; a3[4] = 1;
    insertion_sort(a3, 5, &total_shifts);
    printf("reverse: %d %d %d %d %d\n", a3[0], a3[1], a3[2], a3[3], a3[4]);
    // EXPECT: reverse: 1 2 3 4 5
    printf("reverse shifts: %d\n", total_shifts);
    // EXPECT: reverse shifts: 10

    // Test 4: Verbose per-pass tracking
    int a4[7];
    a4[0] = 50; a4[1] = 30; a4[2] = 40; a4[3] = 10;
    a4[4] = 60; a4[5] = 20; a4[6] = 70;
    int spp[6];
    insertion_sort_verbose(a4, 7, spp);
    printf("per pass: %d %d %d %d %d %d\n", spp[0], spp[1], spp[2], spp[3], spp[4], spp[5]);
    // EXPECT: per pass: 1 1 3 0 4 0
    printf("verb sorted: %d %d %d %d %d %d %d\n", a4[0], a4[1], a4[2], a4[3], a4[4], a4[5], a4[6]);
    // EXPECT: verb sorted: 10 20 30 40 50 60 70

    // Test 5: Duplicates
    int a5[6];
    a5[0] = 3; a5[1] = 1; a5[2] = 3; a5[3] = 1; a5[4] = 2; a5[5] = 2;
    insertion_sort(a5, 6, &total_shifts);
    printf("dupes: %d %d %d %d %d %d\n", a5[0], a5[1], a5[2], a5[3], a5[4], a5[5]);
    // EXPECT: dupes: 1 1 2 2 3 3
    printf("dupes shifts: %d\n", total_shifts);
    // EXPECT: dupes shifts: 7

    // Test 6: Binary insertion sort
    int a6[8];
    a6[0] = 37; a6[1] = 23; a6[2] = 0; a6[3] = 17;
    a6[4] = 12; a6[5] = 72; a6[6] = 31; a6[7] = 46;
    int bin_comps;
    binary_insertion_sort(a6, 8, &bin_comps);
    printf("bin sorted: %d %d %d %d %d %d %d %d\n",
           a6[0], a6[1], a6[2], a6[3], a6[4], a6[5], a6[6], a6[7]);
    // EXPECT: bin sorted: 0 12 17 23 31 37 46 72
    printf("bin comps: %d\n", bin_comps);
    // EXPECT: bin comps: 16
    printf("bin is_sorted: %d\n", is_sorted(a6, 8));
    // EXPECT: bin is_sorted: 1

    // Test 7: Single element
    int a7[1];
    a7[0] = 99;
    insertion_sort(a7, 1, &total_shifts);
    printf("single: %d shifts: %d\n", a7[0], total_shifts);
    // EXPECT: single: 99 shifts: 0

    // Test 8: All same elements
    int a8[4];
    a8[0] = 5; a8[1] = 5; a8[2] = 5; a8[3] = 5;
    insertion_sort(a8, 4, &total_shifts);
    printf("all same: %d %d %d %d shifts: %d\n", a8[0], a8[1], a8[2], a8[3], total_shifts);
    // EXPECT: all same: 5 5 5 5 shifts: 0

    printf("done\n");
    // EXPECT: done

    return 0;
}
