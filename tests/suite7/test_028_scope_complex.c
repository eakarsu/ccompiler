int printf(const char *fmt, ...);

void test_if_shadow(void) {
    int x = 0;
    if (1) {
        int x = 100;
        // EXPECT: if branch x = 100
        printf("if branch x = %d\n", x);
        x = x + 5;
        // EXPECT: if branch modified x = 105
        printf("if branch modified x = %d\n", x);
    }
    // EXPECT: after if x = 0
    printf("after if x = %d\n", x);
}

void test_else_shadow(void) {
    int x = 0;
    if (0) {
        int x = 100;
        printf("should not print\n");
    } else {
        int x = 200;
        // EXPECT: else branch x = 200
        printf("else branch x = %d\n", x);
        x = x + 7;
        // EXPECT: else branch modified x = 207
        printf("else branch modified x = %d\n", x);
    }
    // EXPECT: after else x = 0
    printf("after else x = %d\n", x);
}

void test_chain_case1(void) {
    int result = 0;
    int val = 1;
    if (val == 1) {
        int result = 10;
        // EXPECT: chain1 result = 10
        printf("chain1 result = %d\n", result);
    } else {
        int result = 99;
        printf("chain1 else = %d\n", result);
    }
    // EXPECT: after chain1 result = 0
    printf("after chain1 result = %d\n", result);
}

void test_chain_case2(void) {
    int result = 0;
    int val = 2;
    if (val == 1) {
        int result = 10;
        printf("chain2 if = %d\n", result);
    } else if (val == 2) {
        int result = 20;
        // EXPECT: chain2 result = 20
        printf("chain2 result = %d\n", result);
    }
    // EXPECT: after chain2 result = 0
    printf("after chain2 result = %d\n", result);
}

void test_chain_case3(void) {
    int result = 0;
    int val = 3;
    if (val == 1) {
        int result = 10;
        printf("chain3a = %d\n", result);
    } else if (val == 2) {
        int result = 20;
        printf("chain3b = %d\n", result);
    } else if (val == 3) {
        int result = 30;
        // EXPECT: chain3 result = 30
        printf("chain3 result = %d\n", result);
    }
    // EXPECT: after chain3 result = 0
    printf("after chain3 result = %d\n", result);
}

void test_chain_default(void) {
    int result = 0;
    int val = 0;
    if (val == 1) {
        int result = 10;
        printf("chaind1 = %d\n", result);
    } else {
        int result = 99;
        // EXPECT: chain_default result = 99
        printf("chain_default result = %d\n", result);
    }
    // EXPECT: after chain_default result = 0
    printf("after chain_default result = %d\n", result);
}

void test_mixed_scope(void) {
    int a = 1;
    int b = 2;
    {
        int a = 10;
        if (a > 5) {
            int b = 20;
            // EXPECT: mixed inner a=10 b=20
            printf("mixed inner a=%d b=%d\n", a, b);
        }
        // EXPECT: mixed mid a=10 b=2
        printf("mixed mid a=%d b=%d\n", a, b);
    }
    // EXPECT: mixed outer a=1 b=2
    printf("mixed outer a=%d b=%d\n", a, b);
}

void test_complex_nesting(void) {
    int x = 1;
    int y = 2;
    int z = 3;
    if (x) {
        int y = 20;
        if (y > 10) {
            int z = 300;
            // EXPECT: complex deep x=1 y=20 z=300
            printf("complex deep x=%d y=%d z=%d\n", x, y, z);
        }
        {
            int x = 10;
            // EXPECT: complex block x=10 y=20 z=3
            printf("complex block x=%d y=%d z=%d\n", x, y, z);
        }
        // EXPECT: complex mid x=1 y=20 z=3
        printf("complex mid x=%d y=%d z=%d\n", x, y, z);
    }
    // EXPECT: complex outer x=1 y=2 z=3
    printf("complex outer x=%d y=%d z=%d\n", x, y, z);
}

void test_loop_if_shadow(void) {
    int sum = 0;
    int x = 5;
    int i;
    for (i = 0; i < 4; i = i + 1) {
        int x = i;
        if (x % 2 == 0) {
            int x = 100;
            sum = sum + x;
        } else {
            int x = 1;
            sum = sum + x;
        }
    }
    // EXPECT: loop_if sum = 202
    printf("loop_if sum = %d\n", sum);
    // EXPECT: loop_if x = 5
    printf("loop_if x = %d\n", x);
}

void test_ternary_scope(void) {
    int x = 10;
    int r;
    {
        int x = 20;
        r = (x > 15) ? x : 0;
    }
    // EXPECT: ternary r = 20
    printf("ternary r = %d\n", r);
    // EXPECT: ternary x = 10
    printf("ternary x = %d\n", x);
}

int main(void) {
    test_if_shadow();
    test_else_shadow();
    test_chain_case1();
    test_chain_case2();
    test_chain_case3();
    test_chain_default();
    test_mixed_scope();
    test_complex_nesting();
    test_loop_if_shadow();
    test_ternary_scope();
    return 0;
}
