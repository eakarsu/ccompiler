int printf(const char *fmt, ...);
// EXPECT: primes: 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97 \ncount = 25\nsum = 1060\nprimorial(5) = 2310\nmax gap = 8\n[1-10]:4 [11-20]:4 [21-30]:2 [31-40]:2 [41-50]:3 [51-60]:2 [61-70]:2 [71-80]:3 [81-90]:2 [91-100]:1 \nsemi-primes: 4 6 9 10 14 15 21 22 25 26 
// Test: sieve of Eratosthenes and prime sums

int main(void) {
    int sieve[101];
    int i, j;
    int count;
    int sum;
    int max_gap, prev_prime;

    /* Initialize sieve */
    for (i = 0; i <= 100; i++) {
        sieve[i] = 1;
    }
    sieve[0] = 0;
    sieve[1] = 0;

    /* Sieve of Eratosthenes */
    for (i = 2; i * i <= 100; i++) {
        if (sieve[i]) {
            for (j = i * i; j <= 100; j += i) {
                sieve[j] = 0;
            }
        }
    }

    /* Print primes */
    printf("primes: ");
    for (i = 2; i <= 100; i++) {
        if (sieve[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");

    /* Count primes */
    count = 0;
    for (i = 2; i <= 100; i++) {
        if (sieve[i]) count++;
    }
    printf("count = %d\n", count);

    /* Sum of all primes */
    sum = 0;
    for (i = 2; i <= 100; i++) {
        if (sieve[i]) sum += i;
    }
    printf("sum = %d\n", sum);

    /* Product of first 5 primes */
    {
        int prod, c;
        prod = 1;
        c = 0;
        for (i = 2; i <= 100 && c < 5; i++) {
            if (sieve[i]) {
                prod *= i;
                c++;
            }
        }
        printf("primorial(5) = %d\n", prod);
    }

    /* Maximum gap between consecutive primes */
    max_gap = 0;
    prev_prime = 2;
    for (i = 3; i <= 100; i++) {
        if (sieve[i]) {
            int gap;
            gap = i - prev_prime;
            if (gap > max_gap) {
                max_gap = gap;
            }
            prev_prime = i;
        }
    }
    printf("max gap = %d\n", max_gap);

    /* Count primes in ranges */
    for (i = 0; i < 10; i++) {
        int lo, hi;
        lo = i * 10 + 1;
        hi = (i + 1) * 10;
        count = 0;
        for (j = lo; j <= hi; j++) {
            if (j >= 2 && j <= 100 && sieve[j]) count++;
        }
        printf("[%d-%d]:%d ", lo, hi, count);
    }
    printf("\n");

    /* Semi-primes (product of exactly two primes) up to 30 */
    printf("semi-primes: ");
    for (i = 4; i <= 30; i++) {
        int n, fc;
        n = i;
        fc = 0;
        for (j = 2; j <= n; j++) {
            while (n % j == 0) {
                fc++;
                n /= j;
            }
        }
        if (fc == 2) {
            printf("%d ", i);
        }
    }
    printf("\n");

    return 0;
}
