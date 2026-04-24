int printf(const char *fmt, ...);

/* Check if n is prime */
int is_prime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    int i;
    for (i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

/* Sieve of Eratosthenes up to limit (limit <= 200) */
/* Returns count of primes found */
int sieve(int limit, int out[]) {
    int composite[201];
    int i, j, count;
    for (i = 0; i <= limit; i++) composite[i] = 0;
    composite[0] = 1;
    composite[1] = 1;
    for (i = 2; i * i <= limit; i++) {
        if (!composite[i]) {
            for (j = i * i; j <= limit; j += i) {
                composite[j] = 1;
            }
        }
    }
    count = 0;
    for (i = 2; i <= limit; i++) {
        if (!composite[i]) {
            out[count++] = i;
        }
    }
    return count;
}

/* Count primes up to n (inclusive) */
int count_primes(int n) {
    int i, c = 0;
    for (i = 2; i <= n; i++) {
        if (is_prime(i)) c++;
    }
    return c;
}

/* Return the nth prime (1-indexed) */
int nth_prime(int n) {
    int count = 0, candidate = 1;
    while (count < n) {
        candidate++;
        if (is_prime(candidate)) count++;
    }
    return candidate;
}

/* Prime factorization: store factors in factors[], return count */
int prime_factors(int n, int factors[]) {
    int count = 0, d;
    for (d = 2; (long)d * d <= n; d++) {
        while (n % d == 0) {
            factors[count++] = d;
            n /= d;
        }
    }
    if (n > 1) factors[count++] = n;
    return count;
}

/* Check if n is a perfect square of a prime (prime^2) */
int is_prime_square(int n) {
    int i;
    for (i = 2; i * i <= n; i++) {
        if (i * i == n && is_prime(i)) return 1;
    }
    return 0;
}

/* Sum of primes up to n */
int sum_primes(int n) {
    int i, s = 0;
    for (i = 2; i <= n; i++) {
        if (is_prime(i)) s += i;
    }
    return s;
}

int main(void) {
    int primes[50];
    int factors[20];
    int count, i;

    /* is_prime tests */
    printf("%d\n", is_prime(1));   // EXPECT: 0
    printf("%d\n", is_prime(2));   // EXPECT: 1
    printf("%d\n", is_prime(3));   // EXPECT: 1
    printf("%d\n", is_prime(4));   // EXPECT: 0
    printf("%d\n", is_prime(17));  // EXPECT: 1
    printf("%d\n", is_prime(18));  // EXPECT: 0
    printf("%d\n", is_prime(97));  // EXPECT: 1

    /* Sieve up to 50 - print all primes */
    count = sieve(50, primes);
    printf("%d\n", count);  // EXPECT: 15
    /* primes up to 50: 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47 */
    for (i = 0; i < count; i++) {
        printf("%d\n", primes[i]);
    }
    // EXPECT: 2
    // EXPECT: 3
    // EXPECT: 5
    // EXPECT: 7
    // EXPECT: 11
    // EXPECT: 13
    // EXPECT: 17
    // EXPECT: 19
    // EXPECT: 23
    // EXPECT: 29
    // EXPECT: 31
    // EXPECT: 37
    // EXPECT: 41
    // EXPECT: 43
    // EXPECT: 47

    /* count_primes tests */
    printf("%d\n", count_primes(10));   // EXPECT: 4
    printf("%d\n", count_primes(20));   // EXPECT: 8
    printf("%d\n", count_primes(100));  // EXPECT: 25

    /* nth_prime tests */
    printf("%d\n", nth_prime(1));   // EXPECT: 2
    printf("%d\n", nth_prime(5));   // EXPECT: 11
    printf("%d\n", nth_prime(10));  // EXPECT: 29
    printf("%d\n", nth_prime(25));  // EXPECT: 97

    /* prime_factors tests */
    /* 12 = 2*2*3 */
    count = prime_factors(12, factors);
    printf("%d\n", count);  // EXPECT: 3
    for (i = 0; i < count; i++) printf("%d\n", factors[i]);
    // EXPECT: 2
    // EXPECT: 2
    // EXPECT: 3

    /* 60 = 2*2*3*5 */
    count = prime_factors(60, factors);
    printf("%d\n", count);  // EXPECT: 4
    for (i = 0; i < count; i++) printf("%d\n", factors[i]);
    // EXPECT: 2
    // EXPECT: 2
    // EXPECT: 3
    // EXPECT: 5

    /* 97 is prime */
    count = prime_factors(97, factors);
    printf("%d\n", count);  // EXPECT: 1
    printf("%d\n", factors[0]);  // EXPECT: 97

    /* sum_primes tests */
    printf("%d\n", sum_primes(10));  // EXPECT: 17
    /* 2+3+5+7 = 17 */
    printf("%d\n", sum_primes(20));  // EXPECT: 77
    /* 2+3+5+7+11+13+17+19 = 77 */

    return 0;
}
