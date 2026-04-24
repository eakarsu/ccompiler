int printf(const char *fmt, ...);

// Euler's totient function phi(n)

int gcd(int a, int b) {
    int temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int euler_totient_naive(int n) {
    int count;
    int i;
    if (n == 1) return 1;
    count = 0;
    for (i = 1; i < n; i++) {
        if (gcd(i, n) == 1) {
            count = count + 1;
        }
    }
    return count;
}

int euler_totient(int n) {
    int result;
    int p;
    result = n;
    p = 2;
    while (p * p <= n) {
        if (n % p == 0) {
            while (n % p == 0) {
                n = n / p;
            }
            result = result - result / p;
        }
        p = p + 1;
    }
    if (n > 1) {
        result = result - result / n;
    }
    return result;
}

void totient_sieve(int limit, int *phi) {
    int i;
    int j;
    for (i = 0; i <= limit; i++) {
        phi[i] = i;
    }
    for (i = 2; i <= limit; i++) {
        if (phi[i] == i) {
            for (j = i; j <= limit; j = j + i) {
                phi[j] = phi[j] - phi[j] / i;
            }
        }
    }
}

int sum_totient(int n, int *phi) {
    int sum;
    int i;
    sum = 0;
    for (i = 1; i <= n; i++) {
        sum = sum + phi[i];
    }
    return sum;
}

int is_prime_from_totient(int n, int *phi) {
    if (n < 2) return 0;
    return (phi[n] == n - 1) ? 1 : 0;
}

int count_coprimes(int a, int b) {
    int count;
    int i;
    count = 0;
    for (i = 1; i <= b; i++) {
        if (gcd(a, i) == 1) {
            count = count + 1;
        }
    }
    return count;
}

int verify_totient_sum_property(int n) {
    int sum;
    int d;
    sum = 0;
    for (d = 1; d <= n; d++) {
        if (n % d == 0) {
            sum = sum + euler_totient(d);
        }
    }
    return sum;
}

int totient_product_check(int n) {
    int product_sum;
    int i;
    product_sum = 0;
    for (i = 1; i <= n; i++) {
        if (gcd(i, n) == 1) {
            product_sum = product_sum + i;
        }
    }
    return product_sum;
}

int main() {
    int phi_arr[101];
    int i;
    int s;
    int v;
    int cp;
    int ps;

    // EXPECT: phi(1) = 1
    printf("phi(1) = %d\n", euler_totient(1));
    // EXPECT: phi(2) = 1
    printf("phi(2) = %d\n", euler_totient(2));
    // EXPECT: phi(6) = 2
    printf("phi(6) = %d\n", euler_totient(6));
    // EXPECT: phi(10) = 4
    printf("phi(10) = %d\n", euler_totient(10));
    // EXPECT: phi(12) = 4
    printf("phi(12) = %d\n", euler_totient(12));
    // EXPECT: phi(36) = 12
    printf("phi(36) = %d\n", euler_totient(36));
    // EXPECT: phi(97) = 96
    printf("phi(97) = %d\n", euler_totient(97));

    // EXPECT: Naive phi(12) = 4
    printf("Naive phi(12) = %d\n", euler_totient_naive(12));
    // EXPECT: Naive phi(36) = 12
    printf("Naive phi(36) = %d\n", euler_totient_naive(36));

    totient_sieve(100, phi_arr);
    // EXPECT: Sieve: phi(10) = 4
    printf("Sieve: phi(10) = %d\n", phi_arr[10]);
    // EXPECT: Sieve: phi(12) = 4
    printf("Sieve: phi(12) = %d\n", phi_arr[12]);
    // EXPECT: Sieve: phi(100) = 40
    printf("Sieve: phi(100) = %d\n", phi_arr[100]);

    s = sum_totient(10, phi_arr);
    // EXPECT: Sum phi(1..10) = 32
    printf("Sum phi(1..10) = %d\n", s);

    // EXPECT: is_prime(7) via totient: 1
    printf("is_prime(7) via totient: %d\n", is_prime_from_totient(7, phi_arr));
    // EXPECT: is_prime(9) via totient: 0
    printf("is_prime(9) via totient: %d\n", is_prime_from_totient(9, phi_arr));

    v = verify_totient_sum_property(12);
    // EXPECT: Sum of phi(d) for d|12 = 12
    printf("Sum of phi(d) for d|12 = %d\n", v);

    cp = count_coprimes(12, 12);
    // EXPECT: Coprimes to 12 up to 12: 4
    printf("Coprimes to 12 up to 12: %d\n", cp);

    ps = totient_product_check(10);
    // EXPECT: Sum of coprimes to 10: 20
    printf("Sum of coprimes to 10: %d\n", ps);

    return 0;
}
