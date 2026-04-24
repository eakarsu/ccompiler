int printf(const char *fmt, ...);
// EXPECT: Multi-condition exit:\nx=1 y=100\nx=4 y=93\nx=7 y=86\nx=10 y=79\nx=13 y=72\nx=16 y=65\nx=19 y=58\nx=22 y=51\nx=25 y=44\nx=28 y=37\nended: x=31 y=30 count=10\nConvergence loop:\nfound: lo=376 hi=377 steps=10\nOscillation check:\nval=67 val=45 \niters=2\nTwo sequence meeting:\ns1=255 s2=135 iters=7\nGCD chain: 12 12 12 12 12 
int main(void) {
    int i;
    int j;

    printf("Multi-condition exit:\n");
    int x = 1;
    int y = 100;
    int count = 0;
    while (x < y && count < 20 && x * y > 50) {
        printf("x=%d y=%d\n", x, y);
        x = x + 3;
        y = y - 7;
        count++;
    }
    printf("ended: x=%d y=%d count=%d\n", x, y, count);

    printf("Convergence loop:\n");
    int lo = 0;
    int hi = 1000;
    int target = 377;
    count = 0;
    while (hi - lo > 1) {
        int mid = (lo + hi) / 2;
        if (mid < target) {
            lo = mid;
        } else {
            hi = mid;
        }
        count++;
    }
    printf("found: lo=%d hi=%d steps=%d\n", lo, hi, count);

    printf("Oscillation check:\n");
    int val = 100;
    int prev = 0;
    int iters = 0;
    while (iters < 30) {
        int next_val;
        if (val > 50) {
            next_val = val - val / 3;
        } else {
            next_val = val + val / 2;
        }
        if (next_val == val || next_val == prev) {
            break;
        }
        prev = val;
        val = next_val;
        iters++;
        printf("val=%d ", val);
    }
    printf("\niters=%d\n", iters);

    printf("Two sequence meeting:\n");
    int s1 = 1;
    int s2 = 1000;
    iters = 0;
    while (s1 < s2) {
        s1 = s1 * 2 + 1;
        s2 = s2 - s2 / 4;
        iters++;
    }
    printf("s1=%d s2=%d iters=%d\n", s1, s2, iters);

    printf("GCD chain: ");
    int arr[6];
    arr[0] = 120; arr[1] = 84; arr[2] = 36; arr[3] = 60; arr[4] = 48; arr[5] = 72;
    int result = arr[0];
    for (i = 1; i < 6; i++) {
        int a = result;
        int b = arr[i];
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        result = a;
        printf("%d ", result);
    }
    printf("\n");

    return 0;
}
