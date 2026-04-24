int printf(const char *fmt, ...);

void swap(int arr[], int i, int j) {
    int tmp;
    tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

void sift_down(int arr[], int n, int i) {
    int largest;
    int left;
    int right;
    largest = i;
    left = 2 * i + 1;
    right = 2 * i + 2;
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }
    if (largest != i) {
        swap(arr, i, largest);
        sift_down(arr, n, largest);
    }
}

void build_max_heap(int arr[], int n) {
    int i;
    i = n / 2 - 1;
    while (i >= 0) {
        sift_down(arr, n, i);
        i = i - 1;
    }
}

int is_max_heap(int arr[], int n) {
    int i;
    int left;
    int right;
    i = 0;
    while (i < n / 2) {
        left = 2 * i + 1;
        right = 2 * i + 2;
        if (left < n && arr[left] > arr[i]) {
            return 0;
        }
        if (right < n && arr[right] > arr[i]) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

void heap_sort(int arr[], int n) {
    int i;
    build_max_heap(arr, n);
    i = n - 1;
    while (i > 0) {
        swap(arr, 0, i);
        sift_down(arr, i, 0);
        i = i - 1;
    }
}

int is_sorted(int arr[], int n) {
    int i;
    i = 0;
    while (i < n - 1) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

void print_array(int arr[], int n) {
    int i;
    i = 0;
    while (i < n) {
        printf("%d ", arr[i]);
        i = i + 1;
    }
}

int main(void) {
    int a[10];
    int b[8];
    int c[5];
    int d[6];

    a[0] = 12; a[1] = 11; a[2] = 13; a[3] = 5; a[4] = 6;
    a[5] = 7; a[6] = 15; a[7] = 1; a[8] = 9; a[9] = 3;

    build_max_heap(a, 10);
    // EXPECT: heap_ok: 1
    printf("heap_ok: %d\n", is_max_heap(a, 10));
    // EXPECT: root: 15
    printf("root: %d\n", a[0]);

    a[0] = 12; a[1] = 11; a[2] = 13; a[3] = 5; a[4] = 6;
    a[5] = 7; a[6] = 15; a[7] = 1; a[8] = 9; a[9] = 3;
    heap_sort(a, 10);
    // EXPECT: hs10: 1
    printf("hs10: %d\n", is_sorted(a, 10));
    // EXPECT: 1 3 5 6 7 9 11 12 13 15 done
    print_array(a, 10);
    printf("done\n");

    b[0] = 8; b[1] = 7; b[2] = 6; b[3] = 5;
    b[4] = 4; b[5] = 3; b[6] = 2; b[7] = 1;
    heap_sort(b, 8);
    // EXPECT: rev8: 1
    printf("rev8: %d\n", is_sorted(b, 8));
    // EXPECT: 1 2 3 4 5 6 7 8 done
    print_array(b, 8);
    printf("done\n");

    c[0] = 1; c[1] = 2; c[2] = 3; c[3] = 4; c[4] = 5;
    heap_sort(c, 5);
    // EXPECT: already5: 1
    printf("already5: %d\n", is_sorted(c, 5));
    // EXPECT: 1 2 3 4 5 done
    print_array(c, 5);
    printf("done\n");

    d[0] = 4; d[1] = 4; d[2] = 4; d[3] = 4; d[4] = 4; d[5] = 4;
    heap_sort(d, 6);
    // EXPECT: allsame: 1
    printf("allsame: %d\n", is_sorted(d, 6));
    // EXPECT: 4 4 4 4 4 4 done
    print_array(d, 6);
    printf("done\n");

    a[0] = 99;
    heap_sort(a, 1);
    // EXPECT: single: 99
    printf("single: %d\n", a[0]);

    a[0] = 50; a[1] = 10; a[2] = 90; a[3] = 30; a[4] = 70;
    a[5] = 20; a[6] = 80; a[7] = 40; a[8] = 60; a[9] = 0;
    heap_sort(a, 10);
    // EXPECT: tens: 1
    printf("tens: %d\n", is_sorted(a, 10));
    // EXPECT: 0 10 20 30 40 50 60 70 80 90 done
    print_array(a, 10);
    printf("done\n");

    b[0] = 2; b[1] = 1;
    heap_sort(b, 2);
    // EXPECT: pair: 1 2
    printf("pair: %d %d\n", b[0], b[1]);

    c[0] = 3; c[1] = 1; c[2] = 2;
    heap_sort(c, 3);
    // EXPECT: triple: 1 2 3
    printf("triple: %d %d %d\n", c[0], c[1], c[2]);

    return 0;
}
