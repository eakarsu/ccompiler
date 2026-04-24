int printf(const char *fmt, ...);
// EXPECT: 42 in binary: 101010\n255 in binary: 11111111\n0 in binary: 0\n1 in binary: 1\n42 in octal: 52\n255 in octal: 377\n42 in hex: 2a\n255 in hex: ff\n256 in hex: 100\n4096 in hex: 1000\n100 -> binary 1100100 -> back 100\nff in base 16 = 255\n777 in base 8 = 511\n1010 in base 2 = 10\npowers of 2:\n  2^0 = 1 = 1\n  2^1 = 2 = 10\n  2^2 = 4 = 100\n  2^3 = 8 = 1000\n  2^4 = 16 = 10000\n  2^5 = 32 = 100000\n  2^6 = 64 = 1000000\n  2^7 = 128 = 10000000
// Test: base conversion (decimal to binary, octal, hex and back)

void to_binary(int n, char *buf) {
    if (n == 0) {
        buf[0] = '0'; buf[1] = '\0';
        return;
    }
    char tmp[33];
    int i = 0;
    int num = n;
    while (num > 0) {
        tmp[i] = '0' + (num % 2);
        num = num / 2;
        i = i + 1;
    }
    // Reverse
    int j;
    for (j = 0; j < i; j++) {
        buf[j] = tmp[i - 1 - j];
    }
    buf[i] = '\0';
}

void to_octal(int n, char *buf) {
    if (n == 0) {
        buf[0] = '0'; buf[1] = '\0';
        return;
    }
    char tmp[20];
    int i = 0;
    int num = n;
    while (num > 0) {
        tmp[i] = '0' + (num % 8);
        num = num / 8;
        i = i + 1;
    }
    int j;
    for (j = 0; j < i; j++) buf[j] = tmp[i - 1 - j];
    buf[i] = '\0';
}

void to_hex(int n, char *buf) {
    if (n == 0) {
        buf[0] = '0'; buf[1] = '\0';
        return;
    }
    char digits[16];
    digits[0]='0'; digits[1]='1'; digits[2]='2'; digits[3]='3';
    digits[4]='4'; digits[5]='5'; digits[6]='6'; digits[7]='7';
    digits[8]='8'; digits[9]='9'; digits[10]='a'; digits[11]='b';
    digits[12]='c'; digits[13]='d'; digits[14]='e'; digits[15]='f';

    char tmp[20];
    int i = 0;
    int num = n;
    while (num > 0) {
        tmp[i] = digits[num % 16];
        num = num / 16;
        i = i + 1;
    }
    int j;
    for (j = 0; j < i; j++) buf[j] = tmp[i - 1 - j];
    buf[i] = '\0';
}

int from_binary(char *s) {
    int result = 0;
    int i = 0;
    while (s[i] != '\0') {
        result = result * 2 + (s[i] - '0');
        i = i + 1;
    }
    return result;
}

int from_base(char *s, int base) {
    int result = 0;
    int i = 0;
    while (s[i] != '\0') {
        int digit;
        if (s[i] >= '0' && s[i] <= '9') digit = s[i] - '0';
        else if (s[i] >= 'a' && s[i] <= 'f') digit = 10 + s[i] - 'a';
        else digit = 0;
        result = result * base + digit;
        i = i + 1;
    }
    return result;
}

int main(void) {
    char buf[40];

    // Decimal to binary
    to_binary(42, buf);
    printf("42 in binary: %s\n", buf);
    to_binary(255, buf);
    printf("255 in binary: %s\n", buf);
    to_binary(0, buf);
    printf("0 in binary: %s\n", buf);
    to_binary(1, buf);
    printf("1 in binary: %s\n", buf);

    // Decimal to octal
    to_octal(42, buf);
    printf("42 in octal: %s\n", buf);
    to_octal(255, buf);
    printf("255 in octal: %s\n", buf);

    // Decimal to hex
    to_hex(42, buf);
    printf("42 in hex: %s\n", buf);
    to_hex(255, buf);
    printf("255 in hex: %s\n", buf);
    to_hex(256, buf);
    printf("256 in hex: %s\n", buf);
    to_hex(4096, buf);
    printf("4096 in hex: %s\n", buf);

    // Round-trip: decimal -> binary -> decimal
    to_binary(100, buf);
    int back = from_binary(buf);
    printf("100 -> binary %s -> back %d\n", buf, back);

    // From base
    printf("ff in base 16 = %d\n", from_base("ff", 16));
    printf("777 in base 8 = %d\n", from_base("777", 8));
    printf("1010 in base 2 = %d\n", from_base("1010", 2));

    // Powers of 2
    int i;
    printf("powers of 2:\n");
    int p = 1;
    for (i = 0; i < 8; i++) {
        to_binary(p, buf);
        printf("  2^%d = %d = %s\n", i, p, buf);
        p = p * 2;
    }

    return 0;
}
