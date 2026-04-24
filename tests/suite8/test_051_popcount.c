int printf(const char *fmt, ...);

int popcount(int x) {
    int count;
    count = 0;
    while (x) {
        count = count + (x & 1);
        x = x >> 1;
    }
    return count;
}

int popcount_kernighan(int x) {
    int count;
    count = 0;
    while (x) {
        x = x & (x - 1);
        count = count + 1;
    }
    return count;
}

int popcount_lookup(int x) {
    int table[16];
    int count;
    int i;
    table[0] = 0; table[1] = 1; table[2] = 1; table[3] = 2;
    table[4] = 1; table[5] = 2; table[6] = 2; table[7] = 3;
    table[8] = 1; table[9] = 2; table[10] = 2; table[11] = 3;
    table[12] = 2; table[13] = 3; table[14] = 3; table[15] = 4;
    count = 0;
    for (i = 0; i < 8; i = i + 1) {
        count = count + table[(x >> (i * 4)) & 15];
    }
    return count;
}

int count_trailing_zeros(int x) {
    int count;
    if (x == 0) return 32;
    count = 0;
    while ((x & 1) == 0) {
        count = count + 1;
        x = x >> 1;
    }
    return count;
}

int count_leading_zeros(int x) {
    int count;
    int mask;
    if (x == 0) return 32;
    count = 0;
    mask = 1 << 30;
    while (mask > 0 && (x & mask) == 0) {
        count = count + 1;
        mask = mask >> 1;
    }
    return count;
}

int main() {
    int val;
    int r1;
    int r2;
    int r3;
    int i;

    printf("=== Popcount Tests ===\n");
    // EXPECT: === Popcount Tests ===

    val = 0;
    printf("popcount(%d) = %d\n", val, popcount(val));
    // EXPECT: popcount(0) = 0

    val = 1;
    printf("popcount(%d) = %d\n", val, popcount(val));
    // EXPECT: popcount(1) = 1

    val = 7;
    printf("popcount(%d) = %d\n", val, popcount(val));
    // EXPECT: popcount(7) = 3

    val = 255;
    printf("popcount(%d) = %d\n", val, popcount(val));
    // EXPECT: popcount(255) = 8

    val = 1023;
    printf("popcount(%d) = %d\n", val, popcount(val));
    // EXPECT: popcount(1023) = 10

    printf("=== Kernighan Method ===\n");
    // EXPECT: === Kernighan Method ===

    val = 0;
    printf("kernighan(%d) = %d\n", val, popcount_kernighan(val));
    // EXPECT: kernighan(0) = 0

    val = 128;
    printf("kernighan(%d) = %d\n", val, popcount_kernighan(val));
    // EXPECT: kernighan(128) = 1

    val = 170;
    printf("kernighan(%d) = %d\n", val, popcount_kernighan(val));
    // EXPECT: kernighan(170) = 4

    printf("=== Lookup Table Method ===\n");
    // EXPECT: === Lookup Table Method ===

    val = 255;
    printf("lookup(%d) = %d\n", val, popcount_lookup(val));
    // EXPECT: lookup(255) = 8

    val = 12345;
    r1 = popcount(val);
    r2 = popcount_kernighan(val);
    r3 = popcount_lookup(val);
    printf("all_agree(%d): %d %d %d\n", val, r1, r2, r3);
    // EXPECT: all_agree(12345): 6 6 6

    printf("=== Trailing Zeros ===\n");
    // EXPECT: === Trailing Zeros ===

    printf("ctz(%d) = %d\n", 0, count_trailing_zeros(0));
    // EXPECT: ctz(0) = 32
    printf("ctz(%d) = %d\n", 1, count_trailing_zeros(1));
    // EXPECT: ctz(1) = 0
    printf("ctz(%d) = %d\n", 8, count_trailing_zeros(8));
    // EXPECT: ctz(8) = 3
    printf("ctz(%d) = %d\n", 1024, count_trailing_zeros(1024));
    // EXPECT: ctz(1024) = 10

    printf("=== Leading Zeros ===\n");
    // EXPECT: === Leading Zeros ===

    printf("clz(%d) = %d\n", 0, count_leading_zeros(0));
    // EXPECT: clz(0) = 32
    printf("clz(%d) = %d\n", 1, count_leading_zeros(1));
    // EXPECT: clz(1) = 30
    printf("clz(%d) = %d\n", 1024, count_leading_zeros(1024));
    // EXPECT: clz(1024) = 20

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
