int printf(const char *fmt, ...);

void selection_sort(int *arr, int n) {
    int i;
    int j;
    int min_idx;
    int tmp;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
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

int find_min(int *arr, int n) {
    int i;
    int min_val = arr[0];
    for (i = 1; i < n; i++) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }
    return min_val;
}

int find_max(int *arr, int n) {
    int i;
    int max_val = arr[0];
    for (i = 1; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    return max_val;
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

int count_swaps(int *arr, int n) {
    int copy[20];
    int i;
    int j;
    int min_idx;
    int tmp;
    int swaps = 0;
    for (i = 0; i < n; i++) {
        copy[i] = arr[i];
    }
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (copy[j] < copy[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            tmp = copy[i];
            copy[i] = copy[min_idx];
            copy[min_idx] = tmp;
            swaps = swaps + 1;
        }
    }
    return swaps;
}

int main() {
    int arr1[8];
    int arr2[6];
    int arr3[5];
    int arr4[10];
    int swaps;

    arr1[0] = 29; arr1[1] = 10; arr1[2] = 14; arr1[3] = 37;
    arr1[4] = 13; arr1[5] = 8;  arr1[6] = 25; arr1[7] = 3;

    printf("Test 1: Basic selection sort\n");
    // EXPECT: Test 1: Basic selection sort
    printf("Before: ");
    // EXPECT: Before: 29 10 14 37 13 8 25 3
    print_array(arr1, 8);
    swaps = count_swaps(arr1, 8);
    printf("Swaps needed: %d\n", swaps);
    // EXPECT: Swaps needed: 7
    selection_sort(arr1, 8);
    printf("After:  ");
    // EXPECT: After:  3 8 10 13 14 25 29 37
    print_array(arr1, 8);
    printf("Sorted: %d\n", is_sorted(arr1, 8));
    // EXPECT: Sorted: 1

    arr2[0] = 100; arr2[1] = 50; arr2[2] = 75; arr2[3] = 25; arr2[4] = 0; arr2[5] = -10;
    printf("Test 2: With negative and zero\n");
    // EXPECT: Test 2: With negative and zero
    printf("Min: %d Max: %d\n", find_min(arr2, 6), find_max(arr2, 6));
    // EXPECT: Min: -10 Max: 100
    selection_sort(arr2, 6);
    printf("After:  ");
    // EXPECT: After:  -10 0 25 50 75 100
    print_array(arr2, 6);

    arr3[0] = 1; arr3[1] = 2; arr3[2] = 3; arr3[3] = 4; arr3[4] = 5;
    printf("Test 3: Already sorted\n");
    // EXPECT: Test 3: Already sorted
    swaps = count_swaps(arr3, 5);
    printf("Swaps needed: %d\n", swaps);
    // EXPECT: Swaps needed: 0
    selection_sort(arr3, 5);
    printf("After:  ");
    // EXPECT: After:  1 2 3 4 5
    print_array(arr3, 5);

    arr4[0] = 9; arr4[1] = 8; arr4[2] = 7; arr4[3] = 6; arr4[4] = 5;
    arr4[5] = 4; arr4[6] = 3; arr4[7] = 2; arr4[8] = 1; arr4[9] = 0;
    printf("Test 4: Reverse sorted\n");
    // EXPECT: Test 4: Reverse sorted
    selection_sort(arr4, 10);
    printf("After:  ");
    // EXPECT: After:  0 1 2 3 4 5 6 7 8 9
    print_array(arr4, 10);
    printf("Sorted: %d\n", is_sorted(arr4, 10));
    // EXPECT: Sorted: 1

    arr3[0] = 42; arr3[1] = 42; arr3[2] = 42; arr3[3] = 42; arr3[4] = 42;
    printf("Test 5: All same values\n");
    // EXPECT: Test 5: All same values
    selection_sort(arr3, 5);
    printf("After:  ");
    // EXPECT: After:  42 42 42 42 42
    print_array(arr3, 5);
    printf("Sorted: %d\n", is_sorted(arr3, 5));
    // EXPECT: Sorted: 1

    printf("All selection sort tests passed\n");
    // EXPECT: All selection sort tests passed
    return 0;
}
