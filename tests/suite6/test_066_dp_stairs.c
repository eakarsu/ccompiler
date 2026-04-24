int printf(const char *fmt, ...);

int climb_stairs_2(int n) {
    if (n <= 1) return 1;
    int dp[60];
    dp[0] = 1;
    dp[1] = 1;
    int i = 2;
    while (i <= n) {
        dp[i] = dp[i - 1] + dp[i - 2];
        i = i + 1;
    }
    return dp[n];
}

int climb_stairs_3(int n) {
    if (n <= 1) return 1;
    if (n == 2) return 2;
    int dp[60];
    dp[0] = 1;
    dp[1] = 1;
    dp[2] = 2;
    int i = 3;
    while (i <= n) {
        dp[i] = dp[i - 1] + dp[i - 2] + dp[i - 3];
        i = i + 1;
    }
    return dp[n];
}

int climb_var_steps(int n, int steps[], int nsteps) {
    int dp[60];
    int i = 0;
    while (i <= n) {
        dp[i] = 0;
        i = i + 1;
    }
    dp[0] = 1;
    i = 1;
    while (i <= n) {
        int j = 0;
        while (j < nsteps) {
            if (steps[j] <= i) {
                dp[i] = dp[i] + dp[i - steps[j]];
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return dp[n];
}

int grid_paths(int m, int n) {
    int dp[20][20];
    int i = 0;
    while (i < m) {
        dp[i][0] = 1;
        i = i + 1;
    }
    int j = 0;
    while (j < n) {
        dp[0][j] = 1;
        j = j + 1;
    }
    i = 1;
    while (i < m) {
        j = 1;
        while (j < n) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
            j = j + 1;
        }
        i = i + 1;
    }
    return dp[m - 1][n - 1];
}

int grid_paths_obstacles(int grid[], int m, int n) {
    int dp[20][20];
    int i = 0;
    while (i < m) {
        int j = 0;
        while (j < n) {
            dp[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
    if (grid[0 * n + 0] == 1) return 0;
    dp[0][0] = 1;
    i = 1;
    while (i < m) {
        if (grid[i * n + 0] == 0 && dp[i - 1][0] == 1) {
            dp[i][0] = 1;
        }
        i = i + 1;
    }
    int j2 = 1;
    while (j2 < n) {
        if (grid[0 * n + j2] == 0 && dp[0][j2 - 1] == 1) {
            dp[0][j2] = 1;
        }
        j2 = j2 + 1;
    }
    i = 1;
    while (i < m) {
        int j = 1;
        while (j < n) {
            if (grid[i * n + j] == 0) {
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return dp[m - 1][n - 1];
}

int min_cost_stairs(int cost[], int n) {
    if (n == 0) return 0;
    if (n == 1) return cost[0];
    int dp[60];
    dp[0] = cost[0];
    dp[1] = cost[1];
    int i = 2;
    while (i < n) {
        int a = dp[i - 1];
        int b = dp[i - 2];
        dp[i] = cost[i] + (a < b ? a : b);
        i = i + 1;
    }
    int ra = dp[n - 1];
    int rb = dp[n - 2];
    return ra < rb ? ra : rb;
}

int main(void) {
    // EXPECT: stairs2(5) = 8
    printf("stairs2(5) = %d\n", climb_stairs_2(5));
    // EXPECT: stairs2(10) = 89
    printf("stairs2(10) = %d\n", climb_stairs_2(10));
    // EXPECT: stairs2(0) = 1
    printf("stairs2(0) = %d\n", climb_stairs_2(0));
    // EXPECT: stairs2(1) = 1
    printf("stairs2(1) = %d\n", climb_stairs_2(1));

    // EXPECT: stairs3(4) = 7
    printf("stairs3(4) = %d\n", climb_stairs_3(4));
    // EXPECT: stairs3(5) = 13
    printf("stairs3(5) = %d\n", climb_stairs_3(5));

    int steps[3];
    steps[0] = 1; steps[1] = 3; steps[2] = 5;
    // EXPECT: var(6) = 8
    printf("var(6) = %d\n", climb_var_steps(6, steps, 3));
    // EXPECT: var(1) = 1
    printf("var(1) = %d\n", climb_var_steps(1, steps, 3));

    int steps2[2];
    steps2[0] = 1; steps2[1] = 2;
    // EXPECT: var12(5) = 8
    printf("var12(5) = %d\n", climb_var_steps(5, steps2, 2));

    // EXPECT: grid(3,3) = 6
    printf("grid(3,3) = %d\n", grid_paths(3, 3));
    // EXPECT: grid(3,7) = 28
    printf("grid(3,7) = %d\n", grid_paths(3, 7));
    // EXPECT: grid(1,1) = 1
    printf("grid(1,1) = %d\n", grid_paths(1, 1));
    // EXPECT: grid(2,2) = 2
    printf("grid(2,2) = %d\n", grid_paths(2, 2));

    int g1[9];
    g1[0]=0; g1[1]=0; g1[2]=0;
    g1[3]=0; g1[4]=1; g1[5]=0;
    g1[6]=0; g1[7]=0; g1[8]=0;
    // EXPECT: obs(3,3) = 2
    printf("obs(3,3) = %d\n", grid_paths_obstacles(g1, 3, 3));

    int g2[4];
    g2[0]=1; g2[1]=0; g2[2]=0; g2[3]=0;
    // EXPECT: obs_blocked = 0
    printf("obs_blocked = %d\n", grid_paths_obstacles(g2, 2, 2));

    int cost1[10];
    cost1[0]=10; cost1[1]=15; cost1[2]=20;
    // EXPECT: mincost1 = 15
    printf("mincost1 = %d\n", min_cost_stairs(cost1, 3));

    int cost2[10];
    cost2[0]=1; cost2[1]=100; cost2[2]=1; cost2[3]=1;
    cost2[4]=1; cost2[5]=100; cost2[6]=1; cost2[7]=1;
    cost2[8]=100; cost2[9]=1;
    // EXPECT: mincost2 = 6
    printf("mincost2 = %d\n", min_cost_stairs(cost2, 10));

    return 0;
}
