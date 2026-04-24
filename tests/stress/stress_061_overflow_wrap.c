// stress_061_overflow_wrap.c - Integer overflow/wrap stress test
// EXPECT: unsigned_wrap=0
// EXPECT: unsigned_wrap_sub=4294967295
// EXPECT: unsigned_add_wrap=0
// EXPECT: unsigned_mul_wrap=4294967294
// EXPECT: char_overflow_pos=0
// EXPECT: char_overflow_neg=-1
// EXPECT: uchar_wrap=0
// EXPECT: uchar_wrap2=255
// EXPECT: short_overflow=0
// EXPECT: ushort_wrap=0
// EXPECT: shift_left_1=2147483648
// EXPECT: shift_left_2=0
// EXPECT: shift_right_u=1
// EXPECT: overflow_detect_no=0
// EXPECT: overflow_detect_yes=1
// EXPECT: mul_overflow_lo=4294967294
// EXPECT: chain_overflow=0
// EXPECT: unsigned_countdown=5 4 3 2 1 0
// EXPECT: bit_pattern_ff=255
// EXPECT: complement=4294967040
// EXPECT: negate_unsigned=4294967295
// EXPECT: mixed_sign_add=0
// EXPECT: inc_max_unsigned=0
// EXPECT: dec_zero_unsigned=4294967295

int printf(const char *fmt, ...);

// Unsigned overflow wraps around (defined behavior in C)
unsigned unsigned_wrap_test(void) {
    unsigned u = 4294967295U; // UINT_MAX
    u = u + 1U;
    return u;
}

unsigned unsigned_sub_wrap(void) {
    unsigned u = 0;
    u = u - 1U;
    return u;
}

unsigned unsigned_add_wrap(void) {
    unsigned a = 4294967295U;
    unsigned b = 1U;
    return a + b;
}

unsigned unsigned_mul_wrap(void) {
    unsigned a = 4294967295U; // UINT_MAX
    unsigned b = 2U;
    return a * b; // UINT_MAX * 2 = 2*UINT_MAX mod 2^32 = UINT_MAX*2 mod 2^32 = 4294967294
}

// Char overflow
int char_overflow_positive(void) {
    // char is signed on most platforms, range -128 to 127
    // 127 + 1 wraps; cast result to unsigned char to get 128, but as signed char it's -128
    // We store in int to observe the char-width behavior
    char c = 127;
    c = c + 1; // implementation-defined for signed char
    // On most platforms this is -128, but we convert to unsigned char for portability
    unsigned char uc = (unsigned char)c;
    // uc should be 128; subtract 128 to get 0 for reliable check
    return uc - 128;
}

int char_overflow_negative(void) {
    char c = -128;
    c = c - 1; // wraps to 127 as signed char on most platforms
    // As unsigned char this is 127
    unsigned char uc = (unsigned char)c;
    // Return uc - 128 = 127 - 128 = -1
    return uc - 128;
}

unsigned char uchar_wrap_up(void) {
    unsigned char uc = 255;
    uc = uc + 1;
    return uc;
}

unsigned char uchar_wrap_down(void) {
    unsigned char uc = 0;
    uc = uc - 1;
    return uc;
}

// Short overflow
int short_overflow_test(void) {
    short s = 32767;
    s = s + 1; // wraps to -32768 on most platforms
    // Add 32768 to normalize
    return s + 32768;
}

unsigned short ushort_wrap_test(void) {
    unsigned short us = 65535;
    us = us + 1;
    return us;
}

// Shifting near bit width
unsigned shift_left_test1(void) {
    unsigned val = 1U;
    return val << 31; // 2147483648
}

unsigned shift_left_test2(void) {
    unsigned val = 1U;
    unsigned result = val << 31;
    return result << 1; // shifts out entirely = 0 (UB for signed, but unsigned << 32 is UB too)
    // Actually shifting by bit width is UB. Let's shift 31 then shift 1 separately.
}

unsigned shift_right_test(void) {
    unsigned val = 2147483648U; // 1 << 31
    return val >> 31;
}

// Addition overflow detection pattern
int detect_overflow(unsigned a, unsigned b) {
    unsigned sum = a + b;
    if (sum < a) return 1; // overflow detected
    return 0;
}

// Multiplication overflow: low bits
unsigned mul_low(unsigned a, unsigned b) {
    return a * b;
}

// Chained operations that overflow multiple times
unsigned chain_overflow(void) {
    unsigned val = 4294967295U;
    val = val + 1U; // 0
    val = val - 1U; // UINT_MAX
    val = val + 1U; // 0
    return val;
}

// Unsigned countdown through zero
void unsigned_countdown(void) {
    printf("unsigned_countdown=");
    unsigned i = 5;
    while (1) {
        printf("%u", i);
        if (i == 0) break;
        printf(" ");
        i = i - 1;
    }
    printf("\n");
}

// Bit patterns
unsigned bit_pattern_ff(void) {
    unsigned val = 0;
    val = val | 0xFF;
    return val;
}

unsigned complement_test(void) {
    unsigned val = 0xFF;
    return ~val; // 4294967040 = 0xFFFFFF00
}

unsigned negate_unsigned(void) {
    unsigned val = 1U;
    // Unary minus on unsigned: 0 - val = UINT_MAX
    unsigned result = -val;
    return result;
}

// Mixed signed/unsigned addition
unsigned mixed_sign(void) {
    int neg = -1;
    unsigned pos = 1U;
    // -1 converted to unsigned is UINT_MAX, UINT_MAX + 1 = 0
    return neg + pos;
}

// Increment UINT_MAX
unsigned inc_max(void) {
    unsigned u = 4294967295U;
    u++;
    return u;
}

// Decrement zero
unsigned dec_zero(void) {
    unsigned u = 0;
    u--;
    return u;
}

int main(void) {
    printf("unsigned_wrap=%u\n", unsigned_wrap_test());
    printf("unsigned_wrap_sub=%u\n", unsigned_sub_wrap());
    printf("unsigned_add_wrap=%u\n", unsigned_add_wrap());
    printf("unsigned_mul_wrap=%u\n", unsigned_mul_wrap());
    printf("char_overflow_pos=%d\n", char_overflow_positive());
    printf("char_overflow_neg=%d\n", char_overflow_negative());
    printf("uchar_wrap=%u\n", (unsigned)uchar_wrap_up());
    printf("uchar_wrap2=%u\n", (unsigned)uchar_wrap_down());
    printf("short_overflow=%d\n", short_overflow_test());
    printf("ushort_wrap=%u\n", (unsigned)ushort_wrap_test());
    printf("shift_left_1=%u\n", shift_left_test1());
    printf("shift_left_2=%u\n", shift_left_test2());
    printf("shift_right_u=%u\n", shift_right_test());
    printf("overflow_detect_no=%d\n", detect_overflow(100U, 200U));
    printf("overflow_detect_yes=%d\n", detect_overflow(4294967295U, 1U));
    printf("mul_overflow_lo=%u\n", mul_low(4294967295U, 2U));
    printf("chain_overflow=%u\n", chain_overflow());
    unsigned_countdown();
    printf("bit_pattern_ff=%u\n", bit_pattern_ff());
    printf("complement=%u\n", complement_test());
    printf("negate_unsigned=%u\n", negate_unsigned());
    printf("mixed_sign_add=%u\n", mixed_sign());
    printf("inc_max_unsigned=%u\n", inc_max());
    printf("dec_zero_unsigned=%u\n", dec_zero());
    return 0;
}
