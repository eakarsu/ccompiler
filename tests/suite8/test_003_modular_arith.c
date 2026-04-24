int printf(const char *fmt, ...);

// Modular exponentiation: (base^exp) % mod
int mod_pow(int base, int exp, int mod) {
    int result;
    result = 1;
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

// Modular inverse using extended euclidean algorithm
int ext_gcd(int a, int b, int *x, int *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int x1;
    int y1;
    int g;
    g = ext_gcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

int mod_inverse(int a, int m) {
    int x;
    int y;
    int g;
    g = ext_gcd(a, m, &x, &y);
    if (g != 1) return -1;  // No inverse
    return ((x % m) + m) % m;
}

// Modular addition (safe from overflow for reasonable values)
int mod_add(int a, int b, int mod) {
    return ((a % mod) + (b % mod)) % mod;
}

// Modular multiplication
int mod_mul(int a, int b, int mod) {
    return ((a % mod) * (b % mod)) % mod;
}

// Modular subtraction
int mod_sub(int a, int b, int mod) {
    return (((a % mod) - (b % mod)) + mod) % mod;
}

// Check if a is a quadratic residue mod p (Euler's criterion)
int is_quad_residue(int a, int p) {
    if (a % p == 0) return 1;
    int r;
    r = mod_pow(a, (p - 1) / 2, p);
    return r == 1;
}

// Chinese Remainder Theorem for 2 congruences
// x = a1 (mod m1), x = a2 (mod m2)
int crt_two(int a1, int m1, int a2, int m2) {
    int inv;
    int x;
    inv = mod_inverse(m1, m2);
    if (inv == -1) return -1;
    x = a1 + m1 * (((a2 - a1) % m2 + m2) % m2 * inv % m2);
    return x % (m1 * m2);
}

// Fermat primality test (probabilistic)
int fermat_test(int n, int a) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    return mod_pow(a, n - 1, n) == 1;
}

// Compute factorial mod m
int factorial_mod(int n, int m) {
    int result;
    int i;
    result = 1;
    for (i = 2; i <= n; i++) {
        result = (result * i) % m;
    }
    return result;
}

// Compute nCr mod p (for small values)
int ncr_mod(int n, int r, int p) {
    int num;
    int den;
    int i;
    if (r > n) return 0;
    if (r == 0 || r == n) return 1;
    num = factorial_mod(n, p);
    den = mod_mul(factorial_mod(r, p), factorial_mod(n - r, p), p);
    return mod_mul(num, mod_inverse(den, p), p);
}

int main() {
    // EXPECT: mod_pow(2, 10, 1000) = 24
    printf("mod_pow(2, 10, 1000) = %d\n", mod_pow(2, 10, 1000));

    // EXPECT: mod_pow(3, 13, 7) = 3
    printf("mod_pow(3, 13, 7) = %d\n", mod_pow(3, 13, 7));

    // EXPECT: mod_pow(5, 3, 13) = 8
    printf("mod_pow(5, 3, 13) = %d\n", mod_pow(5, 3, 13));

    // EXPECT: mod_inverse(3, 7) = 5
    printf("mod_inverse(3, 7) = %d\n", mod_inverse(3, 7));

    // EXPECT: mod_inverse(10, 17) = 12
    printf("mod_inverse(10, 17) = %d\n", mod_inverse(10, 17));

    // EXPECT: mod_add(15, 20, 7) = 0
    printf("mod_add(15, 20, 7) = %d\n", mod_add(15, 20, 7));

    // EXPECT: mod_mul(12, 13, 5) = 1
    printf("mod_mul(12, 13, 5) = %d\n", mod_mul(12, 13, 5));

    // EXPECT: mod_sub(3, 10, 7) = 0
    printf("mod_sub(3, 10, 7) = %d\n", mod_sub(3, 10, 7));

    // EXPECT: quad_residue(2, 7) = 1
    printf("quad_residue(2, 7) = %d\n", is_quad_residue(2, 7));

    // EXPECT: quad_residue(3, 7) = 0
    printf("quad_residue(3, 7) = %d\n", is_quad_residue(3, 7));

    // EXPECT: crt(2,3, 3,5) = 8
    printf("crt(2,3, 3,5) = %d\n", crt_two(2, 3, 3, 5));

    // EXPECT: fermat(17, 2) = 1
    printf("fermat(17, 2) = %d\n", fermat_test(17, 2));

    // EXPECT: fermat(15, 2) = 0
    printf("fermat(15, 2) = %d\n", fermat_test(15, 2));

    // EXPECT: factorial_mod(10, 97) = 30
    printf("factorial_mod(10, 97) = %d\n", factorial_mod(10, 97));

    // EXPECT: ncr_mod(10, 3, 97) = 23
    printf("ncr_mod(10, 3, 97) = %d\n", ncr_mod(10, 3, 97));

    return 0;
}
