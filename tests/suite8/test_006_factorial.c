int printf(const char *fmt, ...);

// Iterative factorial
int factorial(int n) {
    int result;
    int i;
    result = 1;
    for (i = 2; i <= n; i++) {
        result = result * i;
    }
    return result;
}

// Recursive factorial
int factorial_rec(int n) {
    if (n <= 1) return 1;
    return n * factorial_rec(n - 1);
}

// Number of trailing zeros in n!
int trailing_zeros(int n) {
    int count;
    count = 0;
    while (n >= 5) {
        n = n / 5;
        count = count + n;
    }
    return count;
}

// Permutation: P(n, r) = n! / (n-r)!
int permutation(int n, int r) {
    int result;
    int i;
    if (r > n) return 0;
    result = 1;
    for (i = n; i > n - r; i--) {
        result = result * i;
    }
    return result;
}

// Combination: C(n, r) = n! / (r! * (n-r)!)
int combination(int n, int r) {
    int result;
    int i;
    if (r > n) return 0;
    if (r > n - r) r = n - r;
    result = 1;
    for (i = 0; i < r; i++) {
        result = result * (n - i);
        result = result / (i + 1);
    }
    return result;
}

// Catalan number: C(n) = C(2n, n) / (n+1)
int catalan(int n) {
    return combination(2 * n, n) / (n + 1);
}

// Derangement: D(n) = (n-1) * (D(n-1) + D(n-2))
int derangement(int n) {
    int prev2;
    int prev1;
    int curr;
    int i;
    if (n == 0) return 1;
    if (n == 1) return 0;
    prev2 = 1;
    prev1 = 0;
    for (i = 2; i <= n; i++) {
        curr = (i - 1) * (prev1 + prev2);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

// Double factorial: n!! = n * (n-2) * (n-4) * ...
int double_factorial(int n) {
    int result;
    result = 1;
    while (n > 1) {
        result = result * n;
        n = n - 2;
    }
    return result;
}

// Rising factorial: x^(n) = x * (x+1) * ... * (x+n-1)
int rising_factorial(int x, int n) {
    int result;
    int i;
    result = 1;
    for (i = 0; i < n; i++) {
        result = result * (x + i);
    }
    return result;
}

// Falling factorial: x_(n) = x * (x-1) * ... * (x-n+1)
int falling_factorial(int x, int n) {
    int result;
    int i;
    result = 1;
    for (i = 0; i < n; i++) {
        result = result * (x - i);
    }
    return result;
}

// Subfactorial (number of derangements) - verify via inclusion-exclusion
int subfactorial_ie(int n) {
    int sum;
    int sign;
    int i;
    int fac_n;
    fac_n = factorial(n);
    sum = 0;
    sign = 1;
    for (i = 0; i <= n; i++) {
        sum = sum + sign * fac_n / factorial(i);
        sign = -sign;
    }
    return sum;
}

void print_pascal_row(int n) {
    int i;
    printf("Pascal row %d:", n);
    for (i = 0; i <= n; i++) {
        printf(" %d", combination(n, i));
    }
    printf("\n");
}

int main() {
    // EXPECT: 0! = 1
    printf("0! = %d\n", factorial(0));

    // EXPECT: 5! = 120
    printf("5! = %d\n", factorial(5));

    // EXPECT: 10! = 3628800
    printf("10! = %d\n", factorial(10));

    // EXPECT: 5!_rec = 120
    printf("5!_rec = %d\n", factorial_rec(5));

    // EXPECT: zeros(100!) = 24
    printf("zeros(100!) = %d\n", trailing_zeros(100));

    // EXPECT: zeros(25!) = 6
    printf("zeros(25!) = %d\n", trailing_zeros(25));

    // EXPECT: P(5,3) = 60
    printf("P(5,3) = %d\n", permutation(5, 3));

    // EXPECT: P(10,2) = 90
    printf("P(10,2) = %d\n", permutation(10, 2));

    // EXPECT: C(10,3) = 120
    printf("C(10,3) = %d\n", combination(10, 3));

    // EXPECT: C(20,10) = 184756
    printf("C(20,10) = %d\n", combination(20, 10));

    // EXPECT: Catalan(5) = 42
    printf("Catalan(5) = %d\n", catalan(5));

    // EXPECT: Catalan(6) = 132
    printf("Catalan(6) = %d\n", catalan(6));

    // EXPECT: D(5) = 44
    printf("D(5) = %d\n", derangement(5));

    // EXPECT: D(6) = 265
    printf("D(6) = %d\n", derangement(6));

    // EXPECT: 7!! = 105
    printf("7!! = %d\n", double_factorial(7));

    // EXPECT: 8!! = 384
    printf("8!! = %d\n", double_factorial(8));

    // EXPECT: rising(3, 4) = 360
    printf("rising(3, 4) = %d\n", rising_factorial(3, 4));

    // EXPECT: falling(7, 3) = 210
    printf("falling(7, 3) = %d\n", falling_factorial(7, 3));

    // EXPECT: subfac_ie(5) = 44
    printf("subfac_ie(5) = %d\n", subfactorial_ie(5));

    // EXPECT: Pascal row 6: 1 6 15 20 15 6 1
    print_pascal_row(6);

    return 0;
}
