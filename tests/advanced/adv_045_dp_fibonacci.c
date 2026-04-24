// EXPECT: === DP: Fibonacci, Stairs, Coins, Kadane ===
// EXPECT: --- Fibonacci (Memoized) ---
// EXPECT: fib(0)=0
// EXPECT: fib(1)=1
// EXPECT: fib(2)=1
// EXPECT: fib(3)=2
// EXPECT: fib(4)=3
// EXPECT: fib(5)=5
// EXPECT: fib(6)=8
// EXPECT: fib(7)=13
// EXPECT: fib(8)=21
// EXPECT: fib(9)=34
// EXPECT: fib(10)=55
// EXPECT: fib(15)=610
// EXPECT: fib(20)=6765
// EXPECT: fib(30)=832040
// EXPECT: --- Fibonacci (Bottom-Up) ---
// EXPECT: fib_bu(10)=55
// EXPECT: fib_bu(20)=6765
// EXPECT: fib_bu(30)=832040
// EXPECT: --- Climbing Stairs ---
// EXPECT: stairs(1)=1
// EXPECT: stairs(2)=2
// EXPECT: stairs(3)=3
// EXPECT: stairs(4)=5
// EXPECT: stairs(5)=8
// EXPECT: stairs(6)=13
// EXPECT: stairs(10)=89
// EXPECT: stairs(15)=987
// EXPECT: --- Climbing Stairs (1,2,3 steps) ---
// EXPECT: stairs3(1)=1
// EXPECT: stairs3(2)=2
// EXPECT: stairs3(3)=4
// EXPECT: stairs3(4)=7
// EXPECT: stairs3(5)=13
// EXPECT: stairs3(10)=274
// EXPECT: --- Coin Change ---
// EXPECT: coins([1,5,10], 0)=0
// EXPECT: coins([1,5,10], 1)=1
// EXPECT: coins([1,5,10], 5)=1
// EXPECT: coins([1,5,10], 6)=2
// EXPECT: coins([1,5,10], 11)=2
// EXPECT: coins([1,5,10], 15)=2
// EXPECT: coins([1,5,10], 23)=5
// EXPECT: coins([1,5,10], 30)=3
// EXPECT: coins([2,5], 3)=-1
// EXPECT: coins([1,3,4], 6)=2
// EXPECT: --- Max Subarray (Kadane) ---
// EXPECT: kadane([-2,1,-3,4,-1,2,1,-5,4])=6
// EXPECT: kadane([1,2,3,4,5])=15
// EXPECT: kadane([-1,-2,-3])=-1
// EXPECT: kadane([5,-3,5])=7
// EXPECT: kadane([-2,3,-1,2])=4
// EXPECT: kadane([1])=1
// EXPECT: kadane([-5])=-5
// EXPECT: === Done ===

int printf(const char *fmt, ...);

int memo[50];
int memo_set[50];

void memo_init(void) {
    int i = 0;
    while (i < 50) {
        memo_set[i] = 0;
        i = i + 1;
    }
}

int fib_memo(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    if (memo_set[n]) return memo[n];
    memo[n] = fib_memo(n - 1) + fib_memo(n - 2);
    memo_set[n] = 1;
    return memo[n];
}

int fib_bottom_up(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    int a = 0;
    int b = 1;
    int i = 2;
    while (i <= n) {
        int c = a + b;
        a = b;
        b = c;
        i = i + 1;
    }
    return b;
}

int climb_stairs(int n) {
    if (n <= 1) return 1;
    int a = 1;
    int b = 1;
    int i = 2;
    while (i <= n) {
        int c = a + b;
        a = b;
        b = c;
        i = i + 1;
    }
    return b;
}

