int printf(const char *fmt, ...);
// EXPECT: 256 >> 0 = 256\n256 >> 1 = 128\n256 >> 2 = 64\n256 >> 4 = 16\n256 >> 8 = 1\n100 >> 1 = 50 (100/2)\n100 >> 2 = 25 (100/4)\n100 >> 3 = 12 (100/8)\nbyte0=0x78 byte1=0x56 byte2=0x34 byte3=0x12\nlog2(1024) = 10\navg(100,200) = 150\nfields: D C B A\nhighest bit position in 255 = 8\ndivide by 4: 1 3 7 15 31\n-128 >> 1 = -64\n-128 >> 2 = -32
// Test: right shift operations

int main(void) {
    // Basic right shifts
    int a = 256;
    printf("256 >> 0 = %d\n", a >> 0);
    printf("256 >> 1 = %d\n", a >> 1);
    printf("256 >> 2 = %d\n", a >> 2);
    printf("256 >> 4 = %d\n", a >> 4);
    printf("256 >> 8 = %d\n", a >> 8);

    // Right shift as division by power of 2
    int b = 100;
    printf("100 >> 1 = %d (100/2)\n", b >> 1);
    printf("100 >> 2 = %d (100/4)\n", b >> 2);
    printf("100 >> 3 = %d (100/8)\n", b >> 3);

    // Extracting individual bytes
    int word = 0x12345678;
    int byte0 = word & 0xFF;
    int byte1 = (word >> 8) & 0xFF;
    int byte2 = (word >> 16) & 0xFF;
    int byte3 = (word >> 24) & 0xFF;
    printf("byte0=0x%X byte1=0x%X byte2=0x%X byte3=0x%X\n",
           byte0, byte1, byte2, byte3);

    // Right shift for integer log2
    int val = 1024;
    int log2 = 0;
    int temp = val;
    while (temp > 1) {
        temp = temp >> 1;
        log2++;
    }
    printf("log2(1024) = %d\n", log2);

    // Right shift for averaging without overflow
    int x = 100;
    int y = 200;
    int avg = (x & y) + ((x ^ y) >> 1);
    printf("avg(100,200) = %d\n", avg);

    // Extracting bit fields
    int packed = 0xABCD;
    int field1 = packed & 0xF;
    int field2 = (packed >> 4) & 0xF;
    int field3 = (packed >> 8) & 0xF;
    int field4 = (packed >> 12) & 0xF;
    printf("fields: %X %X %X %X\n", field1, field2, field3, field4);

    // Shift right to find highest set bit position
    int n = 255;
    int pos = 0;
    int t = n;
    while (t > 0) {
        pos++;
        t = t >> 1;
    }
    printf("highest bit position in 255 = %d\n", pos);

    // Right shift for rounding down
    int values[5];
    values[0] = 7;
    values[1] = 15;
    values[2] = 31;
    values[3] = 63;
    values[4] = 127;
    int i;
    printf("divide by 4: ");
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        printf("%d", values[i] >> 2);
    }
    printf("\n");

    // Arithmetic right shift on negative
    int neg = -128;
    printf("-128 >> 1 = %d\n", neg >> 1);
    printf("-128 >> 2 = %d\n", neg >> 2);

    return 0;
}
