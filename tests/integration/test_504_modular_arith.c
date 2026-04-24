int printf(const char *fmt, ...);
// EXPECT: Modular arithmetic mod 97:\n(45 + 73) mod 97 = 21\n(45 * 73) mod 97 = 84\n2^10 mod 97 = 54\n3^20 mod 97 = 91\nFermat's little theorem:\n  2^96 mod 97 = 1\n  3^96 mod 97 = 1\n  4^96 mod 97 = 1\n  5^96 mod 97 = 1\n  6^96 mod 97 = 1\nModular inverses:\n  1^(-1) mod 97 = 1 (check: 1*1 mod 97 = 1)\n  2^(-1) mod 97 = 49 (check: 2*49 mod 97 = 1)\n  3^(-1) mod 97 = 65 (check: 3*65 mod 97 = 1)\n  4^(-1) mod 97 = 73 (check: 4*73 mod 97 = 1)\n  5^(-1) mod 97 = 39 (check: 5*39 mod 97 = 1)\nSolution of 3x = 7 mod 97: x = 67\nVerification: 3*67 mod 97 = 7\nPowers of 2 mod 13: 1 2 4 8 3 6 12 11 9 5 10 7 
// Test: Modular arithmetic

int mod_add(int a, int b, int m) {
    return ((a % m) + (b % m)) % m;
}

int mod_mul(int a, int b, int m) {
    return ((a % m) * (b % m)) % m;
}

int mod_pow(int base, int exp, int m) {
    int result = 1;
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

int mod_inverse(int a, int m) {
    // Using extended Euclidean algorithm
    int old_r = a, r = m;
    int old_s = 1, s = 0;
    while (r != 0) {
        int q = old_r / r;
        int tmp = r;
        r = old_r - q * r;
        old_r = tmp;
        tmp = s;
        s = old_s - q * s;
        old_s = tmp;
    }
    if (old_r != 1) return -1; // no inverse
    if (old_s < 0) old_s = old_s + m;
    return old_s;
}

int main(void) {
    int m = 97; // prime modulus
    int i;

    printf("Modular arithmetic mod %d:\n", m);

    // Basic operations
    printf("(45 + 73) mod 97 = %d\n", mod_add(45, 73, m));
    printf("(45 * 73) mod 97 = %d\n", mod_mul(45, 73, m));
    printf("2^10 mod 97 = %d\n", mod_pow(2, 10, m));
    printf("3^20 mod 97 = %d\n", mod_pow(3, 20, m));

    // Fermat's little theorem: a^(p-1) = 1 mod p for prime p
    printf("Fermat's little theorem:\n");
    for (i = 2; i <= 6; i++) {
        int result = mod_pow(i, m - 1, m);
        printf("  %d^%d mod %d = %d\n", i, m - 1, m, result);
    }

    // Modular inverses
    printf("Modular inverses:\n");
    for (i = 1; i <= 5; i++) {
        int inv = mod_inverse(i, m);
        int check = mod_mul(i, inv, m);
        printf("  %d^(-1) mod %d = %d (check: %d*%d mod %d = %d)\n",
               i, m, inv, i, inv, m, check);
    }

    // Solve linear congruence: 3x = 7 mod 97
    int inv3 = mod_inverse(3, m);
    int x = mod_mul(7, inv3, m);
    printf("Solution of 3x = 7 mod 97: x = %d\n", x);
    printf("Verification: 3*%d mod 97 = %d\n", x, mod_mul(3, x, m));

    // Powers of 2 mod 13
    printf("Powers of 2 mod 13: ");
    for (i = 0; i < 12; i++) {
        printf("%d ", mod_pow(2, i, 13));
    }
    printf("\n");

    return 0;
}
