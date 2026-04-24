int printf(const char *fmt, ...);
// EXPECT: === Test 1: char + char promotion ===\na+b = 150\na+b > 127: 1\nresult_int = 150\n=== Test 2: char * char overflow ===\na*b = 10000\na*b > 255: 1\na*b > 1000: 1\n=== Test 3: char left shift ===\na << 1 = 2\na << 8 = 256\na << 12 = 4096\n5 << 8 = 1280\n=== Test 4: signed vs unsigned comparison ===\n-1 < 1u: 0\n-1 == 1u: 0\n-1 > 1u: 1\nneg as unsigned > 0: 1\n0 < 1u: 1\n=== Test 5: sign extension char to int ===\nc = -50, i = -50\nc = -1, i = -1\nc = -128, i = -128\nc2 = 127, i2 = 127\n=== Test 6: char bit patterns ===\nc_neg1 as int = -1\nc_127 as int = 127\nc_neg128 as int = -128\nc_neg1 + 1 = 0\nc_neg128 + 127 = -1\n=== Test 7: integer division truncation ===\n7 / 2 = 3\n-7 / 2 = -3\n7 / -2 = -3\n-7 / -2 = 3\n13 / 4 = 3\n-13 / 4 = -3\n7 % 2 = 1\n-7 % 2 = -1\n7 % -2 = 1\n-7 % -2 = -1\n=== Test 8: mixed char/int arithmetic ===\nexpr1 = 610\nexpr2 = 297\nexpr3 = 26004\nexpr4 = -49\nexpr5 = 1\n=== Test 9: char increment ===\nbefore=10, post=10, after=11\nbefore=11, pre=12, after=12\nbefore=12, post=12, after=13\nbefore=13, pre=14, after=14\nchain = 42\n=== Test 10: signed/unsigned cast ===\nneg_as_unsigned > 0: 1\ncast_back = -1\nneg100_as_unsigned > 0: 1\ncast_back100 = -100\nunsigned_max + 1 == 0: 1\n=== Test 11: promotion in conditionals ===\nchar_cond = 1\nmixed_cond = 1\nzero_cond = 0\nneg_cond = 1\n=== Test 12: arithmetic edge cases ===\nmin_plus_max = -1\nmax_plus_one = -2147483648\nmin_minus_one = 2147483647\nneg_neg = 5\nneg_mul = 6\n=== Test 13: char in array indexing ===\narr[0]=10 arr[1]=20 arr[2]=30 arr[3]=40 arr[4]=50\nsum = 150\nchar_idx_sum = 150\n=== Test 14: unsigned arithmetic ===\nua - ub = 4294967295\nub - ua = 1\nua * big = 2000000000\nwrap = 0\n=== Test 15: complex promotion chains ===\nresult1 = 200\nresult2 = -200\nresult3 = 40000\nresult4 = 1\nresult5 = -56\nAll tests passed!

/* Test 1: char + char promotes both to int before addition */
int test_char_addition(void) {
    char a = 100;
    char b = 50;
    int result = a + b;
    printf("=== Test 1: char + char promotion ===\n");
    printf("a+b = %d\n", a + b);
    printf("a+b > 127: %d\n", (a + b) > 127);
    printf("result_int = %d\n", result);
    return result == 150;
}

/* Test 2: char * char overflows char range but fits in int */
int test_char_multiply(void) {
    char a = 100;
    char b = 100;
    int result = a * b;
    printf("=== Test 2: char * char overflow ===\n");
    printf("a*b = %d\n", a * b);
    printf("a*b > 255: %d\n", (a * b) > 255);
    printf("a*b > 1000: %d\n", (a * b) > 1000);
    return result == 10000;
}

/* Test 3: char << N promotes char to int first */
int test_char_shift(void) {
    char a = 1;
    char b = 5;
    printf("=== Test 3: char left shift ===\n");
    printf("a << 1 = %d\n", a << 1);
    printf("a << 8 = %d\n", a << 8);
    printf("a << 12 = %d\n", a << 12);
    printf("5 << 8 = %d\n", b << 8);
    return (a << 8) == 256;
}

/* Test 4: signed int vs unsigned: -1 converts to UINT_MAX, so -1 < 1u is FALSE */
int test_signed_unsigned_compare(void) {
    int neg = -1;
    unsigned u = 1;
    int lt = (neg < u) ? 1 : 0;
    int eq = (neg == u) ? 1 : 0;
    int gt = (neg > u) ? 1 : 0;
    unsigned neg_as_u = (unsigned)neg;
    int zero = 0;
    printf("=== Test 4: signed vs unsigned comparison ===\n");
    printf("-1 < 1u: %d\n", lt);
    printf("-1 == 1u: %d\n", eq);
    printf("-1 > 1u: %d\n", gt);
    printf("neg as unsigned > 0: %d\n", neg_as_u > 0);
    printf("0 < 1u: %d\n", (zero < u) ? 1 : 0);
    return lt == 0 && eq == 0 && gt == 1;
}

