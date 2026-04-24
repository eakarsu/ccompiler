int printf(const char *fmt, ...);
// EXPECT: Safe divide:\n10/3=3\n10/0=-1\n100/5=20\n7/0=-1\nMap values:\nmap(0)=0\nmap(1)=2\nmap(2)=4\nmap(3)=6\nmap(4)=8\nmap(5)=10\nmap(6)=12\nmap(7)=14\nClamp tests:\nclamp(-5,0,10)=0\nclamp(0,0,10)=0\nclamp(3,0,10)=3\nclamp(5,0,10)=5\nclamp(7,0,10)=7\nclamp(10,0,10)=10\nclamp(15,0,10)=10\nError codes:\nerr(0)=-999\nerr(1)=-999\nerr(2)=-999\nerr(3)=-999\nerr(4)=-999\nPriority:\npri(0)=100\npri(1)=49\npri(2)=48\npri(3)=47\npri(4)=46\npri(5)=45\nDefault-only switch in loop:\nsum=55
int safe_divide(int a, int b) {
    switch (b) {
        default:
            return a / b;
        case 0:
            return -1;
    }
}

int map_value(int x) {
    switch (x) {
        default:
            return x * 2;
    }
}

int clamp(int x, int lo, int hi) {
    int result;
    if (x < lo) {
        result = lo;
    } else if (x > hi) {
        result = hi;
    } else {
        result = x;
    }
    switch (result) {
        default:
            return result;
    }
}

int error_code(int code) {
    switch (code) {
        default:
            return -999;
    }
}

int priority(int level) {
    switch (level) {
        case 0:
            return 100;
        default:
            return 50 - level;
    }
}

int main(void) {
    int i;
    printf("Safe divide:\n");
    printf("10/3=%d\n", safe_divide(10, 3));
    printf("10/0=%d\n", safe_divide(10, 0));
    printf("100/5=%d\n", safe_divide(100, 5));
    printf("7/0=%d\n", safe_divide(7, 0));

    printf("Map values:\n");
    for (i = 0; i < 8; i++) {
        printf("map(%d)=%d\n", i, map_value(i));
    }

    printf("Clamp tests:\n");
    int vals[7];
    vals[0] = -5;
    vals[1] = 0;
    vals[2] = 3;
    vals[3] = 5;
    vals[4] = 7;
    vals[5] = 10;
    vals[6] = 15;
    for (i = 0; i < 7; i++) {
        printf("clamp(%d,0,10)=%d\n", vals[i], clamp(vals[i], 0, 10));
    }

    printf("Error codes:\n");
    for (i = 0; i < 5; i++) {
        printf("err(%d)=%d\n", i, error_code(i));
    }

    printf("Priority:\n");
    for (i = 0; i < 6; i++) {
        printf("pri(%d)=%d\n", i, priority(i));
    }

    printf("Default-only switch in loop:\n");
    int sum = 0;
    for (i = 1; i <= 10; i++) {
        switch (i) {
            default:
                sum = sum + i;
                break;
        }
    }
    printf("sum=%d\n", sum);

    return 0;
}
