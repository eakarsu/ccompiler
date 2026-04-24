int printf(const char *fmt, ...);

int triangular(int n) {
    return n * (n + 1) / 2;
}

int is_triangular(int num) {
    int i = 1;
    while (triangular(i) < num) {
        i = i + 1;
    }
    return triangular(i) == num;
}

int catalan(int n) {
    if (n <= 1) return 1;
    int dp[20];
    dp[0] = 1;
    dp[1] = 1;
    int i = 2;
    while (i <= n) {
        dp[i] = 0;
        int j = 0;
        while (j < i) {
            dp[i] = dp[i] + dp[j] * dp[i - 1 - j];
            j = j + 1;
        }
        i = i + 1;
    }
    return dp[n];
}

int pascal_element(int row, int col) {
    if (col == 0 || col == row) return 1;
    int tri[15][15];
    int i = 0;
    while (i <= row) {
        tri[i][0] = 1;
        int j = 1;
        while (j < i) {
            tri[i][j] = tri[i-1][j-1] + tri[i-1][j];
            j = j + 1;
        }
        tri[i][i] = 1;
        i = i + 1;
    }
    return tri[row][col];
}

int pascal_row_sum(int row) {
    int sum = 0;
    int k = 0;
    while (k <= row) {
        sum = sum + pascal_element(row, k);
        k = k + 1;
    }
    return sum;
}

int pentagonal(int n) {
    return n * (3 * n - 1) / 2;
}

int square_pyramidal(int n) {
    return n * (n + 1) * (2 * n + 1) / 6;
}

int tetrahedral(int n) {
    return n * (n + 1) * (n + 2) / 6;
}

int sum_of_squares(int n) {
    int sum = 0;
    int i = 1;
    while (i <= n) {
        sum = sum + i * i;
        i = i + 1;
    }
    return sum;
}

int sum_of_cubes(int n) {
    int sum = 0;
    int i = 1;
    while (i <= n) {
        sum = sum + i * i * i;
        i = i + 1;
    }
    return sum;
}

int bell_number(int n) {
    int bell[12][12];
    bell[0][0] = 1;
    int i = 1;
    while (i <= n) {
        bell[i][0] = bell[i - 1][i - 1];
        int j = 1;
        while (j <= i) {
            bell[i][j] = bell[i][j - 1] + bell[i - 1][j - 1];
            j = j + 1;
        }
        i = i + 1;
    }
    return bell[n][0];
}

int main(void) {
    // EXPECT: tri(1) = 1
    printf("tri(1) = %d\n", triangular(1));
    // EXPECT: tri(5) = 15
    printf("tri(5) = %d\n", triangular(5));
    // EXPECT: tri(10) = 55
    printf("tri(10) = %d\n", triangular(10));

    // EXPECT: is_tri(10) = 1
    printf("is_tri(10) = %d\n", is_triangular(10));
    // EXPECT: is_tri(12) = 0
    printf("is_tri(12) = %d\n", is_triangular(12));

    // EXPECT: cat(0) = 1
    printf("cat(0) = %d\n", catalan(0));
    // EXPECT: cat(3) = 5
    printf("cat(3) = %d\n", catalan(3));
    // EXPECT: cat(5) = 42
    printf("cat(5) = %d\n", catalan(5));
    // EXPECT: cat(7) = 429
    printf("cat(7) = %d\n", catalan(7));

    // EXPECT: pas(4,2) = 6
    printf("pas(4,2) = %d\n", pascal_element(4, 2));
    // EXPECT: pas(6,3) = 20
    printf("pas(6,3) = %d\n", pascal_element(6, 3));
    // EXPECT: pas_row(5) = 32
    printf("pas_row(5) = %d\n", pascal_row_sum(5));

    // EXPECT: pent(4) = 22
    printf("pent(4) = %d\n", pentagonal(4));
    // EXPECT: sqpyr(4) = 30
    printf("sqpyr(4) = %d\n", square_pyramidal(4));
    // EXPECT: tetra(4) = 20
    printf("tetra(4) = %d\n", tetrahedral(4));

    // EXPECT: sum_sq(5) = 55
    printf("sum_sq(5) = %d\n", sum_of_squares(5));
    // EXPECT: sum_cu(4) = 100
    printf("sum_cu(4) = %d\n", sum_of_cubes(4));

    // EXPECT: bell(0) = 1
    printf("bell(0) = %d\n", bell_number(0));
    // EXPECT: bell(4) = 15
    printf("bell(4) = %d\n", bell_number(4));
    // EXPECT: bell(6) = 203
    printf("bell(6) = %d\n", bell_number(6));

    return 0;
}
