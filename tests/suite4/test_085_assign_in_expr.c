int printf(const char *fmt, ...);

int counter = 0;

int next(void) {
    counter = counter + 1;
    return counter;
}

int compute(int v) {
    return v * 2 + 1;
}

int main(void) {
    // Chained assignment: x = y = z = 5
    int x;
    int y;
    int z;
    x = y = z = 5;
    // EXPECT: 5
    printf("%d\n", x);
    // EXPECT: 5
    printf("%d\n", y);
    // EXPECT: 5
    printf("%d\n", z);

    // Chained assignment with different value
    int a;
    int b;
    int c;
    a = b = c = 100;
    // EXPECT: 100
    printf("%d\n", a);
    // EXPECT: 100
    printf("%d\n", b);

    // Assignment in if condition
    int val = 0;
    if ((val = 42) > 0) {
        // EXPECT: 42
        printf("%d\n", val);
    }

    // Assignment from function in condition
    int fval;
    fval = compute(10);
    // compute(10) = 21
    // EXPECT: 21
    printf("%d\n", fval);

    // Assignment in while condition
    int wval;
    int wsum = 0;
    counter = 0;
    while ((wval = next()) <= 5) {
        wsum = wsum + wval;
    }
    // next returns 1,2,3,4,5,6 -> loop runs for 1..5
    // 1+2+3+4+5 = 15
    // EXPECT: 15
    printf("%d\n", wsum);
    // wval should be 6 (the one that failed the condition)
    // EXPECT: 6
    printf("%d\n", wval);

    // Assignment result used in arithmetic
    int m = 0;
    int n = (m = 10) + 5;
    // m is set to 10, then 10 + 5 = 15
    // EXPECT: 10
    printf("%d\n", m);
    // EXPECT: 15
    printf("%d\n", n);

    // Nested assignment in expression
    int p = 0;
    int q = 0;
    int r = (p = (q = 7) + 3);
    // q = 7, p = 7 + 3 = 10, r = 10
    // EXPECT: 7
    printf("%d\n", q);
    // EXPECT: 10
    printf("%d\n", p);
    // EXPECT: 10
    printf("%d\n", r);

    // Assignment in ternary condition
    int tv = 0;
    int tr = ((tv = 15) > 10) ? tv : 0;
    // tv = 15, 15 > 10 is true, so tr = 15
    // EXPECT: 15
    printf("%d\n", tr);

    // Multiple assignments building a chain
    int s1 = 0;
    int s2 = 0;
    int s3 = 0;
    s1 = 1;
    s2 = s1 + 1;
    s3 = s2 + 1;
    // EXPECT: 1
    printf("%d\n", s1);
    // EXPECT: 2
    printf("%d\n", s2);
    // EXPECT: 3
    printf("%d\n", s3);

    // Assignment to array elements in expression
    int arr[5];
    int aidx = 0;
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    int asum = (arr[3] = 40) + (arr[4] = 50);
    // EXPECT: 90
    printf("%d\n", asum);
    // EXPECT: 40
    printf("%d\n", arr[3]);
    // EXPECT: 50
    printf("%d\n", arr[4]);

    // Assignment via pointer in expression
    int pval = 0;
    int *pp = &pval;
    int pres = (*pp = 99) + 1;
    // EXPECT: 99
    printf("%d\n", pval);
    // EXPECT: 100
    printf("%d\n", pres);

    // Assignment in for loop init
    int fi;
    int fsum2 = 0;
    for (fi = 1; fi <= 4; fi++) {
        fsum2 = fsum2 + fi;
    }
    // 1+2+3+4 = 10
    // EXPECT: 10
    printf("%d\n", fsum2);

    // Chained assignment with computation
    int ca;
    int cb;
    ca = cb = 3 * 4 + 1;
    // 13
    // EXPECT: 13
    printf("%d\n", ca);
    // EXPECT: 13
    printf("%d\n", cb);

    // Assignment to struct member in expression
    struct { int x; int y; } pt;
    int ptsum = (pt.x = 30) + (pt.y = 40);
    // EXPECT: 70
    printf("%d\n", ptsum);
    // EXPECT: 30
    printf("%d\n", pt.x);
    // EXPECT: 40
    printf("%d\n", pt.y);

    // Assignment result compared
    int cmp = 0;
    int is_positive = ((cmp = -5) > 0);
    // EXPECT: 0
    printf("%d\n", is_positive);
    // EXPECT: -5
    printf("%d\n", cmp);

    return 0;
}
