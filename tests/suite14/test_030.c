int printf(const char *fmt, ...);

// Stirling numbers of the first and second kind

// Stirling numbers of the second kind S(n,k):
// Number of ways to partition n elements into k non-empty subsets
// S(n,k) = k*S(n-1,k) + S(n-1,k-1)
int stirling2(int n, int k) {
    if (n == 0 && k == 0) return 1;
    if (n == 0 || k == 0) return 0;
    if (k > n) return 0;
    int dp[12][12];
    int i;
    int j;
    for (i = 0; i <= n; i++) {
        for (j = 0; j <= k; j++) {
            dp[i][j] = 0;
        }
    }
    dp[0][0] = 1;
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= i && j <= k; j++) {
            dp[i][j] = j * dp[i - 1][j] + dp[i - 1][j - 1];
        }
    }
    return dp[n][k];
}

// Unsigned Stirling numbers of the first kind |s(n,k)|:
// Number of permutations of n elements with exactly k cycles
// |s(n,k)| = (n-1)*|s(n-1,k)| + |s(n-1,k-1)|
int stirling1_unsigned(int n, int k) {
    if (n == 0 && k == 0) return 1;
    if (n == 0 || k == 0) return 0;
    if (k > n) return 0;
    int dp[12][12];
    int i;
    int j;
    for (i = 0; i <= n; i++) {
        for (j = 0; j <= k; j++) {
            dp[i][j] = 0;
        }
    }
    dp[0][0] = 1;
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= i && j <= k; j++) {
            dp[i][j] = (i - 1) * dp[i - 1][j] + dp[i - 1][j - 1];
        }
    }
    return dp[n][k];
}

// Signed Stirling numbers of the first kind s(n,k) = (-1)^(n-k) * |s(n,k)|
int stirling1_signed(int n, int k) {
    int val = stirling1_unsigned(n, k);
    if ((n - k) % 2 == 0) return val;
    return -val;
}

int factorial(int n) {
    int r = 1;
    int i;
    for (i = 2; i <= n; i++) r = r * i;
    return r;
}

// Verify: sum of |s(n,k)| for k=0..n = n!
int sum_stirling1(int n) {
    int sum = 0;
    int k;
    for (k = 0; k <= n; k++) {
        sum = sum + stirling1_unsigned(n, k);
    }
    return sum;
}

// Verify: sum of S(n,k) for k=0..n = B(n) (Bell number)
int sum_stirling2(int n) {
    int sum = 0;
    int k;
    for (k = 0; k <= n; k++) {
        sum = sum + stirling2(n, k);
    }
    return sum;
}

// Orthogonality: sum_k s(n,k)*S(k,m) = delta(n,m) for signed s
// Actually: sum_k |s(n,k)|*S(k,m)*(-1)^(n-k) = delta(n,m)
// Or equivalently: x^n = sum_k S(n,k) * x*(x-1)*...*(x-k+1)
// and x*(x-1)*...*(x-n+1) = sum_k s(n,k) * x^k

// Evaluate falling factorial: x*(x-1)*...*(x-n+1)
int falling_factorial(int x, int n) {
    int result = 1;
    int i;
    for (i = 0; i < n; i++) {
        result = result * (x - i);
    }
    return result;
}

// Verify: x^n = sum_k S(n,k)*falling_factorial(x,k)
int verify_stirling2_identity(int x, int n) {
    int sum = 0;
    int k;
    for (k = 0; k <= n; k++) {
        sum = sum + stirling2(n, k) * falling_factorial(x, k);
    }
    // Should equal x^n
    int power = 1;
    int i;
    for (i = 0; i < n; i++) power = power * x;
    return sum == power;
}

// Verify: falling_factorial(x,n) = sum_k s(n,k)*x^k (signed)
int verify_stirling1_identity(int x, int n) {
    int sum = 0;
    int k;
    for (k = 0; k <= n; k++) {
        int power = 1;
        int i;
        for (i = 0; i < k; i++) power = power * x;
        sum = sum + stirling1_signed(n, k) * power;
    }
    return sum == falling_factorial(x, n);
}

// Number of permutations with exactly one cycle = |s(n,1)| = (n-1)!
int check_single_cycle(int n) {
    return stirling1_unsigned(n, 1) == factorial(n - 1);
}

// S(n,2) = 2^(n-1) - 1
int check_stirling2_k2(int n) {
    int power = 1;
    int i;
    for (i = 0; i < n - 1; i++) power = power * 2;
    return stirling2(n, 2) == power - 1;
}

