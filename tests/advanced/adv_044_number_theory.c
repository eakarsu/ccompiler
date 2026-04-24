// EXPECT: === Number Theory ===
// EXPECT: --- Primality Check ---
// EXPECT: is_prime(0)=0
// EXPECT: is_prime(1)=0
// EXPECT: is_prime(2)=1
// EXPECT: is_prime(3)=1
// EXPECT: is_prime(4)=0
// EXPECT: is_prime(5)=1
// EXPECT: is_prime(9)=0
// EXPECT: is_prime(11)=1
// EXPECT: is_prime(13)=1
// EXPECT: is_prime(15)=0
// EXPECT: is_prime(17)=1
// EXPECT: is_prime(25)=0
// EXPECT: is_prime(29)=1
// EXPECT: is_prime(97)=1
// EXPECT: is_prime(100)=0
// EXPECT: --- Sieve of Eratosthenes (up to 50) ---
// EXPECT: Primes: 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47
// EXPECT: Count: 15
// EXPECT: --- Prime Factorization ---
// EXPECT: factors(1):
// EXPECT: factors(2): 2
// EXPECT: factors(12): 2 2 3
// EXPECT: factors(30): 2 3 5
// EXPECT: factors(60): 2 2 3 5
// EXPECT: factors(97): 97
// EXPECT: factors(100): 2 2 5 5
// EXPECT: factors(360): 2 2 2 3 3 5
// EXPECT: --- Coprime Check ---
// EXPECT: coprime(3,5)=1
// EXPECT: coprime(4,6)=0
// EXPECT: coprime(7,11)=1
// EXPECT: coprime(12,8)=0
// EXPECT: coprime(15,28)=1
// EXPECT: coprime(1,anything)=1
// EXPECT: --- Euler Totient ---
// EXPECT: phi(1)=1
// EXPECT: phi(2)=1
// EXPECT: phi(3)=2
// EXPECT: phi(4)=2
// EXPECT: phi(5)=4
// EXPECT: phi(6)=2
// EXPECT: phi(7)=6
// EXPECT: phi(8)=4
// EXPECT: phi(9)=6
// EXPECT: phi(10)=4
// EXPECT: phi(12)=4
// EXPECT: phi(36)=12
// EXPECT: --- Count Divisors ---
// EXPECT: ndiv(1)=1
// EXPECT: ndiv(6)=4
// EXPECT: ndiv(12)=6
// EXPECT: ndiv(28)=6
// EXPECT: ndiv(36)=9
// EXPECT: --- Sum of Divisors ---
// EXPECT: sigma(1)=1
// EXPECT: sigma(6)=12
// EXPECT: sigma(12)=28
// EXPECT: sigma(28)=56
// EXPECT: --- Perfect Numbers (up to 500) ---
// EXPECT: Perfect: 6 28 496
// EXPECT: === Done ===

int printf(const char *fmt, ...);

int is_prime(int n) {
    if (n < 2) return 0;
    if (n < 4) return 1;
    if (n % 2 == 0) return 0;
    if (n % 3 == 0) return 0;
    int i = 5;
    while (i * i <= n) {
        if (n % i == 0) return 0;
        if (n % (i + 2) == 0) return 0;
        i = i + 6;
    }
    return 1;
}

void sieve(int limit, int *primes, int *count) {
    int sieve_arr[200];
    int i = 0;
    while (i <= limit) {
        sieve_arr[i] = 1;
        i = i + 1;
    }
    sieve_arr[0] = 0;
    sieve_arr[1] = 0;

    i = 2;
    while (i * i <= limit) {
        if (sieve_arr[i]) {
            int j = i * i;
            while (j <= limit) {
                sieve_arr[j] = 0;
                j = j + i;
            }
        }
        i = i + 1;
    }

    *count = 0;
    i = 2;
    while (i <= limit) {
        if (sieve_arr[i]) {
            primes[*count] = i;
            *count = *count + 1;
        }
        i = i + 1;
    }
}

void factorize(int n, int *factors, int *count) {
    *count = 0;
    if (n <= 1) return;
    int d = 2;
    while (d * d <= n) {
        while (n % d == 0) {
            factors[*count] = d;
            *count = *count + 1;
            n = n / d;
        }
        d = d + 1;
    }
    if (n > 1) {
        factors[*count] = n;
        *count = *count + 1;
    }
}

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int coprime(int a, int b) {
    return gcd(a, b) == 1;
}

int euler_totient(int n) {
    if (n <= 1) return n;
    int result = n;
    int p = 2;
    int temp = n;
    while (p * p <= temp) {
        if (temp % p == 0) {
            while (temp % p == 0) {
                temp = temp / p;
            }
            result = result - result / p;
        }
        p = p + 1;
    }
    if (temp > 1) {
        result = result - result / temp;
    }
    return result;
}

