int printf(const char *fmt, ...);

int check_levels(int start) {
    int x = start;
    // EXPECT: level0 x = 1
    printf("level0 x = %d\n", x);
    {
        int x = start + 10;
        // EXPECT: level1 x = 11
        printf("level1 x = %d\n", x);
        {
            int x = start + 100;
            // EXPECT: level2 x = 101
            printf("level2 x = %d\n", x);
            {
                int x = start + 1000;
                // EXPECT: level3 x = 1001
                printf("level3 x = %d\n", x);
                {
                    int x = start + 10000;
                    // EXPECT: level4 x = 10001
                    printf("level4 x = %d\n", x);
                }
                // EXPECT: back3 x = 1001
                printf("back3 x = %d\n", x);
            }
            // EXPECT: back2 x = 101
            printf("back2 x = %d\n", x);
        }
        // EXPECT: back1 x = 11
        printf("back1 x = %d\n", x);
    }
    // EXPECT: back0 x = 1
    printf("back0 x = %d\n", x);
    // EXPECT: check_levels returned 1
    return x;
}

int nested_multi_var(void) {
    int a = 1;
    int b = 2;
    // EXPECT: outer a=1 b=2
    printf("outer a=%d b=%d\n", a, b);
    {
        int a = 10;
        int b = 20;
        // EXPECT: mid a=10 b=20
        printf("mid a=%d b=%d\n", a, b);
        {
            int a = 100;
            int b = 200;
            // EXPECT: inner a=100 b=200
            printf("inner a=%d b=%d\n", a, b);
            {
                int a = 1000;
                int b = 2000;
                // EXPECT: deep a=1000 b=2000
                printf("deep a=%d b=%d\n", a, b);
            }
            // EXPECT: post-deep a=100 b=200
            printf("post-deep a=%d b=%d\n", a, b);
        }
        // EXPECT: post-inner a=10 b=20
        printf("post-inner a=%d b=%d\n", a, b);
    }
    // EXPECT: post-all a=1 b=2
    printf("post-all a=%d b=%d\n", a, b);
    // EXPECT: nested_multi_var returned 3
    return a + b;
}

int accumulate_shadows(void) {
    int sum = 0;
    int x = 1;
    sum = sum + x;
    {
        int x = 2;
        sum = sum + x;
        {
            int x = 4;
            sum = sum + x;
            {
                int x = 8;
                sum = sum + x;
                {
                    int x = 16;
                    sum = sum + x;
                }
            }
        }
    }
    // EXPECT: accumulated sum = 31
    printf("accumulated sum = %d\n", sum);
    // EXPECT: final x = 1
    printf("final x = %d\n", x);
    // EXPECT: accumulate_shadows returned 31
    return sum;
}

int main(void) {
    int r1 = check_levels(1);
    printf("check_levels returned %d\n", r1);

    int r2 = nested_multi_var();
    printf("nested_multi_var returned %d\n", r2);

    int r3 = accumulate_shadows();
    printf("accumulate_shadows returned %d\n", r3);

    int v = 0;
    {
        int v = 5;
        {
            int v = 50;
            {
                int v = 500;
                {
                    int v = 5000;
                    // EXPECT: deepest v = 5000
                    printf("deepest v = %d\n", v);
                }
                // EXPECT: 4th v = 500
                printf("4th v = %d\n", v);
            }
            // EXPECT: 3rd v = 50
            printf("3rd v = %d\n", v);
        }
        // EXPECT: 2nd v = 5
        printf("2nd v = %d\n", v);
    }
    // EXPECT: 1st v = 0
    printf("1st v = %d\n", v);

    return 0;
}
