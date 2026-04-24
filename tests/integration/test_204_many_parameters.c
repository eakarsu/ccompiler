int printf(const char *fmt, ...);
// EXPECT: sum5=15\nsum6=21\nsum7=28\nsum8=36\nweighted=550\nmax5=9\nmin5=1\ndot=32\nclamp=5\npoly(2)=15\npoly(3)=69\npoly(0)=3\ntotal=125
// Test 204: Functions with many parameters (5+)

int sum5(int a, int b, int c, int d, int e) {
    return a + b + c + d + e;
}

int sum6(int a, int b, int c, int d, int e, int f) {
    return a + b + c + d + e + f;
}

int sum7(int a, int b, int c, int d, int e, int f, int g) {
    return a + b + c + d + e + f + g;
}

int sum8(int a, int b, int c, int d, int e, int f, int g, int h) {
    return a + b + c + d + e + f + g + h;
}

int weighted_sum(int a, int b, int c, int d, int e) {
    return 1*a + 2*b + 3*c + 4*d + 5*e;
}

int max5(int a, int b, int c, int d, int e) {
    int m = a;
    if (b > m) m = b;
    if (c > m) m = c;
    if (d > m) m = d;
    if (e > m) m = e;
    return m;
}

int min5(int a, int b, int c, int d, int e) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    if (d < m) m = d;
    if (e < m) m = e;
    return m;
}

int dot_product(int a1, int a2, int a3, int b1, int b2, int b3) {
    return a1*b1 + a2*b2 + a3*b3;
}

int clamp_range(int val, int lo, int hi, int step, int offset) {
    int result = val + offset;
    result = result - (result / step) * step + lo;
    if (result > hi) result = hi;
    if (result < lo) result = lo;
    return result;
}

int polynomial(int x, int a, int b, int c, int d, int e) {
    return a*x*x*x*x + b*x*x*x + c*x*x + d*x + e;
}

int main(void) {
    printf("sum5=%d\n", sum5(1, 2, 3, 4, 5));
    printf("sum6=%d\n", sum6(1, 2, 3, 4, 5, 6));
    printf("sum7=%d\n", sum7(1, 2, 3, 4, 5, 6, 7));
    printf("sum8=%d\n", sum8(1, 2, 3, 4, 5, 6, 7, 8));
    printf("weighted=%d\n", weighted_sum(10, 20, 30, 40, 50));
    printf("max5=%d\n", max5(3, 7, 2, 9, 1));
    printf("min5=%d\n", min5(3, 7, 2, 9, 1));
    printf("dot=%d\n", dot_product(1, 2, 3, 4, 5, 6));
    printf("clamp=%d\n", clamp_range(100, 0, 50, 7, 3));
    printf("poly(2)=%d\n", polynomial(2, 1, -1, 2, -2, 3));
    printf("poly(3)=%d\n", polynomial(3, 1, -1, 2, -2, 3));
    printf("poly(0)=%d\n", polynomial(0, 1, -1, 2, -2, 3));

    int total = 0;
    int i = 1;
    while (i <= 5) {
        total = total + sum5(i, i+1, i+2, i+3, i+4);
        i = i + 1;
    }
    printf("total=%d\n", total);

    return 0;
}
