int printf(const char *fmt, ...);

int side_effect_val = 0;

int set_and_return(int v) {
    side_effect_val = v;
    return v;
}

int main(void) {
    // Basic comma operator: evaluates left, discards, returns right
    int a = (1, 2, 3);
    // EXPECT: 3
    printf("%d\n", a);

    // Comma with side effects
    int x = 0;
    int y = 0;
    int z = (x = 10, y = 20, x + y);
    // EXPECT: 10
    printf("%d\n", x);
    // EXPECT: 20
    printf("%d\n", y);
    // EXPECT: 30
    printf("%d\n", z);

    // Comma in for loop init
    int i;
    int j;
    int sum = 0;
    for (i = 0, j = 10; i < 5; i++, j--) {
        sum = sum + i + j;
    }
    // i: 0,1,2,3,4  j: 10,9,8,7,6
    // pairs: 10, 10, 10, 10, 10 = 50
    // EXPECT: 50
    printf("%d\n", sum);

    // Verify final values after for loop
    // EXPECT: 5
    printf("%d\n", i);
    // EXPECT: 5
    printf("%d\n", j);

    // Comma in for loop increment only
    int p = 0;
    int q = 0;
    for (int k = 0; k < 4; k++, p = p + 2, q = q + 3) {
        // empty body, side effects in increment
    }
    // EXPECT: 8
    printf("%d\n", p);
    // EXPECT: 12
    printf("%d\n", q);

    // Comma operator with function calls
    int r = (set_and_return(42), set_and_return(99));
    // EXPECT: 99
    printf("%d\n", r);
    // EXPECT: 99
    printf("%d\n", side_effect_val);

    // Nested comma operators
    int na = 0;
    int nb = 0;
    int nc = 0;
    int nd = (na = 1, (nb = 2, (nc = 3, na + nb + nc)));
    // EXPECT: 6
    printf("%d\n", nd);

    // Comma in while condition (unusual but valid)
    int wcount = 0;
    int wval = 5;
    while (wval--, wval > 0) {
        wcount++;
    }
    // wval goes 4,3,2,1,0 -> loop runs when wval>0 after decrement
    // runs 4 times (wval=4,3,2,1 are >0, wval=0 stops)
    // EXPECT: 4
    printf("%d\n", wcount);

    // Comma in conditional expression
    int cval = (1, 2, 3) > 2 ? 100 : 200;
    // (1,2,3) evaluates to 3, 3 > 2 is true
    // EXPECT: 100
    printf("%d\n", cval);

    // Comma to sequence multiple assignments
    int s1 = 0;
    int s2 = 0;
    int s3 = 0;
    s1 = 5, s2 = 10, s3 = 15;
    // EXPECT: 5
    printf("%d\n", s1);
    // EXPECT: 10
    printf("%d\n", s2);
    // EXPECT: 15
    printf("%d\n", s3);

    // Comma operator returns the rightmost value
    int rv = (100, 200, 300, 400, 500);
    // EXPECT: 500
    printf("%d\n", rv);

    // Comma in array index (evaluates left, uses right as index)
    int arr[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;
    int aidx = 0;
    int aval = arr[(aidx = 3, aidx)];
    // EXPECT: 40
    printf("%d\n", aval);
    // EXPECT: 3
    printf("%d\n", aidx);

    // For loop with comma in all three parts
    int fsum = 0;
    int fi;
    int fj;
    for (fi = 0, fj = 100; fi < 3; fi++, fj -= 10) {
        fsum = fsum + fi + fj;
    }
    // fi=0,fj=100: 100; fi=1,fj=90: 91; fi=2,fj=80: 82
    // 100 + 91 + 82 = 273
    // EXPECT: 273
    printf("%d\n", fsum);

    // Comma with pointer operations
    int pval = 0;
    int *pp = &pval;
    int pres = (*pp = 42, *pp + 8);
    // EXPECT: 50
    printf("%d\n", pres);
    // EXPECT: 42
    printf("%d\n", pval);

    // Multiple comma operators building a value
    int build = 0;
    build = (build = build + 1, build = build * 10, build = build + 5);
    // build = 0+1=1, build = 1*10=10, build = 10+5=15
    // EXPECT: 15
    printf("%d\n", build);

    return 0;
}
