int printf(const char *fmt, ...);

/* Shift patterns: left/right shifts, arithmetic vs logical, rotate emulation,
   shift-based multiply/divide */

/* Rotate left for 32-bit unsigned */
unsigned int rotl32(unsigned int val, int n) {
    n = n & 31;   /* keep in [0,31] */
    if (n == 0) return val;
    return (val << n) | (val >> (32 - n));
}

/* Rotate right for 32-bit unsigned */
unsigned int rotr32(unsigned int val, int n) {
    n = n & 31;
    if (n == 0) return val;
    return (val >> n) | (val << (32 - n));
}

int main(void) {
    /* --- LEFT SHIFT --- */
    int x = 1;
    printf("%d\n", x << 0);                // EXPECT: 1
    printf("%d\n", x << 1);                // EXPECT: 2
    printf("%d\n", x << 2);                // EXPECT: 4
    printf("%d\n", x << 3);                // EXPECT: 8
    printf("%d\n", x << 10);               // EXPECT: 1024
    printf("%d\n", x << 16);               // EXPECT: 65536
    printf("%d\n", x << 20);               // EXPECT: 1048576

    /* Multiply by powers of 2 via shift */
    int n = 7;
    printf("%d\n", n << 1);                // EXPECT: 14
    printf("%d\n", n << 2);                // EXPECT: 28
    printf("%d\n", n << 3);                // EXPECT: 56
    printf("%d\n", n << 4);                // EXPECT: 112

    /* Left shift on negative values: -1 << k = -(2^k) when high bit not affected */
    int neg = -1;
    printf("%d\n", neg << 1);              // EXPECT: -2
    printf("%d\n", neg << 2);              // EXPECT: -4
    printf("%d\n", neg << 3);              // EXPECT: -8

    /* --- RIGHT SHIFT ON SIGNED (ARITHMETIC) --- */
    /* On ARM64 (and virtually all C11 platforms) right-shift of signed is arithmetic */
    int pos = 16;
    printf("%d\n", pos >> 1);              // EXPECT: 8
    printf("%d\n", pos >> 2);              // EXPECT: 4
    printf("%d\n", pos >> 3);              // EXPECT: 2
    printf("%d\n", pos >> 4);              // EXPECT: 1

    int neg2 = -16;
    printf("%d\n", neg2 >> 1);             // EXPECT: -8
    printf("%d\n", neg2 >> 2);             // EXPECT: -4
    printf("%d\n", neg2 >> 3);             // EXPECT: -2
    printf("%d\n", neg2 >> 4);             // EXPECT: -1

    /* Arithmetic right shift of -1 stays -1 (all ones) */
    printf("%d\n", (-1) >> 1);             // EXPECT: -1
    printf("%d\n", (-1) >> 16);            // EXPECT: -1
    printf("%d\n", (-1) >> 30);            // EXPECT: -1

    /* --- RIGHT SHIFT ON UNSIGNED (LOGICAL) --- */
    unsigned int u = 0xFFFFFFFFu;          /* all bits set */
    printf("%u\n", u >> 1);                // EXPECT: 2147483647
    /* 0xFFFFFFFF >> 1 = 0x7FFFFFFF = 2147483647 */
    printf("%u\n", u >> 8);                // EXPECT: 16777215
    /* 0xFFFFFFFF >> 8 = 0x00FFFFFF = 16777215 */
    printf("%u\n", u >> 24);               // EXPECT: 255

    unsigned int u2 = 0x80000000u;         /* only MSB set */
    printf("%u\n", u2 >> 1);               // EXPECT: 1073741824
    /* 0x80000000 >> 1 = 0x40000000 = 1073741824 */
    printf("%u\n", u2 >> 31);              // EXPECT: 1
    printf("%u\n", u2 >> 0);               // EXPECT: 2147483648

    /* --- SHIFT BY 0 --- */
    int val_42 = 42;
    printf("%d\n", val_42 >> 0);           // EXPECT: 42
    printf("%d\n", val_42 << 0);           // EXPECT: 42
    int val_neg42 = -42;
    printf("%d\n", val_neg42 >> 0);        // EXPECT: -42

    /* --- ROTATE EMULATION --- */
    unsigned int v = 0x12345678u;
    printf("%u\n", rotl32(v, 4));          // EXPECT: 591751041
    /* 0x12345678 rotl 4:
       top 4 bits (0001) move to bottom, rest shift left
       = 0x23456781 = 591751041 */

    printf("%u\n", rotr32(v, 4));          // EXPECT: 2166572391
    /* 0x12345678 rotr 4:
       bottom 4 bits (1000) move to top
       = 0x81234567 = 2166572391 */

    printf("%u\n", rotl32(0x80000000u, 1)); // EXPECT: 1
    /* MSB rotated left 1 wraps to bit 0 = 1 */
    printf("%u\n", rotr32(1u, 1));          // EXPECT: 2147483648
    /* bit 0 rotated right 1 = MSB = 0x80000000 = 2147483648 */

    printf("%u\n", rotl32(0xABCDEF01u, 8)); // EXPECT: 3454992811
    /* 0xABCDEF01 rotl 8 = 0xCDEF01AB = 3454992811 */

    /* --- SHIFT-BASED MULTIPLY AND DIVIDE --- */
    /* Multiply by non-power-of-2: n * 10 = n*8 + n*2 */
    int m = 13;
    int times10 = (m << 3) + (m << 1);    /* 8*13 + 2*13 = 104+26 = 130 */
    printf("%d\n", times10);               // EXPECT: 130

    /* Multiply by 3: (n<<1) + n */
    int times3 = (m << 1) + m;            /* 26+13 = 39 */
    printf("%d\n", times3);               // EXPECT: 39

    /* Multiply by 5: (n<<2) + n */
    int times5 = (m << 2) + m;            /* 52+13 = 65 */
    printf("%d\n", times5);               // EXPECT: 65

    /* Divide by power of 2 via shift (positive values) */
    int p = 200;
    printf("%d\n", p >> 1);               // EXPECT: 100
    printf("%d\n", p >> 2);               // EXPECT: 50
    printf("%d\n", p >> 3);               // EXPECT: 25

    /* Mod by power of 2 via mask */
    printf("%d\n", p & (8 - 1));          // EXPECT: 0
    /* 200 % 8 = 0 */
    printf("%d\n", 201 & (8 - 1));        // EXPECT: 1
    printf("%d\n", 207 & (8 - 1));        // EXPECT: 7

    /* --- SHIFT TO EXTRACT FIELDS --- */
    int packed = 0x05060708;
    printf("%d\n", (packed >> 24) & 0xFF); // EXPECT: 5
    printf("%d\n", (packed >> 16) & 0xFF); // EXPECT: 6
    printf("%d\n", (packed >> 8) & 0xFF);  // EXPECT: 7
    printf("%d\n", packed & 0xFF);         // EXPECT: 8

    /* --- SHIFT AND MASK COMBINED PATTERNS --- */
    /* Extract bits [11:4] from value (8-bit field) */
    int data = 0b111011010000;  /* = 3792 */
    /* >> 4 = 0b11101101 = 237; & 0xFF = 237 */
    int field = (data >> 4) & 0xFF;
    printf("%d\n", field);                 // EXPECT: 237

    /* Interleave: combine two 4-bit fields into a byte */
    int hi4 = 0xB;             /* 11 */
    int lo4 = 0x7;             /* 7 */
    int byte_val = (hi4 << 4) | lo4;
    printf("%d\n", byte_val);             // EXPECT: 183
    /* 0xB7 = 11*16 + 7 = 183 */

    return 0;
}
