int printf(const char *fmt, ...);

// Test deeply nested scopes and variable visibility at each level.

int g_depth = 0;

int deep_nesting_test(void) {
    int a = 1;
    {
        int b = 2;
        {
            int c = 3;
            {
                int d = 4;
                {
                    int e = 5;
                    {
                        int f = 6;
                        // All visible here
                        return a + b + c + d + e + f;
                    }
                }
            }
        }
    }
}

int cascade_shadow(int x) {
    int result = 0;
    {
        int x = 10;
        result = result + x;
        {
            int x = 20;
            result = result + x;
            {
                int x = 30;
                result = result + x;
                {
                    int x = 40;
                    result = result + x;
                }
            }
        }
    }
    result = result + x;
    return result;
}

int accumulate_depths(void) {
    int total = 0;
    int depth = 0;
    total = total + depth;
    {
        depth = 1;
        total = total + depth;
        {
            depth = 2;
            total = total + depth;
            {
                depth = 3;
                total = total + depth;
                {
                    depth = 4;
                    total = total + depth;
                }
                // depth is still 4 after inner block
                total = total + depth;
            }
            total = total + depth;
        }
        total = total + depth;
    }
    total = total + depth;
    // total = 0+1+2+3+4+4+4+4+4 = 26
    return total;
}

int interleaved_scopes(int n) {
    int sum = 0;
    {
        int a = n;
        sum = sum + a;
    }
    {
        int a = n * 2;
        sum = sum + a;
    }
    {
        int a = n * 3;
        sum = sum + a;
        {
            int b = a + 1;
            sum = sum + b;
        }
    }
    // sum = n + 2n + 3n + (3n+1) = 9n + 1
    return sum;
}

int scope_in_loop(void) {
    int total = 0;
    int i;
    for (i = 0; i < 5; i = i + 1) {
        int val = i;
        {
            int inner = val * 2;
            total = total + inner;
        }
    }
    // total = 0+2+4+6+8 = 20
    return total;
}

int scope_in_while(void) {
    int sum = 0;
    int n = 4;
    while (n > 0) {
        int sq = n * n;
        {
            int doubled = sq * 2;
            sum = sum + doubled;
        }
        n = n - 1;
    }
    // sum = 2*(16+9+4+1) = 2*30 = 60
    return sum;
}

int scope_in_if(int x) {
    int result = 0;
    if (x > 0) {
        int pos = x;
        {
            int doubled = pos * 2;
            result = doubled;
        }
    } else {
        int neg = -x;
        {
            int tripled = neg * 3;
            result = tripled;
        }
    }
    return result;
}

int scope_and_goto(void) {
    int x = 10;
    {
        int y = 20;
        if (y > 15) {
            goto done;
        }
        x = 0;
    }
done:
    return x;
}

int main(void) {
    // deep_nesting_test: 1+2+3+4+5+6 = 21
    // EXPECT: 21
    printf("%d\n", deep_nesting_test());

    // cascade_shadow(5):
    // inner1: x=10, result=10. inner2: x=20, result=30.
    // inner3: x=30, result=60. inner4: x=40, result=100.
    // outer: result = 100+5 = 105
    // EXPECT: 105
    printf("%d\n", cascade_shadow(5));

    // EXPECT: 26
    printf("%d\n", accumulate_depths());

    // interleaved_scopes(5): 9*5+1 = 46
    // EXPECT: 46
    printf("%d\n", interleaved_scopes(5));

    // EXPECT: 20
    printf("%d\n", scope_in_loop());

    // EXPECT: 60
    printf("%d\n", scope_in_while());

    // scope_in_if(7): pos=7, doubled=14
    // EXPECT: 14
    printf("%d\n", scope_in_if(7));

    // scope_in_if(-4): neg=4, tripled=12
    // EXPECT: 12
    printf("%d\n", scope_in_if(-4));

    // scope_and_goto: y=20>15, goto done, x=10
    // EXPECT: 10
    printf("%d\n", scope_and_goto());

    // Deep inline nesting in main
    {
        int v1 = 100;
        {
            int v2 = 200;
            {
                int v3 = 300;
                {
                    int v4 = 400;
                    // EXPECT: 1000
                    printf("%d\n", v1 + v2 + v3 + v4);
                }
                // EXPECT: 600
                printf("%d\n", v1 + v2 + v3);
            }
            // EXPECT: 300
            printf("%d\n", v1 + v2);
        }
        // EXPECT: 100
        printf("%d\n", v1);
    }

    // Test that same names in sequential blocks are independent
    {
        int z = 1;
        // EXPECT: 1
        printf("%d\n", z);
    }
    {
        int z = 2;
        // EXPECT: 2
        printf("%d\n", z);
    }
    {
        int z = 3;
        // EXPECT: 3
        printf("%d\n", z);
    }

    // Global was never touched
    // EXPECT: 0
    printf("%d\n", g_depth);

    return 0;
}
