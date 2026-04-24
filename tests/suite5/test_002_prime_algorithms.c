int printf(const char *fmt, ...);

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

int count_primes_sieve(int limit) {
    int sieve[200];
    int i = 0;
    while (i <= limit) {
        sieve[i] = 1;
        i = i + 1;
    }
    sieve[0] = 0;
    sieve[1] = 0;
    i = 2;
    while (i * i <= limit) {
        if (sieve[i]) {
            int j = i * i;
            while (j <= limit) {
                sieve[j] = 0;
                j = j + i;
            }
        }
        i = i + 1;
    }
    int count = 0;
    i = 0;
    while (i <= limit) {
        if (sieve[i]) count = count + 1;
        i = i + 1;
    }
    return count;
}

int smallest_prime_factor(int n) {
    if (n < 2) return 0;
    int i = 2;
    while (i * i <= n) {
        if (n % i == 0) return i;
        i = i + 1;
    }
    return n;
}

int count_prime_factors(int n) {
    int count = 0;
    int d = 2;
    while (d * d <= n) {
        while (n % d == 0) {
            count = count + 1;
            n = n / d;
        }
        d = d + 1;
    }
    if (n > 1) count = count + 1;
    return count;
}

int largest_prime_factor(int n) {
    int largest = 1;
    int d = 2;
    while (d * d <= n) {
        while (n % d == 0) {
            largest = d;
            n = n / d;
        }
        d = d + 1;
    }
    if (n > 1) largest = n;
    return largest;
}

int count_twin_primes(int limit) {
    int count = 0;
    int i = 2;
    while (i <= limit - 2) {
        if (is_prime(i) && is_prime(i + 2)) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int nth_prime(int n) {
    int count = 0;
    int num = 2;
    while (1) {
        if (is_prime(num)) {
            count = count + 1;
            if (count == n) return num;
        }
        num = num + 1;
    }
    return -1;
}

int main(void) {
    // EXPECT: is_prime(2) = 1
    printf("is_prime(2) = %d\n", is_prime(2));
    // EXPECT: is_prime(7) = 1
    printf("is_prime(7) = %d\n", is_prime(7));
    // EXPECT: is_prime(15) = 0
    printf("is_prime(15) = %d\n", is_prime(15));
    // EXPECT: is_prime(97) = 1
    printf("is_prime(97) = %d\n", is_prime(97));

    // EXPECT: primes_to_50 = 15
    printf("primes_to_50 = %d\n", count_primes_sieve(50));
    // EXPECT: primes_to_100 = 25
    printf("primes_to_100 = %d\n", count_primes_sieve(100));

    // EXPECT: spf(12) = 2
    printf("spf(12) = %d\n", smallest_prime_factor(12));
    // EXPECT: spf(35) = 5
    printf("spf(35) = %d\n", smallest_prime_factor(35));
    // EXPECT: spf(97) = 97
    printf("spf(97) = %d\n", smallest_prime_factor(97));

    // EXPECT: cpf(12) = 3
    printf("cpf(12) = %d\n", count_prime_factors(12));
    // EXPECT: cpf(60) = 4
    printf("cpf(60) = %d\n", count_prime_factors(60));
    // EXPECT: cpf(100) = 4
    printf("cpf(100) = %d\n", count_prime_factors(100));

    // EXPECT: lpf(60) = 5
    printf("lpf(60) = %d\n", largest_prime_factor(60));
    // EXPECT: lpf(84) = 7
    printf("lpf(84) = %d\n", largest_prime_factor(84));

    // EXPECT: twin_to_50 = 6
    printf("twin_to_50 = %d\n", count_twin_primes(50));
    // EXPECT: twin_to_100 = 8
    printf("twin_to_100 = %d\n", count_twin_primes(100));

    // EXPECT: prime_10 = 29
    printf("prime_10 = %d\n", nth_prime(10));
    // EXPECT: prime_25 = 97
    printf("prime_25 = %d\n", nth_prime(25));

    return 0;
}
