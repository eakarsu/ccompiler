int printf(const char *fmt, ...);

int add_ten(int x) {
    // EXPECT: param x = 5
    printf("param x = %d\n", x);
    {
        int x = 99;
        // EXPECT: inner x = 99
        printf("inner x = %d\n", x);
        x = x + 1;
        // EXPECT: inner x after inc = 100
        printf("inner x after inc = %d\n", x);
    }
    // EXPECT: param x restored = 5
    printf("param x restored = %d\n", x);
    // EXPECT: add_ten result = 15
    return x + 10;
}

int shadow_twice(int a, int b) {
    // EXPECT: params a=10 b=20
    printf("params a=%d b=%d\n", a, b);
    {
        int a = 77;
        int b = 88;
        // EXPECT: shadow a=77 b=88
        printf("shadow a=%d b=%d\n", a, b);
        {
            int a = 111;
            // EXPECT: deep shadow a=111 b=88
            printf("deep shadow a=%d b=%d\n", a, b);
        }
        // EXPECT: mid a=77 b=88
        printf("mid a=%d b=%d\n", a, b);
    }
    // EXPECT: restored a=10 b=20
    printf("restored a=%d b=%d\n", a, b);
    // EXPECT: shadow_twice result = 30
    return a + b;
}

int modify_shadow(int val) {
    int result = val;
    int saved = val * 2;
    {
        int val = saved;
        result = result + val;
        // EXPECT: inner val = 14
        printf("inner val = %d\n", val);
    }
    // EXPECT: outer val = 7
    printf("outer val = %d\n", val);
    // EXPECT: modify_shadow result = 21
    return result;
}

int triple_param(int n) {
    // EXPECT: triple param n = 3
    printf("triple param n = %d\n", n);
    {
        int n = 30;
        // EXPECT: first shadow n = 30
        printf("first shadow n = %d\n", n);
        {
            int n = 300;
            // EXPECT: second shadow n = 300
            printf("second shadow n = %d\n", n);
        }
        // EXPECT: back to first shadow n = 30
        printf("back to first shadow n = %d\n", n);
    }
    // EXPECT: back to param n = 3
    printf("back to param n = %d\n", n);
    // EXPECT: triple_param result = 3
    return n;
}

int main(void) {
    int r1 = add_ten(5);
    printf("add_ten result = %d\n", r1);

    int r2 = shadow_twice(10, 20);
    printf("shadow_twice result = %d\n", r2);

    int r3 = modify_shadow(7);
    printf("modify_shadow result = %d\n", r3);

    int r4 = triple_param(3);
    printf("triple_param result = %d\n", r4);

    int x = 42;
    {
        int x = 1000;
        // EXPECT: main inner x = 1000
        printf("main inner x = %d\n", x);
    }
    // EXPECT: main outer x = 42
    printf("main outer x = %d\n", x);

    int y = 55;
    {
        int y = 66;
        {
            int y = 77;
            // EXPECT: deep y = 77
            printf("deep y = %d\n", y);
        }
        // EXPECT: mid y = 66
        printf("mid y = %d\n", y);
    }
    // EXPECT: outer y = 55
    printf("outer y = %d\n", y);

    return 0;
}
