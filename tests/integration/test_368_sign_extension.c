int printf(const char *fmt, ...);
// EXPECT: char to int:\n  127 -> 127\n  -128 -> -128\n  -1 -> -1\n  0 -> 0\nsign_extend_8_to_32:\n  0x00 -> 0\n  0x7F -> 127\n  0x80 -> -128\n  0xFF -> -1\n  0x01 -> 1\n  0xFE -> -2\nsign_extend_16_to_32:\n  0x0000 -> 0\n  0x7FFF -> 32767\n  0x8000 -> -32768\n  0xFFFF -> -1\n  0x0001 -> 1\nchar -5 as int: -5\nchar -5 + 10: 5\nchar array as int:\n  arr[0] = 100 (0x64)\n  arr[1] = -100 (0x9C)\n  arr[2] = 127 (0x7F)\n  arr[3] = -128 (0x80)\nright shift of -256:\n  >> 1 = -128\n  >> 4 = -16\n  >> 8 = -1\nzero extend -1: 255\nsign extend -1: -1
// Test: sign extension behavior

int sign_extend_8_to_32(int val) {
    // Sign extend from 8 bits to 32 bits
    int byte_val = val & 0xFF;
    if (byte_val & 0x80) {
        return byte_val | (~0xFF);
    }
    return byte_val;
}

int sign_extend_16_to_32(int val) {
    int short_val = val & 0xFFFF;
    if (short_val & 0x8000) {
        return short_val | (~0xFFFF);
    }
    return short_val;
}

int main(void) {
    // Char to int sign extension
    char c1 = 127;
    char c2 = -128;
    char c3 = -1;
    char c4 = 0;
    printf("char to int:\n");
    printf("  127 -> %d\n", (int)c1);
    printf("  -128 -> %d\n", (int)c2);
    printf("  -1 -> %d\n", (int)c3);
    printf("  0 -> %d\n", (int)c4);

    // Manual sign extension from 8 bits
    printf("sign_extend_8_to_32:\n");
    printf("  0x00 -> %d\n", sign_extend_8_to_32(0x00));
    printf("  0x7F -> %d\n", sign_extend_8_to_32(0x7F));
    printf("  0x80 -> %d\n", sign_extend_8_to_32(0x80));
    printf("  0xFF -> %d\n", sign_extend_8_to_32(0xFF));
    printf("  0x01 -> %d\n", sign_extend_8_to_32(0x01));
    printf("  0xFE -> %d\n", sign_extend_8_to_32(0xFE));

    // Manual sign extension from 16 bits
    printf("sign_extend_16_to_32:\n");
    printf("  0x0000 -> %d\n", sign_extend_16_to_32(0x0000));
    printf("  0x7FFF -> %d\n", sign_extend_16_to_32(0x7FFF));
    printf("  0x8000 -> %d\n", sign_extend_16_to_32(0x8000));
    printf("  0xFFFF -> %d\n", sign_extend_16_to_32(0xFFFF));
    printf("  0x0001 -> %d\n", sign_extend_16_to_32(0x0001));

    // Sign extension with arithmetic
    char small = -5;
    int big = small;
    printf("char -5 as int: %d\n", big);
    printf("char -5 + 10: %d\n", big + 10);

    // Sign extension in array context
    char arr[4];
    arr[0] = 100;
    arr[1] = -100;
    arr[2] = 127;
    arr[3] = -128;
    int i;
    printf("char array as int:\n");
    for (i = 0; i < 4; i++) {
        printf("  arr[%d] = %d (0x%X)\n", i, (int)arr[i], (int)arr[i] & 0xFF);
    }

    // Sign bit propagation in right shift
    int neg = -256;
    printf("right shift of -256:\n");
    printf("  >> 1 = %d\n", neg >> 1);
    printf("  >> 4 = %d\n", neg >> 4);
    printf("  >> 8 = %d\n", neg >> 8);

    // Zero extension vs sign extension
    char negative_byte = -1;  // 0xFF
    int zero_ext = negative_byte & 0xFF;  // mask to zero-extend
    int sign_ext = negative_byte;          // natural sign extend
    printf("zero extend -1: %d\n", zero_ext);
    printf("sign extend -1: %d\n", sign_ext);

    return 0;
}
