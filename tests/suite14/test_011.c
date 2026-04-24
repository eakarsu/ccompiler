int printf(const char *fmt, ...);

// Sieve of Eratosthenes - find all primes up to N

void sieve(int *is_prime, int n) {
    int i;
    int j;
    for (i = 0; i <= n; i++) {
        is_prime[i] = 1;
    }
    is_prime[0] = 0;
    is_prime[1] = 0;
    for (i = 2; i * i <= n; i++) {
        if (is_prime[i]) {
            for (j = i * i; j <= n; j = j + i) {
                is_prime[j] = 0;
            }
        }
    }
}

int count_primes(int *is_prime, int n) {
    int count = 0;
    int i;
    for (i = 2; i <= n; i++) {
        if (is_prime[i]) {
            count = count + 1;
        }
    }
    return count;
}

int nth_prime(int *is_prime, int n, int nth) {
    int count = 0;
    int i;
    for (i = 2; i <= n; i++) {
        if (is_prime[i]) {
            count = count + 1;
            if (count == nth) {
                return i;
            }
        }
    }
    return -1;
}

int sum_primes(int *is_prime, int n) {
    int sum = 0;
    int i;
    for (i = 2; i <= n; i++) {
        if (is_prime[i]) {
            sum = sum + i;
        }
    }
    return sum;
}

int is_twin_prime(int *is_prime, int n, int p) {
    if (p + 2 <= n && is_prime[p] && is_prime[p + 2]) {
        return 1;
    }
    return 0;
}

int count_twin_primes(int *is_prime, int n) {
    int count = 0;
    int i;
    for (i = 2; i + 2 <= n; i++) {
        if (is_prime[i] && is_prime[i + 2]) {
            count = count + 1;
        }
    }
    return count;
}

int largest_prime_gap(int *is_prime, int n) {
    int max_gap = 0;
    int prev = 2;
    int i;
    for (i = 3; i <= n; i++) {
        if (is_prime[i]) {
            int gap = i - prev;
            if (gap > max_gap) {
                max_gap = gap;
            }
            prev = i;
        }
    }
    return max_gap;
}

int goldbach_check(int *is_prime, int even) {
    int i;
    for (i = 2; i <= even / 2; i++) {
        if (is_prime[i] && is_prime[even - i]) {
            return 1;
        }
    }
    return 0;
}

int main(void) {
    int is_prime[201];
    sieve(is_prime, 200);

    int c = count_primes(is_prime, 200);
    printf("Primes up to 200: %d\n", c);
    // EXPECT: Primes up to 200: 46

    int p1 = nth_prime(is_prime, 200, 1);
    printf("1st prime: %d\n", p1);
    // EXPECT: 1st prime: 2

    int p10 = nth_prime(is_prime, 200, 10);
    printf("10th prime: %d\n", p10);
    // EXPECT: 10th prime: 29

    int p25 = nth_prime(is_prime, 200, 25);
    printf("25th prime: %d\n", p25);
    // EXPECT: 25th prime: 97

    int s = sum_primes(is_prime, 100);
    printf("Sum of primes up to 100: %d\n", s);
    // EXPECT: Sum of primes up to 100: 1060

    int tw = count_twin_primes(is_prime, 200);
    printf("Twin prime pairs up to 200: %d\n", tw);
    // EXPECT: Twin prime pairs up to 200: 15

    int gap = largest_prime_gap(is_prime, 200);
    printf("Largest prime gap up to 200: %d\n", gap);
    // EXPECT: Largest prime gap up to 200: 14

    int g1 = goldbach_check(is_prime, 28);
    printf("Goldbach check 28: %d\n", g1);
    // EXPECT: Goldbach check 28: 1

    int g2 = goldbach_check(is_prime, 100);
    printf("Goldbach check 100: %d\n", g2);
    // EXPECT: Goldbach check 100: 1

    int small_primes[11];
    sieve(small_primes, 10);
    int sc = count_primes(small_primes, 10);
    printf("Primes up to 10: %d\n", sc);
    // EXPECT: Primes up to 10: 4

    int ss = sum_primes(small_primes, 10);
    printf("Sum of primes up to 10: %d\n", ss);
    // EXPECT: Sum of primes up to 10: 17

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
