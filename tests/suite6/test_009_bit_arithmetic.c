int printf(const char *fmt, ...);

int add_without_plus(int a, int b) {
    while (b != 0) {
        int carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

int subtract_without_minus(int a, int b) {
    while (b != 0) {
        int borrow = (~a) & b;
        a = a ^ b;
        b = borrow << 1;
    }
    return a;
}

int multiply_by_shift(int a, int b) {
    int result = 0;
    int negative = 0;
    if (a < 0) { a = -a; negative = 1; }
    if (b < 0) { b = -b; negative = negative ^ 1; }
    while (b > 0) {
        if (b & 1) {
            result = result + a;
        }
        a = a << 1;
        b = (b >> 1) & 0x7FFFFFFF;
    }
    if (negative) result = -result;
    return result;
}

int abs_no_branch(int x) {
    int mask = x >> 31;
    return (x ^ mask) - mask;
}

int min_no_branch(int a, int b) {
    int diff = a - b;
    int sign = (diff >> 31) & 1;
    return b + (diff & (-sign));
}

int max_no_branch(int a, int b) {
    int diff = a - b;
    int sign = (diff >> 31) & 1;
    return a - (diff & (-sign));
}

int negate_without_minus(int x) {
    return ~x + 1;
}

int is_negative(int x) {
    return (x >> 31) & 1;
}

int sign_of(int x) {
    if (x == 0) return 0;
    return ((x >> 31) & 1) ? -1 : 1;
}

int divide_by_power_of_2(int x, int p) {
    return (x >> p) & (0x7FFFFFFF >> (p - 1));
}

int modulo_power_of_2(int x, int p) {
    return x & ((1 << p) - 1);
}

int is_even(int x) {
    return (x & 1) == 0;
}

int is_odd(int x) {
    return x & 1;
}

int avg_no_overflow(int a, int b) {
    return (a & b) + (((a ^ b) >> 1) & 0x7FFFFFFF);
}

int clamp(int x, int lo, int hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

int multiply_by_3(int x) {
    return (x << 1) + x;
}

int multiply_by_7(int x) {
    return (x << 3) - x;
}

int multiply_by_10(int x) {
    return (x << 3) + (x << 1);
}

int main(void) {
    // EXPECT: add(5, 3) = 8
    printf("add(5, 3) = %d\n", add_without_plus(5, 3));
    // EXPECT: add(0, 0) = 0
    printf("add(0, 0) = %d\n", add_without_plus(0, 0));
    // EXPECT: add(100, 200) = 300
    printf("add(100, 200) = %d\n", add_without_plus(100, 200));

    // EXPECT: sub(10, 3) = 7
    printf("sub(10, 3) = %d\n", subtract_without_minus(10, 3));
    // EXPECT: sub(5, 5) = 0
    printf("sub(5, 5) = %d\n", subtract_without_minus(5, 5));

    // EXPECT: mul(6, 7) = 42
    printf("mul(6, 7) = %d\n", multiply_by_shift(6, 7));
    // EXPECT: mul(0, 100) = 0
    printf("mul(0, 100) = %d\n", multiply_by_shift(0, 100));
    // EXPECT: mul(-3, 4) = -12
    printf("mul(-3, 4) = %d\n", multiply_by_shift(-3, 4));
    // EXPECT: mul(-3, -4) = 12
    printf("mul(-3, -4) = %d\n", multiply_by_shift(-3, -4));

    // EXPECT: abs(5) = 5
    printf("abs(5) = %d\n", abs_no_branch(5));
    // EXPECT: abs(-5) = 5
    printf("abs(-5) = %d\n", abs_no_branch(-5));
    // EXPECT: abs(0) = 0
    printf("abs(0) = %d\n", abs_no_branch(0));

    // EXPECT: min(3, 7) = 3
    printf("min(3, 7) = %d\n", min_no_branch(3, 7));
    // EXPECT: min(7, 3) = 3
    printf("min(7, 3) = %d\n", min_no_branch(7, 3));
    // EXPECT: max(3, 7) = 7
    printf("max(3, 7) = %d\n", max_no_branch(3, 7));
    // EXPECT: max(7, 3) = 7
    printf("max(7, 3) = %d\n", max_no_branch(7, 3));

    // EXPECT: neg(5) = -5
    printf("neg(5) = %d\n", negate_without_minus(5));
    // EXPECT: neg(-5) = 5
    printf("neg(-5) = %d\n", negate_without_minus(-5));

    // EXPECT: is_neg(-1) = 1
    printf("is_neg(-1) = %d\n", is_negative(-1));
    // EXPECT: is_neg(1) = 0
    printf("is_neg(1) = %d\n", is_negative(1));

    // EXPECT: sign(-5) = -1
    printf("sign(-5) = %d\n", sign_of(-5));
    // EXPECT: sign(5) = 1
    printf("sign(5) = %d\n", sign_of(5));
    // EXPECT: sign(0) = 0
    printf("sign(0) = %d\n", sign_of(0));

    // EXPECT: div_pow2(100, 2) = 25
    printf("div_pow2(100, 2) = %d\n", divide_by_power_of_2(100, 2));
    // EXPECT: mod_pow2(100, 3) = 4
    printf("mod_pow2(100, 3) = %d\n", modulo_power_of_2(100, 3));

    // EXPECT: is_even(4) = 1
    printf("is_even(4) = %d\n", is_even(4));
    // EXPECT: is_odd(5) = 1
    printf("is_odd(5) = %d\n", is_odd(5));

    // EXPECT: avg(10, 20) = 15
    printf("avg(10, 20) = %d\n", avg_no_overflow(10, 20));

    // EXPECT: mul3(7) = 21
    printf("mul3(7) = %d\n", multiply_by_3(7));
    // EXPECT: mul7(5) = 35
    printf("mul7(5) = %d\n", multiply_by_7(5));
    // EXPECT: mul10(6) = 60
    printf("mul10(6) = %d\n", multiply_by_10(6));

    return 0;
}
