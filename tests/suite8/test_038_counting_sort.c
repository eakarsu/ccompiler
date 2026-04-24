int printf(const char *fmt, ...);

void counting_sort(int *arr, int n, int max_val) {
    int count[101];
    int output[64];
    int i;
    int j;
    for (i = 0; i <= max_val; i++) {
        count[i] = 0;
    }
    for (i = 0; i < n; i++) {
        count[arr[i]] = count[arr[i]] + 1;
    }
    for (i = 1; i <= max_val; i++) {
        count[i] = count[i] + count[i - 1];
    }
    for (i = n - 1; i >= 0; i--) {
        output[count[arr[i]] - 1] = arr[i];
        count[arr[i]] = count[arr[i]] - 1;
    }
    for (i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}

void counting_sort_with_offset(int *arr, int n, int min_val, int max_val) {
    int range;
    int count[201];
    int output[64];
    int i;
    range = max_val - min_val + 1;
    for (i = 0; i < range; i++) {
        count[i] = 0;
    }
    for (i = 0; i < n; i++) {
        count[arr[i] - min_val] = count[arr[i] - min_val] + 1;
    }
    for (i = 1; i < range; i++) {
        count[i] = count[i] + count[i - 1];
    }
    for (i = n - 1; i >= 0; i--) {
        output[count[arr[i] - min_val] - 1] = arr[i];
        count[arr[i] - min_val] = count[arr[i] - min_val] - 1;
    }
    for (i = 0; i < n; i++) {
        arr[i] = output[i];
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

int find_max(int *arr, int n) {
    int i;
    int m;
    m = arr[0];
    for (i = 1; i < n; i++) {
        if (arr[i] > m) {
            m = arr[i];
        }
    }
    return m;
}

int find_min(int *arr, int n) {
    int i;
    int m;
    m = arr[0];
    for (i = 1; i < n; i++) {
        if (arr[i] < m) {
            m = arr[i];
        }
    }
    return m;
}

int main() {
    int arr1[10];
    int arr2[8];
    int arr3[12];
    int arr4[6];
    int arr5[15];
    int i;

    arr1[0] = 4; arr1[1] = 2; arr1[2] = 2; arr1[3] = 8; arr1[4] = 3;
    arr1[5] = 3; arr1[6] = 1; arr1[7] = 7; arr1[8] = 5; arr1[9] = 6;

    printf("Test 1: Basic counting sort\n");
    // EXPECT: Test 1: Basic counting sort
    printf("Before: ");
    // EXPECT: Before: 4 2 2 8 3 3 1 7 5 6
    print_array(arr1, 10);
    counting_sort(arr1, 10, find_max(arr1, 10));
    printf("After:  ");
    // EXPECT: After:  1 2 2 3 3 4 5 6 7 8
    print_array(arr1, 10);
    printf("Sorted: %d\n", is_sorted(arr1, 10));
    // EXPECT: Sorted: 1

    arr2[0] = 0; arr2[1] = 5; arr2[2] = 3; arr2[3] = 0;
    arr2[4] = 5; arr2[5] = 3; arr2[6] = 0; arr2[7] = 5;
    printf("Test 2: Many duplicates\n");
    // EXPECT: Test 2: Many duplicates
    counting_sort(arr2, 8, 5);
    printf("After:  ");
    // EXPECT: After:  0 0 0 3 3 5 5 5
    print_array(arr2, 8);

    printf("Test 3: With negative numbers (offset method)\n");
    // EXPECT: Test 3: With negative numbers (offset method)
    arr3[0] = -5; arr3[1] = 3; arr3[2] = -2; arr3[3] = 0;
    arr3[4] = 7;  arr3[5] = -3; arr3[6] = 1; arr3[7] = -1;
    arr3[8] = 4;  arr3[9] = -4; arr3[10] = 2; arr3[11] = 5;
    counting_sort_with_offset(arr3, 12, find_min(arr3, 12), find_max(arr3, 12));
    printf("After:  ");
    // EXPECT: After:  -5 -4 -3 -2 -1 0 1 2 3 4 5 7
    print_array(arr3, 12);
    printf("Sorted: %d\n", is_sorted(arr3, 12));
    // EXPECT: Sorted: 1

    printf("Test 4: All same values\n");
    // EXPECT: Test 4: All same values
    for (i = 0; i < 6; i++) {
        arr4[i] = 3;
    }
    counting_sort(arr4, 6, 3);
    printf("After:  ");
    // EXPECT: After:  3 3 3 3 3 3
    print_array(arr4, 6);

    printf("Test 5: Large range of values\n");
    // EXPECT: Test 5: Large range of values
    arr5[0] = 50; arr5[1] = 10; arr5[2] = 90; arr5[3] = 30; arr5[4] = 70;
    arr5[5] = 20; arr5[6] = 80; arr5[7] = 40; arr5[8] = 60; arr5[9] = 100;
    arr5[10] = 5; arr5[11] = 15; arr5[12] = 25; arr5[13] = 35; arr5[14] = 45;
    counting_sort(arr5, 15, 100);
    printf("After:  ");
    // EXPECT: After:  5 10 15 20 25 30 35 40 45 50 60 70 80 90 100
    print_array(arr5, 15);
    printf("Sorted: %d\n", is_sorted(arr5, 15));
    // EXPECT: Sorted: 1

    printf("Test 6: Single element\n");
    // EXPECT: Test 6: Single element
    arr1[0] = 7;
    counting_sort(arr1, 1, 7);
    printf("After:  ");
    // EXPECT: After:  7
    print_array(arr1, 1);

    printf("All counting sort tests passed\n");
    // EXPECT: All counting sort tests passed
    return 0;
}
