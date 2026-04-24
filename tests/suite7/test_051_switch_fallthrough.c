int printf(const char *fmt, ...);

int classify(int x) {
    int result = 0;
    switch (x) {
        case 1:
            result = result + 10;
        case 2:
            result = result + 20;
        case 3:
            result = result + 30;
            break;
        case 4:
            result = result + 40;
        case 5:
            result = result + 50;
            break;
        default:
            result = -1;
    }
    return result;
}

int default_first(int x) {
    int r = 0;
    switch (x) {
        default:
            r = 99;
            break;
        case 0:
            r = 0;
            break;
        case 1:
            r = 1;
            break;
    }
    return r;
}

int default_middle(int x) {
    int r = 0;
    switch (x) {
        case 0:
            r = 10;
            break;
        default:
            r = 50;
            break;
        case 1:
            r = 20;
            break;
        case 2:
            r = 30;
            break;
    }
    return r;
}

int full_fallthrough(int x) {
    int sum = 0;
    switch (x) {
        case 5:
            sum = sum + 5;
        case 4:
            sum = sum + 4;
        case 3:
            sum = sum + 3;
        case 2:
            sum = sum + 2;
        case 1:
            sum = sum + 1;
    }
    return sum;
}

int mixed_break_fall(int x) {
    int val = 0;
    switch (x) {
        case 1:
            val = 100;
            break;
        case 2:
            val = 200;
        case 3:
            val = 300;
            break;
        case 4:
            val = 400;
        case 5:
            val = 500;
        case 6:
            val = 600;
            break;
    }
    return val;
}

int accumulate_cases(int start) {
    int total = 0;
    int i = start;
    while (i <= 5) {
        switch (i) {
            case 1:
            case 2:
            case 3:
                total = total + i;
                break;
            case 4:
            case 5:
                total = total + i * 10;
                break;
            default:
                total = total - 1;
                break;
        }
        i = i + 1;
    }
    return total;
}

int main(void) {
    // EXPECT: fallthrough 1: 60
    printf("fallthrough 1: %d\n", classify(1));
    // EXPECT: fallthrough 2: 50
    printf("fallthrough 2: %d\n", classify(2));
    // EXPECT: fallthrough 3: 30
    printf("fallthrough 3: %d\n", classify(3));
    // EXPECT: fallthrough 4: 90
    printf("fallthrough 4: %d\n", classify(4));
    // EXPECT: fallthrough 5: 50
    printf("fallthrough 5: %d\n", classify(5));
    // EXPECT: fallthrough 99: -1
    printf("fallthrough 99: %d\n", classify(99));

    // EXPECT: default_first 0: 0
    printf("default_first 0: %d\n", default_first(0));
    // EXPECT: default_first 1: 1
    printf("default_first 1: %d\n", default_first(1));
    // EXPECT: default_first 7: 99
    printf("default_first 7: %d\n", default_first(7));

    // EXPECT: default_middle 0: 10
    printf("default_middle 0: %d\n", default_middle(0));
    // EXPECT: default_middle 1: 20
    printf("default_middle 1: %d\n", default_middle(1));
    // EXPECT: default_middle 9: 50
    printf("default_middle 9: %d\n", default_middle(9));

    // EXPECT: full_fall 5: 15
    printf("full_fall 5: %d\n", full_fallthrough(5));
    // EXPECT: full_fall 3: 6
    printf("full_fall 3: %d\n", full_fallthrough(3));
    // EXPECT: full_fall 1: 1
    printf("full_fall 1: %d\n", full_fallthrough(1));
    // EXPECT: full_fall 0: 0
    printf("full_fall 0: %d\n", full_fallthrough(0));

    // EXPECT: mixed 1: 100
    printf("mixed 1: %d\n", mixed_break_fall(1));
    // EXPECT: mixed 2: 300
    printf("mixed 2: %d\n", mixed_break_fall(2));
    // EXPECT: mixed 4: 600
    printf("mixed 4: %d\n", mixed_break_fall(4));
    // EXPECT: mixed 6: 600
    printf("mixed 6: %d\n", mixed_break_fall(6));

    // EXPECT: accum from 1: 96
    printf("accum from 1: %d\n", accumulate_cases(1));
    // EXPECT: accum from 3: 93
    printf("accum from 3: %d\n", accumulate_cases(3));

    return 0;
}
