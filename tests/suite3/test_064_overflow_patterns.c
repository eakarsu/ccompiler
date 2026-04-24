int printf(const char *fmt, ...);

/* Overflow detection patterns using unsigned wrapping and bit tricks */

/* Check if unsigned addition would overflow (result < either operand) */
int uadd_overflows(unsigned int a, unsigned int b) {
    return (a + b) < a;
}

/* Check if unsigned multiplication might overflow */
/* If a != 0 and result/a != b then overflow occurred */
int umul_overflows(unsigned int a, unsigned int b) {
    if (a == 0) return 0;
    unsigned int result = a * b;
    return (result / a) != b;
}

/* Safe signed add overflow detection: using unsigned arithmetic */
/* Overflow iff operands have same sign and result has different sign */
int sadd_overflows(int a, int b) {
    /* Cast to unsigned to avoid UB */
    unsigned int ua = (unsigned int)a;
    unsigned int ub = (unsigned int)b;
    unsigned int ur = ua + ub;
    int r = (int)ur;
    /* Overflow if: both positive and result negative, or both negative and result positive */
    if (a > 0 && b > 0 && r < 0) return 1;
    if (a < 0 && b < 0 && r > 0) return 1;
    return 0;
}

/* Unsigned max value for 32-bit */
#define UINT_MAX_VAL 4294967295u
#define INT_MAX_VAL  2147483647
#define INT_MIN_VAL  (-2147483647 - 1)

