int printf(const char *fmt, ...);
// EXPECT: primes: 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 \npi(100) = 25\nsum primes <= 50 = 328\nnext primes: 2 3 5 7 11 13 17 19 23 29 \nfactor count: 1 1 2 1 2 1 3 2 2 1 3 1 2 2 4 1 3 1 3 \ntwin primes: (3,5) (5,7) (11,13) (17,19) (29,31) (41,43) \ngoldbach: 4=2+2 6=3+3 8=3+5 10=3+7 12=5+7 14=3+11 16=3+13 18=5+13 20=3+17 
// Test: prime number checking and generation

int is_prime(int n) {
    int i;
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int next_prime(int n) {
    int candidate;
    candidate = n + 1;
    while (!is_prime(candidate)) {
        candidate++;
    }
    return candidate;
}

int count_prime_factors(int n) {
    int count, d;
    count = 0;
    d = 2;
    while (n > 1) {
        while (n % d == 0) {
            count++;
            n = n / d;
        }
        d++;
    }
    return count;
}

int main(void) {
    int i;
    int count;
    int sum;

    /* Primes up to 50 */
    printf("primes: ");
    for (i = 2; i <= 50; i++) {
        if (is_prime(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");

    /* Count primes up to 100 */
    count = 0;
    for (i = 2; i <= 100; i++) {
        if (is_prime(i)) count++;
    }
    printf("pi(100) = %d\n", count);

    /* Sum of primes up to 50 */
    sum = 0;
    for (i = 2; i <= 50; i++) {
        if (is_prime(i)) sum += i;
    }
    printf("sum primes <= 50 = %d\n", sum);

    /* Next prime */
    printf("next primes: ");
    i = 1;
    {
        int j;
        for (j = 0; j < 10; j++) {
            i = next_prime(i);
            printf("%d ", i);
        }
    }
    printf("\n");

    /* Prime factorization count */
    printf("factor count: ");
    for (i = 2; i <= 20; i++) {
        printf("%d ", count_prime_factors(i));
    }
    printf("\n");

    /* Twin primes */
    printf("twin primes: ");
    for (i = 2; i <= 50; i++) {
        if (is_prime(i) && is_prime(i + 2)) {
            printf("(%d,%d) ", i, i + 2);
        }
    }
    printf("\n");

    /* Goldbach: even number as sum of two primes */
    printf("goldbach: ");
    for (i = 4; i <= 20; i += 2) {
        int j;
        for (j = 2; j <= i / 2; j++) {
            if (is_prime(j) && is_prime(i - j)) {
                printf("%d=%d+%d ", i, j, i - j);
                break;
            }
        }
    }
    printf("\n");

    return 0;
}
