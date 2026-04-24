int printf(const char *fmt, ...);
// EXPECT: is_prime 1: 0\nis_prime 2: 1\nis_prime 3: 1\nis_prime 4: 0\nis_prime 7: 1\nis_prime 10: 0\nis_prime 13: 1\nis_prime 17: 1\nis_prime 25: 0\nis_prime 29: 1\nis_prime 97: 1\nprimes to 10: 4\nprimes to 20: 8\nprimes to 50: 15\nprimes to 100: 25\ngcd 12 8: 4\ngcd 54 24: 6\ngcd 7 13: 1\ngcd 100 75: 25\nlcm 4 6: 12\nlcm 12 18: 36\nlcm 7 5: 35\nmod_exp 2 10 1000: 24\nmod_exp 3 7 100: 87\nmod_exp 5 3 13: 8\ntotient 1: 1\ntotient 6: 2\ntotient 10: 4\ntotient 12: 4\nfactors of 60: 3\n  2^2\n  3^1\n  5^1\nfactors of 100: 2\n  2^2\n  5^2\ndivisors 12: 6\nsum div 12: 28\ndivisors 28: 6\nsum div 28: 56\nperfect 6: 1\nperfect 28: 1\nperfect 12: 0\nfib 0: 0\nfib 1: 1\nfib 5: 5\nfib 10: 55\npalindrome 121: 1\npalindrome 123: 0\npalindrome 1221: 1\ndigit sum 12345: 15\ndigit count 12345: 5\ndigit sum 999: 27\ncoprime 8 15: 1\ncoprime 8 12: 0\nperfect sq 16: 1\nperfect sq 15: 0\nisqrt 17: 4\next gcd 35 15: 5\next x: 1\next y: -2\npow 2 0: 1\npow 2 8: 256\npow 3 4: 81\nsieve 2: 1\nsieve 9: 0\nsieve 11: 1\nsieve 15: 0\nsieve 23: 1\nsieve 49: 0

// ============================================================
// test_014_number_theory.c
// Number theory algorithms
// ============================================================

// -----------------------------------------------------------
// Check if n is prime
// -----------------------------------------------------------
int is_prime(int n) {
    int i;
    if (n <= 1) {
        return 0;
    }
    if (n <= 3) {
        return 1;
    }
    if (n % 2 == 0) {
        return 0;
    }
    i = 3;
    while (i * i <= n) {
        if (n % i == 0) {
            return 0;
        }
        i = i + 2;
    }
    return 1;
}

// -----------------------------------------------------------
// Sieve of Eratosthenes up to limit
// sieve[i] = 1 means i is prime
// -----------------------------------------------------------
int sieve[200];

void do_sieve(int limit) {
    int i;
    int j;
    for (i = 0; i < 200; i = i + 1) {
        sieve[i] = 1;
    }
    sieve[0] = 0;
    sieve[1] = 0;
    i = 2;
    while (i * i <= limit) {
        if (sieve[i]) {
            j = i * i;
            while (j <= limit) {
                sieve[j] = 0;
                j = j + i;
            }
        }
        i = i + 1;
    }
}

// -----------------------------------------------------------
// Count primes up to n using sieve
// -----------------------------------------------------------
int count_primes(int n) {
    int i;
    int cnt;
    cnt = 0;
    for (i = 2; i <= n; i = i + 1) {
        if (sieve[i]) {
            cnt = cnt + 1;
        }
    }
    return cnt;
}

