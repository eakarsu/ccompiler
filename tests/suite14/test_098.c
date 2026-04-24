int printf(const char *fmt, ...);

// Test 098: Scope and variable shadowing (deep nesting)

int x = 100;  // global x

void test_basic_shadow(void) {
    int x = 10;
    printf("%d\n", x);
    // EXPECT: 10
    {
        int x = 20;
        printf("%d\n", x);
        // EXPECT: 20
    }
    printf("%d\n", x);
    // EXPECT: 10
}

void test_global_shadow(void) {
    // x here refers to global x
    printf("%d\n", x);
    // EXPECT: 100
    {
        int x = 50;
        printf("%d\n", x);
        // EXPECT: 50
    }
    printf("%d\n", x);
    // EXPECT: 100
}

void test_deep_nesting(void) {
    int val = 1;
    printf("%d\n", val);
    // EXPECT: 1
    {
        int val = 2;
        printf("%d\n", val);
        // EXPECT: 2
        {
            int val = 3;
            printf("%d\n", val);
            // EXPECT: 3
            {
                int val = 4;
                printf("%d\n", val);
                // EXPECT: 4
                {
                    int val = 5;
                    printf("%d\n", val);
                    // EXPECT: 5
                }
                printf("%d\n", val);
                // EXPECT: 4
            }
            printf("%d\n", val);
            // EXPECT: 3
        }
        printf("%d\n", val);
        // EXPECT: 2
    }
    printf("%d\n", val);
    // EXPECT: 1
}

void test_shadow_in_for(void) {
    int i = 999;
    int sum = 0;
    {
        int i;
        for (i = 0; i < 5; i++) {
            sum = sum + i;
        }
    }
    printf("%d\n", sum);
    // EXPECT: 10
    printf("%d\n", i);  // original i unchanged
    // EXPECT: 999
}

void test_shadow_different_types(void) {
    int val = 42;
    printf("%d\n", val);
    // EXPECT: 42
    {
        char val = 65;  // 'A'
        printf("%d\n", val);
        // EXPECT: 65
    }
    printf("%d\n", val);
    // EXPECT: 42
}

void test_shadow_with_pointers(void) {
    int a = 10;
    int b = 20;
    int *p = &a;
    printf("%d\n", *p);
    // EXPECT: 10
    {
        int *p = &b;
        printf("%d\n", *p);
        // EXPECT: 20
        *p = 25;
    }
    printf("%d\n", *p);  // still points to a
    // EXPECT: 10
    printf("%d\n", b);   // modified through inner p
    // EXPECT: 25
}

void test_shadow_accumulate(void) {
    int result = 0;
    {
        int x = 1;
        result = result + x;
        {
            int x = 2;
            result = result + x;
            {
                int x = 4;
                result = result + x;
                {
                    int x = 8;
                    result = result + x;
                }
            }
        }
    }
    // 1 + 2 + 4 + 8 = 15
    printf("%d\n", result);
    // EXPECT: 15
}

void test_shadow_in_branches(void) {
    int x = 100;
    int flag = 1;

    if (flag) {
        int x = 200;
        printf("%d\n", x);
        // EXPECT: 200
    } else {
        int x = 300;
        printf("%d\n", x);
    }
    printf("%d\n", x);
    // EXPECT: 100

    flag = 0;
    if (flag) {
        int x = 200;
        printf("%d\n", x);
    } else {
        int x = 300;
        printf("%d\n", x);
        // EXPECT: 300
    }
    printf("%d\n", x);
    // EXPECT: 100
}

void test_shadow_struct(void) {
    struct S { int v; };
    struct S s;
    s.v = 111;
    printf("%d\n", s.v);
    // EXPECT: 111
    {
        struct S s;
        s.v = 222;
        printf("%d\n", s.v);
        // EXPECT: 222
    }
    printf("%d\n", s.v);
    // EXPECT: 111
}

void test_shadow_in_while(void) {
    int count = 3;
    int total = 0;
    while (count > 0) {
        int count = 10;  // shadows outer count
        total = total + count;
        count--;  // modifies inner count, not outer
        break;    // must break since inner count doesn't control loop
    }
    // Loop executes once (outer count=3 > 0), total = 10
    // Then we need to decrement outer count manually
    // Actually, count-- modified inner count. Outer count is still 3.
    // This would be infinite loop without the break.
    printf("%d\n", total);
    // EXPECT: 10
    printf("%d\n", count);  // outer count still 3
    // EXPECT: 3
}

int main(void) {
    test_basic_shadow();
    test_global_shadow();
    test_deep_nesting();
    test_shadow_in_for();
    test_shadow_different_types();
    test_shadow_with_pointers();
    test_shadow_accumulate();
    test_shadow_in_branches();
    test_shadow_struct();
    test_shadow_in_while();
    return 0;
}
