int printf(const char *fmt, ...);
// EXPECT: before: 38 27 43 3 9 82 10 1 56 77\nafter: 1 3 9 10 27 38 43 56 77 82\nsorted: 1\nreversed: 1 2 3 4 5 6 7 8\nsorted: 1\ndupes: 1 3 3 5 5\nsingle: 42\nlarge: 7 11 12 22 25 33 45 55 64 78 90 99
// Test 212: Recursive merge sort

int temp[100];

void merge(int *arr, int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            i = i + 1;
        } else {
            temp[k] = arr[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i <= mid) {
        temp[k] = arr[i];
        i = i + 1;
        k = k + 1;
    }
    while (j <= right) {
        temp[k] = arr[j];
        j = j + 1;
        k = k + 1;
    }
    i = left;
    while (i <= right) {
        arr[i] = temp[i];
        i = i + 1;
    }
}

void merge_sort(int *arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void print_array(int *arr, int n) {
    int i = 0;
    while (i < n) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
        i = i + 1;
    }
    printf("\n");
}

int is_sorted(int *arr, int n) {
    int i = 1;
    while (i < n) {
        if (arr[i] < arr[i - 1]) return 0;
        i = i + 1;
    }
    return 1;
}

int main(void) {
    int a[10];
    a[0] = 38; a[1] = 27; a[2] = 43; a[3] = 3; a[4] = 9;
    a[5] = 82; a[6] = 10; a[7] = 1; a[8] = 56; a[9] = 77;
    printf("before: ");
    print_array(a, 10);
    merge_sort(a, 0, 9);
    printf("after: ");
    print_array(a, 10);
    printf("sorted: %d\n", is_sorted(a, 10));

    int b[8];
    b[0] = 8; b[1] = 7; b[2] = 6; b[3] = 5; b[4] = 4; b[5] = 3; b[6] = 2; b[7] = 1;
    merge_sort(b, 0, 7);
    printf("reversed: ");
    print_array(b, 8);
    printf("sorted: %d\n", is_sorted(b, 8));

    int c[5];
    c[0] = 5; c[1] = 5; c[2] = 3; c[3] = 3; c[4] = 1;
    merge_sort(c, 0, 4);
    printf("dupes: ");
    print_array(c, 5);

    int d[1];
    d[0] = 42;
    merge_sort(d, 0, 0);
    printf("single: ");
    print_array(d, 1);

    int e[12];
    e[0] = 64; e[1] = 25; e[2] = 12; e[3] = 22; e[4] = 11;
    e[5] = 90; e[6] = 45; e[7] = 78; e[8] = 33; e[9] = 55;
    e[10] = 7; e[11] = 99;
    merge_sort(e, 0, 11);
    printf("large: ");
    print_array(e, 12);

    return 0;
}
