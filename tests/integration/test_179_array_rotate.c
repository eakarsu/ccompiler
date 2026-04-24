int printf(const char *fmt, ...);
// EXPECT: Original: 1 2 3 4 5 6 7 8 \nLeft 3:   4 5 6 7 8 1 2 3 \nRight 2:  7 8 1 2 3 4 5 6 \nLeft 8:   1 2 3 4 5 6 7 8 \nReverse rotation:\nOriginal:  10 20 30 40 50 60 70 80 90 100 \nRevRot 4:  50 60 70 80 90 100 10 20 30 40 \nCyclic shifts:\nshift 0: 1 2 3 4 5 6 \nshift 1: 2 3 4 5 6 1 \nshift 2: 3 4 5 6 1 2 \nshift 3: 4 5 6 1 2 3 \nshift 4: 5 6 1 2 3 4 \nshift 5: 6 1 2 3 4 5 \nshift 6: 1 2 3 4 5 6 
void print_arr(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void rotate_left(int arr[], int n, int k) {
    int temp[20];
    int i;
    k = k % n;
    for (i = 0; i < n; i++) {
        temp[i] = arr[(i + k) % n];
    }
    for (i = 0; i < n; i++) {
        arr[i] = temp[i];
    }
}

void rotate_right(int arr[], int n, int k) {
    int temp[20];
    int i;
    k = k % n;
    for (i = 0; i < n; i++) {
        temp[(i + k) % n] = arr[i];
    }
    for (i = 0; i < n; i++) {
        arr[i] = temp[i];
    }
}

void reverse_range(int arr[], int lo, int hi) {
    int temp;
    while (lo < hi) {
        temp = arr[lo];
        arr[lo] = arr[hi];
        arr[hi] = temp;
        lo++;
        hi--;
    }
}

int main(void) {
    int arr1[8];
    int i;
    for (i = 0; i < 8; i++) arr1[i] = i + 1;

    printf("Original: ");
    print_arr(arr1, 8);

    rotate_left(arr1, 8, 3);
    printf("Left 3:   ");
    print_arr(arr1, 8);

    for (i = 0; i < 8; i++) arr1[i] = i + 1;
    rotate_right(arr1, 8, 2);
    printf("Right 2:  ");
    print_arr(arr1, 8);

    for (i = 0; i < 8; i++) arr1[i] = i + 1;
    rotate_left(arr1, 8, 8);
    printf("Left 8:   ");
    print_arr(arr1, 8);

    printf("Reverse rotation:\n");
    int arr2[10];
    for (i = 0; i < 10; i++) arr2[i] = (i + 1) * 10;
    printf("Original:  ");
    print_arr(arr2, 10);

    int k = 4;
    reverse_range(arr2, 0, k - 1);
    reverse_range(arr2, k, 9);
    reverse_range(arr2, 0, 9);
    printf("RevRot 4:  ");
    print_arr(arr2, 10);

    printf("Cyclic shifts:\n");
    int arr3[6];
    for (i = 0; i < 6; i++) arr3[i] = i + 1;
    int shift;
    for (shift = 0; shift <= 6; shift++) {
        int temp[6];
        int j;
        for (j = 0; j < 6; j++) {
            temp[j] = arr3[(j + shift) % 6];
        }
        printf("shift %d: ", shift);
        for (j = 0; j < 6; j++) printf("%d ", temp[j]);
        printf("\n");
    }

    return 0;
}
