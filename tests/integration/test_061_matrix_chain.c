int printf(const char *fmt, ...);
// EXPECT: 1: 5000\n5000\n10500\n7500\n1\n5500\n2: 750\n1300\n0\n3: 78\n78\n0 0 0 0 0 \n24 24 40 30 \n0 1 2 3 \n4: 15125\n40500\n47500\n2 2 2\n5: 120\n120\n6: 81\n81\n7: 38\n38\n100\n8: 38\n100\n9: 808\n1200\n1200\n1\n10: 1 1 2 5 14 42 132 429 \n11: 15750 2625 750 1000 5000 \n7875 4375 2500 3500 \n12: 18000\n32000\n18000\n18000\n13: 2184\n2184\n3620\n5808\n1436\n1\n14: 4\n4\n15: 200\n200\n20000\n16: 5000\n5000\n0\n4750\n4750\n0\n17: 0\n18: 2060\n0 0 0 0 \n2060\n\n1750 108 432 840 100 \n3230

// ============================================================================
// Matrix Chain Multiplication - Dynamic Programming
// ============================================================================
// This file implements matrix chain multiplication using dynamic programming.
// We compute the optimal parenthesization cost using tabulation.
// We also compute the actual split points and verify the results.
// All matrices are represented by their dimensions (rows x cols).
// ============================================================================

// Maximum number of matrices in a chain
#define MAX_MATRICES 20

// DP table for minimum multiplication costs
int dp[MAX_MATRICES][MAX_MATRICES];

// Split table to record optimal split points
int split[MAX_MATRICES][MAX_MATRICES];

// Dimensions array: matrix i has dimensions dims[i] x dims[i+1]
int dims[MAX_MATRICES + 1];

// ============================================================================
// Utility: absolute value
// ============================================================================
int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

// ============================================================================
// Utility: minimum of two integers
// ============================================================================
int min_val(int a, int b) {
    if (a < b) return a;
    return b;
}

// ============================================================================
// Utility: maximum of two integers
// ============================================================================
int max_val(int a, int b) {
    if (a > b) return a;
    return b;
}

// ============================================================================
// Initialize the DP and split tables to zero
// ============================================================================
void init_tables(int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            dp[i][j] = 0;
            split[i][j] = 0;
        }
    }
}

// ============================================================================
// Matrix Chain Order: compute minimum number of scalar multiplications
// n = number of matrices
// dims[] has n+1 entries
// ============================================================================
int matrix_chain_order(int n) {
    int i;
    int j;
    int k;
    int len;
    int cost;

    // Initialize diagonal to 0 (single matrix, no multiplication)
    for (i = 0; i < n; i = i + 1) {
        dp[i][i] = 0;
    }

    // len is the chain length
    for (len = 2; len <= n; len = len + 1) {
        for (i = 0; i <= n - len; i = i + 1) {
            j = i + len - 1;
            dp[i][j] = 999999999; // large number as initial
            for (k = i; k < j; k = k + 1) {
                // Cost = cost of left subchain + cost of right subchain
                //        + cost of multiplying the two resulting matrices
                cost = dp[i][k] + dp[k + 1][j] + dims[i] * dims[k + 1] * dims[j + 1];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    split[i][j] = k;
                }
            }
        }
    }

    return dp[0][n - 1];
}

// ============================================================================
// Print the optimal parenthesization recursively
// We print it as a sequence of split decisions
// ============================================================================
void print_parens(int i, int j) {
    if (i == j) {
        printf("%c", 'A' + i);
        return;
    }
    printf("%c", '(');
    print_parens(i, split[i][j]);
    print_parens(split[i][j] + 1, j);
    printf("%c", ')');
}

// ============================================================================
// Count the number of different parenthesizations for n matrices
// This is the (n-1)th Catalan number
// ============================================================================
int count_parenthesizations(int n) {
    int cat[MAX_MATRICES];
    int i;
    int j;
    int sum;

    if (n <= 1) return 1;

    cat[0] = 1;
    cat[1] = 1;

    for (i = 2; i < n; i = i + 1) {
        cat[i] = 0;
        for (j = 0; j < i; j = j + 1) {
            cat[i] = cat[i] + cat[j] * cat[i - 1 - j];
        }
    }

    return cat[n - 1];
}

