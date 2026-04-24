int printf(const char *fmt, ...);

/* Type promotion rules: char+char=int, short+short=int,
   unsigned+signed, comparisons, sizeof arithmetic */

int main(void) {
    /* --- CHAR ARITHMETIC PROMOTES TO INT --- */
    /* char + char: both chars are promoted to int before the operation */
    char a = 100;
    char b = 100;
    int sum_ab = a + b;            /* 100+100=200, fine as int */
    printf("%d\n", sum_ab);                  // EXPECT: 200

    char ca = 127;
    char cb = 1;
    int sum_c = ca + cb;           /* 127+1=128, fine as int (not char overflow) */
    printf("%d\n", sum_c);                   // EXPECT: 128

    /* Negative char promotion */
    char neg = -1;
    int promoted = neg;            /* sign-extended: -1 as int */
    printf("%d\n", promoted);               // EXPECT: -1

    char cn = -100;
    char cp = 50;
    int diff = cn + cp;            /* -100+50 = -50 */
    printf("%d\n", diff);                    // EXPECT: -50

    /* char * int: char is promoted */
    char ch = 10;
    int big = 1000;
    int prod = ch * big;           /* 10000 */
    printf("%d\n", prod);                    // EXPECT: 10000

    /* --- UNSIGNED CHAR ZERO-EXTENDS --- */
    unsigned char uc = 200;
    int uci = uc;                  /* zero-extended: 200 */
    printf("%d\n", uci);                     // EXPECT: 200

    unsigned char uc2 = 255;
    int uci2 = uc2;                /* 255, not -1 */
    printf("%d\n", uci2);                    // EXPECT: 255

    /* unsigned char + unsigned char */
    unsigned char ua = 200;
    unsigned char ub = 100;
    int usum = ua + ub;            /* 300, no overflow as int */
    printf("%d\n", usum);                    // EXPECT: 300

    /* --- SHORT ARITHMETIC PROMOTES TO INT --- */
    short sa = 30000;
    short sb = 30000;
    int ssum = sa + sb;            /* 60000, fine as int */
    printf("%d\n", ssum);                    // EXPECT: 60000

    short sc = -20000;
    short sd = 10000;
    int sdiff = sc + sd;           /* -10000 */
    printf("%d\n", sdiff);                   // EXPECT: -10000

    /* --- UNSIGNED + SIGNED CONVERSION --- */
    /* When mixing signed and unsigned of same width, signed converts to unsigned */
    unsigned int ui = 1u;
    int si = -1;
    /* ui + (unsigned)si = 1 + 4294967295 = 4294967296, wraps to 0 in 32-bit */
    unsigned int ures = ui + (unsigned int)si;
    printf("%u\n", ures);                    // EXPECT: 0
    /* 1 + 0xFFFFFFFF mod 2^32 = 0x100000000 mod 2^32 = 0 */

    unsigned int u2 = 100u;
    int s2 = -50;
    unsigned int ures2 = u2 + (unsigned int)s2;
    printf("%u\n", ures2);                   // EXPECT: 50
    /* 100 + (2^32 - 50) = 2^32 + 50, mod 2^32 = 50 */

    /* --- COMPARISON PROMOTION --- */
    char c1 = -1;
    unsigned char uc3 = 255;
    /* As signed char: c1 == -1 (0xFF), as unsigned char: uc3 == 255 (0xFF) */
    /* Comparing int vs int (after promotion): -1 != 255 */
    int cmp1 = (c1 == uc3) ? 1 : 0;       /* -1 == 255? No */
    printf("%d\n", cmp1);                   // EXPECT: 0

    /* Both promoted to int: (int)(-1) == (int)(255) => -1 == 255 => false */
    int c1i = c1;
    int uc3i = uc3;
    printf("%d\n", c1i);                    // EXPECT: -1
    printf("%d\n", uc3i);                   // EXPECT: 255

    /* --- INTEGER SIZES AND PROMOTION EFFECTS --- */
    /* sizeof results in size_t (unsigned) */
    int char_size = (int)sizeof(char);
    int short_size = (int)sizeof(short);
    int int_size = (int)sizeof(int);
    printf("%d\n", char_size);              // EXPECT: 1
    printf("%d\n", short_size);             // EXPECT: 2
    printf("%d\n", int_size);               // EXPECT: 4

    /* --- TRUNCATION WHEN ASSIGNING BACK TO SMALLER TYPE --- */
    int big_val = 300;
    char truncated = (char)big_val;         /* 300 mod 256 = 44, but stored as signed: 44 */
    printf("%d\n", (int)truncated);         // EXPECT: 44
    /* 300 = 256 + 44, so low byte = 44 */

    int big_neg = -300;
    char trunc_neg = (char)big_neg;
    /* -300 in binary: ...1111111111111111111111011010100
       low 8 bits: 0b11010100 = 0xD4 = 212 as unsigned, = -44 as signed */
    printf("%d\n", (int)trunc_neg);         // EXPECT: -44

    int val_256 = 256;
    char trunc_256 = (char)val_256;         /* 256 mod 256 = 0 */
    printf("%d\n", (int)trunc_256);         // EXPECT: 0

    int val_255 = 255;
    char trunc_255 = (char)val_255;         /* 255 -> -1 as signed char */
    printf("%d\n", (int)trunc_255);         // EXPECT: -1

    /* --- MIXED ARITHMETIC CHAINS --- */
    char cx = 50;
    short sy = 200;
    int iz = 1000;
    int result = cx + sy + iz;              /* 50+200+1000=1250 */
    printf("%d\n", result);                 // EXPECT: 1250

    /* Multiplication chain with promotion */
    char ca2 = 3;
    short sb2 = 100;
    int prod2 = ca2 * sb2;                  /* 300 */
    printf("%d\n", prod2);                  // EXPECT: 300

    /* Unary minus on char */
    char neg2 = 5;
    int negated = -neg2;                    /* -5, promoted */
    printf("%d\n", negated);               // EXPECT: -5

    char neg3 = -5;
    int negated3 = -neg3;                   /* 5 */
    printf("%d\n", negated3);              // EXPECT: 5

    /* --- UNSIGNED ARITHMETIC WRAPPING --- */
    unsigned char uc4 = 10;
    unsigned char uc5 = 20;
    /* uc4 - uc5: both promoted to int, result is -10 as int */
    int uchdiff = uc4 - uc5;
    printf("%d\n", uchdiff);               // EXPECT: -10

    unsigned int u3 = 5u;
    unsigned int u4 = 10u;
    unsigned int uwrap = u3 - u4;          /* wraps: 2^32 - 5 = 4294967291 */
    printf("%u\n", uwrap);                  // EXPECT: 4294967291

    /* --- INT CONSTANT EXPRESSIONS --- */
    int x = 'A';                            /* ASCII 65 */
    printf("%d\n", x);                      // EXPECT: 65
    int y = 'A' + 1;                        /* 66 = 'B' */
    printf("%d\n", y);                      // EXPECT: 66
    int z = 'a' - 'A';                      /* lowercase - uppercase = 32 */
    printf("%d\n", z);                      // EXPECT: 32

    return 0;
}
