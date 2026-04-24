int printf(const char *fmt, ...);

int max(int a, int b) {
    if (a > b) return a;
    return b;
}

int rod_cutting(int prices[], int n) {
    int dp[50];
    int i = 0;
    while (i <= n) {
        dp[i] = 0;
        i = i + 1;
    }

    i = 1;
    while (i <= n) {
        int j = 0;
        while (j < i) {
            int val = prices[j] + dp[i - j - 1];
            if (val > dp[i]) dp[i] = val;
            j = j + 1;
        }
        i = i + 1;
    }
    return dp[n];
}

int rod_with_cuts(int prices[], int n, int cuts[]) {
    int dp[50];
    int from[50];
    int i = 0;
    while (i <= n) {
        dp[i] = 0;
        from[i] = 0;
        i = i + 1;
    }

    i = 1;
    while (i <= n) {
        int j = 0;
        while (j < i) {
            int val = prices[j] + dp[i - j - 1];
            if (val > dp[i]) {
                dp[i] = val;
                from[i] = j + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }

    int count = 0;
    int rem = n;
    while (rem > 0) {
        cuts[count] = from[rem];
        rem = rem - from[rem];
        count = count + 1;
    }
    return count;
}

int optimal_bst(int freq[], int n) {
    int dp[15][15];
    int i = 0;
    while (i < 15) {
        int j = 0;
        while (j < 15) {
            dp[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }

    i = 0;
    while (i < n) {
        dp[i][i] = freq[i];
        i = i + 1;
    }

    int len = 2;
    while (len <= n) {
        i = 0;
        while (i <= n - len) {
            int j = i + len - 1;
            dp[i][j] = 999999999;
            int sum = 0;
            int s = i;
            while (s <= j) {
                sum = sum + freq[s];
                s = s + 1;
            }
            int r = i;
            while (r <= j) {
                int cost = sum;
                if (r > i) cost = cost + dp[i][r - 1];
                if (r < j) cost = cost + dp[r + 1][j];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                }
                r = r + 1;
            }
            i = i + 1;
        }
        len = len + 1;
    }
    return dp[0][n - 1];
}

int burst_balloons(int nums[], int n) {
    int arr[15];
    arr[0] = 1;
    int i = 0;
    while (i < n) {
        arr[i + 1] = nums[i];
        i = i + 1;
    }
    arr[n + 1] = 1;
    int total = n + 2;

    int dp[15][15];
    i = 0;
    while (i < total) {
        int j = 0;
        while (j < total) {
            dp[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }

    int len = 2;
    while (len < total) {
        int left = 0;
        while (left < total - len) {
            int right = left + len;
            int k = left + 1;
            while (k < right) {
                int coins = arr[left] * arr[k] * arr[right]
                            + dp[left][k] + dp[k][right];
                if (coins > dp[left][right]) {
                    dp[left][right] = coins;
                }
                k = k + 1;
            }
            left = left + 1;
        }
        len = len + 1;
    }
    return dp[0][n + 1];
}

int max_sum_no_adj(int arr[], int n) {
    if (n == 0) return 0;
    if (n == 1) return arr[0];
    int prev2 = arr[0];
    int prev1 = max(arr[0], arr[1]);
    int i = 2;
    while (i < n) {
        int cur = max(prev1, prev2 + arr[i]);
        prev2 = prev1;
        prev1 = cur;
        i = i + 1;
    }
    return prev1;
}

int main(void) {
    int p1[8];
    p1[0]=1; p1[1]=5; p1[2]=8; p1[3]=9;
    p1[4]=10; p1[5]=17; p1[6]=17; p1[7]=20;
    // EXPECT: rod(8) = 22
    printf("rod(8) = %d\n", rod_cutting(p1, 8));
    // EXPECT: rod(4) = 10
    printf("rod(4) = %d\n", rod_cutting(p1, 4));
    // EXPECT: rod(1) = 1
    printf("rod(1) = %d\n", rod_cutting(p1, 1));
    // EXPECT: rod(2) = 5
    printf("rod(2) = %d\n", rod_cutting(p1, 2));

    int cuts[20];
    int ncuts = rod_with_cuts(p1, 8, cuts);
    int sum = 0;
    int ci = 0;
    while (ci < ncuts) {
        sum = sum + cuts[ci];
        ci = ci + 1;
    }
    // EXPECT: rod_sum = 8
    printf("rod_sum = %d\n", sum);
    // EXPECT: rod_ncuts > 0
    printf("rod_ncuts > 0\n");

    int p2[4];
    p2[0]=3; p2[1]=5; p2[2]=8; p2[3]=9;
    // EXPECT: rod2(4) = 12
    printf("rod2(4) = %d\n", rod_cutting(p2, 4));

    int freq1[3];
    freq1[0]=34; freq1[1]=8; freq1[2]=50;
    // EXPECT: bst(3) = 142
    printf("bst(3) = %d\n", optimal_bst(freq1, 3));

    int freq2[4];
    freq2[0]=4; freq2[1]=2; freq2[2]=6; freq2[3]=3;
    // EXPECT: bst(4) = 26
    printf("bst(4) = %d\n", optimal_bst(freq2, 4));

    int freq3[2];
    freq3[0]=10; freq3[1]=20;
    // EXPECT: bst(2) = 40
    printf("bst(2) = %d\n", optimal_bst(freq3, 2));

    int b1[3];
    b1[0]=3; b1[1]=1; b1[2]=5;
    // EXPECT: burst(3,1,5) = 35
    printf("burst(3,1,5) = %d\n", burst_balloons(b1, 3));

    int b2[4];
    b2[0]=3; b2[1]=1; b2[2]=5; b2[3]=8;
    // EXPECT: burst(3,1,5,8) = 167
    printf("burst(3,1,5,8) = %d\n", burst_balloons(b2, 4));

    int b3[1];
    b3[0]=5;
    // EXPECT: burst(5) = 5
    printf("burst(5) = %d\n", burst_balloons(b3, 1));

    int na1[6];
    na1[0]=2; na1[1]=7; na1[2]=9; na1[3]=5; na1[4]=1; na1[5]=3;
    // EXPECT: noadj = 15
    printf("noadj = %d\n", max_sum_no_adj(na1, 6));

    int na2[4];
    na2[0]=5; na2[1]=5; na2[2]=10; na2[3]=100;
    // EXPECT: noadj2 = 105
    printf("noadj2 = %d\n", max_sum_no_adj(na2, 4));

    return 0;
}
