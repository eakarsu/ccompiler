int printf(const char *fmt, ...);
// EXPECT: 1 << 0 = 1\n1 << 1 = 2\n1 << 2 = 4\n1 << 3 = 8\n1 << 4 = 16\n1 << 10 = 1024\n5 << 1 = 10 (5*2)\n5 << 2 = 20 (5*4)\n5 << 3 = 40 (5*8)\nalternating bits = 0x55\nnibble swap of 0xAB = 0xBA\ncombined = 0x78563412\nPowers of 2: 1 2 4 8 16 32 64 128\n13 aligned to 4 = 16\n3 << 1 << 2 = 24\n3 << 3 = 24\nhash of 'hello' = 112475631
// Test: left shift operations

int main(void) {
    // Basic left shifts
    int a = 1;
    printf("1 << 0 = %d\n", a << 0);
    printf("1 << 1 = %d\n", a << 1);
    printf("1 << 2 = %d\n", a << 2);
    printf("1 << 3 = %d\n", a << 3);
    printf("1 << 4 = %d\n", a << 4);
    printf("1 << 10 = %d\n", a << 10);

    // Shift as multiplication by power of 2
    int b = 5;
    printf("5 << 1 = %d (5*2)\n", b << 1);
    printf("5 << 2 = %d (5*4)\n", b << 2);
    printf("5 << 3 = %d (5*8)\n", b << 3);

    // Shift to build bit patterns
    int pattern = 0;
    pattern = pattern | (1 << 0);
    pattern = pattern | (1 << 2);
    pattern = pattern | (1 << 4);
    pattern = pattern | (1 << 6);
    printf("alternating bits = 0x%X\n", pattern);

    // Shift and mask
    int c = 0xAB;
    int upper = (c >> 4) & 0xF;
    int lower = c & 0xF;
    int swapped = (lower << 4) | upper;
    printf("nibble swap of 0xAB = 0x%X\n", swapped);

    // Building a value byte by byte
    int byte0 = 0x12;
    int byte1 = 0x34;
    int byte2 = 0x56;
    int byte3 = 0x78;
    int word = (byte3 << 24) | (byte2 << 16) | (byte1 << 8) | byte0;
    printf("combined = 0x%X\n", word);

    // Powers of 2 via shift
    int i;
    printf("Powers of 2: ");
    for (i = 0; i < 8; i++) {
        if (i > 0) printf(" ");
        printf("%d", 1 << i);
    }
    printf("\n");

    // Shift for alignment
    int unaligned = 13;
    int align = 4;
    int mask = align - 1;
    int aligned = (unaligned + mask) & ~mask;
    printf("13 aligned to 4 = %d\n", aligned);

    // Multiple shifts
    int d = 3;
    printf("3 << 1 << 2 = %d\n", (d << 1) << 2);
    printf("3 << 3 = %d\n", d << 3);

    // Shift used in hash-like computation
    int hash = 0;
    char str[6];
    str[0] = 'h'; str[1] = 'e'; str[2] = 'l';
    str[3] = 'l'; str[4] = 'o'; str[5] = 0;
    for (i = 0; i < 5; i++) {
        hash = (hash << 5) + str[i];
    }
    printf("hash of 'hello' = %d\n", hash);

    return 0;
}
