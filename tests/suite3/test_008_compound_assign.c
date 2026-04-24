int printf(const char *fmt, ...);

int main(void) {
    /* += operator */
    int a = 10;
    a += 5;
    printf("%d\n", a);              // EXPECT: 15
    a += (-3);
    printf("%d\n", a);              // EXPECT: 12
    a += 0;
    printf("%d\n", a);              // EXPECT: 12

    /* -= operator */
    int b = 20;
    b -= 7;
    printf("%d\n", b);              // EXPECT: 13
    b -= (-3);
    printf("%d\n", b);              // EXPECT: 16
    /* 13 - (-3) = 16 */

    /* *= operator */
    int c = 4;
    c *= 5;
    printf("%d\n", c);              // EXPECT: 20
    c *= 3;
    printf("%d\n", c);              // EXPECT: 60
    c *= (-1);
    printf("%d\n", c);              // EXPECT: -60

    /* /= operator */
    int d = 100;
    d /= 4;
    printf("%d\n", d);              // EXPECT: 25
    d /= 5;
    printf("%d\n", d);              // EXPECT: 5
    d /= 2;
    printf("%d\n", d);              // EXPECT: 2
    /* 5/2 = 2 (truncation) */

    /* %= operator */
    int e = 100;
    e %= 7;
    printf("%d\n", e);              // EXPECT: 2
    /* 100 % 7 = 2 */
    int f = 17;
    f %= 5;
    printf("%d\n", f);              // EXPECT: 2
    /* 17 % 5 = 2 */

    /* &= operator */
    int g = 0xFF;
    g &= 0x0F;
    printf("%d\n", g);              // EXPECT: 15
    /* 11111111 & 00001111 = 00001111 = 15 */
    g &= 0b1010;
    printf("%d\n", g);              // EXPECT: 10
    /* 00001111 & 00001010 = 00001010 = 10 */

    /* |= operator */
    int h = 0b1010;
    h |= 0b0101;
    printf("%d\n", h);              // EXPECT: 15
    /* 1010 | 0101 = 1111 = 15 */
    h |= 0b10000;
    printf("%d\n", h);              // EXPECT: 31
    /* 01111 | 10000 = 11111 = 31 */

    /* ^= operator */
    int i = 0xFF;
    i ^= 0x0F;
    printf("%d\n", i);              // EXPECT: 240
    /* 11111111 ^ 00001111 = 11110000 = 240 */
    i ^= 0xFF;
    printf("%d\n", i);              // EXPECT: 15
    /* 11110000 ^ 11111111 = 00001111 = 15 */
    i ^= i;
    printf("%d\n", i);              // EXPECT: 0
    /* x ^ x = 0 */

    /* <<= operator */
    int j = 1;
    j <<= 1;
    printf("%d\n", j);              // EXPECT: 2
    j <<= 1;
    printf("%d\n", j);              // EXPECT: 4
    j <<= 3;
    printf("%d\n", j);              // EXPECT: 32
    /* 4 << 3 = 32 */
    j <<= 0;
    printf("%d\n", j);              // EXPECT: 32

    /* >>= operator */
    int k = 256;
    k >>= 1;
    printf("%d\n", k);              // EXPECT: 128
    k >>= 2;
    printf("%d\n", k);              // EXPECT: 32
    k >>= 3;
    printf("%d\n", k);              // EXPECT: 4
    k >>= 2;
    printf("%d\n", k);              // EXPECT: 1
    k >>= 1;
    printf("%d\n", k);              // EXPECT: 0

    /* Chained compound assignments */
    int x = 10;
    x += 5;
    /* x = 15 */
    x *= 2;
    /* x = 30 */
    x -= 6;
    /* x = 24 */
    x /= 4;
    /* x = 6 */
    x %= 4;
    /* x = 2 */
    printf("%d\n", x);              // EXPECT: 2

    /* Compound on negative values */
    int neg = -10;
    neg += 3;
    printf("%d\n", neg);            // EXPECT: -7
    neg *= 2;
    printf("%d\n", neg);            // EXPECT: -14
    neg /= 7;
    printf("%d\n", neg);            // EXPECT: -2
    /* -14 / 7 = -2 */

    /* Compound assign with expression on right */
    int base = 5;
    int step = 3;
    base += step * 2;
    printf("%d\n", base);           // EXPECT: 11
    /* 5 + 3*2 = 11 */

    /* Unsigned compound assign */
    unsigned int u = 100u;
    u += 200u;
    printf("%u\n", u);              // EXPECT: 300
    u -= 50u;
    printf("%u\n", u);              // EXPECT: 250
    u *= 3u;
    printf("%u\n", u);              // EXPECT: 750

    /* char compound assign */
    char ch = 10;
    ch += 5;
    printf("%d\n", ch);             // EXPECT: 15
    ch *= 2;
    printf("%d\n", ch);             // EXPECT: 30
    ch -= 8;
    printf("%d\n", ch);             // EXPECT: 22

    /* Bitwise compound in sequence */
    int mask = 0;
    mask |= (1 << 0);
    mask |= (1 << 2);
    mask |= (1 << 4);
    printf("%d\n", mask);           // EXPECT: 21
    /* bit0 + bit2 + bit4 = 1 + 4 + 16 = 21 */

    mask &= ~(1 << 2);
    printf("%d\n", mask);           // EXPECT: 17
    /* clear bit2: 21 & ~4 = 21 & 0xFFFFFFFB = 17 */

    mask ^= (1 << 1);
    printf("%d\n", mask);           // EXPECT: 19
    /* toggle bit1: 17 ^ 2 = 19 */

    return 0;
}
