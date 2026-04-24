int printf(const char *fmt, ...);
// EXPECT: sum2=3\nsum3=6\nsum4=10\nsum5=15\nsum6=21\nsum7=28\nsum8=36\nprod3=60\nmixed=771\nsel(0)=10\nsel(1)=20\nsel(2)=30\nsel(3)=40\nsel(4)=50\nsel(5)=60\nsel(6)=70\nsel(7)=80\nweighted=35\nnested=40\nrec_params(1,2,3,4,5)=15\nrec_params(10,20,30,40,3)=103\nbefore rotate: 1 2 3 4 5\nafter rotate: 2 3 4 5 1\nafter rotate2: 3 4 5 1 2\nloop_total=640
// Test 247: Stress test - many parameters

int sum2(int a, int b) { return a + b; }
int sum3(int a, int b, int c) { return a + b + c; }
int sum4(int a, int b, int c, int d) { return a + b + c + d; }
int sum5(int a, int b, int c, int d, int e) { return a + b + c + d + e; }
int sum6(int a, int b, int c, int d, int e, int f) { return a+b+c+d+e+f; }
int sum7(int a, int b, int c, int d, int e, int f, int g) { return a+b+c+d+e+f+g; }
int sum8(int a, int b, int c, int d, int e, int f, int g, int h) { return a+b+c+d+e+f+g+h; }

int prod3(int a, int b, int c) { return a * b * c; }

int mixed_ops(int a, int b, int c, int d, int e, int f) {
    return (a + b) * (c - d) + e * f;
}

int select_nth(int a, int b, int c, int d, int e, int f, int g, int h, int n) {
    if (n == 0) return a;
    if (n == 1) return b;
    if (n == 2) return c;
    if (n == 3) return d;
    if (n == 4) return e;
    if (n == 5) return f;
    if (n == 6) return g;
    return h;
}

int weighted(int a, int b, int c, int d, int e, int wa, int wb, int wc, int wd, int we) {
    return a*wa + b*wb + c*wc + d*wd + e*we;
}

int nested_params(int a, int b, int c, int d) {
    return sum4(a * 2, b * 3, c * 4, d * 5);
}

int recursive_params(int a, int b, int c, int d, int n) {
    if (n <= 0) return a + b + c + d;
    return recursive_params(b, c, d, a + 1, n - 1);
}

void rotate_values(int *a, int *b, int *c, int *d, int *e) {
    int temp = *a;
    *a = *b;
    *b = *c;
    *c = *d;
    *d = *e;
    *e = temp;
}

int main(void) {
    printf("sum2=%d\n", sum2(1, 2));
    printf("sum3=%d\n", sum3(1, 2, 3));
    printf("sum4=%d\n", sum4(1, 2, 3, 4));
    printf("sum5=%d\n", sum5(1, 2, 3, 4, 5));
    printf("sum6=%d\n", sum6(1, 2, 3, 4, 5, 6));
    printf("sum7=%d\n", sum7(1, 2, 3, 4, 5, 6, 7));
    printf("sum8=%d\n", sum8(1, 2, 3, 4, 5, 6, 7, 8));

    printf("prod3=%d\n", prod3(3, 4, 5));
    printf("mixed=%d\n", mixed_ops(10, 20, 30, 5, 3, 7));

    int i = 0;
    while (i < 8) {
        printf("sel(%d)=%d\n", i, select_nth(10, 20, 30, 40, 50, 60, 70, 80, i));
        i = i + 1;
    }

    printf("weighted=%d\n", weighted(1, 2, 3, 4, 5, 5, 4, 3, 2, 1));
    printf("nested=%d\n", nested_params(1, 2, 3, 4));

    printf("rec_params(1,2,3,4,5)=%d\n", recursive_params(1, 2, 3, 4, 5));
    printf("rec_params(10,20,30,40,3)=%d\n", recursive_params(10, 20, 30, 40, 3));

    int a = 1; int b = 2; int c = 3; int d = 4; int e = 5;
    printf("before rotate: %d %d %d %d %d\n", a, b, c, d, e);
    rotate_values(&a, &b, &c, &d, &e);
    printf("after rotate: %d %d %d %d %d\n", a, b, c, d, e);
    rotate_values(&a, &b, &c, &d, &e);
    printf("after rotate2: %d %d %d %d %d\n", a, b, c, d, e);

    int total = 0;
    i = 0;
    while (i < 10) {
        total = total + sum8(i, i+1, i+2, i+3, i+4, i+5, i+6, i+7);
        i = i + 1;
    }
    printf("loop_total=%d\n", total);

    return 0;
}
