int printf(const char *fmt, ...);
// EXPECT: 12 = 2^2 * 3\n100 = 2^2 * 5^2\n97 = 97\n360 = 2^3 * 3^2 * 5\n1024 = 2^10\nDivisors of 12: 6\nDivisors of 100: 9\nDivisors of 97: 2\nSum divisors 12: 28\nSum divisors 28: 56\nGCD(12,8): 4\nGCD(100,75): 25\nLCM(12,8): 24\nLCM(15,20): 60
// Test: Prime factorization

int factors[64];
int powers[64];
int num_factors;

void factorize(int n) {
    num_factors = 0;
    int d = 2;
    while (d * d <= n) {
        if (n % d == 0) {
            factors[num_factors] = d;
            int count = 0;
            while (n % d == 0) {
                n = n / d;
                count++;
            }
            powers[num_factors] = count;
            num_factors++;
        }
        d++;
    }
    if (n > 1) {
        factors[num_factors] = n;
        powers[num_factors] = 1;
        num_factors++;
    }
}

void print_factorization(int n) {
    factorize(n);
    printf("%d = ", n);
    int i;
    for (i = 0; i < num_factors; i++) {
        if (i > 0) printf(" * ");
        if (powers[i] > 1)
            printf("%d^%d", factors[i], powers[i]);
        else
            printf("%d", factors[i]);
    }
    printf("\n");
}

int count_divisors(int n) {
    factorize(n);
    int count = 1;
    int i;
    for (i = 0; i < num_factors; i++) {
        count = count * (powers[i] + 1);
    }
    return count;
}

int sum_divisors(int n) {
    int sum = 0;
    int i;
    for (i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            sum = sum + i;
            if (i != n / i) sum = sum + n / i;
        }
    }
    return sum;
}

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int lcm(int a, int b) {
    return a / gcd(a, b) * b;
}

int main(void) {
    print_factorization(12);
    print_factorization(100);
    print_factorization(97);
    print_factorization(360);
    print_factorization(1024);

    printf("Divisors of 12: %d\n", count_divisors(12));
    printf("Divisors of 100: %d\n", count_divisors(100));
    printf("Divisors of 97: %d\n", count_divisors(97));

    printf("Sum divisors 12: %d\n", sum_divisors(12));
    printf("Sum divisors 28: %d\n", sum_divisors(28));

    printf("GCD(12,8): %d\n", gcd(12, 8));
    printf("GCD(100,75): %d\n", gcd(100, 75));
    printf("LCM(12,8): %d\n", lcm(12, 8));
    printf("LCM(15,20): %d\n", lcm(15, 20));

    return 0;
}
