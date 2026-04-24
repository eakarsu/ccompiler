int printf(const char *fmt, ...);

// Pascal's triangle computation

int pascal[15][15];

void build_pascal(int rows) {
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        pascal[i][0] = 1;
        pascal[i][i] = 1;
        for (j = 1; j < i; j++) {
            pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
        }
    }
}

// Sum of row n should be 2^n
int row_sum(int n) {
    int sum = 0;
    int j;
    for (j = 0; j <= n; j++) {
        sum = sum + pascal[n][j];
    }
    return sum;
}

// Sum of diagonal: sum pascal[i+k][i] for i=0..
int diagonal_sum(int k, int max_rows) {
    int sum = 0;
    int i;
    for (i = 0; i + k < max_rows; i++) {
        sum = sum + pascal[i + k][i];
    }
    return sum;
}

// Hockey stick identity: sum C(i,k) for i=k..n equals C(n+1,k+1)
int hockey_stick(int n, int k) {
    int sum = 0;
    int i;
    for (i = k; i <= n; i++) {
        sum = sum + pascal[i][k];
    }
    return sum;
}

// Check symmetry: C(n,k) = C(n, n-k)
int check_symmetry(int rows) {
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j <= i; j++) {
            if (pascal[i][j] != pascal[i][i - j]) return 0;
        }
    }
    return 1;
}

// Power of 11: Row n of Pascal's gives digits of 11^n (for small n)
int power_of_11(int n) {
    // 11^0=1, 11^1=11, 11^2=121, 11^3=1331, 11^4=14641
    int result = 0;
    int j;
    int place = 1;
    for (j = n; j >= 0; j--) {
        result = result + pascal[n][j] * place;
        place = place * 10;
    }
    return result;
}

// Sum of squares of row n = C(2n, n)
int sum_of_squares(int n) {
    int sum = 0;
    int j;
    for (j = 0; j <= n; j++) {
        sum = sum + pascal[n][j] * pascal[n][j];
    }
    return sum;
}

// Count odd entries in row n (should be 2^(popcount of n))
int popcount(int x) {
    int c = 0;
    while (x) {
        c = c + (x & 1);
        x = x >> 1;
    }
    return c;
}

int count_odd_in_row(int n) {
    int count = 0;
    int j;
    for (j = 0; j <= n; j++) {
        if (pascal[n][j] % 2 != 0) count++;
    }
    return count;
}

int power_of_2(int n) {
    int r = 1;
    int i;
    for (i = 0; i < n; i++) r = r * 2;
    return r;
}

