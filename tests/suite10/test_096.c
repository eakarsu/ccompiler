int printf(const char *fmt, ...);

// Complex multi-file-like patterns: many functions calling each other

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int min2(int a, int b) {
    if (a < b) return a;
    return b;
}

int max2(int a, int b) {
    if (a > b) return a;
    return b;
}

int clamp(int x, int lo, int hi) {
    return max2(lo, min2(x, hi));
}

int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int gcd(int a, int b) {
    a = abs_val(a);
    b = abs_val(b);
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int lcm(int a, int b) {
    int g = gcd(a, b);
    if (g == 0) return 0;
    return abs_val(a) / g * abs_val(b);
}

int power(int base, int exp) {
    int result = 1;
    while (exp > 0) {
        result = result * base;
        exp = exp - 1;
    }
    return result;
}

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    int a = 0;
    int b = 1;
    int i = 2;
    while (i <= n) {
        int c = a + b;
        a = b;
        b = c;
        i = i + 1;
    }
    return b;
}

int is_prime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    int i = 3;
    while (i * i <= n) {
        if (n % i == 0) return 0;
        i = i + 2;
    }
    return 1;
}

int count_primes(int limit) {
    int cnt = 0;
    int i = 2;
    while (i <= limit) {
        if (is_prime(i)) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

int sum_of_digits(int n) {
    if (n < 0) n = -n;
    int s = 0;
    while (n > 0) {
        s = s + n % 10;
        n = n / 10;
    }
    return s;
}

int reverse_number(int n) {
    int neg = 0;
    if (n < 0) { neg = 1; n = -n; }
    int r = 0;
    while (n > 0) {
        r = r * 10 + n % 10;
        n = n / 10;
    }
    if (neg) r = -r;
    return r;
}

int is_palindrome_num(int n) {
    if (n < 0) return 0;
    return n == reverse_number(n);
}

int collatz_steps(int n) {
    int steps = 0;
    while (n != 1) {
        if (n % 2 == 0) n = n / 2;
        else n = 3 * n + 1;
        steps = steps + 1;
    }
    return steps;
}

int complex_computation(int x) {
    int a = clamp(x, 0, 100);
    int b = sign(a - 50);
    int c = abs_val(b * 10 + a);
    int d = gcd(c, 12);
    int e = sum_of_digits(c);
    int f = is_prime(e) ? e * 2 : e + 1;
    return f + d;
}

int main() {
    printf("clamp -5: %d\n", clamp(-5, 0, 100));
    // EXPECT: clamp -5: 0
    printf("clamp 50: %d\n", clamp(50, 0, 100));
    // EXPECT: clamp 50: 50
    printf("clamp 200: %d\n", clamp(200, 0, 100));
    // EXPECT: clamp 200: 100

    printf("gcd 48 18: %d\n", gcd(48, 18));
    // EXPECT: gcd 48 18: 6
    printf("lcm 4 6: %d\n", lcm(4, 6));
    // EXPECT: lcm 4 6: 12

    printf("pow 2 10: %d\n", power(2, 10));
    // EXPECT: pow 2 10: 1024
    printf("fact 10: %d\n", factorial(10));
    // EXPECT: fact 10: 3628800

    printf("fib 10: %d\n", fibonacci(10));
    // EXPECT: fib 10: 55
    printf("fib 20: %d\n", fibonacci(20));
    // EXPECT: fib 20: 6765

    printf("primes to 100: %d\n", count_primes(100));
    // EXPECT: primes to 100: 25

    printf("digits 12345: %d\n", sum_of_digits(12345));
    // EXPECT: digits 12345: 15

    printf("reverse 12345: %d\n", reverse_number(12345));
    // EXPECT: reverse 12345: 54321

    printf("palindrome 12321: %d\n", is_palindrome_num(12321));
    // EXPECT: palindrome 12321: 1
    printf("palindrome 12345: %d\n", is_palindrome_num(12345));
    // EXPECT: palindrome 12345: 0

    printf("collatz 27: %d\n", collatz_steps(27));
    // EXPECT: collatz 27: 111

    printf("complex 25: %d\n", complex_computation(25));
    // EXPECT: complex 25: 10
    printf("complex 75: %d\n", complex_computation(75));
    // EXPECT: complex 75: 27

    return 0;
}
