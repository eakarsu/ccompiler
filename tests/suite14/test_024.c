int printf(const char *fmt, ...);

// Catalan numbers and their applications

// Catalan(n) = C(2n, n) / (n+1)
// Also: C(0)=1, C(n) = sum C(i)*C(n-1-i) for i=0..n-1

int catalan_recursive_helper(int n, int *memo) {
    if (n <= 1) return 1;
    if (memo[n] != 0) return memo[n];
    int result = 0;
    int i;
    for (i = 0; i < n; i++) {
        result = result + catalan_recursive_helper(i, memo) * catalan_recursive_helper(n - 1 - i, memo);
    }
    memo[n] = result;
    return result;
}

int catalan(int n) {
    int memo[15];
    int i;
    for (i = 0; i < 15; i++) {
        memo[i] = 0;
    }
    return catalan_recursive_helper(n, memo);
}

// Direct formula using binomial coefficient
int choose(int n, int k) {
    if (k > n - k) k = n - k;
    int result = 1;
    int i;
    for (i = 0; i < k; i++) {
        result = result * (n - i);
        result = result / (i + 1);
    }
    return result;
}

int catalan_direct(int n) {
    return choose(2 * n, n) / (n + 1);
}

// Application 1: Number of valid parenthesizations with n pairs
// This equals Catalan(n)
// Verify by counting directly for small n
int valid_parens_count(int n) {
    // Use DP: dp[i][j] = ways with i open and j close remaining
    // Actually, count sequences of n '(' and n ')' that are valid
    // dp[len][balance] where balance = #open - #close so far
    int dp[13][13]; // dp[length][balance]
    int i;
    int j;
    for (i = 0; i <= 2 * n; i++) {
        for (j = 0; j <= n; j++) {
            dp[i][j] = 0;
        }
    }
    dp[0][0] = 1;
    int len;
    for (len = 0; len < 2 * n; len++) {
        for (j = 0; j <= n; j++) {
            if (dp[len][j] == 0) continue;
            // Add '(' -> balance increases
            if (j + 1 <= n) {
                dp[len + 1][j + 1] = dp[len + 1][j + 1] + dp[len][j];
            }
            // Add ')' -> balance decreases
            if (j - 1 >= 0) {
                dp[len + 1][j - 1] = dp[len + 1][j - 1] + dp[len][j];
            }
        }
    }
    return dp[2 * n][0];
}

// Application 2: Number of distinct BSTs with n nodes (also Catalan)
int count_bsts(int n) {
    int dp[12];
    int i;
    int j;
    dp[0] = 1;
    dp[1] = 1;
    for (i = 2; i <= n; i++) {
        dp[i] = 0;
        for (j = 0; j < i; j++) {
            dp[i] = dp[i] + dp[j] * dp[i - 1 - j];
        }
    }
    return dp[n];
}

// Application 3: Number of ways to triangulate a polygon with n+2 sides
// This is also Catalan(n)
int triangulations(int n) {
    return catalan(n);
}

// Application 4: Ballot problem - ways to count votes so one
// candidate is always ahead. With p>q votes, answer is (p-q)/(p+q) * C(p+q, p)
int ballot_sequences(int p, int q) {
    if (p <= q) return 0;
    int total = choose(p + q, p);
    return total * (p - q) / (p + q);
}

// Verify Catalan recurrence: C(n+1) = 2(2n+1)/(n+2) * C(n)
int verify_catalan_recurrence(int n) {
    int cn = catalan(n);
    int cn1 = catalan(n + 1);
    // C(n+1) * (n+2) should equal C(n) * 2*(2n+1)
    return cn1 * (n + 2) == cn * 2 * (2 * n + 1);
}

int main(void) {
    // Test 1: First several Catalan numbers
    printf("C(0): %d\n", catalan(0));
    // EXPECT: C(0): 1
    printf("C(1): %d\n", catalan(1));
    // EXPECT: C(1): 1
    printf("C(2): %d\n", catalan(2));
    // EXPECT: C(2): 2
    printf("C(3): %d\n", catalan(3));
    // EXPECT: C(3): 5
    printf("C(4): %d\n", catalan(4));
    // EXPECT: C(4): 14
    printf("C(5): %d\n", catalan(5));
    // EXPECT: C(5): 42
    printf("C(6): %d\n", catalan(6));
    // EXPECT: C(6): 132
    printf("C(7): %d\n", catalan(7));
    // EXPECT: C(7): 429

    // Test 2: Direct formula matches recursive
    int i;
    int all_match = 1;
    for (i = 0; i <= 10; i++) {
        if (catalan(i) != catalan_direct(i)) {
            all_match = 0;
        }
    }
    printf("formulas match: %d\n", all_match);
    // EXPECT: formulas match: 1

    // Test 3: Valid parenthesizations = Catalan
    printf("parens(1): %d\n", valid_parens_count(1));
    // EXPECT: parens(1): 1
    printf("parens(2): %d\n", valid_parens_count(2));
    // EXPECT: parens(2): 2
    printf("parens(3): %d\n", valid_parens_count(3));
    // EXPECT: parens(3): 5
    printf("parens(4): %d\n", valid_parens_count(4));
    // EXPECT: parens(4): 14
    printf("parens(5): %d\n", valid_parens_count(5));
    // EXPECT: parens(5): 42

    // Test 4: BSTs = Catalan
    printf("bst(1): %d\n", count_bsts(1));
    // EXPECT: bst(1): 1
    printf("bst(3): %d\n", count_bsts(3));
    // EXPECT: bst(3): 5
    printf("bst(5): %d\n", count_bsts(5));
    // EXPECT: bst(5): 42

    // Test 5: Triangulations = Catalan
    printf("tri(3): %d\n", triangulations(3));
    // EXPECT: tri(3): 5
    printf("tri(4): %d\n", triangulations(4));
    // EXPECT: tri(4): 14

    // Test 6: Ballot problem
    printf("ballot(3,1): %d\n", ballot_sequences(3, 1));
    // EXPECT: ballot(3,1): 2
    printf("ballot(4,2): %d\n", ballot_sequences(4, 2));
    // EXPECT: ballot(4,2): 5
    printf("ballot(5,1): %d\n", ballot_sequences(5, 1));
    // EXPECT: ballot(5,1): 4

    // Test 7: Catalan recurrence verification
    printf("recur(0): %d\n", verify_catalan_recurrence(0));
    // EXPECT: recur(0): 1
    printf("recur(3): %d\n", verify_catalan_recurrence(3));
    // EXPECT: recur(3): 1
    printf("recur(6): %d\n", verify_catalan_recurrence(6));
    // EXPECT: recur(6): 1

    // Test 8: Larger Catalan values
    printf("C(8): %d\n", catalan(8));
    // EXPECT: C(8): 1430
    printf("C(9): %d\n", catalan(9));
    // EXPECT: C(9): 4862
    printf("C(10): %d\n", catalan(10));
    // EXPECT: C(10): 16796

    printf("done\n");
    // EXPECT: done
    return 0;
}
