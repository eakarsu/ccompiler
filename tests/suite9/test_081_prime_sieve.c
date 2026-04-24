int printf(const char *fmt, ...);

// Sieve of Eratosthenes - find all primes up to N

int sieve[200];

void init_sieve(int n) {
    int i;
    for (i = 0; i < n; i++) {
        sieve[i] = 1;
    }
    sieve[0] = 0;
    sieve[1] = 0;
}

void run_sieve(int n) {
    int i;
    int j;
    for (i = 2; i * i <= n; i++) {
        if (sieve[i]) {
            for (j = i * i; j <= n; j = j + i) {
                sieve[j] = 0;
            }
        }
    }
}

int count_primes(int n) {
    int count;
    int i;
    count = 0;
    for (i = 2; i <= n; i++) {
        if (sieve[i]) {
            count = count + 1;
        }
    }
    return count;
}

void print_primes_up_to(int n) {
    int i;
    int first;
    first = 1;
    for (i = 2; i <= n; i++) {
        if (sieve[i]) {
            if (first) {
                printf("%d", i);
                first = 0;
            } else {
                printf(" %d", i);
            }
        }
    }
    printf("\n");
}

int is_prime_via_sieve(int n) {
    if (n < 2) return 0;
    return sieve[n];
}

int nth_prime(int n) {
    int count;
    int i;
    count = 0;
    for (i = 2; i < 200; i++) {
        if (sieve[i]) {
            count = count + 1;
            if (count == n) return i;
        }
    }
    return -1;
}

int sum_primes(int limit) {
    int sum;
    int i;
    sum = 0;
    for (i = 2; i <= limit; i++) {
        if (sieve[i]) {
            sum = sum + i;
        }
    }
    return sum;
}

int prime_gap_max(int limit) {
    int max_gap;
    int prev;
    int i;
    max_gap = 0;
    prev = 2;
    for (i = 3; i <= limit; i++) {
        if (sieve[i]) {
            if (i - prev > max_gap) {
                max_gap = i - prev;
            }
            prev = i;
        }
    }
    return max_gap;
}

int twin_prime_count(int limit) {
    int count;
    int i;
    count = 0;
    for (i = 2; i <= limit - 2; i++) {
        if (sieve[i] && sieve[i + 2]) {
            count = count + 1;
        }
    }
    return count;
}

int main() {
    int c;
    int p10;
    int s;
    int gap;
    int twins;

    init_sieve(200);
    run_sieve(199);

    // EXPECT: Primes up to 30:
    printf("Primes up to 30:\n");
    // EXPECT: 2 3 5 7 11 13 17 19 23 29
    print_primes_up_to(30);

    c = count_primes(100);
    // EXPECT: Primes up to 100: 25
    printf("Primes up to 100: %d\n", c);

    // EXPECT: is_prime(7): 1
    printf("is_prime(7): %d\n", is_prime_via_sieve(7));
    // EXPECT: is_prime(10): 0
    printf("is_prime(10): %d\n", is_prime_via_sieve(10));
    // EXPECT: is_prime(1): 0
    printf("is_prime(1): %d\n", is_prime_via_sieve(1));
    // EXPECT: is_prime(2): 1
    printf("is_prime(2): %d\n", is_prime_via_sieve(2));

    p10 = nth_prime(10);
    // EXPECT: 10th prime: 29
    printf("10th prime: %d\n", p10);

    s = sum_primes(50);
    // EXPECT: Sum of primes up to 50: 328
    printf("Sum of primes up to 50: %d\n", s);

    gap = prime_gap_max(100);
    // EXPECT: Max prime gap up to 100: 8
    printf("Max prime gap up to 100: %d\n", gap);

    twins = twin_prime_count(100);
    // EXPECT: Twin prime pairs up to 100: 8
    printf("Twin prime pairs up to 100: %d\n", twins);

    return 0;
}
