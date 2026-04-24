int printf(const char *fmt, ...);
// EXPECT: 12 = 2^2 * 3\n60 = 2^2 * 3 * 5\n100 = 2^2 * 5^2\n360 = 2^3 * 3^2 * 5\n1024 = 2^10\n97 = 97\n2310 = 2 * 3 * 5 * 7 * 11\n5040 = 2^4 * 3^2 * 5 * 7\n999 = 3^3 * 37\n1 = 1\nTotal factors with multiplicity:\n12: 3\n60: 4\n100: 4\n360: 6\n1024: 10\n97: 1\n2310: 5\n5040: 8\n999: 4\n1: 0
// Test: Prime factorization

int main(void) {
    int numbers[10];
    int factors[20];
    int exponents[20];
    int nfactors;
    int i, j, n, d;

    numbers[0] = 12;
    numbers[1] = 60;
    numbers[2] = 100;
    numbers[3] = 360;
    numbers[4] = 1024;
    numbers[5] = 97;
    numbers[6] = 2310;

    // Compute factorial of 7
    int f7 = 1;
    for (i = 1; i <= 7; i++) {
        f7 = f7 * i;
    }
    numbers[7] = f7;
    numbers[8] = 999;
    numbers[9] = 1;

    for (i = 0; i < 10; i++) {
        n = numbers[i];
        nfactors = 0;
        int temp = n;

        if (temp < 2) {
            printf("%d = %d\n", n, n);
        } else {
            d = 2;
            while (d * d <= temp) {
                if (temp % d == 0) {
                    factors[nfactors] = d;
                    exponents[nfactors] = 0;
                    while (temp % d == 0) {
                        exponents[nfactors]++;
                        temp = temp / d;
                    }
                    nfactors++;
                }
                d++;
            }
            if (temp > 1) {
                factors[nfactors] = temp;
                exponents[nfactors] = 1;
                nfactors++;
            }

            printf("%d = ", n);
            for (j = 0; j < nfactors; j++) {
                if (j > 0) printf(" * ");
                if (exponents[j] == 1) {
                    printf("%d", factors[j]);
                } else {
                    printf("%d^%d", factors[j], exponents[j]);
                }
            }
            printf("\n");
        }
    }

    // Count total prime factors (with multiplicity) for each
    printf("Total factors with multiplicity:\n");
    for (i = 0; i < 10; i++) {
        n = numbers[i];
        int temp = n;
        int total = 0;
        d = 2;
        while (d * d <= temp) {
            while (temp % d == 0) {
                total++;
                temp = temp / d;
            }
            d++;
        }
        if (temp > 1) total++;
        printf("%d: %d\n", n, total);
    }

    return 0;
}
