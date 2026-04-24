// EXPECT: === Arithmetic Series ===\nSum 1 to 100: 5050\nSum 1 to 50: 1275\nAP(2,3,10): 2 5 8 11 14 17 20 23 26 29\nAP sum: 155\n=== Geometric Series (integer approx) ===\nGP(1,2,10): 1 2 4 8 16 32 64 128 256 512\nGP sum: 1023\nGP(3,3,6): 3 9 27 81 243 729\nGP sum: 1092\n=== Harmonic (scaled x1000) ===\nH(1)=1000 H(2)=1500 H(3)=1833 H(4)=2083 H(5)=2283\nH(10)=2927\n=== Fibonacci ===\nFib: 0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 610 987 1597 2584 4181\nFib(20): 6765\n=== Tribonacci ===\nTrib: 0 0 1 1 2 4 7 13 24 44 81 149 274 504 927 1705\n=== Lucas Numbers ===\nLucas: 2 1 3 4 7 11 18 29 47 76 123 199 322 521 843 1364\n=== Catalan Numbers ===\nC(0)=1 C(1)=1 C(2)=2 C(3)=5 C(4)=14 C(5)=42 C(6)=132 C(7)=429 C(8)=1430 C(9)=4862\n=== Bell Numbers ===\nB(0)=1 B(1)=1 B(2)=2 B(3)=5 B(4)=15 B(5)=52 B(6)=203 B(7)=877\n=== Stirling Numbers (2nd kind) ===\nS(0,0)=1\nS(1,1)=1\nS(2,1)=1 S(2,2)=1\nS(3,1)=1 S(3,2)=3 S(3,3)=1\nS(4,1)=1 S(4,2)=7 S(4,3)=6 S(4,4)=1\nS(5,1)=1 S(5,2)=15 S(5,3)=25 S(5,4)=10 S(5,5)=1\n=== Pentagonal Numbers ===\nPent: 1 5 12 22 35 51 70 92 117 145\n=== Triangular Numbers ===\nTri: 1 3 6 10 15 21 28 36 45 55\n=== Square Pyramidal ===\nSP(1)=1 SP(2)=5 SP(3)=14 SP(4)=30 SP(5)=55 SP(10)=385\n=== Power sums ===\nSum of squares 1..10: 385\nSum of cubes 1..10: 3025\n=== Collatz ===\nCollatz(27): 111 steps, max=9232\nDone.\n
int printf(const char *fmt, ...);

// ---- Arithmetic Series ----

int arithmetic_sum(int a, int d, int n) {
    // Sum = n/2 * (2a + (n-1)*d)
    return n * (2 * a + (n - 1) * d) / 2;
}

int arithmetic_nth(int a, int d, int n) {
    return a + (n - 1) * d;
}

void print_arithmetic_series(int a, int d, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", a + i * d);
    }
    printf("\n");
}

// ---- Geometric Series (integer) ----

int power_int(int base, int exp) {
    int result;
    result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = result * base;
        }
        base = base * base;
        exp = exp / 2;
    }
    return result;
}

int geometric_nth(int a, int r, int n) {
    return a * power_int(r, n - 1);
}

int geometric_sum(int a, int r, int n) {
    // For r != 1: a * (r^n - 1) / (r - 1)
    int i;
    int sum;
    int term;
    if (r == 1) {
        return a * n;
    }
    sum = 0;
    term = a;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + term;
        term = term * r;
    }
    return sum;
}

void print_geometric_series(int a, int r, int n) {
    int i;
    int term;
    term = a;
    for (i = 0; i < n; i = i + 1) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", term);
        term = term * r;
    }
    printf("\n");
}

// ---- Harmonic Series (scaled by 1000 for integer approx) ----

int harmonic_scaled[128];

void compute_harmonic(int n) {
    int i;
    int sum;
    sum = 0;
    for (i = 1; i <= n; i = i + 1) {
        sum = sum + 1000 / i;
        harmonic_scaled[i] = sum;
    }
}

// ---- Fibonacci ----

int fib_cache[128];

