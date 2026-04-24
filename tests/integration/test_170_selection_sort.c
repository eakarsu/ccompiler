int printf(const char *fmt, ...);
// EXPECT: Before: 29 10 14 37 13 24 8 19 \nAfter:  8 10 13 14 19 24 29 37 \nBefore: 50 40 30 20 10 9 8 7 6 5 \nAfter:  5 6 7 8 9 10 20 30 40 50 \nComparisons: 45\nSorted comparisons: 15\nBefore: 3 1 4 1 5 9 2 \nAfter:  1 1 2 3 4 5 9 \nSort stability check:\n10 10 20 30 30 
void selection_sort(int arr[], int n) {
    int i;
    int j;
    int min_idx;
    int temp;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

int selection_sort_counted(int arr[], int n) {
    int i;
    int j;
    int min_idx;
    int temp;
    int comparisons = 0;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            comparisons++;
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
    return comparisons;
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
    arr1[0] = 29; arr1[1] = 10; arr1[2] = 14; arr1[3] = 37;
    arr1[4] = 13; arr1[5] = 24; arr1[6] = 8; arr1[7] = 19;
    printf("Before: ");
    print_arr(arr1, 8);
    selection_sort(arr1, 8);
    printf("After:  ");
    print_arr(arr1, 8);

    int arr2[10];
    arr2[0] = 50; arr2[1] = 40; arr2[2] = 30; arr2[3] = 20; arr2[4] = 10;
    arr2[5] = 9; arr2[6] = 8; arr2[7] = 7; arr2[8] = 6; arr2[9] = 5;
    printf("Before: ");
    print_arr(arr2, 10);
    int c = selection_sort_counted(arr2, 10);
    printf("After:  ");
    print_arr(arr2, 10);
    printf("Comparisons: %d\n", c);

    int arr3[6];
    arr3[0] = 1; arr3[1] = 2; arr3[2] = 3; arr3[3] = 4; arr3[4] = 5; arr3[5] = 6;
    c = selection_sort_counted(arr3, 6);
    printf("Sorted comparisons: %d\n", c);

    int arr4[7];
    arr4[0] = 3; arr4[1] = 1; arr4[2] = 4; arr4[3] = 1; arr4[4] = 5; arr4[5] = 9; arr4[6] = 2;
    printf("Before: ");
    print_arr(arr4, 7);
    selection_sort(arr4, 7);
    printf("After:  ");
    print_arr(arr4, 7);

    printf("Sort stability check:\n");
    int keys[5];
    keys[0] = 30; keys[1] = 10; keys[2] = 20; keys[3] = 10; keys[4] = 30;
    selection_sort(keys, 5);
    print_arr(keys, 5);

    return 0;
}
