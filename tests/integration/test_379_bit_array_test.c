int printf(const char *fmt, ...);
// EXPECT: test_bit:\n  bit 0 = 0\n  bit 5 = 1\n  bit 10 = 1\n  bit 11 = 0\n  bit 50 = 1\n  bit 100 = 1\ntest_any:\n  [0,5) = 0\n  [0,6) = 1\n  [6,10) = 0\n  [11,50) = 0\n  [49,51) = 1\ntest_all [0,32) on full: 1\ntest_all [0,5) on sparse: 0\ntest_none:\n  [0,5) = 1\n  [0,6) = 0\n  [11,50) = 1\nfind_first_set = 5\nfind_first_clear = 20\nprimes < 64: 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61
// Test: bit array - test bit

void set_bit(int *arr, int idx) {
    arr[idx / 32] = arr[idx / 32] | (1 << (idx % 32));
}

int test_bit(int *arr, int idx) {
    return (arr[idx / 32] >> (idx % 32)) & 1;
}

int test_any(int *arr, int start, int end) {
    int i;
    for (i = start; i < end; i++) {
        if (test_bit(arr, i)) return 1;
    }
    return 0;
}

int test_all(int *arr, int start, int end) {
    int i;
    for (i = start; i < end; i++) {
        if (!test_bit(arr, i)) return 0;
    }
    return 1;
}

int test_none(int *arr, int start, int end) {
    return !test_any(arr, start, end);
}

int find_first_set(int *arr, int num_bits) {
    int i;
    for (i = 0; i < num_bits; i++) {
        if (test_bit(arr, i)) return i;
    }
    return -1;
}

int find_first_clear(int *arr, int num_bits) {
    int i;
    for (i = 0; i < num_bits; i++) {
        if (!test_bit(arr, i)) return i;
    }
    return -1;
}

int main(void) {
    // Setup test array
    int bits[4];
    bits[0] = 0; bits[1] = 0; bits[2] = 0; bits[3] = 0;

    // Set some bits
    set_bit(bits, 5);
    set_bit(bits, 10);
    set_bit(bits, 50);
    set_bit(bits, 100);

    // Test individual bits
    printf("test_bit:\n");
    printf("  bit 0 = %d\n", test_bit(bits, 0));
    printf("  bit 5 = %d\n", test_bit(bits, 5));
    printf("  bit 10 = %d\n", test_bit(bits, 10));
    printf("  bit 11 = %d\n", test_bit(bits, 11));
    printf("  bit 50 = %d\n", test_bit(bits, 50));
    printf("  bit 100 = %d\n", test_bit(bits, 100));

    // Test range - any
    printf("test_any:\n");
    printf("  [0,5) = %d\n", test_any(bits, 0, 5));
    printf("  [0,6) = %d\n", test_any(bits, 0, 6));
    printf("  [6,10) = %d\n", test_any(bits, 6, 10));
    printf("  [11,50) = %d\n", test_any(bits, 11, 50));
    printf("  [49,51) = %d\n", test_any(bits, 49, 51));

    // Test range - all
    int full[1];
    full[0] = -1;  // all bits set
    printf("test_all [0,32) on full: %d\n", test_all(full, 0, 32));
    printf("test_all [0,5) on sparse: %d\n", test_all(bits, 0, 5));

    // Test range - none
    printf("test_none:\n");
    printf("  [0,5) = %d\n", test_none(bits, 0, 5));
    printf("  [0,6) = %d\n", test_none(bits, 0, 6));
    printf("  [11,50) = %d\n", test_none(bits, 11, 50));

    // Find first set
    printf("find_first_set = %d\n", find_first_set(bits, 128));

    // Find first clear
    int almost_full[1];
    almost_full[0] = -1;
    almost_full[0] = almost_full[0] & ~(1 << 20);  // clear bit 20
    printf("find_first_clear = %d\n", find_first_clear(almost_full, 32));

    // Sieve of Eratosthenes using bit array
    int sieve[2];  // 64 bits
    sieve[0] = -1; sieve[1] = -1;
    // Clear 0 and 1
    sieve[0] = sieve[0] & ~1;
    sieve[0] = sieve[0] & ~2;
    int i;
    int j;
    for (i = 2; i < 8; i++) {
        if (test_bit(sieve, i)) {
            for (j = i * i; j < 64; j += i) {
                sieve[j / 32] = sieve[j / 32] & ~(1 << (j % 32));
            }
        }
    }
    printf("primes < 64: ");
    int first = 1;
    for (i = 2; i < 64; i++) {
        if (test_bit(sieve, i)) {
            if (!first) printf(" ");
            printf("%d", i);
            first = 0;
        }
    }
    printf("\n");

    return 0;
}
