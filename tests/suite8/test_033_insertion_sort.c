int printf(const char *fmt, ...);

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

void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", arr[i]);
    }
    printf("\n");
}

int is_sorted(int *arr, int n) {
    int i;
    for (i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

void insertion_sort_partial(int *arr, int left, int right) {
    int i;
    int j;
    int key;
    for (i = left + 1; i <= right; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

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
        while (lo <= hi) {
            mid = (lo + hi) / 2;
            if (arr[mid] > key) {
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }
        pos = lo;
        for (j = i - 1; j >= pos; j--) {
            arr[j + 1] = arr[j];
        }
        arr[pos] = key;
    }
}

int main() {
    int arr1[8];
    int arr2[6];
    int arr3[10];
    int arr4[5];
    int arr5[12];

    arr1[0] = 12; arr1[1] = 11; arr1[2] = 13; arr1[3] = 5;
    arr1[4] = 6;  arr1[5] = 7;  arr1[6] = 3;  arr1[7] = 1;

    printf("Test 1: Basic insertion sort\n");
    // EXPECT: Test 1: Basic insertion sort
    printf("Before: ");
    // EXPECT: Before: 12 11 13 5 6 7 3 1
    print_array(arr1, 8);
    insertion_sort(arr1, 8);
    printf("After:  ");
    // EXPECT: After:  1 3 5 6 7 11 12 13
    print_array(arr1, 8);
    printf("Sorted: %d\n", is_sorted(arr1, 8));
    // EXPECT: Sorted: 1

    arr2[0] = -3; arr2[1] = -1; arr2[2] = -5; arr2[3] = -2; arr2[4] = -4; arr2[5] = 0;
    printf("Test 2: Negative numbers\n");
    // EXPECT: Test 2: Negative numbers
    insertion_sort(arr2, 6);
    printf("After:  ");
    // EXPECT: After:  -5 -4 -3 -2 -1 0
    print_array(arr2, 6);

    arr3[0] = 50; arr3[1] = 40; arr3[2] = 30; arr3[3] = 20; arr3[4] = 10;
    arr3[5] = 5;  arr3[6] = 4;  arr3[7] = 3;  arr3[8] = 2;  arr3[9] = 1;
    printf("Test 3: Worst case (reverse)\n");
    // EXPECT: Test 3: Worst case (reverse)
    insertion_sort(arr3, 10);
    printf("After:  ");
    // EXPECT: After:  1 2 3 4 5 10 20 30 40 50
    print_array(arr3, 10);

    printf("Test 4: Binary insertion sort\n");
    // EXPECT: Test 4: Binary insertion sort
    arr4[0] = 37; arr4[1] = 23; arr4[2] = 0; arr4[3] = 17; arr4[4] = 12;
    binary_insertion_sort(arr4, 5);
    printf("After:  ");
    // EXPECT: After:  0 12 17 23 37
    print_array(arr4, 5);
    printf("Sorted: %d\n", is_sorted(arr4, 5));
    // EXPECT: Sorted: 1

    printf("Test 5: Partial insertion sort\n");
    // EXPECT: Test 5: Partial insertion sort
    arr5[0] = 1; arr5[1] = 2; arr5[2] = 9; arr5[3] = 7;
    arr5[4] = 5; arr5[5] = 3; arr5[6] = 8; arr5[7] = 6;
    arr5[8] = 4; arr5[9] = 10; arr5[10] = 11; arr5[11] = 12;
    insertion_sort_partial(arr5, 2, 8);
    printf("After:  ");
    // EXPECT: After:  1 2 3 4 5 6 7 8 9 10 11 12
    print_array(arr5, 12);
    printf("Sorted: %d\n", is_sorted(arr5, 12));
    // EXPECT: Sorted: 1

    printf("Test 6: Single and two elements\n");
    // EXPECT: Test 6: Single and two elements
    arr1[0] = 99;
    insertion_sort(arr1, 1);
    printf("Single: ");
    // EXPECT: Single: 99
    print_array(arr1, 1);
    arr1[0] = 5; arr1[1] = 2;
    insertion_sort(arr1, 2);
    printf("Two:    ");
    // EXPECT: Two:    2 5
    print_array(arr1, 2);

    printf("All insertion sort tests passed\n");
    // EXPECT: All insertion sort tests passed
    return 0;
}
