int printf(const char *fmt, ...);
// EXPECT: binary:\n0 = 0\n1 = 1\n2 = 10\n3 = 11\n4 = 100\n5 = 101\n6 = 110\n7 = 111\n8 = 1000\n9 = 1001\n10 = 1010\n11 = 1011\n12 = 1100\n13 = 1101\n14 = 1110\n15 = 1111\n16 = 10000\noctal:\n0 = 0\n1 = 1\n2 = 2\n3 = 3\n4 = 4\n5 = 5\n6 = 6\n7 = 7\n8 = 10\n9 = 11\n10 = 12\n11 = 13\n12 = 14\n13 = 15\n14 = 16\n15 = 17\n16 = 20\n10101101 = 173\n11111111 = 255\npow2 binary:\n1 = 1\n2 = 10\n4 = 100\n8 = 1000\n16 = 10000\n32 = 100000\n64 = 1000000\n128 = 10000000\n0xAB nibbles: 10 11
// Test: number base conversions

void print_binary(int n) {
    int bits[32];
    int count, i;
    if (n == 0) {
        printf("0");
        return;
    }
    count = 0;
    while (n > 0) {
        bits[count] = n % 2;
        n = n / 2;
        count++;
    }
    for (i = count - 1; i >= 0; i--) {
        printf("%d", bits[i]);
    }
}

void print_octal(int n) {
    int digits[16];
    int count, i;
    if (n == 0) {
        printf("0");
        return;
    }
    count = 0;
    while (n > 0) {
        digits[count] = n % 8;
        n = n / 8;
        count++;
    }
    for (i = count - 1; i >= 0; i--) {
        printf("%d", digits[i]);
    }
}

int from_binary(int bin_digits[], int len) {
    int result, i, power;
    result = 0;
    power = 1;
    for (i = len - 1; i >= 0; i--) {
        result += bin_digits[i] * power;
        power *= 2;
    }
    return result;
}

int main(void) {
    int i;
    int bin[8];

    /* Print binary representations */
    printf("binary:\n");
    for (i = 0; i <= 16; i++) {
        printf("%d = ", i);
        print_binary(i);
        printf("\n");
    }

    /* Print octal */
    printf("octal:\n");
    for (i = 0; i <= 16; i++) {
        printf("%d = ", i);
        print_octal(i);
        printf("\n");
    }

    /* Binary to decimal */
    bin[0] = 1; bin[1] = 0; bin[2] = 1; bin[3] = 0;
    bin[4] = 1; bin[5] = 1; bin[6] = 0; bin[7] = 1;
    printf("10101101 = %d\n", from_binary(bin, 8));

    bin[0] = 1; bin[1] = 1; bin[2] = 1; bin[3] = 1;
    bin[4] = 1; bin[5] = 1; bin[6] = 1; bin[7] = 1;
    printf("11111111 = %d\n", from_binary(bin, 8));

    /* Powers of 2 in binary should be single 1 */
    printf("pow2 binary:\n");
    for (i = 0; i < 8; i++) {
        int p, k;
        p = 1;
        for (k = 0; k < i; k++) p *= 2;
        printf("%d = ", p);
        print_binary(p);
        printf("\n");
    }

    /* Hex-like: print nibbles */
    {
        int n;
        int hi, lo;
        n = 0xAB;
        hi = (n >> 4) & 0xF;
        lo = n & 0xF;
        printf("0xAB nibbles: %d %d\n", hi, lo);
    }

    return 0;
}
