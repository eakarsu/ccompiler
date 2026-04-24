int printf(const char *fmt, ...);

int is_power_of_two(int x) {
    if (x <= 0) return 0;
    return (x & (x - 1)) == 0;
}

int next_power_of_two(int x) {
    if (x <= 1) return 1;
    int p = 1;
    while (p < x) {
        p = p << 1;
    }
    return p;
}

int prev_power_of_two(int x) {
    if (x <= 0) return 0;
    int p = 1;
    while (p <= x) {
        p = p << 1;
    }
    return (p >> 1) & 0x7FFFFFFF;
}

int round_up_pow2(int x) {
    if (x <= 1) return 1;
    x = x - 1;
    x = x | ((x >> 1) & 0x7FFFFFFF);
    x = x | ((x >> 2) & 0x3FFFFFFF);
    x = x | ((x >> 4) & 0x0FFFFFFF);
    x = x | ((x >> 8) & 0x00FFFFFF);
    x = x | ((x >> 16) & 0x0000FFFF);
    return x + 1;
}

int round_down_pow2(int x) {
    if (x <= 0) return 0;
    x = x | ((x >> 1) & 0x7FFFFFFF);
    x = x | ((x >> 2) & 0x3FFFFFFF);
    x = x | ((x >> 4) & 0x0FFFFFFF);
    x = x | ((x >> 8) & 0x00FFFFFF);
    x = x | ((x >> 16) & 0x0000FFFF);
    return x - ((x >> 1) & 0x7FFFFFFF);
}

int log2_floor(int x) {
    if (x <= 0) return -1;
    int result = 0;
    while (x > 1) {
        x = (x >> 1) & 0x7FFFFFFF;
        result = result + 1;
    }
    return result;
}

int log2_ceil(int x) {
    if (x <= 0) return -1;
    int floor = log2_floor(x);
    if (is_power_of_two(x)) return floor;
    return floor + 1;
}

int align_up(int x, int alignment) {
    return (x + alignment - 1) & ~(alignment - 1);
}

int align_down(int x, int alignment) {
    return x & ~(alignment - 1);
}

int is_aligned(int x, int alignment) {
    return (x & (alignment - 1)) == 0;
}

int count_powers_of_two_upto(int n) {
    int count = 0;
    int p = 1;
    while (p <= n) {
        count = count + 1;
        p = p << 1;
    }
    return count;
}

int largest_power_of_two_dividing(int n) {
    if (n == 0) return 0;
    return n & (-n);
}

int sum_powers_of_two(int n) {
    int sum = 0;
    int p = 1;
    while (p <= n) {
        sum = sum + p;
        p = p << 1;
    }
    return sum;
}

int main(void) {
    // EXPECT: is_pow2(0) = 0
    printf("is_pow2(0) = %d\n", is_power_of_two(0));
    // EXPECT: is_pow2(1) = 1
    printf("is_pow2(1) = %d\n", is_power_of_two(1));
    // EXPECT: is_pow2(16) = 1
    printf("is_pow2(16) = %d\n", is_power_of_two(16));
    // EXPECT: is_pow2(15) = 0
    printf("is_pow2(15) = %d\n", is_power_of_two(15));
    // EXPECT: is_pow2(64) = 1
    printf("is_pow2(64) = %d\n", is_power_of_two(64));

    // EXPECT: next_pow2(5) = 8
    printf("next_pow2(5) = %d\n", next_power_of_two(5));
    // EXPECT: next_pow2(16) = 16
    printf("next_pow2(16) = %d\n", next_power_of_two(16));
    // EXPECT: next_pow2(1) = 1
    printf("next_pow2(1) = %d\n", next_power_of_two(1));

    // EXPECT: prev_pow2(15) = 8
    printf("prev_pow2(15) = %d\n", prev_power_of_two(15));
    // EXPECT: prev_pow2(16) = 16
    printf("prev_pow2(16) = %d\n", prev_power_of_two(16));

    // EXPECT: round_up(10) = 16
    printf("round_up(10) = %d\n", round_up_pow2(10));
    // EXPECT: round_up(32) = 32
    printf("round_up(32) = %d\n", round_up_pow2(32));
    // EXPECT: round_down(10) = 8
    printf("round_down(10) = %d\n", round_down_pow2(10));
    // EXPECT: round_down(32) = 32
    printf("round_down(32) = %d\n", round_down_pow2(32));

    // EXPECT: log2(1) = 0
    printf("log2(1) = %d\n", log2_floor(1));
    // EXPECT: log2(8) = 3
    printf("log2(8) = %d\n", log2_floor(8));
    // EXPECT: log2(10) = 3
    printf("log2(10) = %d\n", log2_floor(10));
    // EXPECT: log2_ceil(10) = 4
    printf("log2_ceil(10) = %d\n", log2_ceil(10));
    // EXPECT: log2_ceil(8) = 3
    printf("log2_ceil(8) = %d\n", log2_ceil(8));

    // EXPECT: align_up(7, 4) = 8
    printf("align_up(7, 4) = %d\n", align_up(7, 4));
    // EXPECT: align_down(7, 4) = 4
    printf("align_down(7, 4) = %d\n", align_down(7, 4));
    // EXPECT: is_aligned(8, 4) = 1
    printf("is_aligned(8, 4) = %d\n", is_aligned(8, 4));
    // EXPECT: is_aligned(7, 4) = 0
    printf("is_aligned(7, 4) = %d\n", is_aligned(7, 4));

    // EXPECT: count_pow2_upto(100) = 7
    printf("count_pow2_upto(100) = %d\n", count_powers_of_two_upto(100));
    // EXPECT: largest_pow2_div(12) = 4
    printf("largest_pow2_div(12) = %d\n", largest_power_of_two_dividing(12));
    // EXPECT: sum_pow2(100) = 127
    printf("sum_pow2(100) = %d\n", sum_powers_of_two(100));

    return 0;
}