// ============================================================================
// Compute the cost of a specific parenthesization given by a split sequence
// This is used to verify that the DP result is indeed optimal
// ============================================================================
int compute_cost_with_split(int i, int j) {
    if (i == j) return 0;
    int k;
    k = split[i][j];
    int left_cost;
    int right_cost;
    int merge_cost;
    left_cost = compute_cost_with_split(i, k);
    right_cost = compute_cost_with_split(k + 1, j);
    merge_cost = dims[i] * dims[k + 1] * dims[j + 1];
    return left_cost + right_cost + merge_cost;
}

// ============================================================================
// Compute the naive (left-to-right) multiplication cost
// This multiplies ((((A1*A2)*A3)*A4)...*An)
// ============================================================================
int naive_cost(int n) {
    int total;
    int rows;
    int cols;
    int i;

    if (n <= 1) return 0;

    total = 0;
    rows = dims[0];
    cols = dims[1];

    for (i = 1; i < n; i = i + 1) {
        total = total + rows * cols * dims[i + 1];
        cols = dims[i + 1];
    }

    return total;
}

// ============================================================================
// Compute the right-to-left multiplication cost
// This multiplies (A1*(A2*(A3*(...*An))))
// ============================================================================
int right_to_left_cost(int n) {
    int total;
    int rows;
    int cols;
    int i;

    if (n <= 1) return 0;

    total = 0;
    rows = dims[n - 1];
    cols = dims[n];

    for (i = n - 2; i >= 0; i = i - 1) {
        total = total + dims[i] * rows * cols;
        rows = dims[i];
    }

    return total;
}

