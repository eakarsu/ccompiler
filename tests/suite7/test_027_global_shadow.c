int printf(const char *fmt, ...);

int g_val = 100;
int g_x = 42;
int g_count = 0;

void test_basic_shadow(void) {
    // EXPECT: global g_val = 100
    printf("global g_val = %d\n", g_val);
    {
        int g_val = 200;
        // EXPECT: shadow g_val = 200
        printf("shadow g_val = %d\n", g_val);
    }
    // EXPECT: restored g_val = 100
    printf("restored g_val = %d\n", g_val);
}

void test_param_shadows_global(int g_val) {
    // EXPECT: param g_val = 55
    printf("param g_val = %d\n", g_val);
    // EXPECT: main sees g_val = 100
}

void test_multi_level_shadow(void) {
    // EXPECT: ml global g_x = 42
    printf("ml global g_x = %d\n", g_x);
    {
        int g_x = 1000;
        // EXPECT: ml level1 g_x = 1000
        printf("ml level1 g_x = %d\n", g_x);
        {
            int g_x = 2000;
            // EXPECT: ml level2 g_x = 2000
            printf("ml level2 g_x = %d\n", g_x);
            {
                int g_x = 3000;
                // EXPECT: ml level3 g_x = 3000
                printf("ml level3 g_x = %d\n", g_x);
            }
            // EXPECT: ml back2 g_x = 2000
            printf("ml back2 g_x = %d\n", g_x);
        }
        // EXPECT: ml back1 g_x = 1000
        printf("ml back1 g_x = %d\n", g_x);
    }
    // EXPECT: ml back0 g_x = 42
    printf("ml back0 g_x = %d\n", g_x);
}

void modify_global(void) {
    g_count = g_count + 1;
}

void test_global_modify_with_shadow(void) {
    g_count = 10;
    // EXPECT: before shadow g_count = 10
    printf("before shadow g_count = %d\n", g_count);
    {
        int g_count = 99;
        // EXPECT: local g_count = 99
        printf("local g_count = %d\n", g_count);
        modify_global();
        // EXPECT: local still g_count = 99
        printf("local still g_count = %d\n", g_count);
    }
    // EXPECT: global modified g_count = 11
    printf("global modified g_count = %d\n", g_count);
}

int shadow_and_return(void) {
    int g_val = 777;
    return g_val;
}

void test_shadow_return(void) {
    int r = shadow_and_return();
    // EXPECT: shadow returned 777
    printf("shadow returned %d\n", r);
    // EXPECT: global untouched g_val = 100
    printf("global untouched g_val = %d\n", g_val);
}

void test_loop_shadow_global(void) {
    int i;
    int sum = 0;
    for (i = 0; i < 3; i = i + 1) {
        int g_val = i * 5;
        sum = sum + g_val;
    }
    // EXPECT: loop shadow sum = 15
    printf("loop shadow sum = %d\n", sum);
    // EXPECT: global g_val still = 100
    printf("global g_val still = %d\n", g_val);
}

int main(void) {
    test_basic_shadow();
    test_param_shadows_global(55);

    printf("main sees g_val = %d\n", g_val);

    test_multi_level_shadow();
    test_global_modify_with_shadow();
    test_shadow_return();
    test_loop_shadow_global();

    // EXPECT: final g_val = 100
    printf("final g_val = %d\n", g_val);
    // EXPECT: final g_x = 42
    printf("final g_x = %d\n", g_x);

    return 0;
}
