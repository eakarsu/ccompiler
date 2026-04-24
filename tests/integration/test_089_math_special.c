int printf(const char *fmt, ...);
// EXPECT: === Fibonacci Variations ===\nFib: 0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 610\nTrib: 0 0 1 1 2 4 7 13 24 44 81 149 274\nLucas: 2 1 3 4 7 11 18 29 47 76 123 199 322\nGenFib(2,1): 2 1 3 4 7 11 18 29 47 76 123\nFibId F(5): 1\nFibId F(8): 1\nLucas-Fib L(5): 1\nLucas-Fib L(8): 1\nFibGCD(6,9): 1\nFibGCD(8,12): 1\nFibSum(10): 1\nFibSum(7): 1\nFib sum [0,10]: 143\n=== Pell Numbers ===\nPell: 0 1 2 5 12 29 70 169 408 985 2378\nPellLucas: 2 2 6 14 34 82 198 478 1154\nHalfCompanion: 1 1 3 7 17 41 99 239 577\nPellId P(3): 0\nPellId P(4): 0\nPellId P(5): 0\nPellId P(6): 0\n=== Padovan and Perrin ===\nPadovan: 1 1 1 2 2 3 4 5 7 9 12 16 21 28 37 49\nPerrin: 3 0 2 3 2 5 5 7 10 12 17 22 29 39 51 68\nPerrin prime test 2: 1 (prime=1)\nPerrin prime test 3: 1 (prime=1)\nPerrin prime test 5: 1 (prime=1)\nPerrin prime test 7: 1 (prime=1)\nPerrin prime test 11: 1 (prime=1)\nPadovan sum [0,10]: 47\n=== Look-and-Say ===\nLAS(1): 1\nLAS(1) len: 1\nLAS(2): 11\nLAS(2) len: 2\nLAS(3): 21\nLAS(3) len: 2\nLAS(4): 1211\nLAS(4) len: 4\nLAS(5): 111221\nLAS(5) len: 6\nLAS(6): 312211\nLAS(6) len: 6\nLAS(7): 13112221\nLAS(7) len: 8\nLAS(8): 1113213211\nLAS(8) len: 10\nLAS(7) digit_sum: 13\nLAS(7) distinct: 3\nLAS(8) digit_sum: 16\nLAS(8) distinct: 3\n=== Additional Sequences ===\nCatalan: 1 1 2 5 14 42 132 429 1430 4862 16796\nBell: 1 1 2 5 15 52 203 877 4140\nDerangement: 1 0 1 2 9 44 265 1854 14833\nNarayana N(4,k): 1 6 6 1\nNarayana N(5,k): 1 10 20 10 1\nMotzkin: 1 1 2 4 9 21 51 127 323\nBinomial(6,k): 1 6 15 20 15 6 1\nCatVerify(5): 1\nCatVerify(6): 1
// ============================================================
// test_089_math_special.c
// Special mathematical sequences and variations:
//   1. Fibonacci variations (standard, tribonacci, Lucas)
//   2. Pell numbers and Pell-Lucas numbers
//   3. Padovan sequence and Perrin sequence
//   4. Look-and-say sequence (digits)
//   5. Various sequence properties and relationships
// ============================================================

// ============================================================
// 1. Fibonacci Variations
// ============================================================

