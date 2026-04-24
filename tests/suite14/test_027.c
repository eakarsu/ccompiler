int printf(const char *fmt, ...);

// Partition function: number of ways to write n as sum of positive integers

// p(n) = number of partitions of n (order doesn't matter)
// Using DP: p(n) = sum over k of partitions of n using parts <= k
int partition_count(int n) {
    // dp[i][j] = number of partitions of i using parts at most j
    int dp[50][50];
    int i;
    int j;
    for (i = 0; i < 50; i++) {
        for (j = 0; j < 50; j++) {
            dp[i][j] = 0;
        }
    }
    // Base case: one way to partition 0
    for (j = 0; j < 50; j++) {
        dp[0][j] = 1;
    }
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            dp[i][j] = dp[i][j - 1]; // don't use part j
            if (i >= j) {
                dp[i][j] = dp[i][j] + dp[i - j][j]; // use at least one j
            }
        }
    }
    return dp[n][n];
}

// Partition into distinct parts
int partition_distinct(int n) {
    int dp[50][50];
    int i;
    int j;
    for (i = 0; i < 50; i++) {
        for (j = 0; j < 50; j++) {
            dp[i][j] = 0;
        }
    }
    for (j = 0; j < 50; j++) {
        dp[0][j] = 1;
    }
    for (j = 1; j <= n; j++) {
        for (i = 0; i <= n; i++) {
            dp[i][j] = dp[i][j - 1]; // don't use j
            if (i >= j) {
                dp[i][j] = dp[i][j] + dp[i - j][j - 1]; // use j exactly once
            }
        }
    }
    return dp[n][n];
}

// Partition into odd parts
int partition_odd(int n) {
    int dp[50][50];
    int i;
    int j;
    int odd;
    for (i = 0; i < 50; i++) {
        for (j = 0; j < 50; j++) {
            dp[i][j] = 0;
        }
    }
    // We use odd parts: 1, 3, 5, 7, ...
    // Index j means we use odd parts up to the j-th odd number (2j-1)
    int max_odd_idx = (n + 1) / 2; // index for odd parts
    for (j = 0; j <= max_odd_idx; j++) {
        dp[0][j] = 1;
    }
    for (j = 1; j <= max_odd_idx; j++) {
        odd = 2 * j - 1;
        for (i = 0; i <= n; i++) {
            dp[i][j] = dp[i][j - 1];
            if (i >= odd) {
                dp[i][j] = dp[i][j] + dp[i - odd][j];
            }
        }
    }
    return dp[n][max_odd_idx];
}

// Partition into at most k parts
int partition_at_most_k(int n, int k) {
    // Equals partition of n with largest part <= k
    int dp[50][50];
    int i;
    int j;
    for (i = 0; i < 50; i++) {
        for (j = 0; j < 50; j++) {
            dp[i][j] = 0;
        }
    }
    for (j = 0; j <= k; j++) {
        dp[0][j] = 1;
    }
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= k; j++) {
            dp[i][j] = dp[i][j - 1];
            if (i >= j) {
                dp[i][j] = dp[i][j] + dp[i - j][j];
            }
        }
    }
    return dp[n][k];
}

// Partition into exactly k parts
int partition_exactly_k(int n, int k) {
    // p(n,k) = p(n-1,k-1) + p(n-k,k)
    // (either smallest part is 1, remove it; or subtract 1 from each part)
    int dp[50][50];
    int i;
    int j;
    for (i = 0; i < 50; i++) {
        for (j = 0; j < 50; j++) {
            dp[i][j] = 0;
        }
    }
    dp[0][0] = 1;
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= i && j <= k; j++) {
            dp[i][j] = dp[i - 1][j - 1];
            if (i - j >= 0) {
                dp[i][j] = dp[i][j] + dp[i - j][j];
            }
        }
    }
    return dp[n][k];
}

// Euler's pentagonal theorem verification
// p(n) = p(n-1) + p(n-2) - p(n-5) - p(n-7) + p(n-12) + p(n-15) - ...
// Generalized pentagonal numbers: k(3k-1)/2 for k = 1,-1,2,-2,3,-3,...
int pentagonal(int k) {
    return k * (3 * k - 1) / 2;
}

