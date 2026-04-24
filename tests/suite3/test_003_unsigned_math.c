int printf(const char *fmt, ...);

int main(void) {
    /* Basic unsigned arithmetic */
    unsigned int ua = 10;
    unsigned int ub = 20;
    printf("%u\n", ua + ub);        // EXPECT: 30
    printf("%u\n", ub - ua);        // EXPECT: 10
    printf("%u\n", ua * ub);        // EXPECT: 200
    printf("%u\n", ub / ua);        // EXPECT: 2

    /* UINT_MAX = 4294967295 */
    unsigned int umax = 4294967295u;
    printf("%u\n", umax);           // EXPECT: 4294967295

    /* UINT_MAX + 1 wraps to 0 */
    unsigned int wrap = umax + 1u;
    printf("%u\n", wrap);           // EXPECT: 0

    /* 0 - 1 wraps to UINT_MAX */
    unsigned int uzero = 0u;
    unsigned int wraparound = uzero - 1u;
    printf("%u\n", wraparound);     // EXPECT: 4294967295

    /* Wrapping arithmetic */
    /* 4294967290 + 5 = 4294967295, no wrap */
    unsigned int big = 4294967290u;
    printf("%u\n", big + 5u);       // EXPECT: 4294967295
    /* 4294967290 + 6 = 4294967296 mod 2^32 = 0 */
    printf("%u\n", big + 6u);       // EXPECT: 0
    /* 4294967290 + 10 = 4294967300 mod 4294967296 = 4 */
    printf("%u\n", big + 10u);      // EXPECT: 4

    /* Unsigned multiplication overflow */
    /* 65536 * 65536 = 2^32 mod 2^32 = 0 */
    unsigned int um = 65536u;
    printf("%u\n", um * um);        // EXPECT: 0

    /* 65537 * 2 = 131074, fits */
    unsigned int um2 = 65537u;
    printf("%u\n", um2 * 2u);       // EXPECT: 131074

    /* Unsigned division */
    unsigned int ud1 = 100u;
    unsigned int ud2 = 7u;
    printf("%u\n", ud1 / ud2);      // EXPECT: 14
    /* 100/7 = 14 remainder 2 */
    printf("%u\n", ud1 % ud2);      // EXPECT: 2

    /* Large unsigned division */
    /* 4294967295 / 2 = 2147483647, remainder 1 */
    unsigned int udiv = 4294967295u;
    printf("%u\n", udiv / 2u);      // EXPECT: 2147483647
    printf("%u\n", udiv % 2u);      // EXPECT: 1

    /* Unsigned comparison: 1 vs 4294967295 */
    unsigned int up = 1u;
    unsigned int un = 4294967295u;
    printf("%d\n", up < un);        // EXPECT: 1
    printf("%d\n", up > un);        // EXPECT: 0
    printf("%d\n", up == un);       // EXPECT: 0

    /* Sum that does NOT wrap: 3000000000 + 1000000000 = 4000000000 < 4294967296 */
    unsigned int v1 = 3000000000u;
    unsigned int v2 = 1000000000u;
    printf("%u\n", v1 + v2);        // EXPECT: 4000000000

    /* Sum that DOES wrap: 3000000000 + 2000000000 = 5000000000 mod 4294967296 = 705032704 */
    unsigned int v3 = 3000000000u;
    unsigned int v4 = 2000000000u;
    printf("%u\n", v3 + v4);        // EXPECT: 705032704

    /* Unsigned right shift fills with 0 */
    /* 0x80000000 = 2147483648 */
    unsigned int bits = 2147483648u;
    printf("%u\n", bits >> 1);      // EXPECT: 1073741824
    /* 2147483648 >> 1 = 1073741824 */
    printf("%u\n", bits >> 31);     // EXPECT: 1
    /* 2147483648 >> 31 = 1 */

    /* Unsigned left shift */
    unsigned int lsh = 1u;
    printf("%u\n", lsh << 10);      // EXPECT: 1024
    printf("%u\n", lsh << 20);      // EXPECT: 1048576
    printf("%u\n", lsh << 31);      // EXPECT: 2147483648

    /* Unsigned modulo with power of two */
    unsigned int val = 255u;
    printf("%u\n", val % 16u);      // EXPECT: 15
    /* 255 = 15*16 + 15 */
    printf("%u\n", val % 8u);       // EXPECT: 7
    /* 255 = 31*8 + 7 */

    /* Zero operations */
    printf("%u\n", 0u + 0u);        // EXPECT: 0
    printf("%u\n", 0u * 12345u);    // EXPECT: 0
    printf("%u\n", 100u - 100u);    // EXPECT: 0

    /* Accumulation without overflow: 4 * 1000000000 = 4000000000 */
    unsigned int acc = 0u;
    acc = acc + 1000000000u;
    acc = acc + 1000000000u;
    acc = acc + 1000000000u;
    acc = acc + 1000000000u;
    printf("%u\n", acc);            // EXPECT: 4000000000

    /* Accumulation with overflow: 2 * 2147483648 = 4294967296 mod 2^32 = 0 */
    unsigned int acc2 = 0u;
    acc2 = acc2 + 2147483648u;
    acc2 = acc2 + 2147483648u;
    printf("%u\n", acc2);           // EXPECT: 0

    /* Power of two checks */
    unsigned int pw = 1u;
    pw = pw * 2u;
    pw = pw * 2u;
    pw = pw * 2u;
    pw = pw * 2u;
    pw = pw * 2u;
    pw = pw * 2u;
    pw = pw * 2u;
    pw = pw * 2u;
    printf("%u\n", pw);             // EXPECT: 256
    /* 2^8 = 256 */

    /* Unsigned subtraction without borrow */
    printf("%u\n", 1000u - 1u);     // EXPECT: 999
    printf("%u\n", 256u - 256u);    // EXPECT: 0

    /* Division by 1 and self */
    printf("%u\n", 99999u / 1u);    // EXPECT: 99999
    printf("%u\n", 99999u / 99999u);// EXPECT: 1

    return 0;
}
