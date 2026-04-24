int printf(const char *fmt, ...);
// EXPECT: array: 3 1 4 1 5 9\nsum = 23\nmax = 9\nmin = 1\nscaled: 6 2 8 2 10 18\nreversed: 18 10 2 8 2 6\nsum of last 3: 16
// Test: arrays as function parameters

void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

int sum_array(int *arr, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) {
        s = s + arr[i];
    }
    return s;
}

int max_array(int *arr, int n) {
    int mx = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] > mx) mx = arr[i];
    }
    return mx;
}

int min_array(int *arr, int n) {
    int mn = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] < mn) mn = arr[i];
    }
    return mn;
}

void scale_array(int *arr, int n, int factor) {
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = arr[i] * factor;
    }
}

void reverse_array(int *arr, int n) {
    int i;
    int j = n - 1;
    for (i = 0; i < j; i++) {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
        j = j - 1;
    }
}

int main(void) {
    int data[6];
    data[0] = 3; data[1] = 1; data[2] = 4;
    data[3] = 1; data[4] = 5; data[5] = 9;

    printf("array: ");
    print_array(data, 6);

    printf("sum = %d\n", sum_array(data, 6));
    printf("max = %d\n", max_array(data, 6));
    printf("min = %d\n", min_array(data, 6));

    scale_array(data, 6, 2);
    printf("scaled: ");
    print_array(data, 6);

    reverse_array(data, 6);
    printf("reversed: ");
    print_array(data, 6);

    // Partial array pass
    printf("sum of last 3: %d\n", sum_array(data + 3, 3));

    return 0;
}