int main(void) {
    /* --- UNSIGNED WRAPPING --- */
    unsigned int u = 0u;
    u = u - 1u;                    /* wraps to UINT_MAX */
    printf("%u\n", u);                       // EXPECT: 4294967295

    u = UINT_MAX_VAL;
    u = u + 1u;                    /* wraps to 0 */
    printf("%u\n", u);                       // EXPECT: 0

    /* Small wraps */
    unsigned char uc = 255;
    uc = uc + 1;                   /* promotion to int, no wrap here */
    printf("%d\n", (int)uc);                 // EXPECT: 0
    /* uc + 1 = 256, truncated back to unsigned char = 0 */

    unsigned char uc2 = 0;
    uc2 = uc2 - 1;                 /* 0-1 as unsigned char = 255 */
    printf("%d\n", (int)uc2);               // EXPECT: 255

    /* --- UNSIGNED ADD OVERFLOW DETECTION --- */
    printf("%d\n", uadd_overflows(0u, 0u));                         // EXPECT: 0
    printf("%d\n", uadd_overflows(1u, 1u));                         // EXPECT: 0
    printf("%d\n", uadd_overflows(UINT_MAX_VAL, 1u));               // EXPECT: 1
    printf("%d\n", uadd_overflows(UINT_MAX_VAL, UINT_MAX_VAL));     // EXPECT: 1
    printf("%d\n", uadd_overflows(2147483647u, 2147483648u));       // EXPECT: 0
    /* 0x7FFFFFFF + 0x80000000 = 0xFFFFFFFF, no overflow */
    printf("%d\n", uadd_overflows(2147483648u, 2147483648u));       // EXPECT: 1
    /* 0x80000000 + 0x80000000 = 0x100000000, wraps to 0 < 0x80000000 */

    /* --- UNSIGNED MULTIPLY OVERFLOW DETECTION --- */
    printf("%d\n", umul_overflows(0u, 1000000u));                   // EXPECT: 0
    printf("%d\n", umul_overflows(1u, UINT_MAX_VAL));               // EXPECT: 0
    printf("%d\n", umul_overflows(2u, 2147483648u));                // EXPECT: 1
    /* 2 * 2147483648 = 4294967296 > UINT_MAX */
    printf("%d\n", umul_overflows(65536u, 65536u));                 // EXPECT: 1
    /* 65536^2 = 2^32 > UINT_MAX */
    printf("%d\n", umul_overflows(65535u, 65536u));                 // EXPECT: 0
    /* 65535 * 65536 = 4294901760 < 2^32 */
    printf("%d\n", umul_overflows(100u, 100u));                     // EXPECT: 0

    /* --- SIGNED ADD OVERFLOW DETECTION --- */
    printf("%d\n", sadd_overflows(0, 0));                           // EXPECT: 0
    printf("%d\n", sadd_overflows(1, 1));                           // EXPECT: 0
    printf("%d\n", sadd_overflows(INT_MAX_VAL, 1));                 // EXPECT: 1
    printf("%d\n", sadd_overflows(INT_MIN_VAL, -1));                // EXPECT: 1
    printf("%d\n", sadd_overflows(INT_MAX_VAL, INT_MAX_VAL));       // EXPECT: 1
    printf("%d\n", sadd_overflows(INT_MIN_VAL, INT_MIN_VAL));       // EXPECT: 0
    /* INT_MIN+INT_MIN wraps to 0 in unsigned; r==0 so r>0 is false; function returns 0 */
    printf("%d\n", sadd_overflows(INT_MAX_VAL, 0));                 // EXPECT: 0
    printf("%d\n", sadd_overflows(1000000, -1000000));              // EXPECT: 0
    printf("%d\n", sadd_overflows(2000000000, 200000000));          // EXPECT: 1
    /* 2e9 + 2e8 = 2.2e9 > INT_MAX=2147483647 */
    printf("%d\n", sadd_overflows(-2000000000, -200000000));        // EXPECT: 1
    /* -2e9 + -2e8 = -2.2e9 < INT_MIN */

    /* --- UNSIGNED WRAPPING ARITHMETIC AS ALGORITHM --- */
    /* Distance in modular arithmetic: diff when b may have wrapped past a */
    unsigned int seq1 = 4294967290u;   /* near UINT_MAX */
    unsigned int seq2 = 4u;            /* has wrapped around */
    unsigned int dist = seq2 - seq1;   /* = 10 in modular arithmetic */
    printf("%u\n", dist);                    // EXPECT: 10
    /* 4 - 4294967290 = 4 - (2^32 - 6) = 10 mod 2^32 */

    /* --- OVERFLOW IN INTERMEDIATE COMPUTATIONS --- */
    /* Using unsigned for safe wrapping multiplication */
    unsigned int a = 123456789u;
    unsigned int b_val = 10u;
    unsigned int result_mod = (a * b_val);   /* may overflow, wraps mod 2^32 */
    /* 123456789 * 10 = 1234567890, fits in 32 bits */
    printf("%u\n", result_mod);              // EXPECT: 1234567890

    a = 400000000u;
    b_val = 20u;
    result_mod = a * b_val;
    /* 400000000 * 20 = 8000000000 > 2^32=4294967296 */
    /* 8000000000 mod 4294967296 = 8000000000 - 4294967296 = 3705032704 */
    printf("%u\n", result_mod);              // EXPECT: 3705032704

    /* --- INT_MAX and INT_MIN boundary --- */
    int imax = INT_MAX_VAL;
    int imin = INT_MIN_VAL;
    printf("%d\n", imax);                    // EXPECT: 2147483647
    printf("%d\n", imin);                    // EXPECT: -2147483648
    printf("%d\n", imax / 2);               // EXPECT: 1073741823
    printf("%d\n", imin / 2);               // EXPECT: -1073741824
    /* C11: -2147483648 / 2 = -1073741824 (truncation toward zero) */

    /* --- BIT TRICKS FOR OVERFLOW-FREE AVERAGE --- */
    /* Average of two unsigned ints without overflow: (a&b) + ((a^b)>>1) */
    unsigned int x = 3000000000u;
    unsigned int y = 3000000000u;
    unsigned int avg = (x & y) + ((x ^ y) >> 1);
    printf("%u\n", avg);                     // EXPECT: 3000000000

    x = 2000000000u;
    y = 3000000000u;
    avg = (x & y) + ((x ^ y) >> 1);
    printf("%u\n", avg);                     // EXPECT: 2500000000

    return 0;
}
