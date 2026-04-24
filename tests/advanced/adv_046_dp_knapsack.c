// EXPECT: === 0/1 Knapsack ===
// EXPECT: --- Test 1: Basic ---
// EXPECT: Items: wt=[2,3,4,5] val=[3,4,5,6] cap=8
// EXPECT: Recursive: max_val=10
// EXPECT: DP table: max_val=10
// EXPECT: Selected items: 1 3
// EXPECT: Total weight=8, total value=10
// EXPECT: --- Test 2: Exact fit ---
// EXPECT: Items: wt=[1,2,3] val=[6,10,12] cap=5
// EXPECT: Recursive: max_val=22
// EXPECT: DP table: max_val=22
// EXPECT: Selected items: 1 2
// EXPECT: Total weight=5, total value=22
// EXPECT: --- Test 3: All fit ---
// EXPECT: Items: wt=[1,1,1,1] val=[1,2,3,4] cap=10
// EXPECT: Recursive: max_val=10
// EXPECT: DP table: max_val=10
// EXPECT: Selected items: 0 1 2 3
// EXPECT: Total weight=4, total value=10
// EXPECT: --- Test 4: None fit ---
// EXPECT: Items: wt=[5,6,7] val=[10,20,30] cap=4
// EXPECT: Recursive: max_val=0
// EXPECT: DP table: max_val=0
// EXPECT: No items selected
// EXPECT: --- Test 5: Single item ---
// EXPECT: Items: wt=[3] val=[5] cap=3
// EXPECT: Recursive: max_val=5
// EXPECT: DP table: max_val=5
// EXPECT: Selected items: 0
// EXPECT: Total weight=3, total value=5
// EXPECT: --- Test 6: Larger ---
// EXPECT: Items: wt=[2,3,4,5,9] val=[3,4,8,8,10] cap=20
// EXPECT: Recursive: max_val=29
// EXPECT: DP table: max_val=29
// EXPECT: Selected items: 0 2 3 4
// EXPECT: Total weight=20, total value=29
// EXPECT: Correction - recheck cap=20
// EXPECT: Recheck: max_val=29
// EXPECT: === Done ===

int printf(const char *fmt, ...);

int max(int a, int b) {
    if (a > b) return a;
    return b;
}

// Recursive knapsack with memoization
int memo_table[20][200];
int memo_done[20][200];

