int printf(const char *fmt, ...);
// EXPECT: max(5,10) = 10\nmin(5,10) = 5\nsign(-3) = -1\nsign(-2) = -1\nsign(-1) = -1\nsign(0) = 0\nsign(1) = 1\nsign(2) = 1\nsign(3) = 1\nabs(-7) = 7\nabs(7) = 7\nabs(0) = 0\nclamp(5,0,10) = 5\nclamp(-3,0,10) = 0\nclamp(15,0,10) = 10\nmax3(3,7,5) = 7\nmax3(9,2,6) = 9\nmin3(3,7,5) = 3\nmin3(9,2,6) = 2\narr[ternary] = 200\ngrade(85) = B\ngrade(42) = F
// Test: ternary operator chains

int abs_val(int x) {
    return x < 0 ? -x : x;
}

int clamp(int x, int lo, int hi) {
    return x < lo ? lo : (x > hi ? hi : x);
}

int max3(int a, int b, int c) {
    return a > b ? (a > c ? a : c) : (b > c ? b : c);
}

int min3(int a, int b, int c) {
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int sign(int x) {
    return x > 0 ? 1 : (x < 0 ? -1 : 0);
}

int main(void) {
    int a, b, c;
    int i;
    int r;

    /* Simple ternary */
    a = 5;
    b = 10;
    printf("max(%d,%d) = %d\n", a, b, a > b ? a : b);
    printf("min(%d,%d) = %d\n", a, b, a < b ? a : b);

    /* Nested ternary for classification */
    for (i = -3; i <= 3; i++) {
        printf("sign(%d) = %d\n", i, sign(i));
    }

    /* Abs via ternary */
    printf("abs(-7) = %d\n", abs_val(-7));
    printf("abs(7) = %d\n", abs_val(7));
    printf("abs(0) = %d\n", abs_val(0));

    /* Clamp function */
    printf("clamp(5,0,10) = %d\n", clamp(5, 0, 10));
    printf("clamp(-3,0,10) = %d\n", clamp(-3, 0, 10));
    printf("clamp(15,0,10) = %d\n", clamp(15, 0, 10));

    /* Max of three */
    printf("max3(3,7,5) = %d\n", max3(3, 7, 5));
    printf("max3(9,2,6) = %d\n", max3(9, 2, 6));

    /* Min of three */
    printf("min3(3,7,5) = %d\n", min3(3, 7, 5));
    printf("min3(9,2,6) = %d\n", min3(9, 2, 6));

    /* Ternary as array index */
    {
        int arr[3];
        arr[0] = 100;
        arr[1] = 200;
        arr[2] = 300;
        a = 1;
        r = arr[a > 0 ? 1 : 0];
        printf("arr[ternary] = %d\n", r);
    }

    /* Chained ternary for grading */
    {
        int score;
        score = 85;
        printf("grade(%d) = %c\n", score,
               score >= 90 ? 'A' :
               score >= 80 ? 'B' :
               score >= 70 ? 'C' :
               score >= 60 ? 'D' : 'F');
        score = 42;
        printf("grade(%d) = %c\n", score,
               score >= 90 ? 'A' :
               score >= 80 ? 'B' :
               score >= 70 ? 'C' :
               score >= 60 ? 'D' : 'F');
    }

    return 0;
}
