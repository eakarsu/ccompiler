int printf(const char *fmt, ...);
// EXPECT: 100 in different bases:\nBase  2: 1100100\nBase  3: 10201\nBase  4: 1210\nBase  5: 400\nBase  6: 244\nBase  7: 202\nBase  8: 144\nBase  9: 121\nBase 10: 100\nBase 11: 91\nBase 12: 84\nBase 13: 79\nBase 14: 72\nBase 15: 6A\nBase 16: 64\n\n255 in different bases:\nBase  2: 11111111\nBase  3: 100110\nBase  4: 3333\nBase  5: 2010\nBase  6: 1103\nBase  7: 513\nBase  8: 377\nBase  9: 313\nBase 10: 255\nBase 11: 212\nBase 12: 193\nBase 13: 168\nBase 14: 143\nBase 15: 120\nBase 16: FF\n\nDigit counts for 1000:\nBase  2: 10 digits\nBase  3: 7 digits\nBase  4: 5 digits\nBase  5: 5 digits\nBase  6: 4 digits\nBase  7: 4 digits\nBase  8: 4 digits\nBase  9: 4 digits\nBase 10: 4 digits\nBase 11: 3 digits\nBase 12: 3 digits\nBase 13: 3 digits\nBase 14: 3 digits\nBase 15: 3 digits\nBase 16: 3 digits\n\nSelf-descriptive check in base 4 (number 1210 in base 4 = 100):\nDigits: 1210\nCount of 0: 1 (digit says 1)\nCount of 1: 2 (digit says 2)\nCount of 2: 1 (digit says 1)\nCount of 3: 0 (digit says 0)\nSelf-descriptive: 1\n\nBalanced ternary (digits -1,0,1 shown as T,0,1):\n  1 = 1\n  2 = 1T\n  3 = 10\n  5 = 1TT\n 10 = 101\n 15 = 1TT0\n 20 = 1T1T\n 42 = 1TTT0
// Test: Base conversion

void print_in_base(int n, int base) {
    if (n == 0) {
        printf("0");
        return;
    }
    int digits[32];
    int count = 0;
    int temp = n;
    while (temp > 0) {
        digits[count] = temp % base;
        count++;
        temp = temp / base;
    }
    int i;
    for (i = count - 1; i >= 0; i--) {
        if (digits[i] < 10)
            printf("%d", digits[i]);
        else
            printf("%c", 'A' + digits[i] - 10);
    }
}

int digit_count_in_base(int n, int base) {
    if (n == 0) return 1;
    int count = 0;
    while (n > 0) {
        count++;
        n = n / base;
    }
    return count;
}

int main(void) {
    int i, b;

    // Convert 100 to different bases
    printf("100 in different bases:\n");
    for (b = 2; b <= 16; b++) {
        printf("Base %2d: ", b);
        print_in_base(100, b);
        printf("\n");
    }

    // Convert 255 to different bases
    printf("\n255 in different bases:\n");
    for (b = 2; b <= 16; b++) {
        printf("Base %2d: ", b);
        print_in_base(255, b);
        printf("\n");
    }

    // Number of digits needed in each base
    printf("\nDigit counts for 1000:\n");
    for (b = 2; b <= 16; b++) {
        printf("Base %2d: %d digits\n", b, digit_count_in_base(1000, b));
    }

    // Self-descriptive numbers in base 10
    // 6210001000 is too large, check smaller bases
    // Base 4: 1210, Base 5: 21200
    printf("\nSelf-descriptive check in base 4 (number 1210 in base 4 = 100):\n");
    // 1210 base 4 = 1*64 + 2*16 + 1*4 + 0 = 100
    int sd = 100;
    int digs[4];
    int temp = sd;
    for (i = 0; i < 4; i++) {
        digs[3 - i] = temp % 4;
        temp = temp / 4;
    }
    printf("Digits: ");
    for (i = 0; i < 4; i++) printf("%d", digs[i]);
    printf("\n");
    // Check: digit i should be count of i in the number
    int valid = 1;
    for (i = 0; i < 4; i++) {
        int cnt = 0;
        int j;
        for (j = 0; j < 4; j++) {
            if (digs[j] == i) cnt++;
        }
        printf("Count of %d: %d (digit says %d)\n", i, cnt, digs[i]);
        if (cnt != digs[i]) valid = 0;
    }
    printf("Self-descriptive: %d\n", valid);

    // Balanced ternary representation
    printf("\nBalanced ternary (digits -1,0,1 shown as T,0,1):\n");
    int bt_nums[8];
    bt_nums[0] = 1; bt_nums[1] = 2; bt_nums[2] = 3;
    bt_nums[3] = 5; bt_nums[4] = 10; bt_nums[5] = 15;
    bt_nums[6] = 20; bt_nums[7] = 42;

    for (i = 0; i < 8; i++) {
        int n = bt_nums[i];
        int bt[10];
        int nb = 0;
        temp = n;
        while (temp > 0) {
            int rem = temp % 3;
            if (rem == 2) {
                bt[nb] = -1;
                temp = (temp + 1) / 3;
            } else {
                bt[nb] = rem;
                temp = temp / 3;
            }
            nb++;
        }
        printf("%3d = ", n);
        int j;
        for (j = nb - 1; j >= 0; j--) {
            if (bt[j] == -1) printf("T");
            else printf("%d", bt[j]);
        }
        printf("\n");
    }

    return 0;
}
