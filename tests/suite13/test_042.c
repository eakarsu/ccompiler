int printf(const char *fmt, ...);

// Power of 2 checks and next power of 2

int is_power_of_2(int x) {
    if (x <= 0) return 0;
    return (x & (x - 1)) == 0;
}

int next_power_of_2(int x) {
    if (x <= 0) return 1;
    if (is_power_of_2(x)) return x;
    int result = 1;
    while (result < x) {
        result = result << 1;
    }
    return result;
}

int prev_power_of_2(int x) {
    if (x <= 0) return 0;
    int result = 1;
    while (result <= x) {
        result = result << 1;
    }
    // result is now > x, go back one step
    return (result >> 1) & 0x7FFFFFFF;
}

int log2_floor(int x) {
    if (x <= 0) return -1;
    int count = 0;
    int val = x;
    while (val > 1) {
        val = (val >> 1) & 0x7FFFFFFF;
        count = count + 1;
    }
    return count;
}

int log2_ceil(int x) {
    if (x <= 0) return -1;
    int f = log2_floor(x);
    if ((1 << f) == x) return f;
    return f + 1;
}

// Round up to next multiple of a power of 2
int align_up(int val, int alignment) {
    int mask = alignment - 1;
    return (val + mask) & (~mask);
}

// Count powers of 2 up to n
int count_powers(int n) {
    int count = 0;
    int p = 1;
    while (p <= n) {
        count = count + 1;
        p = p << 1;
    }
    return count;
}

// Check if exactly two bits are set
int has_two_bits(int x) {
    if (x == 0) return 0;
    int cleared = x & (x - 1);
    if (cleared == 0) return 0;
    return (cleared & (cleared - 1)) == 0;
}

// Get the position of the single set bit, or -1
int single_bit_pos(int x) {
    if (!is_power_of_2(x)) return -1;
    return log2_floor(x);
}

