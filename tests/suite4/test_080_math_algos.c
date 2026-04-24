int printf(const char *fmt, ...);

int gcd(int a, int b) {
    while (b != 0) {
        int temp;
        temp = b;
        b = a % b;
        a = temp;
    }
    if (a < 0) {
        return -a;
    }
    return a;
}

int lcm(int a, int b) {
    int g;
    g = gcd(a, b);
    if (g == 0) {
        return 0;
    }
    int result;
    result = a / g * b;
    if (result < 0) {
        return -result;
    }
    return result;
}

int is_prime(int n) {
    int i;
    if (n <= 1) {
        return 0;
    }
    if (n <= 3) {
        return 1;
    }
    if (n % 2 == 0 || n % 3 == 0) {
        return 0;
    }
    i = 5;
    while (i * i <= n) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
        i = i + 6;
    }
    return 1;
}

int power(int base, int exp) {
    int result;
    result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = result * base;
        }
        base = base * base;
        exp = exp / 2;
    }
    return result;
}

int abs_val(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

int fibonacci(int n) {
    int a;
    int b;
    int temp;
    int i;
    if (n <= 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    a = 0;
    b = 1;
    for (i = 2; i <= n; i = i + 1) {
        temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

int factorial(int n) {
    int result;
    int i;
    result = 1;
    for (i = 2; i <= n; i = i + 1) {
        result = result * i;
    }
    return result;
}

int count_primes_up_to(int n) {
    int count;
    int i;
    count = 0;
    for (i = 2; i <= n; i = i + 1) {
        if (is_prime(i)) {
            count = count + 1;
        }
    }
    return count;
}

int sum_digits(int n) {
    int sum;
    if (n < 0) {
        n = -n;
    }
    sum = 0;
    while (n > 0) {
        sum = sum + n % 10;
        n = n / 10;
    }
    return sum;
}

int int_sqrt(int n) {
    int x;
    if (n <= 1) {
        return n;
    }
    x = n / 2;
    while (x * x > n) {
        x = (x + n / x) / 2;
    }
    return x;
}

int main(void) {
    // EXPECT: gcd 12 8: 4
    printf("gcd 12 8: %d\n", gcd(12, 8));
    // EXPECT: gcd 17 13: 1
    printf("gcd 17 13: %d\n", gcd(17, 13));
    // EXPECT: gcd 100 75: 25
    printf("gcd 100 75: %d\n", gcd(100, 75));
    // EXPECT: gcd 0 5: 5
    printf("gcd 0 5: %d\n", gcd(0, 5));

    // EXPECT: lcm 4 6: 12
    printf("lcm 4 6: %d\n", lcm(4, 6));
    // EXPECT: lcm 3 7: 21
    printf("lcm 3 7: %d\n", lcm(3, 7));
    // EXPECT: lcm 12 18: 36
    printf("lcm 12 18: %d\n", lcm(12, 18));

    // EXPECT: prime 2: 1
    printf("prime 2: %d\n", is_prime(2));
    // EXPECT: prime 7: 1
    printf("prime 7: %d\n", is_prime(7));
    // EXPECT: prime 11: 1
    printf("prime 11: %d\n", is_prime(11));
    // EXPECT: prime 1: 0
    printf("prime 1: %d\n", is_prime(1));
    // EXPECT: prime 4: 0
    printf("prime 4: %d\n", is_prime(4));
    // EXPECT: prime 9: 0
    printf("prime 9: %d\n", is_prime(9));
    // EXPECT: prime 97: 1
    printf("prime 97: %d\n", is_prime(97));

    // EXPECT: pow 2 10: 1024
    printf("pow 2 10: %d\n", power(2, 10));
    // EXPECT: pow 3 5: 243
    printf("pow 3 5: %d\n", power(3, 5));
    // EXPECT: pow 5 0: 1
    printf("pow 5 0: %d\n", power(5, 0));
    // EXPECT: pow 7 1: 7
    printf("pow 7 1: %d\n", power(7, 1));

    // EXPECT: abs -5: 5
    printf("abs -5: %d\n", abs_val(-5));
    // EXPECT: abs 3: 3
    printf("abs 3: %d\n", abs_val(3));
    // EXPECT: abs 0: 0
    printf("abs 0: %d\n", abs_val(0));

    // EXPECT: fib 0: 0
    printf("fib 0: %d\n", fibonacci(0));
    // EXPECT: fib 1: 1
    printf("fib 1: %d\n", fibonacci(1));
    // EXPECT: fib 10: 55
    printf("fib 10: %d\n", fibonacci(10));
    // EXPECT: fib 15: 610
    printf("fib 15: %d\n", fibonacci(15));

    // EXPECT: fact 5: 120
    printf("fact 5: %d\n", factorial(5));
    // EXPECT: fact 10: 3628800
    printf("fact 10: %d\n", factorial(10));

    // EXPECT: primes to 20: 8
    printf("primes to 20: %d\n", count_primes_up_to(20));
    // EXPECT: primes to 50: 15
    printf("primes to 50: %d\n", count_primes_up_to(50));

    // EXPECT: digits 123: 6
    printf("digits 123: %d\n", sum_digits(123));
    // EXPECT: digits 9999: 36
    printf("digits 9999: %d\n", sum_digits(9999));

    // EXPECT: sqrt 100: 10
    printf("sqrt 100: %d\n", int_sqrt(100));
    // EXPECT: sqrt 81: 9
    printf("sqrt 81: %d\n", int_sqrt(81));

    // EXPECT: done
    printf("done\n");

    return 0;
}
