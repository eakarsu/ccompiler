int printf(const char *fmt, ...);
// EXPECT: ~0 = -1\n~(-1) = 0\n~1 = -2\n~0xFF & 0xFFFF = 65280\n~~42 = 42\n~100 + 1 = -100\n~1 + 1 = -1\n0xFF with bit 3 cleared = 247\n0xFF with lower nibble cleared = 240\n42 sign bit: 0\n-42 sign bit: 1\n~0xAA & 0xFF = 0x55\nx + ~x = -1\n~(char)0 = -1\nupper 16 of 0x12345678 = 0x1234\n~(1<<0) & 0xFF = 254\n~(1<<1) & 0xFF = 253\n~(1<<2) & 0xFF = 251\n~(1<<3) & 0xFF = 247
// Test: bitwise NOT and complement

int main(void) {
    // Bitwise NOT of zero
    int a = ~0;
    printf("~0 = %d\n", a);

    // Bitwise NOT of -1
    int b = ~(-1);
    printf("~(-1) = %d\n", b);

    // Bitwise NOT of 1
    int c = ~1;
    printf("~1 = %d\n", c);

    // NOT and mask to get lower byte
    int d = ~0xFF;
    printf("~0xFF & 0xFFFF = %d\n", d & 0xFFFF);

    // Double NOT is identity
    int e = 42;
    int f = ~~e;
    printf("~~42 = %d\n", f);

    // One's complement for negation check: -x = ~x + 1
    int g = 100;
    int neg_g = ~g + 1;
    printf("~100 + 1 = %d\n", neg_g);

    int h = 1;
    int neg_h = ~h + 1;
    printf("~1 + 1 = %d\n", neg_h);

    // NOT and AND to clear bits
    int flags = 0xFF;
    int clear_bit3 = flags & ~(1 << 3);
    printf("0xFF with bit 3 cleared = %d\n", clear_bit3);

    int clear_bits = flags & ~0x0F;
    printf("0xFF with lower nibble cleared = %d\n", clear_bits);

    // Complement-based sign detection
    int pos_val = 42;
    int neg_val = -42;
    printf("42 sign bit: %d\n", (pos_val >> 31) & 1);
    printf("-42 sign bit: %d\n", (neg_val >> 31) & 1);

    // Using NOT for bit inversion patterns
    int pattern = 0xAA;
    int inverted = (~pattern) & 0xFF;
    printf("~0xAA & 0xFF = 0x%X\n", inverted);

    // Verify complement relationship
    int x = 12345;
    int sum = x + (~x);
    printf("x + ~x = %d\n", sum);

    // NOT preserves bit width
    char ch = 0;
    int not_ch = ~ch;
    printf("~(char)0 = %d\n", not_ch);

    // Building masks with NOT
    int keep_upper = ~0xFFFF;
    int val = 0x12345678;
    int upper = val & keep_upper;
    printf("upper 16 of 0x12345678 = 0x%X\n", (unsigned)upper >> 16);

    // Complement of powers of 2
    int i;
    for (i = 0; i < 4; i++) {
        int pow2 = 1 << i;
        int comp = ~pow2 & 0xFF;
        printf("~(1<<%d) & 0xFF = %d\n", i, comp);
    }

    return 0;
}