int main(void) {
    build_pascal(13);

    // Test 1: Print rows 0-4
    printf("row0: %d\n", pascal[0][0]);
    // EXPECT: row0: 1
    printf("row1: %d %d\n", pascal[1][0], pascal[1][1]);
    // EXPECT: row1: 1 1
    printf("row2: %d %d %d\n", pascal[2][0], pascal[2][1], pascal[2][2]);
    // EXPECT: row2: 1 2 1
    printf("row3: %d %d %d %d\n", pascal[3][0], pascal[3][1], pascal[3][2], pascal[3][3]);
    // EXPECT: row3: 1 3 3 1
    printf("row4: %d %d %d %d %d\n", pascal[4][0], pascal[4][1], pascal[4][2], pascal[4][3], pascal[4][4]);
    // EXPECT: row4: 1 4 6 4 1

    // Test 2: Row 6
    printf("row6: %d %d %d %d %d %d %d\n",
        pascal[6][0], pascal[6][1], pascal[6][2], pascal[6][3],
        pascal[6][4], pascal[6][5], pascal[6][6]);
    // EXPECT: row6: 1 6 15 20 15 6 1

    // Test 3: Row sums = 2^n
    printf("rsum(0): %d\n", row_sum(0));
    // EXPECT: rsum(0): 1
    printf("rsum(4): %d\n", row_sum(4));
    // EXPECT: rsum(4): 16
    printf("rsum(7): %d\n", row_sum(7));
    // EXPECT: rsum(7): 128
    printf("rsum(10): %d\n", row_sum(10));
    // EXPECT: rsum(10): 1024

    // Test 4: Symmetry
    printf("symmetric: %d\n", check_symmetry(13));
    // EXPECT: symmetric: 1

    // Test 5: Hockey stick identity
    // sum C(i,2) for i=2..5 = C(6,3)
    int hs = hockey_stick(5, 2);
    printf("hockey(5,2): %d = %d\n", hs, pascal[6][3]);
    // EXPECT: hockey(5,2): 20 = 20
    // sum C(i,1) for i=1..6 = C(7,2)
    hs = hockey_stick(6, 1);
    printf("hockey(6,1): %d = %d\n", hs, pascal[7][2]);
    // EXPECT: hockey(6,1): 21 = 21

    // Test 6: Power of 11 (works for rows 0-4 where no carry)
    printf("11^0: %d\n", power_of_11(0));
    // EXPECT: 11^0: 1
    printf("11^1: %d\n", power_of_11(1));
    // EXPECT: 11^1: 11
    printf("11^2: %d\n", power_of_11(2));
    // EXPECT: 11^2: 121
    printf("11^3: %d\n", power_of_11(3));
    // EXPECT: 11^3: 1331
    printf("11^4: %d\n", power_of_11(4));
    // EXPECT: 11^4: 14641

    // Test 7: Sum of squares of row = C(2n, n)
    printf("sqsum(3): %d = %d\n", sum_of_squares(3), pascal[6][3]);
    // EXPECT: sqsum(3): 20 = 20
    printf("sqsum(4): %d = %d\n", sum_of_squares(4), pascal[8][4]);
    // EXPECT: sqsum(4): 70 = 70
    printf("sqsum(5): %d = %d\n", sum_of_squares(5), pascal[10][5]);
    // EXPECT: sqsum(5): 252 = 252

    // Test 8: Count odd entries (should be 2^popcount(n))
    printf("odd(0): %d = %d\n", count_odd_in_row(0), power_of_2(popcount(0)));
    // EXPECT: odd(0): 1 = 1
    printf("odd(3): %d = %d\n", count_odd_in_row(3), power_of_2(popcount(3)));
    // EXPECT: odd(3): 4 = 4
    printf("odd(4): %d = %d\n", count_odd_in_row(4), power_of_2(popcount(4)));
    // EXPECT: odd(4): 2 = 2
    printf("odd(7): %d = %d\n", count_odd_in_row(7), power_of_2(popcount(7)));
    // EXPECT: odd(7): 8 = 8

    // Test 9: Specific values
    printf("C(10,5): %d\n", pascal[10][5]);
    // EXPECT: C(10,5): 252
    printf("C(12,4): %d\n", pascal[12][4]);
    // EXPECT: C(12,4): 495
    printf("C(12,6): %d\n", pascal[12][6]);
    // EXPECT: C(12,6): 924

    // Test 10: Diagonal sums (Fibonacci! diagonal k starts at (k,0))
    // diagonal 0: 1,1,1,1,... = sum of C(i,0) -> not Fibonacci
    // Shallow diagonals give Fibonacci
    // sum of C(n-k,k): anti-diagonal sums
    // Let's verify: row sums of antidiagonals
    // Antidiag d: sum pascal[d-j][j] for valid j
    int d;
    int j;
    int fib_sums[10];
    for (d = 0; d < 10; d++) {
        fib_sums[d] = 0;
        for (j = 0; j <= d; j++) {
            if (d - j >= j && d - j < 13) {
                fib_sums[d] = fib_sums[d] + pascal[d - j][j];
            }
        }
    }
    printf("fib: %d %d %d %d %d %d %d %d\n",
        fib_sums[0], fib_sums[1], fib_sums[2], fib_sums[3],
        fib_sums[4], fib_sums[5], fib_sums[6], fib_sums[7]);
    // EXPECT: fib: 1 1 2 3 5 8 13 21

    printf("done\n");
    // EXPECT: done
    return 0;
}
