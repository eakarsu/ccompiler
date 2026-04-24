int printf(const char *fmt, ...);
// EXPECT: Before: 64 34 25 12 22 11 90 1 \nAfter:  1 11 12 22 25 34 64 90 \nBefore: 5 3 8 1 9 2 7 4 6 10 \nAfter:  1 2 3 4 5 6 7 8 9 10 \nSwaps: 17\nAlready sorted swaps: 0\nBefore: 6 5 4 3 2 1 \nAfter:  1 2 3 4 5 6 \nReverse sorted swaps: 15\nAll same swaps: 0
void bubble_sort(int arr[], int n) {
    int i;
    int j;
    int temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int bubble_sort_counted(int arr[], int n) {
    int i;
    int j;
    int temp;
    int swaps = 0;
    for (i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swaps++;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
    return swaps;
}

void print_arr(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int arr1[8];
    arr1[0] = 64; arr1[1] = 34; arr1[2] = 25; arr1[3] = 12;
    arr1[4] = 22; arr1[5] = 11; arr1[6] = 90; arr1[7] = 1;
    printf("Before: ");
    print_arr(arr1, 8);
    bubble_sort(arr1, 8);
    printf("After:  ");
    print_arr(arr1, 8);

    int arr2[10];
    arr2[0] = 5; arr2[1] = 3; arr2[2] = 8; arr2[3] = 1; arr2[4] = 9;
    arr2[5] = 2; arr2[6] = 7; arr2[7] = 4; arr2[8] = 6; arr2[9] = 10;
    printf("Before: ");
    print_arr(arr2, 10);
    int s = bubble_sort_counted(arr2, 10);
    printf("After:  ");
    print_arr(arr2, 10);
    printf("Swaps: %d\n", s);

    int arr3[6];
    arr3[0] = 1; arr3[1] = 2; arr3[2] = 3; arr3[3] = 4; arr3[4] = 5; arr3[5] = 6;
    s = bubble_sort_counted(arr3, 6);
    printf("Already sorted swaps: %d\n", s);

    int arr4[6];
    arr4[0] = 6; arr4[1] = 5; arr4[2] = 4; arr4[3] = 3; arr4[4] = 2; arr4[5] = 1;
    printf("Before: ");
    print_arr(arr4, 6);
    s = bubble_sort_counted(arr4, 6);
    printf("After:  ");
    print_arr(arr4, 6);
    printf("Reverse sorted swaps: %d\n", s);

    int arr5[5];
    arr5[0] = 42; arr5[1] = 42; arr5[2] = 42; arr5[3] = 42; arr5[4] = 42;
    s = bubble_sort_counted(arr5, 5);
    printf("All same swaps: %d\n", s);

    return 0;
}
