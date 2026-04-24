int printf(const char *fmt, ...);
// EXPECT: before: 3 7 1 5 8 2 6 4\npartition(5): 3 1 2 4 7 5 8 6\nboundary at 4\nbefore 3way: 4 2 5 5 1 5 8 3 7\n3way(5): 4 2 1 3 5 5 5 7 8\nequal range: [4,6]\neven|odd: 2 4 6 8 1 3 5 7\nboundary at 4\nall less: 1 2 3 4\nboundary at 4
// Test: array partitioning

void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

// Partition: elements < pivot go left, >= pivot go right
// Returns index of first element >= pivot
int partition(int *arr, int n, int pivot) {
    int write = 0;
    int i;
    // First pass: move elements < pivot to front
    int temp[20];
    for (i = 0; i < n; i++) temp[i] = arr[i];

    int left = 0;
    int right = n - 1;
    for (i = 0; i < n; i++) {
        if (temp[i] < pivot) {
            arr[left] = temp[i];
            left = left + 1;
        }
    }
    write = left;
    for (i = 0; i < n; i++) {
        if (temp[i] >= pivot) {
            arr[left] = temp[i];
            left = left + 1;
        }
    }
    return write;
}

// Dutch national flag partition: <, ==, > pivot
void three_way(int *arr, int n, int pivot, int *lo_out, int *hi_out) {
    int lo = 0;
    int mid = 0;
    int hi = n - 1;
    while (mid <= hi) {
        if (arr[mid] < pivot) {
            int tmp = arr[lo]; arr[lo] = arr[mid]; arr[mid] = tmp;
            lo = lo + 1;
            mid = mid + 1;
        } else if (arr[mid] > pivot) {
            int tmp = arr[mid]; arr[mid] = arr[hi]; arr[hi] = tmp;
            hi = hi - 1;
        } else {
            mid = mid + 1;
        }
    }
    *lo_out = lo;
    *hi_out = hi;
}

// Partition even/odd
int partition_even_odd(int *arr, int n) {
    int write = 0;
    int i;
    int temp[20];
    for (i = 0; i < n; i++) temp[i] = arr[i];
    for (i = 0; i < n; i++) {
        if (temp[i] % 2 == 0) {
            arr[write] = temp[i];
            write = write + 1;
        }
    }
    int boundary = write;
    for (i = 0; i < n; i++) {
        if (temp[i] % 2 != 0) {
            arr[write] = temp[i];
            write = write + 1;
        }
    }
    return boundary;
}

int main(void) {
    // Basic partition around 5
    int a[8]; a[0]=3; a[1]=7; a[2]=1; a[3]=5; a[4]=8; a[5]=2; a[6]=6; a[7]=4;
    printf("before: ");
    print_arr(a, 8);
    int p = partition(a, 8, 5);
    printf("partition(5): ");
    print_arr(a, 8);
    printf("boundary at %d\n", p);

    // Three-way partition
    int b[9]; b[0]=4; b[1]=2; b[2]=5; b[3]=5; b[4]=1; b[5]=5; b[6]=8; b[7]=3; b[8]=7;
    printf("before 3way: ");
    print_arr(b, 9);
    int lo2; int hi2;
    three_way(b, 9, 5, &lo2, &hi2);
    printf("3way(5): ");
    print_arr(b, 9);
    printf("equal range: [%d,%d]\n", lo2, hi2);

    // Even/odd partition
    int c[8]; c[0]=1; c[1]=2; c[2]=3; c[3]=4; c[4]=5; c[5]=6; c[6]=7; c[7]=8;
    int bp = partition_even_odd(c, 8);
    printf("even|odd: ");
    print_arr(c, 8);
    printf("boundary at %d\n", bp);

    // Partition with all less
    int d[4]; d[0]=1; d[1]=2; d[2]=3; d[3]=4;
    int pd = partition(d, 4, 10);
    printf("all less: ");
    print_arr(d, 4);
    printf("boundary at %d\n", pd);

    return 0;
}