int climb_stairs_3(int n) {
    if (n <= 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;
    int a = 1;
    int b = 1;
    int c = 2;
    int i = 3;
    while (i <= n) {
        int d = a + b + c;
        a = b;
        b = c;
        c = d;
        i = i + 1;
    }
    return c;
}

int coin_change(int *coins, int ncoin, int amount) {
    int dp[200];
    int i = 0;
    while (i <= amount) {
        dp[i] = 999999;
        i = i + 1;
    }
    dp[0] = 0;

    i = 1;
    while (i <= amount) {
        int j = 0;
        while (j < ncoin) {
            if (coins[j] <= i) {
                int prev = dp[i - coins[j]];
                if (prev != 999999 && prev + 1 < dp[i]) {
                    dp[i] = prev + 1;
                }
            }
            j = j + 1;
        }
        i = i + 1;
    }
    if (dp[amount] == 999999) return -1;
    return dp[amount];
}

int kadane(int *arr, int n) {
    int max_ending = arr[0];
    int max_so_far = arr[0];
    int i = 1;
    while (i < n) {
        if (max_ending + arr[i] > arr[i]) {
            max_ending = max_ending + arr[i];
        } else {
            max_ending = arr[i];
        }
        if (max_ending > max_so_far) {
            max_so_far = max_ending;
        }
        i = i + 1;
    }
    return max_so_far;
}

int main(void) {
    printf("=== DP: Fibonacci, Stairs, Coins, Kadane ===\n");

    printf("--- Fibonacci (Memoized) ---\n");
    memo_init();
    printf("fib(0)=%d\n", fib_memo(0));
    printf("fib(1)=%d\n", fib_memo(1));
    printf("fib(2)=%d\n", fib_memo(2));
    printf("fib(3)=%d\n", fib_memo(3));
    printf("fib(4)=%d\n", fib_memo(4));
    printf("fib(5)=%d\n", fib_memo(5));
    printf("fib(6)=%d\n", fib_memo(6));
    printf("fib(7)=%d\n", fib_memo(7));
    printf("fib(8)=%d\n", fib_memo(8));
    printf("fib(9)=%d\n", fib_memo(9));
    printf("fib(10)=%d\n", fib_memo(10));
    printf("fib(15)=%d\n", fib_memo(15));
    printf("fib(20)=%d\n", fib_memo(20));
    printf("fib(30)=%d\n", fib_memo(30));

    printf("--- Fibonacci (Bottom-Up) ---\n");
    printf("fib_bu(10)=%d\n", fib_bottom_up(10));
    printf("fib_bu(20)=%d\n", fib_bottom_up(20));
    printf("fib_bu(30)=%d\n", fib_bottom_up(30));

    printf("--- Climbing Stairs ---\n");
    printf("stairs(1)=%d\n", climb_stairs(1));
    printf("stairs(2)=%d\n", climb_stairs(2));
    printf("stairs(3)=%d\n", climb_stairs(3));
    printf("stairs(4)=%d\n", climb_stairs(4));
    printf("stairs(5)=%d\n", climb_stairs(5));
    printf("stairs(6)=%d\n", climb_stairs(6));
    printf("stairs(10)=%d\n", climb_stairs(10));
    printf("stairs(15)=%d\n", climb_stairs(15));

    printf("--- Climbing Stairs (1,2,3 steps) ---\n");
    printf("stairs3(1)=%d\n", climb_stairs_3(1));
    printf("stairs3(2)=%d\n", climb_stairs_3(2));
    printf("stairs3(3)=%d\n", climb_stairs_3(3));
    printf("stairs3(4)=%d\n", climb_stairs_3(4));
    printf("stairs3(5)=%d\n", climb_stairs_3(5));
    printf("stairs3(10)=%d\n", climb_stairs_3(10));

    printf("--- Coin Change ---\n");
    int coins1[3];
    coins1[0] = 1; coins1[1] = 5; coins1[2] = 10;
    printf("coins([1,5,10], 0)=%d\n", coin_change(coins1, 3, 0));
    printf("coins([1,5,10], 1)=%d\n", coin_change(coins1, 3, 1));
    printf("coins([1,5,10], 5)=%d\n", coin_change(coins1, 3, 5));
    printf("coins([1,5,10], 6)=%d\n", coin_change(coins1, 3, 6));
    printf("coins([1,5,10], 11)=%d\n", coin_change(coins1, 3, 11));
    printf("coins([1,5,10], 15)=%d\n", coin_change(coins1, 3, 15));
    printf("coins([1,5,10], 23)=%d\n", coin_change(coins1, 3, 23));
    printf("coins([1,5,10], 30)=%d\n", coin_change(coins1, 3, 30));

    int coins2[2];
    coins2[0] = 2; coins2[1] = 5;
    printf("coins([2,5], 3)=%d\n", coin_change(coins2, 2, 3));

    int coins3[3];
    coins3[0] = 1; coins3[1] = 3; coins3[2] = 4;
    printf("coins([1,3,4], 6)=%d\n", coin_change(coins3, 3, 6));

    printf("--- Max Subarray (Kadane) ---\n");
    int a1[9];
    a1[0]=-2; a1[1]=1; a1[2]=-3; a1[3]=4; a1[4]=-1; a1[5]=2; a1[6]=1; a1[7]=-5; a1[8]=4;
    printf("kadane([-2,1,-3,4,-1,2,1,-5,4])=%d\n", kadane(a1, 9));

    int a2[5];
    a2[0]=1; a2[1]=2; a2[2]=3; a2[3]=4; a2[4]=5;
    printf("kadane([1,2,3,4,5])=%d\n", kadane(a2, 5));

    int a3[3];
    a3[0]=-1; a3[1]=-2; a3[2]=-3;
    printf("kadane([-1,-2,-3])=%d\n", kadane(a3, 3));

    int a4[3];
    a4[0]=5; a4[1]=-3; a4[2]=5;
    printf("kadane([5,-3,5])=%d\n", kadane(a4, 3));

    int a5[4];
    a5[0]=-2; a5[1]=3; a5[2]=-1; a5[3]=2;
    printf("kadane([-2,3,-1,2])=%d\n", kadane(a5, 4));

    int a6[1];
    a6[0] = 1;
    printf("kadane([1])=%d\n", kadane(a6, 1));

    int a7[1];
    a7[0] = -5;
    printf("kadane([-5])=%d\n", kadane(a7, 1));

    printf("=== Done ===\n");
    return 0;
}
