int printf(const char *fmt, ...);

// Euler's totient function

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Naive totient: count numbers 1..n coprime to n
int totient_naive(int n) {
    int count = 0;
    int i;
    for (i = 1; i <= n; i++) {
        if (gcd(i, n) == 1) {
            count = count + 1;
        }
    }
    return count;
}

// Efficient totient using prime factorization
// phi(n) = n * product(1 - 1/p) for each prime factor p of n
int totient(int n) {
    int result = n;
    int p;
    for (p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) {
                n = n / p;
            }
            result = result - result / p;
        }
    }
    if (n > 1) {
        result = result - result / n;
    }
    return result;
}

// Sum of totient for 1..n
int totient_sum(int n) {
    int sum = 0;
    int i;
    for (i = 1; i <= n; i++) {
        sum = sum + totient(i);
    }
    return sum;
}

// Product of all numbers 1..n coprime to n
int coprime_product_mod(int n, int mod) {
    int product = 1;
    int i;
    for (i = 1; i <= n; i++) {
        if (gcd(i, n) == 1) {
            product = (product * i) % mod;
        }
    }
    return product;
}

// Jordan's totient function J_k(n) - generalization
// J_2(n) = n^2 * product(1 - 1/p^2) for each prime p | n
int jordan_totient_2(int n) {
    int result = n * n;
    int orig = n;
    int p;
    for (p = 2; p * p <= orig; p++) {
        if (orig % p == 0) {
            while (orig % p == 0) {
                orig = orig / p;
            }
            // result *= (1 - 1/p^2) = result * (p^2 - 1) / p^2
            result = (result / (p * p)) * (p * p - 1);
        }
    }
    if (orig > 1) {
        int p2 = orig;
        result = (result / (p2 * p2)) * (p2 * p2 - 1);
    }
    return result;
}

// Verify Euler's theorem: a^phi(n) = 1 (mod n) when gcd(a,n)=1
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

int verify_euler_theorem(int a, int n) {
    if (gcd(a, n) != 1) return -1;
    int phi = totient(n);
    return mod_pow(a, phi, n);
}

// Carmichael function lambda(n) - smallest m such that a^m = 1 (mod n) for all coprime a
int carmichael(int n) {
    if (n <= 2) return 1;
    int m;
    for (m = 1; m < n; m++) {
        int valid = 1;
        int a;
        for (a = 1; a < n; a++) {
            if (gcd(a, n) == 1) {
                if (mod_pow(a, m, n) != 1) {
                    valid = 0;
                    break;
                }
            }
        }
        if (valid) return m;
    }
    return n - 1;
}

int main(void) {
    printf("phi(1): %d\n", totient(1));
    // EXPECT: phi(1): 1

    printf("phi(2): %d\n", totient(2));
    // EXPECT: phi(2): 1

    printf("phi(6): %d\n", totient(6));
    // EXPECT: phi(6): 2

    printf("phi(12): %d\n", totient(12));
    // EXPECT: phi(12): 4

    printf("phi(36): %d\n", totient(36));
    // EXPECT: phi(36): 12

    printf("phi(100): %d\n", totient(100));
    // EXPECT: phi(100): 40

    // Prime: phi(p) = p-1
    printf("phi(97): %d\n", totient(97));
    // EXPECT: phi(97): 96

    // Power of 2: phi(2^k) = 2^(k-1)
    printf("phi(64): %d\n", totient(64));
    // EXPECT: phi(64): 32

    // Verify naive matches efficient
    int match = 1;
    int i;
    for (i = 1; i <= 50; i++) {
        if (totient(i) != totient_naive(i)) {
            match = 0;
        }
    }
    printf("naive matches efficient (1-50): %d\n", match);
    // EXPECT: naive matches efficient (1-50): 1

    // Sum of totients
    // sum phi(i) for i=1..10 = 1+1+2+2+4+2+6+4+6+4 = 32
    int s = totient_sum(10);
    printf("sum phi(1..10): %d\n", s);
    // EXPECT: sum phi(1..10): 32

    // Euler's theorem verification
    printf("3^phi(10) mod 10: %d\n", verify_euler_theorem(3, 10));
    // EXPECT: 3^phi(10) mod 10: 1

    printf("7^phi(15) mod 15: %d\n", verify_euler_theorem(7, 15));
    // EXPECT: 7^phi(15) mod 15: 1

    // Jordan's totient J_2
    // J_2(6) = 36 * (1-1/4) * (1-1/9) = 36 * 3/4 * 8/9 = 24
    printf("J2(6): %d\n", jordan_totient_2(6));
    // EXPECT: J2(6): 24

    // Carmichael function
    printf("lambda(8): %d\n", carmichael(8));
    // EXPECT: lambda(8): 2

    printf("lambda(12): %d\n", carmichael(12));
    // EXPECT: lambda(12): 2

    printf("lambda(15): %d\n", carmichael(15));
    // EXPECT: lambda(15): 4

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
