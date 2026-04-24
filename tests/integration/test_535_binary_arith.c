int printf(const char *fmt, ...);
// EXPECT: Binary addition: 13 + 11 = 24\n  00001101  (13)\n+ 00001011  (11)\n= 00011000  (24)\n\nBinary subtraction: 25 - 11 = 14\n  00011001  (25)\n- 00001011  (11)\n= 00001110  (14)\n\nBinary multiplication: 5 * 6 = 30\n  shift 1: 00001010  (10)\n  shift 2: 00010100  (20)\n  result: 00011110  (30)\n\nBitwise operations:\n12 AND 10 = 8\n12 OR  10 = 14\n12 XOR 10 = 6\n15 AND 9 = 9\n15 OR  9 = 15\n15 XOR 9 = 6\n255 AND 170 = 170\n255 OR  170 = 255\n255 XOR 170 = 85\n60 AND 45 = 44\n60 OR  45 = 61\n60 XOR 45 = 17\n\nPopulation count 0-31:\n0:0 1:1 2:1 3:2 4:1 5:2 6:2 7:3 \n8:1 9:2 10:2 11:3 12:2 13:3 14:3 15:4 \n16:1 17:2 18:2 19:3 20:2 21:3 22:3 23:4 \n24:2 25:3 26:3 27:4 28:3 29:4 30:4 31:5 
// Test: Binary arithmetic

void print_binary(int n, int width) {
    int i;
    for (i = width - 1; i >= 0; i--) {
        int bit = (n / 1) % 2;
        int temp = n;
        int j;
        for (j = 0; j < i; j++) temp = temp / 2;
        printf("%d", temp % 2);
    }
}

int popcount(int n) {
    int count = 0;
    while (n > 0) {
        count = count + (n % 2);
        n = n / 2;
    }
    return count;
}

int main(void) {
    int i;

    // Binary addition step by step
    printf("Binary addition: 13 + 11 = 24\n");
    printf("  ");
    print_binary(13, 8);
    printf("  (13)\n");
    printf("+ ");
    print_binary(11, 8);
    printf("  (11)\n");
    printf("= ");
    print_binary(24, 8);
    printf("  (24)\n");

    // Binary subtraction
    printf("\nBinary subtraction: 25 - 11 = 14\n");
    printf("  ");
    print_binary(25, 8);
    printf("  (25)\n");
    printf("- ");
    print_binary(11, 8);
    printf("  (11)\n");
    printf("= ");
    print_binary(14, 8);
    printf("  (14)\n");

    // Binary multiplication (shift-and-add)
    printf("\nBinary multiplication: 5 * 6 = 30\n");
    int a = 5, b = 6;
    int result = 0;
    int shift = 0;
    int temp_b = b;
    while (temp_b > 0) {
        if (temp_b % 2 == 1) {
            int shifted = a;
            int k;
            for (k = 0; k < shift; k++) shifted = shifted * 2;
            printf("  shift %d: ", shift);
            print_binary(shifted, 8);
            printf("  (%d)\n", shifted);
            result = result + shifted;
        }
        temp_b = temp_b / 2;
        shift++;
    }
    printf("  result: ");
    print_binary(result, 8);
    printf("  (%d)\n", result);

    // Bitwise AND, OR, XOR using division/modulo
    printf("\nBitwise operations:\n");
    int pairs_a[4], pairs_b[4];
    pairs_a[0] = 12; pairs_b[0] = 10;
    pairs_a[1] = 15; pairs_b[1] = 9;
    pairs_a[2] = 255; pairs_b[2] = 170;
    pairs_a[3] = 60; pairs_b[3] = 45;

    for (i = 0; i < 4; i++) {
        int and_r = 0, or_r = 0, xor_r = 0;
        int bit = 1;
        int ta = pairs_a[i], tb = pairs_b[i];
        while (ta > 0 || tb > 0) {
            int ba = ta % 2, bb = tb % 2;
            if (ba == 1 && bb == 1) and_r = and_r + bit;
            if (ba == 1 || bb == 1) or_r = or_r + bit;
            if (ba != bb) xor_r = xor_r + bit;
            ta = ta / 2;
            tb = tb / 2;
            bit = bit * 2;
        }
        printf("%d AND %d = %d\n", pairs_a[i], pairs_b[i], and_r);
        printf("%d OR  %d = %d\n", pairs_a[i], pairs_b[i], or_r);
        printf("%d XOR %d = %d\n", pairs_a[i], pairs_b[i], xor_r);
    }

    // Population count for 0-31
    printf("\nPopulation count 0-31:\n");
    for (i = 0; i <= 31; i++) {
        printf("%d:%d ", i, popcount(i));
        if (i % 8 == 7) printf("\n");
    }

    return 0;
}
