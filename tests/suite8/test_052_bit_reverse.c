int printf(const char *fmt, ...);

int reverse_bits_8(int x) {
    int result;
    int i;
    result = 0;
    for (i = 0; i < 8; i = i + 1) {
        result = result << 1;
        result = result | (x & 1);
        x = x >> 1;
    }
    return result;
}

int reverse_bits_16(int x) {
    int result;
    int i;
    result = 0;
    for (i = 0; i < 16; i = i + 1) {
        result = result << 1;
        result = result | (x & 1);
        x = x >> 1;
    }
    return result;
}

int reverse_nibble(int x) {
    int result;
    int i;
    result = 0;
    for (i = 0; i < 4; i = i + 1) {
        result = result << 1;
        result = result | (x & 1);
        x = x >> 1;
    }
    return result;
}

int swap_nibbles(int x) {
    return ((x & 15) << 4) | ((x >> 4) & 15);
}

int reverse_byte_order_16(int x) {
    int lo;
    int hi;
    lo = x & 255;
    hi = (x >> 8) & 255;
    return (lo << 8) | hi;
}

int print_bits_8(int x) {
    int i;
    int bit;
    for (i = 7; i >= 0; i = i - 1) {
        bit = (x >> i) & 1;
        printf("%d", bit);
    }
    return 0;
}

void test_reverse_pair(int val) {
    int rev;
    int rev_rev;
    rev = reverse_bits_8(val);
    rev_rev = reverse_bits_8(rev);
    printf("rev8(%d) = %d, back = %d\n", val, rev, rev_rev);
}

int main() {
    int val;
    int rev;
    int i;

    printf("=== 8-bit Reverse ===\n");
    // EXPECT: === 8-bit Reverse ===

    test_reverse_pair(1);
    // EXPECT: rev8(1) = 128, back = 1
    test_reverse_pair(170);
    // EXPECT: rev8(170) = 85, back = 170
    test_reverse_pair(255);
    // EXPECT: rev8(255) = 255, back = 255
    test_reverse_pair(0);
    // EXPECT: rev8(0) = 0, back = 0

    printf("=== 16-bit Reverse ===\n");
    // EXPECT: === 16-bit Reverse ===

    val = 1;
    rev = reverse_bits_16(val);
    printf("rev16(%d) = %d\n", val, rev);
    // EXPECT: rev16(1) = 32768

    val = 256;
    rev = reverse_bits_16(val);
    printf("rev16(%d) = %d\n", val, rev);
    // EXPECT: rev16(256) = 128

    val = 65535;
    rev = reverse_bits_16(val);
    printf("rev16(%d) = %d\n", val, rev);
    // EXPECT: rev16(65535) = 65535

    printf("=== Nibble Reverse ===\n");
    // EXPECT: === Nibble Reverse ===

    for (i = 0; i < 16; i = i + 1) {
        rev = reverse_nibble(i);
        if (i > 0) printf(" ");
        printf("rnib(%d)=%d", i, rev);
    }
    printf("\n");
    // EXPECT: rnib(0)=0 rnib(1)=8 rnib(2)=4 rnib(3)=12 rnib(4)=2 rnib(5)=10 rnib(6)=6 rnib(7)=14 rnib(8)=1 rnib(9)=9 rnib(10)=5 rnib(11)=13 rnib(12)=3 rnib(13)=11 rnib(14)=7 rnib(15)=15

    printf("=== Swap Nibbles ===\n");
    // EXPECT: === Swap Nibbles ===

    printf("swap_nib(%d) = %d\n", 0, swap_nibbles(0));
    // EXPECT: swap_nib(0) = 0
    printf("swap_nib(%d) = %d\n", 18, swap_nibbles(18));
    // EXPECT: swap_nib(18) = 33
    printf("swap_nib(%d) = %d\n", 171, swap_nibbles(171));
    // EXPECT: swap_nib(171) = 186

    printf("=== Byte Swap 16 ===\n");
    // EXPECT: === Byte Swap 16 ===

    printf("bswap16(%d) = %d\n", 256, reverse_byte_order_16(256));
    // EXPECT: bswap16(256) = 1
    printf("bswap16(%d) = %d\n", 513, reverse_byte_order_16(513));
    // EXPECT: bswap16(513) = 258
    printf("bswap16(%d) = %d\n", 4660, reverse_byte_order_16(4660));
    // EXPECT: bswap16(4660) = 13330

    printf("=== Bits of 170 ===\n");
    // EXPECT: === Bits of 170 ===
    print_bits_8(170);
    printf("\n");
    // EXPECT: 10101010

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
