int printf(const char *fmt, ...);
// EXPECT: Before: 170 45 75 90 802 24 2 66\nAfter: 2 24 45 66 75 90 170 802\nSorted: 329 355 436 457 657 720 839\nMixed: 1 5 10 50 100 500 1000
// Test: Radix sort (LSD, base 10)

void counting_sort_digit(int arr[], int n, int exp) {
    int output[64];
    int count[10];
    int i;

    for (i = 0; i < 10; i++) {
        count[i] = 0;
    }

    for (i = 0; i < n; i++) {
        int digit = (arr[i] / exp) % 10;
        count[digit]++;
    }

    for (i = 1; i < 10; i++) {
        count[i] = count[i] + count[i - 1];
    }

    for (i = n - 1; i >= 0; i--) {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    for (i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}

int find_max(int arr[], int n) {
    int max = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] > max) max = arr[i];
    }
    return max;
}

void radix_sort(int arr[], int n) {
    int mx = find_max(arr, n);
    int exp;
    for (exp = 1; mx / exp > 0; exp = exp * 10) {
        counting_sort_digit(arr, n, exp);
    }
}

void print_array(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int a[] = {170, 45, 75, 90, 802, 24, 2, 66};
    int n = 8;
    printf("Before: ");
    print_array(a, n);
    radix_sort(a, n);
    printf("After: ");
    print_array(a, n);

    int b[] = {329, 457, 657, 839, 436, 720, 355};
    int m = 7;
    radix_sort(b, m);
    printf("Sorted: ");
    print_array(b, m);

    int c[] = {1, 10, 100, 1000, 5, 50, 500};
    radix_sort(c, 7);
    printf("Mixed: ");
    print_array(c, 7);

    return 0;
}
