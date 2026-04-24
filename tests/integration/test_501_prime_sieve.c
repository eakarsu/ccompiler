int printf(const char *fmt, ...);
// EXPECT: Primes up to 199:\nCount: 46\nFirst 10: 2 3 5 7 11 13 17 19 23 29 \nLast 5: 181 191 193 197 199 \nSum of primes: 4227\nTwin prime pairs: 15\nIs 97 prime? 1\nIs 100 prime? 0\nIs 199 prime? 1
// Test: Prime number sieve (Sieve of Eratosthenes)

int main(void) {
    int sieve[200];
    int primes[50];
    int count = 0;
    int i, j;
    int n = 200;

    // Initialize sieve
    for (i = 0; i < n; i++) {
        sieve[i] = 1;
    }
    sieve[0] = 0;
    sieve[1] = 0;

    // Sieve of Eratosthenes
    for (i = 2; i * i < n; i++) {
        if (sieve[i]) {
            for (j = i * i; j < n; j = j + i) {
                sieve[j] = 0;
            }
        }
    }

    // Collect primes
    for (i = 2; i < n; i++) {
        if (sieve[i]) {
            primes[count] = i;
            count++;
        }
    }

    printf("Primes up to %d:\n", n - 1);
    printf("Count: %d\n", count);

    // Print first 10 primes
    printf("First 10: ");
    for (i = 0; i < 10; i++) {
        printf("%d ", primes[i]);
    }
    printf("\n");

    // Print last 5 primes
    printf("Last 5: ");
    for (i = count - 5; i < count; i++) {
        printf("%d ", primes[i]);
    }
    printf("\n");

    // Sum of all primes found
    int sum = 0;
    for (i = 0; i < count; i++) {
        sum = sum + primes[i];
    }
    printf("Sum of primes: %d\n", sum);

    // Count twin primes
    int twins = 0;
    for (i = 0; i < count - 1; i++) {
        if (primes[i + 1] - primes[i] == 2) {
            twins++;
        }
    }
    printf("Twin prime pairs: %d\n", twins);

    // Check specific numbers
    printf("Is 97 prime? %d\n", sieve[97]);
    printf("Is 100 prime? %d\n", sieve[100]);
    printf("Is 199 prime? %d\n", sieve[199]);

    return 0;
}
