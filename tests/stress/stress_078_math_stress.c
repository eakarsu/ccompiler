// stress_078_math_stress.c - Math computation stress
// EXPECT: pow_2_10_mod1000=24
// EXPECT: pow_3_13_mod100=23
// EXPECT: pow_7_7_mod1000=543
// EXPECT: ext_gcd_35_15=5
// EXPECT: ext_gcd_verify=1
// EXPECT: ext_gcd_120_23=1
// EXPECT: ext_gcd_verify2=1
// EXPECT: modinv_3_mod7=5
// EXPECT: modinv_verify=1
// EXPECT: modinv_10_mod17=12
// EXPECT: modinv_no_inverse=-1
// EXPECT: crt_2_3_3_5=8
// EXPECT: crt_1_4_2_3=5
// EXPECT: primes_to_100=25
// EXPECT: first_prime=2
// EXPECT: last_prime=97
// EXPECT: sum_primes=1060
// EXPECT: mcm_4=5000
// EXPECT: gcd_48_18=6
// EXPECT: lcm_12_8=24
// EXPECT: totient_12=4
// EXPECT: totient_7=6
// EXPECT: totient_100=40

int printf(const char *fmt, ...);

// Fast exponentiation: base^exp mod m
int power_mod(int base, int exp, int m) {
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

// Extended GCD: returns gcd, sets *x and *y such that a*x + b*y = gcd
int ext_gcd(int a, int b, int *x, int *y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
    int x1;
    int y1;
    int g = ext_gcd(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return g;
}

// Modular inverse: a^(-1) mod m using extended GCD
int mod_inverse(int a, int m) {
    int x;
    int y;
    int g = ext_gcd(a, m, &x, &y);
    if (g != 1) return -1; // No inverse
    return ((x % m) + m) % m;
}

// Chinese Remainder Theorem for 2 equations:
// x = a1 (mod m1), x = a2 (mod m2)
// Returns x mod (m1*m2)
int crt2(int a1, int m1, int a2, int m2) {
    int x;
    int y;
    int g = ext_gcd(m1, m2, &x, &y);
    if ((a2 - a1) % g != 0) return -1;
    int lcm = m1 * (m2 / g);
    int diff = a2 - a1;
    int mul = diff / g;
    int result = a1 + m1 * ((mul * x) % (m2 / g));
    result = ((result % lcm) + lcm) % lcm;
    return result;
}

// Sieve of Eratosthenes
int primes[101];
int is_prime_arr[101];

int sieve(int limit) {
    int i;
    int j;
    for (i = 0; i <= limit; i++) is_prime_arr[i] = 1;
    is_prime_arr[0] = 0;
    is_prime_arr[1] = 0;
    for (i = 2; i * i <= limit; i++) {
        if (is_prime_arr[i]) {
            for (j = i * i; j <= limit; j += i) {
                is_prime_arr[j] = 0;
            }
        }
    }
    int count = 0;
    for (i = 2; i <= limit; i++) {
        if (is_prime_arr[i]) primes[count++] = i;
    }
    return count;
}

// Matrix chain multiplication (minimum multiplications)
// dims[i] is dimensions: matrix i is dims[i] x dims[i+1]
int mcm(int *dims, int n) {
    int dp[10][10];
    int i;
    int j;
    int k;
    int l;
    for (i = 0; i < n; i++) dp[i][i] = 0;
    for (l = 2; l <= n; l++) {
        for (i = 0; i <= n - l; i++) {
            j = i + l - 1;
            dp[i][j] = 999999999;
            for (k = i; k < j; k++) {
                int cost = dp[i][k] + dp[k + 1][j] + dims[i] * dims[k + 1] * dims[j + 1];
                if (cost < dp[i][j]) dp[i][j] = cost;
            }
        }
    }
    return dp[0][n - 1];
}

// GCD (simple)
int gcd(int a, int b) {
    while (b) { int t = b; b = a % b; a = t; }
    return a;
}

// LCM
int lcm(int a, int b) {
    return a / gcd(a, b) * b;
}

// Euler's totient function
int euler_totient(int n) {
    int result = n;
    int p;
    for (p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

int main(void) {
    // Fast exponentiation
    printf("pow_2_10_mod1000=%d\n", power_mod(2, 10, 1000));
    printf("pow_3_13_mod100=%d\n", power_mod(3, 13, 100));
    printf("pow_7_7_mod1000=%d\n", power_mod(7, 7, 1000));

    // Extended GCD
    int x;
    int y;
    int g = ext_gcd(35, 15, &x, &y);
    printf("ext_gcd_35_15=%d\n", g);
    printf("ext_gcd_verify=%d\n", 35 * x + 15 * y == g);

    g = ext_gcd(120, 23, &x, &y);
    printf("ext_gcd_120_23=%d\n", g);
    printf("ext_gcd_verify2=%d\n", 120 * x + 23 * y == g);

    // Modular inverse
    printf("modinv_3_mod7=%d\n", mod_inverse(3, 7));
    printf("modinv_verify=%d\n", (3 * mod_inverse(3, 7)) % 7);
    printf("modinv_10_mod17=%d\n", mod_inverse(10, 17));
    printf("modinv_no_inverse=%d\n", mod_inverse(6, 9)); // gcd=3, no inverse

    // Chinese Remainder Theorem
    // x = 2 mod 3, x = 3 mod 5 => x = 8 mod 15
    printf("crt_2_3_3_5=%d\n", crt2(2, 3, 3, 5));
    // x = 1 mod 4, x = 2 mod 3 => x = 5 mod 12
    printf("crt_1_4_2_3=%d\n", crt2(1, 4, 2, 3));

    // Sieve of Eratosthenes
    int nprimes = sieve(100);
    printf("primes_to_100=%d\n", nprimes);
    printf("first_prime=%d\n", primes[0]);
    printf("last_prime=%d\n", primes[nprimes - 1]);

    // Sum of primes up to 100
    int psum = 0;
    int i;
    for (i = 0; i < nprimes; i++) psum += primes[i];
    printf("sum_primes=%d\n", psum);

    // Matrix chain multiplication
    // 4 matrices: 10x30, 30x5, 5x60, 60x10 => dims = {10,30,5,60,10}
    int dims[] = {10, 30, 5, 60, 10};
    printf("mcm_4=%d\n", mcm(dims, 4));

    // GCD and LCM
    printf("gcd_48_18=%d\n", gcd(48, 18));
    printf("lcm_12_8=%d\n", lcm(12, 8));

    // Euler's totient
    printf("totient_12=%d\n", euler_totient(12));
    printf("totient_7=%d\n", euler_totient(7));
    printf("totient_100=%d\n", euler_totient(100));

    return 0;
}
