int printf(const char *fmt, ...);

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }
int sub(int a, int b) { return a - b; }

int main(void) {
    // Simple nested arithmetic
    int a = (2 + 3) * 4;
    // EXPECT: 20
    printf("%d\n", a);

    // Deeper nesting
    int b = ((2 + 3) * (4 - 1)) + 1;
    // (5 * 3) + 1 = 16
    // EXPECT: 16
    printf("%d\n", b);

    // Three levels deep
    int c = (1 + (2 * (3 + 4)));
    // 1 + (2 * 7) = 1 + 14 = 15
    // EXPECT: 15
    printf("%d\n", c);

    // Division and modulo nested
    int d = (100 / (2 + 3)) % 7;
    // 100 / 5 = 20, 20 % 7 = 6
    // EXPECT: 6
    printf("%d\n", d);

    // Complex nested expression
    int e = ((10 + 20) * (30 - 10)) / ((5 + 5) * 3);
    // (30 * 20) / (10 * 3) = 600 / 30 = 20
    // EXPECT: 20
    printf("%d\n", e);

    // Nested with negation
    int f = -(3 + 4) * 2;
    // -7 * 2 = -14
    // EXPECT: -14
    printf("%d\n", f);

    // Nested function calls in expression
    int g = add(mul(2, 3), sub(10, 4));
    // add(6, 6) = 12
    // EXPECT: 12
    printf("%d\n", g);

    // Deeply nested function calls
    int h = add(add(add(1, 2), add(3, 4)), add(add(5, 6), add(7, 8)));
    // add(add(3,7), add(11,15)) = add(10, 26) = 36
    // EXPECT: 36
    printf("%d\n", h);

    // Mixed operators with precedence
    int i = 2 + 3 * 4 - 1;
    // 2 + 12 - 1 = 13
    // EXPECT: 13
    printf("%d\n", i);

    // Explicit grouping overriding precedence
    int j = (2 + 3) * (4 - 1);
    // 5 * 3 = 15
    // EXPECT: 15
    printf("%d\n", j);

    // Nested ternary in expression
    int k = 5;
    int l = (k > 3 ? (k > 7 ? 100 : 50) : 10);
    // k=5 > 3 is true, 5 > 7 is false -> 50
    // EXPECT: 50
    printf("%d\n", l);

    // Bitwise nested
    int m = ((0xFF & 0x0F) | (0xF0 & 0xAA));
    // 0x0F | 0xA0 = 0xAF = 175
    // EXPECT: 175
    printf("%d\n", m);

    // Shift in nested expression
    int n = (1 << (2 + 3)) + (256 >> (1 + 1));
    // (1 << 5) + (256 >> 2) = 32 + 64 = 96
    // EXPECT: 96
    printf("%d\n", n);

    // Deeply nested with all basic arithmetic
    int o = ((((1 + 2) * 3) - 4) / 5) + 6;
    // ((3*3) - 4) / 5 + 6 = (9-4)/5 + 6 = 5/5 + 6 = 1 + 6 = 7
    // EXPECT: 7
    printf("%d\n", o);

    // Nested comparison in expression
    int p = (3 > 2) + (4 > 5) + (6 == 6) + (7 != 8);
    // 1 + 0 + 1 + 1 = 3
    // EXPECT: 3
    printf("%d\n", p);

    // Multiple nested subexpressions
    int q = (2 * 3 + 4) * (5 - 2 * 1) + (8 / 4 - 1);
    // (6+4) * (5-2) + (2-1) = 10 * 3 + 1 = 31
    // EXPECT: 31
    printf("%d\n", q);

    // Nested logical operators
    int r = (1 && (0 || 1)) + ((1 || 0) && (1 && 1));
    // (1 && 1) + (1 && 1) = 1 + 1 = 2
    // EXPECT: 2
    printf("%d\n", r);

    // Expression with array indexing
    int arr[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;
    int s = arr[1 + 1] + arr[4 - 2];
    // arr[2] + arr[2] = 30 + 30 = 60
    // EXPECT: 60
    printf("%d\n", s);

    int t = arr[(arr[0] / 10) + 1];
    // arr[0]/10 = 1, arr[1+1] = arr[2] = 30
    // EXPECT: 30
    printf("%d\n", t);

    // Nested expression building a polynomial: 3x^2 + 2x + 1 at x=5
    int xv = 5;
    int poly = 3 * xv * xv + 2 * xv + 1;
    // 75 + 10 + 1 = 86
    // EXPECT: 86
    printf("%d\n", poly);

    // Same polynomial with explicit grouping
    int poly2 = ((3 * (xv * xv)) + (2 * xv)) + 1;
    // EXPECT: 86
    printf("%d\n", poly2);

    // Nested expression with struct
    struct point { int x; int y; };
    struct point pt;
    pt.x = 3;
    pt.y = 4;
    int dist_sq = pt.x * pt.x + pt.y * pt.y;
    // 9 + 16 = 25
    // EXPECT: 25
    printf("%d\n", dist_sq);

    // Complex expression with multiple data sources
    int data1 = 10;
    int data2 = 20;
    int data3 = 30;
    int combined = ((data1 + data2) * data3 - data1 * data2) / (data3 - data2);
    // (30 * 30 - 200) / 10 = (900 - 200) / 10 = 700 / 10 = 70
    // EXPECT: 70
    printf("%d\n", combined);

    // Expression result used as condition
    int cond_val = ((5 * 3 - 14) > 0) ? 1 : 0;
    // 15-14 = 1 > 0 is true
    // EXPECT: 1
    printf("%d\n", cond_val);

    return 0;
}
