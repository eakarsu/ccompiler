int printf(const char *fmt, ...);

// Test bitwise operators: AND, OR, XOR, NOT, left shift, right shift.

int bit_and(int a, int b) { return a & b; }
int bit_or(int a, int b) { return a | b; }
int bit_xor(int a, int b) { return a ^ b; }
int bit_not(int a) { return ~a; }
int shl(int a, int n) { return a << n; }
int shr(int a, int n) { return a >> n; }

int set_bit(int val, int bit) {
    return val | (1 << bit);
}

int clear_bit(int val, int bit) {
    return val & ~(1 << bit);
}

int toggle_bit(int val, int bit) {
    return val ^ (1 << bit);
}

int test_bit(int val, int bit) {
    return (val >> bit) & 1;
}

int count_leading_zeros(int val) {
    if (val == 0) return 32;
    int count = 0;
    int i;
    for (i = 31; i >= 0; i = i - 1) {
        if ((val >> i) & 1) return count;
        count = count + 1;
    }
    return count;
}

int count_trailing_zeros(int val) {
    if (val == 0) return 32;
    int count = 0;
    int i;
    for (i = 0; i < 32; i = i + 1) {
        if ((val >> i) & 1) return count;
        count = count + 1;
    }
    return count;
}

int is_power_of_two(int val) {
    if (val <= 0) return 0;
    return (val & (val - 1)) == 0;
}

int next_power_of_two(int val) {
    if (val <= 1) return 1;
    int p = 1;
    while (p < val) {
        p = p << 1;
    }
    return p;
}

int swap_xor(int a, int b) {
    // XOR swap returns second value (demonstrates XOR properties)
    a = a ^ b;
    b = a ^ b;  // b = original a
    a = a ^ b;  // a = original b
    return a;  // returns original b
}

int main(void) {
    // Basic AND
    // EXPECT: 12 & 10 = 8
    printf("%d & %d = %d\n", 12, 10, bit_and(12, 10));

    // AND with 0
    // EXPECT: 255 & 0 = 0
    printf("%d & %d = %d\n", 255, 0, bit_and(255, 0));

    // AND masking
    // EXPECT: 0xFF & 0x0F = 15
    printf("0xFF & 0x0F = %d\n", bit_and(0xFF, 0x0F));

    // Basic OR
    // EXPECT: 12 | 10 = 14
    printf("%d | %d = %d\n", 12, 10, bit_or(12, 10));

    // OR with 0
    // EXPECT: 42 | 0 = 42
    printf("%d | %d = %d\n", 42, 0, bit_or(42, 0));

    // Basic XOR
    // EXPECT: 12 ^ 10 = 6
    printf("%d ^ %d = %d\n", 12, 10, bit_xor(12, 10));

    // XOR with self = 0
    // EXPECT: 42 ^ 42 = 0
    printf("%d ^ %d = %d\n", 42, 42, bit_xor(42, 42));

    // XOR with 0 = identity
    // EXPECT: 42 ^ 0 = 42
    printf("%d ^ %d = %d\n", 42, 0, bit_xor(42, 0));

    // NOT
    // EXPECT: ~0 = -1
    printf("~%d = %d\n", 0, bit_not(0));

    // EXPECT: ~-1 = 0
    printf("~%d = %d\n", -1, bit_not(-1));

    // EXPECT: ~1 = -2
    printf("~%d = %d\n", 1, bit_not(1));

    // Left shift
    // EXPECT: 1 << 0 = 1
    printf("%d << %d = %d\n", 1, 0, shl(1, 0));

    // EXPECT: 1 << 1 = 2
    printf("%d << %d = %d\n", 1, 1, shl(1, 1));

    // EXPECT: 1 << 10 = 1024
    printf("%d << %d = %d\n", 1, 10, shl(1, 10));

    // EXPECT: 5 << 3 = 40
    printf("%d << %d = %d\n", 5, 3, shl(5, 3));

    // Right shift (arithmetic for signed)
    // EXPECT: 1024 >> 3 = 128
    printf("%d >> %d = %d\n", 1024, 3, shr(1024, 3));

    // EXPECT: 100 >> 1 = 50
    printf("%d >> %d = %d\n", 100, 1, shr(100, 1));

    // Arithmetic right shift preserves sign
    // EXPECT: -1 >> 1 = -1
    printf("%d >> %d = %d\n", -1, 1, shr(-1, 1));

    // Set bit
    // EXPECT: set_bit(0, 3) = 8
    printf("set_bit(0, 3) = %d\n", set_bit(0, 3));

    // EXPECT: set_bit(1, 3) = 9
    printf("set_bit(1, 3) = %d\n", set_bit(1, 3));

    // Clear bit
    // EXPECT: clear_bit(15, 2) = 11
    printf("clear_bit(15, 2) = %d\n", clear_bit(15, 2));

    // Toggle bit
    // EXPECT: toggle_bit(0, 5) = 32
    printf("toggle_bit(0, 5) = %d\n", toggle_bit(0, 5));

    // EXPECT: toggle_bit(32, 5) = 0
    printf("toggle_bit(32, 5) = %d\n", toggle_bit(32, 5));

    // Test bit
    // EXPECT: test_bit(10, 1) = 1
    printf("test_bit(10, 1) = %d\n", test_bit(10, 1));

    // EXPECT: test_bit(10, 2) = 0
    printf("test_bit(10, 2) = %d\n", test_bit(10, 2));

    // Leading zeros
    // EXPECT: clz(1) = 31
    printf("clz(1) = %d\n", count_leading_zeros(1));

    // EXPECT: clz(256) = 23
    printf("clz(256) = %d\n", count_leading_zeros(256));

    // Trailing zeros
    // EXPECT: ctz(8) = 3
    printf("ctz(8) = %d\n", count_trailing_zeros(8));

    // EXPECT: ctz(1) = 0
    printf("ctz(1) = %d\n", count_trailing_zeros(1));

    // Power of two check
    // EXPECT: is_pow2(1) = 1
    printf("is_pow2(1) = %d\n", is_power_of_two(1));

    // EXPECT: is_pow2(16) = 1
    printf("is_pow2(16) = %d\n", is_power_of_two(16));

    // EXPECT: is_pow2(15) = 0
    printf("is_pow2(15) = %d\n", is_power_of_two(15));

    // EXPECT: is_pow2(0) = 0
    printf("is_pow2(0) = %d\n", is_power_of_two(0));

    // Next power of two
    // EXPECT: next_pow2(5) = 8
    printf("next_pow2(5) = %d\n", next_power_of_two(5));

    // EXPECT: next_pow2(16) = 16
    printf("next_pow2(16) = %d\n", next_power_of_two(16));

    // EXPECT: next_pow2(17) = 32
    printf("next_pow2(17) = %d\n", next_power_of_two(17));

    // XOR swap: swap(10, 20) returns original b=20, but we return a which is now 20
    // Actually: after swap, a=original_b. We return a.
    // swap_xor(10, 20): returns 20
    // EXPECT: xor_swap(10,20) = 20
    printf("xor_swap(10,20) = %d\n", swap_xor(10, 20));

    return 0;
}
