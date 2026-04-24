int printf(const char *fmt, ...);

/* Bit fields: pack/unpack, RGB color packing, IP address packing, flag registers */

/* Pack an RGB color into a single int: R in bits 16-23, G in bits 8-15, B in bits 0-7 */
int rgb_pack(int r, int g, int b) {
    return ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
}

int rgb_r(int color) { return (color >> 16) & 0xFF; }
int rgb_g(int color) { return (color >> 8)  & 0xFF; }
int rgb_b(int color) { return (color >> 0)  & 0xFF; }

/* Pack an IPv4 address from four octets */
unsigned int ip_pack(int a, int b, int c, int d) {
    return ((unsigned int)(a & 0xFF) << 24) |
           ((unsigned int)(b & 0xFF) << 16) |
           ((unsigned int)(c & 0xFF) << 8)  |
           ((unsigned int)(d & 0xFF));
}

int ip_octet(unsigned int ip, int n) {
    /* n=0 is most significant octet, n=3 is least */
    return (ip >> (24 - n * 8)) & 0xFF;
}

/* Flag register pattern: individual named bits */
#define FLAG_CARRY    (1 << 0)
#define FLAG_ZERO     (1 << 1)
#define FLAG_OVERFLOW (1 << 2)
#define FLAG_SIGN     (1 << 3)
#define FLAG_PARITY   (1 << 4)

int flag_get(int reg, int flag) { return (reg & flag) ? 1 : 0; }
int flag_set(int reg, int flag) { return reg | flag; }
int flag_clear(int reg, int flag) { return reg & ~flag; }

/* Pack two 4-bit nibbles into one byte */
int nibble_pack(int hi, int lo) {
    return ((hi & 0xF) << 4) | (lo & 0xF);
}

int nibble_hi(int byte) { return (byte >> 4) & 0xF; }
int nibble_lo(int byte) { return byte & 0xF; }

/* Pack four 8-bit values into one 32-bit int */
int pack32(int a, int b, int c, int d) {
    return ((a & 0xFF) << 24) | ((b & 0xFF) << 16) |
           ((c & 0xFF) << 8)  | (d & 0xFF);
}

