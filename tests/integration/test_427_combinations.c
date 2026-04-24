int printf(const char *fmt, ...);
// EXPECT: C(4,2): (1,2) (1,3) (1,4) (2,3) (2,4) (3,4) \nCount: 6\nBinomial: 6\nC(5,3): (1,2,3) (1,2,4) (1,2,5) (1,3,4) (1,3,5) (1,4,5) (2,3,4) (2,3,5) (2,4,5) (3,4,5) \nCount: 10\nBinomial: 10\nC(3,1): (1) (2) (3) \nCount: 3\nC(6,3) = 20\nC(10,5) = 252\nC(7,0) = 1\nC(7,7) = 1
// Test: Generate combinations

int result[256][8];
int num_results;

void combine_helper(int n, int k, int start, int current[], int depth) {
    if (depth == k) {
        int i;
        for (i = 0; i < k; i++) {
            result[num_results][i] = current[i];
        }
        num_results++;
        return;
    }
    int i;
    for (i = start; i <= n; i++) {
        current[depth] = i;
        combine_helper(n, k, i + 1, current, depth + 1);
    }
}

void generate_combinations(int n, int k) {
    int current[8];
    num_results = 0;
    combine_helper(n, k, 1, current, 0);
}

void print_combinations(int k) {
    int i, j;
    for (i = 0; i < num_results; i++) {
        printf("(");
        for (j = 0; j < k; j++) {
            if (j > 0) printf(",");
            printf("%d", result[i][j]);
        }
        printf(") ");
    }
    printf("\n");
}

int binomial(int n, int k) {
    if (k == 0 || k == n) return 1;
    int dp[16][16];
    int i, j;
    for (i = 0; i <= n; i++) {
        for (j = 0; j < 16; j++) dp[i][j] = 0;
        dp[i][0] = 1;
        for (j = 1; j <= i; j++) {
            dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j];
        }
    }
    return dp[n][k];
}

int main(void) {
    generate_combinations(4, 2);
    printf("C(4,2): ");
    print_combinations(2);
    printf("Count: %d\n", num_results);
    printf("Binomial: %d\n", binomial(4, 2));

    generate_combinations(5, 3);
    printf("C(5,3): ");
    print_combinations(3);
    printf("Count: %d\n", num_results);
    printf("Binomial: %d\n", binomial(5, 3));

    generate_combinations(3, 1);
    printf("C(3,1): ");
    print_combinations(1);
    printf("Count: %d\n", num_results);

    printf("C(6,3) = %d\n", binomial(6, 3));
    printf("C(10,5) = %d\n", binomial(10, 5));
    printf("C(7,0) = %d\n", binomial(7, 0));
    printf("C(7,7) = %d\n", binomial(7, 7));

    return 0;
}