int fibonacci(int n) {
    if (n <= 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    if (fib_cache[n] != 0) {
        return fib_cache[n];
    }
    fib_cache[n] = fibonacci(n - 1) + fibonacci(n - 2);
    return fib_cache[n];
}

void compute_fibonacci_iterative(int *arr, int n) {
    int i;
    arr[0] = 0;
    if (n > 1) {
        arr[1] = 1;
    }
    for (i = 2; i < n; i = i + 1) {
        arr[i] = arr[i - 1] + arr[i - 2];
    }
}

// ---- Tribonacci ----

int tribonacci[128];

void compute_tribonacci(int n) {
    int i;
    tribonacci[0] = 0;
    tribonacci[1] = 0;
    tribonacci[2] = 1;
    for (i = 3; i < n; i = i + 1) {
        tribonacci[i] = tribonacci[i - 1] + tribonacci[i - 2] + tribonacci[i - 3];
    }
}

// ---- Lucas Numbers ----

int lucas[128];

void compute_lucas(int n) {
    int i;
    lucas[0] = 2;
    lucas[1] = 1;
    for (i = 2; i < n; i = i + 1) {
        lucas[i] = lucas[i - 1] + lucas[i - 2];
    }
}

// ---- Catalan Numbers ----
// C(n) = C(2n, n) / (n+1) = sum_{i=0}^{n-1} C(i)*C(n-1-i)

int catalan[64];

void compute_catalan(int n) {
    int i;
    int j;
    catalan[0] = 1;
    for (i = 1; i < n; i = i + 1) {
        catalan[i] = 0;
        for (j = 0; j < i; j = j + 1) {
            catalan[i] = catalan[i] + catalan[j] * catalan[i - 1 - j];
        }
    }
}

// ---- Bell Numbers ----
// B(n) using Bell triangle

int bell_tri[32][32];
int bell[32];

void compute_bell(int n) {
    int i;
    int j;
    bell_tri[0][0] = 1;
    bell[0] = 1;
    for (i = 1; i < n; i = i + 1) {
        bell_tri[i][0] = bell_tri[i - 1][i - 1];
        for (j = 1; j <= i; j = j + 1) {
            bell_tri[i][j] = bell_tri[i][j - 1] + bell_tri[i - 1][j - 1];
        }
        bell[i] = bell_tri[i][0];
    }
}

// ---- Stirling Numbers of the Second Kind ----
// S(n, k) = k * S(n-1, k) + S(n-1, k-1)

int stirling2[16][16];

void compute_stirling2(int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            stirling2[i][j] = 0;
        }
    }
    stirling2[0][0] = 1;
    for (i = 1; i < n; i = i + 1) {
        for (j = 1; j <= i; j = j + 1) {
            stirling2[i][j] = j * stirling2[i - 1][j] + stirling2[i - 1][j - 1];
        }
    }
}

// ---- Pentagonal Numbers ----
// P(n) = n(3n-1)/2

int pentagonal(int n) {
    return n * (3 * n - 1) / 2;
}

// ---- Triangular Numbers ----
// T(n) = n(n+1)/2

int triangular(int n) {
    return n * (n + 1) / 2;
}

// ---- Square Pyramidal Numbers ----
// SP(n) = n(n+1)(2n+1)/6

int square_pyramidal(int n) {
    return n * (n + 1) * (2 * n + 1) / 6;
}

// ---- Power sums ----

int sum_of_squares(int n) {
    return n * (n + 1) * (2 * n + 1) / 6;
}

int sum_of_cubes(int n) {
    // (n(n+1)/2)^2
    int t;
    t = n * (n + 1) / 2;
    return t * t;
}

int sum_of_powers(int n, int p) {
    int i;
    int sum;
    sum = 0;
    for (i = 1; i <= n; i = i + 1) {
        sum = sum + power_int(i, p);
    }
    return sum;
}

// ---- Collatz sequence ----

int collatz_steps(int n) {
    int steps;
    steps = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        steps = steps + 1;
    }
    return steps;
}

int collatz_max(int n) {
    int mx;
    mx = n;
    while (n != 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        if (n > mx) {
            mx = n;
        }
    }
    return mx;
}

// ---- Bernoulli numbers (scaled) - using Akiyama-Tanigawa ----

