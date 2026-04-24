int printf(const char *fmt, ...);

// Test 097: Ternary operator chaining and nesting

void test_basic_ternary(void) {
    int x = 10;
    int y = x > 5 ? 100 : 200;
    printf("%d\n", y);
    // EXPECT: 100

    y = x < 5 ? 100 : 200;
    printf("%d\n", y);
    // EXPECT: 200
}

void test_chained_ternary(void) {
    // Chained ternary acts like if-else-if
    int x = 2;
    int result = (x == 1) ? 10 :
                 (x == 2) ? 20 :
                 (x == 3) ? 30 : 40;
    printf("%d\n", result);
    // EXPECT: 20

    x = 4;
    result = (x == 1) ? 10 :
             (x == 2) ? 20 :
             (x == 3) ? 30 : 40;
    printf("%d\n", result);
    // EXPECT: 40

    x = 1;
    result = (x == 1) ? 10 :
             (x == 2) ? 20 :
             (x == 3) ? 30 : 40;
    printf("%d\n", result);
    // EXPECT: 10
}

int classify(int n) {
    return (n < 0)   ? -1 :
           (n == 0)  ?  0 :
           (n < 10)  ?  1 :
           (n < 100) ?  2 : 3;
}

void test_classify(void) {
    printf("%d\n", classify(-5));
    // EXPECT: -1
    printf("%d\n", classify(0));
    // EXPECT: 0
    printf("%d\n", classify(7));
    // EXPECT: 1
    printf("%d\n", classify(42));
    // EXPECT: 2
    printf("%d\n", classify(999));
    // EXPECT: 3
}

void test_nested_ternary(void) {
    int a = 3;
    int b = 7;

    // Nested in both branches
    int result = (a > b) ? ((a > 10) ? 1 : 2) : ((b > 10) ? 3 : 4);
    // a>b is false, b>10 is false, result = 4
    printf("%d\n", result);
    // EXPECT: 4

    a = 15;
    result = (a > b) ? ((a > 10) ? 1 : 2) : ((b > 10) ? 3 : 4);
    // a>b is true, a>10 is true, result = 1
    printf("%d\n", result);
    // EXPECT: 1
}

void test_ternary_side_effects(void) {
    int x = 0;
    int y = 0;

    // Only one branch should execute
    int r = (1) ? (x = 5, x) : (y = 10, y);
    printf("%d\n", r);
    // EXPECT: 5
    printf("%d\n", x);
    // EXPECT: 5
    printf("%d\n", y);
    // EXPECT: 0

    x = 0;
    y = 0;
    r = (0) ? (x = 5, x) : (y = 10, y);
    printf("%d\n", r);
    // EXPECT: 10
    printf("%d\n", x);
    // EXPECT: 0
    printf("%d\n", y);
    // EXPECT: 10
}

int abs_val(int x) {
    return x >= 0 ? x : -x;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int clamp(int val, int lo, int hi) {
    return val < lo ? lo : (val > hi ? hi : val);
}

void test_ternary_functions(void) {
    printf("%d\n", abs_val(-42));
    // EXPECT: 42
    printf("%d\n", abs_val(17));
    // EXPECT: 17
    printf("%d\n", max(3, 9));
    // EXPECT: 9
    printf("%d\n", min(3, 9));
    // EXPECT: 3
    printf("%d\n", clamp(5, 1, 10));
    // EXPECT: 5
    printf("%d\n", clamp(-3, 1, 10));
    // EXPECT: 1
    printf("%d\n", clamp(15, 1, 10));
    // EXPECT: 10
}

void test_ternary_in_loop(void) {
    int i;
    int sum_pos = 0;
    int sum_neg = 0;
    int data[8] = {3, -1, 7, -4, 0, 2, -6, 8};

    for (i = 0; i < 8; i++) {
        int d = data[i];
        sum_pos = sum_pos + (d > 0 ? d : 0);
        sum_neg = sum_neg + (d < 0 ? d : 0);
    }
    // positives: 3+7+2+8 = 20
    printf("%d\n", sum_pos);
    // EXPECT: 20
    // negatives: -1 + -4 + -6 = -11
    printf("%d\n", sum_neg);
    // EXPECT: -11
}

void test_ternary_lvalue_simulation(void) {
    int a = 0;
    int b = 0;
    int flag = 1;

    // Simulate conditional assignment
    if (flag) { a = 99; } else { b = 99; }
    printf("%d\n", a);
    // EXPECT: 99
    printf("%d\n", b);
    // EXPECT: 0

    // Use ternary to pick value
    int chosen = flag ? a : b;
    printf("%d\n", chosen);
    // EXPECT: 99
}

int main(void) {
    test_basic_ternary();
    test_chained_ternary();
    test_classify();
    test_nested_ternary();
    test_ternary_side_effects();
    test_ternary_functions();
    test_ternary_in_loop();
    test_ternary_lvalue_simulation();
    return 0;
}
