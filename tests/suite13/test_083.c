int printf(const char *fmt, ...);

// 0/1 Knapsack Problem

// Standard 0/1 knapsack: maximize value with weight constraint
// weights[], values[], n items, capacity W
int knapsack01(int *weights, int *values, int n, int W) {
    // dp[i][w] = max value using items 0..i-1 with capacity w
    // Use 1D rolling array: dp[w]
    int dp[128];
    int i;
    int w;

    for (w = 0; w <= W; w++) {
        dp[w] = 0;
    }

    for (i = 0; i < n; i++) {
        // Traverse backwards to avoid using item twice
        for (w = W; w >= weights[i]; w--) {
            if (dp[w - weights[i]] + values[i] > dp[w]) {
                dp[w] = dp[w - weights[i]] + values[i];
            }
        }
    }

    return dp[W];
}

// Count the number of ways to fill knapsack exactly to capacity
int knapsack_count(int *weights, int n, int W) {
    int dp[128];
    int i;
    int w;

    for (w = 0; w <= W; w++) {
        dp[w] = 0;
    }
    dp[0] = 1; // one way to have weight 0: take nothing

    for (i = 0; i < n; i++) {
        for (w = W; w >= weights[i]; w--) {
            dp[w] = dp[w] + dp[w - weights[i]];
        }
    }

    return dp[W];
}

// Check which items were selected (store in selected[], return count)
int knapsack_items(int *wt, int *val, int n, int W, int *selected) {
    // Build full 2D table (up to 20 items x 100 capacity)
    int dp[21][101];
    int i;
    int w;
    int count;

    for (i = 0; i <= n; i++) {
        for (w = 0; w <= W; w++) {
            dp[i][w] = 0;
        }
    }

    for (i = 1; i <= n; i++) {
        for (w = 0; w <= W; w++) {
            dp[i][w] = dp[i - 1][w];
            if (wt[i - 1] <= w) {
                if (dp[i - 1][w - wt[i - 1]] + val[i - 1] > dp[i][w]) {
                    dp[i][w] = dp[i - 1][w - wt[i - 1]] + val[i - 1];
                }
            }
        }
    }

    // Traceback
    count = 0;
    w = W;
    for (i = n; i >= 1; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            selected[count] = i - 1; // 0-indexed item
            count = count + 1;
            w = w - wt[i - 1];
        }
    }

    return count;
}

// Subset sum: can we pick items to get exactly target?
int subset_sum(int *arr, int n, int target) {
    int dp[128];
    int i;
    int w;

    for (w = 0; w <= target; w++) {
        dp[w] = 0;
    }
    dp[0] = 1;

    for (i = 0; i < n; i++) {
        for (w = target; w >= arr[i]; w--) {
            if (dp[w - arr[i]]) {
                dp[w] = 1;
            }
        }
    }

    return dp[target];
}

int main() {
    int weights[6];
    int values[6];
    int selected[6];
    int count;
    int i;
    int arr[5];

    // Classic example: 4 items
    // weights: 2, 3, 4, 5
    // values:  3, 4, 5, 6
    // capacity: 5
    weights[0] = 2; weights[1] = 3; weights[2] = 4; weights[3] = 5;
    values[0] = 3; values[1] = 4; values[2] = 5; values[3] = 6;

    printf("%d\n", knapsack01(weights, values, 4, 5));
    // EXPECT: 7

    // capacity 8: take items 0,1,2 (wt=9 no), 0,1 (wt=5,val=7), 0,3 (wt=7,val=9), 1,2 (wt=7,val=9), 1,3 (wt=8,val=10)
    printf("%d\n", knapsack01(weights, values, 4, 8));
    // EXPECT: 10

    // capacity 2: only item 0 fits
    printf("%d\n", knapsack01(weights, values, 4, 2));
    // EXPECT: 3

    // capacity 0: nothing fits
    printf("%d\n", knapsack01(weights, values, 4, 0));
    // EXPECT: 0

    // capacity 14: all items fit (2+3+4+5=14), total value 18
    printf("%d\n", knapsack01(weights, values, 4, 14));
    // EXPECT: 18

    // Item reconstruction for capacity 5
    // Best is items 0,1 (wt 2+3=5, val 3+4=7)
    count = knapsack_items(weights, values, 4, 5, selected);
    printf("%d\n", count);
    // EXPECT: 2

    // Subset sum: {3, 7, 1, 8, 4}, target=11 => 3+8=11 or 7+4=11 or 3+4+4? no.. 3+8=11, 7+4=11
    arr[0] = 3; arr[1] = 7; arr[2] = 1; arr[3] = 8; arr[4] = 4;
    printf("%d\n", subset_sum(arr, 5, 11));
    // EXPECT: 1

    // target=2: no subset sums to 2
    printf("%d\n", subset_sum(arr, 5, 2));
    // EXPECT: 0

    // target=23: 3+7+1+8+4=23, yes
    printf("%d\n", subset_sum(arr, 5, 23));
    // EXPECT: 1

    // target=12: 3+1+8=12 or 8+4=12 or 1+7+4=12
    printf("%d\n", subset_sum(arr, 5, 12));
    // EXPECT: 1

    // Count ways to fill exactly capacity 5 with weights {1,2,3,4,5}
    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    printf("%d\n", knapsack_count(arr, 5, 5));
    // EXPECT: 3

    // Count ways for capacity 3: {3}, {1,2} => 2
    printf("%d\n", knapsack_count(arr, 5, 3));
    // EXPECT: 2

    // Another knapsack: single item
    weights[0] = 10;
    values[0] = 100;
    printf("%d\n", knapsack01(weights, values, 1, 9));
    // EXPECT: 0
    printf("%d\n", knapsack01(weights, values, 1, 10));
    // EXPECT: 100

    printf("done\n");
    // EXPECT: done

    return 0;
}
