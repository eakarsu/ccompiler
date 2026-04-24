int printf(const char *fmt, ...);
// EXPECT: digits(0) = 1\ndigits(5) = 1\ndigits(42) = 2\ndigits(12345) = 5\ndigits(-99) = 2\ndigitsum(123) = 6\ndigitsum(9999) = 36\ndigitsum(12345) = 15\nrev(123) = 321\nrev(4567) = 7654\nrev(1000) = 1\nrev(-123) = -321\npalindromes: 1 2 3 4 5 6 7 8 9 11 22 33 44 55 66 77 88 99 101 111 121 131 141 151 161 171 181 191 \ndigits of 54321: 5 4 3 2 1 \ndigital root of 9875: 2
// Test: digit manipulation

int count_digits(int n) {
    int count;
    if (n == 0) return 1;
    if (n < 0) n = -n;
    count = 0;
    while (n > 0) {
        count++;
        n = n / 10;
    }
    return count;
}

int sum_digits(int n) {
    int sum;
    if (n < 0) n = -n;
    sum = 0;
    while (n > 0) {
        sum += n % 10;
        n = n / 10;
    }
    return sum;
}

int reverse_number(int n) {
    int rev;
    int neg;
    neg = 0;
    if (n < 0) {
        neg = 1;
        n = -n;
    }
    rev = 0;
    while (n > 0) {
        rev = rev * 10 + n % 10;
        n = n / 10;
    }
    if (neg) rev = -rev;
    return rev;
}

int is_palindrome(int n) {
    return n >= 0 && n == reverse_number(n);
}

int main(void) {
    int i;
    int n;

    /* Count digits */
    printf("digits(0) = %d\n", count_digits(0));
    printf("digits(5) = %d\n", count_digits(5));
    printf("digits(42) = %d\n", count_digits(42));
    printf("digits(12345) = %d\n", count_digits(12345));
    printf("digits(-99) = %d\n", count_digits(-99));

    /* Sum of digits */
    printf("digitsum(123) = %d\n", sum_digits(123));
    printf("digitsum(9999) = %d\n", sum_digits(9999));
    printf("digitsum(12345) = %d\n", sum_digits(12345));

    /* Reverse number */
    printf("rev(123) = %d\n", reverse_number(123));
    printf("rev(4567) = %d\n", reverse_number(4567));
    printf("rev(1000) = %d\n", reverse_number(1000));
    printf("rev(-123) = %d\n", reverse_number(-123));

    /* Palindrome check */
    printf("palindromes: ");
    for (i = 1; i <= 200; i++) {
        if (is_palindrome(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");

    /* Extract individual digits */
    n = 54321;
    printf("digits of %d: ", n);
    {
        int digits[10];
        int cnt, j;
        cnt = 0;
        while (n > 0) {
            digits[cnt++] = n % 10;
            n = n / 10;
        }
        for (j = cnt - 1; j >= 0; j--) {
            printf("%d ", digits[j]);
        }
        printf("\n");
    }

    /* Digital root */
    n = 9875;
    printf("digital root of 9875: ");
    while (n >= 10) {
        n = sum_digits(n);
    }
    printf("%d\n", n);

    return 0;
}