// Standard Fibonacci: F(0)=0, F(1)=1, F(n) = F(n-1) + F(n-2)
int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    int a = 0;
    int b = 1;
    int i;
    int c;
    for (i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

// Tribonacci: T(0)=0, T(1)=0, T(2)=1, T(n) = T(n-1)+T(n-2)+T(n-3)
int tribonacci(int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    int a = 0;
    int b = 0;
    int c = 1;
    int i;
    int d;
    for (i = 3; i <= n; i++) {
        d = a + b + c;
        a = b;
        b = c;
        c = d;
    }
    return c;
}

// Lucas numbers: L(0)=2, L(1)=1, L(n) = L(n-1) + L(n-2)
int lucas(int n) {
    if (n == 0) return 2;
    if (n == 1) return 1;
    int a = 2;
    int b = 1;
    int i;
    int c;
    for (i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

// Generalized Fibonacci with custom seeds
int gen_fib(int seed0, int seed1, int n) {
    if (n == 0) return seed0;
    if (n == 1) return seed1;
    int a = seed0;
    int b = seed1;
    int i;
    int c;
    for (i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

// Fibonacci identity: F(n)^2 + F(n+1)^2 = F(2n+1)
int verify_fib_identity(int n) {
    int fn = fibonacci(n);
    int fn1 = fibonacci(n + 1);
    int f2n1 = fibonacci(2 * n + 1);
    return fn * fn + fn1 * fn1 == f2n1;
}

// Lucas-Fibonacci relationship: L(n) = F(n-1) + F(n+1)
int verify_lucas_fib_relation(int n) {
    if (n < 1) return 0;
    int ln = lucas(n);
    int fn_prev = fibonacci(n - 1);
    int fn_next = fibonacci(n + 1);
    return ln == fn_prev + fn_next;
}

// Fibonacci GCD property: gcd(F(m), F(n)) = F(gcd(m, n))
int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int verify_fib_gcd(int m, int n) {
    int fib_gcd = gcd(fibonacci(m), fibonacci(n));
    int gcd_fib = fibonacci(gcd(m, n));
    return fib_gcd == gcd_fib;
}

// Sum of first n Fibonacci numbers = F(n+2) - 1
int fib_sum(int n) {
    int sum = 0;
    int i;
    for (i = 0; i <= n; i++) {
        sum = sum + fibonacci(i);
    }
    return sum;
}

int verify_fib_sum(int n) {
    int sum = fib_sum(n);
    int fn2 = fibonacci(n + 2);
    return sum == fn2 - 1;
}

// ============================================================
// 2. Pell Numbers
// ============================================================

// Pell numbers: P(0)=0, P(1)=1, P(n) = 2*P(n-1) + P(n-2)
int pell(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    int a = 0;
    int b = 1;
    int i;
    int c;
    for (i = 2; i <= n; i++) {
        c = 2 * b + a;
        a = b;
        b = c;
    }
    return b;
}

// Pell-Lucas (companion Pell): Q(0)=1, Q(1)=1, Q(n) = 2*Q(n-1) + Q(n-2)
// Note: Actually companion Pell has Q(0)=2, Q(1)=2
// Let's use the common definition: Q(0)=2, Q(1)=2
int pell_lucas(int n) {
    if (n == 0) return 2;
    if (n == 1) return 2;
    int a = 2;
    int b = 2;
    int i;
    int c;
    for (i = 2; i <= n; i++) {
        c = 2 * b + a;
        a = b;
        b = c;
    }
    return b;
}

// Half-companion Pell: H(0)=1, H(1)=1, H(n) = 2*H(n-1) + H(n-2)
int half_companion_pell(int n) {
    if (n == 0) return 1;
    if (n == 1) return 1;
    int a = 1;
    int b = 1;
    int i;
    int c;
    for (i = 2; i <= n; i++) {
        c = 2 * b + a;
        a = b;
        b = c;
    }
    return b;
}

// Pell identity: P(n)^2 - 2*P(n-1)^2 = (-1)^n (for checking via absolute value)
int verify_pell_identity(int n) {
    if (n < 1) return 0;
    int pn = pell(n);
    int pn1 = pell(n - 1);
    int lhs = pn * pn - 2 * pn1 * pn1;
    // Should equal (-1)^n
    int expected;
    if (n % 2 == 0) {
        expected = 1;
    } else {
        expected = -1;
    }
    return lhs == expected;
}

// ============================================================
// 3. Padovan and Perrin Sequences
// ============================================================

// Padovan: P(0)=1, P(1)=1, P(2)=1, P(n) = P(n-2) + P(n-3)
int padovan(int n) {
    if (n <= 2) return 1;
    int a = 1;
    int b = 1;
    int c = 1;
    int i;
    int d;
    for (i = 3; i <= n; i++) {
        d = a + b;
        a = b;
        b = c;
        c = d;
    }
    return c;
}

// Perrin: P(0)=3, P(1)=0, P(2)=2, P(n) = P(n-2) + P(n-3)
int perrin(int n) {
    if (n == 0) return 3;
    if (n == 1) return 0;
    if (n == 2) return 2;
    int a = 3;
    int b = 0;
    int c = 2;
    int i;
    int d;
    for (i = 3; i <= n; i++) {
        d = a + b;
        a = b;
        b = c;
        c = d;
    }
    return c;
}

// Perrin primality test: If p is prime, then p divides Perrin(p)
int perrin_primality_test(int p) {
    if (p <= 1) return 0;
    int per = perrin(p);
    return per % p == 0;
}

// Is the number prime (standard check)?
int is_prime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0) return 0;
    int i;
    for (i = 3; i * i <= n; i = i + 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Sum of Padovan sequence up to n
int padovan_sum(int n) {
    int sum = 0;
    int i;
    for (i = 0; i <= n; i++) {
        sum = sum + padovan(i);
    }
    return sum;
}

// ============================================================
// 4. Look-and-Say Sequence (digits)
// ============================================================
// Starting from "1", each term describes the previous:
// 1, 11, 21, 1211, 111221, 312211, ...
// We store each term as an array of digits

// Generate look-and-say from a digit array
// Returns length of output
int look_and_say(int *input, int in_len, int *output) {
    int out_idx = 0;
    int i = 0;
    while (i < in_len) {
        int digit = input[i];
        int count = 0;
        while (i < in_len && input[i] == digit) {
            count++;
            i++;
        }
        output[out_idx] = count;
        out_idx++;
        output[out_idx] = digit;
        out_idx++;
    }
    return out_idx;
}

// Print a digit array
void print_digits(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d", arr[i]);
    }
    printf("\n");
}

// Compute n-th term of look-and-say sequence
// Returns length of the term (stored in output)
int look_and_say_nth(int n, int *output) {
    int buf1[200];
    int buf2[200];
    int len;

    // Start with "1"
    buf1[0] = 1;
    len = 1;

    int step;
    for (step = 1; step < n; step++) {
        if (step % 2 == 1) {
            len = look_and_say(buf1, len, buf2);
        } else {
            len = look_and_say(buf2, len, buf1);
        }
    }

    // Copy result to output
    int i;
    if (n == 1) {
        output[0] = 1;
        return 1;
    }
    if ((n - 1) % 2 == 1) {
        for (i = 0; i < len; i++) output[i] = buf2[i];
    } else {
        for (i = 0; i < len; i++) output[i] = buf1[i];
    }
    return len;
}

// Sum of digits in a look-and-say term
int las_digit_sum(int *arr, int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum = sum + arr[i];
    }
    return sum;
}

// Count distinct digits in a look-and-say term
int las_distinct_digits(int *arr, int n) {
    int seen[10];
    int i;
    for (i = 0; i < 10; i++) seen[i] = 0;
    for (i = 0; i < n; i++) {
        seen[arr[i]] = 1;
    }
    int count = 0;
    for (i = 0; i < 10; i++) {
        if (seen[i]) count++;
    }
    return count;
}

// ============================================================
// 5. Additional Sequences
// ============================================================

// Catalan numbers: C(0)=1, C(n) = sum(C(i)*C(n-1-i)) for i=0..n-1
int catalan(int n) {
    int cat[20];
    cat[0] = 1;
    int i;
    int j;
    for (i = 1; i <= n; i++) {
        cat[i] = 0;
        for (j = 0; j < i; j++) {
            cat[i] = cat[i] + cat[j] * cat[i - 1 - j];
        }
    }
    return cat[n];
}

// Bell numbers: B(0)=1, using Bell triangle
int bell(int n) {
    int tri[15][15];
    tri[0][0] = 1;
    int i;
    int j;
    for (i = 1; i <= n; i++) {
        tri[i][0] = tri[i - 1][i - 1];
        for (j = 1; j <= i; j++) {
            tri[i][j] = tri[i][j - 1] + tri[i - 1][j - 1];
        }
    }
    return tri[n][0];
}

// Derangement numbers: D(0)=1, D(1)=0, D(n) = (n-1)*(D(n-1)+D(n-2))
int derangement(int n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    int a = 1;
    int b = 0;
    int i;
    int c;
    for (i = 2; i <= n; i++) {
        c = (i - 1) * (a + b);
        a = b;
        b = c;
    }
    return b;
}

// Narayana numbers: N(n,k) = (1/n) * C(n,k) * C(n,k-1)
// where C(n,k) is binomial coefficient
int binomial(int n, int k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    int result = 1;
    int i;
    for (i = 0; i < k; i++) {
        result = result * (n - i);
        result = result / (i + 1);
    }
    return result;
}

int narayana(int n, int k) {
    if (n <= 0 || k <= 0 || k > n) return 0;
    return binomial(n, k) * binomial(n, k - 1) / n;
}

// Motzkin numbers: M(0)=1, M(1)=1, M(n) = ((2n+1)*M(n-1) + 3*(n-1)*M(n-2))/(n+2)
int motzkin(int n) {
    if (n <= 1) return 1;
    int mot[20];
    mot[0] = 1;
    mot[1] = 1;
    int i;
    for (i = 2; i <= n; i++) {
        mot[i] = ((2 * i + 1) * mot[i - 1] + 3 * (i - 1) * mot[i - 2]) / (i + 2);
    }
    return mot[n];
}

// ============================================================
// Test functions
// ============================================================

void test_fibonacci_variations() {
    printf("=== Fibonacci Variations ===\n");

    // Standard Fibonacci
    int i;
    printf("Fib:");
    for (i = 0; i <= 15; i++) {
        printf(" %d", fibonacci(i));
    }
    printf("\n");

    // Tribonacci
    printf("Trib:");
    for (i = 0; i <= 12; i++) {
        printf(" %d", tribonacci(i));
    }
    printf("\n");

    // Lucas
    printf("Lucas:");
    for (i = 0; i <= 12; i++) {
        printf(" %d", lucas(i));
    }
    printf("\n");

    // Generalized Fibonacci (2, 1)
    printf("GenFib(2,1):");
    for (i = 0; i <= 10; i++) {
        printf(" %d", gen_fib(2, 1, i));
    }
    printf("\n");

    // Verify identities
    printf("FibId F(5): %d\n", verify_fib_identity(5));
    printf("FibId F(8): %d\n", verify_fib_identity(8));
    printf("Lucas-Fib L(5): %d\n", verify_lucas_fib_relation(5));
    printf("Lucas-Fib L(8): %d\n", verify_lucas_fib_relation(8));
    printf("FibGCD(6,9): %d\n", verify_fib_gcd(6, 9));
    printf("FibGCD(8,12): %d\n", verify_fib_gcd(8, 12));
    printf("FibSum(10): %d\n", verify_fib_sum(10));
    printf("FibSum(7): %d\n", verify_fib_sum(7));

    printf("Fib sum [0,10]: %d\n", fib_sum(10));
}

void test_pell_numbers() {
    printf("=== Pell Numbers ===\n");

    int i;
    printf("Pell:");
    for (i = 0; i <= 10; i++) {
        printf(" %d", pell(i));
    }
    printf("\n");

    printf("PellLucas:");
    for (i = 0; i <= 8; i++) {
        printf(" %d", pell_lucas(i));
    }
    printf("\n");

    printf("HalfCompanion:");
    for (i = 0; i <= 8; i++) {
        printf(" %d", half_companion_pell(i));
    }
    printf("\n");

    // Verify Pell identity
    printf("PellId P(3): %d\n", verify_pell_identity(3));
    printf("PellId P(4): %d\n", verify_pell_identity(4));
    printf("PellId P(5): %d\n", verify_pell_identity(5));
    printf("PellId P(6): %d\n", verify_pell_identity(6));
}

void test_padovan_perrin() {
    printf("=== Padovan and Perrin ===\n");

    int i;
    printf("Padovan:");
    for (i = 0; i <= 15; i++) {
        printf(" %d", padovan(i));
    }
    printf("\n");

    printf("Perrin:");
    for (i = 0; i <= 15; i++) {
        printf(" %d", perrin(i));
    }
    printf("\n");

    // Perrin primality test
    printf("Perrin prime test 2: %d (prime=%d)\n", perrin_primality_test(2), is_prime(2));
    printf("Perrin prime test 3: %d (prime=%d)\n", perrin_primality_test(3), is_prime(3));
    printf("Perrin prime test 5: %d (prime=%d)\n", perrin_primality_test(5), is_prime(5));
    printf("Perrin prime test 7: %d (prime=%d)\n", perrin_primality_test(7), is_prime(7));
    printf("Perrin prime test 11: %d (prime=%d)\n", perrin_primality_test(11), is_prime(11));

    printf("Padovan sum [0,10]: %d\n", padovan_sum(10));
}

void test_look_and_say() {
    printf("=== Look-and-Say ===\n");

    int term[200];
    int len;

    // Generate and print terms 1 through 8
    int n;
    for (n = 1; n <= 8; n++) {
        len = look_and_say_nth(n, term);
        printf("LAS(%d): ", n);
        print_digits(term, len);
        printf("LAS(%d) len: %d\n", n, len);
    }

    // Properties of term 7
    len = look_and_say_nth(7, term);
    printf("LAS(7) digit_sum: %d\n", las_digit_sum(term, len));
    printf("LAS(7) distinct: %d\n", las_distinct_digits(term, len));

    // Properties of term 8
    len = look_and_say_nth(8, term);
    printf("LAS(8) digit_sum: %d\n", las_digit_sum(term, len));
    printf("LAS(8) distinct: %d\n", las_distinct_digits(term, len));
}

void test_additional_sequences() {
    printf("=== Additional Sequences ===\n");

    // Catalan numbers
    printf("Catalan:");
    int i;
    for (i = 0; i <= 10; i++) {
        printf(" %d", catalan(i));
    }
    printf("\n");

    // Bell numbers
    printf("Bell:");
    for (i = 0; i <= 8; i++) {
        printf(" %d", bell(i));
    }
    printf("\n");

    // Derangement numbers
    printf("Derangement:");
    for (i = 0; i <= 8; i++) {
        printf(" %d", derangement(i));
    }
    printf("\n");

    // Narayana numbers N(4,k)
    printf("Narayana N(4,k):");
    int k;
    for (k = 1; k <= 4; k++) {
        printf(" %d", narayana(4, k));
    }
    printf("\n");

    // Narayana numbers N(5,k)
    printf("Narayana N(5,k):");
    for (k = 1; k <= 5; k++) {
        printf(" %d", narayana(5, k));
    }
    printf("\n");

    // Motzkin numbers
    printf("Motzkin:");
    for (i = 0; i <= 8; i++) {
        printf(" %d", motzkin(i));
    }
    printf("\n");

    // Binomial coefficients row 6
    printf("Binomial(6,k):");
    for (k = 0; k <= 6; k++) {
        printf(" %d", binomial(6, k));
    }
    printf("\n");

    // Verify Catalan formula: C(n) = C(2n,n)/(n+1)
    printf("CatVerify(5): %d\n", catalan(5) == binomial(10, 5) / 6);
    printf("CatVerify(6): %d\n", catalan(6) == binomial(12, 6) / 7);
}


int main() {
    test_fibonacci_variations();
    test_pell_numbers();
    test_padovan_perrin();
    test_look_and_say();
    test_additional_sequences();
    return 0;
}
