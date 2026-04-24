// Scope hell: deep shadowing, for-loop shadow, block scope, goto, static recursion
int printf(const char *fmt, ...);

void test_eight_level_shadow(void) {
    int x = 1;
    printf("L0 x=%d\n", x);
    // EXPECT: L0 x=1
    {
        int x = 2;
        printf("L1 x=%d\n", x);
        // EXPECT: L1 x=2
        {
            int x = 3;
            printf("L2 x=%d\n", x);
            // EXPECT: L2 x=3
            {
                int x = 4;
                printf("L3 x=%d\n", x);
                // EXPECT: L3 x=4
                {
                    int x = 5;
                    printf("L4 x=%d\n", x);
                    // EXPECT: L4 x=5
                    {
                        int x = 6;
                        printf("L5 x=%d\n", x);
                        // EXPECT: L5 x=6
                        {
                            int x = 7;
                            printf("L6 x=%d\n", x);
                            // EXPECT: L6 x=7
                            {
                                int x = 8;
                                printf("L7 x=%d\n", x);
                                // EXPECT: L7 x=8
                            }
                            printf("back L6 x=%d\n", x);
                            // EXPECT: back L6 x=7
                        }
                        printf("back L5 x=%d\n", x);
                        // EXPECT: back L5 x=6
                    }
                    printf("back L4 x=%d\n", x);
                    // EXPECT: back L4 x=5
                }
                printf("back L3 x=%d\n", x);
                // EXPECT: back L3 x=4
            }
            printf("back L2 x=%d\n", x);
            // EXPECT: back L2 x=3
        }
        printf("back L1 x=%d\n", x);
        // EXPECT: back L1 x=2
    }
    printf("back L0 x=%d\n", x);
    // EXPECT: back L0 x=1
}

void test_for_loop_shadow(void) {
    int i = 999;
    int j = 888;
    int k;
    for (k = 0; k < 3; k++) {
        int i = k * 10;
        printf("inner i=%d\n", i);
    }
    printf("outer i=%d j=%d\n", i, j);
    // EXPECT: inner i=0
    // EXPECT: inner i=10
    // EXPECT: inner i=20
    // EXPECT: outer i=999 j=888
}

void test_block_create_destroy(void) {
    int a = 1;
    {
        int b = 2;
        {
            int c = 3;
            printf("all: %d %d %d\n", a, b, c);
            // EXPECT: all: 1 2 3
        }
        printf("ab: %d %d\n", a, b);
        // EXPECT: ab: 1 2
    }
    printf("a: %d\n", a);
    // EXPECT: a: 1
}

void test_shadow_different_types(void) {
    int x = 42;
    printf("int x=%d\n", x);
    // EXPECT: int x=42
    {
        char x = 'A';
        printf("char x=%c (%d)\n", x, x);
        // EXPECT: char x=A (65)
        {
            int *p;
            int val = 100;
            p = &val;
            int x = *p;
            printf("ptr x=%d\n", x);
            // EXPECT: ptr x=100
        }
    }
    printf("back int x=%d\n", x);
    // EXPECT: back int x=42
}

void test_goto_over_decl(void) {
    int x = 1;
    goto skip;
    {
        int y = 999;
        printf("should not print %d\n", y);
    }
skip:
    printf("after goto x=%d\n", x);
    // EXPECT: after goto x=1
}

void test_goto_backward(void) {
    int count = 0;
again:
    count++;
    if (count < 5) goto again;
    printf("goto count=%d\n", count);
    // EXPECT: goto count=5
}

int recursive_static(int n) {
    static int total = 0;
    total = total + n;
    if (n <= 1) {
        int result = total;
        total = 0;
        return result;
    }
    return recursive_static(n - 1);
}

void test_static_recursive(void) {
    printf("rec(5)=%d\n", recursive_static(5));
    // EXPECT: rec(5)=15
    printf("rec(3)=%d\n", recursive_static(3));
    // EXPECT: rec(3)=6
    printf("rec(10)=%d\n", recursive_static(10));
    // EXPECT: rec(10)=55
}

void test_shadow_in_switch(void) {
    int x = 100;
    int val = 2;
    switch (val) {
        case 1: {
            int x = 10;
            printf("case1 x=%d\n", x);
            break;
        }
        case 2: {
            int x = 20;
            printf("case2 x=%d\n", x);
            // EXPECT: case2 x=20
            break;
        }
        case 3: {
            int x = 30;
            printf("case3 x=%d\n", x);
            break;
        }
    }
    printf("after switch x=%d\n", x);
    // EXPECT: after switch x=100
}

void test_shadow_in_while(void) {
    int x = 0;
    int i = 0;
    while (i < 3) {
        int x = i * 100;
        printf("while x=%d\n", x);
        i++;
    }
    printf("after while x=%d\n", x);
    // EXPECT: while x=0
    // EXPECT: while x=100
    // EXPECT: while x=200
    // EXPECT: after while x=0
}

void test_nested_for_shadow(void) {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        int sum = 0;
        for (j = 0; j <= i; j++) {
            int sum = j;
            if (j == i) printf("inner sum=%d\n", sum);
        }
        sum = i * 10;
        printf("outer sum=%d\n", sum);
    }
    // EXPECT: inner sum=0
    // EXPECT: outer sum=0
    // EXPECT: inner sum=1
    // EXPECT: outer sum=10
    // EXPECT: inner sum=2
    // EXPECT: outer sum=20
}

void test_shadow_param(void) {
    int n = 42;
    {
        int n = 100;
        printf("shadow param=%d\n", n);
        // EXPECT: shadow param=100
    }
    printf("orig param=%d\n", n);
    // EXPECT: orig param=42
}

int main(void) {
    test_eight_level_shadow();
    test_for_loop_shadow();
    test_block_create_destroy();
    test_shadow_different_types();
    test_goto_over_decl();
    test_goto_backward();
    test_static_recursive();
    test_shadow_in_switch();
    test_shadow_in_while();
    test_nested_for_shadow();
    test_shadow_param();
    return 0;
}
