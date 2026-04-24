int printf(const char *fmt, ...);
// EXPECT: Clamp tests:\nclamp(-10,0,100)=0\nclamp(0,0,100)=0\nclamp(5,0,100)=5\nclamp(10,0,100)=10\nclamp(50,0,100)=50\nclamp(100,0,100)=100\nclamp(150,0,100)=100\nclamp(200,0,100)=100\nclamp(255,0,100)=100\nWrap tests:\nwrap(-3,0,9)=7\nwrap(0,0,9)=0\nwrap(5,0,9)=5\nwrap(10,0,9)=0\nwrap(15,0,9)=5\nwrap(20,0,9)=0\nwrap(25,0,9)=5\nOff-by-one tests:\nfirst=0 last=90\ncount=10\ncount_incl=10\ncount_excl_first=9\nBoundary conditions:\n0==0: 1\n-1<0: 1\n1>0: 1\n0>=0: 1\n0<=0: 1\nDivision edge cases:\n7/2=3\n-7/2=-3\n1/3=0\n0/5=0\nModulo edge cases:\n7%2=1\n-7%2=-1\n10%10=0\n0%5=0
int clamp(int x, int lo, int hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

int wrap(int x, int lo, int hi) {
    int range = hi - lo + 1;
    while (x < lo) x = x + range;
    while (x > hi) x = x - range;
    return x;
}

int saturating_add(int a, int b) {
    int result = a + b;
    if (a > 0 && b > 0 && result < 0) return 2147483647;
    if (a < 0 && b < 0 && result > 0) return -2147483647;
    return result;
}

int main(void) {
    int i;
    printf("Clamp tests:\n");
    int vals[9];
    vals[0] = -10; vals[1] = 0; vals[2] = 5;
    vals[3] = 10; vals[4] = 50; vals[5] = 100;
    vals[6] = 150; vals[7] = 200; vals[8] = 255;
    for (i = 0; i < 9; i++) {
        printf("clamp(%d,0,100)=%d\n", vals[i], clamp(vals[i], 0, 100));
    }

    printf("Wrap tests:\n");
    int wvals[7];
    wvals[0] = -3; wvals[1] = 0; wvals[2] = 5;
    wvals[3] = 10; wvals[4] = 15; wvals[5] = 20;
    wvals[6] = 25;
    for (i = 0; i < 7; i++) {
        printf("wrap(%d,0,9)=%d\n", wvals[i], wrap(wvals[i], 0, 9));
    }

    printf("Off-by-one tests:\n");
    int arr[10];
    for (i = 0; i < 10; i++) arr[i] = i * 10;
    printf("first=%d last=%d\n", arr[0], arr[9]);
    int count = 0;
    for (i = 0; i < 10; i++) count++;
    printf("count=%d\n", count);
    count = 0;
    for (i = 0; i <= 9; i++) count++;
    printf("count_incl=%d\n", count);
    count = 0;
    for (i = 1; i < 10; i++) count++;
    printf("count_excl_first=%d\n", count);

    printf("Boundary conditions:\n");
    printf("0==0: %d\n", 0 == 0);
    printf("-1<0: %d\n", -1 < 0);
    printf("1>0: %d\n", 1 > 0);
    printf("0>=0: %d\n", 0 >= 0);
    printf("0<=0: %d\n", 0 <= 0);

    printf("Division edge cases:\n");
    printf("7/2=%d\n", 7 / 2);
    printf("-7/2=%d\n", -7 / 2);
    printf("1/3=%d\n", 1 / 3);
    printf("0/5=%d\n", 0 / 5);

    printf("Modulo edge cases:\n");
    printf("7%%2=%d\n", 7 % 2);
    printf("-7%%2=%d\n", -7 % 2);
    printf("10%%10=%d\n", 10 % 10);
    printf("0%%5=%d\n", 0 % 5);

    return 0;
}
