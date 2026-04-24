int printf(const char *fmt, ...);

/* Byte extraction from int, byte packing, byte reversal patterns */

/* Extract individual bytes from a 32-bit value (big-endian order: byte 0 = MSB) */
int byte_at(unsigned int val, int n) {
    /* n=0 is MSB, n=3 is LSB */
    return (val >> (24 - n * 8)) & 0xFF;
}

/* Reverse bytes in a 32-bit integer */
unsigned int bswap32(unsigned int val) {
    return ((val & 0xFF000000u) >> 24) |
           ((val & 0x00FF0000u) >> 8)  |
           ((val & 0x0000FF00u) << 8)  |
           ((val & 0x000000FFu) << 24);
}

/* Reverse bytes in a 16-bit value (stored in unsigned int) */
unsigned int bswap16(unsigned int val) {
    return ((val & 0xFF00u) >> 8) | ((val & 0x00FFu) << 8);
}

/* Pack four bytes into a 32-bit big-endian integer */
unsigned int pack_be32(int b0, int b1, int b2, int b3) {
    return ((unsigned int)(b0 & 0xFF) << 24) |
           ((unsigned int)(b1 & 0xFF) << 16) |
           ((unsigned int)(b2 & 0xFF) << 8)  |
           ((unsigned int)(b3 & 0xFF));
}

/* Pack four bytes into a 32-bit little-endian integer */
unsigned int pack_le32(int b0, int b1, int b2, int b3) {
    /* b0 is least significant byte */
    return ((unsigned int)(b3 & 0xFF) << 24) |
           ((unsigned int)(b2 & 0xFF) << 16) |
           ((unsigned int)(b1 & 0xFF) << 8)  |
           ((unsigned int)(b0 & 0xFF));
}

