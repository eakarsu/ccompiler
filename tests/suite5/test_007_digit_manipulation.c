int printf(const char *fmt, ...);

int digit_sum(int n) {
    int sum = 0;
    if (n < 0) n = 0 - n;
    while (n > 0) {
        sum = sum + (n % 10);
        n = n / 10;
    }
    return sum;
}

int digit_product(int n) {
    if (n == 0) return 0;
    int prod = 1;
    while (n > 0) {
        prod = prod * (n % 10);
        n = n / 10;
    }
    return prod;
}

int count_digits(int n) {
    if (n == 0) return 1;
    int count = 0;
    while (n > 0) {
        count = count + 1;
        n = n / 10;
    }
    return count;
}

int reverse_number(int n) {
    int rev = 0;
    while (n > 0) {
        rev = rev * 10 + (n % 10);
        n = n / 10;
    }
    return rev;
}

int is_palindrome(int n) {
    return n == reverse_number(n);
}

int power(int base, int exp) {
    int result = 1;
    int i = 0;
    while (i < exp) {
        result = result * base;
        i = i + 1;
    }
    return result;
}

int is_armstrong(int n) {
    int digits = count_digits(n);
    int sum = 0;
    int temp = n;
    while (temp > 0) {
        int d = temp % 10;
        sum = sum + power(d, digits);
        temp = temp / 10;
    }
    return sum == n;
}

int digital_root(int n) {
    while (n >= 10) {
        n = digit_sum(n);
    }
    return n;
}

int get_digit(int n, int pos) {
    int i = 0;
    while (i < pos) {
        n = n / 10;
        i = i + 1;
    }
    return n % 10;
}

int largest_digit(int n) {
    int max = 0;
    while (n > 0) {
        int d = n % 10;
        if (d > max) max = d;
        n = n / 10;
    }
    return max;
}

int smallest_digit(int n) {
    int min = 9;
    while (n > 0) {
        int d = n % 10;
        if (d < min) min = d;
        n = n / 10;
    }
    return min;
}

int count_even_digits(int n) {
    int count = 0;
    while (n > 0) {
        if ((n % 10) % 2 == 0) count = count + 1;
        n = n / 10;
    }
    return count;
}

int is_harshad(int n) {
    return n % digit_sum(n) == 0;
}

int sum_palindromes(int limit) {
    int sum = 0;
    int i = 1;
    while (i <= limit) {
        if (is_palindrome(i)) sum = sum + i;
        i = i + 1;
    }
    return sum;
}

int main(void) {
    // EXPECT: dsum(12345) = 15
    printf("dsum(12345) = %d\n", digit_sum(12345));
    // EXPECT: dsum(9999) = 36
    printf("dsum(9999) = %d\n", digit_sum(9999));

    // EXPECT: dprod(234) = 24
    printf("dprod(234) = %d\n", digit_product(234));
    // EXPECT: dprod(111) = 1
    printf("dprod(111) = %d\n", digit_product(111));

    // EXPECT: ndig(12345) = 5
    printf("ndig(12345) = %d\n", count_digits(12345));
    // EXPECT: ndig(0) = 1
    printf("ndig(0) = %d\n", count_digits(0));

    // EXPECT: rev(12345) = 54321
    printf("rev(12345) = %d\n", reverse_number(12345));
    // EXPECT: rev(1001) = 1001
    printf("rev(1001) = %d\n", reverse_number(1001));

    // EXPECT: pal(121) = 1
    printf("pal(121) = %d\n", is_palindrome(121));
    // EXPECT: pal(123) = 0
    printf("pal(123) = %d\n", is_palindrome(123));

    // EXPECT: arm(153) = 1
    printf("arm(153) = %d\n", is_armstrong(153));
    // EXPECT: arm(370) = 1
    printf("arm(370) = %d\n", is_armstrong(370));
    // EXPECT: arm(100) = 0
    printf("arm(100) = %d\n", is_armstrong(100));

    // EXPECT: droot(9875) = 2
    printf("droot(9875) = %d\n", digital_root(9875));

    // EXPECT: dig(12345,2) = 3
    printf("dig(12345,2) = %d\n", get_digit(12345, 2));

    // EXPECT: maxd(38291) = 9
    printf("maxd(38291) = %d\n", largest_digit(38291));
    // EXPECT: mind(38291) = 1
    printf("mind(38291) = %d\n", smallest_digit(38291));

    // EXPECT: even_d(12345) = 2
    printf("even_d(12345) = %d\n", count_even_digits(12345));

    // EXPECT: harsh(18) = 1
    printf("harsh(18) = %d\n", is_harshad(18));

    // EXPECT: pal_sum(100) = 540
    printf("pal_sum(100) = %d\n", sum_palindromes(100));

    return 0;
}
