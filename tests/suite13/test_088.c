int printf(const char *fmt, ...);

// Matrix Chain Multiplication - find optimal parenthesization

// dp[i][j] = min scalar multiplications to compute A_i * ... * A_j
// dims[]: dimension array where matrix i has dims dims[i] x dims[i+1]
// n = number of matrices, dims has n+1 elements

int mcm_min_cost(int *dims, int n) {
    int dp[16][16];
    int i;
    int j;
    int k;
    int len;
    int cost;

    // Initialize diagonal to 0 (single matrix = no cost)
    for (i = 0; i < n; i++) {
        dp[i][i] = 0;
    }

    // len is the chain length
    for (len = 2; len <= n; len++) {
        for (i = 0; i <= n - len; i++) {
            j = i + len - 1;
            dp[i][j] = 999999999;
            for (k = i; k < j; k++) {
                cost = dp[i][k] + dp[k + 1][j] + dims[i] * dims[k + 1] * dims[j + 1];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                }
            }
        }
    }

    return dp[0][n - 1];
}

// Also track the split points for parenthesization
int mcm_with_splits(int *dims, int n, int *splits) {
    int dp[16][16];
    int split[16][16];
    int i;
    int j;
    int k;
    int len;
    int cost;

    for (i = 0; i < n; i++) {
        dp[i][i] = 0;
    }

    for (len = 2; len <= n; len++) {
        for (i = 0; i <= n - len; i++) {
            j = i + len - 1;
            dp[i][j] = 999999999;
            for (k = i; k < j; k++) {
                cost = dp[i][k] + dp[k + 1][j] + dims[i] * dims[k + 1] * dims[j + 1];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    split[i][j] = k;
                }
            }
        }
    }

    // Copy splits to output for reconstruction
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            splits[i * n + j] = split[i][j];
        }
    }

    return dp[0][n - 1];
}

// Print optimal parenthesization using stored splits
// We use a simple approach: print split points at each level
void print_parens_helper(int *splits, int n, int i, int j) {
    if (i == j) {
        printf("M%d", i);
        return;
    }
    int k;
    k = splits[i * n + j];
    printf("(");
    print_parens_helper(splits, n, i, k);
    printf("*");
    print_parens_helper(splits, n, k + 1, j);
    printf(")");
}

// Compute dimensions of result matrix for chain i..j
int result_rows(int *dims, int i) {
    return dims[i];
}

int result_cols(int *dims, int j) {
    return dims[j + 1];
}

// Naive matrix multiply cost (no optimization): just left to right
int naive_cost(int *dims, int n) {
    int cost;
    int rows;
    int cols;
    int i;

    if (n <= 1) return 0;

    cost = 0;
    rows = dims[0];
    cols = dims[1];
    for (i = 1; i < n; i++) {
        // Multiply (rows x cols) * (dims[i] x dims[i+1])
        cost = cost + rows * cols * dims[i + 1];
        cols = dims[i + 1];
    }

    return cost;
}

// Maximum cost parenthesization (worst case)
int mcm_max_cost(int *dims, int n) {
    int dp[16][16];
    int i;
    int j;
    int k;
    int len;
    int cost;

    for (i = 0; i < n; i++) {
        dp[i][i] = 0;
    }

    for (len = 2; len <= n; len++) {
        for (i = 0; i <= n - len; i++) {
            j = i + len - 1;
            dp[i][j] = 0;
            for (k = i; k < j; k++) {
                cost = dp[i][k] + dp[k + 1][j] + dims[i] * dims[k + 1] * dims[j + 1];
                if (cost > dp[i][j]) {
                    dp[i][j] = cost;
                }
            }
        }
    }

    return dp[0][n - 1];
}

int main() {
    int dims1[5];
    int dims2[7];
    int dims3[4];
    int splits[256];
    int result;

    // Example 1: matrices 10x30, 30x5, 5x60
    // dims = {10, 30, 5, 60}, n=3 matrices
    // Options: (AB)C = 10*30*5 + 10*5*60 = 1500 + 3000 = 4500
    //          A(BC) = 30*5*60 + 10*30*60 = 9000 + 18000 = 27000
    // Optimal = 4500
    dims1[0] = 10; dims1[1] = 30; dims1[2] = 5; dims1[3] = 60;

    printf("%d\n", mcm_min_cost(dims1, 3));
    // EXPECT: 4500

    // Max cost
    printf("%d\n", mcm_max_cost(dims1, 3));
    // EXPECT: 27000

    // Naive (left to right) cost
    printf("%d\n", naive_cost(dims1, 3));
    // EXPECT: 4500

    // Example 2: dims = {40, 20, 30, 10, 30}, n=4 matrices
    // Classic textbook example
    dims1[0] = 40; dims1[1] = 20; dims1[2] = 30; dims1[3] = 10; dims1[4] = 30;

    printf("%d\n", mcm_min_cost(dims1, 4));
    // EXPECT: 26000

    // With splits: print optimal parenthesization
    result = mcm_with_splits(dims1, 4, splits);
    printf("%d\n", result);
    // EXPECT: 26000

    // Print parenthesization
    print_parens_helper(splits, 4, 0, 3);
    printf("\n");
    // EXPECT: ((M0*(M1*M2))*M3)

    // Example 3: dims = {10, 20, 30}, n=2 matrices
    // Only one way: 10*20*30 = 6000
    dims3[0] = 10; dims3[1] = 20; dims3[2] = 30;
    printf("%d\n", mcm_min_cost(dims3, 2));
    // EXPECT: 6000

    // Single matrix: cost = 0
    printf("%d\n", mcm_min_cost(dims3, 1));
    // EXPECT: 0

    // Example 4: 6 matrices
    // dims = {30, 35, 15, 5, 10, 20, 25}
    dims2[0]=30; dims2[1]=35; dims2[2]=15; dims2[3]=5;
    dims2[4]=10; dims2[5]=20; dims2[6]=25;

    printf("%d\n", mcm_min_cost(dims2, 6));
    // EXPECT: 15125

    // Result dimensions for chain 0..5: 30 rows, 25 cols
    printf("%d %d\n", result_rows(dims2, 0), result_cols(dims2, 5));
    // EXPECT: 30 25

    // Square matrices: dims={5,5,5,5}, n=3
    // All splits give same cost: 5*5*5 + 5*5*5 = 250
    dims3[0]=5; dims3[1]=5; dims3[2]=5; dims3[3]=5;
    printf("%d\n", mcm_min_cost(dims3, 3));
    // EXPECT: 250

    // Naive vs optimal: dims = {30,35,15,5,10,20,25}
    // Naive (left to right):
    // (30x35)*(35x15) = 30*35*15=15750, result 30x15
    // (30x15)*(15x5)  = 30*15*5=2250,   result 30x5
    // (30x5)*(5x10)   = 30*5*10=1500,   result 30x10
    // (30x10)*(10x20) = 30*10*20=6000,  result 30x20
    // (30x20)*(20x25) = 30*20*25=15000, result 30x25
    // Total = 15750+2250+1500+6000+15000 = 40500
    printf("%d\n", naive_cost(dims2, 6));
    // EXPECT: 40500

    // Savings
    printf("%d\n", naive_cost(dims2, 6) - mcm_min_cost(dims2, 6));
    // EXPECT: 25375

    printf("done\n");
    // EXPECT: done

    return 0;
}
