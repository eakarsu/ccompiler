int printf(const char *fmt, ...);
// EXPECT: Integer square roots:\nisqrt(0)=0 verify=0\nisqrt(1)=1 verify=1\nisqrt(2)=1 verify=1\nisqrt(4)=2 verify=4\nisqrt(9)=3 verify=9\nisqrt(10)=3 verify=9\nisqrt(16)=4 verify=16\nisqrt(25)=5 verify=25\nisqrt(50)=7 verify=49\nisqrt(100)=10 verify=100\nisqrt(144)=12 verify=144\nisqrt(1000)=31 verify=961\nInteger cube roots:\nicbrt(0)=0 verify=0\nicbrt(1)=1 verify=1\nicbrt(8)=2 verify=8\nicbrt(27)=3 verify=27\nicbrt(64)=4 verify=64\nicbrt(125)=5 verify=125\nicbrt(100)=4 verify=64\nicbrt(1000)=10 verify=1000\nPerfect squares up to 200: 1 4 9 16 25 36 49 64 81 100 121 144 169 196 \nConvergence speed:\nstep 0: x=10000\nstep 1: x=5000\nstep 2: x=2501\nstep 3: x=1252\nstep 4: x=629\nstep 5: x=322\nstep 6: x=176\nstep 7: x=116\nstep 8: x=101\nstep 9: x=100\nconverged in 9 steps to 100
int isqrt_newton(int n) {
    if (n <= 1) return n;
    int x = n;
    int y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

int icbrt(int n) {
    if (n <= 0) return 0;
    int x = n;
    while (x > 0 && x > n / x / x + 1) {
        x = (2 * x + n / x / x) / 3;
        if (x <= 0) x = 1;
    }
    while (x * x * x > n) x--;
    while ((x + 1) * (x + 1) * (x + 1) <= n) x++;
    return x;
}

int reciprocal_scaled(int n, int scale) {
    if (n == 0) return -1;
    int x = scale / n;
    int i;
    for (i = 0; i < 10; i++) {
        int fx = x * n - scale;
        int dfx = n;
        if (dfx == 0) break;
        int correction = fx / dfx;
        if (correction == 0) break;
        x = x - correction;
    }
    return x;
}

int main(void) {
    int i;
    printf("Integer square roots:\n");
    int sqvals[12];
    sqvals[0] = 0; sqvals[1] = 1; sqvals[2] = 2; sqvals[3] = 4;
    sqvals[4] = 9; sqvals[5] = 10; sqvals[6] = 16; sqvals[7] = 25;
    sqvals[8] = 50; sqvals[9] = 100; sqvals[10] = 144; sqvals[11] = 1000;
    for (i = 0; i < 12; i++) {
        int s = isqrt_newton(sqvals[i]);
        printf("isqrt(%d)=%d verify=%d\n", sqvals[i], s, s * s);
    }

    printf("Integer cube roots:\n");
    int cbvals[8];
    cbvals[0] = 0; cbvals[1] = 1; cbvals[2] = 8; cbvals[3] = 27;
    cbvals[4] = 64; cbvals[5] = 125; cbvals[6] = 100; cbvals[7] = 1000;
    for (i = 0; i < 8; i++) {
        int c = icbrt(cbvals[i]);
        printf("icbrt(%d)=%d verify=%d\n", cbvals[i], c, c * c * c);
    }

    printf("Perfect squares up to 200: ");
    for (i = 1; i <= 200; i++) {
        int s = isqrt_newton(i);
        if (s * s == i) {
            printf("%d ", i);
        }
    }
    printf("\n");

    printf("Convergence speed:\n");
    int n = 10000;
    int x = n;
    int steps = 0;
    while (1) {
        int y = (x + n / x) / 2;
        printf("step %d: x=%d\n", steps, x);
        if (y >= x) break;
        x = y;
        steps++;
    }
    printf("converged in %d steps to %d\n", steps, x);

    return 0;
}
