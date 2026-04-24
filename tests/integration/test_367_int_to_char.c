int printf(const char *fmt, ...);
// EXPECT: int_to_str:\n  0 -> "0"\n  42 -> "42"\n  -17 -> "-17"\n  12345 -> "12345"\n  -9999 -> "-9999"\n  1000000 -> "1000000"\nint_to_hex:\n  0 -> 0x00000000\n  255 -> 0x000000FF\n  57005 -> 0x0000DEAD\n  305419896 -> 0x12345678\ndigit_to_char:\n  0 -> '0'\n  1 -> '1'\n  2 -> '2'\n  3 -> '3'\n  4 -> '4'\n  5 -> '5'\n  6 -> '6'\n  7 -> '7'\n  8 -> '8'\n  9 -> '9'\nprintable range:\n  32 -> ' ' (printable=1)\n  65 -> 'A' (printable=1)\n  97 -> 'a' (printable=1)\n  48 -> '0' (printable=1)\n  126 -> '~' (printable=1)\nbinary:\n    0 = 00000000\n    5 = 00000101\n  170 = 10101010\n  255 = 11111111
// Test: int to char conversions

void int_to_str(int n, char *buf) {
    int i = 0;
    int neg = 0;
    int val = n;
    if (val < 0) {
        neg = 1;
        val = -val;
    }
    if (val == 0) {
        buf[0] = '0';
        buf[1] = 0;
        return;
    }
    char tmp[12];
    while (val > 0) {
        tmp[i] = '0' + (val % 10);
        val = val / 10;
        i++;
    }
    int j = 0;
    if (neg) {
        buf[j] = '-';
        j++;
    }
    int k;
    for (k = i - 1; k >= 0; k--) {
        buf[j] = tmp[k];
        j++;
    }
    buf[j] = 0;
}

void int_to_hex(int n, char *buf) {
    char hex_digits[16];
    hex_digits[0] = '0'; hex_digits[1] = '1'; hex_digits[2] = '2';
    hex_digits[3] = '3'; hex_digits[4] = '4'; hex_digits[5] = '5';
    hex_digits[6] = '6'; hex_digits[7] = '7'; hex_digits[8] = '8';
    hex_digits[9] = '9'; hex_digits[10] = 'A'; hex_digits[11] = 'B';
    hex_digits[12] = 'C'; hex_digits[13] = 'D'; hex_digits[14] = 'E';
    hex_digits[15] = 'F';

    buf[0] = '0';
    buf[1] = 'x';
    int i;
    for (i = 0; i < 8; i++) {
        int nibble = (n >> (28 - i * 4)) & 0xF;
        buf[2 + i] = hex_digits[nibble];
    }
    buf[10] = 0;
}

int main(void) {
    // Integer to string
    char buf[20];
    int test_vals[6];
    test_vals[0] = 0;
    test_vals[1] = 42;
    test_vals[2] = -17;
    test_vals[3] = 12345;
    test_vals[4] = -9999;
    test_vals[5] = 1000000;

    int i;
    printf("int_to_str:\n");
    for (i = 0; i < 6; i++) {
        int_to_str(test_vals[i], buf);
        printf("  %d -> \"%s\"\n", test_vals[i], buf);
    }

    // Integer to hex string
    int hex_vals[4];
    hex_vals[0] = 0;
    hex_vals[1] = 255;
    hex_vals[2] = 0xDEAD;
    hex_vals[3] = 0x12345678;

    printf("int_to_hex:\n");
    for (i = 0; i < 4; i++) {
        int_to_hex(hex_vals[i], buf);
        printf("  %d -> %s\n", hex_vals[i], buf);
    }

    // Digit to character
    printf("digit_to_char:\n");
    for (i = 0; i < 10; i++) {
        char c = '0' + i;
        printf("  %d -> '%c'\n", i, c);
    }

    // Int to ASCII printable range check
    printf("printable range:\n");
    int codes[5];
    codes[0] = 32;   // space
    codes[1] = 65;   // 'A'
    codes[2] = 97;   // 'a'
    codes[3] = 48;   // '0'
    codes[4] = 126;  // '~'
    for (i = 0; i < 5; i++) {
        int printable = codes[i] >= 32 && codes[i] <= 126;
        printf("  %d -> '%c' (printable=%d)\n", codes[i], codes[i], printable);
    }

    // Integer to binary string
    printf("binary:\n");
    int bin_vals[4];
    bin_vals[0] = 0;
    bin_vals[1] = 5;
    bin_vals[2] = 170;
    bin_vals[3] = 255;
    int j;
    for (i = 0; i < 4; i++) {
        printf("  %3d = ", bin_vals[i]);
        for (j = 7; j >= 0; j--) {
            printf("%c", ((bin_vals[i] >> j) & 1) ? '1' : '0');
        }
        printf("\n");
    }

    return 0;
}
