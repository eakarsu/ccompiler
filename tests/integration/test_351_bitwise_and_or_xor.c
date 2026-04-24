int printf(const char *fmt, ...);
// EXPECT: 0xFF & 0x0F = 0xF\n12345 & 0 = 0\n0xAB & 0xFF = 0xAB\n0xF0 | 0x0F = 0xFF\n42 | 0 = 42\nflags = 21\n0xFF ^ 0x0F = 0xF0\n123 ^ 123 = 0\noriginal=42 encrypted=73 decrypted=42\n(0xAA & 0xF0) | 0x05 = 0xA5\n77 ^ 255 ^ 255 = 77\n-1 & 0xFF = 255\nbits 0,3,7 set = 137\nbit 0: 1\nbit 1: 0\nbit 3: 1\nbit 7: 1\n0xFF ^ 0x0F = 0xF0\ntoggle back = 0xFF
// Test: bitwise AND, OR, XOR basics

int main(void) {
    // Basic AND operations
    int a = 0xFF;
    int b = 0x0F;
    int and_result = a & b;
    printf("0xFF & 0x0F = 0x%X\n", and_result);

    // AND with zero
    int c = 12345 & 0;
    printf("12345 & 0 = %d\n", c);

    // AND with all ones
    int d = 0xAB & 0xFF;
    printf("0xAB & 0xFF = 0x%X\n", d);

    // Basic OR operations
    int e = 0xF0 | 0x0F;
    printf("0xF0 | 0x0F = 0x%X\n", e);

    // OR with zero (identity)
    int f = 42 | 0;
    printf("42 | 0 = %d\n", f);

    // OR combining flags
    int flag1 = 1;
    int flag2 = 4;
    int flag3 = 16;
    int flags = flag1 | flag2 | flag3;
    printf("flags = %d\n", flags);

    // Basic XOR operations
    int g = 0xFF ^ 0x0F;
    printf("0xFF ^ 0x0F = 0x%X\n", g);

    // XOR self-cancellation
    int h = 123 ^ 123;
    printf("123 ^ 123 = %d\n", h);

    // XOR double application
    int original = 42;
    int key = 99;
    int encrypted = original ^ key;
    int decrypted = encrypted ^ key;
    printf("original=%d encrypted=%d decrypted=%d\n", original, encrypted, decrypted);

    // Chained operations
    int x = (0xAA & 0xF0) | 0x05;
    printf("(0xAA & 0xF0) | 0x05 = 0x%X\n", x);

    // XOR properties: a ^ b ^ b = a
    int val = 77;
    int mask = 255;
    printf("77 ^ 255 ^ 255 = %d\n", val ^ mask ^ mask);

    // Bitwise ops on negative numbers
    int neg = -1;
    int pos = neg & 0xFF;
    printf("-1 & 0xFF = %d\n", pos);

    // OR to set specific bits
    int num = 0;
    num = num | (1 << 0);
    num = num | (1 << 3);
    num = num | (1 << 7);
    printf("bits 0,3,7 set = %d\n", num);

    // AND to test specific bits
    int test_val = 137;
    printf("bit 0: %d\n", (test_val & 1) != 0);
    printf("bit 1: %d\n", (test_val & 2) != 0);
    printf("bit 3: %d\n", (test_val & 8) != 0);
    printf("bit 7: %d\n", (test_val & 128) != 0);

    // XOR to toggle bits
    int toggle = 0xFF;
    toggle = toggle ^ 0x0F;
    printf("0xFF ^ 0x0F = 0x%X\n", toggle);
    toggle = toggle ^ 0x0F;
    printf("toggle back = 0x%X\n", toggle);

    return 0;
}
