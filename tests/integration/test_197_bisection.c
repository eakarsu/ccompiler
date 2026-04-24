int printf(const char *fmt, ...);
// EXPECT: Bisection for x^2=25:\nroot=5 check=0\nBisection for x^3-2x-5=0:\nroot=2 check=-1\nBisection for 3x-10=0:\nroot=3 check=-1\nManual bisection trace (x^2=50):\nlo=0 hi=50 mid=25 mid^2=625\nlo=0 hi=24 mid=12 mid^2=144\nlo=0 hi=11 mid=5 mid^2=25\nlo=6 hi=11 mid=8 mid^2=64\nlo=6 hi=7 mid=6 mid^2=36\nlo=7 hi=7 mid=7 mid^2=49\napprox sqrt(50)=7\nFind threshold:\nfirst i where i^2>=100: i=10 val=100\nBisection counts:\nsize=10 steps=3\nsize=100 steps=6\nsize=1000 steps=9\nsize=10000 steps=13\nsize=100000 steps=16
int func1(int x) {
    return x * x - 25;
}

int func2(int x) {
    return x * x * x - 2 * x - 5;
}

int func3(int x) {
    return 3 * x - 10;
}

int bisect(int lo, int hi, int (*f)(int)) {
    int i;
    for (i = 0; i < 50; i++) {
        if (lo >= hi - 1) break;
        int mid = lo + (hi - lo) / 2;
        int fmid = f(mid);
        int flo = f(lo);
        if (fmid == 0) return mid;
        if ((flo < 0 && fmid < 0) || (flo > 0 && fmid > 0)) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    return lo;
}

int main(void) {
    int i;

    printf("Bisection for x^2=25:\n");
    int r = bisect(0, 10, func1);
    printf("root=%d check=%d\n", r, func1(r));

    printf("Bisection for x^3-2x-5=0:\n");
    r = bisect(1, 10, func2);
    printf("root=%d check=%d\n", r, func2(r));

    printf("Bisection for 3x-10=0:\n");
    r = bisect(0, 10, func3);
    printf("root=%d check=%d\n", r, func3(r));

    printf("Manual bisection trace (x^2=50):\n");
    int lo = 0;
    int hi = 50;
    for (i = 0; i < 20; i++) {
        int mid = (lo + hi) / 2;
        int val = mid * mid;
        printf("lo=%d hi=%d mid=%d mid^2=%d\n", lo, hi, mid, val);
        if (val == 50) break;
        if (val < 50) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
        if (lo > hi) break;
    }
    printf("approx sqrt(50)=%d\n", lo > hi ? hi : lo);

    printf("Find threshold:\n");
    int data[16];
    for (i = 0; i < 16; i++) {
        data[i] = i * i;
    }
    int target = 100;
    lo = 0;
    hi = 15;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (data[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    printf("first i where i^2>=%d: i=%d val=%d\n", target, lo, data[lo]);

    printf("Bisection counts:\n");
    int sizes[5];
    sizes[0] = 10; sizes[1] = 100; sizes[2] = 1000;
    sizes[3] = 10000; sizes[4] = 100000;
    for (i = 0; i < 5; i++) {
        int steps = 0;
        lo = 0;
        hi = sizes[i];
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            lo = mid + 1;
            steps++;
        }
        printf("size=%d steps=%d\n", sizes[i], steps);
    }

    return 0;
}