int main(void) {
    /* --- BYTE EXTRACTION --- */
    unsigned int val = 0x12345678u;
    printf("%d\n", byte_at(val, 0));       // EXPECT: 18
    /* 0x12 = 18 */
    printf("%d\n", byte_at(val, 1));       // EXPECT: 52
    /* 0x34 = 52 */
    printf("%d\n", byte_at(val, 2));       // EXPECT: 86
    /* 0x56 = 86 */
    printf("%d\n", byte_at(val, 3));       // EXPECT: 120
    /* 0x78 = 120 */

    /* Using direct shift+mask */
    unsigned int v2 = 0xDEADBEEFu;
    printf("%u\n", (v2 >> 24) & 0xFFu);   // EXPECT: 222
    /* 0xDE = 222 */
    printf("%u\n", (v2 >> 16) & 0xFFu);   // EXPECT: 173
    /* 0xAD = 173 */
    printf("%u\n", (v2 >> 8) & 0xFFu);    // EXPECT: 190
    /* 0xBE = 190 */
    printf("%u\n", v2 & 0xFFu);           // EXPECT: 239
    /* 0xEF = 239 */

    /* --- BYTE PACKING --- */
    unsigned int packed = pack_be32(0x11, 0x22, 0x33, 0x44);
    printf("%u\n", packed);                // EXPECT: 287454020
    /* 0x11223344 = 17*16777216 + 34*65536 + 51*256 + 68 */
    /* = 285212672 + 2228224 + 13056 + 68 = 287454020 */

    unsigned int packed_le = pack_le32(0x11, 0x22, 0x33, 0x44);
    printf("%u\n", packed_le);             // EXPECT: 1144201745
    /* LE: b0=0x11 at bits[7:0], b1=0x22 at bits[15:8], b2=0x33 at bits[23:16], b3=0x44 at bits[31:24] */
    /* = 0x44332211 = 68*16777216 + 51*65536 + 34*256 + 17 */
    /* = 1140850688 + 3342336 + 8704 + 17 = 1144201745 */

    /* --- BYTE REVERSAL --- */
    printf("%u\n", bswap32(0x12345678u));  // EXPECT: 2018915346
    /* 0x78563412 = 120*16777216 + 86*65536 + 52*256 + 18 */
    /* = 2013265920 + 5636096 + 13312 + 18 = 2018915346 */

    printf("%u\n", bswap32(0x00000001u));  // EXPECT: 16777216
    /* 0x01000000 = 16777216 */

    printf("%u\n", bswap32(0xFF000000u));  // EXPECT: 255
    /* 0x000000FF = 255 */

    printf("%u\n", bswap32(0xDEADBEEFu)); // EXPECT: 4022250974
    /* 0xEFBEADDE = 239*16777216 + 190*65536 + 173*256 + 222 */
    /* = 4009754624 + 12451840 + 44288 + 222 = 4022250974 */

    printf("%u\n", bswap16(0x1234u));      // EXPECT: 13330
    /* 0x3412 = 52*256 + 18 = 13312 + 18 = 13330 */

    printf("%u\n", bswap16(0xFF00u));      // EXPECT: 255
    /* 0x00FF = 255 */

    printf("%u\n", bswap16(0x0080u));      // EXPECT: 32768
    /* 0x8000 = 32768 */

    /* --- BYTE MANIPULATION PATTERNS --- */
    /* Replace byte 0 (MSB) of a 32-bit value */
    unsigned int orig = 0x11223344u;
    unsigned int replaced = (orig & 0x00FFFFFFu) | (0xAAu << 24);
    printf("%u\n", replaced);              // EXPECT: 2854368068
    /* 0xAA223344 = 170*16777216 + 34*65536 + 51*256 + 68 */
    /* = 2852126720 + 2228224 + 13056 + 68 = 2854368068 */

    /* Replace byte 3 (LSB) */
    unsigned int rep2 = (orig & 0xFFFFFF00u) | 0x77u;
    printf("%u\n", rep2);                  // EXPECT: 287454071
    /* 0x11223377 = 0x11223300 + 0x77 = 287453952 + 119 = 287454071 */
    /* Wait: 0x11223344 & 0xFFFFFF00 = 0x11223300; | 0x77 = 0x11223377 */
    /* 0x11223377 = 17*16777216 + 34*65536 + 51*256 + 119 */
    /* = 285212672 + 2228224 + 13056 + 119 = 287454071 */

    /* --- NIBBLE EXTRACTION FROM BYTES --- */
    unsigned int byte_val = 0xABu;
    printf("%u\n", (byte_val >> 4) & 0xFu); // EXPECT: 10
    /* high nibble of 0xAB = 0xA = 10 */
    printf("%u\n", byte_val & 0xFu);         // EXPECT: 11
    /* low nibble of 0xAB = 0xB = 11 */

    /* --- ROUND-TRIP: pack then unpack --- */
    int r = 200, g = 150, b = 100;
    unsigned int color = ((unsigned int)r << 16) | ((unsigned int)g << 8) | (unsigned int)b;
    printf("%d\n", (int)((color >> 16) & 0xFF)); // EXPECT: 200
    printf("%d\n", (int)((color >> 8)  & 0xFF)); // EXPECT: 150
    printf("%d\n", (int)(color & 0xFF));          // EXPECT: 100

    /* --- XOR-BASED BYTE SWAP --- */
    /* Swap high and low bytes of a 16-bit value using XOR */
    unsigned int word = 0xABCDu;  /* 43981 */
    unsigned int hi = (word >> 8) & 0xFFu;   /* 0xAB = 171 */
    unsigned int lo = word & 0xFFu;           /* 0xCD = 205 */
    unsigned int swapped = (lo << 8) | hi;
    printf("%u\n", swapped);                  // EXPECT: 52651
    /* 0xCDAB = 205*256 + 171 = 52480 + 171 = 52651 */

    /* --- CONSTRUCTING MAGIC BYTES --- */
    /* Build 0xCAFEBABE */
    unsigned int magic = pack_be32(0xCA, 0xFE, 0xBA, 0xBE);
    printf("%u\n", magic);                    // EXPECT: 3405691582
    /* 0xCAFEBABE = 202*16777216 + 254*65536 + 186*256 + 190 */
    /* = 3388997632 + 16646144 + 47616 + 190 = 3405691582 */

    return 0;
}
