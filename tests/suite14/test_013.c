int printf(const char *fmt, ...);

// Modular exponentiation (fast power)

// Compute (base^exp) % mod using binary exponentiation
int mod_pow(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    if (base < 0) {
        base = base + mod;
    }
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp / 2;
        base = (base * base) % mod;
    }
    return result;
}

// Naive power for small values (no mod)
int naive_pow(int base, int exp) {
    int result = 1;
    int i;
    for (i = 0; i < exp; i++) {
        result = result * base;
    }
    return result;
}

// Fermat primality test: a^(p-1) mod p == 1 for prime p
int fermat_test(int n, int a) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    int result = mod_pow(a, n - 1, n);
    return result == 1;
}

// Simple primality check
int is_prime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    int i = 5;
    while (i * i <= n) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
        i = i + 6;
    }
    return 1;
}

// Compute modular multiplicative order of a mod n
// Smallest k such that a^k mod n == 1
int mod_order(int a, int n) {
    if (a % n == 0) return -1;
    int result = 1;
    int power = a % n;
    while (power != 1) {
        power = (power * a) % n;
        result = result + 1;
        if (result > n) return -1;
    }
    return result;
}

// Repeated squaring to compute sum: 1^k + 2^k + ... + n^k mod m
int sum_of_powers_mod(int n, int k, int m) {
    int sum = 0;
    int i;
    for (i = 1; i <= n; i++) {
        sum = (sum + mod_pow(i, k, m)) % m;
    }
    return sum;
}

// Check if g is a primitive root mod p
int is_primitive_root(int g, int p) {
    if (!is_prime(p)) return 0;
    int order = mod_order(g, p);
    return order == p - 1;
}

int main(void) {
    printf("2^10 mod 1000: %d\n", mod_pow(2, 10, 1000));
    // EXPECT: 2^10 mod 1000: 24

    printf("3^13 mod 100: %d\n", mod_pow(3, 13, 100));
    // EXPECT: 3^13 mod 100: 23

    printf("7^256 mod 13: %d\n", mod_pow(7, 256, 13));
    // EXPECT: 7^256 mod 13: 9

    printf("2^0 mod 5: %d\n", mod_pow(2, 0, 5));
    // EXPECT: 2^0 mod 5: 1

    printf("5^3 mod 1000: %d\n", mod_pow(5, 3, 1000));
    // EXPECT: 5^3 mod 1000: 125

    int np = naive_pow(2, 10);
    printf("naive 2^10: %d\n", np);
    // EXPECT: naive 2^10: 1024

    printf("naive 3^5: %d\n", naive_pow(3, 5));
    // EXPECT: naive 3^5: 243

    // Fermat test on known primes
    printf("Fermat(17,2): %d\n", fermat_test(17, 2));
    // EXPECT: Fermat(17,2): 1

    printf("Fermat(17,3): %d\n", fermat_test(17, 3));
    // EXPECT: Fermat(17,3): 1

    printf("Fermat(15,2): %d\n", fermat_test(15, 2));
    // EXPECT: Fermat(15,2): 0

    printf("Fermat(97,2): %d\n", fermat_test(97, 2));
    // EXPECT: Fermat(97,2): 1

    // Modular order
    printf("order(2, 7): %d\n", mod_order(2, 7));
    // EXPECT: order(2, 7): 3

    printf("order(3, 7): %d\n", mod_order(3, 7));
    // EXPECT: order(3, 7): 6

    printf("order(2, 13): %d\n", mod_order(2, 13));
    // EXPECT: order(2, 13): 12

    // Sum of powers mod
    // 1^2 + 2^2 + 3^2 + ... + 10^2 = 385
    printf("sum 1^2..10^2 mod 1000: %d\n", sum_of_powers_mod(10, 2, 1000));
    // EXPECT: sum 1^2..10^2 mod 1000: 385

    // 1^3 + 2^3 + ... + 5^3 = 1+8+27+64+125 = 225
    printf("sum 1^3..5^3 mod 1000: %d\n", sum_of_powers_mod(5, 3, 1000));
    // EXPECT: sum 1^3..5^3 mod 1000: 225

    // Primitive roots
    printf("prim_root(2, 7): %d\n", is_primitive_root(2, 7));
    // EXPECT: prim_root(2, 7): 0

    printf("prim_root(3, 7): %d\n", is_primitive_root(3, 7));
    // EXPECT: prim_root(3, 7): 1

    printf("prim_root(2, 13): %d\n", is_primitive_root(2, 13));
    // EXPECT: prim_root(2, 13): 1

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
