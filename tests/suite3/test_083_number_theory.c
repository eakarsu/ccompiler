int printf(const char *fmt, ...);

/* Modular addition: (a + b) mod m */
int mod_add(int a, int b, int m) {
    return ((a % m) + (b % m)) % m;
}

/* Modular multiplication: (a * b) mod m */
int mod_mul(int a, int b, int m) {
    int result = 0;
    a = a % m;
    while (b > 0) {
        if (b & 1) result = (result + a) % m;
        a = (a * 2) % m;
        b >>= 1;
    }
    return result;
}

/* Modular exponentiation: base^exp mod m */
int mod_pow(int base, int exp, int m) {
    int result = 1;
    base = base % m;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % m;
        base = (base * base) % m;
        exp >>= 1;
    }
    return result;
}

/* GCD via Euclidean algorithm */
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Extended GCD: returns gcd, sets *x and *y such that a*x + b*y = gcd */
int ext_gcd(int a, int b, int *x, int *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int x1, y1;
    int g = ext_gcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

/* Modular inverse of a mod m (m must be prime or gcd(a,m)=1) */
int mod_inv(int a, int m) {
    int x, y;
    int g = ext_gcd(a, m, &x, &y);
    if (g != 1) return -1; /* no inverse */
    return (x % m + m) % m;
}

/* Chinese Remainder Theorem: solve x = r1 mod m1, x = r2 mod m2
   Requires gcd(m1, m2) = 1. Returns smallest non-negative solution. */
int crt2(int r1, int m1, int r2, int m2) {
    int x, y;
    ext_gcd(m1, m2, &x, &y);
    /* x = m1^{-1} mod m2, y = m2^{-1} mod m1 */
    int mod = m1 * m2;
    int result = (r1 * m2 % mod * y % mod + r2 * m1 % mod * x % mod) % mod;
    return (result + mod) % mod;
}

/* Euler's totient function phi(n) */
int euler_phi(int n) {
    int result = n;
    int p;
    int temp = n;
    for (p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1) result -= result / temp;
    return result;
}

/* Check if n is a perfect number */
int is_perfect(int n) {
    if (n < 2) return 0;
    int sum = 1, i;
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            sum += i;
            if (i != n / i) sum += n / i;
        }
    }
    return sum == n;
}

/* Number of divisors of n */
int num_divisors(int n) {
    int count = 0, i;
    for (i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            count++;
            if (i != n / i) count++;
        }
    }
    return count;
}

/* Sum of divisors of n */
int sum_divisors(int n) {
    int s = 0, i;
    for (i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            s += i;
            if (i != n / i) s += n / i;
        }
    }
    return s;
}

int main(void) {
    int x, y, g;

    /* mod_add */
    printf("%d\n", mod_add(7, 8, 5));   // EXPECT: 0
    printf("%d\n", mod_add(3, 4, 7));   // EXPECT: 0
    printf("%d\n", mod_add(10, 15, 7)); // EXPECT: 4

    /* mod_pow */
    printf("%d\n", mod_pow(2, 10, 1000)); // EXPECT: 24
    printf("%d\n", mod_pow(3, 4, 5));     // EXPECT: 1
    /* 3^4 = 81, 81 mod 5 = 1 */
    printf("%d\n", mod_pow(2, 0, 7));     // EXPECT: 1
    printf("%d\n", mod_pow(5, 3, 13));    // EXPECT: 8
    /* 5^3 = 125, 125 mod 13 = 8 */

    /* gcd */
    printf("%d\n", gcd(48, 18));  // EXPECT: 6
    printf("%d\n", gcd(100, 75)); // EXPECT: 25
    printf("%d\n", gcd(1, 1));    // EXPECT: 1

    /* ext_gcd */
    /* gcd(35, 15): 35*1 + 15*(-2) = 5 */
    g = ext_gcd(35, 15, &x, &y);
    printf("%d\n", g);              // EXPECT: 5
    printf("%d\n", 35*x + 15*y);   // EXPECT: 5

    /* ext_gcd(3, 7): 3*5 + 7*(-2) = 1 */
    g = ext_gcd(3, 7, &x, &y);
    printf("%d\n", g);             // EXPECT: 1
    printf("%d\n", 3*x + 7*y);    // EXPECT: 1

    /* mod_inv */
    /* inv(3) mod 7 = 5 because 3*5=15, 15 mod 7=1 */
    printf("%d\n", mod_inv(3, 7));   // EXPECT: 5
    /* inv(4) mod 11 = 3 because 4*3=12, 12 mod 11=1 */
    printf("%d\n", mod_inv(4, 11));  // EXPECT: 3
    /* inv(2) mod 9 = 5 because 2*5=10, 10 mod 9=1 */
    printf("%d\n", mod_inv(2, 9));   // EXPECT: 5

    /* CRT: x=2 mod 3, x=3 mod 5 => x=8 (8%3=2, 8%5=3) */
    printf("%d\n", crt2(2, 3, 3, 5));   // EXPECT: 8
    /* CRT: x=1 mod 4, x=2 mod 7 => x=9 (9%4=1, 9%7=2) */
    printf("%d\n", crt2(1, 4, 2, 7));   // EXPECT: 9

    /* euler_phi */
    printf("%d\n", euler_phi(1));   // EXPECT: 1
    printf("%d\n", euler_phi(6));   // EXPECT: 2
    printf("%d\n", euler_phi(9));   // EXPECT: 6
    printf("%d\n", euler_phi(10));  // EXPECT: 4

    /* is_perfect */
    printf("%d\n", is_perfect(6));    // EXPECT: 1
    printf("%d\n", is_perfect(28));   // EXPECT: 1
    printf("%d\n", is_perfect(12));   // EXPECT: 0

    /* num_divisors */
    printf("%d\n", num_divisors(12));  // EXPECT: 6
    printf("%d\n", num_divisors(1));   // EXPECT: 1
    printf("%d\n", num_divisors(7));   // EXPECT: 2

    /* sum_divisors */
    printf("%d\n", sum_divisors(6));   // EXPECT: 12
    /* 1+2+3+6 = 12 */
    printf("%d\n", sum_divisors(12));  // EXPECT: 28
    /* 1+2+3+4+6+12 = 28 */

    return 0;
}
