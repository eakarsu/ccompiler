int printf(const char *fmt, ...);
// EXPECT: reverse_byte(0x00) = 0x00\nreverse_byte(0xFF) = 0xFF\nreverse_byte(0x01) = 0x80\nreverse_byte(0x80) = 0x01\nreverse_byte(0xAA) = 0x55\nreverse_byte(0x55) = 0xAA\nreverse_byte(0x0F) = 0xF0\nreverse_byte(0xF0) = 0x0F\nreverse_nibble(0x0) = 0x0\nreverse_nibble(0x1) = 0x8\nreverse_nibble(0x8) = 0x1\nreverse_nibble(0xA) = 0x5\nreverse_nibble(0xF) = 0xF\ndouble reversal identity: 1\nreverse_16(0x0001) = 0x8000\nreverse_16(0x8000) = 0x0001\nreverse_16(0xAAAA) = 0x5555\nreverse_16(0x1234) = 0x2C48\n0xFF palindrome: 1\n0x81 palindrome: 1\n0x66 palindrome: 1\n0x18 palindrome: 1
// Test: bit reversal

int reverse_byte(int b) {
    int result = 0;
    int i;
    for (i = 0; i < 8; i++) {
        result = result | (((b >> i) & 1) << (7 - i));
    }
    return result;
}

int reverse_nibble(int n) {
    int result = 0;
    int i;
    for (i = 0; i < 4; i++) {
        result = result | (((n >> i) & 1) << (3 - i));
    }
    return result;
}

int reverse_16bits(int val) {
    int result = 0;
    int i;
    for (i = 0; i < 16; i++) {
        result = result | (((val >> i) & 1) << (15 - i));
    }
    return result;
}

int main(void) {
    // Reverse individual bytes
    printf("reverse_byte(0x00) = 0x%02X\n", reverse_byte(0x00));
    printf("reverse_byte(0xFF) = 0x%02X\n", reverse_byte(0xFF));
    printf("reverse_byte(0x01) = 0x%02X\n", reverse_byte(0x01));
    printf("reverse_byte(0x80) = 0x%02X\n", reverse_byte(0x80));
    printf("reverse_byte(0xAA) = 0x%02X\n", reverse_byte(0xAA));
    printf("reverse_byte(0x55) = 0x%02X\n", reverse_byte(0x55));
    printf("reverse_byte(0x0F) = 0x%02X\n", reverse_byte(0x0F));
    printf("reverse_byte(0xF0) = 0x%02X\n", reverse_byte(0xF0));

    // Reverse nibbles
    printf("reverse_nibble(0x0) = 0x%X\n", reverse_nibble(0x0));
    printf("reverse_nibble(0x1) = 0x%X\n", reverse_nibble(0x1));
    printf("reverse_nibble(0x8) = 0x%X\n", reverse_nibble(0x8));
    printf("reverse_nibble(0xA) = 0x%X\n", reverse_nibble(0xA));
    printf("reverse_nibble(0xF) = 0x%X\n", reverse_nibble(0xF));

    // Double reversal is identity
    int i;
    int all_match = 1;
    for (i = 0; i < 256; i++) {
        if (reverse_byte(reverse_byte(i)) != i) {
            all_match = 0;
        }
    }
    printf("double reversal identity: %d\n", all_match);

    // Reverse 16-bit values
    printf("reverse_16(0x0001) = 0x%04X\n", reverse_16bits(0x0001));
    printf("reverse_16(0x8000) = 0x%04X\n", reverse_16bits(0x8000));
    printf("reverse_16(0xAAAA) = 0x%04X\n", reverse_16bits(0xAAAA));
    printf("reverse_16(0x1234) = 0x%04X\n", reverse_16bits(0x1234));

    // Palindrome check (bits read same forwards and backwards)
    int palindromes[4];
    palindromes[0] = 0xFF;  // 11111111
    palindromes[1] = 0x81;  // 10000001
    palindromes[2] = 0x66;  // 01100110
    palindromes[3] = 0x18;  // 00011000
    for (i = 0; i < 4; i++) {
        int is_palindrome = (reverse_byte(palindromes[i]) == palindromes[i]);
        printf("0x%02X palindrome: %d\n", palindromes[i], is_palindrome);
    }

    return 0;
}
