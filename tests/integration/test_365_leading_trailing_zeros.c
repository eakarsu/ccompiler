int printf(const char *fmt, ...);
// EXPECT: Trailing zeros:\n  0: 32\n  1: 0\n  2: 1\n  4: 2\n  8: 3\n  12: 2\n  16: 4\n  48: 4\n  1024: 10\nLeading zeros (unsigned):\n  1: 31\n  2: 30\n  255: 24\n  256: 23\n  65535: 16\n  65536: 15\nHighest bit:\n  1: bit 0\n  2: bit 1\n  3: bit 1\n  4: bit 2\n  7: bit 2\n  8: bit 3\n  255: bit 7\n  1024: bit 10\nCLZ + highest = 31 check:\n  1: clz=31 hb=0 sum=31\n  2: clz=30 hb=1 sum=31\n  3: clz=30 hb=1 sum=31\n  4: clz=29 hb=2 sum=31\n  7: clz=29 hb=2 sum=31\n  8: clz=28 hb=3 sum=31\n  255: clz=24 hb=7 sum=31\n  1024: clz=21 hb=10 sum=31\nTrailing zeros in products:\n  1! = 1, tz = 0\n  2! = 2, tz = 1\n  3! = 6, tz = 1\n  4! = 24, tz = 3\n  5! = 120, tz = 3\n  6! = 720, tz = 4\n  7! = 5040, tz = 4\n  8! = 40320, tz = 7
// Test: leading/trailing zeros

int count_trailing_zeros(int n) {
    if (n == 0) return 32;
    int count = 0;
    int val = n;
    while ((val & 1) == 0) {
        count++;
        val = val >> 1;
    }
    return count;
}

int count_leading_zeros(int n) {
    if (n == 0) return 32;
    int count = 0;
    int val = n;
    // For 32-bit int
    int bit = 1 << 30;  // start from bit 30 (bit 31 is sign)
    while (bit > 0 && (val & bit) == 0) {
        count++;
        bit = bit >> 1;
    }
    return count + 1;  // +1 for sign bit (assuming positive numbers)
}

int count_leading_zeros_unsigned(int n) {
    if (n == 0) return 32;
    int count = 0;
    unsigned int val = (unsigned int)n;
    unsigned int bit = 1u << 31;
    while (bit > 0 && (val & bit) == 0) {
        count++;
        bit = bit >> 1;
    }
    return count;
}

int find_highest_bit(int n) {
    if (n == 0) return -1;
    int pos = 0;
    int val = n;
    while (val > 1) {
        val = val >> 1;
        pos++;
    }
    return pos;
}

int main(void) {
    // Trailing zeros
    printf("Trailing zeros:\n");
    printf("  0: %d\n", count_trailing_zeros(0));
    printf("  1: %d\n", count_trailing_zeros(1));
    printf("  2: %d\n", count_trailing_zeros(2));
    printf("  4: %d\n", count_trailing_zeros(4));
    printf("  8: %d\n", count_trailing_zeros(8));
    printf("  12: %d\n", count_trailing_zeros(12));
    printf("  16: %d\n", count_trailing_zeros(16));
    printf("  48: %d\n", count_trailing_zeros(48));
    printf("  1024: %d\n", count_trailing_zeros(1024));

    // Leading zeros (for unsigned interpretation)
    printf("Leading zeros (unsigned):\n");
    printf("  1: %d\n", count_leading_zeros_unsigned(1));
    printf("  2: %d\n", count_leading_zeros_unsigned(2));
    printf("  255: %d\n", count_leading_zeros_unsigned(255));
    printf("  256: %d\n", count_leading_zeros_unsigned(256));
    printf("  65535: %d\n", count_leading_zeros_unsigned(65535));
    printf("  65536: %d\n", count_leading_zeros_unsigned(65536));

    // Highest bit position
    printf("Highest bit:\n");
    int vals[8];
    vals[0] = 1; vals[1] = 2; vals[2] = 3; vals[3] = 4;
    vals[4] = 7; vals[5] = 8; vals[6] = 255; vals[7] = 1024;
    int i;
    for (i = 0; i < 8; i++) {
        printf("  %d: bit %d\n", vals[i], find_highest_bit(vals[i]));
    }

    // Relationship: clz + highest_bit = 31 for positive numbers
    printf("CLZ + highest = 31 check:\n");
    for (i = 0; i < 8; i++) {
        int clz = count_leading_zeros_unsigned(vals[i]);
        int hb = find_highest_bit(vals[i]);
        printf("  %d: clz=%d hb=%d sum=%d\n", vals[i], clz, hb, clz + hb);
    }

    // Trailing zeros for factorial-like products
    int product = 1;
    printf("Trailing zeros in products:\n");
    for (i = 1; i <= 8; i++) {
        product = product * i;
        printf("  %d! = %d, tz = %d\n", i, product,
               count_trailing_zeros(product));
    }

    return 0;
}
