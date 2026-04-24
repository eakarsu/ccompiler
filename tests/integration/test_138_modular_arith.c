int printf(const char *fmt, ...);
// EXPECT: (7+8) mod 10 = 5\n(15+25) mod 7 = 5\n(7*8) mod 10 = 6\n(15*25) mod 7 = 4\n2^10 mod 1000 = 24\n3^7 mod 100 = 87\n7^4 mod 13 = 9\nFermat test:\n  2^12 mod 13 = 1\n  3^12 mod 13 = 1\n  4^12 mod 13 = 1\n  5^12 mod 13 = 1\n  6^12 mod 13 = 1\n  7^12 mod 13 = 1\n  8^12 mod 13 = 1\n  9^12 mod 13 = 1\n  10^12 mod 13 = 1\n  11^12 mod 13 = 1\n  12^12 mod 13 = 1\nmul table mod 5:\n0 0 0 0 0 \n0 1 2 3 4 \n0 2 4 1 3 \n0 3 1 4 2 \n0 4 3 2 1 \ninverses mod 7: 1^-1=1 2^-1=4 3^-1=5 4^-1=2 5^-1=3 6^-1=6 \nCRT: 8 23 38 
// Test: modular arithmetic

int mod_add(int a, int b, int m) {
    return (a % m + b % m) % m;
}

int mod_mul(int a, int b, int m) {
    return ((a % m) * (b % m)) % m;
}

int mod_pow(int base, int exp, int m) {
    int result;
    result = 1;
    base = base % m;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % m;
        }
        exp = exp / 2;
        base = (base * base) % m;
    }
    return result;
}

int gcd(int a, int b) {
    int temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int main(void) {
    int i;

    /* Basic modular addition */
    printf("(7+8) mod 10 = %d\n", mod_add(7, 8, 10));
    printf("(15+25) mod 7 = %d\n", mod_add(15, 25, 7));

    /* Modular multiplication */
    printf("(7*8) mod 10 = %d\n", mod_mul(7, 8, 10));
    printf("(15*25) mod 7 = %d\n", mod_mul(15, 25, 7));

    /* Modular exponentiation */
    printf("2^10 mod 1000 = %d\n", mod_pow(2, 10, 1000));
    printf("3^7 mod 100 = %d\n", mod_pow(3, 7, 100));
    printf("7^4 mod 13 = %d\n", mod_pow(7, 4, 13));

    /* Fermat's little theorem: a^(p-1) ≡ 1 (mod p) for prime p */
    printf("Fermat test:\n");
    {
        int p;
        p = 13;
        for (i = 2; i < p; i++) {
            printf("  %d^%d mod %d = %d\n", i, p - 1, p, mod_pow(i, p - 1, p));
        }
    }

    /* Multiplication table mod 5 */
    printf("mul table mod 5:\n");
    for (i = 0; i < 5; i++) {
        int j;
        for (j = 0; j < 5; j++) {
            printf("%d ", (i * j) % 5);
        }
        printf("\n");
    }

    /* Find multiplicative inverse mod 7 */
    printf("inverses mod 7: ");
    for (i = 1; i < 7; i++) {
        int j;
        for (j = 1; j < 7; j++) {
            if ((i * j) % 7 == 1) {
                printf("%d^-1=%d ", i, j);
                break;
            }
        }
    }
    printf("\n");

    /* Chinese Remainder: find x where x%3==2 and x%5==3 */
    printf("CRT: ");
    for (i = 0; i < 50; i++) {
        if (i % 3 == 2 && i % 5 == 3) {
            printf("%d ", i);
        }
    }
    printf("\n");

    return 0;
}
