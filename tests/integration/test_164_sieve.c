int printf(const char *fmt, ...);
// EXPECT: Primes up to 100:\n2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97 \ncount=25\nTwin primes: (3,5) (5,7) (11,13) (17,19) (29,31) (41,43) (59,61) (71,73) \nPrime gaps: [2-3]=1 [3-5]=2 [7-11]=4 [23-29]=6 [89-97]=8 \nGoldbach (even 4-30): 4=2+2 6=3+3 8=3+5 10=3+7 12=5+7 14=3+11 16=3+13 18=5+13 20=3+17 22=3+19 24=5+19 26=3+23 28=5+23 30=7+23 \nSum of primes: 1060
int main(void) {
    int sieve[101];
    int i;
    int j;

    for (i = 0; i <= 100; i++) {
        sieve[i] = 1;
    }
    sieve[0] = 0;
    sieve[1] = 0;

    for (i = 2; i * i <= 100; i++) {
        if (sieve[i]) {
            for (j = i * i; j <= 100; j = j + i) {
                sieve[j] = 0;
            }
        }
    }

    printf("Primes up to 100:\n");
    int count = 0;
    for (i = 2; i <= 100; i++) {
        if (sieve[i]) {
            printf("%d ", i);
            count++;
        }
    }
    printf("\ncount=%d\n", count);

    printf("Twin primes: ");
    for (i = 2; i <= 98; i++) {
        if (sieve[i] && sieve[i + 2]) {
            printf("(%d,%d) ", i, i + 2);
        }
    }
    printf("\n");

    printf("Prime gaps: ");
    int last_prime = 2;
    int max_gap = 0;
    for (i = 3; i <= 100; i++) {
        if (sieve[i]) {
            int gap = i - last_prime;
            if (gap > max_gap) {
                max_gap = gap;
                printf("[%d-%d]=%d ", last_prime, i, gap);
            }
            last_prime = i;
        }
    }
    printf("\n");

    printf("Goldbach (even 4-30): ");
    int even;
    for (even = 4; even <= 30; even = even + 2) {
        int found = 0;
        for (i = 2; i <= even / 2; i++) {
            if (sieve[i] && sieve[even - i]) {
                if (!found) {
                    printf("%d=%d+%d ", even, i, even - i);
                    found = 1;
                }
            }
        }
    }
    printf("\n");

    printf("Sum of primes: ");
    int sum = 0;
    for (i = 2; i <= 100; i++) {
        if (sieve[i]) {
            sum = sum + i;
        }
    }
    printf("%d\n", sum);

    return 0;
}
