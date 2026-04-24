int printf(const char *fmt, ...);
// EXPECT: poly vs horner:\n  x=-3: poly=352 horner=352 match=1\n  x=-2: poly=93 horner=93 match=1\n  x=-1: poly=18 horner=18 match=1\n  x=0: poly=7 horner=7 match=1\n  x=1: poly=12 horner=12 match=1\n  x=2: poly=57 horner=57 match=1\n  x=3: poly=238 horner=238 match=1\nmixed arith+bit = 12\nall ops = 36\ndot product = 35\nexpr tree = 11\nchar arith = 13\nsteps: 250 40 1600 0\n2->2 3->3 4->2 5->5 6->5 8->2 9->3 10->7 \nbig expr = 131
// Test: mixed operators in complex expressions

int polynomial(int x) {
    /* 3x^4 - 2x^3 + 5x^2 - x + 7 */
    return 3*x*x*x*x - 2*x*x*x + 5*x*x - x + 7;
}

int horner(int x) {
    /* Same polynomial using Horner's method: ((3x - 2)x + 5)x - 1)x + 7 */
    return (((3 * x - 2) * x + 5) * x - 1) * x + 7;
}

int dot_product(int a[], int b[], int n) {
    int sum, i;
    sum = 0;
    for (i = 0; i < n; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

int main(void) {
    int i;
    int r;
    int a[5], b[5];

    /* Polynomial evaluation */
    printf("poly vs horner:\n");
    for (i = -3; i <= 3; i++) {
        int p, h;
        p = polynomial(i);
        h = horner(i);
        printf("  x=%d: poly=%d horner=%d match=%d\n", i, p, h, p == h);
    }

    /* Mixed arithmetic and bitwise */
    r = (15 & 0xF) + (3 << 2) - (0xFF >> 4);
    printf("mixed arith+bit = %d\n", r);

    /* Complex expression with all operators */
    {
        int a2, b2, c2;
        a2 = 10; b2 = 3; c2 = 7;
        r = a2 * b2 + c2 % a2 - b2 * (a2 / c2) + (a2 & b2) | (c2 ^ b2);
        printf("all ops = %d\n", r);
    }

    /* Nested operations in function args */
    a[0] = 1; a[1] = 2; a[2] = 3; a[3] = 4; a[4] = 5;
    b[0] = 5; b[1] = 4; b[2] = 3; b[3] = 2; b[4] = 1;
    printf("dot product = %d\n", dot_product(a, b, 5));

    /* Arithmetic expression tree */
    {
        int x, y, z;
        x = 12; y = 7; z = 5;
        r = ((x + y) * z - x * (y - z)) / (z + 1);
        printf("expr tree = %d\n", r);
    }

    /* Compound expression with casts */
    {
        char c;
        int val;
        c = 'A';
        val = ((int)c - 'A') * 26 + ((int)c - 'A' + 13) % 26;
        printf("char arith = %d\n", val);
    }

    /* Multi-step computation */
    {
        int step1, step2, step3, step4;
        step1 = 100 + 50 * 3;
        step2 = step1 / 5 - 10;
        step3 = step2 * step2;
        step4 = step3 % 100 + step1 % 10;
        printf("steps: %d %d %d %d\n", step1, step2, step3, step4);
    }

    /* Boolean expressions as values in arithmetic */
    for (i = 1; i <= 10; i++) {
        int score;
        score = (i % 2 == 0) * 2 + (i % 3 == 0) * 3 + (i % 5 == 0) * 5;
        if (score > 0) {
            printf("%d->%d ", i, score);
        }
    }
    printf("\n");

    /* Large compound expression */
    r = 1 + 2 * 3 - 4 / 2 + 5 % 3 + 6 * 7 - 8 + 9 * 10;
    printf("big expr = %d\n", r);

    return 0;
}
