int printf(const char *fmt, ...);

// Matrix Chain Multiplication: minimum cost of multiplying matrices

// Given dimensions p[0..n], matrix i has dimensions p[i-1] x p[i]
// Returns minimum number of scalar multiplications
int matrix_chain_cost(int *p, int n) {
    // dp[i][j] = min cost to multiply matrices i..j (1-indexed)
    // Flat array: dp[i*16+j], max 15 matrices
    int dp[256];
    int i;
    int j;
    int k;
    int len;

    // Initialize
    for (i = 0; i < 256; i++) {
        dp[i] = 0;
    }

    // len is chain length
    for (len = 2; len <= n; len++) {
        for (i = 1; i <= n - len + 1; i++) {
            j = i + len - 1;
            dp[i * 16 + j] = 999999999;
            for (k = i; k < j; k++) {
                int cost = dp[i * 16 + k] + dp[(k + 1) * 16 + j]
                         + p[i - 1] * p[k] * p[j];
                if (cost < dp[i * 16 + j]) {
                    dp[i * 16 + j] = cost;
                }
            }
        }
    }

    return dp[1 * 16 + n];
}

// Also find the optimal parenthesization split points
int matrix_chain_split(int *p, int n, int *split) {
    int dp[256];
    int i;
    int j;
    int k;
    int len;

    for (i = 0; i < 256; i++) {
        dp[i] = 0;
        split[i] = 0;
    }

    for (len = 2; len <= n; len++) {
        for (i = 1; i <= n - len + 1; i++) {
            j = i + len - 1;
            dp[i * 16 + j] = 999999999;
            for (k = i; k < j; k++) {
                int cost = dp[i * 16 + k] + dp[(k + 1) * 16 + j]
                         + p[i - 1] * p[k] * p[j];
                if (cost < dp[i * 16 + j]) {
                    dp[i * 16 + j] = cost;
                    split[i * 16 + j] = k;
                }
            }
        }
    }

    return dp[1 * 16 + n];
}

// Recursive (top-down) matrix chain multiplication
int mcm_recursive(int *p, int i, int j) {
    if (i == j) return 0;
    int min_cost = 999999999;
    int k;
    for (k = i; k < j; k++) {
        int cost = mcm_recursive(p, i, k) + mcm_recursive(p, k + 1, j)
                 + p[i - 1] * p[k] * p[j];
        if (cost < min_cost) {
            min_cost = cost;
        }
    }
    return min_cost;
}

// Count number of possible parenthesizations for n matrices
// This is the Catalan number C(n-1)
int count_parenthesizations(int n) {
    if (n <= 1) return 1;
    int dp[16];
    int i;
    int k;
    for (i = 0; i < 16; i++) dp[i] = 0;
    dp[0] = 1;
    dp[1] = 1;
    for (i = 2; i < n; i++) {
        dp[i] = 0;
        for (k = 0; k < i; k++) {
            dp[i] = dp[i] + dp[k] * dp[i - 1 - k];
        }
    }
    return dp[n - 1];
}

// Compute total cost of a specific ordering (left-to-right, no optimization)
int naive_cost(int *p, int n) {
    // Multiply matrices left to right: ((M1*M2)*M3)*...
    if (n <= 1) return 0;
    int rows = p[0];
    int cols = p[1];
    int total = 0;
    int i;
    for (i = 2; i <= n; i++) {
        total = total + rows * cols * p[i];
        cols = p[i];
    }
    return total;
}

int main() {
    // Test case 1: 4 matrices
    // A1: 10x30, A2: 30x5, A3: 5x60
    int p1[4];
    p1[0] = 10; p1[1] = 30; p1[2] = 5; p1[3] = 60;
    // EXPECT: mcm1 = 4500
    printf("mcm1 = %d\n", matrix_chain_cost(p1, 3));

    // Test case 2: 4 matrices
    // A1: 40x20, A2: 20x30, A3: 30x10, A4: 10x30
    int p2[5];
    p2[0] = 40; p2[1] = 20; p2[2] = 30; p2[3] = 10; p2[4] = 30;
    // EXPECT: mcm2 = 26000
    printf("mcm2 = %d\n", matrix_chain_cost(p2, 4));

    // Test case 3: 3 matrices
    int p3[4];
    p3[0] = 10; p3[1] = 20; p3[2] = 30; p3[3] = 40;
    // EXPECT: mcm3 = 18000
    printf("mcm3 = %d\n", matrix_chain_cost(p3, 3));

    // Test case 4: 5 matrices
    int p4[6];
    p4[0] = 30; p4[1] = 35; p4[2] = 15; p4[3] = 5; p4[4] = 10; p4[5] = 20;
    // EXPECT: mcm4 = 11875
    printf("mcm4 = %d\n", matrix_chain_cost(p4, 5));

    // Verify recursive matches DP for small cases
    // EXPECT: rec1 = 4500
    printf("rec1 = %d\n", mcm_recursive(p1, 1, 3));
    // EXPECT: rec3 = 18000
    printf("rec3 = %d\n", mcm_recursive(p3, 1, 3));

    // Test split tracking
    int split[256];
    int cost = matrix_chain_split(p4, 5, split);
    // EXPECT: split_cost = 11875
    printf("split_cost = %d\n", cost);
    // The optimal split for the full range [1..5]
    // EXPECT: split[1,5] = 3
    printf("split[1,5] = %d\n", split[1 * 16 + 5]);

    // Count parenthesizations
    // EXPECT: parens(2) = 1
    printf("parens(2) = %d\n", count_parenthesizations(2));
    // EXPECT: parens(3) = 2
    printf("parens(3) = %d\n", count_parenthesizations(3));
    // EXPECT: parens(4) = 5
    printf("parens(4) = %d\n", count_parenthesizations(4));
    // EXPECT: parens(5) = 14
    printf("parens(5) = %d\n", count_parenthesizations(5));
    // EXPECT: parens(6) = 42
    printf("parens(6) = %d\n", count_parenthesizations(6));

    // Compare naive vs optimal
    int naive1 = naive_cost(p1, 3);
    int opt1 = matrix_chain_cost(p1, 3);
    // EXPECT: naive1 = 4500
    printf("naive1 = %d\n", naive1);
    // EXPECT: opt_better = 0
    printf("opt_better = %d\n", opt1 < naive1 ? 1 : 0);

    // Edge: 2 matrices, only one way
    int p5[3];
    p5[0] = 5; p5[1] = 10; p5[2] = 20;
    // EXPECT: mcm_2mat = 1000
    printf("mcm_2mat = %d\n", matrix_chain_cost(p5, 2));

    // Single matrix: no multiplication needed
    int p6[2];
    p6[0] = 5; p6[1] = 10;
    // EXPECT: mcm_1mat = 0
    printf("mcm_1mat = %d\n", matrix_chain_cost(p6, 1));

    return 0;
}
