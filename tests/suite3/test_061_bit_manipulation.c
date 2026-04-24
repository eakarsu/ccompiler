int printf(const char *fmt, ...);

/* Bit manipulation: set/clear/toggle/test, popcount, clz, ctz, is_power_of_2 */

/* Count set bits (popcount) using Brian Kernighan's method */
int popcount(unsigned int n) {
    int count = 0;
    while (n) {
        n = n & (n - 1);
        count++;
    }
    return count;
}

/* Count trailing zeros (position of lowest set bit) */
int ctz(unsigned int n) {
    if (n == 0) return 32;
    int count = 0;
    while ((n & 1) == 0) {
        n = n >> 1;
        count++;
    }
    return count;
}

/* Count leading zeros for 32-bit value */
int clz(unsigned int n) {
    if (n == 0) return 32;
    int count = 0;
    unsigned int mask = 1u << 31;
    while ((n & mask) == 0) {
        mask = mask >> 1;
        count++;
    }
    return count;
}

/* Is power of 2 (nonzero and exactly one bit set) */
int is_pow2(unsigned int n) {
    return (n != 0) && ((n & (n - 1)) == 0);
}

int main(void) {
    unsigned int x;

    /* --- SET BIT --- */
    x = 0;
    x = x | (1u << 3);    /* set bit 3 */
    printf("%u\n", x);                       // EXPECT: 8
    x = x | (1u << 7);    /* set bit 7 */
    printf("%u\n", x);                       // EXPECT: 136
    x = x | (1u << 0);    /* set bit 0 */
    printf("%u\n", x);                       // EXPECT: 137

    /* --- CLEAR BIT --- */
    x = 0xFFu;             /* 255, all low 8 bits set */
    x = x & ~(1u << 4);   /* clear bit 4 */
    printf("%u\n", x);                       // EXPECT: 239
    /* 0xFF = 255, ~(1<<4) = ~16 clears bit4: 255-16=239 */
    x = x & ~(1u << 0);   /* clear bit 0 */
    printf("%u\n", x);                       // EXPECT: 238

    /* --- TOGGLE BIT --- */
    x = 0;
    x = x ^ (1u << 5);    /* toggle bit 5 on */
    printf("%u\n", x);                       // EXPECT: 32
    x = x ^ (1u << 5);    /* toggle bit 5 off */
    printf("%u\n", x);                       // EXPECT: 0
    x = 0b10101010u;       /* 170 */
    x = x ^ 0xFFu;        /* toggle all low 8 bits */
    printf("%u\n", x);                       // EXPECT: 85
    /* 0xAA ^ 0xFF = 0x55 = 85 */

    /* --- TEST BIT --- */
    unsigned int val = 0b01101001u;  /* 105 */
    printf("%d\n", (val >> 0) & 1);          // EXPECT: 1
    printf("%d\n", (val >> 1) & 1);          // EXPECT: 0
    printf("%d\n", (val >> 3) & 1);          // EXPECT: 1
    printf("%d\n", (val >> 5) & 1);          // EXPECT: 1
    printf("%d\n", (val >> 7) & 1);          // EXPECT: 0
    /* 0b01101001 = 105: bits 0,3,5,6 set */

    /* --- POPCOUNT --- */
    printf("%d\n", popcount(0));             // EXPECT: 0
    printf("%d\n", popcount(1));             // EXPECT: 1
    printf("%d\n", popcount(0xFFu));         // EXPECT: 8
    printf("%d\n", popcount(0xAAAAAAAAu));   // EXPECT: 16
    /* 0xAAAAAAAA = alternating 10 pattern, 16 bits set in 32 bits */
    printf("%d\n", popcount(0b10110101u));   // EXPECT: 5
    /* bits: 0,2,4,5,7 => 5 set */
    printf("%d\n", popcount(255u * 256u + 255u)); /* 0xFFFF */
    /* = popcount(65535) = 16 */                  // EXPECT: 16

    /* --- TRAILING ZEROS --- */
    printf("%d\n", ctz(1u));                 // EXPECT: 0
    printf("%d\n", ctz(2u));                 // EXPECT: 1
    printf("%d\n", ctz(16u));                // EXPECT: 4
    printf("%d\n", ctz(96u));               // EXPECT: 5
    /* 96 = 0b1100000, lowest set bit at position 5 */
    printf("%d\n", ctz(256u));               // EXPECT: 8

    /* --- LEADING ZEROS --- */
    printf("%d\n", clz(0x80000000u));        // EXPECT: 0
    printf("%d\n", clz(0x40000000u));        // EXPECT: 1
    printf("%d\n", clz(1u));                 // EXPECT: 31
    printf("%d\n", clz(255u));               // EXPECT: 24
    /* 255 = 0x000000FF, 24 leading zeros */
    printf("%d\n", clz(65536u));             // EXPECT: 15
    /* 65536 = 2^16 = 0x00010000, 15 leading zeros */

    /* --- IS POWER OF 2 --- */
    printf("%d\n", is_pow2(0));              // EXPECT: 0
    printf("%d\n", is_pow2(1));              // EXPECT: 1
    printf("%d\n", is_pow2(2));              // EXPECT: 1
    printf("%d\n", is_pow2(3));              // EXPECT: 0
    printf("%d\n", is_pow2(4));              // EXPECT: 1
    printf("%d\n", is_pow2(5));              // EXPECT: 0
    printf("%d\n", is_pow2(256));            // EXPECT: 1
    printf("%d\n", is_pow2(255));            // EXPECT: 0
    printf("%d\n", is_pow2(1024));           // EXPECT: 1
    printf("%d\n", is_pow2(1023));           // EXPECT: 0

    /* --- NEXT POWER OF 2 --- */
    /* Round up to next power of 2 using bit tricks */
    unsigned int n = 100u;
    n = n - 1;
    n = n | (n >> 1);
    n = n | (n >> 2);
    n = n | (n >> 4);
    n = n | (n >> 8);
    n = n | (n >> 16);
    n = n + 1;
    printf("%u\n", n);                       // EXPECT: 128
    /* next power of 2 after 100 is 128 */

    /* --- BIT REVERSAL (8-bit) --- */
    /* reverse bits of a byte manually */
    unsigned int b = 0b10110001u;  /* 177 */
    unsigned int r = 0;
    r = r | ((b >> 0) & 1) << 7;
    r = r | ((b >> 1) & 1) << 6;
    r = r | ((b >> 2) & 1) << 5;
    r = r | ((b >> 3) & 1) << 4;
    r = r | ((b >> 4) & 1) << 3;
    r = r | ((b >> 5) & 1) << 2;
    r = r | ((b >> 6) & 1) << 1;
    r = r | ((b >> 7) & 1) << 0;
    printf("%u\n", r);                       // EXPECT: 141
    /* 0b10110001 reversed = 0b10001101 = 141 */

    return 0;
}
