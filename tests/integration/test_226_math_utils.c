int printf(const char *fmt, ...);
// EXPECT: abs(-5)=5 abs(5)=5 abs(0)=0\nsign(-7)=-1 sign(3)=1 sign(0)=0\ngcd(12,8)=4\ngcd(100,75)=25\ngcd(17,13)=1\nlcm(4,6)=12\nlcm(12,8)=24\nprimes: 2 3 5 7 11 13 17 19 23 29 \nnth primes: 2 3 5 7 11 13 17 19 23 29 \nisqrt(100)=10\nisqrt(50)=7\nisqrt(1)=1\ndivisors(12)=6\ndivisors(28)=6\nsum_div(12)=28\nsum_div(28)=56
// Test 226: Helper function library - math utilities

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int t = b;
        b = a - (a / b) * b;
        a = t;
    }
    return a;
}

int lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    int g = gcd(a, b);
    return abs_val(a / g * b);
}

int is_prime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if ((n / 2) * 2 == n) return 0;
    int i = 3;
    while (i * i <= n) {
        if ((n / i) * i == n) return 0;
        i = i + 2;
    }
    return 1;
}

int nth_prime(int n) {
    int count = 0;
    int candidate = 2;
    while (count < n) {
        if (is_prime(candidate)) count = count + 1;
        if (count < n) candidate = candidate + 1;
    }
    return candidate;
}

int isqrt(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    int x = n;
    int y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

int count_divisors(int n) {
    if (n <= 0) return 0;
    int count = 0;
    int i = 1;
    while (i * i <= n) {
        if ((n / i) * i == n) {
            count = count + 1;
            if (i != n / i) count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int sum_divisors(int n) {
    int sum = 0;
    int i = 1;
    while (i <= n) {
        if ((n / i) * i == n) sum = sum + i;
        i = i + 1;
    }
    return sum;
}

int main(void) {
    printf("abs(-5)=%d abs(5)=%d abs(0)=%d\n", abs_val(-5), abs_val(5), abs_val(0));
    printf("sign(-7)=%d sign(3)=%d sign(0)=%d\n", sign(-7), sign(3), sign(0));

    printf("gcd(12,8)=%d\n", gcd(12, 8));
    printf("gcd(100,75)=%d\n", gcd(100, 75));
    printf("gcd(17,13)=%d\n", gcd(17, 13));

    printf("lcm(4,6)=%d\n", lcm(4, 6));
    printf("lcm(12,8)=%d\n", lcm(12, 8));

    printf("primes: ");
    int i = 2;
    while (i <= 30) {
        if (is_prime(i)) printf("%d ", i);
        i = i + 1;
    }
    printf("\n");

    printf("nth primes: ");
    i = 1;
    while (i <= 10) {
        printf("%d ", nth_prime(i));
        i = i + 1;
    }
    printf("\n");

    printf("isqrt(100)=%d\n", isqrt(100));
    printf("isqrt(50)=%d\n", isqrt(50));
    printf("isqrt(1)=%d\n", isqrt(1));

    printf("divisors(12)=%d\n", count_divisors(12));
    printf("divisors(28)=%d\n", count_divisors(28));
    printf("sum_div(12)=%d\n", sum_divisors(12));
    printf("sum_div(28)=%d\n", sum_divisors(28));

    return 0;
}
