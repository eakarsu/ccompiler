int printf(const char *fmt, ...);

int classify(int a, int b) {
    int result = 0;
    switch (a) {
        case 1:
            switch (b) {
                case 10: result = 110; break;
                case 20: result = 120; break;
                default: result = 100; break;
            }
            break;
        case 2:
            switch (b) {
                case 10: result = 210; break;
                case 20: result = 220; break;
                default: result = 200; break;
            }
            break;
        case 3:
            result = 300;
            break;
        default:
            result = -1;
            break;
    }
    return result;
}

int switch_in_loop(int n) {
    int sum = 0;
    int i = 0;
    while (i < n) {
        switch (i) {
            case 0: sum = sum + 10; break;
            case 1: sum = sum + 20; break;
            case 2: sum = sum + 30; break;
            default: sum = sum + 1; break;
        }
        i = i + 1;
    }
    return sum;
}

int fallthrough_sum(int start) {
    int val = 0;
    switch (start) {
        case 1:
            val = val + 1;
        case 2:
            val = val + 2;
        case 3:
            val = val + 3;
            break;
        case 4:
            val = val + 4;
            break;
        default:
            val = -1;
            break;
    }
    return val;
}

int triple_nested(int x, int y, int z) {
    int r = 0;
    switch (x) {
        case 0:
            switch (y) {
                case 0:
                    switch (z) {
                        case 0: r = 1; break;
                        case 1: r = 2; break;
                        default: r = 3; break;
                    }
                    break;
                case 1: r = 4; break;
                default: r = 5; break;
            }
            break;
        case 1:
            r = 6;
            break;
        default:
            r = 7;
            break;
    }
    return r;
}

int switch_with_shared_cases(int v) {
    int out = 0;
    switch (v) {
        case 1:
        case 2:
        case 3:
            out = 10;
            break;
        case 4:
        case 5:
            out = 20;
            break;
        default:
            out = 30;
            break;
    }
    return out;
}

int main(void) {
    // EXPECT: classify(1,10)=110
    printf("classify(1,10)=%d\n", classify(1, 10));
    // EXPECT: classify(1,20)=120
    printf("classify(1,20)=%d\n", classify(1, 20));
    // EXPECT: classify(1,99)=100
    printf("classify(1,99)=%d\n", classify(1, 99));
    // EXPECT: classify(2,10)=210
    printf("classify(2,10)=%d\n", classify(2, 10));
    // EXPECT: classify(2,20)=220
    printf("classify(2,20)=%d\n", classify(2, 20));
    // EXPECT: classify(3,10)=300
    printf("classify(3,10)=%d\n", classify(3, 10));
    // EXPECT: classify(9,9)=-1
    printf("classify(9,9)=%d\n", classify(9, 9));
    // EXPECT: loop(5)=62
    printf("loop(5)=%d\n", switch_in_loop(5));
    // EXPECT: loop(3)=60
    printf("loop(3)=%d\n", switch_in_loop(3));
    // EXPECT: fall(1)=6
    printf("fall(1)=%d\n", fallthrough_sum(1));
    // EXPECT: fall(2)=5
    printf("fall(2)=%d\n", fallthrough_sum(2));
    // EXPECT: fall(3)=3
    printf("fall(3)=%d\n", fallthrough_sum(3));
    // EXPECT: fall(4)=4
    printf("fall(4)=%d\n", fallthrough_sum(4));
    // EXPECT: triple(0,0,0)=1
    printf("triple(0,0,0)=%d\n", triple_nested(0, 0, 0));
    // EXPECT: triple(0,0,1)=2
    printf("triple(0,0,1)=%d\n", triple_nested(0, 0, 1));
    // EXPECT: triple(0,1,0)=4
    printf("triple(0,1,0)=%d\n", triple_nested(0, 1, 0));
    // EXPECT: triple(1,0,0)=6
    printf("triple(1,0,0)=%d\n", triple_nested(1, 0, 0));
    // EXPECT: shared(2)=10
    printf("shared(2)=%d\n", switch_with_shared_cases(2));
    // EXPECT: shared(5)=20
    printf("shared(5)=%d\n", switch_with_shared_cases(5));
    // EXPECT: shared(9)=30
    printf("shared(9)=%d\n", switch_with_shared_cases(9));
    return 0;
}
