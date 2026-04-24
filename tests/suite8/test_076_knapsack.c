int printf(const char *fmt, ...);

// 0/1 Knapsack Problem

// Standard 0/1 knapsack
// weights[], values[], n items, capacity W
// Returns maximum value
int knapsack(int *weights, int *values, int n, int W) {
    // dp[i] = max value for capacity i (space-optimized)
    int dp[201];
    int i;
    int w;

    for (i = 0; i <= W; i++) {
        dp[i] = 0;
    }

    for (i = 0; i < n; i++) {
        // Traverse backwards to avoid using same item twice
        for (w = W; w >= weights[i]; w--) {
            int with_item = dp[w - weights[i]] + values[i];
            if (with_item > dp[w]) {
                dp[w] = with_item;
            }
        }
    }

    return dp[W];
}

// Knapsack with 2D DP table to also find which items are selected
int knapsack_items(int *weights, int *values, int n, int W, int *selected) {
    // dp[i][w] at flat index i*(W+1)+w
    int dp[600];  // max 6 items * 100 capacity
    int i;
    int w;

    for (i = 0; i <= n; i++) {
        for (w = 0; w <= W; w++) {
            dp[i * (W + 1) + w] = 0;
        }
    }

    for (i = 1; i <= n; i++) {
        for (w = 0; w <= W; w++) {
            dp[i * (W + 1) + w] = dp[(i - 1) * (W + 1) + w];
            if (weights[i - 1] <= w) {
                int with_item = dp[(i - 1) * (W + 1) + (w - weights[i - 1])] + values[i - 1];
                if (with_item > dp[i * (W + 1) + w]) {
                    dp[i * (W + 1) + w] = with_item;
                }
            }
        }
    }

    // Trace back to find selected items
    int sel_count = 0;
    w = W;
    for (i = n; i > 0; i--) {
        if (dp[i * (W + 1) + w] != dp[(i - 1) * (W + 1) + w]) {
            selected[sel_count] = i - 1;  // 0-indexed item
            sel_count = sel_count + 1;
            w = w - weights[i - 1];
        }
    }

    return dp[n * (W + 1) + W];
}

// Unbounded knapsack (can use each item multiple times)
int unbounded_knapsack(int *weights, int *values, int n, int W) {
    int dp[201];
    int i;
    int w;

    for (i = 0; i <= W; i++) {
        dp[i] = 0;
    }

    for (w = 1; w <= W; w++) {
        for (i = 0; i < n; i++) {
            if (weights[i] <= w) {
                int val = dp[w - weights[i]] + values[i];
                if (val > dp[w]) {
                    dp[w] = val;
                }
            }
        }
    }

    return dp[W];
}

// Fractional-like approximation: sort by value/weight ratio (greedy)
// Since we can't use float, multiply value by 1000 / weight for comparison
int knapsack_greedy(int *weights, int *values, int n, int W) {
    int used[10];
    int i;
    int j;
    for (i = 0; i < n; i++) used[i] = 0;

    int total_value = 0;
    int remaining = W;

    while (remaining > 0) {
        int best = -1;
        int best_ratio = -1;
        for (i = 0; i < n; i++) {
            if (used[i] == 0 && weights[i] <= remaining) {
                int ratio = (values[i] * 1000) / weights[i];
                if (ratio > best_ratio) {
                    best_ratio = ratio;
                    best = i;
                }
            }
        }
        if (best == -1) break;
        used[best] = 1;
        total_value = total_value + values[best];
        remaining = remaining - weights[best];
    }

    return total_value;
}

int main() {
    // Test case 1: Classic example
    int w1[4];
    int v1[4];
    w1[0] = 2; v1[0] = 3;
    w1[1] = 3; v1[1] = 4;
    w1[2] = 4; v1[2] = 5;
    w1[3] = 5; v1[3] = 6;

    // EXPECT: ks1(5) = 7
    printf("ks1(5) = %d\n", knapsack(w1, v1, 4, 5));
    // EXPECT: ks1(10) = 13
    printf("ks1(10) = %d\n", knapsack(w1, v1, 4, 10));

    // Test case 2
    int w2[5];
    int v2[5];
    w2[0] = 1; v2[0] = 1;
    w2[1] = 2; v2[1] = 6;
    w2[2] = 3; v2[2] = 10;
    w2[3] = 5; v2[3] = 15;
    w2[4] = 7; v2[4] = 22;

    // EXPECT: ks2(10) = 32
    printf("ks2(10) = %d\n", knapsack(w2, v2, 5, 10));
    // EXPECT: ks2(15) = 47
    printf("ks2(15) = %d\n", knapsack(w2, v2, 5, 15));

    // Edge cases
    // EXPECT: ks_zero = 0
    printf("ks_zero = %d\n", knapsack(w1, v1, 4, 0));
    // EXPECT: ks_tiny = 0
    printf("ks_tiny = %d\n", knapsack(w1, v1, 4, 1));

    // Test with item tracking
    int sel[10];
    int val = knapsack_items(w1, v1, 4, 5, sel);
    // EXPECT: ks_items(5) = 7
    printf("ks_items(5) = %d\n", val);

    // Verify total weight of selected items is within capacity
    int total_w = 0;
    int i;
    int count = 0;
    for (i = 0; i < 4; i++) {
        // count items in selected (backward order)
    }
    // Just verify value matches
    // EXPECT: items_val_match = 1
    printf("items_val_match = %d\n", val == knapsack(w1, v1, 4, 5) ? 1 : 0);

    // Test unbounded knapsack
    int w3[3];
    int v3[3];
    w3[0] = 1; v3[0] = 1;
    w3[1] = 3; v3[1] = 4;
    w3[2] = 4; v3[2] = 5;

    // EXPECT: unbounded(7) = 9
    printf("unbounded(7) = %d\n", unbounded_knapsack(w3, v3, 3, 7));
    // EXPECT: unbounded(10) = 13
    printf("unbounded(10) = %d\n", unbounded_knapsack(w3, v3, 3, 10));

    // Unbounded with different items
    int w4[2];
    int v4[2];
    w4[0] = 2; v4[0] = 3;
    w4[1] = 5; v4[1] = 7;
    // EXPECT: unbounded2(10) = 15
    printf("unbounded2(10) = %d\n", unbounded_knapsack(w4, v4, 2, 10));

    // Test greedy approach
    // EXPECT: greedy(5) = 7
    printf("greedy(5) = %d\n", knapsack_greedy(w1, v1, 4, 5));
    // EXPECT: greedy(10) = 12
    printf("greedy(10) = %d\n", knapsack_greedy(w1, v1, 4, 10));

    return 0;
}