// -----------------------------------------------------------
// GCD using Euclidean algorithm
// -----------------------------------------------------------
int gcd(int a, int b) {
    int t;
    while (b != 0) {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// -----------------------------------------------------------
// LCM
// -----------------------------------------------------------
int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;
}

// -----------------------------------------------------------
// Modular exponentiation: (base^exp) % mod
// -----------------------------------------------------------
int mod_exp(int base, int exp, int mod) {
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

// -----------------------------------------------------------
// Euler's totient function
// -----------------------------------------------------------
int euler_totient(int n) {
    int result;
    int p;
    result = n;
    p = 2;
    while (p * p <= n) {
        if (n % p == 0) {
            while (n % p == 0) {
                n = n / p;
            }
            result = result - result / p;
        }
        p = p + 1;
    }
    if (n > 1) {
        result = result - result / n;
    }
    return result;
}

// -----------------------------------------------------------
// Prime factorization: store factors and exponents
// Returns number of distinct factors
// -----------------------------------------------------------
int factors[20];
int exponents[20];

int prime_factorize(int n) {
    int cnt;
    int p;
    int e;
    cnt = 0;
    p = 2;
    while (p * p <= n) {
        if (n % p == 0) {
            e = 0;
            while (n % p == 0) {
                n = n / p;
                e = e + 1;
            }
            factors[cnt] = p;
            exponents[cnt] = e;
            cnt = cnt + 1;
        }
        p = p + 1;
    }
    if (n > 1) {
        factors[cnt] = n;
        exponents[cnt] = 1;
        cnt = cnt + 1;
    }
    return cnt;
}

// -----------------------------------------------------------
// Count divisors of n
// -----------------------------------------------------------
int count_divisors(int n) {
    int i;
    int cnt;
    cnt = 0;
    for (i = 1; i <= n; i = i + 1) {
        if (n % i == 0) {
            cnt = cnt + 1;
        }
    }
    return cnt;
}

// -----------------------------------------------------------
// Sum of divisors of n
// -----------------------------------------------------------
int sum_divisors(int n) {
    int i;
    int s;
    s = 0;
    for (i = 1; i <= n; i = i + 1) {
        if (n % i == 0) {
            s = s + i;
        }
    }
    return s;
}

// -----------------------------------------------------------
// Check if n is a perfect number (sum of proper divisors == n)
// -----------------------------------------------------------
int is_perfect(int n) {
    int s;
    s = sum_divisors(n) - n;
    if (s == n) {
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------
// Fibonacci
// -----------------------------------------------------------
int fibonacci(int n) {
    int a;
    int b;
    int c;
    int i;
    if (n <= 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    a = 0;
    b = 1;
    for (i = 2; i <= n; i = i + 1) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

// -----------------------------------------------------------
// Power function (integer)
// -----------------------------------------------------------
int int_pow(int base, int exp) {
    int result;
    result = 1;
    while (exp > 0) {
        result = result * base;
        exp = exp - 1;
    }
    return result;
}

// -----------------------------------------------------------
// Check if number is a palindrome
// -----------------------------------------------------------
int is_palindrome(int n) {
    int rev;
    int orig;
    rev = 0;
    orig = n;
    while (n > 0) {
        rev = rev * 10 + n % 10;
        n = n / 10;
    }
    if (rev == orig) {
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------
// Sum of digits
// -----------------------------------------------------------
int digit_sum(int n) {
    int s;
    s = 0;
    if (n < 0) {
        n = -n;
    }
    while (n > 0) {
        s = s + n % 10;
        n = n / 10;
    }
    return s;
}

// -----------------------------------------------------------
// Number of digits
// -----------------------------------------------------------
int digit_count(int n) {
    int c;
    if (n == 0) {
        return 1;
    }
    if (n < 0) {
        n = -n;
    }
    c = 0;
    while (n > 0) {
        c = c + 1;
        n = n / 10;
    }
    return c;
}

// -----------------------------------------------------------
// Check if two numbers are coprime
// -----------------------------------------------------------
int are_coprime(int a, int b) {
    if (gcd(a, b) == 1) {
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------
// Absolute value
// -----------------------------------------------------------
int abs_val(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

// -----------------------------------------------------------
// Integer square root (floor)
// -----------------------------------------------------------
int isqrt(int n) {
    int x;
    if (n < 0) {
        return -1;
    }
    if (n == 0) {
        return 0;
    }
    x = 1;
    while (x * x <= n) {
        x = x + 1;
    }
    return x - 1;
}

// -----------------------------------------------------------
// Check if n is a perfect square
// -----------------------------------------------------------
int is_perfect_square(int n) {
    int s;
    s = isqrt(n);
    if (s * s == n) {
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------
// Extended GCD: returns gcd and sets x, y such that
// a*x + b*y = gcd(a,b)
// We'll return gcd and store x,y in globals
// -----------------------------------------------------------
int ext_gcd_x;
int ext_gcd_y;

int extended_gcd(int a, int b) {
    int g;
    int x1;
    int y1;
    if (a == 0) {
        ext_gcd_x = 0;
        ext_gcd_y = 1;
        return b;
    }
    g = extended_gcd(b % a, a);
    x1 = ext_gcd_x;
    y1 = ext_gcd_y;
    ext_gcd_x = y1 - (b / a) * x1;
    ext_gcd_y = x1;
    return g;
}

// -----------------------------------------------------------
// MAIN
// -----------------------------------------------------------
int main(void) {
    int i;
    int n;
    int r;

    // Prime checking
    printf("is_prime 1: %d\n", is_prime(1));
    printf("is_prime 2: %d\n", is_prime(2));
    printf("is_prime 3: %d\n", is_prime(3));
    printf("is_prime 4: %d\n", is_prime(4));
    printf("is_prime 7: %d\n", is_prime(7));
    printf("is_prime 10: %d\n", is_prime(10));
    printf("is_prime 13: %d\n", is_prime(13));
    printf("is_prime 17: %d\n", is_prime(17));
    printf("is_prime 25: %d\n", is_prime(25));
    printf("is_prime 29: %d\n", is_prime(29));
    printf("is_prime 97: %d\n", is_prime(97));

    // Sieve
    do_sieve(100);
    printf("primes to 10: %d\n", count_primes(10));
    printf("primes to 20: %d\n", count_primes(20));
    printf("primes to 50: %d\n", count_primes(50));
    printf("primes to 100: %d\n", count_primes(100));

    // GCD
    printf("gcd 12 8: %d\n", gcd(12, 8));
    printf("gcd 54 24: %d\n", gcd(54, 24));
    printf("gcd 7 13: %d\n", gcd(7, 13));
    printf("gcd 100 75: %d\n", gcd(100, 75));

    // LCM
    printf("lcm 4 6: %d\n", lcm(4, 6));
    printf("lcm 12 18: %d\n", lcm(12, 18));
    printf("lcm 7 5: %d\n", lcm(7, 5));

    // Modular exponentiation
    printf("mod_exp 2 10 1000: %d\n", mod_exp(2, 10, 1000));
    printf("mod_exp 3 7 100: %d\n", mod_exp(3, 7, 100));
    printf("mod_exp 5 3 13: %d\n", mod_exp(5, 3, 13));

    // Euler totient
    printf("totient 1: %d\n", euler_totient(1));
    printf("totient 6: %d\n", euler_totient(6));
    printf("totient 10: %d\n", euler_totient(10));
    printf("totient 12: %d\n", euler_totient(12));

    // Prime factorization of 60 = 2^2 * 3 * 5
    n = prime_factorize(60);
    printf("factors of 60: %d\n", n);
    for (i = 0; i < n; i = i + 1) {
        printf("  %d^%d\n", factors[i], exponents[i]);
    }

    // Prime factorization of 100 = 2^2 * 5^2
    n = prime_factorize(100);
    printf("factors of 100: %d\n", n);
    for (i = 0; i < n; i = i + 1) {
        printf("  %d^%d\n", factors[i], exponents[i]);
    }

    // Count and sum of divisors
    printf("divisors 12: %d\n", count_divisors(12));
    printf("sum div 12: %d\n", sum_divisors(12));
    printf("divisors 28: %d\n", count_divisors(28));
    printf("sum div 28: %d\n", sum_divisors(28));

    // Perfect numbers
    printf("perfect 6: %d\n", is_perfect(6));
    printf("perfect 28: %d\n", is_perfect(28));
    printf("perfect 12: %d\n", is_perfect(12));

    // Fibonacci
    printf("fib 0: %d\n", fibonacci(0));
    printf("fib 1: %d\n", fibonacci(1));
    printf("fib 5: %d\n", fibonacci(5));
    printf("fib 10: %d\n", fibonacci(10));

    // Palindrome
    printf("palindrome 121: %d\n", is_palindrome(121));
    printf("palindrome 123: %d\n", is_palindrome(123));
    printf("palindrome 1221: %d\n", is_palindrome(1221));

    // Digit sum and count
    printf("digit sum 12345: %d\n", digit_sum(12345));
    printf("digit count 12345: %d\n", digit_count(12345));
    printf("digit sum 999: %d\n", digit_sum(999));

    // Coprime
    printf("coprime 8 15: %d\n", are_coprime(8, 15));
    printf("coprime 8 12: %d\n", are_coprime(8, 12));

    // Perfect square
    printf("perfect sq 16: %d\n", is_perfect_square(16));
    printf("perfect sq 15: %d\n", is_perfect_square(15));
    printf("isqrt 17: %d\n", isqrt(17));

    // -----------------------------------------------------------
    // Extended GCD tests
    // -----------------------------------------------------------
    r = extended_gcd(35, 15);
    printf("ext gcd 35 15: %d\n", r);
    printf("ext x: %d\n", ext_gcd_x);
    printf("ext y: %d\n", ext_gcd_y);

    // -----------------------------------------------------------
    // Integer power tests
    // -----------------------------------------------------------
    printf("pow 2 0: %d\n", int_pow(2, 0));
    printf("pow 2 8: %d\n", int_pow(2, 8));
    printf("pow 3 4: %d\n", int_pow(3, 4));

    // -----------------------------------------------------------
    // More prime tests from sieve
    // -----------------------------------------------------------
    printf("sieve 2: %d\n", sieve[2]);
    printf("sieve 9: %d\n", sieve[9]);
    printf("sieve 11: %d\n", sieve[11]);
    printf("sieve 15: %d\n", sieve[15]);
    printf("sieve 23: %d\n", sieve[23]);
    printf("sieve 49: %d\n", sieve[49]);

    return 0;
}
