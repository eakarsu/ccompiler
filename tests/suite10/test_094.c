int printf(const char *fmt, ...);

// Long expression chains stress test

int compute_chain1(int a, int b, int c, int d, int e) {
    int r = a + b * c - d + e * a - b + c * d - e + a * b;
    r = r + (a - b) * (c + d) - (e * a) + (b + c) * (d - e);
    r = r + a * a + b * b + c * c + d * d + e * e;
    r = r - (a + b + c + d + e) * 2;
    return r;
}

int compute_chain2(int x) {
    int a = x * x + x + 1;
    int b = a * a - a + 2;
    int c = b + a - x * 3;
    int d = c * 2 - b + a * x;
    int e = d + c + b + a + x;
    int f = e - d * 2 + c * 3 - b * 4 + a * 5;
    int g = f + e + d + c + b + a;
    int h = g * 2 - f + e - d + c - b + a;
    return h;
}

int long_ternary(int x) {
    int r = (x > 90) ? 10 :
            (x > 80) ? 9 :
            (x > 70) ? 8 :
            (x > 60) ? 7 :
            (x > 50) ? 6 :
            (x > 40) ? 5 :
            (x > 30) ? 4 :
            (x > 20) ? 3 :
            (x > 10) ? 2 :
            (x > 0)  ? 1 : 0;
    return r;
}

int chained_logical(int a, int b, int c, int d, int e, int f) {
    if (a > 0 && b > 0 && c > 0 && d > 0 && e > 0 && f > 0) {
        return 1;
    }
    if (a > 0 || b > 0 || c > 0 || d > 0 || e > 0 || f > 0) {
        return 2;
    }
    return 0;
}

int nested_arith(int x) {
    return ((((((((x + 1) * 2) + 3) * 4) + 5) * 6) + 7) * 8) + 9;
}

int bitwise_chain(int x) {
    int a = x & 255;
    int b = (x >> 8) & 255;
    int c = a ^ b;
    int d = c | (a & b);
    int e = d ^ (a | b);
    int f = ~e & 255;
    int g = (f << 4) | (f >> 4);
    return g & 255;
}

int complex_cond(int a, int b, int c) {
    int r = 0;
    if ((a > b && b > c) || (a < b && b < c)) {
        r = r + 100;
    }
    if ((a + b > c) && (b + c > a) && (a + c > b)) {
        r = r + 200;
    }
    if ((a * a + b * b > c * c) || (b * b + c * c > a * a)) {
        r = r + 300;
    }
    return r;
}

int accumulate_expr(int n) {
    int sum = 0;
    int prod = 1;
    int alt = 0;
    int i = 1;
    while (i <= n) {
        sum = sum + i;
        if (i <= 10) {
            prod = prod * i;
        }
        alt = alt + ((i % 2 == 0) ? -i : i);
        i = i + 1;
    }
    return sum + prod + alt;
}

int main() {
    int r1 = compute_chain1(1, 2, 3, 4, 5);
    printf("chain1: %d\n", r1);
    // EXPECT: chain1: 23

    int r2 = compute_chain1(10, 20, 30, 40, 50);
    printf("chain1 big: %d\n", r2);
    // EXPECT: chain1 big: 5900

    int r3 = compute_chain2(2);
    printf("chain2 x=2: %d\n", r3);
    // EXPECT: chain2 x=2: 766

    int r4 = compute_chain2(5);
    printf("chain2 x=5: %d\n", r4);
    // EXPECT: chain2 x=5: 14161

    printf("ternary 95: %d\n", long_ternary(95));
    // EXPECT: ternary 95: 10
    printf("ternary 55: %d\n", long_ternary(55));
    // EXPECT: ternary 55: 6
    printf("ternary 5: %d\n", long_ternary(5));
    // EXPECT: ternary 5: 1
    printf("ternary 0: %d\n", long_ternary(0));
    // EXPECT: ternary 0: 0

    printf("all pos: %d\n", chained_logical(1, 2, 3, 4, 5, 6));
    // EXPECT: all pos: 1
    printf("some pos: %d\n", chained_logical(-1, -2, 3, -4, -5, -6));
    // EXPECT: some pos: 2
    printf("all neg: %d\n", chained_logical(-1, -2, -3, -4, -5, -6));
    // EXPECT: all neg: 0

    printf("nested 1: %d\n", nested_arith(1));
    // EXPECT: nested 1: 1649
    printf("nested 0: %d\n", nested_arith(0));
    // EXPECT: nested 0: 1265

    printf("bitwise 0: %d\n", bitwise_chain(0));
    // EXPECT: bitwise 0: 255
    printf("bitwise 255: %d\n", bitwise_chain(255));
    // EXPECT: bitwise 255: 255
    printf("bitwise 256: %d\n", bitwise_chain(256));
    // EXPECT: bitwise 256: 255

    printf("cond 3 2 1: %d\n", complex_cond(3, 2, 1));
    // EXPECT: cond 3 2 1: 400
    printf("cond 1 2 3: %d\n", complex_cond(1, 2, 3));
    // EXPECT: cond 1 2 3: 400

    printf("accum 20: %d\n", accumulate_expr(20));
    // EXPECT: accum 20: 3629000

    return 0;
}