int count_divisors(int n) {
    int count = 0;
    int i = 1;
    while (i * i <= n) {
        if (n % i == 0) {
            count = count + 1;
            if (i != n / i) count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int sum_divisors(int n) {
    int sum = 0;
    int i = 1;
    while (i * i <= n) {
        if (n % i == 0) {
            sum = sum + i;
            if (i != n / i) sum = sum + n / i;
        }
        i = i + 1;
    }
    return sum;
}

int main(void) {
    printf("=== Number Theory ===\n");

    printf("--- Primality Check ---\n");
    printf("is_prime(0)=%d\n", is_prime(0));
    printf("is_prime(1)=%d\n", is_prime(1));
    printf("is_prime(2)=%d\n", is_prime(2));
    printf("is_prime(3)=%d\n", is_prime(3));
    printf("is_prime(4)=%d\n", is_prime(4));
    printf("is_prime(5)=%d\n", is_prime(5));
    printf("is_prime(9)=%d\n", is_prime(9));
    printf("is_prime(11)=%d\n", is_prime(11));
    printf("is_prime(13)=%d\n", is_prime(13));
    printf("is_prime(15)=%d\n", is_prime(15));
    printf("is_prime(17)=%d\n", is_prime(17));
    printf("is_prime(25)=%d\n", is_prime(25));
    printf("is_prime(29)=%d\n", is_prime(29));
    printf("is_prime(97)=%d\n", is_prime(97));
    printf("is_prime(100)=%d\n", is_prime(100));

    printf("--- Sieve of Eratosthenes (up to 50) ---\n");
    int primes[50];
    int pcount;
    sieve(50, primes, &pcount);
    printf("Primes:");
    int i = 0;
    while (i < pcount) {
        printf(" %d", primes[i]);
        i = i + 1;
    }
    printf("\n");
    printf("Count: %d\n", pcount);

    printf("--- Prime Factorization ---\n");
    int factors[32];
    int fcount;

    int test_vals[8];
    test_vals[0] = 1;
    test_vals[1] = 2;
    test_vals[2] = 12;
    test_vals[3] = 30;
    test_vals[4] = 60;
    test_vals[5] = 97;
    test_vals[6] = 100;
    test_vals[7] = 360;

    i = 0;
    while (i < 8) {
        factorize(test_vals[i], factors, &fcount);
        printf("factors(%d):", test_vals[i]);
        int j = 0;
        while (j < fcount) {
            printf(" %d", factors[j]);
            j = j + 1;
        }
        printf("\n");
        i = i + 1;
    }

    printf("--- Coprime Check ---\n");
    printf("coprime(3,5)=%d\n", coprime(3, 5));
    printf("coprime(4,6)=%d\n", coprime(4, 6));
    printf("coprime(7,11)=%d\n", coprime(7, 11));
    printf("coprime(12,8)=%d\n", coprime(12, 8));
    printf("coprime(15,28)=%d\n", coprime(15, 28));
    printf("coprime(1,anything)=%d\n", coprime(1, 999));

    printf("--- Euler Totient ---\n");
    printf("phi(1)=%d\n", euler_totient(1));
    printf("phi(2)=%d\n", euler_totient(2));
    printf("phi(3)=%d\n", euler_totient(3));
    printf("phi(4)=%d\n", euler_totient(4));
    printf("phi(5)=%d\n", euler_totient(5));
    printf("phi(6)=%d\n", euler_totient(6));
    printf("phi(7)=%d\n", euler_totient(7));
    printf("phi(8)=%d\n", euler_totient(8));
    printf("phi(9)=%d\n", euler_totient(9));
    printf("phi(10)=%d\n", euler_totient(10));
    printf("phi(12)=%d\n", euler_totient(12));
    printf("phi(36)=%d\n", euler_totient(36));

    printf("--- Count Divisors ---\n");
    printf("ndiv(1)=%d\n", count_divisors(1));
    printf("ndiv(6)=%d\n", count_divisors(6));
    printf("ndiv(12)=%d\n", count_divisors(12));
    printf("ndiv(28)=%d\n", count_divisors(28));
    printf("ndiv(36)=%d\n", count_divisors(36));

    printf("--- Sum of Divisors ---\n");
    printf("sigma(1)=%d\n", sum_divisors(1));
    printf("sigma(6)=%d\n", sum_divisors(6));
    printf("sigma(12)=%d\n", sum_divisors(12));
    printf("sigma(28)=%d\n", sum_divisors(28));

    printf("--- Perfect Numbers (up to 500) ---\n");
    printf("Perfect:");
    i = 2;
    while (i <= 500) {
        if (sum_divisors(i) == 2 * i) {
            printf(" %d", i);
        }
        i = i + 1;
    }
    printf("\n");

    printf("=== Done ===\n");
    return 0;
}
