int printf(const char *fmt, ...);

// Count digits in a number
int count_digits(int n) {
    int count;
    if (n == 0) return 1;
    if (n < 0) n = -n;
    count = 0;
    while (n > 0) {
        count = count + 1;
        n = n / 10;
    }
    return count;
}

// Sum of digits
int digit_sum(int n) {
    int s;
    if (n < 0) n = -n;
    s = 0;
    while (n > 0) {
        s = s + n % 10;
        n = n / 10;
    }
    return s;
}

// Product of digits
int digit_product(int n) {
    int p;
    if (n < 0) n = -n;
    if (n == 0) return 0;
    p = 1;
    while (n > 0) {
        p = p * (n % 10);
        n = n / 10;
    }
    return p;
}

// Reverse a number
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
    if (neg) return -rev;
    return rev;
}

// Check if palindrome number
int is_palindrome(int n) {
    if (n < 0) return 0;
    return n == reverse_number(n);
}

// Get nth digit from right (0-indexed)
int get_digit(int n, int pos) {
    int i;
    if (n < 0) n = -n;
    for (i = 0; i < pos; i++) {
        n = n / 10;
    }
    return n % 10;
}

// Check if Armstrong number (narcissistic)
// Sum of (each digit ^ num_digits) == number
int power(int base, int exp) {
    int result;
    int i;
    result = 1;
    for (i = 0; i < exp; i++) {
        result = result * base;
    }
    return result;
}

int is_armstrong(int n) {
    int nd;
    int sum;
    int temp;
    if (n < 0) return 0;
    nd = count_digits(n);
    sum = 0;
    temp = n;
    while (temp > 0) {
        sum = sum + power(temp % 10, nd);
        temp = temp / 10;
    }
    return sum == n;
}

// Digital root (repeated digit sum until single digit)
int digital_root(int n) {
    if (n < 0) n = -n;
    while (n >= 10) {
        n = digit_sum(n);
    }
    return n;
}

// Check if happy number (iterate sum of squared digits -> 1)
int is_happy(int n) {
    int slow;
    int fast;
    int temp;
    int s;
    if (n <= 0) return 0;
    slow = n;
    fast = n;
    while (1) {
        // advance slow by 1 step
        s = 0;
        temp = slow;
        while (temp > 0) {
            s = s + (temp % 10) * (temp % 10);
            temp = temp / 10;
        }
        slow = s;

        // advance fast by 2 steps
        s = 0;
        temp = fast;
        while (temp > 0) {
            s = s + (temp % 10) * (temp % 10);
            temp = temp / 10;
        }
        fast = s;
        s = 0;
        temp = fast;
        while (temp > 0) {
            s = s + (temp % 10) * (temp % 10);
            temp = temp / 10;
        }
        fast = s;

        if (fast == 1) return 1;
        if (slow == fast) return 0;
    }
}

// Count numbers with digit sum = target in range [1, limit]
int count_with_digit_sum(int limit, int target) {
    int count;
    int i;
    count = 0;
    for (i = 1; i <= limit; i++) {
        if (digit_sum(i) == target) {
            count = count + 1;
        }
    }
    return count;
}

// Sum of factorials of digits
int digit_factorial_sum(int n) {
    int facts[10];
    int i;
    int s;
    facts[0] = 1;
    for (i = 1; i < 10; i++) {
        facts[i] = facts[i - 1] * i;
    }
    s = 0;
    while (n > 0) {
        s = s + facts[n % 10];
        n = n / 10;
    }
    return s;
}

int main() {
    // EXPECT: digits(12345) = 5
    printf("digits(12345) = %d\n", count_digits(12345));

    // EXPECT: digits(0) = 1
    printf("digits(0) = %d\n", count_digits(0));

    // EXPECT: digit_sum(9999) = 36
    printf("digit_sum(9999) = %d\n", digit_sum(9999));

    // EXPECT: digit_product(234) = 24
    printf("digit_product(234) = %d\n", digit_product(234));

    // EXPECT: reverse(12345) = 54321
    printf("reverse(12345) = %d\n", reverse_number(12345));

    // EXPECT: reverse(-456) = -654
    printf("reverse(-456) = %d\n", reverse_number(-456));

    // EXPECT: palindrome(12321) = 1
    printf("palindrome(12321) = %d\n", is_palindrome(12321));

    // EXPECT: palindrome(12345) = 0
    printf("palindrome(12345) = %d\n", is_palindrome(12345));

    // EXPECT: get_digit(54321, 2) = 3
    printf("get_digit(54321, 2) = %d\n", get_digit(54321, 2));

    // EXPECT: armstrong(153) = 1
    printf("armstrong(153) = %d\n", is_armstrong(153));

    // EXPECT: armstrong(370) = 1
    printf("armstrong(370) = %d\n", is_armstrong(370));

    // EXPECT: armstrong(100) = 0
    printf("armstrong(100) = %d\n", is_armstrong(100));

    // EXPECT: digital_root(9875) = 2
    printf("digital_root(9875) = %d\n", digital_root(9875));

    // EXPECT: digital_root(493) = 7
    printf("digital_root(493) = %d\n", digital_root(493));

    // EXPECT: happy(7) = 1
    printf("happy(7) = %d\n", is_happy(7));

    // EXPECT: happy(4) = 0
    printf("happy(4) = %d\n", is_happy(4));

    // EXPECT: happy(19) = 1
    printf("happy(19) = %d\n", is_happy(19));

    // EXPECT: count_dsum(100, 5) = 6
    printf("count_dsum(100, 5) = %d\n", count_with_digit_sum(100, 5));

    // EXPECT: digit_fact_sum(145) = 145
    printf("digit_fact_sum(145) = %d\n", digit_factorial_sum(145));

    return 0;
}
