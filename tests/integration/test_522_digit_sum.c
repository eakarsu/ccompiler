int printf(const char *fmt, ...);
// EXPECT: Digit sums:\ndigit_sum(0) = 0\ndigit_sum(5) = 5\ndigit_sum(123) = 6\ndigit_sum(999) = 27\ndigit_sum(12345) = 15\ndigit_sum(99999) = 45\ndigit_sum(100000) = 1\ndigit_sum(54321) = 15\ndigit_sum(11111) = 5\ndigit_sum(86420) = 20\ndigit_sum(13579) = 25\ndigit_sum(1000000) = 1\n\nDigital roots:\ndigital_root(0) = 0\ndigital_root(5) = 5\ndigital_root(123) = 6\ndigital_root(999) = 9\ndigital_root(12345) = 6\ndigital_root(99999) = 9\ndigital_root(100000) = 1\ndigital_root(54321) = 6\ndigital_root(11111) = 5\ndigital_root(86420) = 2\ndigital_root(13579) = 7\ndigital_root(1000000) = 1\n\nDigital root formula verification:\nn=1: dr=1, formula=1, match=1\nn=2: dr=2, formula=2, match=1\nn=3: dr=3, formula=3, match=1\nn=4: dr=4, formula=4, match=1\nn=5: dr=5, formula=5, match=1\nn=6: dr=6, formula=6, match=1\nn=7: dr=7, formula=7, match=1\nn=8: dr=8, formula=8, match=1\nn=9: dr=9, formula=9, match=1\nn=10: dr=1, formula=1, match=1\nn=11: dr=2, formula=2, match=1\nn=12: dr=3, formula=3, match=1\nn=13: dr=4, formula=4, match=1\nn=14: dr=5, formula=5, match=1\nn=15: dr=6, formula=6, match=1\nn=16: dr=7, formula=7, match=1\nn=17: dr=8, formula=8, match=1\nn=18: dr=9, formula=9, match=1\nn=19: dr=1, formula=1, match=1\nn=20: dr=2, formula=2, match=1\nn=21: dr=3, formula=3, match=1\nn=22: dr=4, formula=4, match=1\nn=23: dr=5, formula=5, match=1\nn=24: dr=6, formula=6, match=1\nn=25: dr=7, formula=7, match=1\nn=26: dr=8, formula=8, match=1\nn=27: dr=9, formula=9, match=1\nn=28: dr=1, formula=1, match=1\nn=29: dr=2, formula=2, match=1\nn=30: dr=3, formula=3, match=1\n\nAdditive persistence:\npersistence(0) = 0, root = 0\npersistence(10) = 1, root = 1\npersistence(25) = 1, root = 7\npersistence(39) = 2, root = 3\npersistence(199) = 3, root = 1\npersistence(679) = 2, root = 4\npersistence(6788) = 3, root = 2\npersistence(68889) = 3, root = 3\n\nCumulative digit sums:\nSum of digit_sum(1..10) = 46\nSum of digit_sum(1..20) = 102\nSum of digit_sum(1..30) = 168\nSum of digit_sum(1..40) = 244\nSum of digit_sum(1..50) = 330
// Test: Digit sum and digital root

int digit_sum(int n) {
    int sum = 0;
    if (n < 0) n = -n;
    while (n > 0) {
        sum = sum + n % 10;
        n = n / 10;
    }
    return sum;
}

int digital_root(int n) {
    if (n < 0) n = -n;
    while (n >= 10) {
        n = digit_sum(n);
    }
    return n;
}

int count_digits(int n) {
    if (n == 0) return 1;
    if (n < 0) n = -n;
    int count = 0;
    while (n > 0) {
        count++;
        n = n / 10;
    }
    return count;
}

int main(void) {
    int i;

    printf("Digit sums:\n");
    int test_nums[12];
    test_nums[0] = 0;
    test_nums[1] = 5;
    test_nums[2] = 123;
    test_nums[3] = 999;
    test_nums[4] = 12345;
    test_nums[5] = 99999;
    test_nums[6] = 100000;
    test_nums[7] = 54321;
    test_nums[8] = 11111;
    test_nums[9] = 86420;
    test_nums[10] = 13579;
    test_nums[11] = 1000000;

    for (i = 0; i < 12; i++) {
        printf("digit_sum(%d) = %d\n", test_nums[i], digit_sum(test_nums[i]));
    }

    printf("\nDigital roots:\n");
    for (i = 0; i < 12; i++) {
        printf("digital_root(%d) = %d\n", test_nums[i], digital_root(test_nums[i]));
    }

    // Digital root formula: dr(n) = 1 + (n-1)%9 for n>0
    printf("\nDigital root formula verification:\n");
    for (i = 1; i <= 30; i++) {
        int dr = digital_root(i);
        int formula = 1 + (i - 1) % 9;
        printf("n=%d: dr=%d, formula=%d, match=%d\n", i, dr, formula, dr == formula);
    }

    // Additive persistence: how many times to sum digits until single digit
    printf("\nAdditive persistence:\n");
    int persist_tests[8];
    persist_tests[0] = 0;
    persist_tests[1] = 10;
    persist_tests[2] = 25;
    persist_tests[3] = 39;
    persist_tests[4] = 199;
    persist_tests[5] = 679;
    persist_tests[6] = 6788;
    persist_tests[7] = 68889;

    for (i = 0; i < 8; i++) {
        int n = persist_tests[i];
        int pers = 0;
        int temp = n;
        while (temp >= 10) {
            temp = digit_sum(temp);
            pers++;
        }
        printf("persistence(%d) = %d, root = %d\n", n, pers, temp);
    }

    // Sum of digit sums from 1 to n
    printf("\nCumulative digit sums:\n");
    int cumsum = 0;
    for (i = 1; i <= 50; i++) {
        cumsum = cumsum + digit_sum(i);
        if (i % 10 == 0) {
            printf("Sum of digit_sum(1..%d) = %d\n", i, cumsum);
        }
    }

    return 0;
}
