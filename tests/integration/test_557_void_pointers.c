int printf(const char *fmt, ...);
// EXPECT: Before swap: a=42 b=99\nAfter swap: a=99 b=42\nFilled: 7 7 7 7 7 7\nOriginal: 10 20 30 40 50 60\nSum: 210\nScaled x2: 20 40 60 80 100 120\nReversed: 120 100 80 60 40 20\nFind 60: index=3\nFind 99: index=-1

void swap_ints(void *a, void *b) {
    int *ia = (int *)a;
    int *ib = (int *)b;
    int tmp = *ia;
    *ia = *ib;
    *ib = tmp;
}

void fill_array(void *arr, int count, int value) {
    int *p = (int *)arr;
    int i;
    for (i = 0; i < count; i++) {
        p[i] = value;
    }
}

int sum_via_void(void *arr, int count) {
    int *p = (int *)arr;
    int total = 0;
    int i;
    for (i = 0; i < count; i++) {
        total = total + p[i];
    }
    return total;
}

void scale_array(void *arr, int count, int factor) {
    int *p = (int *)arr;
    int i;
    for (i = 0; i < count; i++) {
        p[i] = p[i] * factor;
    }
}

void reverse_array(void *arr, int count) {
    int *p = (int *)arr;
    int i = 0;
    int j = count - 1;
    while (i < j) {
        int tmp = p[i];
        p[i] = p[j];
        p[j] = tmp;
        i++;
        j--;
    }
}

void print_via_void(void *arr, int count) {
    int *p = (int *)arr;
    int i;
    for (i = 0; i < count; i++) {
        if (i > 0) printf(" ");
        printf("%d", p[i]);
    }
    printf("\n");
}

int find_in_void(void *arr, int count, int target) {
    int *p = (int *)arr;
    int i;
    for (i = 0; i < count; i++) {
        if (p[i] == target) return i;
    }
    return -1;
}

int main(void) {
    int a = 42;
    int b = 99;
    printf("Before swap: a=%d b=%d\n", a, b);
    swap_ints(&a, &b);
    printf("After swap: a=%d b=%d\n", a, b);

    int arr[6];
    fill_array(arr, 6, 7);
    printf("Filled: ");
    print_via_void(arr, 6);

    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50; arr[5] = 60;
    printf("Original: ");
    print_via_void(arr, 6);
    printf("Sum: %d\n", sum_via_void(arr, 6));

    scale_array(arr, 6, 2);
    printf("Scaled x2: ");
    print_via_void(arr, 6);

    reverse_array(arr, 6);
    printf("Reversed: ");
    print_via_void(arr, 6);

    printf("Find 60: index=%d\n", find_in_void(arr, 6, 60));
    printf("Find 99: index=%d\n", find_in_void(arr, 6, 99));

    return 0;
}
