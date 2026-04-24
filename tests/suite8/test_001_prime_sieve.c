int printf(const char *fmt, ...);

// Sieve of Eratosthenes to find primes up to a limit
// Uses a boolean array to mark composites

int sieve[200];  // 0 = prime, 1 = composite

void init_sieve(int limit) {
    int i;
    for (i = 0; i <= limit; i++) {
        sieve[i] = 0;
    }
    sieve[0] = 1;
    sieve[1] = 1;
}

void run_sieve(int limit) {
    int i;
    int j;
    for (i = 2; i * i <= limit; i++) {
        if (sieve[i] == 0) {
            for (j = i * i; j <= limit; j = j + i) {
                sieve[j] = 1;
            }
        }
    }
}

int count_primes(int limit) {
    int count;
    int i;
    count = 0;
    for (i = 2; i <= limit; i++) {
        if (sieve[i] == 0) {
            count = count + 1;
        }
    }
    return count;
}

int is_prime(int n) {
    if (n < 2) return 0;
    if (n >= 200) return 0;
    return sieve[n] == 0;
}

void print_primes_in_range(int lo, int hi) {
    int i;
    int first;
    first = 1;
    printf("Primes %d-%d:", lo, hi);
    for (i = lo; i <= hi; i++) {
        if (is_prime(i)) {
            printf(" %d", i);
        }
    }
    printf("\n");
}

int nth_prime(int n) {
    int count;
    int i;
    count = 0;
    for (i = 2; i < 200; i++) {
        if (sieve[i] == 0) {
            count = count + 1;
            if (count == n) return i;
        }
    }
    return -1;
}

int sum_primes(int limit) {
    int s;
    int i;
    s = 0;
    for (i = 2; i <= limit; i++) {
        if (sieve[i] == 0) {
            s = s + i;
        }
    }
    return s;
}

int prime_gap_max(int limit) {
    int prev;
    int maxgap;
    int i;
    prev = 2;
    maxgap = 0;
    for (i = 3; i <= limit; i++) {
        if (sieve[i] == 0) {
            if (i - prev > maxgap) {
                maxgap = i - prev;
            }
            prev = i;
        }
    }
    return maxgap;
}

int twin_prime_count(int limit) {
    int count;
    int i;
    count = 0;
    for (i = 2; i + 2 <= limit; i++) {
        if (sieve[i] == 0 && sieve[i + 2] == 0) {
            count = count + 1;
        }
    }
    return count;
}

int main() {
    init_sieve(199);
    run_sieve(199);

    // EXPECT: Count primes up to 100: 25
    printf("Count primes up to 100: %d\n", count_primes(100));

    // EXPECT: Count primes up to 50: 15
    printf("Count primes up to 50: %d\n", count_primes(50));

    // EXPECT: Primes 2-30: 2 3 5 7 11 13 17 19 23 29
    print_primes_in_range(2, 30);

    // EXPECT: Is 2 prime: 1
    printf("Is 2 prime: %d\n", is_prime(2));

    // EXPECT: Is 4 prime: 0
    printf("Is 4 prime: %d\n", is_prime(4));

    // EXPECT: Is 97 prime: 1
    printf("Is 97 prime: %d\n", is_prime(97));

    // EXPECT: 10th prime: 29
    printf("10th prime: %d\n", nth_prime(10));

    // EXPECT: 25th prime: 97
    printf("25th prime: %d\n", nth_prime(25));

    // EXPECT: Sum primes up to 20: 77
    printf("Sum primes up to 20: %d\n", sum_primes(20));

    // EXPECT: Max prime gap up to 100: 8
    printf("Max prime gap up to 100: %d\n", prime_gap_max(100));

    // EXPECT: Twin prime pairs up to 100: 8
    printf("Twin prime pairs up to 100: %d\n", twin_prime_count(100));

    return 0;
}
