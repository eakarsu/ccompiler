int printf(const char *fmt, ...);

// Test unsigned arithmetic behavior using explicit casting.
// Since we only have int (signed), we simulate unsigned operations
// and observe behavior when interpreting bit patterns differently.

int as_unsigned_mod(int val, int mod) {
    // For positive mod values, this works as unsigned mod
    // when val is also positive.
    return val % mod;
}

int unsigned_compare(int a, int b) {
    // Compare as if unsigned by checking signs first
    // If both positive or both negative, normal compare works
    // If a is negative (high bit set = large unsigned), a > b unsigned
    if (a < 0 && b >= 0) return 1;  // a is "large" unsigned
    if (a >= 0 && b < 0) return -1; // b is "large" unsigned
    if (a > b) return 1;
    if (a < b) return -1;
    return 0;
}

int high_bit_set(int val) {
    return val < 0;
}

int low_byte(int val) {
    return val & 255;
}

int second_byte(int val) {
    return (val >> 8) & 255;
}

int third_byte(int val) {
    return (val >> 16) & 255;
}

int high_byte(int val) {
    // Arithmetic right shift for signed, but we mask
    return (val >> 24) & 255;
}

int pack_bytes(int b3, int b2, int b1, int b0) {
    return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
}

int count_ones(int val) {
    int count = 0;
    int i;
    // Check 32 bits
    for (i = 0; i < 32; i = i + 1) {
        if ((val >> i) & 1) {
            count = count + 1;
        }
    }
    return count;
}

int rotate_left(int val, int n) {
    // Rotate left by n bits (treating as unsigned 32-bit)
    return (val << n) | ((val >> (32 - n)) & ((1 << n) - 1));
}

int reverse_byte(int byte) {
    int result = 0;
    int i;
    for (i = 0; i < 8; i = i + 1) {
        if ((byte >> i) & 1) {
            result = result | (1 << (7 - i));
        }
    }
    return result;
}

int main(void) {
    // Low byte extraction
    // EXPECT: low_byte(0x12345678) = 120
    printf("low_byte(0x12345678) = %d\n", low_byte(0x12345678));

    // Actually 0x78 = 120
    // EXPECT: second_byte(0x12345678) = 86
    printf("second_byte(0x12345678) = %d\n", second_byte(0x12345678));

    // 0x34 = 52... wait let me recheck: 0x12345678
    // low byte: 0x78 = 120, second: 0x56 = 86, third: 0x34 = 52, high: 0x12 = 18
    // EXPECT: third_byte(0x12345678) = 52
    printf("third_byte(0x12345678) = %d\n", third_byte(0x12345678));

    // EXPECT: high_byte(0x12345678) = 18
    printf("high_byte(0x12345678) = %d\n", high_byte(0x12345678));

    // Pack bytes back together
    int packed = pack_bytes(18, 52, 86, 120);
    // EXPECT: packed = 305419896
    printf("packed = %d\n", packed);

    // Verify 0x12345678 = 305419896
    // EXPECT: original = 305419896
    printf("original = %d\n", 0x12345678);

    // High bit check: positive number
    // EXPECT: high_bit(100) = 0
    printf("high_bit(100) = %d\n", high_bit_set(100));

    // High bit check: negative number (high bit set in two's complement)
    // EXPECT: high_bit(-1) = 1
    printf("high_bit(-1) = %d\n", high_bit_set(-1));

    // Unsigned comparison: -1 as unsigned > 1 as unsigned
    // EXPECT: ucmp(-1, 1) = 1
    printf("ucmp(-1, 1) = %d\n", unsigned_compare(-1, 1));

    // EXPECT: ucmp(1, -1) = -1
    printf("ucmp(1, -1) = %d\n", unsigned_compare(1, -1));

    // EXPECT: ucmp(5, 5) = 0
    printf("ucmp(5, 5) = %d\n", unsigned_compare(5, 5));

    // EXPECT: ucmp(10, 5) = 1
    printf("ucmp(10, 5) = %d\n", unsigned_compare(10, 5));

    // Count ones (popcount)
    // EXPECT: popcount(0) = 0
    printf("popcount(0) = %d\n", count_ones(0));

    // EXPECT: popcount(1) = 1
    printf("popcount(1) = %d\n", count_ones(1));

    // EXPECT: popcount(7) = 3
    printf("popcount(7) = %d\n", count_ones(7));

    // 255 = 0xFF = 8 ones
    // EXPECT: popcount(255) = 8
    printf("popcount(255) = %d\n", count_ones(255));

    // -1 = all ones = 32 ones
    // EXPECT: popcount(-1) = 32
    printf("popcount(-1) = %d\n", count_ones(-1));

    // Byte reversal: 0b10110100 = 0xB4 = 180
    // reversed: 0b00101101 = 0x2D = 45
    // EXPECT: reverse(180) = 45
    printf("reverse(180) = %d\n", reverse_byte(180));

    // EXPECT: reverse(1) = 128
    printf("reverse(1) = %d\n", reverse_byte(1));

    // EXPECT: reverse(255) = 255
    printf("reverse(255) = %d\n", reverse_byte(255));

    // EXPECT: reverse(0) = 0
    printf("reverse(0) = %d\n", reverse_byte(0));

    // Masking operations
    int val = -1;  // all bits set
    // EXPECT: -1 & 0xFF = 255
    printf("-1 & 0xFF = %d\n", val & 0xFF);

    // EXPECT: -1 & 0xFFFF = 65535
    printf("-1 & 0xFFFF = %d\n", val & 0xFFFF);

    // Shift and mask: extract bits 8-15
    int v = 0xABCD;  // 43981
    int bits_8_15 = (v >> 8) & 0xFF;
    // 0xABCD >> 8 = 0xAB = 171
    // EXPECT: bits[8:15] = 171
    printf("bits[8:15] = %d\n", bits_8_15);

    // Rotate left by 4
    int rl = rotate_left(0x12345678, 4);
    // 0x12345678 << 4 = 0x23456780, top 4 bits of original = 0x1
    // result = 0x23456780 | 0x1 = 0x23456781
    // 0x23456781 = 591751041
    // EXPECT: rotl(0x12345678, 4) = 591751041
    printf("rotl(0x12345678, 4) = %d\n", rl);

    return 0;
}
