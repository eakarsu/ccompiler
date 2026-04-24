int printf(const char *fmt, ...);
// EXPECT: 2^10 mod 1000 = 24\n3^13 mod 100 = 23\n7^256 mod 13 = 9\n2^20 mod 1000000007 = 1048576\n\nModular inverses mod 17:\n  1 * 1 = 1 (mod 17)\n  2 * 9 = 1 (mod 17)\n  3 * 6 = 1 (mod 17)\n  4 * 13 = 1 (mod 17)\n  5 * 7 = 1 (mod 17)\n  6 * 3 = 1 (mod 17)\n  7 * 5 = 1 (mod 17)\n  8 * 15 = 1 (mod 17)\n  9 * 2 = 1 (mod 17)\n  10 * 12 = 1 (mod 17)\n  11 * 14 = 1 (mod 17)\n  12 * 10 = 1 (mod 17)\n  13 * 4 = 1 (mod 17)\n  14 * 11 = 1 (mod 17)\n  15 * 8 = 1 (mod 17)\n  16 * 16 = 1 (mod 17)\n\nFermat primality:\n  2: prime\n  7: prime\n  13: prime\n  15: not prime\n  97: prime\n  100: not prime\n  561: not prime\n\nC(10,3) mod 97 = 23\nC(20,10) mod 97 = 68
// Test: Modular exponentiation

int mod_pow(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp / 2;
        base = (base * base) % mod;
    }
    return result;
}

int mod_inverse(int a, int mod) {
    return mod_pow(a, mod - 2, mod);
}

int is_fermat_prime(int n) {
    if (n < 2) return 0;
    if (n < 4) return 1;
    int tests[] = {2, 3, 5, 7, 11};
    int num_tests = 5;
    int i;
    for (i = 0; i < num_tests; i++) {
        if (tests[i] >= n) continue;
        if (mod_pow(tests[i], n - 1, n) != 1) return 0;
    }
    return 1;
}

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int mod_nCr(int n, int r, int p) {
    if (r > n) return 0;
    if (r == 0 || r == n) return 1;

    int num = 1;
    int den = 1;
    int i;
    for (i = 0; i < r; i++) {
        num = (num * ((n - i) % p)) % p;
        den = (den * ((i + 1) % p)) % p;
    }
    return (num * mod_inverse(den, p)) % p;
}

int main(void) {
    printf("2^10 mod 1000 = %d\n", mod_pow(2, 10, 1000));
    printf("3^13 mod 100 = %d\n", mod_pow(3, 13, 100));
    printf("7^256 mod 13 = %d\n", mod_pow(7, 256, 13));
    printf("2^20 mod 1000000007 = %d\n", mod_pow(2, 20, 1000000007));

    int p = 17;
    printf("\nModular inverses mod %d:\n", p);
    int i;
    for (i = 1; i < p; i++) {
        int inv = mod_inverse(i, p);
        printf("  %d * %d = %d (mod %d)\n", i, inv, (i * inv) % p, p);
    }

    printf("\nFermat primality:\n");
    int test_nums[] = {2, 7, 13, 15, 97, 100, 561};
    int n = 7;
    for (i = 0; i < n; i++) {
        printf("  %d: %s\n", test_nums[i],
               is_fermat_prime(test_nums[i]) ? "prime" : "not prime");
    }

    printf("\nC(10,3) mod 97 = %d\n", mod_nCr(10, 3, 97));
    printf("C(20,10) mod 97 = %d\n", mod_nCr(20, 10, 97));

    return 0;
}
