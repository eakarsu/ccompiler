int printf(const char *fmt, ...);

int partition(int *arr, int low, int high) {
    int pivot;
    int i;
    int j;
    int tmp;
    pivot = arr[high];
    i = low - 1;
    for (j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i = i + 1;
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    tmp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = tmp;
    return i + 1;
}

void quicksort_iterative(int *arr, int n) {
    int stack[64];
    int top;
    int low;
    int high;
    int p;
    if (n <= 1) {
        return;
    }
    top = -1;
    top = top + 1;
    stack[top] = 0;
    top = top + 1;
    stack[top] = n - 1;
    while (top >= 0) {
        high = stack[top];
        top = top - 1;
        low = stack[top];
        top = top - 1;
        p = partition(arr, low, high);
        if (p - 1 > low) {
            top = top + 1;
            stack[top] = low;
            top = top + 1;
            stack[top] = p - 1;
        }
        if (p + 1 < high) {
            top = top + 1;
            stack[top] = p + 1;
            top = top + 1;
            stack[top] = high;
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
    int arr1[10];
    int arr2[8];
    int arr3[5];
    int arr4[12];
    int arr5[6];
    int i;

    arr1[0] = 10; arr1[1] = 7; arr1[2] = 8; arr1[3] = 9; arr1[4] = 1;
    arr1[5] = 5;  arr1[6] = 3; arr1[7] = 6; arr1[8] = 2; arr1[9] = 4;

    printf("Test 1: Basic quicksort\n");
    // EXPECT: Test 1: Basic quicksort
    printf("Before: ");
    // EXPECT: Before: 10 7 8 9 1 5 3 6 2 4
    print_array(arr1, 10);
    quicksort_iterative(arr1, 10);
    printf("After:  ");
    // EXPECT: After:  1 2 3 4 5 6 7 8 9 10
    print_array(arr1, 10);
    printf("Sorted: %d\n", is_sorted(arr1, 10));
    // EXPECT: Sorted: 1

    arr2[0] = 100; arr2[1] = 90; arr2[2] = 80; arr2[3] = 70;
    arr2[4] = 60;  arr2[5] = 50; arr2[6] = 40; arr2[7] = 30;
    printf("Test 2: Reverse order\n");
    // EXPECT: Test 2: Reverse order
    quicksort_iterative(arr2, 8);
    printf("After:  ");
    // EXPECT: After:  30 40 50 60 70 80 90 100
    print_array(arr2, 8);

    arr3[0] = 1; arr3[1] = 2; arr3[2] = 3; arr3[3] = 4; arr3[4] = 5;
    printf("Test 3: Already sorted\n");
    // EXPECT: Test 3: Already sorted
    quicksort_iterative(arr3, 5);
    printf("After:  ");
    // EXPECT: After:  1 2 3 4 5
    print_array(arr3, 5);

    printf("Test 4: With negatives\n");
    // EXPECT: Test 4: With negatives
    arr4[0] = -5; arr4[1] = 12; arr4[2] = -3; arr4[3] = 0;
    arr4[4] = 7;  arr4[5] = -8; arr4[6] = 15; arr4[7] = -1;
    arr4[8] = 4;  arr4[9] = -6; arr4[10] = 9; arr4[11] = 2;
    quicksort_iterative(arr4, 12);
    printf("After:  ");
    // EXPECT: After:  -8 -6 -5 -3 -1 0 2 4 7 9 12 15
    print_array(arr4, 12);
    printf("Sorted: %d\n", is_sorted(arr4, 12));
    // EXPECT: Sorted: 1

    printf("Test 5: Duplicates\n");
    // EXPECT: Test 5: Duplicates
    arr5[0] = 3; arr5[1] = 1; arr5[2] = 3; arr5[3] = 1; arr5[4] = 2; arr5[5] = 2;
    quicksort_iterative(arr5, 6);
    printf("After:  ");
    // EXPECT: After:  1 1 2 2 3 3
    print_array(arr5, 6);

    printf("Test 6: Two elements\n");
    // EXPECT: Test 6: Two elements
    arr1[0] = 20; arr1[1] = 10;
    quicksort_iterative(arr1, 2);
    printf("After:  ");
    // EXPECT: After:  10 20
    print_array(arr1, 2);

    printf("Test 7: All same\n");
    // EXPECT: Test 7: All same
    for (i = 0; i < 5; i++) {
        arr3[i] = 7;
    }
    quicksort_iterative(arr3, 5);
    printf("After:  ");
    // EXPECT: After:  7 7 7 7 7
    print_array(arr3, 5);

    printf("All iterative quicksort tests passed\n");
    // EXPECT: All iterative quicksort tests passed
    return 0;
}
