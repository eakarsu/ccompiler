int printf(const char *fmt, ...);
// EXPECT: before: a=10 b=20\nafter:  a=20 b=10\nswap(42,0): c=0 d=42\nswap(-5,100): e=100 f=-5\nswap(77,77): g=77 h=77\nsorted: 1 2 3 4 5 7 8 9\nnibble swap 0xAB = 0xBA\nbyte swap 0x1234 = 0x3412\nreversed: 6 5 4 3 2 1\nconditional swap(30,10): p=10 q=30
// Test: bitwise swap without temp

void xor_swap(int *a, int *b) {
    if (a != b) {
        *a = *a ^ *b;
        *b = *a ^ *b;
        *a = *a ^ *b;
    }
}

int main(void) {
    // Basic XOR swap
    int a = 10;
    int b = 20;
    printf("before: a=%d b=%d\n", a, b);
    xor_swap(&a, &b);
    printf("after:  a=%d b=%d\n", a, b);

    // Swap with zero
    int c = 42;
    int d = 0;
    xor_swap(&c, &d);
    printf("swap(42,0): c=%d d=%d\n", c, d);

    // Swap negative numbers
    int e = -5;
    int f = 100;
    xor_swap(&e, &f);
    printf("swap(-5,100): e=%d f=%d\n", e, f);

    // Swap equal values
    int g = 77;
    int h = 77;
    xor_swap(&g, &h);
    printf("swap(77,77): g=%d h=%d\n", g, h);

    // Sort array using XOR swap
    int arr[8];
    arr[0] = 5; arr[1] = 3; arr[2] = 8; arr[3] = 1;
    arr[4] = 9; arr[5] = 2; arr[6] = 7; arr[7] = 4;
    int i;
    int j;
    for (i = 0; i < 8; i++) {
        for (j = i + 1; j < 8; j++) {
            if (arr[i] > arr[j]) {
                xor_swap(&arr[i], &arr[j]);
            }
        }
    }
    printf("sorted: ");
    for (i = 0; i < 8; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");

    // Swap nibbles without temp
    int val = 0xAB;
    int swapped = ((val & 0x0F) << 4) | ((val & 0xF0) >> 4);
    printf("nibble swap 0xAB = 0x%X\n", swapped);

    // Swap bytes without temp
    int word = 0x1234;
    int byte_swapped = ((word & 0xFF) << 8) | ((word >> 8) & 0xFF);
    printf("byte swap 0x1234 = 0x%X\n", byte_swapped);

    // Reverse array using XOR swap
    int rev[6];
    rev[0] = 1; rev[1] = 2; rev[2] = 3;
    rev[3] = 4; rev[4] = 5; rev[5] = 6;
    for (i = 0; i < 3; i++) {
        xor_swap(&rev[i], &rev[5 - i]);
    }
    printf("reversed: ");
    for (i = 0; i < 6; i++) {
        if (i > 0) printf(" ");
        printf("%d", rev[i]);
    }
    printf("\n");

    // Conditional swap (swap if first > second)
    int p = 30;
    int q = 10;
    int should_swap = p > q;
    int xor_val = (p ^ q) & -(should_swap);
    p = p ^ xor_val;
    q = q ^ xor_val;
    printf("conditional swap(30,10): p=%d q=%d\n", p, q);

    return 0;
}
