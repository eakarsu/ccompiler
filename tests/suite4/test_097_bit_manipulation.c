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

int check_bit(int x, int pos) {
    return (x >> pos) & 1;
}

int count_set_bits(int x) {
    int count = 0;
    /* Work with positive values, count up to 31 bits */
    int i;
    for (i = 0; i < 31; i = i + 1) {
        if ((x >> i) & 1) {
            count = count + 1;
        }
    }
    return count;
}

int is_power_of_two(int x) {
    if (x <= 0) return 0;
    return (x & (x - 1)) == 0;
}

int lowest_set_bit(int x) {
    if (x == 0) return -1;
    int pos = 0;
    while ((x & 1) == 0) {
        x = x >> 1;
        pos = pos + 1;
    }
    return pos;
}

int highest_set_bit(int x) {
    if (x <= 0) return -1;
    int pos = 0;
    while (x > 1) {
        x = x >> 1;
        pos = pos + 1;
    }
    return pos;
}

int swap_bits(int x, int i, int j) {
    int bi = (x >> i) & 1;
    int bj = (x >> j) & 1;
    if (bi != bj) {
        x = x ^ (1 << i);
        x = x ^ (1 << j);
    }
    return x;
}

int reverse_low_byte(int x) {
    int result = 0;
    int i;
    for (i = 0; i < 8; i = i + 1) {
        if ((x >> i) & 1) {
            result = result | (1 << (7 - i));
        }
    }
    return result;
}

int rotate_left(int x, int n, int width) {
    /* Rotate the low 'width' bits left by n positions */
    int mask = (1 << width) - 1;
    x = x & mask;
    n = n % width;
    return ((x << n) | (x >> (width - n))) & mask;
}

int main(void) {
    /* Test set_bit */
    // EXPECT: set_bit(0, 3): 8
    printf("set_bit(0, 3): %d\n", set_bit(0, 3));
    // EXPECT: set_bit(5, 1): 7
    printf("set_bit(5, 1): %d\n", set_bit(5, 1));  /* 101 | 010 = 111 = 7 */

    /* Test clear_bit */
    // EXPECT: clear_bit(7, 1): 5
    printf("clear_bit(7, 1): %d\n", clear_bit(7, 1));  /* 111 & ~010 = 101 = 5 */
    // EXPECT: clear_bit(15, 3): 7
    printf("clear_bit(15, 3): %d\n", clear_bit(15, 3));  /* 1111 & ~1000 = 0111 = 7 */

    /* Test toggle_bit */
    // EXPECT: toggle_bit(5, 1): 7
    printf("toggle_bit(5, 1): %d\n", toggle_bit(5, 1));  /* 101 ^ 010 = 111 = 7 */
    // EXPECT: toggle_bit(7, 1): 5
    printf("toggle_bit(7, 1): %d\n", toggle_bit(7, 1));  /* 111 ^ 010 = 101 = 5 */

    /* Test check_bit */
    // EXPECT: check_bit(5, 0): 1
    printf("check_bit(5, 0): %d\n", check_bit(5, 0));  /* 101, bit 0 = 1 */
    // EXPECT: check_bit(5, 1): 0
    printf("check_bit(5, 1): %d\n", check_bit(5, 1));  /* 101, bit 1 = 0 */
    // EXPECT: check_bit(5, 2): 1
    printf("check_bit(5, 2): %d\n", check_bit(5, 2));  /* 101, bit 2 = 1 */

    /* Test count_set_bits */
    // EXPECT: bits in 0: 0
    printf("bits in 0: %d\n", count_set_bits(0));
    // EXPECT: bits in 7: 3
    printf("bits in 7: %d\n", count_set_bits(7));  /* 111 = 3 bits */
    // EXPECT: bits in 255: 8
    printf("bits in 255: %d\n", count_set_bits(255));  /* 11111111 = 8 bits */
    // EXPECT: bits in 1023: 10
    printf("bits in 1023: %d\n", count_set_bits(1023));  /* 10 bits all set */

    /* Test is_power_of_two */
    // EXPECT: pow2(1): 1
    printf("pow2(1): %d\n", is_power_of_two(1));
    // EXPECT: pow2(2): 1
    printf("pow2(2): %d\n", is_power_of_two(2));
    // EXPECT: pow2(3): 0
    printf("pow2(3): %d\n", is_power_of_two(3));
    // EXPECT: pow2(16): 1
    printf("pow2(16): %d\n", is_power_of_two(16));
    // EXPECT: pow2(0): 0
    printf("pow2(0): %d\n", is_power_of_two(0));
    // EXPECT: pow2(64): 1
    printf("pow2(64): %d\n", is_power_of_two(64));

    /* Test lowest_set_bit */
    // EXPECT: lsb(12): 2
    printf("lsb(12): %d\n", lowest_set_bit(12));  /* 1100, lowest set at pos 2 */
    // EXPECT: lsb(1): 0
    printf("lsb(1): %d\n", lowest_set_bit(1));
    // EXPECT: lsb(8): 3
    printf("lsb(8): %d\n", lowest_set_bit(8));

    /* Test highest_set_bit */
    // EXPECT: hsb(1): 0
    printf("hsb(1): %d\n", highest_set_bit(1));
    // EXPECT: hsb(8): 3
    printf("hsb(8): %d\n", highest_set_bit(8));
    // EXPECT: hsb(255): 7
    printf("hsb(255): %d\n", highest_set_bit(255));

    /* Test swap_bits */
    int val = 5;  /* 101 */
    val = swap_bits(val, 0, 1);  /* swap bit 0 and 1: 101 -> 110 = 6 */
    // EXPECT: swap(5, 0,1): 6
    printf("swap(5, 0,1): %d\n", val);

    /* Test reverse_low_byte */
    // EXPECT: reverse(1): 128
    printf("reverse(1): %d\n", reverse_low_byte(1));  /* 00000001 -> 10000000 = 128 */
    // EXPECT: reverse(3): 192
    printf("reverse(3): %d\n", reverse_low_byte(3));  /* 00000011 -> 11000000 = 192 */
    // EXPECT: reverse(170): 85
    printf("reverse(170): %d\n", reverse_low_byte(170));  /* 10101010 -> 01010101 = 85 */

    /* Test rotate_left */
    // EXPECT: rotl(1, 3, 8): 8
    printf("rotl(1, 3, 8): %d\n", rotate_left(1, 3, 8));  /* 00000001 -> 00001000 = 8 */
    // EXPECT: rotl(128, 1, 8): 1
    printf("rotl(128, 1, 8): %d\n", rotate_left(128, 1, 8));  /* 10000000 -> 00000001 = 1 */

    /* Build a bitmask pattern */
    int mask = 0;
    int i;
    for (i = 0; i < 8; i = i + 2) {
        mask = set_bit(mask, i);
    }
    /* bits 0, 2, 4, 6 set = 1+4+16+64 = 85 */
    // EXPECT: Alternating mask: 85
    printf("Alternating mask: %d\n", mask);
    // EXPECT: Bits in mask: 4
    printf("Bits in mask: %d\n", count_set_bits(mask));

    return 0;
}
