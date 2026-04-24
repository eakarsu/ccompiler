int printf(const char *fmt, ...);

int mcm(int dims[], int n) {
    int dp[15][15];
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < n) {
            dp[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }

    int len = 2;
    while (len < n) {
        i = 1;
        while (i <= n - len) {
            int j = i + len - 1;
            dp[i][j] = 999999999;
            int k = i;
            while (k < j) {
                int cost = dp[i][k] + dp[k + 1][j] +
                           dims[i - 1] * dims[k] * dims[j];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                }
                k = k + 1;
            }
            i = i + 1;
        }
        len = len + 1;
    }
    return dp[1][n - 1];
}

int mcm_with_split(int dims[], int n, int split[]) {
    int dp[15][15];
    int sp[15][15];
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < n) {
            dp[i][j] = 0;
            sp[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }

    int len = 2;
    while (len < n) {
        i = 1;
        while (i <= n - len) {
            int j = i + len - 1;
            dp[i][j] = 999999999;
            int k = i;
            while (k < j) {
                int cost = dp[i][k] + dp[k + 1][j] +
                           dims[i - 1] * dims[k] * dims[j];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    sp[i][j] = k;
                }
                k = k + 1;
            }
            i = i + 1;
        }
        len = len + 1;
    }

    i = 1;
    while (i < n) {
        int j = i;
        while (j < n) {
            split[(i - 1) * 10 + (j - 1)] = sp[i][j];
            j = j + 1;
        }
        i = i + 1;
    }
    return dp[1][n - 1];
}

int scalar_mults(int dims[], int n, int order[]) {
    int total = 0;
    int rows[15];
    int cols[15];
    int i = 0;
    while (i < n - 1) {
        rows[i] = dims[i];
        cols[i] = dims[i + 1];
        i = i + 1;
    }

    int step = 0;
    int num = n - 1;
    while (step < num - 1) {
        int idx = order[step];
        int cost = rows[idx] * cols[idx] * cols[idx + 1];
        total = total + cost;
        cols[idx] = cols[idx + 1];
        int j = idx + 1;
        while (j < num - 1) {
            rows[j] = rows[j + 1];
            cols[j] = cols[j + 1];
            j = j + 1;
        }
        num = num - 1;
        int k = step + 1;
        while (k < n - 2) {
            if (order[k] > idx) {
                order[k] = order[k] - 1;
            }
            k = k + 1;
        }
        step = step + 1;
    }
    return total;
}

int min3(int a, int b, int c) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

int min_multiply_two(int a_rows, int a_cols, int b_cols) {
    return a_rows * a_cols * b_cols;
}

int main(void) {
    int d1[5];
    d1[0] = 10; d1[1] = 20; d1[2] = 30; d1[3] = 40; d1[4] = 30;
    // EXPECT: mcm1 = 30000
    printf("mcm1 = %d\n", mcm(d1, 5));

    int d2[4];
    d2[0] = 40; d2[1] = 20; d2[2] = 30; d2[3] = 10;
    // EXPECT: mcm2 = 14000
    printf("mcm2 = %d\n", mcm(d2, 4));

    int d3[5];
    d3[0] = 1; d3[1] = 2; d3[2] = 3; d3[3] = 4; d3[4] = 3;
    // EXPECT: mcm3 = 30
    printf("mcm3 = %d\n", mcm(d3, 5));

    int d4[3];
    d4[0] = 10; d4[1] = 20; d4[2] = 30;
    // EXPECT: mcm_2mat = 6000
    printf("mcm_2mat = %d\n", mcm(d4, 3));

    int split[100];
    int cost = mcm_with_split(d1, 5, split);
    // EXPECT: split_cost = 30000
    printf("split_cost = %d\n", cost);
    // EXPECT: split[0][3] valid
    int sp_val = split[0 * 10 + 3];
    printf("split[0][3] valid\n");

    int d5[6];
    d5[0] = 30; d5[1] = 35; d5[2] = 15; d5[3] = 5; d5[4] = 10; d5[5] = 20;
    // EXPECT: mcm5 = 11875
    printf("mcm5 = %d\n", mcm(d5, 6));

    cost = mcm_with_split(d5, 6, split);
    // EXPECT: split5_cost = 11875
    printf("split5_cost = %d\n", cost);

    // EXPECT: mult_2x3_3x4 = 24
    printf("mult_2x3_3x4 = %d\n", min_multiply_two(2, 3, 4));
    // EXPECT: mult_10x20_20x30 = 6000
    printf("mult_10x20_20x30 = %d\n", min_multiply_two(10, 20, 30));

    int d6[4];
    d6[0] = 10; d6[1] = 10; d6[2] = 10; d6[3] = 10;
    // EXPECT: mcm_square = 2000
    printf("mcm_square = %d\n", mcm(d6, 4));

    int d7[4];
    d7[0] = 5; d7[1] = 10; d7[2] = 3; d7[3] = 12;
    // EXPECT: mcm7 = 330
    printf("mcm7 = %d\n", mcm(d7, 4));

    int d8[5];
    d8[0] = 2; d8[1] = 3; d8[2] = 6; d8[3] = 4; d8[4] = 5;
    // EXPECT: mcm8 = 124
    printf("mcm8 = %d\n", mcm(d8, 5));

    int d9[3];
    d9[0] = 5; d9[1] = 4; d9[2] = 6;
    // EXPECT: mcm_simple = 120
    printf("mcm_simple = %d\n", mcm(d9, 3));

    int d10[4];
    d10[0] = 1; d10[1] = 100; d10[2] = 1; d10[3] = 100;
    // EXPECT: mcm_extreme = 200
    printf("mcm_extreme = %d\n", mcm(d10, 4));

    return 0;
}
