int printf(const char *fmt, ...);

void test_for_shadow(void) {
    int x = 100;
    int i;
    // EXPECT: before for x = 100
    printf("before for x = %d\n", x);
    for (i = 0; i < 3; i = i + 1) {
        int x = i * 10;
        // EXPECT: for body x = 0
        // EXPECT: for body x = 10
        // EXPECT: for body x = 20
        printf("for body x = %d\n", x);
    }
    // EXPECT: after for x = 100
    printf("after for x = %d\n", x);
}

void test_while_shadow(void) {
    int val = 42;
    int count = 0;
    // EXPECT: before while val = 42
    printf("before while val = %d\n", val);
    while (count < 4) {
        int val = count + 200;
        // EXPECT: while body val = 200
        // EXPECT: while body val = 201
        // EXPECT: while body val = 202
        // EXPECT: while body val = 203
        printf("while body val = %d\n", val);
        count = count + 1;
    }
    // EXPECT: after while val = 42
    printf("after while val = %d\n", val);
}

void test_dowhile_shadow(void) {
    int n = 999;
    int iter = 0;
    // EXPECT: before dowhile n = 999
    printf("before dowhile n = %d\n", n);
    do {
        int n = iter + 500;
        // EXPECT: dowhile body n = 500
        // EXPECT: dowhile body n = 501
        printf("dowhile body n = %d\n", n);
        iter = iter + 1;
    } while (iter < 2);
    // EXPECT: after dowhile n = 999
    printf("after dowhile n = %d\n", n);
}

void test_nested_loop_shadow(void) {
    int x = 7;
    int i;
    int j;
    // EXPECT: nested x = 0
    // EXPECT: nested x = 1
    // EXPECT: mid loop x = 50
    // EXPECT: nested x = 10
    // EXPECT: nested x = 11
    // EXPECT: mid loop x = 51
    for (i = 0; i < 2; i = i + 1) {
        int x = i + 50;
        for (j = 0; j < 2; j = j + 1) {
            int x = i * 10 + j;
            printf("nested x = %d\n", x);
        }
        printf("mid loop x = %d\n", x);
    }
    // EXPECT: outer x = 7
    printf("outer x = %d\n", x);
}

void test_loop_accum_shadow(void) {
    int total = 0;
    int x = 5;
    int i;
    for (i = 0; i < 5; i = i + 1) {
        int x = i * 3;
        total = total + x;
    }
    // EXPECT: total = 30
    printf("total = %d\n", total);
    // EXPECT: x unchanged = 5
    printf("x unchanged = %d\n", x);
}

int main(void) {
    test_for_shadow();
    test_while_shadow();
    test_dowhile_shadow();
    test_nested_loop_shadow();
    test_loop_accum_shadow();

    int a = 11;
    int i;
    for (i = 0; i < 1; i = i + 1) {
        int a = 22;
        // EXPECT: main loop a = 22
        printf("main loop a = %d\n", a);
    }
    // EXPECT: main a = 11
    printf("main a = %d\n", a);

    return 0;
}
