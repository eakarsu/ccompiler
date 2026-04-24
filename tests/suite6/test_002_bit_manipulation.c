int printf(const char *fmt, ...);

int set_bit(int x, int pos) {
    return x | (1 << pos);
}

int clear_bit(int x, int pos) {
    return x & ~(1 << pos);
}

int toggle_bit(int x, int pos) {
    return x ^ (1 << pos);
}

int get_bit(int x, int pos) {
    return (x >> pos) & 1;
}

int extract_field(int x, int start, int width) {
    int mask = (1 << width) - 1;
    return (x >> start) & mask;
}

int insert_field(int x, int val, int start, int width) {
    int mask = (1 << width) - 1;
    x = x & ~(mask << start);
    x = x | ((val & mask) << start);
    return x;
}

int rotate_left(int x, int n, int bits) {
    int mask = (1 << bits) - 1;
    x = x & mask;
    return ((x << n) | ((x >> (bits - n)) & ((1 << n) - 1))) & mask;
}

int rotate_right(int x, int n, int bits) {
    int mask = (1 << bits) - 1;
    x = x & mask;
    return (((x >> n) & ((1 << (bits - n)) - 1)) | (x << (bits - n))) & mask;
}

int reverse_bits_8(int x) {
    int result = 0;
    int i = 0;
    while (i < 8) {
        result = result << 1;
        result = result | (x & 1);
        x = (x >> 1) & 0x7FFFFFFF;
        i = i + 1;
    }
    return result;
}

int swap_nibbles(int x) {
    return ((x & 0x0F) << 4) | ((x >> 4) & 0x0F);
}

int isolate_rightmost_one(int x) {
    return x & (-x);
}

int turn_off_rightmost_one(int x) {
    return x & (x - 1);
}

int smear_rightmost_one(int x) {
    return x | (x - 1);
}

int set_bits_in_range(int x, int lo, int hi) {
    int i = lo;
    while (i <= hi) {
        x = x | (1 << i);
        i = i + 1;
    }
    return x;
}

int clear_bits_in_range(int x, int lo, int hi) {
    int i = lo;
    while (i <= hi) {
        x = x & ~(1 << i);
        i = i + 1;
    }
    return x;
}

int count_flips_to_convert(int a, int b) {
    int diff = a ^ b;
    int count = 0;
    while (diff) {
        count = count + (diff & 1);
        diff = (diff >> 1) & 0x7FFFFFFF;
    }
    return count;
}

int swap_even_odd_bits(int x) {
    int evens = x & 0x55555555;
    int odds = x & 0x2AAAAAAA;
    return (evens << 1) | ((odds >> 1) & 0x7FFFFFFF);
}

int main(void) {
    // EXPECT: set_bit(0, 3) = 8
    printf("set_bit(0, 3) = %d\n", set_bit(0, 3));
    // EXPECT: clear_bit(15, 2) = 11
    printf("clear_bit(15, 2) = %d\n", clear_bit(15, 2));
    // EXPECT: toggle_bit(10, 1) = 8
    printf("toggle_bit(10, 1) = %d\n", toggle_bit(10, 1));
    // EXPECT: toggle_bit(8, 1) = 10
    printf("toggle_bit(8, 1) = %d\n", toggle_bit(8, 1));
    // EXPECT: get_bit(10, 3) = 1
    printf("get_bit(10, 3) = %d\n", get_bit(10, 3));
    // EXPECT: get_bit(10, 2) = 0
    printf("get_bit(10, 2) = %d\n", get_bit(10, 2));

    // EXPECT: extract(0xABCD, 4, 8) = 188
    printf("extract(0xABCD, 4, 8) = %d\n", extract_field(0xABCD, 4, 8));
    // EXPECT: insert(0xFF, 5, 4, 4) = 95
    printf("insert(0xFF, 5, 4, 4) = %d\n", insert_field(0xFF, 5, 4, 4));

    // EXPECT: rot_left(0b1100, 2, 8) = 48
    printf("rot_left(0b1100, 2, 8) = %d\n", rotate_left(12, 2, 8));
    // EXPECT: rot_right(0b1100, 2, 8) = 3
    printf("rot_right(0b1100, 2, 8) = %d\n", rotate_right(12, 2, 8));

    // EXPECT: reverse_8(0b10110000) = 13
    printf("reverse_8(0b10110000) = %d\n", reverse_bits_8(0xB0));
    // EXPECT: reverse_8(0b11111111) = 255
    printf("reverse_8(0b11111111) = %d\n", reverse_bits_8(255));
    // EXPECT: reverse_8(1) = 128
    printf("reverse_8(1) = %d\n", reverse_bits_8(1));

    // EXPECT: swap_nibbles(0xAB) = 186
    printf("swap_nibbles(0xAB) = %d\n", swap_nibbles(0xAB));

    // EXPECT: isolate_right_1(12) = 4
    printf("isolate_right_1(12) = %d\n", isolate_rightmost_one(12));
    // EXPECT: turnoff_right_1(12) = 8
    printf("turnoff_right_1(12) = %d\n", turn_off_rightmost_one(12));
    // EXPECT: smear_right_1(12) = 15
    printf("smear_right_1(12) = %d\n", smear_rightmost_one(12));

    // EXPECT: set_range(0, 2, 5) = 60
    printf("set_range(0, 2, 5) = %d\n", set_bits_in_range(0, 2, 5));
    // EXPECT: clear_range(255, 2, 5) = 195
    printf("clear_range(255, 2, 5) = %d\n", clear_bits_in_range(255, 2, 5));

    // EXPECT: flips(29, 15) = 2
    printf("flips(29, 15) = %d\n", count_flips_to_convert(29, 15));

    // EXPECT: swap_eo(0b1010) = 5
    printf("swap_eo(0b1010) = %d\n", swap_even_odd_bits(10));

    return 0;
}