/* Test 5: char assigned to int preserves sign extension */
int test_sign_extension(void) {
    char c = -50;
    int i = c;
    char c2;
    int i2;
    printf("=== Test 5: sign extension char to int ===\n");
    printf("c = %d, i = %d\n", c, i);
    c = -1;
    i = c;
    printf("c = %d, i = %d\n", c, i);
    c = -128;
    i = c;
    printf("c = %d, i = %d\n", c, i);
    c2 = 127;
    i2 = c2;
    printf("c2 = %d, i2 = %d\n", c2, i2);
    return i == -128 && i2 == 127;
}

/* Test 6: char bit patterns - same bits, different interpretation */
int test_char_bit_patterns(void) {
    char c_neg1 = -1;
    char c_127 = 127;
    char c_neg128 = -128;
    int sum1 = c_neg1 + 1;
    int sum2 = c_neg128 + 127;
    printf("=== Test 6: char bit patterns ===\n");
    printf("c_neg1 as int = %d\n", (int)c_neg1);
    printf("c_127 as int = %d\n", (int)c_127);
    printf("c_neg128 as int = %d\n", (int)c_neg128);
    printf("c_neg1 + 1 = %d\n", sum1);
    printf("c_neg128 + 127 = %d\n", sum2);
    return sum1 == 0 && sum2 == -1;
}

/* Test 7: integer division truncates toward zero (C99+) */
int test_division_truncation(void) {
    printf("=== Test 7: integer division truncation ===\n");
    printf("7 / 2 = %d\n", 7 / 2);
    printf("-7 / 2 = %d\n", -7 / 2);
    printf("7 / -2 = %d\n", 7 / -2);
    printf("-7 / -2 = %d\n", -7 / -2);
    printf("13 / 4 = %d\n", 13 / 4);
    printf("-13 / 4 = %d\n", -13 / 4);
    printf("7 %% 2 = %d\n", 7 % 2);
    printf("-7 %% 2 = %d\n", -7 % 2);
    printf("7 %% -2 = %d\n", 7 % -2);
    printf("-7 %% -2 = %d\n", -7 % -2);
    return (-7 / 2 == -3) && (-7 % 2 == -1);
}

/* Test 8: mixing char and int in complex expressions */
int test_mixed_arithmetic(void) {
    char c1 = 100;
    char c2 = 10;
    int i1 = 500;
    int i2 = 3;
    char c3 = -1;
    int expr1, expr2, expr3, expr4, expr5;
    printf("=== Test 8: mixed char/int arithmetic ===\n");
    /* 100 + 500 + 10 = 610 */
    expr1 = c1 + i1 + c2;
    printf("expr1 = %d\n", expr1);
    /* 100 * 3 - 3 = 297 */
    expr2 = c1 * i2 - i2;
    printf("expr2 = %d\n", expr2);
    /* 100*10 + 500*500/10 + 4 = 1000 + 25000 + 4 = 26004 */
    expr3 = c1 * c2 + i1 * i1 / 10 + 4;
    printf("expr3 = %d\n", expr3);
    /* (-1)*500/10 - (-1) = -50 + 1 = -49 */
    expr4 = c3 * i1 / c2 - c3;
    printf("expr4 = %d\n", expr4);
    /* (100 > 10) && (500 > 100) = 1 */
    expr5 = (c1 > c2) && (i1 > c1);
    printf("expr5 = %d\n", expr5);
    return expr1 == 610 && expr2 == 297;
}

/* Test 9: char post-increment and pre-increment */
int test_char_increment(void) {
    char c = 10;
    char r;
    int before, after, val;
    printf("=== Test 9: char increment ===\n");
    /* post-increment: returns old value, then increments */
    before = c; r = c++; after = c;
    printf("before=%d, post=%d, after=%d\n", before, (int)r, after);
    /* pre-increment: increments first, returns new value */
    before = c; r = ++c; after = c;
    printf("before=%d, pre=%d, after=%d\n", before, (int)r, after);
    /* post-increment again */
    before = c; r = c++; after = c;
    printf("before=%d, post=%d, after=%d\n", before, (int)r, after);
    /* pre-increment again */
    before = c; r = ++c; after = c;
    printf("before=%d, pre=%d, after=%d\n", before, (int)r, after);
    /* post-increment in expression: 40 + 2 = 42 */
    c = 40;
    val = c++ + 2;
    printf("chain = %d\n", val);
    return val == 42;
}

/* Test 10: casting between signed and unsigned with negative values */
int test_signed_unsigned_cast(void) {
    int neg = -1;
    unsigned u_neg = (unsigned)neg;
    int back = (int)u_neg;
    int neg100 = -100;
    unsigned u_neg100 = (unsigned)neg100;
    int back100 = (int)u_neg100;
    unsigned zero_u = 0;
    unsigned max_u = zero_u - 1;
    unsigned wrap = max_u + 1;
    printf("=== Test 10: signed/unsigned cast ===\n");
    printf("neg_as_unsigned > 0: %d\n", u_neg > 0);
    printf("cast_back = %d\n", back);
    printf("neg100_as_unsigned > 0: %d\n", u_neg100 > 0);
    printf("cast_back100 = %d\n", back100);
    printf("unsigned_max + 1 == 0: %d\n", wrap == 0);
    return back == -1 && back100 == -100 && wrap == 0;
}

