int printf(const char *fmt, ...);
// EXPECT: Binary representations:\n   0 = 0\n   1 = 1\n   2 = 10\n   7 = 111\n  10 = 1010\n  42 = 101010\n 100 = 1100100\n 127 = 1111111\n 128 = 10000000\n 255 = 11111111\n1000 = 1111101000\n1023 = 1111111111\n\nBit counts:\n0: ones=0, bits=1\n1: ones=1, bits=1\n2: ones=1, bits=2\n7: ones=3, bits=3\n10: ones=2, bits=4\n42: ones=3, bits=6\n100: ones=3, bits=7\n127: ones=7, bits=7\n128: ones=1, bits=8\n255: ones=8, bits=8\n1000: ones=6, bits=10\n1023: ones=10, bits=10\n\nPowers of 2:\n2^ 0 =     1 = 1\n2^ 1 =     2 = 10\n2^ 2 =     4 = 100\n2^ 3 =     8 = 1000\n2^ 4 =    16 = 10000\n2^ 5 =    32 = 100000\n2^ 6 =    64 = 1000000\n2^ 7 =   128 = 10000000\n2^ 8 =   256 = 100000000\n2^ 9 =   512 = 1000000000\n2^10 =  1024 = 10000000000\n2^11 =  2048 = 100000000000\n2^12 =  4096 = 1000000000000\n2^13 =  8192 = 10000000000000\n2^14 = 16384 = 100000000000000\n2^15 = 32768 = 1000000000000000\n\nBinary palindromes up to 100:\n1(1) 3(11) 5(101) 7(111) 9(1001) 15(1111) 17(10001) 21(10101) 27(11011) 31(11111) 33(100001) 45(101101) 51(110011) 63(111111) 65(1000001) 73(1001001) 85(1010101) 93(1011101) 99(1100011) \n\nHamming distances:\nhamming(1, 4) = 2\nhamming(7, 0) = 3\nhamming(15, 10) = 2\nhamming(255, 128) = 7
// Test: Binary representation

void print_binary(int n) {
    if (n == 0) {
        printf("0");
        return;
    }
    int bits[32];
    int count = 0;
    int temp = n;
    while (temp > 0) {
        bits[count] = temp % 2;
        count++;
        temp = temp / 2;
    }
    int i;
    for (i = count - 1; i >= 0; i--) {
        printf("%d", bits[i]);
    }
}

int count_ones(int n) {
    int count = 0;
    while (n > 0) {
        count = count + (n % 2);
        n = n / 2;
    }
    return count;
}

int bit_length(int n) {
    if (n == 0) return 1;
    int len = 0;
    while (n > 0) {
        len++;
        n = n / 2;
    }
    return len;
}

int main(void) {
    int i;

    printf("Binary representations:\n");
    int nums[12];
    nums[0] = 0; nums[1] = 1; nums[2] = 2; nums[3] = 7;
    nums[4] = 10; nums[5] = 42; nums[6] = 100; nums[7] = 127;
    nums[8] = 128; nums[9] = 255; nums[10] = 1000; nums[11] = 1023;

    for (i = 0; i < 12; i++) {
        printf("%4d = ", nums[i]);
        print_binary(nums[i]);
        printf("\n");
    }

    // Popcount (number of 1 bits)
    printf("\nBit counts:\n");
    for (i = 0; i < 12; i++) {
        printf("%d: ones=%d, bits=%d\n", nums[i], count_ones(nums[i]), bit_length(nums[i]));
    }

    // Powers of 2 in binary
    printf("\nPowers of 2:\n");
    int p = 1;
    for (i = 0; i < 16; i++) {
        printf("2^%2d = %5d = ", i, p);
        print_binary(p);
        printf("\n");
        p = p * 2;
    }

    // Binary palindromes
    printf("\nBinary palindromes up to 100:\n");
    for (i = 1; i <= 100; i++) {
        int bits[10];
        int nb = 0;
        int temp = i;
        while (temp > 0) {
            bits[nb] = temp % 2;
            nb++;
            temp = temp / 2;
        }
        int is_pal = 1;
        int j;
        for (j = 0; j < nb / 2; j++) {
            if (bits[j] != bits[nb - 1 - j]) {
                is_pal = 0;
                break;
            }
        }
        if (is_pal) {
            printf("%d(", i);
            print_binary(i);
            printf(") ");
        }
    }
    printf("\n");

    // Hamming distance between pairs
    printf("\nHamming distances:\n");
    int pairs_a[4], pairs_b[4];
    pairs_a[0] = 1; pairs_b[0] = 4;
    pairs_a[1] = 7; pairs_b[1] = 0;
    pairs_a[2] = 15; pairs_b[2] = 10;
    pairs_a[3] = 255; pairs_b[3] = 128;

    for (i = 0; i < 4; i++) {
        int xor_val = pairs_a[i];
        int temp = pairs_b[i];
        // XOR manually using bit operations
        int result = 0;
        int bit = 1;
        int a = pairs_a[i], b = pairs_b[i];
        while (a > 0 || b > 0) {
            int ba = a % 2;
            int bb = b % 2;
            if (ba != bb) result = result + bit;
            a = a / 2;
            b = b / 2;
            bit = bit * 2;
        }
        printf("hamming(%d, %d) = %d\n", pairs_a[i], pairs_b[i], count_ones(result));
    }

    return 0;
}
