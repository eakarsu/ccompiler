int printf(const char *fmt, ...);

int get_max(int *arr, int n) {
    int i;
    int mx;
    mx = arr[0];
    for (i = 1; i < n; i++) {
        if (arr[i] > mx) {
            mx = arr[i];
        }
    }
    return mx;
}

void counting_sort_by_digit(int *arr, int n, int exp) {
    int output[64];
    int count[10];
    int i;
    int digit;
    for (i = 0; i < 10; i++) {
        count[i] = 0;
    }
    for (i = 0; i < n; i++) {
        digit = (arr[i] / exp) % 10;
        count[digit] = count[digit] + 1;
    }
    for (i = 1; i < 10; i++) {
        count[i] = count[i] + count[i - 1];
    }
    for (i = n - 1; i >= 0; i--) {
        digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit] = count[digit] - 1;
    }
    for (i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}

void radix_sort(int *arr, int n) {
    int mx;
    int exp;
    if (n <= 1) {
        return;
    }
    mx = get_max(arr, n);
    exp = 1;
    while (mx / exp > 0) {
        counting_sort_by_digit(arr, n, exp);
        exp = exp * 10;
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

int num_digits(int val) {
    int d;
    if (val == 0) {
        return 1;
    }
    d = 0;
    while (val > 0) {
        d = d + 1;
        val = val / 10;
    }
    return d;
}

int main() {
    int arr1[10];
    int arr2[8];
    int arr3[6];
    int arr4[12];
    int arr5[5];
    int i;

    arr1[0] = 170; arr1[1] = 45;  arr1[2] = 75;  arr1[3] = 90;  arr1[4] = 802;
    arr1[5] = 24;  arr1[6] = 2;   arr1[7] = 66;  arr1[8] = 345; arr1[9] = 111;

    printf("Test 1: Basic radix sort (base 10)\n");
    // EXPECT: Test 1: Basic radix sort (base 10)
    printf("Before: ");
    // EXPECT: Before: 170 45 75 90 802 24 2 66 345 111
    print_array(arr1, 10);
    printf("Max value: %d (digits: %d)\n", get_max(arr1, 10), num_digits(get_max(arr1, 10)));
    // EXPECT: Max value: 802 (digits: 3)
    radix_sort(arr1, 10);
    printf("After:  ");
    // EXPECT: After:  2 24 45 66 75 90 111 170 345 802
    print_array(arr1, 10);
    printf("Sorted: %d\n", is_sorted(arr1, 10));
    // EXPECT: Sorted: 1

    arr2[0] = 9; arr2[1] = 1; arr2[2] = 5; arr2[3] = 3;
    arr2[4] = 7; arr2[5] = 2; arr2[6] = 8; arr2[7] = 4;
    printf("Test 2: Single digit numbers\n");
    // EXPECT: Test 2: Single digit numbers
    radix_sort(arr2, 8);
    printf("After:  ");
    // EXPECT: After:  1 2 3 4 5 7 8 9
    print_array(arr2, 8);

    arr3[0] = 1000; arr3[1] = 999; arr3[2] = 500;
    arr3[3] = 100;  arr3[4] = 50;  arr3[5] = 1;
    printf("Test 3: Various digit lengths\n");
    // EXPECT: Test 3: Various digit lengths
    radix_sort(arr3, 6);
    printf("After:  ");
    // EXPECT: After:  1 50 100 500 999 1000
    print_array(arr3, 6);
    printf("Sorted: %d\n", is_sorted(arr3, 6));
    // EXPECT: Sorted: 1

    printf("Test 4: Already sorted\n");
    // EXPECT: Test 4: Already sorted
    arr4[0] = 10; arr4[1] = 20; arr4[2] = 30; arr4[3] = 40;
    arr4[4] = 50; arr4[5] = 60; arr4[6] = 70; arr4[7] = 80;
    arr4[8] = 90; arr4[9] = 100; arr4[10] = 110; arr4[11] = 120;
    radix_sort(arr4, 12);
    printf("After:  ");
    // EXPECT: After:  10 20 30 40 50 60 70 80 90 100 110 120
    print_array(arr4, 12);

    printf("Test 5: Duplicates\n");
    // EXPECT: Test 5: Duplicates
    arr5[0] = 55; arr5[1] = 11; arr5[2] = 55; arr5[3] = 11; arr5[4] = 33;
    radix_sort(arr5, 5);
    printf("After:  ");
    // EXPECT: After:  11 11 33 55 55
    print_array(arr5, 5);

    printf("Test 6: Large numbers\n");
    // EXPECT: Test 6: Large numbers
    arr1[0] = 54321; arr1[1] = 12345; arr1[2] = 99999; arr1[3] = 10000;
    arr1[4] = 55555; arr1[5] = 11111; arr1[6] = 77777; arr1[7] = 33333;
    radix_sort(arr1, 8);
    printf("After:  ");
    // EXPECT: After:  10000 11111 12345 33333 54321 55555 77777 99999
    print_array(arr1, 8);
    printf("Sorted: %d\n", is_sorted(arr1, 8));
    // EXPECT: Sorted: 1

    printf("Test 7: Digit counting\n");
    // EXPECT: Test 7: Digit counting
    printf("Digits in 0: %d\n", num_digits(0));
    // EXPECT: Digits in 0: 1
    printf("Digits in 5: %d\n", num_digits(5));
    // EXPECT: Digits in 5: 1
    printf("Digits in 99: %d\n", num_digits(99));
    // EXPECT: Digits in 99: 2
    printf("Digits in 1000: %d\n", num_digits(1000));
    // EXPECT: Digits in 1000: 4

    printf("All radix sort tests passed\n");
    // EXPECT: All radix sort tests passed
    return 0;
}
