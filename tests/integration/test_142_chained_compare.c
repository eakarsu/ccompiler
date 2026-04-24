int printf(const char *fmt, ...);
// EXPECT: range [0,10]: 001111111111100\neq(1,1,1) = 1\neq(1,2,1) = 0\neq(1,1,2) = 0\nany0(1,2,3) = 0\nany0(1,0,3) = 1\nany0(0,0,0) = 1\nsorted(1,2,3) = 1\nsorted(1,3,2) = 0\nsorted(3,2,1) = 0\nsorted(1,1,1) = 1\n!(a&&b) = 1, !a||!b = 1\n!(a||b) = 0, !a&&!b = 0\ncompare matrix:\n010 100 100 100 100 \n001 010 100 100 100 \n001 001 010 100 100 \n001 001 001 010 100 \n001 001 001 001 010 \ncount > 4: 2\ncount even: 0\nclassify: mid\nXOR(0,0) = 0  XOR(0,1) = 1  XOR(1,0) = 1  XOR(1,1) = 0  
// Test: chained comparisons and boolean expressions

int in_range(int x, int lo, int hi) {
    return x >= lo && x <= hi;
}

int all_equal(int a, int b, int c) {
    return a == b && b == c;
}

int any_zero(int a, int b, int c) {
    return a == 0 || b == 0 || c == 0;
}

int is_sorted3(int a, int b, int c) {
    return a <= b && b <= c;
}

int main(void) {
    int i, j;
    int a, b, c;

    /* Range checks */
    printf("range [0,10]: ");
    for (i = -2; i <= 12; i++) {
        printf("%d", in_range(i, 0, 10));
    }
    printf("\n");

    /* Chained equality */
    printf("eq(1,1,1) = %d\n", all_equal(1, 1, 1));
    printf("eq(1,2,1) = %d\n", all_equal(1, 2, 1));
    printf("eq(1,1,2) = %d\n", all_equal(1, 1, 2));

    /* Any zero */
    printf("any0(1,2,3) = %d\n", any_zero(1, 2, 3));
    printf("any0(1,0,3) = %d\n", any_zero(1, 0, 3));
    printf("any0(0,0,0) = %d\n", any_zero(0, 0, 0));

    /* Sorted check */
    printf("sorted(1,2,3) = %d\n", is_sorted3(1, 2, 3));
    printf("sorted(1,3,2) = %d\n", is_sorted3(1, 3, 2));
    printf("sorted(3,2,1) = %d\n", is_sorted3(3, 2, 1));
    printf("sorted(1,1,1) = %d\n", is_sorted3(1, 1, 1));

    /* Complex boolean: De Morgan's laws */
    a = 1; b = 0;
    printf("!(a&&b) = %d, !a||!b = %d\n", !(a && b), !a || !b);
    printf("!(a||b) = %d, !a&&!b = %d\n", !(a || b), !a && !b);

    /* Multiple comparison chain */
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            int lt, eq, gt;
            lt = (i < j);
            eq = (i == j);
            gt = (i > j);
            if (i == 0 && j == 0) printf("compare matrix:\n");
            printf("%d%d%d ", lt, eq, gt);
        }
        printf("\n");
    }

    /* Boolean to int arithmetic */
    a = 5; b = 3; c = 7;
    printf("count > 4: %d\n", (a > 4) + (b > 4) + (c > 4));
    printf("count even: %d\n", (a % 2 == 0) + (b % 2 == 0) + (c % 2 == 0));

    /* Nested boolean */
    a = 10;
    printf("classify: %s\n",
        (a > 0 && a <= 5) ? "low" :
        (a > 5 && a <= 15) ? "mid" :
        (a > 15) ? "high" : "non-pos");

    /* XOR via boolean */
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            printf("XOR(%d,%d) = %d  ", i, j, (i || j) && !(i && j));
        }
    }
    printf("\n");

    return 0;
}
