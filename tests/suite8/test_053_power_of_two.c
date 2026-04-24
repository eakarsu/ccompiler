int printf(const char *fmt, ...);

int is_power_of_2(int x) {
    if (x <= 0) return 0;
    return (x & (x - 1)) == 0;
}

int next_power_of_2(int x) {
    int p;
    if (x <= 1) return 1;
    p = 1;
    while (p < x) {
        p = p << 1;
    }
    return p;
}

int prev_power_of_2(int x) {
    int p;
    if (x <= 0) return 0;
    p = 1;
    while (p <= x) {
        p = p << 1;
    }
    return p >> 1;
}

int log2_floor(int x) {
    int result;
    if (x <= 0) return -1;
    result = 0;
    while (x > 1) {
        x = x >> 1;
        result = result + 1;
    }
    return result;
}

int log2_ceil(int x) {
    int f;
    if (x <= 0) return -1;
    f = log2_floor(x);
    if ((1 << f) == x) return f;
    return f + 1;
}

int highest_set_bit(int x) {
    int pos;
    if (x == 0) return -1;
    pos = 0;
    while (x > 1) {
        x = x >> 1;
        pos = pos + 1;
    }
    return pos;
}

int lowest_set_bit(int x) {
    int pos;
    if (x == 0) return -1;
    pos = 0;
    while ((x & 1) == 0) {
        x = x >> 1;
        pos = pos + 1;
    }
    return pos;
}

int isolate_lowest_set(int x) {
    return x & (-x);
}

int turn_off_lowest_set(int x) {
    return x & (x - 1);
}

int main() {
    int i;
    int val;

    printf("=== Power of 2 Check ===\n");
    // EXPECT: === Power of 2 Check ===

    for (i = 0; i <= 16; i = i + 1) {
        if (i > 0) printf(" ");
        printf("is_pow2(%d)=%d", i, is_power_of_2(i));
    }
    printf("\n");
    // EXPECT: is_pow2(0)=0 is_pow2(1)=1 is_pow2(2)=1 is_pow2(3)=0 is_pow2(4)=1 is_pow2(5)=0 is_pow2(6)=0 is_pow2(7)=0 is_pow2(8)=1 is_pow2(9)=0 is_pow2(10)=0 is_pow2(11)=0 is_pow2(12)=0 is_pow2(13)=0 is_pow2(14)=0 is_pow2(15)=0 is_pow2(16)=1

    printf("=== Next Power of 2 ===\n");
    // EXPECT: === Next Power of 2 ===

    printf("next(%d)=%d\n", 0, next_power_of_2(0));
    // EXPECT: next(0)=1
    printf("next(%d)=%d\n", 1, next_power_of_2(1));
    // EXPECT: next(1)=1
    printf("next(%d)=%d\n", 3, next_power_of_2(3));
    // EXPECT: next(3)=4
    printf("next(%d)=%d\n", 5, next_power_of_2(5));
    // EXPECT: next(5)=8
    printf("next(%d)=%d\n", 17, next_power_of_2(17));
    // EXPECT: next(17)=32
    printf("next(%d)=%d\n", 1000, next_power_of_2(1000));
    // EXPECT: next(1000)=1024

    printf("=== Prev Power of 2 ===\n");
    // EXPECT: === Prev Power of 2 ===

    printf("prev(%d)=%d\n", 1, prev_power_of_2(1));
    // EXPECT: prev(1)=1
    printf("prev(%d)=%d\n", 7, prev_power_of_2(7));
    // EXPECT: prev(7)=4
    printf("prev(%d)=%d\n", 16, prev_power_of_2(16));
    // EXPECT: prev(16)=16
    printf("prev(%d)=%d\n", 100, prev_power_of_2(100));
    // EXPECT: prev(100)=64

    printf("=== Log2 ===\n");
    // EXPECT: === Log2 ===

    printf("log2f(%d)=%d log2c(%d)=%d\n", 1, log2_floor(1), 1, log2_ceil(1));
    // EXPECT: log2f(1)=0 log2c(1)=0
    printf("log2f(%d)=%d log2c(%d)=%d\n", 8, log2_floor(8), 8, log2_ceil(8));
    // EXPECT: log2f(8)=3 log2c(8)=3
    printf("log2f(%d)=%d log2c(%d)=%d\n", 10, log2_floor(10), 10, log2_ceil(10));
    // EXPECT: log2f(10)=3 log2c(10)=4
    printf("log2f(%d)=%d log2c(%d)=%d\n", 1024, log2_floor(1024), 1024, log2_ceil(1024));
    // EXPECT: log2f(1024)=10 log2c(1024)=10

    printf("=== Highest/Lowest Set Bit ===\n");
    // EXPECT: === Highest/Lowest Set Bit ===

    printf("hsb(%d)=%d lsb(%d)=%d\n", 12, highest_set_bit(12), 12, lowest_set_bit(12));
    // EXPECT: hsb(12)=3 lsb(12)=2
    printf("hsb(%d)=%d lsb(%d)=%d\n", 48, highest_set_bit(48), 48, lowest_set_bit(48));
    // EXPECT: hsb(48)=5 lsb(48)=4

    printf("=== Isolate/TurnOff Lowest ===\n");
    // EXPECT: === Isolate/TurnOff Lowest ===

    printf("iso(%d)=%d off(%d)=%d\n", 12, isolate_lowest_set(12), 12, turn_off_lowest_set(12));
    // EXPECT: iso(12)=4 off(12)=8
    printf("iso(%d)=%d off(%d)=%d\n", 80, isolate_lowest_set(80), 80, turn_off_lowest_set(80));
    // EXPECT: iso(80)=16 off(80)=64

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
