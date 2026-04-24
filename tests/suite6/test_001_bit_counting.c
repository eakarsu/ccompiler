int printf(const char *fmt, ...);

int popcount(int x) {
    int count = 0;
    while (x) {
        count = count + (x & 1);
        x = (x >> 1) & 0x7FFFFFFF;
    }
    return count;
}

int count_leading_zeros(int x) {
    if (x == 0) return 32;
    int count = 0;
    int mask = 1 << 30;
    while (mask > 0) {
        if (x & mask) return count;
        count = count + 1;
        mask = (mask >> 1) & 0x7FFFFFFF;
    }
    if (x & 1) return 30;
    return 31;
}

int count_trailing_zeros(int x) {
    if (x == 0) return 32;
    int count = 0;
    while ((x & 1) == 0) {
        count = count + 1;
        x = (x >> 1) & 0x7FFFFFFF;
    }
    return count;
}

int parity(int x) {
    int p = 0;
    while (x) {
        p = p ^ (x & 1);
        x = (x >> 1) & 0x7FFFFFFF;
    }
    return p;
}

int hamming_distance(int a, int b) {
    return popcount(a ^ b);
}

int count_set_bits_range(int x, int lo, int hi) {
    int count = 0;
    int i = lo;
    while (i <= hi) {
        if (x & (1 << i)) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int highest_set_bit(int x) {
    if (x == 0) return -1;
    int pos = 0;
    int tmp = x;
    while (tmp > 1) {
        tmp = (tmp >> 1) & 0x7FFFFFFF;
        pos = pos + 1;
    }
    return pos;
}

int lowest_set_bit_pos(int x) {
    if (x == 0) return -1;
    int pos = 0;
    while ((x & 1) == 0) {
        x = (x >> 1) & 0x7FFFFFFF;
        pos = pos + 1;
    }
    return pos;
}

int is_bit_palindrome(int x, int nbits) {
    int i = 0;
    while (i < nbits / 2) {
        int left = (x >> (nbits - 1 - i)) & 1;
        int right = (x >> i) & 1;
        if (left != right) return 0;
        i = i + 1;
    }
    return 1;
}

int main(void) {
    // EXPECT: popcount(0) = 0
    printf("popcount(0) = %d\n", popcount(0));
    // EXPECT: popcount(1) = 1
    printf("popcount(1) = %d\n", popcount(1));
    // EXPECT: popcount(7) = 3
    printf("popcount(7) = %d\n", popcount(7));
    // EXPECT: popcount(255) = 8
    printf("popcount(255) = %d\n", popcount(255));
    // EXPECT: popcount(1023) = 10
    printf("popcount(1023) = %d\n", popcount(1023));

    // EXPECT: clz(1) = 30
    printf("clz(1) = %d\n", count_leading_zeros(1));
    // EXPECT: clz(16) = 26
    printf("clz(16) = %d\n", count_leading_zeros(16));
    // EXPECT: clz(0) = 32
    printf("clz(0) = %d\n", count_leading_zeros(0));

    // EXPECT: ctz(1) = 0
    printf("ctz(1) = %d\n", count_trailing_zeros(1));
    // EXPECT: ctz(8) = 3
    printf("ctz(8) = %d\n", count_trailing_zeros(8));
    // EXPECT: ctz(0) = 32
    printf("ctz(0) = %d\n", count_trailing_zeros(0));
    // EXPECT: ctz(48) = 4
    printf("ctz(48) = %d\n", count_trailing_zeros(48));

    // EXPECT: parity(7) = 1
    printf("parity(7) = %d\n", parity(7));
    // EXPECT: parity(3) = 0
    printf("parity(3) = %d\n", parity(3));
    // EXPECT: parity(0) = 0
    printf("parity(0) = %d\n", parity(0));

    // EXPECT: hamming(7, 0) = 3
    printf("hamming(7, 0) = %d\n", hamming_distance(7, 0));
    // EXPECT: hamming(15, 10) = 2
    printf("hamming(15, 10) = %d\n", hamming_distance(15, 10));

    // EXPECT: bits[0..3] of 0xAB = 3
    printf("bits[0..3] of 0xAB = %d\n", count_set_bits_range(0xAB, 0, 3));
    // EXPECT: bits[4..7] of 0xAB = 2
    printf("bits[4..7] of 0xAB = %d\n", count_set_bits_range(0xAB, 4, 7));

    // EXPECT: highest_set(0) = -1
    printf("highest_set(0) = %d\n", highest_set_bit(0));
    // EXPECT: highest_set(255) = 7
    printf("highest_set(255) = %d\n", highest_set_bit(255));
    // EXPECT: lowest_set(12) = 2
    printf("lowest_set(12) = %d\n", lowest_set_bit_pos(12));

    // EXPECT: palindrome(0b1001, 4) = 1
    printf("palindrome(0b1001, 4) = %d\n", is_bit_palindrome(9, 4));
    // EXPECT: palindrome(0b1010, 4) = 0
    printf("palindrome(0b1010, 4) = %d\n", is_bit_palindrome(10, 4));
    // EXPECT: palindrome(0b10101, 5) = 1
    printf("palindrome(0b10101, 5) = %d\n", is_bit_palindrome(21, 5));

    return 0;
}
