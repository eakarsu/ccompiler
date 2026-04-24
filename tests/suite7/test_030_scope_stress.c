int printf(const char *fmt, ...);

int deep_shadow(int depth) {
    int x = 0;
    {
        int x = 1;
        {
            int x = 2;
            {
                int x = 3;
                {
                    int x = 4;
                    {
                        int x = 5;
                        {
                            int x = 6;
                            {
                                int x = 7;
                                {
                                    int x = 8;
                                    {
                                        int x = 9;
                                        {
                                            int x = 10;
                                            if (depth == 10) return x;
                                        }
                                        if (depth == 9) return x;
                                    }
                                    if (depth == 8) return x;
                                }
                                if (depth == 7) return x;
                            }
                            if (depth == 6) return x;
                        }
                        if (depth == 5) return x;
                    }
                    if (depth == 4) return x;
                }
                if (depth == 3) return x;
            }
            if (depth == 2) return x;
        }
        if (depth == 1) return x;
    }
    return x;
}

void test_deep_shadow(void) {
    // EXPECT: depth 0 = 0
    printf("depth 0 = %d\n", deep_shadow(0));
    // EXPECT: depth 1 = 1
    printf("depth 1 = %d\n", deep_shadow(1));
    // EXPECT: depth 5 = 5
    printf("depth 5 = %d\n", deep_shadow(5));
    // EXPECT: depth 10 = 10
    printf("depth 10 = %d\n", deep_shadow(10));
}

int recursive_scope(int n) {
    if (n <= 0) return 0;
    int x = n;
    {
        int x = n * 10;
        {
            int x = n * 100;
            if (n == 1) {
                // EXPECT: rec deep x = 100
                printf("rec deep x = %d\n", x);
            }
        }
    }
    return x + recursive_scope(n - 1);
}

void test_recursive_scope(void) {
    int r = recursive_scope(5);
    // EXPECT: recursive sum = 15
    printf("recursive sum = %d\n", r);
}

void stress_sequential(void) {
    int total = 0;
    { int v = 1; total = total + v; }
    { int v = 2; total = total + v; }
    { int v = 3; total = total + v; }
    { int v = 4; total = total + v; }
    { int v = 5; total = total + v; }
    { int v = 6; total = total + v; }
    { int v = 7; total = total + v; }
    { int v = 8; total = total + v; }
    { int v = 9; total = total + v; }
    { int v = 10; total = total + v; }
    { int v = 11; total = total + v; }
    { int v = 12; total = total + v; }
    // EXPECT: sequential total = 78
    printf("sequential total = %d\n", total);
}

int nested_call_a(int n);
int nested_call_b(int n);

int nested_call_a(int n) {
    if (n <= 0) return 0;
    int x = n;
    {
        int x = n + 100;
        return x + nested_call_b(n - 1);
    }
}

int nested_call_b(int n) {
    if (n <= 0) return 0;
    int x = n;
    {
        int x = n + 200;
        return x + nested_call_a(n - 1);
    }
}

void test_mutual_recursion(void) {
    int r = nested_call_a(4);
    // EXPECT: mutual rec = 610
    printf("mutual rec = %d\n", r);
}

void stress_mixed(void) {
    int a = 1;
    int sum = 0;
    {
        int a = 10;
        int i;
        for (i = 0; i < 3; i = i + 1) {
            int a = i + 100;
            if (a > 101) {
                int a = 999;
                sum = sum + a;
            } else {
                int a = 1;
                sum = sum + a;
            }
        }
        sum = sum + a;
    }
    sum = sum + a;
    // EXPECT: stress mixed sum = 1012
    printf("stress mixed sum = %d\n", sum);
}

void deeply_nested_arrays(void) {
    int sum = 0;
    {
        int a[2]; a[0] = 1; a[1] = 2;
        sum = sum + a[0] + a[1];
        {
            int a[2]; a[0] = 10; a[1] = 20;
            sum = sum + a[0] + a[1];
            {
                int a[2]; a[0] = 100; a[1] = 200;
                sum = sum + a[0] + a[1];
            }
        }
    }
    // EXPECT: nested arrays sum = 333
    printf("nested arrays sum = %d\n", sum);
}

void test_shadow_in_all_loops(void) {
    int x = 7;
    int sum = 0;
    int i;

    for (i = 0; i < 2; i = i + 1) {
        int x = 10;
        sum = sum + x;
    }

    i = 0;
    while (i < 2) {
        int x = 20;
        sum = sum + x;
        i = i + 1;
    }

    i = 0;
    do {
        int x = 30;
        sum = sum + x;
        i = i + 1;
    } while (i < 2);

    // EXPECT: all loops sum = 120
    printf("all loops sum = %d\n", sum);
    // EXPECT: x preserved = 7
    printf("x preserved = %d\n", x);
}

void test_many_vars_same_name(void) {
    int r = 0;
    { int z = 1; r = r + z; }
    { int z = 2; r = r + z; }
    { int z = 3; r = r + z; }
    { int z = 4; r = r + z; }
    { int z = 5; r = r + z; }
    // EXPECT: many vars r = 15
    printf("many vars r = %d\n", r);
}

int main(void) {
    test_deep_shadow();
    test_recursive_scope();
    stress_sequential();
    test_mutual_recursion();
    stress_mixed();
    deeply_nested_arrays();
    test_shadow_in_all_loops();
    test_many_vars_same_name();

    int check = 0;
    {
        int check = 42;
        {
            int check = 84;
            // EXPECT: main deep check = 84
            printf("main deep check = %d\n", check);
        }
        // EXPECT: main mid check = 42
        printf("main mid check = %d\n", check);
    }
    // EXPECT: main outer check = 0
    printf("main outer check = %d\n", check);

    // EXPECT: all stress tests passed
    printf("all stress tests passed\n");

    return 0;
}
