int printf(const char *fmt, ...);

int safe_add(int a, int b, int *result) {
    if (b > 0 && a > 2147483647 - b) return 0;
    if (b < 0 && a < (-2147483647 - 1) - b) return 0;
    *result = a + b;
    return 1;
}

int safe_sub(int a, int b, int *result) {
    if (b < 0 && a > 2147483647 + b) return 0;
    if (b > 0 && a < (-2147483647 - 1) + b) return 0;
    *result = a - b;
    return 1;
}

int safe_mul(int a, int b, int *result) {
    if (a == 0 || b == 0) {
        *result = 0;
        return 1;
    }
    if (a > 0 && b > 0 && a > 2147483647 / b) return 0;
    if (a > 0 && b < 0 && b < (-2147483647 - 1) / a) return 0;
    if (a < 0 && b > 0 && a < (-2147483647 - 1) / b) return 0;
    if (a < 0 && b < 0 && a < 2147483647 / b) return 0;
    *result = a * b;
    return 1;
}

int would_add_overflow(int a, int b) {
    if (b > 0 && a > 2147483647 - b) return 1;
    if (b < 0 && a < (-2147483647 - 1) - b) return 1;
    return 0;
}

int would_mul_overflow(int a, int b) {
    if (a == 0 || b == 0) return 0;
    if (a > 0 && b > 0 && a > 2147483647 / b) return 1;
    if (a > 0 && b < 0 && b < (-2147483647 - 1) / a) return 1;
    if (a < 0 && b > 0 && a < (-2147483647 - 1) / b) return 1;
    if (a < 0 && b < 0 && a < 2147483647 / b) return 1;
    return 0;
}

int checked_add_chain(int a, int b, int c) {
    int tmp;
    int res;
    if (!safe_add(a, b, &tmp)) return -1;
    if (!safe_add(tmp, c, &res)) return -1;
    return res;
}

int checked_mul_chain(int a, int b, int c) {
    int tmp;
    int res;
    if (!safe_mul(a, b, &tmp)) return -1;
    if (!safe_mul(tmp, c, &res)) return -1;
    return res;
}

int safe_square(int x, int *result) {
    if (x == 0) { *result = 0; return 1; }
    int abs_x = x;
    if (abs_x < 0) abs_x = -abs_x;
    if (abs_x > 46340) return 0;
    *result = x * x;
    return 1;
}

int main(void) {
    int r;

    // EXPECT: safe_add(100,200)=300 ok=1
    int ok1 = safe_add(100, 200, &r);
    printf("safe_add(100,200)=%d ok=%d\n", r, ok1);

    // EXPECT: safe_add(2147483647,1) ok=0
    int ok2 = safe_add(2147483647, 1, &r);
    printf("safe_add(2147483647,1) ok=%d\n", ok2);

    // EXPECT: safe_add(-2147483648,-1) ok=0
    int ok3 = safe_add(-2147483647 - 1, -1, &r);
    printf("safe_add(-2147483648,-1) ok=%d\n", ok3);

    // EXPECT: safe_sub(10,3)=7 ok=1
    int ok4 = safe_sub(10, 3, &r);
    printf("safe_sub(10,3)=%d ok=%d\n", r, ok4);

    // EXPECT: safe_sub(-2147483648,1) ok=0
    int ok5 = safe_sub(-2147483647 - 1, 1, &r);
    printf("safe_sub(-2147483648,1) ok=%d\n", ok5);

    // EXPECT: safe_mul(100,200)=20000 ok=1
    int ok6 = safe_mul(100, 200, &r);
    printf("safe_mul(100,200)=%d ok=%d\n", r, ok6);

    // EXPECT: safe_mul(2147483647,2) ok=0
    int ok7 = safe_mul(2147483647, 2, &r);
    printf("safe_mul(2147483647,2) ok=%d\n", ok7);

    // EXPECT: safe_mul(-1,-2147483648) ok=0
    int ok8 = safe_mul(-1, -2147483647 - 1, &r);
    printf("safe_mul(-1,-2147483648) ok=%d\n", ok8);

    // EXPECT: safe_mul(0,2147483647)=0 ok=1
    int ok9 = safe_mul(0, 2147483647, &r);
    printf("safe_mul(0,2147483647)=%d ok=%d\n", r, ok9);

    // EXPECT: would_add_overflow(2147483640,10)=1
    printf("would_add_overflow(2147483640,10)=%d\n", would_add_overflow(2147483640, 10));

    // EXPECT: would_add_overflow(100,200)=0
    printf("would_add_overflow(100,200)=%d\n", would_add_overflow(100, 200));

    // EXPECT: would_mul_overflow(50000,50000)=1
    printf("would_mul_overflow(50000,50000)=%d\n", would_mul_overflow(50000, 50000));

    // EXPECT: would_mul_overflow(100,100)=0
    printf("would_mul_overflow(100,100)=%d\n", would_mul_overflow(100, 100));

    // EXPECT: checked_add_chain(100,200,300)=600
    printf("checked_add_chain(100,200,300)=%d\n", checked_add_chain(100, 200, 300));

    // EXPECT: checked_add_chain(2147483647,1,1)=-1
    printf("checked_add_chain(2147483647,1,1)=%d\n", checked_add_chain(2147483647, 1, 1));

    // EXPECT: checked_mul_chain(10,20,30)=6000
    printf("checked_mul_chain(10,20,30)=%d\n", checked_mul_chain(10, 20, 30));

    // EXPECT: checked_mul_chain(100000,100000,2)=-1
    printf("checked_mul_chain(100000,100000,2)=%d\n", checked_mul_chain(100000, 100000, 2));

    // EXPECT: safe_square(46340)=2147395600 ok=1
    int ok10 = safe_square(46340, &r);
    printf("safe_square(46340)=%d ok=%d\n", r, ok10);

    // EXPECT: safe_square(46341) ok=0
    int ok11 = safe_square(46341, &r);
    printf("safe_square(46341) ok=%d\n", ok11);

    // EXPECT: safe_square(-100)=10000 ok=1
    int ok12 = safe_square(-100, &r);
    printf("safe_square(-100)=%d ok=%d\n", r, ok12);

    return 0;
}