int main() {
    // is_power_of_2
    printf("p2(0)=%d\n", is_power_of_2(0));
    // EXPECT: p2(0)=0
    printf("p2(1)=%d\n", is_power_of_2(1));
    // EXPECT: p2(1)=1
    printf("p2(2)=%d\n", is_power_of_2(2));
    // EXPECT: p2(2)=1
    printf("p2(3)=%d\n", is_power_of_2(3));
    // EXPECT: p2(3)=0
    printf("p2(4)=%d\n", is_power_of_2(4));
    // EXPECT: p2(4)=1
    printf("p2(5)=%d\n", is_power_of_2(5));
    // EXPECT: p2(5)=0
    printf("p2(16)=%d\n", is_power_of_2(16));
    // EXPECT: p2(16)=1
    printf("p2(1024)=%d\n", is_power_of_2(1024));
    // EXPECT: p2(1024)=1
    printf("p2(1000)=%d\n", is_power_of_2(1000));
    // EXPECT: p2(1000)=0

    // next_power_of_2
    printf("np2(0)=%d\n", next_power_of_2(0));
    // EXPECT: np2(0)=1
    printf("np2(1)=%d\n", next_power_of_2(1));
    // EXPECT: np2(1)=1
    printf("np2(3)=%d\n", next_power_of_2(3));
    // EXPECT: np2(3)=4
    printf("np2(5)=%d\n", next_power_of_2(5));
    // EXPECT: np2(5)=8
    printf("np2(7)=%d\n", next_power_of_2(7));
    // EXPECT: np2(7)=8
    printf("np2(8)=%d\n", next_power_of_2(8));
    // EXPECT: np2(8)=8
    printf("np2(9)=%d\n", next_power_of_2(9));
    // EXPECT: np2(9)=16
    printf("np2(100)=%d\n", next_power_of_2(100));
    // EXPECT: np2(100)=128
    printf("np2(1000)=%d\n", next_power_of_2(1000));
    // EXPECT: np2(1000)=1024

    // prev_power_of_2
    printf("pp2(1)=%d\n", prev_power_of_2(1));
    // EXPECT: pp2(1)=1
    printf("pp2(3)=%d\n", prev_power_of_2(3));
    // EXPECT: pp2(3)=2
    printf("pp2(5)=%d\n", prev_power_of_2(5));
    // EXPECT: pp2(5)=4
    printf("pp2(8)=%d\n", prev_power_of_2(8));
    // EXPECT: pp2(8)=8
    printf("pp2(100)=%d\n", prev_power_of_2(100));
    // EXPECT: pp2(100)=64
    printf("pp2(1000)=%d\n", prev_power_of_2(1000));
    // EXPECT: pp2(1000)=512

    // log2
    printf("lg2(1)=%d\n", log2_floor(1));
    // EXPECT: lg2(1)=0
    printf("lg2(2)=%d\n", log2_floor(2));
    // EXPECT: lg2(2)=1
    printf("lg2(4)=%d\n", log2_floor(4));
    // EXPECT: lg2(4)=2
    printf("lg2(7)=%d\n", log2_floor(7));
    // EXPECT: lg2(7)=2
    printf("lg2(8)=%d\n", log2_floor(8));
    // EXPECT: lg2(8)=3
    printf("lg2(1024)=%d\n", log2_floor(1024));
    // EXPECT: lg2(1024)=10

    // log2_ceil
    printf("lc2(1)=%d\n", log2_ceil(1));
    // EXPECT: lc2(1)=0
    printf("lc2(2)=%d\n", log2_ceil(2));
    // EXPECT: lc2(2)=1
    printf("lc2(3)=%d\n", log2_ceil(3));
    // EXPECT: lc2(3)=2
    printf("lc2(5)=%d\n", log2_ceil(5));
    // EXPECT: lc2(5)=3
    printf("lc2(8)=%d\n", log2_ceil(8));
    // EXPECT: lc2(8)=3
    printf("lc2(9)=%d\n", log2_ceil(9));
    // EXPECT: lc2(9)=4

    // align_up
    printf("al(0,4)=%d\n", align_up(0, 4));
    // EXPECT: al(0,4)=0
    printf("al(1,4)=%d\n", align_up(1, 4));
    // EXPECT: al(1,4)=4
    printf("al(4,4)=%d\n", align_up(4, 4));
    // EXPECT: al(4,4)=4
    printf("al(5,8)=%d\n", align_up(5, 8));
    // EXPECT: al(5,8)=8
    printf("al(13,16)=%d\n", align_up(13, 16));
    // EXPECT: al(13,16)=16
    printf("al(17,16)=%d\n", align_up(17, 16));
    // EXPECT: al(17,16)=32

    // count_powers
    printf("cp(1)=%d\n", count_powers(1));
    // EXPECT: cp(1)=1
    printf("cp(10)=%d\n", count_powers(10));
    // EXPECT: cp(10)=4
    printf("cp(100)=%d\n", count_powers(100));
    // EXPECT: cp(100)=7
    printf("cp(1000)=%d\n", count_powers(1000));
    // EXPECT: cp(1000)=10

    // has_two_bits
    printf("h2(0)=%d\n", has_two_bits(0));
    // EXPECT: h2(0)=0
    printf("h2(1)=%d\n", has_two_bits(1));
    // EXPECT: h2(1)=0
    printf("h2(3)=%d\n", has_two_bits(3));
    // EXPECT: h2(3)=1
    printf("h2(5)=%d\n", has_two_bits(5));
    // EXPECT: h2(5)=1
    printf("h2(7)=%d\n", has_two_bits(7));
    // EXPECT: h2(7)=0
    printf("h2(6)=%d\n", has_two_bits(6));
    // EXPECT: h2(6)=1

    // single_bit_pos
    printf("sb(1)=%d\n", single_bit_pos(1));
    // EXPECT: sb(1)=0
    printf("sb(4)=%d\n", single_bit_pos(4));
    // EXPECT: sb(4)=2
    printf("sb(3)=%d\n", single_bit_pos(3));
    // EXPECT: sb(3)=-1
    printf("sb(64)=%d\n", single_bit_pos(64));
    // EXPECT: sb(64)=6

    printf("done\n");
    // EXPECT: done
    return 0;
}
