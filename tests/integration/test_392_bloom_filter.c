int printf(const char *fmt, ...);
// EXPECT: membership tests (should be 1):\n  42: 1\n  100: 1\n  255: 1\n  1000: 1\n  7: 1\nnon-member tests:\n  1: 0\n  50: 0\n  200: 1\n  500: 0\n  999: 0\nfalse positives: 1\nbits used: 15 / 64\nfilter[0] = 0x31840262\nfilter[1] = 0x11520100\nempty filter test: 0\nfull filter test: 1\nhash values for 42: 54 6 18
// Test: bloom filter sketch

int hash1(int val) {
    int h = val * 2654435761;
    if (h < 0) h = -h;
    return h % 64;
}

int hash2(int val) {
    int h = val * 40503;
    if (h < 0) h = -h;
    return h % 64;
}

int hash3(int val) {
    int h = (val ^ (val >> 16)) * 73244475;
    if (h < 0) h = -h;
    return h % 64;
}

void bloom_add(int *filter, int val) {
    int h1 = hash1(val);
    int h2 = hash2(val);
    int h3 = hash3(val);
    filter[h1 / 32] = filter[h1 / 32] | (1 << (h1 % 32));
    filter[h2 / 32] = filter[h2 / 32] | (1 << (h2 % 32));
    filter[h3 / 32] = filter[h3 / 32] | (1 << (h3 % 32));
}

int bloom_test(int *filter, int val) {
    int h1 = hash1(val);
    int h2 = hash2(val);
    int h3 = hash3(val);
    int b1 = (filter[h1 / 32] >> (h1 % 32)) & 1;
    int b2 = (filter[h2 / 32] >> (h2 % 32)) & 1;
    int b3 = (filter[h3 / 32] >> (h3 % 32)) & 1;
    return b1 && b2 && b3;
}

int popcount(int x) {
    int c = 0;
    int v = x;
    while (v) { v = v & (v - 1); c++; }
    return c;
}

int main(void) {
    // Create bloom filter (64 bits = 2 ints)
    int filter[2];
    filter[0] = 0;
    filter[1] = 0;

    // Add some values
    int values[5];
    values[0] = 42;
    values[1] = 100;
    values[2] = 255;
    values[3] = 1000;
    values[4] = 7;

    int i;
    for (i = 0; i < 5; i++) {
        bloom_add(filter, values[i]);
    }

    // Test for values we added (should all be positive)
    printf("membership tests (should be 1):\n");
    for (i = 0; i < 5; i++) {
        printf("  %d: %d\n", values[i], bloom_test(filter, values[i]));
    }

    // Test for values we didn't add (may have false positives)
    printf("non-member tests:\n");
    int test_vals[5];
    test_vals[0] = 1;
    test_vals[1] = 50;
    test_vals[2] = 200;
    test_vals[3] = 500;
    test_vals[4] = 999;
    int false_pos = 0;
    for (i = 0; i < 5; i++) {
        int result = bloom_test(filter, test_vals[i]);
        printf("  %d: %d\n", test_vals[i], result);
        if (result) false_pos++;
    }
    printf("false positives: %d\n", false_pos);

    // Bits used
    int bits_used = popcount(filter[0]) + popcount(filter[1]);
    printf("bits used: %d / 64\n", bits_used);

    // Filter state
    printf("filter[0] = 0x%X\n", (unsigned)filter[0]);
    printf("filter[1] = 0x%X\n", (unsigned)filter[1]);

    // Empty filter should return false for everything
    int empty[2];
    empty[0] = 0; empty[1] = 0;
    printf("empty filter test: %d\n", bloom_test(empty, 42));

    // Full filter should return true for everything
    int full[2];
    full[0] = -1; full[1] = -1;
    printf("full filter test: %d\n", bloom_test(full, 42));

    // Hash distribution
    printf("hash values for 42: %d %d %d\n",
           hash1(42), hash2(42), hash3(42));

    return 0;
}
