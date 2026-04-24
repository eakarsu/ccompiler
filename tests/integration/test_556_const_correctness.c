int printf(const char *fmt, ...);
// EXPECT: Array: 15 42 8 93 27 61 4 38\nSum: 288\nMax: 93\nMin: 4\nAbove 30: 4\nAbove 50: 2\nDot product: 70\nCopy: 15 42 8 93 27 61 4 38\nFactor: 10\nScaled sum: 2880

int sum_array(const int *arr, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i++) {
        total = total + arr[i];
    }
    return total;
}

int find_max(const int *arr, int n) {
    int max_val = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] > max_val) max_val = arr[i];
    }
    return max_val;
}

int find_min(const int *arr, int n) {
    int min_val = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] < min_val) min_val = arr[i];
    }
    return min_val;
}

int count_above(const int *arr, int n, int threshold) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] > threshold) count++;
    }
    return count;
}

void print_array(const int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

int dot_product(const int *a, const int *b, int n) {
    int result = 0;
    int i;
    for (i = 0; i < n; i++) {
        result = result + a[i] * b[i];
    }
    return result;
}

void copy_array(int *dest, const int *src, int n) {
    int i;
    for (i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

int main(void) {
    const int size = 8;
    int data[8];
    data[0] = 15; data[1] = 42; data[2] = 8; data[3] = 93;
    data[4] = 27; data[5] = 61; data[6] = 4; data[7] = 38;

    printf("Array: ");
    print_array(data, size);

    printf("Sum: %d\n", sum_array(data, size));
    printf("Max: %d\n", find_max(data, size));
    printf("Min: %d\n", find_min(data, size));
    printf("Above 30: %d\n", count_above(data, size, 30));
    printf("Above 50: %d\n", count_above(data, size, 50));

    int vec_a[4];
    vec_a[0] = 1; vec_a[1] = 2; vec_a[2] = 3; vec_a[3] = 4;
    int vec_b[4];
    vec_b[0] = 5; vec_b[1] = 6; vec_b[2] = 7; vec_b[3] = 8;
    printf("Dot product: %d\n", dot_product(vec_a, vec_b, 4));

    int copied[8];
    copy_array(copied, data, size);
    printf("Copy: ");
    print_array(copied, size);

    const int factor = 10;
    printf("Factor: %d\n", factor);
    printf("Scaled sum: %d\n", sum_array(data, size) * factor);

    return 0;
}