void memo_init(int n, int cap) {
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j <= cap) {
            memo_done[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
}

int knapsack_rec(int *wt, int *val, int n, int cap, int idx) {
    if (idx >= n || cap <= 0) return 0;
    if (memo_done[idx][cap]) return memo_table[idx][cap];

    int without = knapsack_rec(wt, val, n, cap, idx + 1);
    int with_item = 0;
    if (wt[idx] <= cap) {
        with_item = val[idx] + knapsack_rec(wt, val, n, cap - wt[idx], idx + 1);
    }
    memo_table[idx][cap] = max(without, with_item);
    memo_done[idx][cap] = 1;
    return memo_table[idx][cap];
}

// DP table knapsack
int dp[20][200];

int knapsack_dp(int *wt, int *val, int n, int cap) {
    int i = 0;
    while (i <= n) {
        int w = 0;
        while (w <= cap) {
            dp[i][w] = 0;
            w = w + 1;
        }
        i = i + 1;
    }

    i = 1;
    while (i <= n) {
        int w = 0;
        while (w <= cap) {
            dp[i][w] = dp[i - 1][w];
            if (wt[i - 1] <= w) {
                int take = val[i - 1] + dp[i - 1][w - wt[i - 1]];
                if (take > dp[i][w]) dp[i][w] = take;
            }
            w = w + 1;
        }
        i = i + 1;
    }
    return dp[n][cap];
}

void backtrack(int *wt, int *val, int n, int cap, int *selected, int *sel_count) {
    *sel_count = 0;
    int w = cap;
    int i = n;
    while (i > 0) {
        if (dp[i][w] != dp[i - 1][w]) {
            selected[*sel_count] = i - 1;
            *sel_count = *sel_count + 1;
            w = w - wt[i - 1];
        }
        i = i - 1;
    }
    // Reverse the selected array
    int lo = 0;
    int hi = *sel_count - 1;
    while (lo < hi) {
        int tmp = selected[lo];
        selected[lo] = selected[hi];
        selected[hi] = tmp;
        lo = lo + 1;
        hi = hi - 1;
    }
}

void test_knapsack(int *wt, int *val, int n, int cap, int test_num) {
    printf("Items: wt=[");
    int i = 0;
    while (i < n) {
        if (i > 0) printf(",");
        printf("%d", wt[i]);
        i = i + 1;
    }
    printf("] val=[");
    i = 0;
    while (i < n) {
        if (i > 0) printf(",");
        printf("%d", val[i]);
        i = i + 1;
    }
    printf("] cap=%d\n", cap);

    if (n <= 10 && cap <= 50) {
        memo_init(n, cap);
        int rec_val = knapsack_rec(wt, val, n, cap, 0);
        printf("Recursive: max_val=%d\n", rec_val);
    }

    int dp_val = knapsack_dp(wt, val, n, cap);
    printf("DP table: max_val=%d\n", dp_val);

    int selected[20];
    int sel_count;
    backtrack(wt, val, n, cap, selected, &sel_count);

    if (sel_count == 0) {
        printf("No items selected\n");
    } else {
        printf("Selected items:");
        i = 0;
        while (i < sel_count) {
            printf(" %d", selected[i]);
            i = i + 1;
        }
        printf("\n");

        int tw = 0;
        int tv = 0;
        i = 0;
        while (i < sel_count) {
            tw = tw + wt[selected[i]];
            tv = tv + val[selected[i]];
            i = i + 1;
        }
        printf("Total weight=%d, total value=%d\n", tw, tv);
    }
}

int main(void) {
    printf("=== 0/1 Knapsack ===\n");

    // Test 1: Basic
    printf("--- Test 1: Basic ---\n");
    int wt1[4]; wt1[0]=2; wt1[1]=3; wt1[2]=4; wt1[3]=5;
    int val1[4]; val1[0]=3; val1[1]=4; val1[2]=5; val1[3]=6;
    test_knapsack(wt1, val1, 4, 8, 1);

    // Test 2: Classic example
    printf("--- Test 2: Exact fit ---\n");
    int wt2[3]; wt2[0]=1; wt2[1]=2; wt2[2]=3;
    int val2[3]; val2[0]=6; val2[1]=10; val2[2]=12;
    test_knapsack(wt2, val2, 3, 5, 2);

    // Test 3: All items fit
    printf("--- Test 3: All fit ---\n");
    int wt3[4]; wt3[0]=1; wt3[1]=1; wt3[2]=1; wt3[3]=1;
    int val3[4]; val3[0]=1; val3[1]=2; val3[2]=3; val3[3]=4;
    test_knapsack(wt3, val3, 4, 10, 3);

    // Test 4: No items fit
    printf("--- Test 4: None fit ---\n");
    int wt4[3]; wt4[0]=5; wt4[1]=6; wt4[2]=7;
    int val4[3]; val4[0]=10; val4[1]=20; val4[2]=30;
    test_knapsack(wt4, val4, 3, 4, 4);

    // Test 5: Single item
    printf("--- Test 5: Single item ---\n");
    int wt5[1]; wt5[0]=3;
    int val5[1]; val5[0]=5;
    test_knapsack(wt5, val5, 1, 3, 5);

    // Test 6: Larger test
    printf("--- Test 6: Larger ---\n");
    int wt6[5]; wt6[0]=2; wt6[1]=3; wt6[2]=4; wt6[3]=5; wt6[4]=9;
    int val6[5]; val6[0]=3; val6[1]=4; val6[2]=8; val6[3]=8; val6[4]=10;
    // With cap=20, all items fit (2+3+4+5+9=23 > 20)
    // But let's see what DP picks
    test_knapsack(wt6, val6, 5, 20, 6);

    // total wt of all = 23 > 20, so not all fit
    // dp should find best subset that fits in 20
    // need to recheck
    printf("Correction - recheck cap=20\n");
    // Let's verify: best without item 4 (wt=9,val=10): wt=2+3+4+5=14, val=3+4+8+8=23
    // best without item 0 (wt=2,val=3): wt=3+4+5+9=21>20, skip...
    // Actually items 0,1,2,3: wt=14, val=23. items 0,1,2,4: wt=18, val=25. items 0,2,3,4: wt=20, val=29? No: 3+8+8+10=29, wt=2+4+5+9=20
    // Wait: items {0,2,3,4}: wt=2+4+5+9=20, val=3+8+8+10=29
    // items {1,2,3,4}: wt=3+4+5+9=21>20
    // items {0,1,2,3}: wt=14, val=23
    // items {0,1,3,4}: wt=2+3+5+9=19, val=3+4+8+10=25
    // items {0,1,2,4}: wt=2+3+4+9=18, val=3+4+8+10=25
    // So best is {0,2,3,4} with val=29? Let me recompute...
    // Actually I think my DP output might differ. Let me just call it directly.
    int recheck = knapsack_dp(wt6, val6, 5, 20);
    printf("Recheck: max_val=%d\n", recheck);

    printf("=== Done ===\n");
    return 0;
}
