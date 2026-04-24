int printf(const char *fmt, ...);

void xor_swap(int *a, int *b) {
    if (a == b) return;
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

void xor_swap_array(int *arr, int i, int j) {
    if (i == j) return;
    arr[i] = arr[i] ^ arr[j];
    arr[j] = arr[i] ^ arr[j];
    arr[i] = arr[i] ^ arr[j];
}

int swap_bits(int x, int i, int j) {
    int bit_i;
    int bit_j;
    int xor_val;
    bit_i = (x >> i) & 1;
    bit_j = (x >> j) & 1;
    if (bit_i != bit_j) {
        xor_val = (1 << i) | (1 << j);
        x = x ^ xor_val;
    }
    return x;
}

int swap_even_odd_bits(int x) {
    int even;
    int odd;
    even = x & 1431655765;
    odd = x & 715827882;
    return (even << 1) | (odd >> 1);
}

void reverse_array_xor(int *arr, int n) {
    int left;
    int right;
    left = 0;
    right = n - 1;
    while (left < right) {
        xor_swap_array(arr, left, right);
        left = left + 1;
        right = right - 1;
    }
}

void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

void xor_sort_bubble(int *arr, int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (arr[j] > arr[j + 1]) {
                xor_swap_array(arr, j, j + 1);
            }
        }
    }
}

int conditional_negate(int x, int negate) {
    int mask;
    mask = 0;
    if (negate) mask = -1;
    return (x ^ mask) - mask;
}

int abs_no_branch(int x) {
    int mask;
    mask = x >> 31;
    return (x + mask) ^ mask;
}

int main() {
    int a;
    int b;
    int arr[8];
    int val;
    int i;

    printf("=== XOR Swap ===\n");
    // EXPECT: === XOR Swap ===

    a = 42;
    b = 99;
    printf("before: a=%d b=%d\n", a, b);
    // EXPECT: before: a=42 b=99
    xor_swap(&a, &b);
    printf("after: a=%d b=%d\n", a, b);
    // EXPECT: after: a=99 b=42

    a = -5;
    b = 10;
    printf("before: a=%d b=%d\n", a, b);
    // EXPECT: before: a=-5 b=10
    xor_swap(&a, &b);
    printf("after: a=%d b=%d\n", a, b);
    // EXPECT: after: a=10 b=-5

    a = 0;
    b = 0;
    xor_swap(&a, &b);
    printf("swap zeros: a=%d b=%d\n", a, b);
    // EXPECT: swap zeros: a=0 b=0

    printf("=== Swap Bits ===\n");
    // EXPECT: === Swap Bits ===

    val = 5;
    printf("swap_bits(%d,0,2) = %d\n", val, swap_bits(val, 0, 2));
    // EXPECT: swap_bits(5,0,2) = 5

    val = 6;
    printf("swap_bits(%d,0,2) = %d\n", val, swap_bits(val, 0, 2));
    // EXPECT: swap_bits(6,0,2) = 3

    val = 1;
    printf("swap_bits(%d,0,3) = %d\n", val, swap_bits(val, 0, 3));
    // EXPECT: swap_bits(1,0,3) = 8

    printf("=== Swap Even/Odd Bits ===\n");
    // EXPECT: === Swap Even/Odd Bits ===

    printf("swap_eo(%d) = %d\n", 170, swap_even_odd_bits(170));
    // EXPECT: swap_eo(170) = 85
    printf("swap_eo(%d) = %d\n", 85, swap_even_odd_bits(85));
    // EXPECT: swap_eo(85) = 170

    printf("=== Reverse Array via XOR ===\n");
    // EXPECT: === Reverse Array via XOR ===

    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4;
    arr[4] = 5; arr[5] = 6; arr[6] = 7; arr[7] = 8;
    printf("before: ");
    // EXPECT: before: 1 2 3 4 5 6 7 8
    print_array(arr, 8);
    reverse_array_xor(arr, 8);
    printf("after:  ");
    // EXPECT: after:  8 7 6 5 4 3 2 1
    print_array(arr, 8);

    printf("=== XOR Bubble Sort ===\n");
    // EXPECT: === XOR Bubble Sort ===

    arr[0] = 5; arr[1] = 3; arr[2] = 8; arr[3] = 1;
    arr[4] = 9; arr[5] = 2; arr[6] = 7; arr[7] = 4;
    printf("before: ");
    // EXPECT: before: 5 3 8 1 9 2 7 4
    print_array(arr, 8);
    xor_sort_bubble(arr, 8);
    printf("sorted: ");
    // EXPECT: sorted: 1 2 3 4 5 7 8 9
    print_array(arr, 8);

    printf("=== Conditional Negate ===\n");
    // EXPECT: === Conditional Negate ===

    printf("cneg(%d,%d) = %d\n", 5, 0, conditional_negate(5, 0));
    // EXPECT: cneg(5,0) = 5
    printf("cneg(%d,%d) = %d\n", 5, 1, conditional_negate(5, 1));
    // EXPECT: cneg(5,1) = -5

    printf("=== Abs Without Branch ===\n");
    // EXPECT: === Abs Without Branch ===

    printf("abs(%d) = %d\n", 5, abs_no_branch(5));
    // EXPECT: abs(5) = 5
    printf("abs(%d) = %d\n", -5, abs_no_branch(-5));
    // EXPECT: abs(-5) = 5
    printf("abs(%d) = %d\n", 0, abs_no_branch(0));
    // EXPECT: abs(0) = 0

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
