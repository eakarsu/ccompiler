int printf(const char *fmt, ...);
// EXPECT: popcount(0) = 0\npopcount(1) = 1\npopcount(2) = 1\npopcount(3) = 2\npopcount(7) = 3\npopcount(255) = 8\nPowers of 2 popcount: 1 1 1 1 1 1 1 1\nfast popcount(0) = 0\nfast popcount(1) = 1\nfast popcount(255) = 8\nfast popcount(0xAA) = 4\nfast popcount(0x55) = 4\nmethods agree: 1\nhamming(0xAA, 0x55) = 8\nhamming(0xFF, 0xFE) = 1\nnibble 0 (0xF): 4 bits\nnibble 1 (0xE): 3 bits\nnibble 2 (0xE): 3 bits\nnibble 3 (0xB): 3 bits
// Test: bit counting (popcount)

int popcount(int x) {
    int count = 0;
    int val = x;
    while (val) {
        count = count + (val & 1);
        val = (unsigned)val >> 1;
    }
    return count;
}

// Brian Kernighan's algorithm
int popcount_fast(int x) {
    int count = 0;
    int val = x;
    while (val) {
        val = val & (val - 1);
        count++;
    }
    return count;
}

int main(void) {
    // Basic popcount
    printf("popcount(0) = %d\n", popcount(0));
    printf("popcount(1) = %d\n", popcount(1));
    printf("popcount(2) = %d\n", popcount(2));
    printf("popcount(3) = %d\n", popcount(3));
    printf("popcount(7) = %d\n", popcount(7));
    printf("popcount(255) = %d\n", popcount(255));

    // Popcount of powers of 2
    int i;
    printf("Powers of 2 popcount: ");
    for (i = 0; i < 8; i++) {
        if (i > 0) printf(" ");
        printf("%d", popcount(1 << i));
    }
    printf("\n");

    // Fast popcount comparison
    printf("fast popcount(0) = %d\n", popcount_fast(0));
    printf("fast popcount(1) = %d\n", popcount_fast(1));
    printf("fast popcount(255) = %d\n", popcount_fast(255));
    printf("fast popcount(0xAA) = %d\n", popcount_fast(0xAA));
    printf("fast popcount(0x55) = %d\n", popcount_fast(0x55));

    // Verify both methods agree
    int test_vals[8];
    test_vals[0] = 0;
    test_vals[1] = 1;
    test_vals[2] = 42;
    test_vals[3] = 255;
    test_vals[4] = 1023;
    test_vals[5] = 0xDEAD;
    test_vals[6] = 0x5555;
    test_vals[7] = 0xFFFF;

    int all_match = 1;
    for (i = 0; i < 8; i++) {
        if (popcount(test_vals[i]) != popcount_fast(test_vals[i])) {
            all_match = 0;
        }
    }
    printf("methods agree: %d\n", all_match);

    // Popcount for Hamming distance
    int a = 0xAA;
    int b = 0x55;
    int hamming = popcount(a ^ b);
    printf("hamming(0xAA, 0x55) = %d\n", hamming);

    int c = 0xFF;
    int d = 0xFE;
    printf("hamming(0xFF, 0xFE) = %d\n", popcount(c ^ d));

    // Count bits set in each nibble
    int val = 0xBEEF;
    for (i = 0; i < 4; i++) {
        int nibble = (val >> (i * 4)) & 0xF;
        printf("nibble %d (0x%X): %d bits\n", i, nibble, popcount(nibble));
    }

    return 0;
}
