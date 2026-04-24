int printf(const char *fmt, ...);

int main(void) {
    /* AND operator */
    printf("%d\n", 0xFF & 0x0F);    // EXPECT: 15
    /* 11111111 & 00001111 = 00001111 = 15 */
    printf("%d\n", 0xAB & 0xF0);    // EXPECT: 160
    /* 10101011 & 11110000 = 10100000 = 160 */
    printf("%d\n", 12 & 10);        // EXPECT: 8
    /* 1100 & 1010 = 1000 = 8 */
    printf("%d\n", 255 & 0);        // EXPECT: 0
    printf("%d\n", 255 & 255);      // EXPECT: 255

    /* OR operator */
    printf("%d\n", 0xF0 | 0x0F);    // EXPECT: 255
    /* 11110000 | 00001111 = 11111111 = 255 */
    printf("%d\n", 12 | 3);         // EXPECT: 15
    /* 1100 | 0011 = 1111 = 15 */
    printf("%d\n", 0 | 42);         // EXPECT: 42
    printf("%d\n", 128 | 64);       // EXPECT: 192
    /* 10000000 | 01000000 = 11000000 = 192 */

    /* XOR operator */
    printf("%d\n", 0xFF ^ 0x0F);    // EXPECT: 240
    /* 11111111 ^ 00001111 = 11110000 = 240 */
    printf("%d\n", 12 ^ 10);        // EXPECT: 6
    /* 1100 ^ 1010 = 0110 = 6 */
    printf("%d\n", 42 ^ 42);        // EXPECT: 0
    /* anything XOR itself = 0 */
    printf("%d\n", 0 ^ 99);         // EXPECT: 99
    /* 0 XOR x = x */

    /* NOT (bitwise complement) */
    /* ~0 = all bits set = -1 as signed */
    printf("%d\n", ~0);             // EXPECT: -1
    printf("%d\n", ~1);             // EXPECT: -2
    /* ~00000001 = 11111110 = -2 as signed */
    printf("%d\n", ~(-1));          // EXPECT: 0
    /* ~11...1 = 00...0 = 0 */
    printf("%d\n", ~127);           // EXPECT: -128
    /* ~01111111 = 10000000 = -128 */
    printf("%d\n", ~(-128));        // EXPECT: 127

    /* Left shift */
    printf("%d\n", 1 << 0);         // EXPECT: 1
    printf("%d\n", 1 << 1);         // EXPECT: 2
    printf("%d\n", 1 << 4);         // EXPECT: 16
    printf("%d\n", 1 << 8);         // EXPECT: 256
    printf("%d\n", 1 << 16);        // EXPECT: 65536
    printf("%d\n", 3 << 3);         // EXPECT: 24
    /* 3 * 2^3 = 3 * 8 = 24 */
    printf("%d\n", 5 << 2);         // EXPECT: 20
    /* 5 * 4 = 20 */

    /* Right shift (arithmetic for signed) */
    printf("%d\n", 256 >> 1);       // EXPECT: 128
    printf("%d\n", 256 >> 4);       // EXPECT: 16
    printf("%d\n", 256 >> 8);       // EXPECT: 1
    printf("%d\n", 100 >> 2);       // EXPECT: 25
    /* 100 / 4 = 25 */
    printf("%d\n", 7 >> 1);         // EXPECT: 3
    /* 7 / 2 = 3 (truncate) */
    printf("%d\n", -8 >> 1);        // EXPECT: -4
    /* arithmetic right shift: -8 / 2 = -4 */
    printf("%d\n", -1 >> 1);        // EXPECT: -1
    /* arithmetic right shift of -1 stays -1 */

    /* Bit manipulation patterns */
    /* Set bit n: x | (1 << n) */
    int x = 0;
    x = x | (1 << 3);
    printf("%d\n", x);              // EXPECT: 8
    x = x | (1 << 5);
    printf("%d\n", x);              // EXPECT: 40
    /* 8 | 32 = 40 */

    /* Clear bit n: x & ~(1 << n) */
    int y = 0xFF;
    y = y & ~(1 << 4);
    printf("%d\n", y);              // EXPECT: 239
    /* 11111111 & ~00010000 = 11111111 & 11101111 = 11101111 = 239 */

    /* Toggle bit n: x ^ (1 << n) */
    int z = 0;
    z = z ^ (1 << 7);
    printf("%d\n", z);              // EXPECT: 128
    z = z ^ (1 << 7);
    printf("%d\n", z);              // EXPECT: 0

    /* Check if bit is set */
    int flags = 0b00101010;         /* = 42 */
    printf("%d\n", (flags >> 1) & 1); // EXPECT: 1
    /* bit 1 of 42 (0b00101010) = 1 */
    printf("%d\n", (flags >> 3) & 1); // EXPECT: 1
    /* bit 3 of 42 (0b00101010) = 1 */
    printf("%d\n", (flags >> 2) & 1); // EXPECT: 0
    /* bit 2 of 42 (0b00101010) = 0 */

    /* Population count (count set bits) in 0b10110101 = 181 */
    int n = 181;
    int count = 0;
    count = count + (n & 1);        /* bit 0: 1 */
    count = count + ((n >> 1) & 1); /* bit 1: 0 */
    count = count + ((n >> 2) & 1); /* bit 2: 1 */
    count = count + ((n >> 3) & 1); /* bit 3: 0 */
    count = count + ((n >> 4) & 1); /* bit 4: 1 */
    count = count + ((n >> 5) & 1); /* bit 5: 1 */
    count = count + ((n >> 6) & 1); /* bit 6: 0 */
    count = count + ((n >> 7) & 1); /* bit 7: 1 */
    printf("%d\n", count);          // EXPECT: 5
    /* 181 = 10110101: bits set at positions 0,2,4,5,7 => 5 bits */

    /* Extract nibbles */
    int byte_val = 0xAB;            /* 171 */
    printf("%d\n", byte_val & 0xF);     // EXPECT: 11
    /* low nibble: 0xB = 11 */
    printf("%d\n", (byte_val >> 4) & 0xF); // EXPECT: 10
    /* high nibble: 0xA = 10 */

    /* Combine nibbles */
    int low = 5, high = 9;
    int combined = (high << 4) | low;
    printf("%d\n", combined);       // EXPECT: 149
    /* (9 << 4) | 5 = 144 | 5 = 149 = 0x95 */

    /* Mask and shift: extract bits 4-7 of value */
    int data = 0b11010110;          /* 214 */
    int masked = (data >> 4) & 0xF;
    printf("%d\n", masked);         // EXPECT: 13
    /* bits 4-7: 0b1101 = 13 */

    return 0;
}
