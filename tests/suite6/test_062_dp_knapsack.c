int printf(const char *fmt, ...);

int max(int a, int b) {
    if (a > b) return a;
    return b;
}

int knapsack_01(int weights[], int values[], int n, int capacity) {
    int dp[20][110];
    int i = 0;
    while (i <= n) {
        int w = 0;
        while (w <= capacity) {
            dp[i][w] = 0;
            w = w + 1;
        }
        i = i + 1;
    }
    i = 1;
    while (i <= n) {
        int w = 0;
        while (w <= capacity) {
            dp[i][w] = dp[i - 1][w];
            if (weights[i - 1] <= w) {
                int include_val = dp[i - 1][w - weights[i - 1]] + values[i - 1];
                if (include_val > dp[i][w]) {
                    dp[i][w] = include_val;
                }
            }
            w = w + 1;
        }
        i = i + 1;
    }
    return dp[n][capacity];
}

int knapsack_trace(int weights[], int values[], int n, int capacity, int selected[]) {
    int dp[20][110];
    int i = 0;
    while (i <= n) {
        int w = 0;
        while (w <= capacity) {
            dp[i][w] = 0;
            w = w + 1;
        }
        i = i + 1;
    }
    i = 1;
    while (i <= n) {
        int w = 0;
        while (w <= capacity) {
            dp[i][w] = dp[i - 1][w];
            if (weights[i - 1] <= w) {
                int include_val = dp[i - 1][w - weights[i - 1]] + values[i - 1];
                if (include_val > dp[i][w]) {
                    dp[i][w] = include_val;
                }
            }
            w = w + 1;
        }
        i = i + 1;
    }
    int result = dp[n][capacity];

    int k = 0;
    while (k < n) {
        selected[k] = 0;
        k = k + 1;
    }
    int w = capacity;
    i = n;
    while (i > 0 && w > 0) {
        if (dp[i][w] != dp[i - 1][w]) {
            selected[i - 1] = 1;
            w = w - weights[i - 1];
        }
        i = i - 1;
    }
    return result;
}

int knapsack_1d(int weights[], int values[], int n, int capacity) {
    int dp[110];
    int w = 0;
    while (w <= capacity) {
        dp[w] = 0;
        w = w + 1;
    }
    int i = 0;
    while (i < n) {
        w = capacity;
        while (w >= weights[i]) {
            int val = dp[w - weights[i]] + values[i];
            if (val > dp[w]) {
                dp[w] = val;
            }
            w = w - 1;
        }
        i = i + 1;
    }
    return dp[capacity];
}

int total_weight_selected(int weights[], int selected[], int n) {
    int total = 0;
    int i = 0;
    while (i < n) {
        if (selected[i]) total = total + weights[i];
        i = i + 1;
    }
    return total;
}

int count_selected(int selected[], int n) {
    int count = 0;
    int i = 0;
    while (i < n) {
        if (selected[i]) count = count + 1;
        i = i + 1;
    }
    return count;
}

int main(void) {
    int w1[4];
    w1[0] = 2; w1[1] = 3; w1[2] = 4; w1[3] = 5;
    int v1[4];
    v1[0] = 3; v1[1] = 4; v1[2] = 5; v1[3] = 6;

    // EXPECT: knapsack1(cap=5) = 7
    printf("knapsack1(cap=5) = %d\n", knapsack_01(w1, v1, 4, 5));
    // EXPECT: knapsack1(cap=10) = 13
    printf("knapsack1(cap=10) = %d\n", knapsack_01(w1, v1, 4, 10));
    // EXPECT: knapsack1(cap=0) = 0
    printf("knapsack1(cap=0) = %d\n", knapsack_01(w1, v1, 4, 0));
    // EXPECT: knapsack1(cap=2) = 3
    printf("knapsack1(cap=2) = %d\n", knapsack_01(w1, v1, 4, 2));

    int sel[10];
    int val = knapsack_trace(w1, v1, 4, 5, sel);
    // EXPECT: trace(cap=5) val=7
    printf("trace(cap=5) val=%d\n", val);
    // EXPECT: trace items=2
    printf("trace items=%d\n", count_selected(sel, 4));
    // EXPECT: trace weight=5
    printf("trace weight=%d\n", total_weight_selected(w1, sel, 4));

    val = knapsack_trace(w1, v1, 4, 10, sel);
    // EXPECT: trace(cap=10) val=13
    printf("trace(cap=10) val=%d\n", val);

    // EXPECT: 1d(cap=5) = 7
    printf("1d(cap=5) = %d\n", knapsack_1d(w1, v1, 4, 5));
    // EXPECT: 1d(cap=10) = 13
    printf("1d(cap=10) = %d\n", knapsack_1d(w1, v1, 4, 10));

    int w2[5];
    w2[0] = 1; w2[1] = 2; w2[2] = 3; w2[3] = 8; w2[4] = 7;
    int v2[5];
    v2[0] = 1; v2[1] = 4; v2[2] = 5; v2[3] = 8; v2[4] = 7;

    // EXPECT: ks2(cap=10) = 12
    printf("ks2(cap=10) = %d\n", knapsack_01(w2, v2, 5, 10));
    // EXPECT: ks2(cap=6) = 10
    printf("ks2(cap=6) = %d\n", knapsack_01(w2, v2, 5, 6));

    val = knapsack_trace(w2, v2, 5, 10, sel);
    // EXPECT: trace2(cap=10) val=12
    printf("trace2(cap=10) val=%d\n", val);
    // EXPECT: trace2 items=2
    printf("trace2 items=%d\n", count_selected(sel, 5));
    // EXPECT: trace2 weight<=10 = 1
    printf("trace2 weight<=10 = %d\n", total_weight_selected(w2, sel, 5) <= 10);

    int w3[3];
    w3[0] = 10; w3[1] = 20; w3[2] = 30;
    int v3[3];
    v3[0] = 60; v3[1] = 100; v3[2] = 120;
    // EXPECT: ks3(cap=50) = 220
    printf("ks3(cap=50) = %d\n", knapsack_01(w3, v3, 3, 50));

    // EXPECT: 1d_ks3(cap=50) = 220
    printf("1d_ks3(cap=50) = %d\n", knapsack_1d(w3, v3, 3, 50));

    return 0;
}