// ============================================================================
// Test Case 1: Classic example with 4 matrices
// Dimensions: 10x30, 30x5, 5x60, 60x10
// ============================================================================
void test_case_1(void) {
    int n;
    int result;
    int naive;
    int rtl;
    int verified;

    printf("%d", 1); // Test case number
    printf("%c", ':');
    printf("%c", ' ');

    n = 4;
    dims[0] = 10;
    dims[1] = 30;
    dims[2] = 5;
    dims[3] = 60;
    dims[4] = 10;

    init_tables(n);
    result = matrix_chain_order(n);
    printf("%d", result);
    printf("%c", '\n');

    // Verify by recomputing with stored splits
    verified = compute_cost_with_split(0, n - 1);
    printf("%d", verified);
    printf("%c", '\n');

    // Naive cost
    naive = naive_cost(n);
    printf("%d", naive);
    printf("%c", '\n');

    // Right-to-left cost
    rtl = right_to_left_cost(n);
    printf("%d", rtl);
    printf("%c", '\n');

    // Print split points
    printf("%d", split[0][3]);
    printf("%c", '\n');

    // Print savings compared to naive
    printf("%d", naive - result);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 2: Three matrices
// Dimensions: 5x10, 10x20, 20x3
// ============================================================================
void test_case_2(void) {
    int n;
    int result;
    int naive;

    printf("%d", 2);
    printf("%c", ':');
    printf("%c", ' ');

    n = 3;
    dims[0] = 5;
    dims[1] = 10;
    dims[2] = 20;
    dims[3] = 3;

    init_tables(n);
    result = matrix_chain_order(n);
    printf("%d", result);
    printf("%c", '\n');

    naive = naive_cost(n);
    printf("%d", naive);
    printf("%c", '\n');

    // Split point
    printf("%d", split[0][2]);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 3: Five matrices
// Dimensions: 2x3, 3x4, 4x2, 2x5, 5x3
// ============================================================================
void test_case_3(void) {
    int n;
    int result;
    int verified;
    int i;
    int j;

    printf("%d", 3);
    printf("%c", ':');
    printf("%c", ' ');

    n = 5;
    dims[0] = 2;
    dims[1] = 3;
    dims[2] = 4;
    dims[3] = 2;
    dims[4] = 5;
    dims[5] = 3;

    init_tables(n);
    result = matrix_chain_order(n);
    printf("%d", result);
    printf("%c", '\n');

    verified = compute_cost_with_split(0, n - 1);
    printf("%d", verified);
    printf("%c", '\n');

    // Print the full DP table diagonal values
    for (i = 0; i < n; i = i + 1) {
        printf("%d", dp[i][i]);
        printf("%c", ' ');
    }
    printf("%c", '\n');

    // Print dp[i][i+1] values (chains of length 2)
    for (i = 0; i < n - 1; i = i + 1) {
        printf("%d", dp[i][i + 1]);
        printf("%c", ' ');
    }
    printf("%c", '\n');

    // Print split points for chains of length 2
    for (i = 0; i < n - 1; i = i + 1) {
        printf("%d", split[i][i + 1]);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 4: Six matrices with larger dimensions
// Dimensions: 30x35, 35x15, 15x5, 5x10, 10x20, 20x25
// ============================================================================
void test_case_4(void) {
    int n;
    int result;
    int naive;
    int rtl;

    printf("%d", 4);
    printf("%c", ':');
    printf("%c", ' ');

    n = 6;
    dims[0] = 30;
    dims[1] = 35;
    dims[2] = 15;
    dims[3] = 5;
    dims[4] = 10;
    dims[5] = 20;
    dims[6] = 25;

    init_tables(n);
    result = matrix_chain_order(n);
    printf("%d", result);
    printf("%c", '\n');

    naive = naive_cost(n);
    printf("%d", naive);
    printf("%c", '\n');

    rtl = right_to_left_cost(n);
    printf("%d", rtl);
    printf("%c", '\n');

    // Print all split points
    printf("%d", split[0][5]);
    printf("%c", ' ');
    printf("%d", split[0][4]);
    printf("%c", ' ');
    printf("%d", split[1][5]);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 5: Two matrices (trivial)
// Dimensions: 4x5, 5x6
// ============================================================================
void test_case_5(void) {
    int n;
    int result;

    printf("%d", 5);
    printf("%c", ':');
    printf("%c", ' ');

    n = 2;
    dims[0] = 4;
    dims[1] = 5;
    dims[2] = 6;

    init_tables(n);
    result = matrix_chain_order(n);
    printf("%d", result);
    printf("%c", '\n');

    // For 2 matrices, cost is simply p*q*r
    printf("%d", 4 * 5 * 6);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 6: Identical square matrices
// All 4 matrices are 3x3
// ============================================================================
void test_case_6(void) {
    int n;
    int result;
    int i;

    printf("%d", 6);
    printf("%c", ':');
    printf("%c", ' ');

    n = 4;
    for (i = 0; i <= n; i = i + 1) {
        dims[i] = 3;
    }

    init_tables(n);
    result = matrix_chain_order(n);
    printf("%d", result);
    printf("%c", '\n');

    // For identical square matrices of size s, any parenthesization gives same cost
    // Cost for n matrices of size s: (n-1)*s^3
    printf("%d", 3 * 27);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 7: Increasing dimensions
// Dimensions: 1x2, 2x3, 3x4, 4x5
// ============================================================================
void test_case_7(void) {
    int n;
    int result;
    int naive;
    int rtl;

    printf("%d", 7);
    printf("%c", ':');
    printf("%c", ' ');

    n = 4;
    dims[0] = 1;
    dims[1] = 2;
    dims[2] = 3;
    dims[3] = 4;
    dims[4] = 5;

    init_tables(n);
    result = matrix_chain_order(n);
    printf("%d", result);
    printf("%c", '\n');

    naive = naive_cost(n);
    printf("%d", naive);
    printf("%c", '\n');

    rtl = right_to_left_cost(n);
    printf("%d", rtl);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 8: Decreasing dimensions
// Dimensions: 5x4, 4x3, 3x2, 2x1
// ============================================================================
void test_case_8(void) {
    int n;
    int result;
    int naive;

    printf("%d", 8);
    printf("%c", ':');
    printf("%c", ' ');

    n = 4;
    dims[0] = 5;
    dims[1] = 4;
    dims[2] = 3;
    dims[3] = 2;
    dims[4] = 1;

    init_tables(n);
    result = matrix_chain_order(n);
    printf("%d", result);
    printf("%c", '\n');

    naive = naive_cost(n);
    printf("%d", naive);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 9: Single large dimension in the middle
// Dimensions: 2x100, 100x2, 2x100, 100x2
// ============================================================================
void test_case_9(void) {
    int n;
    int result;
    int naive;
    int rtl;

    printf("%d", 9);
    printf("%c", ':');
    printf("%c", ' ');

    n = 4;
    dims[0] = 2;
    dims[1] = 100;
    dims[2] = 2;
    dims[3] = 100;
    dims[4] = 2;

    init_tables(n);
    result = matrix_chain_order(n);
    printf("%d", result);
    printf("%c", '\n');

    naive = naive_cost(n);
    printf("%d", naive);
    printf("%c", '\n');

    rtl = right_to_left_cost(n);
    printf("%d", rtl);
    printf("%c", '\n');

    printf("%d", split[0][3]);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 10: Count parenthesizations
// ============================================================================
void test_case_10(void) {
    int i;

    printf("%d", 10);
    printf("%c", ':');
    printf("%c", ' ');

    // Catalan numbers C(n-1) for n=1..8
    for (i = 1; i <= 8; i = i + 1) {
        printf("%d", count_parenthesizations(i));
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 11: Verify subproblem costs
// Using the 6-matrix example from test_case_4
// ============================================================================
void test_case_11(void) {
    int n;
    int result;
    int i;
    int j;

    printf("%d", 11);
    printf("%c", ':');
    printf("%c", ' ');

    n = 6;
    dims[0] = 30;
    dims[1] = 35;
    dims[2] = 15;
    dims[3] = 5;
    dims[4] = 10;
    dims[5] = 20;
    dims[6] = 25;

    init_tables(n);
    result = matrix_chain_order(n);

    // Print subproblem costs for chains of length 2
    for (i = 0; i < n - 1; i = i + 1) {
        printf("%d", dp[i][i + 1]);
        printf("%c", ' ');
    }
    printf("%c", '\n');

    // Print subproblem costs for chains of length 3
    for (i = 0; i < n - 2; i = i + 1) {
        printf("%d", dp[i][i + 2]);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 12: Comparison of all parenthesizations for 3 matrices
// Dimensions: 10x20, 20x30, 30x40
// Only 2 parenthesizations: (AB)C and A(BC)
// ============================================================================
void test_case_12(void) {
    int cost1;
    int cost2;

    printf("%d", 12);
    printf("%c", ':');
    printf("%c", ' ');

    // (AB)C: cost = 10*20*30 + 10*30*40 = 6000 + 12000 = 18000
    cost1 = 10 * 20 * 30 + 10 * 30 * 40;
    printf("%d", cost1);
    printf("%c", '\n');

    // A(BC): cost = 20*30*40 + 10*20*40 = 24000 + 8000 = 32000
    cost2 = 20 * 30 * 40 + 10 * 20 * 40;
    printf("%d", cost2);
    printf("%c", '\n');

    // Minimum
    printf("%d", min_val(cost1, cost2));
    printf("%c", '\n');

    // Verify with DP
    dims[0] = 10;
    dims[1] = 20;
    dims[2] = 30;
    dims[3] = 40;
    init_tables(3);
    printf("%d", matrix_chain_order(3));
    printf("%c", '\n');
}

// ============================================================================
// Test Case 13: Stress test with 7 matrices
// Dimensions: 5x10, 10x3, 3x12, 12x5, 5x50, 50x6, 6x8
// ============================================================================
void test_case_13(void) {
    int n;
    int result;
    int naive;
    int rtl;
    int verified;

    printf("%d", 13);
    printf("%c", ':');
    printf("%c", ' ');

    n = 7;
    dims[0] = 5;
    dims[1] = 10;
    dims[2] = 3;
    dims[3] = 12;
    dims[4] = 5;
    dims[5] = 50;
    dims[6] = 6;
    dims[7] = 8;

    init_tables(n);
    result = matrix_chain_order(n);
    printf("%d", result);
    printf("%c", '\n');

    verified = compute_cost_with_split(0, n - 1);
    printf("%d", verified);
    printf("%c", '\n');

    naive = naive_cost(n);
    printf("%d", naive);
    printf("%c", '\n');

    rtl = right_to_left_cost(n);
    printf("%d", rtl);
    printf("%c", '\n');

    // Savings
    printf("%d", naive - result);
    printf("%c", '\n');

    // Split at top level
    printf("%d", split[0][6]);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 14: All dimensions equal to 1 (trivial cost)
// ============================================================================
void test_case_14(void) {
    int n;
    int result;
    int i;

    printf("%d", 14);
    printf("%c", ':');
    printf("%c", ' ');

    n = 5;
    for (i = 0; i <= n; i = i + 1) {
        dims[i] = 1;
    }

    init_tables(n);
    result = matrix_chain_order(n);
    printf("%d", result);
    printf("%c", '\n');

    // Each multiplication costs 1*1*1 = 1, and we need n-1 multiplications
    printf("%d", n - 1);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 15: Asymmetric dimensions
// Dimensions: 1x100, 100x1, 1x100
// ============================================================================
void test_case_15(void) {
    int n;
    int result;
    int cost1;
    int cost2;

    printf("%d", 15);
    printf("%c", ':');
    printf("%c", ' ');

    n = 3;
    dims[0] = 1;
    dims[1] = 100;
    dims[2] = 1;
    dims[3] = 100;

    init_tables(n);
    result = matrix_chain_order(n);
    printf("%d", result);
    printf("%c", '\n');

    // (AB)C: 1*100*1 + 1*1*100 = 100 + 100 = 200
    cost1 = 1 * 100 * 1 + 1 * 1 * 100;
    printf("%d", cost1);
    printf("%c", '\n');

    // A(BC): 100*1*100 + 1*100*100 = 10000 + 10000 = 20000
    cost2 = 100 * 1 * 100 + 1 * 100 * 100;
    printf("%d", cost2);
    printf("%c", '\n');
}

// ============================================================================
// Helper: compute a brute force check for 4 matrices by enumerating all 5
// parenthesizations. Returns the minimum cost.
// ============================================================================
int brute_force_4(int p0, int p1, int p2, int p3, int p4) {
    int costs[5];
    int best;
    int i;

    // ((AB)(CD))
    costs[0] = p0*p1*p2 + p2*p3*p4 + p0*p2*p4;
    // (((AB)C)D)
    costs[1] = p0*p1*p2 + p0*p2*p3 + p0*p3*p4;
    // ((A(BC))D)
    costs[2] = p1*p2*p3 + p0*p1*p3 + p0*p3*p4;
    // (A((BC)D))
    costs[3] = p1*p2*p3 + p1*p3*p4 + p0*p1*p4;
    // (A(B(CD)))
    costs[4] = p2*p3*p4 + p1*p2*p4 + p0*p1*p4;

    best = costs[0];
    for (i = 1; i < 5; i = i + 1) {
        if (costs[i] < best) {
            best = costs[i];
        }
    }

    return best;
}

// ============================================================================
// Test Case 16: Brute force verification for 4 matrices
// ============================================================================
void test_case_16(void) {
    int bf_result;
    int dp_result;

    printf("%d", 16);
    printf("%c", ':');
    printf("%c", ' ');

    // Using dims from test_case_1: 10, 30, 5, 60, 10
    bf_result = brute_force_4(10, 30, 5, 60, 10);
    printf("%d", bf_result);
    printf("%c", '\n');

    dims[0] = 10;
    dims[1] = 30;
    dims[2] = 5;
    dims[3] = 60;
    dims[4] = 10;
    init_tables(4);
    dp_result = matrix_chain_order(4);
    printf("%d", dp_result);
    printf("%c", '\n');

    // Should match
    printf("%d", bf_result - dp_result);
    printf("%c", '\n');

    // Another set: 5, 10, 20, 15, 30
    bf_result = brute_force_4(5, 10, 20, 15, 30);
    printf("%d", bf_result);
    printf("%c", '\n');

    dims[0] = 5;
    dims[1] = 10;
    dims[2] = 20;
    dims[3] = 15;
    dims[4] = 30;
    init_tables(4);
    dp_result = matrix_chain_order(4);
    printf("%d", dp_result);
    printf("%c", '\n');

    printf("%d", bf_result - dp_result);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 17: Edge case - single matrix
// ============================================================================
void test_case_17(void) {
    int n;
    int result;

    printf("%d", 17);
    printf("%c", ':');
    printf("%c", ' ');

    n = 1;
    dims[0] = 10;
    dims[1] = 20;

    init_tables(n);
    result = matrix_chain_order(n);
    printf("%d", result);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 18: Reconstruct and verify parenthesization for 5 matrices
// ============================================================================
void test_case_18(void) {
    int n;
    int result;
    int i;

    printf("%d", 18);
    printf("%c", ':');
    printf("%c", ' ');

    n = 5;
    dims[0] = 10;
    dims[1] = 5;
    dims[2] = 20;
    dims[3] = 8;
    dims[4] = 15;
    dims[5] = 6;

    init_tables(n);
    result = matrix_chain_order(n);
    printf("%d", result);
    printf("%c", '\n');

    // Print all split points for the full range
    for (i = 0; i < n - 1; i = i + 1) {
        printf("%d", split[0][i + 1]);
        printf("%c", ' ');
    }
    printf("%c", '\n');

    // Verify cost
    printf("%d", compute_cost_with_split(0, n - 1));
    printf("%c", '\n');
}

// ============================================================================
// Summary statistics function: runs multiple test configurations and
// prints a summary
// ============================================================================
void print_summary(void) {
    int configs[5][5];
    int i;
    int j;
    int n;
    int result;
    int total;

    printf("%c", '\n');

    // Config 0: 3 matrices
    configs[0][0] = 3; configs[0][1] = 10; configs[0][2] = 20; configs[0][3] = 5; configs[0][4] = 15;
    // Config 1: 3 matrices
    configs[1][0] = 3; configs[1][1] = 2; configs[1][2] = 8; configs[1][3] = 3; configs[1][4] = 10;
    // Config 2: 3 matrices
    configs[2][0] = 3; configs[2][1] = 7; configs[2][2] = 4; configs[2][3] = 11; configs[2][4] = 6;
    // Config 3: 3 matrices
    configs[3][0] = 3; configs[3][1] = 15; configs[3][2] = 3; configs[3][3] = 20; configs[3][4] = 8;
    // Config 4: 3 matrices
    configs[4][0] = 3; configs[4][1] = 1; configs[4][2] = 50; configs[4][3] = 1; configs[4][4] = 50;

    total = 0;
    for (i = 0; i < 5; i = i + 1) {
        n = configs[i][0];
        for (j = 0; j <= n; j = j + 1) {
            dims[j] = configs[i][j + 1];
        }
        init_tables(n);
        result = matrix_chain_order(n);
        printf("%d", result);
        printf("%c", ' ');
        total = total + result;
    }
    printf("%c", '\n');

    printf("%d", total);
    printf("%c", '\n');
}

// ============================================================================
// Main function
// ============================================================================

// EXPECT: 1: 4500
// 4500
// 27000
// 21000
// 1
// 22500
// 2: 750
// 1300
// 0
// 3: 78
// 78
// 0 0 0 0 0
// 24 24 16 30
// 0 1 2 3
// 4: 15125
// 64500
// 40500
// 2 0 2
// 5: 120
// 120
// 6: 81
// 81
// 7: 38
// 38
// 58
// 8: 38
// 58
// 9: 1208
// 40800
// 40800
// 1
// 10: 1 1 2 5 14 42 132 429
// 11: 15750 2625 750 1500 5000
// 7875 2500 750 3000
// 12: 18000
// 32000
// 18000
// 18000
// 13: 2010
// 2010
// 5700
// 7340
// 3690
// 4
// 14: 4
// 4
// 15: 200
// 200
// 20000
// 16: 4500
// 4500
// 0
// 3000
// 3000
// 0
// 17: 0
// 18: 2620
// 0 0 0 0
// 2620
//
// 1550 218 622 810 100
// 3300

int main(void) {
    test_case_1();
    test_case_2();
    test_case_3();
    test_case_4();
    test_case_5();
    test_case_6();
    test_case_7();
    test_case_8();
    test_case_9();
    test_case_10();
    test_case_11();
    test_case_12();
    test_case_13();
    test_case_14();
    test_case_15();
    test_case_16();
    test_case_17();
    test_case_18();
    print_summary();
    return 0;
}
