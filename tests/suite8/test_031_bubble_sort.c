int printf(const char *fmt, ...);

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void bubble_sort(int *arr, int n) {
    int i;
    int j;
    int swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
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
    int arr2[5];
    int arr3[8];
    int arr4[1];
    int arr5[6];

    arr1[0] = 64; arr1[1] = 34; arr1[2] = 25; arr1[3] = 12; arr1[4] = 22;
    arr1[5] = 11; arr1[6] = 90; arr1[7] = 1;  arr1[8] = 55; arr1[9] = 43;

    printf("Test 1: Basic bubble sort\n");
    // EXPECT: Test 1: Basic bubble sort
    printf("Before: ");
    // EXPECT: Before: 64 34 25 12 22 11 90 1 55 43
    print_array(arr1, 10);
    bubble_sort(arr1, 10);
    printf("After:  ");
    // EXPECT: After:  1 11 12 22 25 34 43 55 64 90
    print_array(arr1, 10);
    printf("Sorted: %d\n", is_sorted(arr1, 10));
    // EXPECT: Sorted: 1

    arr2[0] = 5; arr2[1] = 4; arr2[2] = 3; arr2[3] = 2; arr2[4] = 1;
    printf("Test 2: Reverse sorted input\n");
    // EXPECT: Test 2: Reverse sorted input
    bubble_sort(arr2, 5);
    printf("After:  ");
    // EXPECT: After:  1 2 3 4 5
    print_array(arr2, 5);

    arr3[0] = 1; arr3[1] = 2; arr3[2] = 3; arr3[3] = 4;
    arr3[4] = 5; arr3[5] = 6; arr3[6] = 7; arr3[7] = 8;
    printf("Test 3: Already sorted\n");
    // EXPECT: Test 3: Already sorted
    bubble_sort(arr3, 8);
    printf("After:  ");
    // EXPECT: After:  1 2 3 4 5 6 7 8
    print_array(arr3, 8);
    printf("Sorted: %d\n", is_sorted(arr3, 8));
    // EXPECT: Sorted: 1

    arr4[0] = 42;
    printf("Test 4: Single element\n");
    // EXPECT: Test 4: Single element
    bubble_sort(arr4, 1);
    printf("After:  ");
    // EXPECT: After:  42
    print_array(arr4, 1);

    arr5[0] = 3; arr5[1] = 3; arr5[2] = 1; arr5[3] = 1; arr5[4] = 2; arr5[5] = 2;
    printf("Test 5: Duplicates\n");
    // EXPECT: Test 5: Duplicates
    bubble_sort(arr5, 6);
    printf("After:  ");
    // EXPECT: After:  1 1 2 2 3 3
    print_array(arr5, 6);
    printf("Sorted: %d\n", is_sorted(arr5, 6));
    // EXPECT: Sorted: 1

    printf("Test 6: Sort negative numbers\n");
    // EXPECT: Test 6: Sort negative numbers
    arr1[0] = -5; arr1[1] = 3; arr1[2] = -1; arr1[3] = 0; arr1[4] = -9;
    arr1[5] = 7; arr1[6] = -3; arr1[7] = 2; arr1[8] = -8; arr1[9] = 4;
    bubble_sort(arr1, 10);
    printf("After:  ");
    // EXPECT: After:  -9 -8 -5 -3 -1 0 2 3 4 7
    print_array(arr1, 10);
    printf("Sorted: %d\n", is_sorted(arr1, 10));
    // EXPECT: Sorted: 1

    printf("All bubble sort tests passed\n");
    // EXPECT: All bubble sort tests passed
    return 0;
}
