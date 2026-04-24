int printf(const char *fmt, ...);
// EXPECT: Before: 4 2 2 8 3 3 1\nAfter: 1 2 2 3 3 4 8\nBefore: 10 5 3 7 1 9 2 8 4 6\nAfter: 1 2 3 4 5 6 7 8 9 10\nBinary: 0 0 0 1 1 1\nSame: 5 5 5 5
// Test: Counting sort algorithm

void counting_sort(int arr[], int n, int max_val) {
    int count[101];
    int output[64];
    int i;

    for (i = 0; i <= max_val; i++) {
        count[i] = 0;
    }

    for (i = 0; i < n; i++) {
        count[arr[i]]++;
    }

    for (i = 1; i <= max_val; i++) {
        count[i] = count[i] + count[i - 1];
    }

    for (i = n - 1; i >= 0; i--) {
        output[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
    }

    for (i = 0; i < n; i++) {
        arr[i] = output[i];
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

int find_max(int arr[], int n) {
    int max = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] > max) max = arr[i];
    }
    return max;
}

int main(void) {
    int a[] = {4, 2, 2, 8, 3, 3, 1};
    int n = 7;
    int mx = find_max(a, n);
    printf("Before: ");
    print_array(a, n);
    counting_sort(a, n, mx);
    printf("After: ");
    print_array(a, n);

    int b[] = {10, 5, 3, 7, 1, 9, 2, 8, 4, 6};
    int m = 10;
    mx = find_max(b, m);
    printf("Before: ");
    print_array(b, m);
    counting_sort(b, m, mx);
    printf("After: ");
    print_array(b, m);

    int c[] = {0, 0, 0, 1, 1, 1};
    counting_sort(c, 6, 1);
    printf("Binary: ");
    print_array(c, 6);

    int d[] = {5, 5, 5, 5};
    counting_sort(d, 4, 5);
    printf("Same: ");
    print_array(d, 4);

    return 0;
}