/* Test 11: type promotion in conditional/ternary expressions */
int test_promotion_in_conditionals(void) {
    char c = 1;
    int i = 1;
    char z = 0;
    char neg = -1;
    int r1 = c ? 1 : 0;
    int r2 = (c == i) ? 1 : 0;
    int r3 = z ? 1 : 0;
    int r4 = neg ? 1 : 0;
    printf("=== Test 11: promotion in conditionals ===\n");
    printf("char_cond = %d\n", r1);
    printf("mixed_cond = %d\n", r2);
    printf("zero_cond = %d\n", r3);
    printf("neg_cond = %d\n", r4);
    return r1 == 1 && r2 == 1 && r3 == 0 && r4 == 1;
}

/* Test 12: arithmetic edge cases with INT_MIN and INT_MAX */
int test_arithmetic_edge_cases(void) {
    int max_int = 2147483647;
    int min_int = -2147483648;
    int sum = min_int + max_int;        /* -1 */
    int overflow = max_int + 1;          /* wraps to -2147483648 */
    int underflow = min_int - 1;         /* wraps to 2147483647 */
    int neg = -5;
    int pos = -neg;                      /* 5 */
    int product = (-2) * (-3);           /* 6 */
    printf("=== Test 12: arithmetic edge cases ===\n");
    printf("min_plus_max = %d\n", sum);
    printf("max_plus_one = %d\n", overflow);
    printf("min_minus_one = %d\n", underflow);
    printf("neg_neg = %d\n", pos);
    printf("neg_mul = %d\n", product);
    return sum == -1 && pos == 5 && product == 6;
}

/* Test 13: char used as array index - must promote to int */
int test_char_array_indexing(void) {
    int arr[5];
    char idx;
    int sum;
    int char_sum;
    char ci;
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50;
    printf("=== Test 13: char in array indexing ===\n");
    printf("arr[0]=%d arr[1]=%d arr[2]=%d arr[3]=%d arr[4]=%d\n",
           arr[0], arr[1], arr[2], arr[3], arr[4]);
    sum = 0;
    idx = 0;
    while (idx < 5) {
        sum = sum + arr[idx];
        idx = idx + 1;
    }
    printf("sum = %d\n", sum);
    char_sum = 0;
    ci = 0;
    while (ci < 5) {
        char_sum = char_sum + arr[ci];
        ci = ci + 1;
    }
    printf("char_idx_sum = %d\n", char_sum);
    return sum == 150 && char_sum == 150;
}

/* Test 14: unsigned arithmetic - wrapping and subtraction */
int test_unsigned_arithmetic(void) {
    unsigned a = 1;
    unsigned b = 2;
    unsigned diff = a - b;               /* wraps to 4294967295 */
    unsigned diff2 = b - a;              /* 1 */
    unsigned big = 1000000000;
    unsigned product = b * big;          /* 2000000000 */
    unsigned max_u = 0;
    unsigned wrap;
    max_u = max_u - 1;                   /* UINT_MAX */
    wrap = max_u + 1;                    /* 0 */
    printf("=== Test 14: unsigned arithmetic ===\n");
    printf("ua - ub = %u\n", diff);
    printf("ub - ua = %u\n", diff2);
    printf("ua * big = %u\n", product);
    printf("wrap = %u\n", wrap);
    return diff2 == 1 && wrap == 0;
}

/* Test 15: complex promotion chains with multiple char/int operations */
int test_complex_promotion_chains(void) {
    char c1 = 100;
    char c2 = 100;
    char cn1 = -100;
    char cn2 = -100;
    char small = 1;
    int big = 1000;
    char ca = 7;
    char cb = -8;
    int result1 = c1 + c2;              /* 200 */
    int result2 = cn1 + cn2;            /* -200 */
    int result3 = c1 * c2 * 4;          /* 40000 */
    int result4 = (small < big) && (big > c1); /* 1 */
    int result5 = ca * cb;              /* -56 */
    printf("=== Test 15: complex promotion chains ===\n");
    printf("result1 = %d\n", result1);
    printf("result2 = %d\n", result2);
    printf("result3 = %d\n", result3);
    printf("result4 = %d\n", result4);
    printf("result5 = %d\n", result5);
    return result1 == 200 && result2 == -200 && result3 == 40000;
}

int main(void) {
    test_char_addition();
    test_char_multiply();
    test_char_shift();
    test_signed_unsigned_compare();
    test_sign_extension();
    test_char_bit_patterns();
    test_division_truncation();
    test_mixed_arithmetic();
    test_char_increment();
    test_signed_unsigned_cast();
    test_promotion_in_conditionals();
    test_arithmetic_edge_cases();
    test_char_array_indexing();
    test_unsigned_arithmetic();
    test_complex_promotion_chains();
    printf("All tests passed!\n");
    return 0;
}
