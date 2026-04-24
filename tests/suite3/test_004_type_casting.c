int printf(const char *fmt, ...);

int main(void) {
    /* Widening cast: char -> int */
    char c = 65;
    int i = (int)c;
    printf("%d\n", i);              // EXPECT: 65

    /* Widening cast: signed char -> int (sign extension) */
    signed char sc = -10;
    int si = (int)sc;
    printf("%d\n", si);             // EXPECT: -10

    /* Widening cast: unsigned char -> int (zero extension) */
    unsigned char uc = 200;
    int ui = (int)uc;
    printf("%d\n", ui);             // EXPECT: 200

    /* Narrowing cast: int -> char, value fits */
    int big_int = 65;
    char narrow = (char)big_int;
    printf("%d\n", narrow);         // EXPECT: 65

    /* Narrowing cast: int -> char, truncate to low 8 bits, then sign extend */
    /* 256 in binary: 1 00000000, low 8 bits = 00000000 = 0 */
    int val256 = 256;
    char c256 = (char)val256;
    printf("%d\n", c256);           // EXPECT: 0

    /* 257 -> low 8 bits = 00000001 = 1 */
    int val257 = 257;
    char c257 = (char)val257;
    printf("%d\n", c257);           // EXPECT: 1

    /* 300 -> 300 - 256 = 44, low 8 bits = 44 */
    int val300 = 300;
    char c300 = (char)val300;
    printf("%d\n", c300);           // EXPECT: 44

    /* 128 as char: 10000000, signed = -128 */
    int val128 = 128;
    char c128 = (char)val128;
    printf("%d\n", c128);           // EXPECT: -128

    /* 255 as char: 11111111, signed = -1 */
    int val255 = 255;
    char c255 = (char)val255;
    printf("%d\n", c255);           // EXPECT: -1

    /* 127 as char: 01111111, signed = 127 */
    int val127 = 127;
    char c127 = (char)val127;
    printf("%d\n", c127);           // EXPECT: 127

    /* int -> unsigned int */
    int neg = -1;
    unsigned int uneg = (unsigned int)neg;
    printf("%u\n", uneg);           // EXPECT: 4294967295
    /* -1 as unsigned = UINT_MAX = 4294967295 */

    int neg2 = -2147483648;
    unsigned int uneg2 = (unsigned int)neg2;
    printf("%u\n", uneg2);          // EXPECT: 2147483648
    /* INT_MIN as unsigned: 2^31 = 2147483648 */

    /* unsigned int -> int: large value becomes negative */
    unsigned int large = 4294967295u;
    int as_signed = (int)large;
    printf("%d\n", as_signed);      // EXPECT: -1

    unsigned int large2 = 2147483648u;
    int as_signed2 = (int)large2;
    printf("%d\n", as_signed2);     // EXPECT: -2147483648
    /* 2^31 as signed int = INT_MIN = -2147483648 */

    /* Cast in arithmetic expression */
    signed char s8 = -128;
    /* Without cast, char is promoted to int automatically */
    int promoted = s8;
    printf("%d\n", promoted);       // EXPECT: -128
    printf("%d\n", promoted * 2);   // EXPECT: -256

    /* Cast to char in expression to truncate result */
    int sum = 200 + 100;            /* 300 */
    char truncated = (char)sum;
    printf("%d\n", truncated);      // EXPECT: 44
    /* 300 mod 256 = 44, fits in signed range */

    /* unsigned char preserves value up to 255 */
    unsigned char uc2 = (unsigned char)300;
    printf("%d\n", uc2);            // EXPECT: 44
    /* 300 mod 256 = 44 */

    unsigned char uc3 = (unsigned char)(-1);
    printf("%d\n", uc3);            // EXPECT: 255
    /* -1 mod 256 = 255 */

    /* Cast sequence: int -> char -> int */
    int original = 130;
    char step1 = (char)original;    /* 130 > 127, so -126 as signed char */
    int step2 = (int)step1;
    printf("%d\n", step2);          // EXPECT: -126
    /* 130 in 8 bits: 10000010. As signed char: -(128) + 2 = -126 */

    /* Cast in comparison */
    int a = -1;
    unsigned int b = 1u;
    /* (unsigned int)a = UINT_MAX = 4294967295 > 1 */
    printf("%d\n", (unsigned int)a > b); // EXPECT: 1

    /* Explicit cast prevents sign-extension issue */
    unsigned char byte = 0xFF;      /* 255 as unsigned */
    int as_int = (int)(unsigned char)byte;
    printf("%d\n", as_int);         // EXPECT: 255

    /* Cast in arithmetic: force unsigned arithmetic */
    int x = -1;
    int y = 2;
    unsigned int ux = (unsigned int)x;
    printf("%u\n", ux / (unsigned int)y); // EXPECT: 2147483647
    /* UINT_MAX / 2 = 4294967295 / 2 = 2147483647 */

    /* Pointer size cast via intermediate storage */
    int arr[3];
    arr[0] = 100;
    arr[1] = 200;
    arr[2] = 300;
    int *p = arr;
    char *cp = (char *)p;
    /* Advance by sizeof(int) = 4 bytes to get arr[1] */
    int *p1 = (int *)(cp + 4);
    printf("%d\n", *p1);            // EXPECT: 200

    /* Cast result of expression */
    printf("%d\n", (char)(127 + 1)); // EXPECT: -128
    /* 128 as signed char = -128 */

    printf("%d\n", (char)(255));    // EXPECT: -1

    return 0;
}
