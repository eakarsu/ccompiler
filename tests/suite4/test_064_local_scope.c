int printf(const char *fmt, ...);

// Test block scoping, variable shadowing, inner block hides outer variable.

int g_val = 100;

int shadow_test(int x) {
    int result = x;
    {
        int x = 77;
        result = result + x;
    }
    // outer x is unchanged
    result = result + x;
    return result;
}

int nested_shadow(int a) {
    int sum = 0;
    sum = sum + a;   // a = original
    {
        int a = 50;
        sum = sum + a; // a = 50
        {
            int a = 200;
            sum = sum + a; // a = 200
        }
        sum = sum + a; // a = 50 again
    }
    sum = sum + a;   // a = original again
    return sum;
}

int block_var_test(void) {
    int x = 1;
    {
        int y = 10;
        x = x + y;
    }
    // y is no longer visible, x should be 11
    return x;
}

int loop_scope_test(void) {
    int sum = 0;
    int i;
    for (i = 0; i < 5; i = i + 1) {
        int val = i * 10;
        sum = sum + val;
    }
    // val not visible here, sum = 0+10+20+30+40 = 100
    return sum;
}

int if_scope_test(int cond) {
    int result = 0;
    if (cond) {
        int tmp = 42;
        result = tmp;
    } else {
        int tmp = 99;
        result = tmp;
    }
    return result;
}

int reuse_name_test(void) {
    int total = 0;
    {
        int x = 5;
        total = total + x;
    }
    {
        int x = 10;
        total = total + x;
    }
    {
        int x = 15;
        total = total + x;
    }
    return total;
}

int shadow_with_global(void) {
    int g_val = 999;
    return g_val;
}

int while_scope_test(void) {
    int count = 0;
    int n = 3;
    while (n > 0) {
        int step = n * 2;
        count = count + step;
        n = n - 1;
    }
    // step not visible. count = 6+4+2 = 12
    return count;
}

int do_while_scope_test(void) {
    int sum = 0;
    int i = 1;
    do {
        int val = i * i;
        sum = sum + val;
        i = i + 1;
    } while (i <= 4);
    // sum = 1+4+9+16 = 30
    return sum;
}

int main(void) {
    // shadow_test(5): result=5, inner x=77, result=5+77=82, result=82+5=87
    // EXPECT: 87
    printf("%d\n", shadow_test(5));

    // nested_shadow(3):
    // sum = 0+3 = 3
    // inner a = 50, sum = 3+50 = 53
    // innermost a = 200, sum = 53+200 = 253
    // back to inner a = 50, sum = 253+50 = 303
    // back to outer a = 3, sum = 303+3 = 306
    // EXPECT: 306
    printf("%d\n", nested_shadow(3));

    // EXPECT: 11
    printf("%d\n", block_var_test());

    // EXPECT: 100
    printf("%d\n", loop_scope_test());

    // EXPECT: 42
    printf("%d\n", if_scope_test(1));

    // EXPECT: 99
    printf("%d\n", if_scope_test(0));

    // reuse_name_test: 5+10+15 = 30
    // EXPECT: 30
    printf("%d\n", reuse_name_test());

    // shadow_with_global: local shadows global, returns 999
    // EXPECT: 999
    printf("%d\n", shadow_with_global());

    // Global should still be 100
    // EXPECT: 100
    printf("%d\n", g_val);

    // EXPECT: 12
    printf("%d\n", while_scope_test());

    // EXPECT: 30
    printf("%d\n", do_while_scope_test());

    // Inline block scoping in main
    {
        int a = 50;
        // EXPECT: 50
        printf("%d\n", a);
        {
            int a = 60;
            // EXPECT: 60
            printf("%d\n", a);
        }
        // EXPECT: 50
        printf("%d\n", a);
    }

    // More shadowing in main
    {
        int x = 1;
        {
            int x = 2;
            {
                int x = 3;
                // EXPECT: 3
                printf("%d\n", x);
            }
            // EXPECT: 2
            printf("%d\n", x);
        }
        // EXPECT: 1
        printf("%d\n", x);
    }

    // EXPECT: 100
    printf("%d\n", g_val);

    return 0;
}
