int printf(const char *fmt, ...);

void merge(int *arr, int *temp, int left, int mid, int right) {
    int i;
    int j;
    int k;
    for (i = left; i <= right; i++) {
        temp[i] = arr[i];
    }
    i = left;
    j = mid + 1;
    k = left;
    while (i <= mid && j <= right) {
        if (temp[i] <= temp[j]) {
            arr[k] = temp[i];
            i = i + 1;
        } else {
            arr[k] = temp[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i <= mid) {
        arr[k] = temp[i];
        i = i + 1;
        k = k + 1;
    }
    while (j <= right) {
        arr[k] = temp[j];
        j = j + 1;
        k = k + 1;
    }
}

void merge_sort_iterative(int *arr, int n) {
    int temp[64];
    int width;
    int left;
    int mid;
    int right;
    for (width = 1; width < n; width = width * 2) {
        left = 0;
        while (left < n) {
            mid = left + width - 1;
            right = left + 2 * width - 1;
            if (mid >= n) {
                mid = n - 1;
            }
            if (right >= n) {
                right = n - 1;
            }
            if (mid < right) {
                merge(arr, temp, left, mid, right);
            }
            left = left + 2 * width;
        }
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

int main() {
    int arr1[8];
    int arr2[7];
    int arr3[10];
    int arr4[1];
    int arr5[16];
    int i;

    arr1[0] = 38; arr1[1] = 27; arr1[2] = 43; arr1[3] = 3;
    arr1[4] = 9;  arr1[5] = 82; arr1[6] = 10; arr1[7] = 1;

    printf("Test 1: Basic merge sort (power of 2 size)\n");
    // EXPECT: Test 1: Basic merge sort (power of 2 size)
    printf("Before: ");
    // EXPECT: Before: 38 27 43 3 9 82 10 1
    print_array(arr1, 8);
    merge_sort_iterative(arr1, 8);
    printf("After:  ");
    // EXPECT: After:  1 3 9 10 27 38 43 82
    print_array(arr1, 8);
    printf("Sorted: %d\n", is_sorted(arr1, 8));
    // EXPECT: Sorted: 1

    arr2[0] = 70; arr2[1] = 50; arr2[2] = 30; arr2[3] = 10;
    arr2[4] = 60; arr2[5] = 40; arr2[6] = 20;
    printf("Test 2: Odd-length array\n");
    // EXPECT: Test 2: Odd-length array
    merge_sort_iterative(arr2, 7);
    printf("After:  ");
    // EXPECT: After:  10 20 30 40 50 60 70
    print_array(arr2, 7);
    printf("Sorted: %d\n", is_sorted(arr2, 7));
    // EXPECT: Sorted: 1

    arr3[0] = -10; arr3[1] = 5; arr3[2] = -3; arr3[3] = 0; arr3[4] = 8;
    arr3[5] = -7;  arr3[6] = 2; arr3[7] = -1; arr3[8] = 9; arr3[9] = -5;
    printf("Test 3: Mixed positive and negative\n");
    // EXPECT: Test 3: Mixed positive and negative
    merge_sort_iterative(arr3, 10);
    printf("After:  ");
    // EXPECT: After:  -10 -7 -5 -3 -1 0 2 5 8 9
    print_array(arr3, 10);

    arr4[0] = 7;
    printf("Test 4: Single element\n");
    // EXPECT: Test 4: Single element
    merge_sort_iterative(arr4, 1);
    printf("After:  ");
    // EXPECT: After:  7
    print_array(arr4, 1);

    printf("Test 5: 16 elements\n");
    // EXPECT: Test 5: 16 elements
    for (i = 0; i < 16; i++) {
        arr5[i] = 16 - i;
    }
    merge_sort_iterative(arr5, 16);
    printf("After:  ");
    // EXPECT: After:  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
    print_array(arr5, 16);
    printf("Sorted: %d\n", is_sorted(arr5, 16));
    // EXPECT: Sorted: 1

    printf("Test 6: Duplicates\n");
    // EXPECT: Test 6: Duplicates
    arr1[0] = 5; arr1[1] = 3; arr1[2] = 5; arr1[3] = 1;
    arr1[4] = 3; arr1[5] = 1; arr1[6] = 5; arr1[7] = 3;
    merge_sort_iterative(arr1, 8);
    printf("After:  ");
    // EXPECT: After:  1 1 3 3 3 5 5 5
    print_array(arr1, 8);
    printf("Sorted: %d\n", is_sorted(arr1, 8));
    // EXPECT: Sorted: 1

    printf("All iterative merge sort tests passed\n");
    // EXPECT: All iterative merge sort tests passed
    return 0;
}
