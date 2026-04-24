int printf(const char *fmt, ...);
void *malloc(long size);
void free(void *ptr);

// Cast stress: int<->char round trip, pointer casts, signed<->unsigned,
// void* casts, cast in expressions, cast as arg, cast of return value.

int test_int_char_roundtrip(void) {
    int orig = 65;  // 'A'
    char c = (char)orig;
    int back = (int)c;
    return back;  // 65
}

int test_char_truncation_roundtrip(void) {
    int orig = 300;      // doesn't fit in char
    char c = (char)orig;  // 300 mod 256 = 44
    int back = (int)c;
    return back;  // 44
}

int test_negative_char_roundtrip(void) {
    int orig = -50;
    char c = (char)orig;   // -50 fits in signed char
    int back = (int)c;     // sign-extends back to -50
    return back;
}

int test_unsigned_cast(void) {
    int neg = -1;
    unsigned int u = (unsigned int)neg;
    // u should be 4294967295 (UINT_MAX)
    // Cast back
    int back = (int)u;
    return back;  // -1
}

int test_void_ptr_cast(void) {
    int x = 42;
    void *vp = (void *)&x;
    int *ip = (int *)vp;
    return *ip;  // 42
}

int test_void_ptr_malloc(void) {
    int *arr = (int *)malloc(10 * sizeof(int));
    int i;
    for (i = 0; i < 10; i++) arr[i] = i * 7;

    void *vp = (void *)arr;
    int *recovered = (int *)vp;

    int sum = 0;
    for (i = 0; i < 10; i++) sum += recovered[i];
    free(arr);
    // 0+7+14+21+28+35+42+49+56+63 = 315
    return sum;
}

int test_char_ptr_cast(void) {
    int val = 0x41424344;  // bytes depend on endianness
    char *cp = (char *)&val;
    // On little-endian (ARM64): cp[0]=0x44='D', cp[1]=0x43='C', cp[2]=0x42='B', cp[3]=0x41='A'
    int sum = cp[0] + cp[1] + cp[2] + cp[3];
    // 0x44+0x43+0x42+0x41 = 68+67+66+65 = 266
    return sum;
}

int test_multi_level_cast(void) {
    long val = 100000;
    // Chain: long -> short -> char -> int
    int result = (int)(char)(short)val;
    // short: 100000 mod 65536 = 34464, as signed: 34464 - 65536 = -31072
    // char: -31072 mod 256 = 160, as signed: 160 - 256 = -96
    // int: -96
    return result;
}

int test_unsigned_signed_comparison(void) {
    unsigned int u = 10;
    int s = -1;
    // In comparison, s is converted to unsigned: (unsigned)-1 = 4294967295
    // 4294967295 > 10, so s > u in unsigned comparison
    int r = 0;
    if ((unsigned int)s > u) r = 1;
    return r;  // 1
}

int test_cast_in_expression(void) {
    int a = 7, b = 2;
    // Integer division: 7/2 = 3
    int int_div = a / b;
    // With long cast for overflow protection
    long big_a = 100000;
    long big_b = 100000;
    long product = big_a * big_b;  // 10000000000
    int truncated = (int)product;
    // 10000000000 mod 2^32 = 1410065408
    return int_div * 10 + (truncated > 0 ? 1 : 0);
    // 3*10 + 1 = 31
}

int returns_int(void) {
    return 999;
}

int test_cast_of_return(void) {
    char c = (char)returns_int();
    // 999 mod 256 = 231, signed: 231-256 = -25
    return (int)c;
}

int test_cast_as_argument_helper(int x) {
    return x + 1;
}

int test_cast_as_argument(void) {
    char c = 100;
    // c is promoted to int when passed
    return test_cast_as_argument_helper((int)c);  // 101
}

int test_pointer_diff_cast(void) {
    int arr[10];
    int *p1 = &arr[2];
    int *p2 = &arr[7];
    long diff = (long)(p2 - p1);  // pointer diff = 5 elements
    return (int)diff;
}

int test_struct_member_cast(void) {
    struct { int a; char b; } s;
    s.a = 1000;
    s.b = 50;
    int val = (int)s.b + (char)s.a;
    // (int)s.b = 50, (char)s.a = (char)1000 = -24 (1000 mod 256 = 232, signed: 232-256=-24)
    // 50 + (-24) = 26
    return val;
}