int bernoulli_num[16];
int bernoulli_den[16];

void compute_bernoulli_approx(int n) {
    // Simplified: just store B(0)=1, B(1)=-1/2, etc. as scaled ints
    // This is complex with exact fractions; we'll just store numerators for reference
    int i;
    for (i = 0; i < n; i = i + 1) {
        bernoulli_num[i] = 0;
        bernoulli_den[i] = 1;
    }
    bernoulli_num[0] = 1;
    bernoulli_den[0] = 1;
}

// ---- Partition numbers (number of integer partitions) ----

int partition_dp[128];

void compute_partitions(int n) {
    int i;
    int j;
    partition_dp[0] = 1;
    for (i = 1; i <= n; i = i + 1) {
        partition_dp[i] = 0;
    }
    for (i = 1; i <= n; i = i + 1) {
        for (j = i; j <= n; j = j + 1) {
            partition_dp[j] = partition_dp[j] + partition_dp[j - i];
        }
    }
}

// ---- Euler's totient function ----

int euler_totient(int n) {
    int result;
    int p;
    result = n;
    for (p = 2; p * p <= n; p = p + 1) {
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

// ---- Moebius function ----

int moebius(int n) {
    int p;
    int count;
    count = 0;
    for (p = 2; p * p <= n; p = p + 1) {
        if (n % p == 0) {
            n = n / p;
            count = count + 1;
            if (n % p == 0) {
                return 0; // p^2 divides n
            }
        }
    }
    if (n > 1) {
        count = count + 1;
    }
    if (count % 2 == 0) {
        return 1;
    }
    return -1;
}

// ---- GCD and LCM ----

int gcd(int a, int b) {
    while (b != 0) {
        int t;
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int lcm(int a, int b) {
    return a / gcd(a, b) * b;
}

// ---- Binomial coefficients ----

int binom[32][32];

void compute_binomial(int n) {
    int i;
    int j;
    for (i = 0; i <= n; i = i + 1) {
        binom[i][0] = 1;
        for (j = 1; j <= i; j = j + 1) {
            binom[i][j] = binom[i - 1][j - 1] + binom[i - 1][j];
        }
        for (j = i + 1; j <= n; j = j + 1) {
            binom[i][j] = 0;
        }
    }
}

// ---- Derangement count ----
// D(n) = (n-1) * (D(n-1) + D(n-2))

int derangement(int n) {
    int d0;
    int d1;
    int d2;
    int i;
    if (n == 0) {
        return 1;
    }
    if (n == 1) {
        return 0;
    }
    d0 = 1;
    d1 = 0;
    for (i = 2; i <= n; i = i + 1) {
        d2 = (i - 1) * (d1 + d0);
        d0 = d1;
        d1 = d2;
    }
    return d1;
}

// ---- Pell numbers ----
// P(0)=0, P(1)=1, P(n)=2*P(n-1)+P(n-2)

int pell[64];

void compute_pell(int n) {
    int i;
    pell[0] = 0;
    pell[1] = 1;
    for (i = 2; i < n; i = i + 1) {
        pell[i] = 2 * pell[i - 1] + pell[i - 2];
    }
}

int main() {
    int i;
    int fibs[20];
    int sum;

    // Arithmetic Series
    printf("=== Arithmetic Series ===\n");
    printf("Sum 1 to 100: %d\n", arithmetic_sum(1, 1, 100));
    printf("Sum 1 to 50: %d\n", arithmetic_sum(1, 1, 50));
    printf("AP(2,3,10): ");
    print_arithmetic_series(2, 3, 10);
    printf("AP sum: %d\n", arithmetic_sum(2, 3, 10));

    // Geometric Series
    printf("=== Geometric Series (integer approx) ===\n");
    printf("GP(1,2,10): ");
    print_geometric_series(1, 2, 10);
    printf("GP sum: %d\n", geometric_sum(1, 2, 10));
    printf("GP(3,3,6): ");
    print_geometric_series(3, 3, 6);
    printf("GP sum: %d\n", geometric_sum(3, 3, 6));

    // Harmonic
    printf("=== Harmonic (scaled x1000) ===\n");
    compute_harmonic(10);
    printf("H(1)=%d H(2)=%d H(3)=%d H(4)=%d H(5)=%d\n",
        harmonic_scaled[1], harmonic_scaled[2], harmonic_scaled[3],
        harmonic_scaled[4], harmonic_scaled[5]);
    printf("H(10)=%d\n", harmonic_scaled[10]);

    // Fibonacci
    printf("=== Fibonacci ===\n");
    compute_fibonacci_iterative(fibs, 20);
    printf("Fib: ");
    for (i = 0; i < 20; i = i + 1) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", fibs[i]);
    }
    printf("\n");
    // Reset cache
    for (i = 0; i < 128; i = i + 1) {
        fib_cache[i] = 0;
    }
    printf("Fib(20): %d\n", fibonacci(20));

    // Tribonacci
    printf("=== Tribonacci ===\n");
    compute_tribonacci(16);
    printf("Trib: ");
    for (i = 0; i < 16; i = i + 1) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", tribonacci[i]);
    }
    printf("\n");

    // Lucas
    printf("=== Lucas Numbers ===\n");
    compute_lucas(16);
    printf("Lucas: ");
    for (i = 0; i < 16; i = i + 1) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", lucas[i]);
    }
    printf("\n");

    // Catalan
    printf("=== Catalan Numbers ===\n");
    compute_catalan(10);
    for (i = 0; i < 10; i = i + 1) {
        printf("C(%d)=%d", i, catalan[i]);
        if (i < 9) {
            printf(" ");
        }
    }
    printf("\n");

    // Bell
    printf("=== Bell Numbers ===\n");
    compute_bell(8);
    for (i = 0; i < 8; i = i + 1) {
        printf("B(%d)=%d", i, bell[i]);
        if (i < 7) {
            printf(" ");
        }
    }
    printf("\n");

    // Stirling
    printf("=== Stirling Numbers (2nd kind) ===\n");
    compute_stirling2(6);
    printf("S(0,0)=%d\n", stirling2[0][0]);
    printf("S(1,1)=%d\n", stirling2[1][1]);
    printf("S(2,1)=%d S(2,2)=%d\n", stirling2[2][1], stirling2[2][2]);
    printf("S(3,1)=%d S(3,2)=%d S(3,3)=%d\n", stirling2[3][1], stirling2[3][2], stirling2[3][3]);
    printf("S(4,1)=%d S(4,2)=%d S(4,3)=%d S(4,4)=%d\n",
        stirling2[4][1], stirling2[4][2], stirling2[4][3], stirling2[4][4]);
    printf("S(5,1)=%d S(5,2)=%d S(5,3)=%d S(5,4)=%d S(5,5)=%d\n",
        stirling2[5][1], stirling2[5][2], stirling2[5][3], stirling2[5][4], stirling2[5][5]);

    // Pentagonal
    printf("=== Pentagonal Numbers ===\n");
    printf("Pent: ");
    for (i = 1; i <= 10; i = i + 1) {
        if (i > 1) {
            printf(" ");
        }
        printf("%d", pentagonal(i));
    }
    printf("\n");

    // Triangular
    printf("=== Triangular Numbers ===\n");
    printf("Tri: ");
    for (i = 1; i <= 10; i = i + 1) {
        if (i > 1) {
            printf(" ");
        }
        printf("%d", triangular(i));
    }
    printf("\n");

    // Square pyramidal
    printf("=== Square Pyramidal ===\n");
    printf("SP(1)=%d SP(2)=%d SP(3)=%d SP(4)=%d SP(5)=%d SP(10)=%d\n",
        square_pyramidal(1), square_pyramidal(2), square_pyramidal(3),
        square_pyramidal(4), square_pyramidal(5), square_pyramidal(10));

    // Power sums
    printf("=== Power sums ===\n");
    printf("Sum of squares 1..10: %d\n", sum_of_squares(10));
    printf("Sum of cubes 1..10: %d\n", sum_of_cubes(10));

    // Collatz
    printf("=== Collatz ===\n");
    printf("Collatz(27): %d steps, max=%d\n", collatz_steps(27), collatz_max(27));

    printf("Done.\n");
    return 0;
}
