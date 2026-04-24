int printf(const char *fmt, ...);

// Prime factorization of integers

int factors[64];
int exponents[64];
int num_factors;

void factorize(int n) {
    int d;
    int count;
    num_factors = 0;
    d = 2;
    while (d * d <= n) {
        count = 0;
        while (n % d == 0) {
            count = count + 1;
            n = n / d;
        }
        if (count > 0) {
            factors[num_factors] = d;
            exponents[num_factors] = count;
            num_factors = num_factors + 1;
        }
        d = d + 1;
    }
    if (n > 1) {
        factors[num_factors] = n;
        exponents[num_factors] = 1;
        num_factors = num_factors + 1;
    }
}

void print_factorization(int n) {
    int i;
    int first;
    factorize(n);
    printf("%d = ", n);
    first = 1;
    for (i = 0; i < num_factors; i++) {
        if (!first) {
            printf(" * ");
        }
        first = 0;
        if (exponents[i] == 1) {
            printf("%d", factors[i]);
        } else {
            printf("%d^%d", factors[i], exponents[i]);
        }
    }
    printf("\n");
}

int count_divisors(int n) {
    int result;
    int i;
    factorize(n);
    result = 1;
    for (i = 0; i < num_factors; i++) {
        result = result * (exponents[i] + 1);
    }
    return result;
}

int sum_of_divisors(int n) {
    int result;
    int i;
    int term;
    int j;
    int power;
    factorize(n);
    result = 1;
    for (i = 0; i < num_factors; i++) {
        term = 0;
        power = 1;
        for (j = 0; j <= exponents[i]; j++) {
            term = term + power;
            power = power * factors[i];
        }
        result = result * term;
    }
    return result;
}

int gcd(int a, int b) {
    int temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;
}

int is_prime_check(int n) {
    int i;
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (i = 3; i * i <= n; i = i + 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int largest_prime_factor(int n) {
    int largest;
    int i;
    factorize(n);
    largest = 0;
    for (i = 0; i < num_factors; i++) {
        if (factors[i] > largest) {
            largest = factors[i];
        }
    }
    return largest;
}

int is_square_free(int n) {
    int i;
    factorize(n);
    for (i = 0; i < num_factors; i++) {
        if (exponents[i] > 1) return 0;
    }
    return 1;
}

int main() {
    int d;
    int s;
    int g;
    int l;
    int lpf;
    int sf;

    // EXPECT: 12 = 2^2 * 3
    print_factorization(12);
    // EXPECT: 360 = 2^3 * 3^2 * 5
    print_factorization(360);
    // EXPECT: 97 = 97
    print_factorization(97);
    // EXPECT: 1024 = 2^10
    print_factorization(1024);
    // EXPECT: 100 = 2^2 * 5^2
    print_factorization(100);

    d = count_divisors(360);
    // EXPECT: Divisors of 360: 24
    printf("Divisors of 360: %d\n", d);

    s = sum_of_divisors(12);
    // EXPECT: Sum of divisors of 12: 28
    printf("Sum of divisors of 12: %d\n", s);

    g = gcd(48, 18);
    // EXPECT: GCD(48,18): 6
    printf("GCD(48,18): %d\n", g);

    l = lcm(12, 18);
    // EXPECT: LCM(12,18): 36
    printf("LCM(12,18): %d\n", l);

    // EXPECT: is_prime(97): 1
    printf("is_prime(97): %d\n", is_prime_check(97));
    // EXPECT: is_prime(100): 0
    printf("is_prime(100): %d\n", is_prime_check(100));

    lpf = largest_prime_factor(360);
    // EXPECT: Largest prime factor of 360: 5
    printf("Largest prime factor of 360: %d\n", lpf);

    sf = is_square_free(30);
    // EXPECT: is_square_free(30): 1
    printf("is_square_free(30): %d\n", sf);
    sf = is_square_free(12);
    // EXPECT: is_square_free(12): 0
    printf("is_square_free(12): %d\n", sf);

    return 0;
}