int main(void) {
    /* --- RGB COLOR PACKING --- */
    int red   = rgb_pack(255, 0, 0);
    int green = rgb_pack(0, 255, 0);
    int blue  = rgb_pack(0, 0, 255);
    int white = rgb_pack(255, 255, 255);
    int gray  = rgb_pack(128, 128, 128);

    printf("%d\n", red);                     // EXPECT: 16711680
    /* 255<<16 = 16711680 */
    printf("%d\n", green);                   // EXPECT: 65280
    /* 255<<8 = 65280 */
    printf("%d\n", blue);                    // EXPECT: 255
    printf("%d\n", white);                   // EXPECT: 16777215
    /* 0xFFFFFF = 16777215 */
    printf("%d\n", gray);                    // EXPECT: 8421504
    /* 0x808080 = 128*65536 + 128*256 + 128 = 8388608+32768+128 = 8421504 */

    /* Round-trip extract */
    int color = rgb_pack(200, 100, 50);
    printf("%d\n", rgb_r(color));            // EXPECT: 200
    printf("%d\n", rgb_g(color));            // EXPECT: 100
    printf("%d\n", rgb_b(color));            // EXPECT: 50

    /* --- NIBBLE PACK/UNPACK --- */
    int nb = nibble_pack(0xA, 0x5);
    printf("%d\n", nb);                      // EXPECT: 165
    /* 0xA5 = 10*16 + 5 = 165 */
    printf("%d\n", nibble_hi(nb));           // EXPECT: 10
    printf("%d\n", nibble_lo(nb));           // EXPECT: 5

    nb = nibble_pack(0xF, 0x0);
    printf("%d\n", nb);                      // EXPECT: 240
    /* 0xF0 = 240 */
    printf("%d\n", nibble_hi(nb));           // EXPECT: 15
    printf("%d\n", nibble_lo(nb));           // EXPECT: 0

    nb = nibble_pack(0x3, 0xC);
    printf("%d\n", nb);                      // EXPECT: 60
    /* 0x3C = 3*16+12 = 60 */

    /* --- IP ADDRESS PACKING --- */
    unsigned int ip = ip_pack(192, 168, 1, 100);
    printf("%u\n", ip);                      // EXPECT: 3232235876
    /* 192*16777216 + 168*65536 + 1*256 + 100 */
    /* = 3221225472 + 11010048 + 256 + 100 = 3232235876 */

    printf("%d\n", ip_octet(ip, 0));         // EXPECT: 192
    printf("%d\n", ip_octet(ip, 1));         // EXPECT: 168
    printf("%d\n", ip_octet(ip, 2));         // EXPECT: 1
    printf("%d\n", ip_octet(ip, 3));         // EXPECT: 100

    unsigned int loopback = ip_pack(127, 0, 0, 1);
    printf("%u\n", loopback);                // EXPECT: 2130706433
    /* 127*16777216 + 0 + 0 + 1 = 2130706432 + 1 = 2130706433 */

    /* --- FLAG REGISTER --- */
    int flags = 0;
    printf("%d\n", flag_get(flags, FLAG_ZERO));    // EXPECT: 0

    flags = flag_set(flags, FLAG_ZERO);
    printf("%d\n", flag_get(flags, FLAG_ZERO));    // EXPECT: 1
    printf("%d\n", flag_get(flags, FLAG_CARRY));   // EXPECT: 0

    flags = flag_set(flags, FLAG_CARRY);
    flags = flag_set(flags, FLAG_SIGN);
    printf("%d\n", flag_get(flags, FLAG_CARRY));   // EXPECT: 1
    printf("%d\n", flag_get(flags, FLAG_SIGN));    // EXPECT: 1
    printf("%d\n", flag_get(flags, FLAG_OVERFLOW));// EXPECT: 0
    printf("%d\n", flags);                         // EXPECT: 11
    /* CARRY=1, ZERO=2, SIGN=8 => 1+2+8=11 */

    flags = flag_clear(flags, FLAG_ZERO);
    printf("%d\n", flag_get(flags, FLAG_ZERO));    // EXPECT: 0
    printf("%d\n", flags);                         // EXPECT: 9
    /* 11 - 2 = 9 */

    /* Set all flags */
    flags = FLAG_CARRY | FLAG_ZERO | FLAG_OVERFLOW | FLAG_SIGN | FLAG_PARITY;
    printf("%d\n", flags);                         // EXPECT: 31
    /* 1+2+4+8+16 = 31 */

    /* --- PACK32 --- */
    int packed = pack32(1, 2, 3, 4);
    printf("%d\n", packed);                        // EXPECT: 16909060
    /* 1*16777216 + 2*65536 + 3*256 + 4 = 16777216+131072+768+4 = 16909060 */

    packed = pack32(0xFF, 0xFE, 0xFD, 0xFC);
    /* 0xFFFEFDFC = 4294901244 but as signed int: negative */
    /* 0xFFFEFDFC = -66052 in two's complement */
    printf("%d\n", packed);                        // EXPECT: -66052
    /* 0xFFFFFFFF - 0xFFFEFDFC = 0x00010203 = 66051, so -66052 */

    /* Field extraction using mask and shift */
    int data = 0x12345678;
    printf("%d\n", (data >> 24) & 0xFF);           // EXPECT: 18
    /* 0x12 = 18 */
    printf("%d\n", (data >> 16) & 0xFF);           // EXPECT: 52
    /* 0x34 = 52 */
    printf("%d\n", (data >> 8) & 0xFF);            // EXPECT: 86
    /* 0x56 = 86 */
    printf("%d\n", data & 0xFF);                   // EXPECT: 120
    /* 0x78 = 120 */

    return 0;
}
