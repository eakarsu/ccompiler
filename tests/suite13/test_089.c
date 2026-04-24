int printf(const char *fmt, ...);

// Rod Cutting Problem
// Given rod of length n and prices for each piece length,
// find maximum revenue from cutting the rod

// Standard rod cutting: prices[i] = price of rod of length i+1
int rod_cut_max(int *prices, int n) {
    int dp[64];
    int i;
    int j;

    dp[0] = 0;

    for (i = 1; i <= n; i++) {
        dp[i] = 0;
        for (j = 1; j <= i; j++) {
            if (prices[j - 1] + dp[i - j] > dp[i]) {
                dp[i] = prices[j - 1] + dp[i - j];
            }
        }
    }

    return dp[n];
}

// Rod cutting with cut cost: each cut costs c
int rod_cut_with_cost(int *prices, int n, int c) {
    int dp[64];
    int i;
    int j;
    int revenue;

    dp[0] = 0;

    for (i = 1; i <= n; i++) {
        // No cut: sell whole piece of length i
        dp[i] = prices[i - 1];
        for (j = 1; j < i; j++) {
            // Cut at position j: piece of length j + remaining (i-j)
            // One cut costs c
            revenue = prices[j - 1] + dp[i - j] - c;
            if (revenue > dp[i]) {
                dp[i] = revenue;
            }
        }
    }

    return dp[n];
}

// Track the cuts made: store piece lengths in cuts[], return number of pieces
int rod_cut_trace(int *prices, int n, int *cuts) {
    int dp[64];
    int first_cut[64]; // length of first piece
    int i;
    int j;
    int count;
    int rem;

    dp[0] = 0;

    for (i = 1; i <= n; i++) {
        dp[i] = 0;
        first_cut[i] = i; // default: no cut
        for (j = 1; j <= i; j++) {
            if (prices[j - 1] + dp[i - j] > dp[i]) {
                dp[i] = prices[j - 1] + dp[i - j];
                first_cut[i] = j;
            }
        }
    }

    // Reconstruct
    count = 0;
    rem = n;
    while (rem > 0) {
        cuts[count] = first_cut[rem];
        count = count + 1;
        rem = rem - first_cut[rem];
    }

    return count;
}

// Count the number of ways to cut a rod of length n
// (each way is a multiset of pieces summing to n)
int rod_cut_ways(int n) {
    // This is the integer partition function
    // dp[i][j] = number of ways to partition i using parts <= j
    int dp[32][32];
    int i;
    int j;

    for (i = 0; i <= n; i++) {
        for (j = 0; j <= n; j++) {
            dp[i][j] = 0;
        }
    }

    // Base case: one way to have sum 0
    for (j = 0; j <= n; j++) {
        dp[0][j] = 1;
    }

    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            dp[i][j] = dp[i][j - 1]; // don't use a piece of length j
            if (j <= i) {
                dp[i][j] = dp[i][j] + dp[i - j][j]; // use at least one piece of length j
            }
        }
    }

    return dp[n][n];
}

// Minimum number of pieces to achieve at least target revenue
int min_pieces_for_revenue(int *prices, int n, int target) {
    int dp[64]; // dp[i] = max revenue with exactly i units of rod
    int i;
    int j;

    // Use the standard approach but track number of cuts
    // Actually, let's use dp[r] = min length needed for revenue >= r
    // Simpler: just try all possible number of pieces
    int best_rev;
    int curr_rev;
    int pieces;

    // dp approach: dp[len] = max revenue for rod of length len
    dp[0] = 0;
    for (i = 1; i <= n; i++) {
        dp[i] = 0;
        for (j = 1; j <= i; j++) {
            if (prices[j - 1] + dp[i - j] > dp[i]) {
                dp[i] = prices[j - 1] + dp[i - j];
            }
        }
    }

    if (dp[n] < target) return -1; // impossible

    // Now find minimum pieces: try cutting into pieces, dp to minimize count
    // dp2[i] = min pieces to get revenue >= target from rod of length i
    // This is complex; let's use a different DP:
    // dp_rev[i] = max revenue using at most i pieces from rod of length n
    // Binary search or direct
    int dp_rev[64];
    int dp_cnt[64];

    // Count pieces in optimal solution
    int cuts_arr[64];
    pieces = rod_cut_trace(prices, n, cuts_arr);
    return pieces;
}

