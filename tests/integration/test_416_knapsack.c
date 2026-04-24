int printf(const char *fmt, ...);
// EXPECT: Knapsack 1: max value = 10\nItems: 3 1\nKnapsack 2: max value = 22\nItems: 3 1\nKnapsack 3: max value = 220\nItems: 2 1\nKnapsack 4: max value = 120\nItems: 4 3 2
// Test: 0/1 Knapsack dynamic programming

int dp[32][128];

int max(int a, int b) {
    return a > b ? a : b;
}

int knapsack(int weights[], int values[], int n, int capacity) {
    int i, w;
    for (i = 0; i <= n; i++) {
        for (w = 0; w <= capacity; w++) {
            if (i == 0 || w == 0) {
                dp[i][w] = 0;
            } else if (weights[i - 1] <= w) {
                dp[i][w] = max(values[i - 1] + dp[i - 1][w - weights[i - 1]],
                               dp[i - 1][w]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }
    return dp[n][capacity];
}

void find_items(int weights[], int n, int capacity) {
    int i = n;
    int w = capacity;
    printf("Items:");
    while (i > 0 && w > 0) {
        if (dp[i][w] != dp[i - 1][w]) {
            printf(" %d", i - 1);
            w = w - weights[i - 1];
        }
        i--;
    }
    printf("\n");
}

int main(void) {
    int w1[] = {2, 3, 4, 5};
    int v1[] = {3, 4, 5, 6};
    int n1 = 4;
    int cap1 = 8;
    int result = knapsack(w1, v1, n1, cap1);
    printf("Knapsack 1: max value = %d\n", result);
    find_items(w1, n1, cap1);

    int w2[] = {1, 2, 3, 5};
    int v2[] = {1, 6, 10, 16};
    int n2 = 4;
    int cap2 = 7;
    result = knapsack(w2, v2, n2, cap2);
    printf("Knapsack 2: max value = %d\n", result);
    find_items(w2, n2, cap2);

    int w3[] = {10, 20, 30};
    int v3[] = {60, 100, 120};
    int n3 = 3;
    int cap3 = 50;
    result = knapsack(w3, v3, n3, cap3);
    printf("Knapsack 3: max value = %d\n", result);
    find_items(w3, n3, cap3);

    int w4[] = {1, 1, 1, 1, 1};
    int v4[] = {10, 20, 30, 40, 50};
    int n4 = 5;
    int cap4 = 3;
    result = knapsack(w4, v4, n4, cap4);
    printf("Knapsack 4: max value = %d\n", result);
    find_items(w4, n4, cap4);

    return 0;
}
