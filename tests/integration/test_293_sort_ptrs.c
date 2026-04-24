int printf(const char *fmt, ...);
// EXPECT: selection before: 64 25 12 22 11 90 45 33\nselection after:  11 12 22 25 33 45 64 90\nsorted: 1\ninsertion before: 5 2 4 6 1 3\ninsertion after:  1 2 3 4 5 6\nbubble before: 9 7 5 3 1 8 6\nbubble after:  1 3 5 6 7 8 9\nalready sorted: 1\nstill: 1 2 3 4\nreverse sorted: 10 20 30 40 50
// Test: sorting with pointer comparisons

void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// Selection sort using pointer arithmetic
void selection_sort(int *arr, int n) {
    int *p;
    int *end = arr + n;
    for (p = arr; p < end - 1; p = p + 1) {
        int *min_p = p;
        int *q;
        for (q = p + 1; q < end; q = q + 1) {
            if (*q < *min_p) min_p = q;
        }
        if (min_p != p) swap(p, min_p);
    }
}

// Insertion sort using pointer arithmetic
void insertion_sort(int *arr, int n) {
    int *p;
    for (p = arr + 1; p < arr + n; p = p + 1) {
        int key = *p;
        int *q = p - 1;
        while (q >= arr && *q > key) {
            *(q + 1) = *q;
            q = q - 1;
        }
        *(q + 1) = key;
    }
}

// Bubble sort using pointer arithmetic
void bubble_sort(int *arr, int n) {
    int *end = arr + n;
    int *i;
    int swapped;
    do {
        swapped = 0;
        for (i = arr; i < end - 1; i = i + 1) {
            if (*i > *(i + 1)) {
                swap(i, i + 1);
                swapped = 1;
            }
        }
        end = end - 1;
    } while (swapped);
}

// Check if sorted
int is_sorted(int *arr, int n) {
    int *p;
    for (p = arr; p < arr + n - 1; p = p + 1) {
        if (*p > *(p + 1)) return 0;
    }
    return 1;
}

int main(void) {
    // Selection sort
    int a[8];
    a[0]=64; a[1]=25; a[2]=12; a[3]=22; a[4]=11; a[5]=90; a[6]=45; a[7]=33;
    printf("selection before: ");
    print_arr(a, 8);
    selection_sort(a, 8);
    printf("selection after:  ");
    print_arr(a, 8);
    printf("sorted: %d\n", is_sorted(a, 8));

    // Insertion sort
    int b[6];
    b[0]=5; b[1]=2; b[2]=4; b[3]=6; b[4]=1; b[5]=3;
    printf("insertion before: ");
    print_arr(b, 6);
    insertion_sort(b, 6);
    printf("insertion after:  ");
    print_arr(b, 6);

    // Bubble sort
    int c[7];
    c[0]=9; c[1]=7; c[2]=5; c[3]=3; c[4]=1; c[5]=8; c[6]=6;
    printf("bubble before: ");
    print_arr(c, 7);
    bubble_sort(c, 7);
    printf("bubble after:  ");
    print_arr(c, 7);

    // Already sorted
    int d[4];
    d[0]=1; d[1]=2; d[2]=3; d[3]=4;
    printf("already sorted: %d\n", is_sorted(d, 4));
    selection_sort(d, 4);
    printf("still: ");
    print_arr(d, 4);

    // Reverse sorted
    int e[5];
    e[0]=50; e[1]=40; e[2]=30; e[3]=20; e[4]=10;
    bubble_sort(e, 5);
    printf("reverse sorted: ");
    print_arr(e, 5);

    return 0;
}
