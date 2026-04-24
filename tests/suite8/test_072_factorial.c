int printf(const char *fmt, ...);

// Factorial: recursive, iterative, and related computations

int factorial_recursive(int n) {
    if (n <= 1) return 1;
    return n * factorial_recursive(n - 1);
}

int factorial_iterative(int n) {
    int result = 1;
    int i;
    for (i = 2; i <= n; i++) {
        result = result * i;
    }
    return result;
}

// Compute n! / (k! * (n-k)!) = C(n, k) binomial coefficient
int binomial(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    // Use the smaller k for efficiency
    if (k > n - k) k = n - k;
    int result = 1;
    int i;
    for (i = 0; i < k; i++) {
        result = result * (n - i);
        result = result / (i + 1);
    }
    return result;
}

// Permutation: n! / (n-k)!
int permutation(int n, int k) {
    if (k < 0 || k > n) return 0;
    int result = 1;
    int i;
    for (i = n; i > n - k; i--) {
        result = result * i;
    }
    return result;
}

// Derangement: D(n) = (n-1) * (D(n-1) + D(n-2))
int derangement(int n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    int d_prev2 = 1;  // D(0)
    int d_prev1 = 0;  // D(1)
    int d_curr = 0;
    int i;
    for (i = 2; i <= n; i++) {
        d_curr = (i - 1) * (d_prev1 + d_prev2);
        d_prev2 = d_prev1;
        d_prev1 = d_curr;
    }
    return d_curr;
}

// Double factorial: n!! = n * (n-2) * (n-4) * ...
int double_factorial(int n) {
    if (n <= 1) return 1;
    return n * double_factorial(n - 2);
}

// Catalan number: C(n) = C(2n, n) / (n+1)
int catalan(int n) {
    int c = binomial(2 * n, n);
    return c / (n + 1);
}

// Trailing zeros in n! (count factors of 5)
int trailing_zeros(int n) {
    int count = 0;
    while (n >= 5) {
        n = n / 5;
        count = count + n;
    }
    return count;
}

// Sum of digits of n!
int digit_sum(int num) {
    int sum = 0;
    if (num < 0) num = -num;
    while (num > 0) {
        sum = sum + (num % 10);
        num = num / 10;
    }
    return sum;
}

int main() {
    int i;

    // Test recursive factorial
    // EXPECT: fact_rec(0) = 1
    printf("fact_rec(0) = %d\n", factorial_recursive(0));
    // EXPECT: fact_rec(1) = 1
    printf("fact_rec(1) = %d\n", factorial_recursive(1));
    // EXPECT: fact_rec(5) = 120
    printf("fact_rec(5) = %d\n", factorial_recursive(5));
    // EXPECT: fact_rec(10) = 3628800
    printf("fact_rec(10) = %d\n", factorial_recursive(10));

    // Test iterative factorial
    // EXPECT: fact_iter(5) = 120
    printf("fact_iter(5) = %d\n", factorial_iterative(5));
    // EXPECT: fact_iter(10) = 3628800
    printf("fact_iter(10) = %d\n", factorial_iterative(10));

    // Verify match
    int match = 1;
    for (i = 0; i <= 10; i++) {
        if (factorial_recursive(i) != factorial_iterative(i)) {
            match = 0;
        }
    }
    // EXPECT: rec_iter_match = 1
    printf("rec_iter_match = %d\n", match);

    // Test binomial coefficients
    // EXPECT: C(5,2) = 10
    printf("C(5,2) = %d\n", binomial(5, 2));
    // EXPECT: C(10,3) = 120
    printf("C(10,3) = %d\n", binomial(10, 3));
    // EXPECT: C(10,0) = 1
    printf("C(10,0) = %d\n", binomial(10, 0));
    // EXPECT: C(10,10) = 1
    printf("C(10,10) = %d\n", binomial(10, 10));

    // Test permutations
    // EXPECT: P(5,2) = 20
    printf("P(5,2) = %d\n", permutation(5, 2));
    // EXPECT: P(10,3) = 720
    printf("P(10,3) = %d\n", permutation(10, 3));

    // Test derangements
    // EXPECT: D(2) = 1
    printf("D(2) = %d\n", derangement(2));
    // EXPECT: D(3) = 2
    printf("D(3) = %d\n", derangement(3));
    // EXPECT: D(5) = 44
    printf("D(5) = %d\n", derangement(5));

    // Test double factorial
    // EXPECT: 5!! = 15
    printf("5!! = %d\n", double_factorial(5));
    // EXPECT: 6!! = 48
    printf("6!! = %d\n", double_factorial(6));
    // EXPECT: 7!! = 105
    printf("7!! = %d\n", double_factorial(7));

    // Test Catalan numbers
    // EXPECT: Cat(0) = 1
    printf("Cat(0) = %d\n", catalan(0));
    // EXPECT: Cat(3) = 5
    printf("Cat(3) = %d\n", catalan(3));
    // EXPECT: Cat(5) = 42
    printf("Cat(5) = %d\n", catalan(5));

    // Test trailing zeros
    // EXPECT: zeros(10!) = 2
    printf("zeros(10!) = %d\n", trailing_zeros(10));
    // EXPECT: zeros(25!) = 6
    printf("zeros(25!) = %d\n", trailing_zeros(25));
    // EXPECT: zeros(100!) = 24
    printf("zeros(100!) = %d\n", trailing_zeros(100));

    // Test digit sum of factorials
    // EXPECT: digitsum(5!) = 3
    printf("digitsum(5!) = %d\n", digit_sum(factorial_iterative(5)));
    // EXPECT: digitsum(10!) = 27
    printf("digitsum(10!) = %d\n", digit_sum(factorial_iterative(10)));

    return 0;
}
