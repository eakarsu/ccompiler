int printf(const char *fmt, ...);
// EXPECT: Primes up to 50: 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47\nCount: 15\nPrimes up to 100: 25\nIs 97 prime: 1\nIs 100 prime: 0\nIs 2 prime: 1\nIs 1 prime: 0\n1st prime: 2\n10th prime: 29\n25th prime: 97\nSum of primes up to 30: 129
// Test: Sieve of Eratosthenes

int is_prime[256];
int primes[64];
int num_primes;

void sieve(int n) {
    int i, j;
    for (i = 0; i <= n; i++) is_prime[i] = 1;
    is_prime[0] = 0;
    is_prime[1] = 0;

    for (i = 2; i * i <= n; i++) {
        if (is_prime[i]) {
            for (j = i * i; j <= n; j = j + i) {
                is_prime[j] = 0;
            }
        }
    }

    num_primes = 0;
    for (i = 2; i <= n; i++) {
        if (is_prime[i]) {
            primes[num_primes] = i;
            num_primes++;
        }
    }
}

int count_primes(int n) {
    sieve(n);
    return num_primes;
}

int is_prime_check(int n) {
    if (n < 2) return 0;
    if (n < 4) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    int i = 5;
    while (i * i <= n) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
        i = i + 6;
    }
    return 1;
}

int nth_prime(int n) {
    int count = 0;
    int num = 2;
    while (count < n) {
        if (is_prime_check(num)) count++;
        if (count < n) num++;
    }
    return num;
}

int main(void) {
    sieve(50);
    printf("Primes up to 50:");
    int i;
    for (i = 0; i < num_primes; i++) {
        printf(" %d", primes[i]);
    }
    printf("\n");
    printf("Count: %d\n", num_primes);

    printf("Primes up to 100: %d\n", count_primes(100));

    printf("Is 97 prime: %d\n", is_prime_check(97));
    printf("Is 100 prime: %d\n", is_prime_check(100));
    printf("Is 2 prime: %d\n", is_prime_check(2));
    printf("Is 1 prime: %d\n", is_prime_check(1));

    printf("1st prime: %d\n", nth_prime(1));
    printf("10th prime: %d\n", nth_prime(10));
    printf("25th prime: %d\n", nth_prime(25));

    sieve(30);
    int sum = 0;
    for (i = 0; i < num_primes; i++) sum = sum + primes[i];
    printf("Sum of primes up to 30: %d\n", sum);

    return 0;
}
