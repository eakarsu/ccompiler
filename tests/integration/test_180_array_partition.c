int printf(const char *fmt, ...);
// EXPECT: Before partition: 7 2 1 6 8 5 3 4 9 0 \nAfter partition (pivot at 0): 0 2 1 6 8 5 3 4 9 7 \nLeft of pivot: \nPivot: 0\nRight of pivot: 2 1 6 8 5 3 4 9 7 \nBefore even/odd: 1 4 7 2 5 8 3 6 \nAfter even/odd:  6 4 8 2 5 7 3 1 \nThree-way partition:\nBefore: 2 0 1 2 0 1 0 2 1 0 2 1 \nAfter:  0 0 0 0 1 1 1 1 2 2 2 2 \nNegative/positive partition:\nBefore: -3 5 -1 7 -4 2 -8 6 \nAfter:  -3 -8 -1 -4 7 2 5 6 
void print_arr(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int partition(int arr[], int lo, int hi) {
    int pivot = arr[hi];
    int i = lo - 1;
    int j;
    int temp;
    for (j = lo; j < hi; j++) {
        if (arr[j] <= pivot) {
            i++;
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    temp = arr[i + 1];
    arr[i + 1] = arr[hi];
    arr[hi] = temp;
    return i + 1;
}

void separate_even_odd(int arr[], int n) {
    int i = 0;
    int j = n - 1;
    int temp;
    while (i < j) {
        while (i < j && arr[i] % 2 == 0) i++;
        while (i < j && arr[j] % 2 != 0) j--;
        if (i < j) {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
    }
}

int main(void) {
    int arr1[10];
    arr1[0] = 7; arr1[1] = 2; arr1[2] = 1; arr1[3] = 6; arr1[4] = 8;
    arr1[5] = 5; arr1[6] = 3; arr1[7] = 4; arr1[8] = 9; arr1[9] = 0;

    printf("Before partition: ");
    print_arr(arr1, 10);

    int p = partition(arr1, 0, 9);
    printf("After partition (pivot at %d): ", p);
    print_arr(arr1, 10);

    printf("Left of pivot: ");
    int i;
    for (i = 0; i < p; i++) printf("%d ", arr1[i]);
    printf("\nPivot: %d\n", arr1[p]);
    printf("Right of pivot: ");
    for (i = p + 1; i < 10; i++) printf("%d ", arr1[i]);
    printf("\n");

    int arr2[8];
    arr2[0] = 1; arr2[1] = 4; arr2[2] = 7; arr2[3] = 2;
    arr2[4] = 5; arr2[5] = 8; arr2[6] = 3; arr2[7] = 6;

    printf("Before even/odd: ");
    print_arr(arr2, 8);
    separate_even_odd(arr2, 8);
    printf("After even/odd:  ");
    print_arr(arr2, 8);

    printf("Three-way partition:\n");
    int arr3[12];
    arr3[0] = 2; arr3[1] = 0; arr3[2] = 1; arr3[3] = 2;
    arr3[4] = 0; arr3[5] = 1; arr3[6] = 0; arr3[7] = 2;
    arr3[8] = 1; arr3[9] = 0; arr3[10] = 2; arr3[11] = 1;
    printf("Before: ");
    print_arr(arr3, 12);
    int lo = 0;
    int mid = 0;
    int hi = 11;
    int temp;
    while (mid <= hi) {
        if (arr3[mid] == 0) {
            temp = arr3[lo]; arr3[lo] = arr3[mid]; arr3[mid] = temp;
            lo++;
            mid++;
        } else if (arr3[mid] == 1) {
            mid++;
        } else {
            temp = arr3[mid]; arr3[mid] = arr3[hi]; arr3[hi] = temp;
            hi--;
        }
    }
    printf("After:  ");
    print_arr(arr3, 12);

    printf("Negative/positive partition:\n");
    int arr4[8];
    arr4[0] = -3; arr4[1] = 5; arr4[2] = -1; arr4[3] = 7;
    arr4[4] = -4; arr4[5] = 2; arr4[6] = -8; arr4[7] = 6;
    printf("Before: ");
    print_arr(arr4, 8);
    i = 0;
    int j = 7;
    while (i < j) {
        while (i < j && arr4[i] < 0) i++;
        while (i < j && arr4[j] >= 0) j--;
        if (i < j) {
            temp = arr4[i]; arr4[i] = arr4[j]; arr4[j] = temp;
            i++; j--;
        }
    }
    printf("After:  ");
    print_arr(arr4, 8);

    return 0;
}
