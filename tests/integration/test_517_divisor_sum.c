int printf(const char *fmt, ...);
// EXPECT: Divisor functions:\nn    d(n)  sigma(n)  sigma2(n)\n1    1     1         1\n2    2     3         5\n3    2     4         10\n4    3     7         21\n5    2     6         26\n6    4     12        50\n7    2     8         50\n8    4     15        85\n9    3     13        91\n10   4     18        130\n11   2     12        122\n12   6     28        210\n13   2     14        170\n14   4     24        250\n15   4     24        260\n16   5     31        341\n17   2     18        290\n18   6     39        455\n19   2     20        362\n20   6     42        546\n21   4     32        500\n22   4     36        610\n23   2     24        530\n24   8     60        850\n\nHighly composite numbers up to 100:\n1 (divisors: 1)\n2 (divisors: 2)\n4 (divisors: 3)\n6 (divisors: 4)\n12 (divisors: 6)\n24 (divisors: 8)\n36 (divisors: 9)\n48 (divisors: 10)\n60 (divisors: 12)\n\nPerfect number check (sigma(n) = 2n):\n6 is perfect (sigma = 12)\n28 is perfect (sigma = 56)\n\nAbundancy index (sigma(n)*100/n):\nn=1: 100/100\nn=2: 150/100\nn=3: 133/100\nn=4: 175/100\nn=5: 120/100\nn=6: 200/100\nn=7: 114/100\nn=8: 187/100\nn=9: 144/100\nn=10: 180/100\nn=11: 109/100\nn=12: 233/100\nn=13: 107/100\nn=14: 171/100\nn=15: 160/100\nn=16: 193/100\nn=17: 105/100\nn=18: 216/100\nn=19: 105/100\nn=20: 210/100\n\nMultiplicative check: sigma(3*5) = sigma(3)*sigma(5):\nsigma(15) = 24, sigma(3)*sigma(5) = 24
// Test: Divisor sum functions

int sigma0(int n) {
    // Number of divisors
    int count = 0;
    int i;
    for (i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            count++;
            if (i != n / i) count++;
        }
    }
    return count;
}

int sigma1(int n) {
    // Sum of divisors
    int sum = 0;
    int i;
    for (i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            sum = sum + i;
            if (i != n / i) sum = sum + n / i;
        }
    }
    return sum;
}

int sigma2(int n) {
    // Sum of squares of divisors
    int sum = 0;
    int i;
    for (i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            sum = sum + i * i;
            int d = n / i;
            if (i != d) sum = sum + d * d;
        }
    }
    return sum;
}

int main(void) {
    int i;

    printf("Divisor functions:\n");
    printf("n    d(n)  sigma(n)  sigma2(n)\n");
    for (i = 1; i <= 24; i++) {
        printf("%-4d %-5d %-9d %d\n", i, sigma0(i), sigma1(i), sigma2(i));
    }

    // Highly composite numbers: numbers with more divisors than any smaller number
    printf("\nHighly composite numbers up to 100:\n");
    int max_div = 0;
    for (i = 1; i <= 100; i++) {
        int d = sigma0(i);
        if (d > max_div) {
            max_div = d;
            printf("%d (divisors: %d)\n", i, d);
        }
    }

    // Perfect numbers: sigma(n) = 2n
    printf("\nPerfect number check (sigma(n) = 2n):\n");
    for (i = 1; i <= 30; i++) {
        int s = sigma1(i);
        if (s == 2 * i) {
            printf("%d is perfect (sigma = %d)\n", i, s);
        }
    }

    // Abundancy: sigma(n)/n
    printf("\nAbundancy index (sigma(n)*100/n):\n");
    for (i = 1; i <= 20; i++) {
        int idx = sigma1(i) * 100 / i;
        printf("n=%d: %d/100\n", i, idx);
    }

    // Multiplicative property: sigma(mn) = sigma(m)*sigma(n) when gcd(m,n)=1
    printf("\nMultiplicative check: sigma(3*5) = sigma(3)*sigma(5):\n");
    printf("sigma(15) = %d, sigma(3)*sigma(5) = %d\n",
           sigma1(15), sigma1(3) * sigma1(5));

    return 0;
}