int main(void) {
    // Test 1: Stirling second kind table for n=1..6, k=1..n
    printf("S2(1,1): %d\n", stirling2(1, 1));
    // EXPECT: S2(1,1): 1
    printf("S2(2,1): %d S2(2,2): %d\n", stirling2(2, 1), stirling2(2, 2));
    // EXPECT: S2(2,1): 1 S2(2,2): 1
    printf("S2(3,1): %d S2(3,2): %d S2(3,3): %d\n",
        stirling2(3, 1), stirling2(3, 2), stirling2(3, 3));
    // EXPECT: S2(3,1): 1 S2(3,2): 3 S2(3,3): 1
    printf("S2(4,1): %d S2(4,2): %d S2(4,3): %d S2(4,4): %d\n",
        stirling2(4, 1), stirling2(4, 2), stirling2(4, 3), stirling2(4, 4));
    // EXPECT: S2(4,1): 1 S2(4,2): 7 S2(4,3): 6 S2(4,4): 1
    printf("S2(5,2): %d S2(5,3): %d S2(5,4): %d\n",
        stirling2(5, 2), stirling2(5, 3), stirling2(5, 4));
    // EXPECT: S2(5,2): 15 S2(5,3): 25 S2(5,4): 10
    printf("S2(6,2): %d S2(6,3): %d S2(6,4): %d\n",
        stirling2(6, 2), stirling2(6, 3), stirling2(6, 4));
    // EXPECT: S2(6,2): 31 S2(6,3): 90 S2(6,4): 65

    // Test 2: Stirling first kind (unsigned) table
    printf("s1(1,1): %d\n", stirling1_unsigned(1, 1));
    // EXPECT: s1(1,1): 1
    printf("s1(2,1): %d s1(2,2): %d\n",
        stirling1_unsigned(2, 1), stirling1_unsigned(2, 2));
    // EXPECT: s1(2,1): 1 s1(2,2): 1
    printf("s1(3,1): %d s1(3,2): %d s1(3,3): %d\n",
        stirling1_unsigned(3, 1), stirling1_unsigned(3, 2), stirling1_unsigned(3, 3));
    // EXPECT: s1(3,1): 2 s1(3,2): 3 s1(3,3): 1
    printf("s1(4,1): %d s1(4,2): %d s1(4,3): %d s1(4,4): %d\n",
        stirling1_unsigned(4, 1), stirling1_unsigned(4, 2),
        stirling1_unsigned(4, 3), stirling1_unsigned(4, 4));
    // EXPECT: s1(4,1): 6 s1(4,2): 11 s1(4,3): 6 s1(4,4): 1
    printf("s1(5,2): %d s1(5,3): %d s1(5,4): %d\n",
        stirling1_unsigned(5, 2), stirling1_unsigned(5, 3), stirling1_unsigned(5, 4));
    // EXPECT: s1(5,2): 50 s1(5,3): 35 s1(5,4): 10

    // Test 3: Signed Stirling first kind
    printf("signed s(3,1): %d\n", stirling1_signed(3, 1));
    // EXPECT: signed s(3,1): 2
    printf("signed s(3,2): %d\n", stirling1_signed(3, 2));
    // EXPECT: signed s(3,2): -3
    printf("signed s(4,1): %d\n", stirling1_signed(4, 1));
    // EXPECT: signed s(4,1): -6
    printf("signed s(4,2): %d\n", stirling1_signed(4, 2));
    // EXPECT: signed s(4,2): 11
    printf("signed s(4,3): %d\n", stirling1_signed(4, 3));
    // EXPECT: signed s(4,3): -6

    // Test 4: Sum |s(n,k)| = n!
    printf("sum s1(3): %d = %d\n", sum_stirling1(3), factorial(3));
    // EXPECT: sum s1(3): 6 = 6
    printf("sum s1(4): %d = %d\n", sum_stirling1(4), factorial(4));
    // EXPECT: sum s1(4): 24 = 24
    printf("sum s1(5): %d = %d\n", sum_stirling1(5), factorial(5));
    // EXPECT: sum s1(5): 120 = 120

    // Test 5: Sum S(n,k) = Bell number
    printf("sum S2(3): %d\n", sum_stirling2(3));
    // EXPECT: sum S2(3): 5
    printf("sum S2(5): %d\n", sum_stirling2(5));
    // EXPECT: sum S2(5): 52
    printf("sum S2(7): %d\n", sum_stirling2(7));
    // EXPECT: sum S2(7): 877

    // Test 6: Identity x^n = sum S(n,k)*x_falling_k
    printf("id2(3,3): %d\n", verify_stirling2_identity(3, 3));
    // EXPECT: id2(3,3): 1
    printf("id2(5,4): %d\n", verify_stirling2_identity(5, 4));
    // EXPECT: id2(5,4): 1
    printf("id2(4,5): %d\n", verify_stirling2_identity(4, 5));
    // EXPECT: id2(4,5): 1

    // Test 7: Identity falling(x,n) = sum s(n,k)*x^k
    printf("id1(3,3): %d\n", verify_stirling1_identity(3, 3));
    // EXPECT: id1(3,3): 1
    printf("id1(5,4): %d\n", verify_stirling1_identity(5, 4));
    // EXPECT: id1(5,4): 1
    printf("id1(4,5): %d\n", verify_stirling1_identity(4, 5));
    // EXPECT: id1(4,5): 1

    // Test 8: Special values
    printf("1cycle(4): %d\n", check_single_cycle(4));
    // EXPECT: 1cycle(4): 1
    printf("1cycle(5): %d\n", check_single_cycle(5));
    // EXPECT: 1cycle(5): 1
    printf("S2k2(5): %d\n", check_stirling2_k2(5));
    // EXPECT: S2k2(5): 1
    printf("S2k2(6): %d\n", check_stirling2_k2(6));
    // EXPECT: S2k2(6): 1

    // Test 9: Falling factorial values
    printf("ff(5,3): %d\n", falling_factorial(5, 3));
    // EXPECT: ff(5,3): 60
    printf("ff(7,4): %d\n", falling_factorial(7, 4));
    // EXPECT: ff(7,4): 840
    printf("ff(4,4): %d\n", falling_factorial(4, 4));
    // EXPECT: ff(4,4): 24

    printf("done\n");
    // EXPECT: done
    return 0;
}
