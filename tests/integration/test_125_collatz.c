int printf(const char *fmt, ...);
// EXPECT: collatz(27): 27 82 41 124 62 31 94 47 142 71 214 107 322 161 484 242 121 364 182 91 274 ...\nlengths: 0 1 7 2 5 8 16 3 19 6 14 9 9 17 17 4 12 20 20 7 \nmax(27) = 9232\nmax(7) = 52\nlongest in 1..100: start=97 len=118\nn=19: even=14 odd=6 total=20\npow2 lengths: 0 1 2 3 4 5 6 7 8 9 
// Test: Collatz conjecture sequences

int collatz_length(int n) {
    int count;
    count = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        count++;
    }
    return count;
}

int collatz_max(int n) {
    int max_val;
    max_val = n;
    while (n != 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        if (n > max_val) {
            max_val = n;
        }
    }
    return max_val;
}

int main(void) {
    int i;
    int n;
    int max_len, max_start;

    /* Print Collatz sequence for n=27 */
    printf("collatz(27): ");
    n = 27;
    printf("%d", n);
    {
        int steps;
        steps = 0;
        while (n != 1 && steps < 20) {
            if (n % 2 == 0) {
                n = n / 2;
            } else {
                n = 3 * n + 1;
            }
            printf(" %d", n);
            steps++;
        }
    }
    printf(" ...\n");

    /* Collatz lengths for 1-20 */
    printf("lengths: ");
    for (i = 1; i <= 20; i++) {
        printf("%d ", collatz_length(i));
    }
    printf("\n");

    /* Maximum value in sequence */
    printf("max(27) = %d\n", collatz_max(27));
    printf("max(7) = %d\n", collatz_max(7));

    /* Find number with longest Collatz sequence in 1-100 */
    max_len = 0;
    max_start = 1;
    for (i = 1; i <= 100; i++) {
        int len;
        len = collatz_length(i);
        if (len > max_len) {
            max_len = len;
            max_start = i;
        }
    }
    printf("longest in 1..100: start=%d len=%d\n", max_start, max_len);

    /* Count even vs odd steps for n=19 */
    {
        int even_steps, odd_steps;
        even_steps = 0;
        odd_steps = 0;
        n = 19;
        while (n != 1) {
            if (n % 2 == 0) {
                n = n / 2;
                even_steps++;
            } else {
                n = 3 * n + 1;
                odd_steps++;
            }
        }
        printf("n=19: even=%d odd=%d total=%d\n", even_steps, odd_steps, even_steps + odd_steps);
    }

    /* Collatz stopping time for powers of 2 */
    printf("pow2 lengths: ");
    for (i = 0; i < 10; i++) {
        int p;
        p = 1;
        {
            int k;
            for (k = 0; k < i; k++) {
                p = p * 2;
            }
        }
        printf("%d ", collatz_length(p));
    }
    printf("\n");

    return 0;
}
