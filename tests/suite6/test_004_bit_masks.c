int printf(const char *fmt, ...);

int create_mask(int width) {
    if (width >= 31) return 0x7FFFFFFF;
    return (1 << width) - 1;
}

int create_mask_range(int lo, int hi) {
    int mask = 0;
    int i = lo;
    while (i <= hi) {
        mask = mask | (1 << i);
        i = i + 1;
    }
    return mask;
}

int extract_with_mask(int val, int mask) {
    int result = val & mask;
    while (mask && ((mask & 1) == 0)) {
        result = (result >> 1) & 0x7FFFFFFF;
        mask = (mask >> 1) & 0x7FFFFFFF;
    }
    return result;
}

int insert_with_mask(int orig, int val, int mask) {
    int shift = 0;
    int tmp = mask;
    while (tmp && ((tmp & 1) == 0)) {
        shift = shift + 1;
        tmp = (tmp >> 1) & 0x7FFFFFFF;
    }
    orig = orig & ~mask;
    orig = orig | ((val << shift) & mask);
    return orig;
}

int sign_extend(int val, int bits) {
    int shift = 32 - bits;
    return (val << shift) >> shift;
}

int zero_extend(int val, int bits) {
    return val & ((1 << bits) - 1);
}

int count_mask_bits(int mask) {
    int count = 0;
    while (mask) {
        count = count + (mask & 1);
        mask = (mask >> 1) & 0x7FFFFFFF;
    }
    return count;
}

int lowest_zero_bit(int x) {
    return ~x & (x + 1);
}

int propagate_rightmost_one(int x) {
    return x | (x - 1);
}

int isolate_rightmost_zero(int x) {
    return ~x & (x + 1);
}

int mask_trailing_zeros(int x) {
    return (x & (-x)) - 1;
}

int byte_extract(int x, int byte_num) {
    return (x >> (byte_num * 8)) & 0xFF;
}

int byte_insert(int x, int byte_val, int byte_num) {
    int mask = 0xFF << (byte_num * 8);
    x = x & ~mask;
    x = x | ((byte_val & 0xFF) << (byte_num * 8));
    return x;
}

int merge_bits(int a, int b, int mask) {
    return (a & ~mask) | (b & mask);
}

int spread_bits(int x, int n) {
    int result = 0;
    int i = 0;
    while (i < 8) {
        if (x & (1 << i)) {
            result = result | (1 << (i * n));
        }
        i = i + 1;
    }
    return result;
}

int main(void) {
    // EXPECT: mask(4) = 15
    printf("mask(4) = %d\n", create_mask(4));
    // EXPECT: mask(8) = 255
    printf("mask(8) = %d\n", create_mask(8));
    // EXPECT: mask(1) = 1
    printf("mask(1) = %d\n", create_mask(1));

    // EXPECT: mask_range(2,5) = 60
    printf("mask_range(2,5) = %d\n", create_mask_range(2, 5));
    // EXPECT: mask_range(0,7) = 255
    printf("mask_range(0,7) = %d\n", create_mask_range(0, 7));

    // EXPECT: extract(0xABCD, 0x0F00) = 11
    printf("extract(0xABCD, 0x0F00) = %d\n", extract_with_mask(0xABCD, 0x0F00));
    // EXPECT: extract(0xABCD, 0x00F0) = 12
    printf("extract(0xABCD, 0x00F0) = %d\n", extract_with_mask(0xABCD, 0x00F0));

    // EXPECT: insert(0xFF00, 5, 0x00F0) = 65360
    printf("insert(0xFF00, 5, 0x00F0) = %d\n", insert_with_mask(0xFF00, 5, 0x00F0));

    // EXPECT: sign_ext(0xF, 4) = -1
    printf("sign_ext(0xF, 4) = %d\n", sign_extend(0xF, 4));
    // EXPECT: sign_ext(0x7, 4) = 7
    printf("sign_ext(0x7, 4) = %d\n", sign_extend(0x7, 4));
    // EXPECT: sign_ext(0x80, 8) = -128
    printf("sign_ext(0x80, 8) = %d\n", sign_extend(0x80, 8));

    // EXPECT: zero_ext(0xFF, 4) = 15
    printf("zero_ext(0xFF, 4) = %d\n", zero_extend(0xFF, 4));

    // EXPECT: count_mask(0xFF) = 8
    printf("count_mask(0xFF) = %d\n", count_mask_bits(0xFF));
    // EXPECT: count_mask(0xAA) = 4
    printf("count_mask(0xAA) = %d\n", count_mask_bits(0xAA));

    // EXPECT: lowest_zero(6) = 1
    printf("lowest_zero(6) = %d\n", lowest_zero_bit(6));
    // EXPECT: isolate_rz(6) = 1
    printf("isolate_rz(6) = %d\n", isolate_rightmost_zero(6));

    // EXPECT: byte(0x12345678, 0) = 120
    printf("byte(0x12345678, 0) = %d\n", byte_extract(0x12345678, 0));
    // EXPECT: byte(0x12345678, 1) = 86
    printf("byte(0x12345678, 1) = %d\n", byte_extract(0x12345678, 1));

    // EXPECT: byte_ins(0, 255, 1) = 65280
    printf("byte_ins(0, 255, 1) = %d\n", byte_insert(0, 255, 1));

    // EXPECT: merge(0xFF, 0x00, 0x0F) = 240
    printf("merge(0xFF, 0x00, 0x0F) = %d\n", merge_bits(0xFF, 0x00, 0x0F));
    // EXPECT: merge(0xFF, 0x00, 0xF0) = 15
    printf("merge(0xFF, 0x00, 0xF0) = %d\n", merge_bits(0xFF, 0x00, 0xF0));

    // EXPECT: spread(5, 2) = 17
    printf("spread(5, 2) = %d\n", spread_bits(5, 2));

    return 0;
}
