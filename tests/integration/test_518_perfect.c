int printf(const char *fmt, ...);
// EXPECT: Sum of proper divisors s(n):\ns(1) = 0\ns(2) = 1\ns(3) = 1\ns(4) = 3\ns(5) = 1\ns(6) = 6\ns(7) = 1\ns(8) = 7\ns(9) = 4\ns(10) = 8\ns(11) = 1\ns(12) = 16\ns(13) = 1\ns(14) = 10\ns(15) = 9\ns(16) = 15\ns(17) = 1\ns(18) = 21\ns(19) = 1\ns(20) = 22\ns(21) = 11\ns(22) = 14\ns(23) = 1\ns(24) = 36\ns(25) = 6\ns(26) = 16\ns(27) = 13\ns(28) = 28\ns(29) = 1\ns(30) = 42\n\nPerfect numbers up to 10000:\n6 is perfect\n  Divisors: 1 2 3 \n28 is perfect\n  Divisors: 1 2 4 7 14 \n496 is perfect\n  Divisors: 1 2 4 8 16 31 62 124 248 \n8128 is perfect\n  Divisors: 1 2 4 8 16 32 64 127 254 508 1016 2032 4064 \n\nMersenne primes and perfect numbers:\np=2: 2^p-1=3 (prime), perfect=6\np=3: 2^p-1=7 (prime), perfect=28\np=5: 2^p-1=31 (prime), perfect=496\np=7: 2^p-1=127 (prime), perfect=8128\np=13: 2^p-1=8191 (prime), perfect=33550336\n\nAliquot sequences (10 steps):\n10: 8 7 1 \n12: 16 15 9 4 3 1 \n28: 28 28 28 28 28 28 28 28 28 28 \n15: 9 4 3 1 \n220: 284 220 284 220 284 220 284 220 284 220 
// Test: Perfect numbers detection

int sum_of_proper_divisors(int n) {
    int sum = 0;
    int i;
    if (n <= 1) return 0;
    sum = 1;
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            sum = sum + i;
            if (i != n / i) {
                sum = sum + n / i;
            }
        }
    }
    return sum;
}

int is_prime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    int i;
    for (i = 3; i * i <= n; i = i + 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int main(void) {
    int i;

    printf("Sum of proper divisors s(n):\n");
    for (i = 1; i <= 30; i++) {
        printf("s(%d) = %d\n", i, sum_of_proper_divisors(i));
    }

    // Find perfect numbers
    printf("\nPerfect numbers up to 10000:\n");
    for (i = 2; i <= 10000; i++) {
        if (sum_of_proper_divisors(i) == i) {
            printf("%d is perfect\n", i);
            // Print its divisors
            printf("  Divisors: ");
            int j;
            for (j = 1; j < i; j++) {
                if (i % j == 0) printf("%d ", j);
            }
            printf("\n");
        }
    }

    // Even perfect numbers have form 2^(p-1) * (2^p - 1) where 2^p-1 is prime
    printf("\nMersenne primes and perfect numbers:\n");
    for (i = 2; i <= 16; i++) {
        int mersenne = 1;
        int j;
        for (j = 0; j < i; j++) mersenne = mersenne * 2;
        mersenne = mersenne - 1;
        if (is_prime(mersenne)) {
            int power_part = 1;
            for (j = 0; j < i - 1; j++) power_part = power_part * 2;
            int perfect = power_part * mersenne;
            printf("p=%d: 2^p-1=%d (prime), perfect=%d\n", i, mersenne, perfect);
        }
    }

    // Aliquot sequences starting from small numbers
    printf("\nAliquot sequences (10 steps):\n");
    int starts[5];
    starts[0] = 10;
    starts[1] = 12;
    starts[2] = 28;
    starts[3] = 15;
    starts[4] = 220;

    for (i = 0; i < 5; i++) {
        int n = starts[i];
        printf("%d: ", n);
        int j;
        for (j = 0; j < 10; j++) {
            n = sum_of_proper_divisors(n);
            if (n > 100000 || n == 0) break;
            printf("%d ", n);
        }
        printf("\n");
    }

    return 0;
}
