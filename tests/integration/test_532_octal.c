int printf(const char *fmt, ...);
// EXPECT: Decimal to Octal:\n    0 -> 0\n    1 -> 1\n    7 -> 7\n    8 -> 10\n   10 -> 12\n   42 -> 52\n   63 -> 77\n   64 -> 100\n  100 -> 144\n  255 -> 377\n  511 -> 777\n  512 -> 1000\n  999 -> 1747\n 1000 -> 1750\n 4095 -> 7777\n\nOctal to Decimal:\n07 -> 7\n010 -> 8\n017 -> 15\n077 -> 63\n0100 -> 64\n0177 -> 127\n0377 -> 255\n0777 -> 511\n\nOctal arithmetic:\n7 + 1 = 8 (octal: 10)\n63 + 63 = 126 (octal: 176)\n\nOctal digit counts:\n0: 1 octal digits\n1: 1 octal digits\n7: 1 octal digits\n8: 2 octal digits\n10: 2 octal digits\n42: 2 octal digits\n63: 2 octal digits\n64: 3 octal digits\n100: 3 octal digits\n255: 3 octal digits\n511: 3 octal digits\n512: 4 octal digits\n999: 4 octal digits\n1000: 4 octal digits\n4095: 4 octal digits\n\nRepdigit octal numbers up to 1000:\n9 (11) 18 (22) 27 (33) 36 (44) 45 (55) 54 (66) 63 (77) 73 (111) 146 (222) 219 (333) 292 (444) 365 (555) 438 (666) 511 (777) 585 (1111) 
// Test: Octal representation

void print_octal(int n) {
    if (n == 0) {
        printf("0");
        return;
    }
    int digits[20];
    int count = 0;
    int temp = n;
    while (temp > 0) {
        digits[count] = temp % 8;
        count++;
        temp = temp / 8;
    }
    int i;
    for (i = count - 1; i >= 0; i--) {
        printf("%d", digits[i]);
    }
}

int from_octal(int oct) {
    // Convert octal representation (as decimal) to decimal
    int result = 0;
    int base = 1;
    while (oct > 0) {
        result = result + (oct % 10) * base;
        oct = oct / 10;
        base = base * 8;
    }
    return result;
}

int main(void) {
    int i;

    printf("Decimal to Octal:\n");
    int nums[15];
    nums[0] = 0; nums[1] = 1; nums[2] = 7; nums[3] = 8;
    nums[4] = 10; nums[5] = 42; nums[6] = 63; nums[7] = 64;
    nums[8] = 100; nums[9] = 255; nums[10] = 511; nums[11] = 512;
    nums[12] = 999; nums[13] = 1000; nums[14] = 4095;

    for (i = 0; i < 15; i++) {
        printf("%5d -> ", nums[i]);
        print_octal(nums[i]);
        printf("\n");
    }

    // Octal to decimal conversion
    printf("\nOctal to Decimal:\n");
    int octals[8];
    octals[0] = 7;
    octals[1] = 10;
    octals[2] = 17;
    octals[3] = 77;
    octals[4] = 100;
    octals[5] = 177;
    octals[6] = 377;
    octals[7] = 777;

    for (i = 0; i < 8; i++) {
        printf("0%d -> %d\n", octals[i], from_octal(octals[i]));
    }

    // Octal addition: add two numbers in octal
    printf("\nOctal arithmetic:\n");
    int a = 7, b = 1; // 7 + 1 = 10 in octal
    printf("%d + %d = %d (octal: ", a, b, a + b);
    print_octal(a + b);
    printf(")\n");

    a = 77; b = 77; // These are decimal 63 + 63 = 126
    printf("%d + %d = %d (octal: ", from_octal(a), from_octal(b),
           from_octal(a) + from_octal(b));
    print_octal(from_octal(a) + from_octal(b));
    printf(")\n");

    // Count digits in octal
    printf("\nOctal digit counts:\n");
    for (i = 0; i < 15; i++) {
        int n = nums[i];
        int oct_digits = 0;
        int temp = n;
        if (temp == 0) oct_digits = 1;
        while (temp > 0) {
            oct_digits++;
            temp = temp / 8;
        }
        printf("%d: %d octal digits\n", n, oct_digits);
    }

    // Numbers where all octal digits are the same
    printf("\nRepdigit octal numbers up to 1000:\n");
    for (i = 1; i <= 1000; i++) {
        int digits[10];
        int nd = 0;
        int temp = i;
        while (temp > 0) {
            digits[nd] = temp % 8;
            nd++;
            temp = temp / 8;
        }
        int all_same = 1;
        int j;
        for (j = 1; j < nd; j++) {
            if (digits[j] != digits[0]) { all_same = 0; break; }
        }
        if (all_same && nd > 1) {
            printf("%d (", i);
            print_octal(i);
            printf(") ");
        }
    }
    printf("\n");

    return 0;
}
