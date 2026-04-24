int printf(const char *fmt, ...);
// EXPECT: Before: 64 25 12 22 11\nAfter: 11 12 22 25 64\nSorted: 1\nBefore: 5 4 3 2 1\nAfter: 1 2 3 4 5\nSorted: 1\nSingle: 1\nSorted: 1\nEqual: 3 3 3 3\nSorted: 1
// Test: Insertion sort algorithm

void insertion_sort(int arr[], int n) {
    int i, j, key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
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

int is_sorted(int arr[], int n) {
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] < arr[i - 1]) return 0;
    }
    return 1;
}

int main(void) {
    int a[] = {64, 25, 12, 22, 11};
    int n = 5;
    printf("Before: ");
    print_array(a, n);
    insertion_sort(a, n);
    printf("After: ");
    print_array(a, n);
    printf("Sorted: %d\n", is_sorted(a, n));

    int b[] = {5, 4, 3, 2, 1};
    int m = 5;
    printf("Before: ");
    print_array(b, m);
    insertion_sort(b, m);
    printf("After: ");
    print_array(b, m);
    printf("Sorted: %d\n", is_sorted(b, m));

    int c[] = {1};
    insertion_sort(c, 1);
    printf("Single: ");
    print_array(c, 1);
    printf("Sorted: %d\n", is_sorted(c, 1));

    int d[] = {3, 3, 3, 3};
    insertion_sort(d, 4);
    printf("Equal: ");
    print_array(d, 4);
    printf("Sorted: %d\n", is_sorted(d, 4));

    return 0;
}
