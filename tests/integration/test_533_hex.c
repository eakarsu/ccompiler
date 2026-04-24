int printf(const char *fmt, ...);
// EXPECT: Decimal to Hex:\n     0 -> 0x0\n     9 -> 0x9\n    10 -> 0xA\n    15 -> 0xF\n    16 -> 0x10\n    42 -> 0x2A\n   100 -> 0x64\n   255 -> 0xFF\n   256 -> 0x100\n  1000 -> 0x3E8\n  4095 -> 0xFFF\n  4096 -> 0x1000\n 65535 -> 0xFFFF\n 48879 -> 0xBEEF\n\nPowers of 16:\n16^0 = 1 = 0x1\n16^1 = 16 = 0x10\n16^2 = 256 = 0x100\n16^3 = 4096 = 0x1000\n16^4 = 65536 = 0x10000\n16^5 = 1048576 = 0x100000\n16^6 = 16777216 = 0x1000000\n16^7 = 268435456 = 0x10000000\n\nHex palindromes up to 300:\n1(0x1) 2(0x2) 3(0x3) 4(0x4) 5(0x5) 6(0x6) 7(0x7) 8(0x8) 9(0x9) 10(0xA) 11(0xB) 12(0xC) 13(0xD) 14(0xE) 15(0xF) 17(0x11) 34(0x22) 51(0x33) 68(0x44) 85(0x55) 102(0x66) 119(0x77) 136(0x88) 153(0x99) 170(0xAA) 187(0xBB) 204(0xCC) 221(0xDD) 238(0xEE) 255(0xFF) 257(0x101) 273(0x111) 289(0x121) \n\nHex digit sums:\n0 -> hex digit sum = 0\n9 -> hex digit sum = 9\n10 -> hex digit sum = 10\n15 -> hex digit sum = 15\n16 -> hex digit sum = 1\n42 -> hex digit sum = 12\n100 -> hex digit sum = 10\n255 -> hex digit sum = 30\n256 -> hex digit sum = 1\n1000 -> hex digit sum = 25\n4095 -> hex digit sum = 45\n4096 -> hex digit sum = 1\n65535 -> hex digit sum = 60\n48879 -> hex digit sum = 54\n\nNumbers with all hex digits >= A (up to 300):\n10(0xA) 11(0xB) 12(0xC) 13(0xD) 14(0xE) 15(0xF) 170(0xAA) 171(0xAB) 172(0xAC) 173(0xAD) 174(0xAE) 175(0xAF) 186(0xBA) 187(0xBB) 188(0xBC) 189(0xBD) 190(0xBE) 191(0xBF) 202(0xCA) 203(0xCB) 204(0xCC) 205(0xCD) 206(0xCE) 207(0xCF) 218(0xDA) 219(0xDB) 220(0xDC) 221(0xDD) 222(0xDE) 223(0xDF) 234(0xEA) 235(0xEB) 236(0xEC) 237(0xED) 238(0xEE) 239(0xEF) 250(0xFA) 251(0xFB) 252(0xFC) 253(0xFD) 254(0xFE) 255(0xFF) 
// Test: Hexadecimal representation

void print_hex(int n) {
    if (n == 0) {
        printf("0");
        return;
    }
    char hex_chars[16];
    hex_chars[0] = '0'; hex_chars[1] = '1'; hex_chars[2] = '2'; hex_chars[3] = '3';
    hex_chars[4] = '4'; hex_chars[5] = '5'; hex_chars[6] = '6'; hex_chars[7] = '7';
    hex_chars[8] = '8'; hex_chars[9] = '9'; hex_chars[10] = 'A'; hex_chars[11] = 'B';
    hex_chars[12] = 'C'; hex_chars[13] = 'D'; hex_chars[14] = 'E'; hex_chars[15] = 'F';

    int digits[10];
    int count = 0;
    int temp = n;
    while (temp > 0) {
        digits[count] = temp % 16;
        count++;
        temp = temp / 16;
    }
    int i;
    for (i = count - 1; i >= 0; i--) {
        printf("%c", hex_chars[digits[i]]);
    }
}

int main(void) {
    int i;

    printf("Decimal to Hex:\n");
    int nums[14];
    nums[0] = 0; nums[1] = 9; nums[2] = 10; nums[3] = 15;
    nums[4] = 16; nums[5] = 42; nums[6] = 100; nums[7] = 255;
    nums[8] = 256; nums[9] = 1000; nums[10] = 4095; nums[11] = 4096;
    nums[12] = 65535; nums[13] = 48879;

    for (i = 0; i < 14; i++) {
        printf("%6d -> 0x", nums[i]);
        print_hex(nums[i]);
        printf("\n");
    }

    // Powers of 16
    printf("\nPowers of 16:\n");
    int p = 1;
    for (i = 0; i < 8; i++) {
        printf("16^%d = %d = 0x", i, p);
        print_hex(p);
        printf("\n");
        if (i < 7) p = p * 16;
    }

    // Hex palindromes
    printf("\nHex palindromes up to 300:\n");
    for (i = 1; i <= 300; i++) {
        int digits[8];
        int nd = 0;
        int temp = i;
        while (temp > 0) {
            digits[nd] = temp % 16;
            nd++;
            temp = temp / 16;
        }
        int is_pal = 1;
        int j;
        for (j = 0; j < nd / 2; j++) {
            if (digits[j] != digits[nd - 1 - j]) { is_pal = 0; break; }
        }
        if (is_pal) {
            printf("%d(0x", i);
            print_hex(i);
            printf(") ");
        }
    }
    printf("\n");

    // Hex digit sum
    printf("\nHex digit sums:\n");
    for (i = 0; i < 14; i++) {
        int sum = 0;
        int temp = nums[i];
        if (temp == 0) sum = 0;
        while (temp > 0) {
            sum = sum + temp % 16;
            temp = temp / 16;
        }
        printf("%d -> hex digit sum = %d\n", nums[i], sum);
    }

    // Numbers with only hex digits A-F (i.e., all hex digits >= 10)
    printf("\nNumbers with all hex digits >= A (up to 300):\n");
    for (i = 10; i <= 300; i++) {
        int temp = i;
        int all_af = 1;
        while (temp > 0) {
            if (temp % 16 < 10) { all_af = 0; break; }
            temp = temp / 16;
        }
        if (all_af) {
            printf("%d(0x", i);
            print_hex(i);
            printf(") ");
        }
    }
    printf("\n");

    return 0;
}
