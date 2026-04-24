int printf(const char *fmt, ...);

// Edge value testing: INT_MAX, INT_MIN, 0, -1, overflow wrapping,
// unsigned max, char range, pointer NULL.

// Define limits without #include
#define INT_MAX_VAL  2147483647
#define INT_MIN_VAL  (-2147483647 - 1)
#define UINT_MAX_VAL 4294967295u
#define CHAR_MAX_VAL 127
#define CHAR_MIN_VAL (-128)

int test_int_max(void) {
    int x = INT_MAX_VAL;
    return (x == 2147483647) ? 1 : 0;
}

int test_int_min(void) {
    int x = INT_MIN_VAL;
    // INT_MIN is -2147483648, but we express it as -2147483647 - 1
    return (x < 0 && x < -2147483647) ? 1 : 0;
}

int test_int_max_plus_one(void) {
    int x = INT_MAX_VAL;
    // Overflow: INT_MAX + 1 wraps to INT_MIN (undefined in C, but most compilers wrap)
    int y = x + 1;
    return (y < 0) ? 1 : 0;
}

int test_int_min_minus_one(void) {
    int x = INT_MIN_VAL;
    // Overflow: INT_MIN - 1 wraps to INT_MAX
    int y = x - 1;
    return (y > 0) ? 1 : 0;
}

int test_zero(void) {
    int z = 0;
    int r = 0;
    if (!z) r += 1;          // !0 = 1
    if (z == 0) r += 10;     // true
    if (z >= 0) r += 100;    // true
    if (z <= 0) r += 1000;   // true
    return r;  // 1111
}

int test_minus_one(void) {
    int m = -1;
    int r = 0;
    if (m) r += 1;           // nonzero = true
    if (m < 0) r += 10;      // true
    if (m == -1) r += 100;   // true
    // Bitwise: -1 is all 1s
    if ((m & 0xFF) == 0xFF) r += 1000;
    return r;  // 1111
}

int test_unsigned_max(void) {
    unsigned int u = UINT_MAX_VAL;
    // u + 1 should wrap to 0
    unsigned int v = u + 1;
    int r = 0;
    if (v == 0) r += 1;
    // u - 1 should be UINT_MAX - 1
    unsigned int w = u - 1;
    if (w == 4294967294u) r += 10;
    // u / 2
    unsigned int half = u / 2;  // 2147483647
    if (half == 2147483647u) r += 100;
    return r;  // 111
}

int test_unsigned_zero_minus(void) {
    unsigned int u = 0;
    unsigned int v = u - 1;
    // 0 - 1 unsigned wraps to UINT_MAX
    if (v == UINT_MAX_VAL) return 1;
    return 0;
}

int test_char_range(void) {
    char cmax = CHAR_MAX_VAL;   // 127
    char cmin = CHAR_MIN_VAL;   // -128
    int r = 0;
    if (cmax == 127) r += 1;
    if (cmin == -128) r += 10;
    // cmax + 1 wraps to -128 when stored in char
    char overflow = cmax + 1;
    if (overflow == -128) r += 100;
    // cmin - 1 wraps to 127
    char underflow = cmin - 1;
    if (underflow == 127) r += 1000;
    return r;  // 1111
}

int test_null_pointer(void) {
    int *p = (int *)0;  // NULL
    int r = 0;
    if (p == 0) r += 1;
    if (!p) r += 10;
    // Non-null
    int x = 42;
    int *q = &x;
    if (q != 0) r += 100;
    if (q) r += 1000;
    return r;  // 1111
}

int test_shift_edge(void) {
    int x = 1;
    int r = 0;
    // Shift left by 0
    if ((x << 0) == 1) r += 1;
    // Shift left by 30 (max safe for signed positive int)
    int big = x << 30;  // 1073741824
    if (big == 1073741824) r += 10;
    // Shift right
    int y = INT_MAX_VAL;
    int shifted = y >> 30;  // arithmetic shift: 1
    if (shifted == 1) r += 100;
    return r;  // 111
}

int test_multiply_overflow(void) {
    int a = 50000;
    int b = 50000;
    int product = a * b;
    // 50000 * 50000 = 2500000000, which overflows 32-bit signed int
    // 2500000000 - 4294967296 = -1794967296
    return (product < 0) ? 1 : 0;
}

int test_divide_edge(void) {
    int r = 0;
    // Division by 1
    if (42 / 1 == 42) r += 1;
    // Division of 0
    if (0 / 42 == 0) r += 10;
    // Negative division (truncates toward zero in C99)
    if (-7 / 2 == -3) r += 100;
    if (7 / -2 == -3) r += 1000;
    return r;  // 1111
}

