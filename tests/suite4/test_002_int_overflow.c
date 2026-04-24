int printf(const char *fmt, ...);

// Test integer overflow behavior, wrap-around, large multiplications,
// and division edge cases. Assumes 32-bit int with two's complement.

int max_int(void) {
    return 2147483647;
}

int min_int(void) {
    // -2147483648 expressed as calculation to avoid literal issues
    return -2147483647 - 1;
}

int multiply_large(int a, int b) {
    return a * b;
}

int add_checked(int a, int b) {
    return a + b;
}

int negate(int x) {
    return -x;
}

int div_cases(int a, int b) {
    return a / b;
}

int mod_cases(int a, int b) {
    return a % b;
}

int chain_add(int start, int step, int count) {
    int val = start;
    int i;
    for (i = 0; i < count; i = i + 1) {
        val = val + step;
    }
    return val;
}

int accumulate_mul(int base, int times) {
    int result = 1;
    int i;
    for (i = 0; i < times; i = i + 1) {
        result = result * base;
    }
    return result;
}

int int_size_bits(void) {
    int bits = 0;
    int v = 1;
    while (v > 0) {
        v = v + v;
        bits = bits + 1;
    }
    return bits + 1;
}

int main(void) {
    int imax = max_int();
    int imin = min_int();

    // Max int value
    // EXPECT: max_int = 2147483647
    printf("max_int = %d\n", imax);

    // Min int value
    // EXPECT: min_int = -2147483648
    printf("min_int = %d\n", imin);

    // Overflow: max + 1 wraps to min
    // EXPECT: max+1 = -2147483648
    printf("max+1 = %d\n", add_checked(imax, 1));

    // Underflow: min - 1 wraps to max
    // EXPECT: min-1 = 2147483647
    printf("min-1 = %d\n", add_checked(imin, -1));

    // Large multiplication that stays in range
    // EXPECT: 46340 * 46340 = 2147395600
    printf("46340 * 46340 = %d\n", multiply_large(46340, 46340));

    // Division: large / 1
    // EXPECT: 2147483647 / 1 = 2147483647
    printf("%d / 1 = %d\n", imax, div_cases(imax, 1));

    // Division: -1 / 1
    // EXPECT: -1 / 1 = -1
    printf("-1 / 1 = %d\n", div_cases(-1, 1));

    // Modulo: large values
    // EXPECT: 2147483647 % 10 = 7
    printf("%d %% 10 = %d\n", imax, mod_cases(imax, 10));

    // Modulo: min % -1
    // Note: min_int % -1 is 0 (though behavior can be UB, most compilers give 0)
    // EXPECT: min % -1 = 0
    printf("min %% -1 = %d\n", mod_cases(imin, -1));

    // Negation of -1
    // EXPECT: negate(-1) = 1
    printf("negate(-1) = %d\n", negate(-1));

    // Negation of 0
    // EXPECT: negate(0) = 0
    printf("negate(0) = %d\n", negate(0));

    // Negation of max
    // EXPECT: negate(max) = -2147483647
    printf("negate(max) = %d\n", negate(imax));

    // Repeated multiplication: 3^20 (overflow wraps)
    // 3^20 = 3486784401 which overflows 32-bit: 3486784401 - 4294967296 = -808182895
    // EXPECT: 3^20 = -808182895
    printf("3^20 = %d\n", accumulate_mul(3, 20));

    // 2^30
    // EXPECT: 2^30 = 1073741824
    printf("2^30 = %d\n", accumulate_mul(2, 30));

    // 2^31 wraps: 2147483648 wraps to -2147483648
    // EXPECT: 2^31 = -2147483648
    printf("2^31 = %d\n", accumulate_mul(2, 31));

    // Chain add: start at max, add 10 steps of 1
    // max + 10 = 2147483647 + 10 wraps: -2147483648 + 9 = -2147483639
    // EXPECT: max+10 = -2147483639
    printf("max+10 = %d\n", chain_add(imax, 1, 10));

    // Integer size in bits
    // EXPECT: int_bits = 32
    printf("int_bits = %d\n", int_size_bits());

    // Multiplication by -1
    // EXPECT: max * -1 = -2147483647
    printf("max * -1 = %d\n", multiply_large(imax, -1));

    // Zero divided by large number
    // EXPECT: 0 / max = 0
    printf("0 / max = %d\n", div_cases(0, imax));

    // Successive additions near boundary
    int v = imax - 5;
    v = v + 3;
    // EXPECT: near_max+3 = 2147483645
    printf("near_max+3 = %d\n", v);

    v = v + 3;
    // wraps: 2147483645 + 3 = 2147483648 -> -2147483648
    // EXPECT: near_max+6 = -2147483648
    printf("near_max+6 = %d\n", v);

    // Large subtraction
    // EXPECT: min - max = 1
    printf("min - max = %d\n", imin - imax);

    // Division truncates toward zero
    // EXPECT: -10 / 3 = -3
    printf("-10 / 3 = %d\n", div_cases(-10, 3));

    // EXPECT: 10 / -3 = -3
    printf("10 / -3 = %d\n", div_cases(10, -3));

    // Modulo signs
    // EXPECT: -10 % 3 = -1
    printf("-10 %% 3 = %d\n", mod_cases(-10, 3));

    // EXPECT: 10 % -3 = 1
    printf("10 %% -3 = %d\n", mod_cases(10, -3));

    return 0;
}