int test_array_index_cast(void) {
    int arr[10];
    int i;
    for (i = 0; i < 10; i++) arr[i] = i * 11;

    char idx = 7;
    long lidx = 3;
    return arr[(int)idx] + arr[(int)lidx];
    // arr[7]=77, arr[3]=33 => 110
}

int test_conditional_cast(void) {
    int x = 500;
    char c = (x > 255) ? (char)(x - 256) : (char)x;
    // x=500, 500>255, so c = (char)(500-256) = (char)244 = -12 (signed)
    return (int)c;
}

int test_bitwise_with_cast(void) {
    unsigned int u = 0xFF00FF00u;
    char c = (char)u;         // low byte: 0x00 = 0
    short s = (short)u;       // low 2 bytes: 0xFF00, signed: -256
    int high = (int)(u >> 16); // 0xFF00 = 65280
    return (int)c + (int)s + high;
    // 0 + (-256) + 65280 = 65024
}

int test_sign_extension_chain(void) {
    char c = -1;               // 0xFF
    short s = (short)c;        // sign-extend: 0xFFFF = -1
    int i = (int)s;            // sign-extend: 0xFFFFFFFF = -1
    long l = (long)i;          // sign-extend: -1
    unsigned long ul = (unsigned long)l;  // reinterpret: huge number
    // Cast back chain
    int back = (int)(short)(char)(int)ul;
    return back;  // -1 through all truncations and sign extensions
}

int test_pointer_to_different_type(void) {
    int arr[3] = {0x11223344, 0x55667788, 0x99AABBCC};
    char *cp = (char *)arr;
    // Little-endian ARM64:
    // arr[0] bytes: 44 33 22 11
    // arr[1] bytes: 88 77 66 55
    // cp[4] should be 0x88 = -120 (signed char)
    int val = (int)(unsigned char)cp[4];  // 0x88 = 136 unsigned
    return val;
}

int test_nested_casts_in_arithmetic(void) {
    int a = 300, b = 400;
    // Force overflow through char
    char ca = (char)a;  // 300 mod 256 = 44
    char cb = (char)b;  // 400 mod 256 = 144, signed: -112
    int sum = (int)ca + (int)cb;
    // 44 + (-112) = -68
    return sum;
}

int main(void) {
    printf("%d\n", test_int_char_roundtrip());
    // EXPECT: 65

    printf("%d\n", test_char_truncation_roundtrip());
    // EXPECT: 44

    printf("%d\n", test_negative_char_roundtrip());
    // EXPECT: -50

    printf("%d\n", test_unsigned_cast());
    // EXPECT: -1

    printf("%d\n", test_void_ptr_cast());
    // EXPECT: 42

    printf("%d\n", test_void_ptr_malloc());
    // EXPECT: 315

    printf("%d\n", test_char_ptr_cast());
    // EXPECT: 266

    printf("%d\n", test_multi_level_cast());
    // EXPECT: -96

    printf("%d\n", test_unsigned_signed_comparison());
    // EXPECT: 1

    printf("%d\n", test_cast_in_expression());
    // EXPECT: 31

    printf("%d\n", test_cast_of_return());
    // EXPECT: -25

    printf("%d\n", test_cast_as_argument());
    // EXPECT: 101

    printf("%d\n", test_pointer_diff_cast());
    // EXPECT: 5

    printf("%d\n", test_struct_member_cast());
    // EXPECT: 26

    printf("%d\n", test_array_index_cast());
    // EXPECT: 110

    printf("%d\n", test_conditional_cast());
    // EXPECT: -12

    printf("%d\n", test_bitwise_with_cast());
    // EXPECT: 65024

    printf("%d\n", test_sign_extension_chain());
    // EXPECT: -1

    printf("%d\n", test_pointer_to_different_type());
    // EXPECT: 136

    printf("%d\n", test_nested_casts_in_arithmetic());
    // EXPECT: -68

    printf("ALL PASSED\n");
    // EXPECT: ALL PASSED
    return 0;
}
