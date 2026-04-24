int printf(const char *fmt, ...);

int nested_switch(int a, int b) {
    int result = 0;
    switch (a) {
        case 1:
            switch (b) {
                case 10:
                    result = 110;
                    break;
                case 20:
                    result = 120;
                    break;
                default:
                    result = 100;
                    break;
            }
            break;
        case 2:
            switch (b) {
                case 10:
                    result = 210;
                    break;
                case 20:
                    result = 220;
                    break;
                default:
                    result = 200;
                    break;
            }
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
        switch (i % 3) {
            case 0:
                sum = sum + 1;
                break;
            case 1:
                sum = sum + 10;
                break;
            case 2:
                sum = sum + 100;
                break;
        }
        i = i + 1;
    }
    return sum;
}

int loop_in_switch(int mode, int n) {
    int result = 0;
    switch (mode) {
        case 1: {
            int i = 0;
            while (i < n) {
                result = result + i;
                i = i + 1;
            }
            break;
        }
        case 2: {
            int i = 0;
            while (i < n) {
                result = result + i * i;
                i = i + 1;
            }
            break;
        }
        case 3: {
            int i = 1;
            result = 1;
            while (i <= n) {
                result = result * i;
                i = i + 1;
            }
            break;
        }
        default:
            result = -1;
            break;
    }
    return result;
}

int triple_nested(int x, int y, int z) {
    int r = 0;
    switch (x) {
        case 0:
            switch (y) {
                case 0:
                    switch (z) {
                        case 0: r = 0; break;
                        case 1: r = 1; break;
                        default: r = 2; break;
                    }
                    break;
                case 1:
                    r = 10;
                    break;
            }
            break;
        case 1:
            r = 100;
            break;
    }
    return r;
}

int switch_loop_switch(int n) {
    int total = 0;
    int i = 0;
    switch (n) {
        case 1:
            while (i < 5) {
                switch (i) {
                    case 0: total = total + 1; break;
                    case 1: total = total + 2; break;
                    case 2: total = total + 4; break;
                    case 3: total = total + 8; break;
                    case 4: total = total + 16; break;
                }
                i = i + 1;
            }
            break;
        case 2:
            total = 999;
            break;
    }
    return total;
}

int main(void) {
    // EXPECT: nested 1,10: 110
    printf("nested 1,10: %d\n", nested_switch(1, 10));
    // EXPECT: nested 1,20: 120
    printf("nested 1,20: %d\n", nested_switch(1, 20));
    // EXPECT: nested 1,99: 100
    printf("nested 1,99: %d\n", nested_switch(1, 99));
    // EXPECT: nested 2,10: 210
    printf("nested 2,10: %d\n", nested_switch(2, 10));
    // EXPECT: nested 2,20: 220
    printf("nested 2,20: %d\n", nested_switch(2, 20));
    // EXPECT: nested 9,9: -1
    printf("nested 9,9: %d\n", nested_switch(9, 9));

    // EXPECT: swloop 6: 222
    printf("swloop 6: %d\n", switch_in_loop(6));
    // EXPECT: swloop 3: 111
    printf("swloop 3: %d\n", switch_in_loop(3));
    // EXPECT: swloop 1: 1
    printf("swloop 1: %d\n", switch_in_loop(1));

    // EXPECT: loopsw 1,5: 10
    printf("loopsw 1,5: %d\n", loop_in_switch(1, 5));
    // EXPECT: loopsw 2,4: 14
    printf("loopsw 2,4: %d\n", loop_in_switch(2, 4));
    // EXPECT: loopsw 3,5: 120
    printf("loopsw 3,5: %d\n", loop_in_switch(3, 5));
    // EXPECT: loopsw 9,5: -1
    printf("loopsw 9,5: %d\n", loop_in_switch(9, 5));

    // EXPECT: triple 0,0,0: 0
    printf("triple 0,0,0: %d\n", triple_nested(0, 0, 0));
    // EXPECT: triple 0,0,1: 1
    printf("triple 0,0,1: %d\n", triple_nested(0, 0, 1));
    // EXPECT: triple 0,1,0: 10
    printf("triple 0,1,0: %d\n", triple_nested(0, 1, 0));
    // EXPECT: triple 1,0,0: 100
    printf("triple 1,0,0: %d\n", triple_nested(1, 0, 0));

    // EXPECT: sls 1: 31
    printf("sls 1: %d\n", switch_loop_switch(1));
    // EXPECT: sls 2: 999
    printf("sls 2: %d\n", switch_loop_switch(2));

    return 0;
}
