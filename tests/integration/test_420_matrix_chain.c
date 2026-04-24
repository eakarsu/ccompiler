int printf(const char *fmt, ...);
// EXPECT: Cost 1: 4500\nOrder: ((M1 x M2) x M3)\nCost 2: 26000\nOrder: ((M1 x (M2 x M3)) x M4)\nCost 3: 30000\nOrder: (((M1 x M2) x M3) x M4)\nCost 4: 2010\nOrder: ((M1 x M2) x ((M3 x M4) x (M5 x M6)))
// Test: Matrix chain multiplication

int dp[16][16];
int split[16][16];

int matrix_chain(int dims[], int n) {
    int i, j, k, len;
    int INF = 999999999;

    for (i = 1; i < n; i++) {
        dp[i][i] = 0;
    }

    for (len = 2; len < n; len++) {
        for (i = 1; i <= n - len; i++) {
            j = i + len - 1;
            dp[i][j] = INF;
            for (k = i; k < j; k++) {
                int cost = dp[i][k] + dp[k + 1][j] + dims[i - 1] * dims[k] * dims[j];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    split[i][j] = k;
                }
            }
        }
    }
    return dp[1][n - 1];
}

void print_parens(int i, int j) {
    if (i == j) {
        printf("M%d", i);
        return;
    }
    printf("(");
    print_parens(i, split[i][j]);
    printf(" x ");
    print_parens(split[i][j] + 1, j);
    printf(")");
}

int main(void) {
    int d1[] = {10, 30, 5, 60};
    int n1 = 4;
    int cost1 = matrix_chain(d1, n1);
    printf("Cost 1: %d\n", cost1);
    printf("Order: ");
    print_parens(1, n1 - 1);
    printf("\n");

    int d2[] = {40, 20, 30, 10, 30};
    int n2 = 5;
    int cost2 = matrix_chain(d2, n2);
    printf("Cost 2: %d\n", cost2);
    printf("Order: ");
    print_parens(1, n2 - 1);
    printf("\n");

    int d3[] = {10, 20, 30, 40, 30};
    int n3 = 5;
    int cost3 = matrix_chain(d3, n3);
    printf("Cost 3: %d\n", cost3);
    printf("Order: ");
    print_parens(1, n3 - 1);
    printf("\n");

    int d4[] = {5, 10, 3, 12, 5, 50, 6};
    int n4 = 7;
    int cost4 = matrix_chain(d4, n4);
    printf("Cost 4: %d\n", cost4);
    printf("Order: ");
    print_parens(1, n4 - 1);
    printf("\n");

    return 0;
}
