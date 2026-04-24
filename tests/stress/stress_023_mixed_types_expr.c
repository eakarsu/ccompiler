int printf(const char *fmt, ...);

// Type promotion stress: expressions mixing char, short, int, long, unsigned.
// Cast chains, signed/unsigned traps, implicit conversions.

int test_char_promotion(void) {
    char a = 100;
    char b = 100;
    // char * char promotes to int, result = 10000 (fits in int)
    int result = a * b;
    return result;
}

int test_char_overflow(void) {
    char a = 127;
    char b = 1;
    // a + b in int = 128, then assigned to char => -128 (signed overflow wrap)
    char c = a + b;
    return (int)c;
}

int test_unsigned_comparison(void) {
    unsigned int u = 1;
    int s = -1;
    // When comparing unsigned with signed, signed is converted to unsigned
    // -1 as unsigned = 4294967295, which is > 1
    // So (s < u) is FALSE in C (counterintuitive)
    if (s < u) {
        return 0;  // should NOT reach here
    } else {
        return 1;  // should reach here
    }
}

int test_unsigned_subtraction(void) {
    unsigned int a = 5;
    unsigned int b = 10;
    unsigned int c = a - b;
    // 5 - 10 unsigned wraps to 4294967291
    // Check it's > 1000000
    if (c > 1000000) return 1;
    return 0;
}

int test_cast_chain(void) {
    long val = 100000;
    // (int)(char)(long)(short)val
    // short: 100000 mod 65536 = 34464, as signed short = 34464 - 65536 = -31072
    // long: -31072
    // char: -31072 mod 256 = 160 as unsigned byte, signed = 160-256 = -96
    // int: -96
    int result = (int)(char)(long)(short)val;
    return result;
}

int test_mixed_arithmetic(void) {
    char c = 10;
    short s = 200;
    int i = 3000;
    long l = 40000;

    // All promote to at least int, then to long due to l
    long result = c + s + i + l;
    return (int)result;
}

int test_unsigned_division(void) {
    unsigned int a = 4294967295u;  // UINT_MAX
    unsigned int b = 10;
    unsigned int q = a / b;
    // 4294967295 / 10 = 429496729
    return (int)(q % 1000000);  // last 6 digits: 496729
}

int test_signed_shift(void) {
    int x = 1;
    int shifted = x << 30;  // 1073741824
    return shifted / 1000000;  // 1073
}

int test_char_sign_extend(void) {
    char c = -50;
    int i = c;      // sign-extends to -50
    unsigned int u = (unsigned int)c;  // sign-extends then reinterpret: large number
    // u should be 4294967246
    // Check that i is negative and u is very large
    int r1 = (i == -50) ? 1 : 0;
    int r2 = (u > 4000000000u) ? 1 : 0;
    return r1 * 10 + r2;
}

int test_truncation(void) {
    int big = 0x12345678;
    char c = (char)big;   // takes low byte: 0x78 = 120
    short s = (short)big; // takes low 2 bytes: 0x5678 = 22136
    return (int)c * 1000 + (int)s % 1000;
}

int test_implicit_conversion_in_call(int x) {
    return x * 2;
}

int test_pointer_arithmetic_types(void) {
    int arr[10];
    int i;
    for (i = 0; i < 10; i++) arr[i] = i * 11;
    int *p = arr;
    // pointer + int
    int v1 = *(p + 3);        // arr[3] = 33
    int v2 = *(p + (char)7);  // arr[7] = 77, char promoted to ptrdiff
    return v1 * 100 + v2;
}

int test_ternary_type(void) {
    int a = 1;
    unsigned int b = 2;
    // Result type is unsigned int due to promotion
    // But both values are small positives, so it's fine
    int result = (a > 0) ? a : b;
    return result;
}

int test_compound_promotions(void) {
    char a = 50;
    char b = 60;
    char c = 70;
    // a+b+c: all promoted to int: 50+60+70 = 180
    int r = a + b + c;
    return r;
}

int test_negation_of_unsigned(void) {
    unsigned int u = 100;
    unsigned int neg = -u;
    // -100u = 4294967196u
    // Check it's large
    if (neg > 4000000000u) return 1;
    return 0;
}

int test_bitwise_on_chars(void) {
    char a = 0x55;  // 01010101 = 85
    char b = 0x33;  // 00110011 = 51
    // Promoted to int for bitwise ops
    int and_r = a & b;   // 0x11 = 17
    int or_r = a | b;    // 0x77 = 119
    int xor_r = a ^ b;   // 0x66 = 102
    return and_r + or_r * 1000 + xor_r;
}

int test_modulo_signed(void) {
    int a = -7;
    int b = 3;
    int r = a % b;  // C99: result has sign of dividend => -1
    return r;
}

int test_division_truncation(void) {
    int a = 7;
    int b = -2;
    int r = a / b;  // C99: truncates toward zero => -3
    return r;
}

int test_long_mul_then_truncate(void) {
    long a = 100000;
    long b = 100000;
    long product = a * b;  // 10000000000
    int truncated = (int)product;
    // 10000000000 mod 2^32 = 10000000000 - 2*4294967296 = 10000000000 - 8589934592 = 1410065408
    return truncated;
}

int main(void) {
    printf("%d\n", test_char_promotion());
    // EXPECT: 10000

    printf("%d\n", test_char_overflow());
    // EXPECT: -128

    printf("%d\n", test_unsigned_comparison());
    // EXPECT: 1

    printf("%d\n", test_unsigned_subtraction());
    // EXPECT: 1

    printf("%d\n", test_cast_chain());
    // EXPECT: -96

    printf("%d\n", test_mixed_arithmetic());
    // EXPECT: 43210

    printf("%d\n", test_unsigned_division());
    // EXPECT: 496729

    printf("%d\n", test_signed_shift());
    // EXPECT: 1073

    printf("%d\n", test_char_sign_extend());
    // EXPECT: 11

    printf("%d\n", test_truncation());
    // EXPECT: 120136

    // implicit conversion: char 50 passed as int
    char cv = 50;
    printf("%d\n", test_implicit_conversion_in_call(cv));
    // EXPECT: 100

    printf("%d\n", test_pointer_arithmetic_types());
    // EXPECT: 3377

    printf("%d\n", test_ternary_type());
    // EXPECT: 1

    printf("%d\n", test_compound_promotions());
    // EXPECT: 180

    printf("%d\n", test_negation_of_unsigned());
    // EXPECT: 1

    printf("%d\n", test_bitwise_on_chars());
    // EXPECT: 119119

    printf("%d\n", test_modulo_signed());
    // EXPECT: -1

    printf("%d\n", test_division_truncation());
    // EXPECT: -3

    printf("%d\n", test_long_mul_then_truncate());
    // EXPECT: 1410065408

    printf("ALL PASSED\n");
    // EXPECT: ALL PASSED
    return 0;
}
