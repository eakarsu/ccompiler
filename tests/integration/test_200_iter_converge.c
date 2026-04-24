int printf(const char *fmt, ...);
// EXPECT: Iterative mean:\nmean[0..0]=10\nmean[0..1]=15\nmean[0..2]=20\nmean[0..3]=25\nmean[0..4]=30\nmean[0..5]=35\nmean[0..6]=40\nmean[0..7]=45\nBabylonian sqrt convergence (x100):\nstep 0: x=20000 (sqrt~200.00)\nstep 1: x=10050 (sqrt~100.50)\nstep 2: x=5124 (sqrt~51.24)\nstep 3: x=2757 (sqrt~27.57)\nstep 4: x=1741 (sqrt~17.41)\nstep 5: x=1444 (sqrt~14.44)\nstep 6: x=1414 (sqrt~14.14)\nFixed-point iteration:\nx=450 x=447 x=447 x=447 x=447 x=447 x=447 x=447 x=447 x=447 x=447 x=447 x=447 x=447 x=447 \nconverged to isqrt(200000)=447\nJacobi-like iteration:\na=33 b=100 c=133\na=111 b=122 c=144\na=122 b=151 c=177\na=142 b=166 c=191\na=152 b=177 c=202\na=159 b=184 c=209\na=164 b=189 c=214\na=167 b=192 c=217\na=169 b=194 c=219\na=171 b=196 c=221\nDamped iteration:\n717 528 402 318 262 225 200 184 173 166 161 158 156 154 153 \nGauss-Seidel 2x2 (x100):\nx1=150 x2=116 x1=92 x2=136 x1=82 x2=139 x1=80 x2=140 x1=80 x2=140 x1=80 x2=140 x1=80 x2=140 x1=80 x2=140 x1=80 x2=140 x1=80 x2=140 
int abs_val(int x) {
    return x >= 0 ? x : -x;
}

int isqrt(int n) {
    if (n <= 1) return n;
    int x = n / 2;
    while (1) {
        int nx = (x + n / x) / 2;
        if (nx >= x) break;
        x = nx;
    }
    return x;
}

int main(void) {
    int i;

    printf("Iterative mean:\n");
    int data[8];
    data[0] = 10; data[1] = 20; data[2] = 30; data[3] = 40;
    data[4] = 50; data[5] = 60; data[6] = 70; data[7] = 80;
    int running_mean = 0;
    for (i = 0; i < 8; i++) {
        running_mean = (running_mean * i + data[i]) / (i + 1);
        printf("mean[0..%d]=%d\n", i, running_mean);
    }

    printf("Babylonian sqrt convergence (x100):\n");
    int target = 200;
    int x = target * 100;
    int steps = 0;
    while (steps < 20) {
        int nx = (x + target * 10000 / x) / 2;
        printf("step %d: x=%d (sqrt~%d.%02d)\n", steps, x, x / 100, x % 100);
        if (abs_val(nx - x) <= 0) break;
        x = nx;
        steps++;
    }

    printf("Fixed-point iteration:\n");
    x = 500;
    for (i = 0; i < 15; i++) {
        x = (x + 200000 / x) / 2;
        printf("x=%d ", x);
    }
    printf("\nconverged to isqrt(200000)=%d\n", isqrt(200000));

    printf("Jacobi-like iteration:\n");
    int a = 100;
    int b = 0;
    int c = 0;
    for (i = 0; i < 10; i++) {
        int na = (b + c + 100) / 3;
        int nb = (a + c + 200) / 3;
        int nc = (a + b + 300) / 3;
        a = na;
        b = nb;
        c = nc;
        printf("a=%d b=%d c=%d\n", a, b, c);
    }

    printf("Damped iteration:\n");
    x = 1000;
    int target_val = 150;
    for (i = 0; i < 15; i++) {
        int error = target_val - x;
        x = x + error / 3;
        printf("%d ", x);
    }
    printf("\n");

    printf("Gauss-Seidel 2x2 (x100):\n");
    int x1 = 0;
    int x2 = 0;
    for (i = 0; i < 10; i++) {
        x1 = (300 - x2) / 2;
        x2 = (500 - x1) / 3;
        printf("x1=%d x2=%d ", x1, x2);
    }
    printf("\n");

    return 0;
}
