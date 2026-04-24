int printf(const char *fmt, ...);

// Staircase Climbing - ways to reach the top

// Basic: 1 or 2 steps at a time
int climb_stairs_2(int n) {
    int dp[64];
    int i;

    if (n <= 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;

    dp[1] = 1;
    dp[2] = 2;
    for (i = 3; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}

// 1, 2, or 3 steps at a time
int climb_stairs_3(int n) {
    int dp[64];
    int i;

    if (n <= 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 4;

    dp[1] = 1;
    dp[2] = 2;
    dp[3] = 4;
    for (i = 4; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2] + dp[i - 3];
    }
    return dp[n];
}

// Arbitrary step sizes from a set
int climb_stairs_set(int n, int *steps, int nsteps) {
    int dp[64];
    int i;
    int j;

    for (i = 0; i <= n; i++) {
        dp[i] = 0;
    }
    dp[0] = 1;

    for (i = 1; i <= n; i++) {
        for (j = 0; j < nsteps; j++) {
            if (steps[j] <= i) {
                dp[i] = dp[i] + dp[i - steps[j]];
            }
        }
    }
    return dp[n];
}

// Minimum cost climbing: cost[i] = cost of stepping on stair i
// Can start from stair 0 or stair 1, take 1 or 2 steps
int min_cost_climb(int *cost, int n) {
    int dp[64];
    int i;

    if (n == 0) return 0;
    if (n == 1) return 0; // can start at stair 0 or 1, reach top (past n-1)

    dp[0] = cost[0];
    dp[1] = cost[1];
    for (i = 2; i < n; i++) {
        if (dp[i - 1] < dp[i - 2]) {
            dp[i] = dp[i - 1] + cost[i];
        } else {
            dp[i] = dp[i - 2] + cost[i];
        }
    }

    // Can reach top from n-1 or n-2
    if (dp[n - 1] < dp[n - 2]) {
        return dp[n - 1];
    }
    return dp[n - 2];
}

// Ways to climb with exactly k steps
int climb_exact_steps(int n, int k) {
    // dp[i][j] = ways to reach stair i using exactly j steps (1 or 2)
    int dp[32][32];
    int i;
    int j;

    for (i = 0; i <= n; i++) {
        for (j = 0; j <= k; j++) {
            dp[i][j] = 0;
        }
    }
    dp[0][0] = 1;

    for (j = 1; j <= k; j++) {
        for (i = 1; i <= n; i++) {
            dp[i][j] = dp[i - 1][j - 1];
            if (i >= 2) {
                dp[i][j] = dp[i][j] + dp[i - 2][j - 1];
            }
        }
    }

    return dp[n][k];
}

// Ways avoiding specific stairs (forbidden[i] = 1 means can't step on stair i)
int climb_avoid(int n, int *forbidden) {
    int dp[64];
    int i;

    for (i = 0; i <= n; i++) {
        dp[i] = 0;
    }
    dp[0] = 1;

    for (i = 1; i <= n; i++) {
        if (forbidden[i]) {
            dp[i] = 0;
        } else {
            dp[i] = dp[i - 1];
            if (i >= 2) {
                dp[i] = dp[i] + dp[i - 2];
            }
        }
    }

    return dp[n];
}

// Max points collecting: each stair has points, take 1 or 2 steps
int max_points_climb(int *points, int n) {
    int dp[64];
    int i;

    if (n == 0) return 0;
    if (n == 1) return points[1];

    dp[0] = 0;
    dp[1] = points[1];
    for (i = 2; i <= n; i++) {
        if (dp[i - 1] + points[i] > dp[i - 2] + points[i]) {
            dp[i] = dp[i - 1] + points[i];
        } else {
            dp[i] = dp[i - 2] + points[i];
        }
    }

    return dp[n];
}

int main() {
    int steps[3];
    int cost[10];
    int forbidden[32];
    int points[32];
    int i;

    // Basic 2-step: n=1 => 1 way (1)
    printf("%d\n", climb_stairs_2(1));
    // EXPECT: 1

    // n=2 => 2 ways (1+1, 2)
    printf("%d\n", climb_stairs_2(2));
    // EXPECT: 2

    // n=3 => 3 ways (1+1+1, 1+2, 2+1)
    printf("%d\n", climb_stairs_2(3));
    // EXPECT: 3

    // n=4 => 5 ways
    printf("%d\n", climb_stairs_2(4));
    // EXPECT: 5

    // n=5 => 8 ways
    printf("%d\n", climb_stairs_2(5));
    // EXPECT: 8

    // n=10 => 89
    printf("%d\n", climb_stairs_2(10));
    // EXPECT: 89

    // 3-step: n=3 => 4 (1+1+1, 1+2, 2+1, 3)
    printf("%d\n", climb_stairs_3(3));
    // EXPECT: 4

    // n=4 => 7 (1111, 112, 121, 211, 13, 31, 22)
    printf("%d\n", climb_stairs_3(4));
    // EXPECT: 7

    // n=5 => 13 (dp[5]=dp[4]+dp[3]+dp[2]=7+4+2)
    printf("%d\n", climb_stairs_3(5));
    // EXPECT: 13

    // Custom steps: {1, 3, 5}, n=6
    // dp[0]=1
    // dp[1]=dp[0]=1
    // dp[2]=dp[1]=1
    // dp[3]=dp[2]+dp[0]=1+1=2
    // dp[4]=dp[3]+dp[1]=2+1=3
    // dp[5]=dp[4]+dp[2]+dp[0]=3+1+1=5
    // dp[6]=dp[5]+dp[3]+dp[1]=5+2+1=8
    steps[0] = 1; steps[1] = 3; steps[2] = 5;
    printf("%d\n", climb_stairs_set(6, steps, 3));
    // EXPECT: 8

    // Steps {2, 3}, n=6
    // dp[0]=1, dp[1]=0, dp[2]=dp[0]=1, dp[3]=dp[0]=1
    // dp[4]=dp[2]+dp[1]=1+0=1, dp[5]=dp[3]+dp[2]=1+1=2
    // dp[6]=dp[4]+dp[3]=1+1=2
    steps[0] = 2; steps[1] = 3;
    printf("%d\n", climb_stairs_set(6, steps, 2));
    // EXPECT: 2

    // Min cost: cost={10, 15, 20}, n=3
    // dp[0]=10, dp[1]=15
    // dp[2]=min(15,10)+20=10+20=30
    // reach top from dp[1]=15 or dp[2]=30 => 15
    cost[0]=10; cost[1]=15; cost[2]=20;
    printf("%d\n", min_cost_climb(cost, 3));
    // EXPECT: 15

    // Min cost: cost={1, 100, 1, 1, 1, 100, 1, 1, 100, 1}, n=10
    // dp[0]=1, dp[1]=100
    // dp[2]=min(100,1)+1=2, dp[3]=min(2,100)+1=3
    // dp[4]=min(3,2)+1=3, dp[5]=min(3,3)+100=103
    // dp[6]=min(103,3)+1=4, dp[7]=min(4,103)+1=5
    // dp[8]=min(5,4)+100=104, dp[9]=min(104,5)+1=6
    // top from dp[8]=104 or dp[9]=6 => 6
    cost[0]=1; cost[1]=100; cost[2]=1; cost[3]=1; cost[4]=1;
    cost[5]=100; cost[6]=1; cost[7]=1; cost[8]=100; cost[9]=1;
    printf("%d\n", min_cost_climb(cost, 10));
    // EXPECT: 6

    // Exact steps: n=4, k=3 => ways to reach 4 in exactly 3 steps
    // 1+1+2, 1+2+1, 2+1+1 => 3
    printf("%d\n", climb_exact_steps(4, 3));
    // EXPECT: 3

    // n=4, k=2 => only 2+2 => 1
    printf("%d\n", climb_exact_steps(4, 2));
    // EXPECT: 1

    // n=4, k=4 => only 1+1+1+1 => 1
    printf("%d\n", climb_exact_steps(4, 4));
    // EXPECT: 1

    // n=5, k=3 => 1+2+2, 2+1+2, 2+2+1 => 3
    printf("%d\n", climb_exact_steps(5, 3));
    // EXPECT: 3

    // Avoid stairs: n=5, forbidden = {0,0,0,1,0,0} (stair 3 forbidden)
    // dp[0]=1, dp[1]=1, dp[2]=2, dp[3]=0 (forbidden)
    // dp[4]=dp[3]+dp[2]=0+2=2, dp[5]=dp[4]+dp[3]=2+0=2
    for (i = 0; i < 32; i++) forbidden[i] = 0;
    forbidden[3] = 1;
    printf("%d\n", climb_avoid(5, forbidden));
    // EXPECT: 2

    // Avoid stairs 2 and 4: n=5
    // dp[0]=1, dp[1]=1, dp[2]=0, dp[3]=dp[2]+dp[1]=0+1=1
    // dp[4]=0, dp[5]=dp[4]+dp[3]=0+1=1
    for (i = 0; i < 32; i++) forbidden[i] = 0;
    forbidden[2] = 1;
    forbidden[4] = 1;
    printf("%d\n", climb_avoid(5, forbidden));
    // EXPECT: 1

    // Max points: stairs 1..5, points={0,3,1,5,2,8}
    // dp[0]=0, dp[1]=3, dp[2]=max(3,0)+1=4
    // dp[3]=max(4,3)+5=9, dp[4]=max(9,4)+2=11
    // dp[5]=max(11,9)+8=19
    points[0]=0; points[1]=3; points[2]=1; points[3]=5; points[4]=2; points[5]=8;
    printf("%d\n", max_points_climb(points, 5));
    // EXPECT: 19

    // n=0
    printf("%d\n", climb_stairs_2(0));
    // EXPECT: 0

    printf("done\n");
    // EXPECT: done

    return 0;
}
