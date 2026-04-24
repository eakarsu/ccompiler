int printf(const char *fmt, ...);

/* Factorial (iterative) */
int factorial(int n) {
    int r = 1, i;
    for (i = 2; i <= n; i++) r *= i;
    return r;
}

/* Combinations C(n, k) using Pascal's triangle row to avoid overflow */
int comb(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    /* Use symmetry */
    if (k > n - k) k = n - k;
    int result = 1, i;
    for (i = 0; i < k; i++) {
        result = result * (n - i) / (i + 1);
    }
    return result;
}

/* Permutation count P(n, k) = n! / (n-k)! */
int perm(int n, int k) {
    if (k < 0 || k > n) return 0;
    int result = 1, i;
    for (i = n; i > n - k; i--) result *= i;
    return result;
}

/* Compute row n of Pascal's triangle (0-indexed), store in row[] */
/* row[k] = C(n, k) */
void pascal_row(int n, int row[]) {
    int k;
    row[0] = 1;
    for (k = 1; k <= n; k++) {
        row[k] = row[k-1] * (n - k + 1) / k;
    }
}

/* Catalan number C_n = C(2n, n) / (n+1) */
int catalan(int n) {
    return comb(2*n, n) / (n + 1);
}

/* Number of ways to tile a 2*n board with 1*2 dominoes = F(n+1) Fibonacci */
int tiling_ways(int n) {
    if (n <= 0) return 1;
    if (n == 1) return 1;
    int a = 1, b = 1, i;
    for (i = 2; i <= n; i++) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}

/* Stirling numbers of the second kind S(n, k) */
/* S(n, k) = k*S(n-1, k) + S(n-1, k-1) */
int stirling2(int n, int k) {
    /* dp table */
    int dp[10][10];
    int i, j;
    for (i = 0; i <= n; i++)
        for (j = 0; j <= k; j++)
            dp[i][j] = 0;
    dp[0][0] = 1;
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= i && j <= k; j++) {
            dp[i][j] = j * dp[i-1][j] + dp[i-1][j-1];
        }
    }
    return dp[n][k];
}

/* Derangement count D(n): permutations with no fixed point */
/* D(n) = (n-1)*(D(n-1) + D(n-2)) */
int derangement(int n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    int d0 = 1, d1 = 0, i;
    for (i = 2; i <= n; i++) {
        int d = (i - 1) * (d0 + d1);
        d0 = d1;
        d1 = d;
    }
    return d1;
}

/* Bell number B(n): number of partitions of n-element set */
/* Use Bell triangle */
int bell(int n) {
    int triangle[8][8];
    int i, j;
    triangle[0][0] = 1;
    for (i = 1; i <= n; i++) {
        triangle[i][0] = triangle[i-1][i-1];
        for (j = 1; j <= i; j++) {
            triangle[i][j] = triangle[i][j-1] + triangle[i-1][j-1];
        }
    }
    return triangle[n][0];
}

int main(void) {
    int row[10];
    int i;

    /* factorial tests */
    printf("%d\n", factorial(0));  // EXPECT: 1
    printf("%d\n", factorial(1));  // EXPECT: 1
    printf("%d\n", factorial(5));  // EXPECT: 120
    printf("%d\n", factorial(7));  // EXPECT: 5040
    printf("%d\n", factorial(10)); // EXPECT: 3628800

    /* comb tests */
    printf("%d\n", comb(5, 0));   // EXPECT: 1
    printf("%d\n", comb(5, 1));   // EXPECT: 5
    printf("%d\n", comb(5, 2));   // EXPECT: 10
    printf("%d\n", comb(5, 3));   // EXPECT: 10
    printf("%d\n", comb(5, 5));   // EXPECT: 1
    printf("%d\n", comb(10, 4));  // EXPECT: 210
    printf("%d\n", comb(8, 3));   // EXPECT: 56

    /* perm tests */
    printf("%d\n", perm(5, 0));   // EXPECT: 1
    printf("%d\n", perm(5, 1));   // EXPECT: 5
    printf("%d\n", perm(5, 2));   // EXPECT: 20
    printf("%d\n", perm(5, 3));   // EXPECT: 60
    printf("%d\n", perm(5, 5));   // EXPECT: 120

    /* pascal_row for n=6: 1 6 15 20 15 6 1 */
    pascal_row(6, row);
    for (i = 0; i <= 6; i++) printf("%d\n", row[i]);
    // EXPECT: 1
    // EXPECT: 6
    // EXPECT: 15
    // EXPECT: 20
    // EXPECT: 15
    // EXPECT: 6
    // EXPECT: 1

    /* catalan numbers: 1,1,2,5,14,42 */
    printf("%d\n", catalan(0));  // EXPECT: 1
    printf("%d\n", catalan(1));  // EXPECT: 1
    printf("%d\n", catalan(2));  // EXPECT: 2
    printf("%d\n", catalan(3));  // EXPECT: 5
    printf("%d\n", catalan(4));  // EXPECT: 14
    printf("%d\n", catalan(5));  // EXPECT: 42

    /* tiling_ways */
    printf("%d\n", tiling_ways(1));  // EXPECT: 1
    printf("%d\n", tiling_ways(2));  // EXPECT: 2
    printf("%d\n", tiling_ways(3));  // EXPECT: 3
    printf("%d\n", tiling_ways(4));  // EXPECT: 5
    printf("%d\n", tiling_ways(5));  // EXPECT: 8

    /* stirling2: S(4,2)=7, S(4,3)=6, S(5,2)=15 */
    printf("%d\n", stirling2(4, 2));  // EXPECT: 7
    printf("%d\n", stirling2(4, 3));  // EXPECT: 6
    printf("%d\n", stirling2(5, 2));  // EXPECT: 15

    /* derangement: D(0)=1,D(1)=0,D(2)=1,D(3)=2,D(4)=9 */
    printf("%d\n", derangement(0));  // EXPECT: 1
    printf("%d\n", derangement(1));  // EXPECT: 0
    printf("%d\n", derangement(2));  // EXPECT: 1
    printf("%d\n", derangement(3));  // EXPECT: 2
    printf("%d\n", derangement(4));  // EXPECT: 9

    /* bell numbers: B(0)=1,B(1)=1,B(2)=2,B(3)=5,B(4)=15,B(5)=52 */
    printf("%d\n", bell(0));  // EXPECT: 1
    printf("%d\n", bell(1));  // EXPECT: 1
    printf("%d\n", bell(2));  // EXPECT: 2
    printf("%d\n", bell(3));  // EXPECT: 5
    printf("%d\n", bell(4));  // EXPECT: 15
    printf("%d\n", bell(5));  // EXPECT: 52

    return 0;
}