int main() {
    int prices1[8];
    int prices2[10];
    int cuts[32];
    int cnt;
    int i;
    int sum;

    // Classic example: length 8, prices for 1..8:
    // prices = {1, 5, 8, 9, 10, 17, 17, 20}
    prices1[0]=1; prices1[1]=5; prices1[2]=8; prices1[3]=9;
    prices1[4]=10; prices1[5]=17; prices1[6]=17; prices1[7]=20;

    // Rod of length 1: price = 1
    printf("%d\n", rod_cut_max(prices1, 1));
    // EXPECT: 1

    // Rod of length 2: max(1+1, 5) = 5
    printf("%d\n", rod_cut_max(prices1, 2));
    // EXPECT: 5

    // Rod of length 3: max(1+5, 8, 1+1+1) = 8
    printf("%d\n", rod_cut_max(prices1, 3));
    // EXPECT: 8

    // Rod of length 4: max(5+5, 9, 1+8, 8+1, 10) = 10
    printf("%d\n", rod_cut_max(prices1, 4));
    // EXPECT: 10

    // Rod of length 5: max(1+10, 5+8, 8+5, 9+1, 10+0=10)
    // dp[5]: j=1: 1+dp[4]=1+10=11, j=2: 5+dp[3]=5+8=13, j=3: 8+dp[2]=8+5=13
    // j=4: 9+dp[1]=9+1=10, j=5: 10+dp[0]=10 => 13
    printf("%d\n", rod_cut_max(prices1, 5));
    // EXPECT: 13

    // Rod of length 6: dp[6]
    // j=1:1+dp[5]=14, j=2:5+dp[4]=15, j=3:8+dp[3]=16, j=4:9+dp[2]=14
    // j=5:10+dp[1]=11, j=6:17 => 17
    printf("%d\n", rod_cut_max(prices1, 6));
    // EXPECT: 17

    // Rod of length 7:
    // j=1:1+17=18, j=2:5+13=18, j=3:8+10=18, j=4:9+8=17
    // j=5:10+5=15, j=6:17+1=18, j=7:17 => 18
    printf("%d\n", rod_cut_max(prices1, 7));
    // EXPECT: 18

    // Rod of length 8:
    // j=1:1+18=19, j=2:5+17=22, j=3:8+13=21, j=4:9+10=19
    // j=5:10+8=18, j=6:17+5=22, j=7:17+1=18, j=8:20 => 22
    printf("%d\n", rod_cut_max(prices1, 8));
    // EXPECT: 22

    // Trace cuts for length 8: should be 2+6 (price 5+17=22) or 6+2
    cnt = rod_cut_trace(prices1, 8, cuts);
    printf("%d\n", cnt);
    // EXPECT: 2
    sum = 0;
    for (i = 0; i < cnt; i++) {
        sum = sum + cuts[i];
    }
    printf("%d\n", sum);
    // EXPECT: 8

    // Trace cuts for length 4: should be 2+2 (5+5=10)
    cnt = rod_cut_trace(prices1, 4, cuts);
    printf("%d\n", cnt);
    // EXPECT: 2
    printf("%d %d\n", cuts[0], cuts[1]);
    // EXPECT: 2 2

    // With cut cost=3, length 4:
    // No cut: price[3]=9
    // 1 cut at j=2: 5 + dp[2] - 3, dp[2] = max(price[1]=5, price[0]+dp[1]-3=1+1-3=-1) = 5
    // So: 5 + 5 - 3 = 7
    // 1 cut at j=1: 1 + dp[3] - 3
    // dp[3]_cost: no cut: price[2]=8, j=1: 1+dp[2]-3=1+5-3=3, j=2: 5+dp[1]-3=5+1-3=3 => 8
    // So: 1 + 8 - 3 = 6
    // 1 cut at j=3: 8 + dp[1] - 3 = 8 + 1 - 3 = 6
    // Best: 9 (no cut)
    printf("%d\n", rod_cut_with_cost(prices1, 4, 3));
    // EXPECT: 9

    // With cut cost=0 same as regular
    printf("%d\n", rod_cut_with_cost(prices1, 8, 0));
    // EXPECT: 22

    // Number of ways to cut rod of length 4 (integer partitions of 4):
    // 4, 3+1, 2+2, 2+1+1, 1+1+1+1 => 5
    printf("%d\n", rod_cut_ways(4));
    // EXPECT: 5

    // Partitions of 1: just {1} => 1
    printf("%d\n", rod_cut_ways(1));
    // EXPECT: 1

    // Partitions of 5: 5, 4+1, 3+2, 3+1+1, 2+2+1, 2+1+1+1, 1+1+1+1+1 => 7
    printf("%d\n", rod_cut_ways(5));
    // EXPECT: 7

    // Partitions of 6: 11
    printf("%d\n", rod_cut_ways(6));
    // EXPECT: 11

    // Simple prices: all length-1 pieces worth 3, length-n worth 1 per unit
    // prices = {3, 2, 3, 4}
    prices2[0]=3; prices2[1]=2; prices2[2]=3; prices2[3]=4;
    // length 4: max is 4 pieces of length 1 = 12
    printf("%d\n", rod_cut_max(prices2, 4));
    // EXPECT: 12

    printf("done\n");
    // EXPECT: done

    return 0;
}