int test_modulo_edge(void) {
    int r = 0;
    // x % 1 == 0 for all x
    if (42 % 1 == 0) r += 1;
    // 0 % x == 0
    if (0 % 42 == 0) r += 10;
    // Negative modulo: result has sign of dividend (C99)
    if (-7 % 3 == -1) r += 100;
    if (7 % -3 == 1) r += 1000;
    return r;  // 1111
}

int test_comparison_edge(void) {
    int r = 0;
    // INT_MAX > INT_MIN
    if (INT_MAX_VAL > INT_MIN_VAL) r += 1;
    // INT_MIN < 0
    if (INT_MIN_VAL < 0) r += 10;
    // INT_MAX > 0
    if (INT_MAX_VAL > 0) r += 100;
    // -1 != 0
    if (-1 != 0) r += 1000;
    return r;  // 1111
}

int test_bitwise_edge(void) {
    int r = 0;
    // AND with 0
    if ((INT_MAX_VAL & 0) == 0) r += 1;
    // OR with 0
    if ((INT_MAX_VAL | 0) == INT_MAX_VAL) r += 10;
    // XOR with self
    if ((12345 ^ 12345) == 0) r += 100;
    // NOT 0
    if (~0 == -1) r += 1000;
    return r;  // 1111
}

int test_negation_edge(void) {
    int r = 0;
    // -0 == 0
    if (-0 == 0) r += 1;
    // -(-1) == 1
    int x = -1;
    if (-x == 1) r += 10;
    // -INT_MAX
    int nmax = -INT_MAX_VAL;
    if (nmax == -2147483647) r += 100;
    return r;  // 111
}

int test_bool_semantics(void) {
    int r = 0;
    // Any nonzero is true
    if (1) r += 1;
    if (-1) r += 10;
    if (42) r += 100;
    if (INT_MAX_VAL) r += 1000;
    if (INT_MIN_VAL) r += 10000;
    // 0 is false
    int zero = 0;
    if (!zero) r += 100000;
    return r;  // 111111
}

int test_successive_operations(void) {
    // Start with INT_MAX, perform operations that should give deterministic results
    unsigned int u = UINT_MAX_VAL;
    unsigned int a = u / 3;        // 1431655765
    unsigned int b = u % 3;        // 0 (since 3 * 1431655765 = 4294967295)
    unsigned int c = a + b;        // 1431655765
    unsigned int check = c % 1000; // 765
    return (int)check;
}

int test_alternating_sign(void) {
    int arr[10];
    int i;
    // Fill with alternating sign: 1, -2, 3, -4, 5, -6, 7, -8, 9, -10
    for (i = 0; i < 10; i++) {
        arr[i] = (i + 1) * ((i % 2 == 0) ? 1 : -1);
    }
    int sum = 0;
    for (i = 0; i < 10; i++) sum += arr[i];
    // 1-2+3-4+5-6+7-8+9-10 = -5
    return sum;
}

int test_power_of_two_detection(void) {
    // n & (n-1) == 0 for powers of 2 (and 0, but we skip 0)
    int count = 0;
    int i;
    for (i = 1; i <= 1024; i++) {
        if ((i & (i - 1)) == 0) count++;
    }
    // Powers of 2 in [1,1024]: 1,2,4,8,16,32,64,128,256,512,1024 = 11
    return count;
}

int main(void) {
    printf("%d\n", test_int_max());
    // EXPECT: 1

    printf("%d\n", test_int_min());
    // EXPECT: 1

    printf("%d\n", test_int_max_plus_one());
    // EXPECT: 1

    printf("%d\n", test_int_min_minus_one());
    // EXPECT: 1

    printf("%d\n", test_zero());
    // EXPECT: 1111

    printf("%d\n", test_minus_one());
    // EXPECT: 1111

    printf("%d\n", test_unsigned_max());
    // EXPECT: 111

    printf("%d\n", test_unsigned_zero_minus());
    // EXPECT: 1

    printf("%d\n", test_char_range());
    // EXPECT: 1111

    printf("%d\n", test_null_pointer());
    // EXPECT: 1111

    printf("%d\n", test_shift_edge());
    // EXPECT: 111

    printf("%d\n", test_multiply_overflow());
    // EXPECT: 1

    printf("%d\n", test_divide_edge());
    // EXPECT: 1111

    printf("%d\n", test_modulo_edge());
    // EXPECT: 1111

    printf("%d\n", test_comparison_edge());
    // EXPECT: 1111

    printf("%d\n", test_bitwise_edge());
    // EXPECT: 1111

    printf("%d\n", test_negation_edge());
    // EXPECT: 111

    printf("%d\n", test_bool_semantics());
    // EXPECT: 111111

    printf("%d\n", test_successive_operations());
    // EXPECT: 765

    printf("%d\n", test_alternating_sign());
    // EXPECT: -5

    printf("%d\n", test_power_of_two_detection());
    // EXPECT: 11

    printf("ALL PASSED\n");
    // EXPECT: ALL PASSED
    return 0;
}
