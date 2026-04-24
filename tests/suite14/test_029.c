int printf(const char *fmt, ...);

// Bell numbers: B(n) = number of partitions of a set of n elements
// B(0)=1, B(n+1) = sum_{k=0}^{n} C(n,k)*B(k)
// Also computed via Bell triangle

int choose(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    int r = 1;
    int i;
    for (i = 0; i < k; i++) {
        r = r * (n - i);
        r = r / (i + 1);
    }
    return r;
}

// Compute Bell numbers using the Bell triangle (Aitken's array)
// Row 0: B(0)=1
// Each subsequent row starts with the last element of previous row
// a[i][j] = a[i][j-1] + a[i-1][j]  (but indexing is tricky)
void bell_triangle(int *bell, int max_n) {
    int tri[15][15];
    int i;
    int j;
    tri[0][0] = 1;
    bell[0] = 1;

    for (i = 1; i <= max_n; i++) {
        tri[i][0] = tri[i - 1][i - 1];
        bell[i] = tri[i][0];
        for (j = 1; j <= i; j++) {
            tri[i][j] = tri[i][j - 1] + tri[i - 1][j - 1];
        }
    }
}

// Compute Bell number using the summation formula
// B(n+1) = sum_{k=0}^{n} C(n,k)*B(k)
int bell_summation(int n) {
    int b[15];
    int i;
    int k;
    b[0] = 1;
    for (i = 1; i <= n; i++) {
        b[i] = 0;
        for (k = 0; k < i; k++) {
            b[i] = b[i] + choose(i - 1, k) * b[k];
        }
    }
    return b[n];
}

// Verify Bell number using Stirling numbers of second kind
// B(n) = sum_{k=0}^{n} S(n,k) where S(n,k) = Stirling number 2nd kind
int stirling2(int n, int k) {
    if (n == 0 && k == 0) return 1;
    if (n == 0 || k == 0) return 0;
    if (k > n) return 0;
    if (k == 1 || k == n) return 1;
    // S(n,k) = k*S(n-1,k) + S(n-1,k-1)
    int dp[15][15];
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

int bell_via_stirling(int n) {
    int sum = 0;
    int k;
    for (k = 0; k <= n; k++) {
        sum = sum + stirling2(n, k);
    }
    return sum;
}

// Dobinski's formula: B(n) = (1/e) * sum_{k=0}^{inf} k^n / k!
// We can't do this exactly with integers, but we can verify approximate
// by computing sum * e (using integer arithmetic for small n)
// Actually, let's use the exact formula with partial sums and check convergence

// Number of surjections from n-set to k-set = k! * S(n,k)
int surjections(int n, int k) {
    int fact = 1;
    int i;
    for (i = 2; i <= k; i++) fact = fact * i;
    return fact * stirling2(n, k);
}

// Bell polynomial evaluation: Bn(x) = sum S(n,k)*x^k
int bell_polynomial(int n, int x) {
    int sum = 0;
    int k;
    int xpow;
    for (k = 0; k <= n; k++) {
        xpow = 1;
        int i;
        for (i = 0; i < k; i++) xpow = xpow * x;
        sum = sum + stirling2(n, k) * xpow;
    }
    return sum;
}

int main(void) {
    // Test 1: Bell numbers via triangle
    int bell[15];
    bell_triangle(bell, 12);

    printf("B(0): %d\n", bell[0]);
    // EXPECT: B(0): 1
    printf("B(1): %d\n", bell[1]);
    // EXPECT: B(1): 1
    printf("B(2): %d\n", bell[2]);
    // EXPECT: B(2): 2
    printf("B(3): %d\n", bell[3]);
    // EXPECT: B(3): 5
    printf("B(4): %d\n", bell[4]);
    // EXPECT: B(4): 15
    printf("B(5): %d\n", bell[5]);
    // EXPECT: B(5): 52
    printf("B(6): %d\n", bell[6]);
    // EXPECT: B(6): 203
    printf("B(7): %d\n", bell[7]);
    // EXPECT: B(7): 877
    printf("B(8): %d\n", bell[8]);
    // EXPECT: B(8): 4140

    // Test 2: Summation formula matches triangle
    printf("sum B(3): %d\n", bell_summation(3));
    // EXPECT: sum B(3): 5
    printf("sum B(5): %d\n", bell_summation(5));
    // EXPECT: sum B(5): 52
    printf("sum B(7): %d\n", bell_summation(7));
    // EXPECT: sum B(7): 877

    // Test 3: Stirling formula matches
    printf("stir B(4): %d\n", bell_via_stirling(4));
    // EXPECT: stir B(4): 15
    printf("stir B(6): %d\n", bell_via_stirling(6));
    // EXPECT: stir B(6): 203
    printf("stir B(8): %d\n", bell_via_stirling(8));
    // EXPECT: stir B(8): 4140

    // Test 4: Individual Stirling numbers S(n,k)
    printf("S(4,1): %d\n", stirling2(4, 1));
    // EXPECT: S(4,1): 1
    printf("S(4,2): %d\n", stirling2(4, 2));
    // EXPECT: S(4,2): 7
    printf("S(4,3): %d\n", stirling2(4, 3));
    // EXPECT: S(4,3): 6
    printf("S(4,4): %d\n", stirling2(4, 4));
    // EXPECT: S(4,4): 1
    printf("S(5,2): %d\n", stirling2(5, 2));
    // EXPECT: S(5,2): 15
    printf("S(5,3): %d\n", stirling2(5, 3));
    // EXPECT: S(5,3): 25

    // Test 5: Surjections
    printf("surj(3,2): %d\n", surjections(3, 2));
    // EXPECT: surj(3,2): 6
    printf("surj(4,2): %d\n", surjections(4, 2));
    // EXPECT: surj(4,2): 14
    printf("surj(4,3): %d\n", surjections(4, 3));
    // EXPECT: surj(4,3): 36
    printf("surj(5,3): %d\n", surjections(5, 3));
    // EXPECT: surj(5,3): 150

    // Test 6: Bell polynomial Bn(x) at x=1 should give B(n)
    printf("Bp(3,1): %d\n", bell_polynomial(3, 1));
    // EXPECT: Bp(3,1): 5
    printf("Bp(4,1): %d\n", bell_polynomial(4, 1));
    // EXPECT: Bp(4,1): 15
    // At x=2: B_n(2) gives different values
    printf("Bp(3,2): %d\n", bell_polynomial(3, 2));
    // EXPECT: Bp(3,2): 22
    printf("Bp(4,2): %d\n", bell_polynomial(4, 2));
    // EXPECT: Bp(4,2): 94

    // Test 7: Verify B(n) = sum S(n,k) explicitly for n=5
    int s50 = stirling2(5, 0);
    int s51 = stirling2(5, 1);
    int s52 = stirling2(5, 2);
    int s53 = stirling2(5, 3);
    int s54 = stirling2(5, 4);
    int s55 = stirling2(5, 5);
    printf("S5: %d+%d+%d+%d+%d+%d=%d\n",
        s50, s51, s52, s53, s54, s55,
        s50 + s51 + s52 + s53 + s54 + s55);
    // EXPECT: S5: 0+1+15+25+10+1=52

    printf("done\n");
    // EXPECT: done
    return 0;
}