int partition_euler(int n) {
    int dp[50];
    int i;
    dp[0] = 1;
    for (i = 1; i <= n; i++) {
        dp[i] = 0;
        int k;
        int sign;
        for (k = 1; ; k++) {
            int p1 = k * (3 * k - 1) / 2;
            int p2 = k * (3 * k + 1) / 2;
            sign = (k % 2 == 1) ? 1 : -1;
            if (p1 > i) break;
            dp[i] = dp[i] + sign * dp[i - p1];
            if (p2 <= i) {
                dp[i] = dp[i] + sign * dp[i - p2];
            }
        }
    }
    return dp[n];
}

int main(void) {
    // Test 1: Basic partition numbers
    printf("p(0): %d\n", partition_count(0));
    // EXPECT: p(0): 1
    printf("p(1): %d\n", partition_count(1));
    // EXPECT: p(1): 1
    printf("p(2): %d\n", partition_count(2));
    // EXPECT: p(2): 2
    printf("p(3): %d\n", partition_count(3));
    // EXPECT: p(3): 3
    printf("p(4): %d\n", partition_count(4));
    // EXPECT: p(4): 5
    printf("p(5): %d\n", partition_count(5));
    // EXPECT: p(5): 7
    printf("p(6): %d\n", partition_count(6));
    // EXPECT: p(6): 11
    printf("p(10): %d\n", partition_count(10));
    // EXPECT: p(10): 42
    printf("p(15): %d\n", partition_count(15));
    // EXPECT: p(15): 176
    printf("p(20): %d\n", partition_count(20));
    // EXPECT: p(20): 627

    // Test 2: Distinct parts = Odd parts (Euler's theorem)
    printf("dist(5): %d odd(5): %d\n", partition_distinct(5), partition_odd(5));
    // EXPECT: dist(5): 3 odd(5): 3
    printf("dist(8): %d odd(8): %d\n", partition_distinct(8), partition_odd(8));
    // EXPECT: dist(8): 6 odd(8): 6
    printf("dist(10): %d odd(10): %d\n", partition_distinct(10), partition_odd(10));
    // EXPECT: dist(10): 10 odd(10): 10
    printf("dist(15): %d odd(15): %d\n", partition_distinct(15), partition_odd(15));
    // EXPECT: dist(15): 27 odd(15): 27

    // Test 3: Partitions into at most k parts
    printf("atmost(5,1): %d\n", partition_at_most_k(5, 1));
    // EXPECT: atmost(5,1): 1
    printf("atmost(5,2): %d\n", partition_at_most_k(5, 2));
    // EXPECT: atmost(5,2): 3
    printf("atmost(5,3): %d\n", partition_at_most_k(5, 3));
    // EXPECT: atmost(5,3): 5
    printf("atmost(10,3): %d\n", partition_at_most_k(10, 3));
    // EXPECT: atmost(10,3): 14

    // Test 4: Partitions into exactly k parts
    printf("exact(5,1): %d\n", partition_exactly_k(5, 1));
    // EXPECT: exact(5,1): 1
    printf("exact(5,2): %d\n", partition_exactly_k(5, 2));
    // EXPECT: exact(5,2): 2
    printf("exact(5,3): %d\n", partition_exactly_k(5, 3));
    // EXPECT: exact(5,3): 2
    printf("exact(10,3): %d\n", partition_exactly_k(10, 3));
    // EXPECT: exact(10,3): 8

    // Test 5: Sum of exactly k for all k should equal total
    int sum = 0;
    int k;
    for (k = 1; k <= 8; k++) {
        sum = sum + partition_exactly_k(8, k);
    }
    printf("sum exact(8): %d = %d\n", sum, partition_count(8));
    // EXPECT: sum exact(8): 22 = 22

    // Test 6: Euler pentagonal formula matches DP
    printf("euler(5): %d\n", partition_euler(5));
    // EXPECT: euler(5): 7
    printf("euler(10): %d\n", partition_euler(10));
    // EXPECT: euler(10): 42
    printf("euler(20): %d\n", partition_euler(20));
    // EXPECT: euler(20): 627

    // Test 7: Pentagonal numbers
    printf("pent(1): %d\n", pentagonal(1));
    // EXPECT: pent(1): 1
    printf("pent(2): %d\n", pentagonal(2));
    // EXPECT: pent(2): 5
    printf("pent(3): %d\n", pentagonal(3));
    // EXPECT: pent(3): 12

    printf("done\n");
    // EXPECT: done
    return 0;
}
