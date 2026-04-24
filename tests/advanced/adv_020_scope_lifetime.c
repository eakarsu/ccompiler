int printf(const char *fmt, ...);
// EXPECT: === Scope and Lifetime Tests ===
// EXPECT: global_var: 100
// EXPECT: global_modify: 200
// EXPECT: local_basic: x=10
// EXPECT: local_no_clash: x=10 y=20
// EXPECT: shadow_outer: x=1
// EXPECT: shadow_inner: x=2
// EXPECT: shadow_after: x=1
// EXPECT: shadow_deep_0: x=0
// EXPECT: shadow_deep_1: x=10
// EXPECT: shadow_deep_2: x=200
// EXPECT: shadow_deep_after_2: x=10
// EXPECT: shadow_deep_after_1: x=0
// EXPECT: block_scope: inner=42
// EXPECT: block_after: outer=10
// EXPECT: loop_scope: i=0 i=1 i=2
// EXPECT: loop_after: sum=3
// EXPECT: loop_var_reset: 0 0 0
// EXPECT: static_call_1: 1
// EXPECT: static_call_2: 2
// EXPECT: static_call_3: 3
// EXPECT: static_call_4: 4
// EXPECT: static_call_5: 5
// EXPECT: static_accumulate_1: 10
// EXPECT: static_accumulate_2: 30
// EXPECT: static_accumulate_3: 60
// EXPECT: multi_static_a1: 1 b1: 100
// EXPECT: multi_static_a2: 2 b2: 200
// EXPECT: if_scope_true: inner=5
// EXPECT: if_scope_after: outer=10
// EXPECT: nested_blocks: a=1 b=2 c=3
// EXPECT: nested_after: a=1
// EXPECT: param_scope: x=42 y=99
// EXPECT: param_scope: x=10 y=0
// EXPECT: param_no_modify: orig=10
// EXPECT: for_body_scope: 0 1 2 3 4
// EXPECT: while_scope: count=5
// EXPECT: global_from_func: 500
// EXPECT: shadow_param: x=42
// EXPECT: shadow_param: x=100
// EXPECT: shadow_param_after: x=100
// EXPECT: same_name_diff_scope: 10 20
// EXPECT: reuse_name_loop: 0 1 2
// EXPECT: === All Scope and Lifetime Tests Passed ===

int g_var = 100;

int counter(void) {
    static int count = 0;
    count = count + 1;
    return count;
}

int accumulator(int val) {
    static int total = 0;
    total = total + val;
    return total;
}

int static_a(void) {
    static int a = 0;
    a = a + 1;
    return a;
}

int static_b(void) {
    static int b = 0;
    b = b + 100;
    return b;
}

void set_global(int val) {
    g_var = val;
}

void param_test(int x, int y) {
    printf("param_scope: x=%d y=%d\n", x, y);
}

void shadow_param_test(int x) {
    printf("shadow_param: x=%d\n", x);
    {
        int x = 100;
        printf("shadow_param: x=%d\n", x);
    }
    printf("shadow_param_after: x=%d\n", x);
}

int main() {
    printf("=== Scope and Lifetime Tests ===\n");

    // global variable access
    printf("global_var: %d\n", g_var);

    // global variable modification
    g_var = 200;
    printf("global_modify: %d\n", g_var);

    // basic local variable
    int x = 10;
    printf("local_basic: x=%d\n", x);

    // locals don't clash
    int y = 20;
    printf("local_no_clash: x=%d y=%d\n", x, y);

    // variable shadowing
    {
        int x = 1;
        printf("shadow_outer: x=%d\n", x);
        {
            int x = 2;
            printf("shadow_inner: x=%d\n", x);
        }
        printf("shadow_after: x=%d\n", x);
    }

    // deep shadowing
    {
        int x = 0;
        printf("shadow_deep_0: x=%d\n", x);
        {
            int x = 10;
            printf("shadow_deep_1: x=%d\n", x);
            {
                int x = 200;
                printf("shadow_deep_2: x=%d\n", x);
            }
            printf("shadow_deep_after_2: x=%d\n", x);
        }
        printf("shadow_deep_after_1: x=%d\n", x);
    }

    // block scope
    int outer = 10;
    {
        int inner = 42;
        printf("block_scope: inner=%d\n", inner);
    }
    printf("block_after: outer=%d\n", outer);

    // loop scope - loop counter
    int i;
    printf("loop_scope:");
    for (i = 0; i < 3; i++) {
        printf(" i=%d", i);
    }
    printf("\n");

    // variable after loop
    int sum = 0;
    for (i = 0; i < 3; i++) {
        sum = sum + 1;
    }
    printf("loop_after: sum=%d\n", sum);

    // variable reset in each loop iteration
    printf("loop_var_reset:");
    for (i = 0; i < 3; i++) {
        int tmp = 0;
        printf(" %d", tmp);
    }
    printf("\n");

    // static local variable - persists across calls
    printf("static_call_1: %d\n", counter());
    printf("static_call_2: %d\n", counter());
    printf("static_call_3: %d\n", counter());
    printf("static_call_4: %d\n", counter());
    printf("static_call_5: %d\n", counter());

    // static accumulator
    printf("static_accumulate_1: %d\n", accumulator(10));
    printf("static_accumulate_2: %d\n", accumulator(20));
    printf("static_accumulate_3: %d\n", accumulator(30));

    // multiple static variables in different functions
    int a1 = static_a();
    int b1 = static_b();
    printf("multi_static_a1: %d b1: %d\n", a1, b1);
    int a2 = static_a();
    int b2 = static_b();
    printf("multi_static_a2: %d b2: %d\n", a2, b2);

    // scope in if statements
    int if_outer = 10;
    if (1) {
        int inner = 5;
        printf("if_scope_true: inner=%d\n", inner);
    }
    printf("if_scope_after: outer=%d\n", if_outer);

    // nested blocks
    {
        int a = 1;
        {
            int b = 2;
            {
                int c = 3;
                printf("nested_blocks: a=%d b=%d c=%d\n", a, b, c);
            }
        }
        printf("nested_after: a=%d\n", a);
    }

    // function parameter scope
    param_test(42, 99);

    // parameter doesn't modify caller's variable
    int orig = 10;
    param_test(orig, 0);
    printf("param_no_modify: orig=%d\n", orig);

    // for loop body scope
    printf("for_body_scope:");
    for (i = 0; i < 5; i++) {
        int val = i;
        printf(" %d", val);
    }
    printf("\n");

    // while loop scope
    int count = 0;
    int wn = 0;
    while (wn < 5) {
        int step = 1;
        count = count + step;
        wn = wn + 1;
    }
    printf("while_scope: count=%d\n", count);

    // global modified from function
    set_global(500);
    printf("global_from_func: %d\n", g_var);

    // function parameter shadowing
    shadow_param_test(42);

    // same name in different scopes
    {
        int v = 10;
        printf("same_name_diff_scope: %d", v);
    }
    {
        int v = 20;
        printf(" %d\n", v);
    }

    // reuse variable name in loop
    printf("reuse_name_loop:");
    for (i = 0; i < 3; i++) {
        int r = i;
        printf(" %d", r);
    }
    printf("\n");

    printf("=== All Scope and Lifetime Tests Passed ===\n");
    return 0;
}
