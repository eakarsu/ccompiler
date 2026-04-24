int printf(const char *fmt, ...);
// EXPECT: 0xFF & 0x0F = 15\n0xFF | 0x0F = 255\n0xFF ^ 0x0F = 240\n~0 = -1\n~0xFF & 0xFF = 0\n1<<0 = 1\n1<<1 = 2\n1<<4 = 16\n1<<10 = 1024\n1024>>1 = 512\n1024>>2 = 256\n1024>>10 = 1\n0xAB bits: 10101011\nset bit 3: 8\nset bit 5: 40\nclear bit 3: 32\ntoggle bit 5: 0\nmask 4 bits: 15\n0xFF & mask = 15\n1:pow2 2:pow2 4:pow2 8:pow2 16:pow2 \nswap: a=99 b=42
// Test: unsigned-like behavior with bitwise ops

int main(void) {
    int a, b, r;
    int i;
    int mask;
    int bits[8];

    /* Basic bitwise AND */
    a = 0xFF;
    b = 0x0F;
    r = a & b;
    printf("0xFF & 0x0F = %d\n", r);

    /* Basic bitwise OR */
    r = a | b;
    printf("0xFF | 0x0F = %d\n", r);

    /* Basic bitwise XOR */
    r = a ^ b;
    printf("0xFF ^ 0x0F = %d\n", r);

    /* Bitwise NOT (complement) */
    a = 0;
    r = ~a;
    printf("~0 = %d\n", r);

    a = 0xFF;
    r = ~a & 0xFF;
    printf("~0xFF & 0xFF = %d\n", r);

    /* Left shift as multiply by power of 2 */
    a = 1;
    printf("1<<0 = %d\n", a << 0);
    printf("1<<1 = %d\n", a << 1);
    printf("1<<4 = %d\n", a << 4);
    printf("1<<10 = %d\n", a << 10);

    /* Right shift as divide by power of 2 */
    a = 1024;
    printf("1024>>1 = %d\n", a >> 1);
    printf("1024>>2 = %d\n", a >> 2);
    printf("1024>>10 = %d\n", a >> 10);

    /* Extract bits */
    a = 0xAB;
    for (i = 7; i >= 0; i--) {
        bits[7 - i] = (a >> i) & 1;
    }
    printf("0xAB bits: ");
    for (i = 0; i < 8; i++) {
        printf("%d", bits[i]);
    }
    printf("\n");

    /* Set/clear/toggle bits */
    a = 0;
    a = a | (1 << 3);
    printf("set bit 3: %d\n", a);
    a = a | (1 << 5);
    printf("set bit 5: %d\n", a);
    a = a & ~(1 << 3);
    printf("clear bit 3: %d\n", a);
    a = a ^ (1 << 5);
    printf("toggle bit 5: %d\n", a);

    /* Mask lower N bits */
    mask = (1 << 4) - 1;
    printf("mask 4 bits: %d\n", mask);
    printf("0xFF & mask = %d\n", 0xFF & mask);

    /* Check if power of 2 */
    for (i = 1; i <= 16; i++) {
        if ((i & (i - 1)) == 0) {
            printf("%d:pow2 ", i);
        }
    }
    printf("\n");

    /* Swap without temp using XOR */
    a = 42;
    b = 99;
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
    printf("swap: a=%d b=%d\n", a, b);

    return 0;
}
