int printf(const char *fmt, ...);

// Pascal's triangle and binomial coefficients

int triangle[15][15];

void build_pascal(int rows) {
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        triangle[i][0] = 1;
        triangle[i][i] = 1;
        for (j = 1; j < i; j++) {
            triangle[i][j] = triangle[i - 1][j - 1] + triangle[i - 1][j];
        }
    }
}

void print_pascal_row(int row) {
    int j;
    int first;
    first = 1;
    for (j = 0; j <= row; j++) {
        if (first) {
            printf("%d", triangle[row][j]);
            first = 0;
        } else {
            printf(" %d", triangle[row][j]);
        }
    }
    printf("\n");
}

int binomial(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    return triangle[n][k];
}

int row_sum(int row) {
    int sum;
    int j;
    sum = 0;
    for (j = 0; j <= row; j++) {
        sum = sum + triangle[row][j];
    }
    return sum;
}

int alternating_row_sum(int row) {
    int sum;
    int j;
    int sign;
    sum = 0;
    sign = 1;
    for (j = 0; j <= row; j++) {
        sum = sum + sign * triangle[row][j];
        sign = -sign;
    }
    return sum;
}

int is_row_symmetric(int row) {
    int j;
    for (j = 0; j <= row / 2; j++) {
        if (triangle[row][j] != triangle[row][row - j]) {
            return 0;
        }
    }
    return 1;
}

int max_in_row(int row) {
    int max_val;
    int j;
    max_val = 0;
    for (j = 0; j <= row; j++) {
        if (triangle[row][j] > max_val) {
            max_val = triangle[row][j];
        }
    }
    return max_val;
}

int diagonal_sum(int diag, int rows) {
    int sum;
    int i;
    sum = 0;
    for (i = diag; i < rows; i++) {
        sum = sum + triangle[i][diag];
    }
    return sum;
}

int hockey_stick(int start_row, int col, int count) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < count; i++) {
        sum = sum + triangle[start_row + i][col];
    }
    return sum;
}

int count_odds_in_row(int row) {
    int count;
    int j;
    count = 0;
    for (j = 0; j <= row; j++) {
        if (triangle[row][j] % 2 != 0) {
            count = count + 1;
        }
    }
    return count;
}

int count_evens_in_row(int row) {
    int count;
    int j;
    count = 0;
    for (j = 0; j <= row; j++) {
        if (triangle[row][j] % 2 == 0) {
            count = count + 1;
        }
    }
    return count;
}

int catalan_from_pascal(int n) {
    return triangle[2 * n][n] / (n + 1);
}

int main() {
    int i;
    int s;
    int a;
    int sym;
    int mx;
    int ds;
    int hs;
    int odds;
    int evens;
    int cat;

    build_pascal(15);

    // EXPECT: Pascal's triangle (rows 0-7):
    printf("Pascal's triangle (rows 0-7):\n");
    // EXPECT: 1
    // EXPECT: 1 1
    // EXPECT: 1 2 1
    // EXPECT: 1 3 3 1
    // EXPECT: 1 4 6 4 1
    // EXPECT: 1 5 10 10 5 1
    // EXPECT: 1 6 15 20 15 6 1
    // EXPECT: 1 7 21 35 35 21 7 1
    for (i = 0; i <= 7; i++) {
        print_pascal_row(i);
    }

    // EXPECT: C(10,3) = 120
    printf("C(10,3) = %d\n", binomial(10, 3));
    // EXPECT: C(12,6) = 924
    printf("C(12,6) = %d\n", binomial(12, 6));
    // EXPECT: C(14,7) = 3432
    printf("C(14,7) = %d\n", binomial(14, 7));

    s = row_sum(10);
    // EXPECT: Row 10 sum: 1024
    printf("Row 10 sum: %d\n", s);

    a = alternating_row_sum(5);
    // EXPECT: Row 5 alternating sum: 0
    printf("Row 5 alternating sum: %d\n", a);

    sym = is_row_symmetric(10);
    // EXPECT: Row 10 symmetric: 1
    printf("Row 10 symmetric: %d\n", sym);

    mx = max_in_row(10);
    // EXPECT: Max in row 10: 252
    printf("Max in row 10: %d\n", mx);

    ds = diagonal_sum(0, 10);
    // EXPECT: Diagonal 0 sum (10 rows): 10
    printf("Diagonal 0 sum (10 rows): %d\n", ds);

    ds = diagonal_sum(1, 10);
    // EXPECT: Diagonal 1 sum (10 rows): 45
    printf("Diagonal 1 sum (10 rows): %d\n", ds);

    hs = hockey_stick(2, 1, 4);
    // EXPECT: Hockey stick C(2,1)+C(3,1)+C(4,1)+C(5,1): 14
    printf("Hockey stick C(2,1)+C(3,1)+C(4,1)+C(5,1): %d\n", hs);

    odds = count_odds_in_row(7);
    // EXPECT: Odd entries in row 7: 8
    printf("Odd entries in row 7: %d\n", odds);

    evens = count_evens_in_row(6);
    // EXPECT: Even entries in row 6: 3
    printf("Even entries in row 6: %d\n", evens);

    cat = catalan_from_pascal(5);
    // EXPECT: Catalan(5) from Pascal: 42
    printf("Catalan(5) from Pascal: %d\n", cat);

    return 0;
}
