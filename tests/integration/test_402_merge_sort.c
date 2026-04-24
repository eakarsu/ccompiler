int printf(const char *fmt, ...);
// EXPECT: Before: 38 27 43 3 9 82 10\nAfter: 3 9 10 27 38 43 82\nReversed: 1 2 3 4 5 6 7 8 9 10\nAlready: 1 2 3 4 5
// Test: Bottom-up iterative merge sort

void merge(int arr[], int temp[], int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            i++;
        } else {
            temp[k] = arr[j];
            j++;
        }
        k++;
    }
    while (i <= mid) {
        temp[k] = arr[i];
        i++;
        k++;
    }
    while (j <= right) {
        temp[k] = arr[j];
        j++;
        k++;
    }
    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

void merge_sort(int arr[], int n) {
    int temp[64];
    int width, i;
    for (width = 1; width < n; width = width * 2) {
        for (i = 0; i < n; i = i + 2 * width) {
            int left = i;
            int mid = i + width - 1;
            int right = i + 2 * width - 1;
            if (mid >= n) mid = n - 1;
            if (right >= n) right = n - 1;
            if (mid < right) {
                merge(arr, temp, left, mid, right);
            }
        }
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
    int a[] = {38, 27, 43, 3, 9, 82, 10};
    int n = 7;
    printf("Before: ");
    print_array(a, n);
    merge_sort(a, n);
    printf("After: ");
    print_array(a, n);

    int b[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int m = 10;
    merge_sort(b, m);
    printf("Reversed: ");
    print_array(b, m);

    int c[] = {1, 2, 3, 4, 5};
    merge_sort(c, 5);
    printf("Already: ");
    print_array(c, 5);

    return 0;
}
