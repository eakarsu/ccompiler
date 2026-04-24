int printf(const char *fmt, ...);

/* Sign extension: widening signed types (sign-extend) vs unsigned (zero-extend),
   casting negative values, byte extraction patterns */

int main(void) {
    /* --- SIGNED CHAR -> INT (SIGN EXTENSION) --- */
    /* Positive signed chars: zero-fill upper bits */
    signed char sc1 = 1;
    int i1 = (int)sc1;
    printf("%d\n", i1);                     // EXPECT: 1

    signed char sc2 = 127;    /* max signed char */
    int i2 = (int)sc2;
    printf("%d\n", i2);                     // EXPECT: 127

    /* Negative signed chars: sign-extend (fill with 1s) */
    signed char sc3 = -1;     /* 0xFF in binary */
    int i3 = (int)sc3;
    printf("%d\n", i3);                     // EXPECT: -1

    signed char sc4 = -128;   /* 0x80, min signed char */
    int i4 = (int)sc4;
    printf("%d\n", i4);                     // EXPECT: -128

    signed char sc5 = -64;    /* 0xC0 */
    int i5 = (int)sc5;
    printf("%d\n", i5);                     // EXPECT: -64

    /* --- UNSIGNED CHAR -> INT (ZERO EXTENSION) --- */
    unsigned char uc1 = 1;
    int ui1 = (int)uc1;
    printf("%d\n", ui1);                    // EXPECT: 1

    unsigned char uc2 = 127;
    int ui2 = (int)uc2;
    printf("%d\n", ui2);                    // EXPECT: 127

    /* Values >= 128: these would be negative if treated as signed char */
    unsigned char uc3 = 128;  /* 0x80 */
    int ui3 = (int)uc3;
    printf("%d\n", ui3);                    // EXPECT: 128

    unsigned char uc4 = 255;  /* 0xFF */
    int ui4 = (int)uc4;
    printf("%d\n", ui4);                    // EXPECT: 255

    unsigned char uc5 = 200;
    int ui5 = (int)uc5;
    printf("%d\n", ui5);                    // EXPECT: 200

    /* --- CONTRAST: SAME BIT PATTERN, DIFFERENT TYPE --- */
    /* 0xFF: as signed char = -1, as unsigned char = 255 */
    signed char   scFF = (signed char)0xFF;
    unsigned char ucFF = (unsigned char)0xFF;
    printf("%d\n", (int)scFF);              // EXPECT: -1
    printf("%d\n", (int)ucFF);             // EXPECT: 255

    /* 0x80: as signed char = -128, as unsigned char = 128 */
    signed char   sc80 = (signed char)0x80;
    unsigned char uc80 = (unsigned char)0x80;
    printf("%d\n", (int)sc80);              // EXPECT: -128
    printf("%d\n", (int)uc80);             // EXPECT: 128

    /* --- INT -> LONG (SIGN EXTENSION) using int cast patterns --- */
    /* We simulate with int widening: negative int to unsigned int is zero-extend pattern */
    int neg_int = -1;
    unsigned int as_uint = (unsigned int)neg_int;
    printf("%u\n", as_uint);               // EXPECT: 4294967295
    /* -1 in 32-bit two's complement = 0xFFFFFFFF = 4294967295 */

    int neg_int2 = -128;
    unsigned int as_uint2 = (unsigned int)neg_int2;
    printf("%u\n", as_uint2);              // EXPECT: 4294967168
    /* -128 = 0xFFFFFF80 = 4294967168 */

    int neg_int3 = -32768;
    unsigned int as_uint3 = (unsigned int)neg_int3;
    printf("%u\n", as_uint3);              // EXPECT: 4294934528
    /* -32768 = 0xFFFF8000 = 4294934528 */

    /* --- CASTING NEGATIVE LARGER INT DOWN TO SMALLER --- */
    int big_neg = -1;
    char as_char = (char)big_neg;
    printf("%d\n", (int)as_char);          // EXPECT: -1
    /* -1 = 0xFFFFFFFF, low byte = 0xFF = -1 as signed char */

    int val = -257;     /* 0xFFFFFEFF */
    char as_char2 = (char)val;
    printf("%d\n", (int)as_char2);         // EXPECT: -1
    /* low byte of 0xFFFFFEFF = 0xFF = -1 as signed char */

    int val2 = -256;    /* 0xFFFFFF00 */
    char as_char3 = (char)val2;
    printf("%d\n", (int)as_char3);         // EXPECT: 0
    /* low byte = 0x00 = 0 */

    int val3 = -255;    /* 0xFFFFFF01 */
    char as_char4 = (char)val3;
    printf("%d\n", (int)as_char4);         // EXPECT: 1
    /* low byte = 0x01 = 1 */

    /* --- SIGN EXTENSION PATTERN USING SHIFTS --- */
    /* Manually sign-extend 8-bit value stored in int to full 32-bit */
    int raw = 0xFF;          /* stored as int, value 255 */
    /* To sign-extend: shift left 24, then arithmetic right shift 24 */
    int sign_ext = (raw << 24) >> 24;
    printf("%d\n", sign_ext);              // EXPECT: -1
    /* 0xFF << 24 = 0xFF000000 (negative), >> 24 arithmetic = 0xFFFFFFFF = -1 */

    raw = 0x80;
    sign_ext = (raw << 24) >> 24;
    printf("%d\n", sign_ext);              // EXPECT: -128

    raw = 0x7F;
    sign_ext = (raw << 24) >> 24;
    printf("%d\n", sign_ext);              // EXPECT: 127

    raw = 0x40;
    sign_ext = (raw << 24) >> 24;
    printf("%d\n", sign_ext);             // EXPECT: 64

    /* --- SIGN-EXTEND 16-BIT STORED AS INT --- */
    int raw16 = 0xFFFF;      /* 65535 as unsigned */
    int se16 = (raw16 << 16) >> 16;
    printf("%d\n", se16);                 // EXPECT: -1

    raw16 = 0x8000;          /* 32768 as unsigned, -32768 as signed short */
    se16 = (raw16 << 16) >> 16;
    printf("%d\n", se16);                 // EXPECT: -32768

    raw16 = 0x7FFF;          /* 32767 */
    se16 = (raw16 << 16) >> 16;
    printf("%d\n", se16);                 // EXPECT: 32767

    /* --- USEFUL PATTERN: extract signed byte from packed int --- */
    int packed = 0x12FF5678;
    /* Extract byte 1 (bits 8-15) as signed: */
    int byte1 = (packed >> 8) & 0xFF;    /* unsigned extract: 0x56 = 86 */
    printf("%d\n", byte1);               // EXPECT: 86

    /* Extract byte 2 (bits 16-23): 0xFF = 255 unsigned */
    int byte2_u = (packed >> 16) & 0xFF;
    printf("%d\n", byte2_u);             // EXPECT: 255

    /* Sign-extend that byte */
    int byte2_s = ((packed << 8) >> 24);
    printf("%d\n", byte2_s);             // EXPECT: -1
    /* packed = 0x12FF5678, << 8 = 0xFF567800, >> 24 arithmetic = 0xFFFFFFFF = -1 */

    return 0;
}
