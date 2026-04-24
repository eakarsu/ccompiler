int printf(const char *fmt, ...);

void test_switch_case1(void) {
    int x = 50;
    // EXPECT: sw1 before x = 50
    printf("sw1 before x = %d\n", x);
    switch (1) {
        case 1: {
            int x = 100;
            // EXPECT: sw1 case1 x = 100
            printf("sw1 case1 x = %d\n", x);
            break;
        }
        case 2: {
            int x = 200;
            printf("sw1 case2 x = %d\n", x);
            break;
        }
    }
    // EXPECT: sw1 after x = 50
    printf("sw1 after x = %d\n", x);
}

void test_switch_case2(void) {
    int x = 60;
    // EXPECT: sw2 before x = 60
    printf("sw2 before x = %d\n", x);
    switch (2) {
        case 1: {
            int x = 100;
            printf("sw2 case1 x = %d\n", x);
            break;
        }
        case 2: {
            int x = 200;
            // EXPECT: sw2 case2 x = 200
            printf("sw2 case2 x = %d\n", x);
            break;
        }
    }
    // EXPECT: sw2 after x = 60
    printf("sw2 after x = %d\n", x);
}

void test_switch_default(void) {
    int x = 70;
    // EXPECT: sw3 before x = 70
    printf("sw3 before x = %d\n", x);
    switch (99) {
        case 1: {
            int x = 100;
            printf("sw3 case1 x = %d\n", x);
            break;
        }
        default: {
            int x = 999;
            // EXPECT: sw3 default x = 999
            printf("sw3 default x = %d\n", x);
            break;
        }
    }
    // EXPECT: sw3 after x = 70
    printf("sw3 after x = %d\n", x);
}

void test_switch_multi_var(void) {
    int a = 1;
    int b = 2;
    switch (0) {
        case 0: {
            int a = 10;
            int b = 20;
            // EXPECT: multi0 a=10 b=20
            printf("multi0 a=%d b=%d\n", a, b);
            break;
        }
        case 1: {
            int a = 30;
            int b = 40;
            printf("multi1 a=%d b=%d\n", a, b);
            break;
        }
    }
    // EXPECT: multi after a=1 b=2
    printf("multi after a=%d b=%d\n", a, b);
}

void test_nested_switch_11(void) {
    int val = 77;
    switch (1) {
        case 1: {
            int val = 100;
            switch (1) {
                case 1: {
                    int val = 111;
                    // EXPECT: nested 1-1 val = 111
                    printf("nested 1-1 val = %d\n", val);
                    break;
                }
            }
            // EXPECT: nest11 outer val = 100
            printf("nest11 outer val = %d\n", val);
            break;
        }
    }
    // EXPECT: nest11 original val = 77
    printf("nest11 original val = %d\n", val);
}

void test_nested_switch_12(void) {
    int val = 88;
    switch (1) {
        case 1: {
            int val = 100;
            switch (2) {
                case 2: {
                    int val = 222;
                    // EXPECT: nested 1-2 val = 222
                    printf("nested 1-2 val = %d\n", val);
                    break;
                }
            }
            // EXPECT: nest12 outer val = 100
            printf("nest12 outer val = %d\n", val);
            break;
        }
    }
    // EXPECT: nest12 original val = 88
    printf("nest12 original val = %d\n", val);
}

void test_switch_accum(void) {
    int sum = 0;
    int i;
    for (i = 0; i < 4; i = i + 1) {
        int x = 0;
        switch (i) {
            case 0: { int x = 10; sum = sum + x; break; }
            case 1: { int x = 20; sum = sum + x; break; }
            case 2: { int x = 30; sum = sum + x; break; }
            case 3: { int x = 40; sum = sum + x; break; }
        }
    }
    // EXPECT: switch accum sum = 100
    printf("switch accum sum = %d\n", sum);
}

void test_switch_shadow_var(void) {
    int y = 33;
    switch (2) {
        case 1: {
            int y = 11;
            printf("case1 y = %d\n", y);
            break;
        }
        case 2: {
            int y = 22;
            // EXPECT: case2 y = 22
            printf("case2 y = %d\n", y);
            break;
        }
    }
    // EXPECT: outer y = 33
    printf("outer y = %d\n", y);
}

int main(void) {
    test_switch_case1();
    test_switch_case2();
    test_switch_default();
    test_switch_multi_var();
    test_nested_switch_11();
    test_nested_switch_12();
    test_switch_accum();
    test_switch_shadow_var();

    int z = 5;
    switch (3) {
        case 3: {
            int z = 55;
            // EXPECT: main case3 z = 55
            printf("main case3 z = %d\n", z);
            break;
        }
    }
    // EXPECT: main z = 5
    printf("main z = %d\n", z);

    return 0;
}
