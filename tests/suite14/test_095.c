int printf(const char *fmt, ...);

// Test 095: Comma operator in various contexts

void test_basic_comma(void) {
    int x;
    // Comma operator evaluates left, discards, returns right
    x = (1, 2, 3);
    printf("%d\n", x);
    // EXPECT: 3

    x = (10 + 5, 20 + 3);
    printf("%d\n", x);
    // EXPECT: 23

    x = (100, 200, 300, 400);
    printf("%d\n", x);
    // EXPECT: 400
}

void test_comma_side_effects(void) {
    int a = 0;
    int b = 0;
    int c;

    // Side effects in comma expressions
    c = (a = 5, b = 10, a + b);
    printf("%d\n", a);
    // EXPECT: 5
    printf("%d\n", b);
    // EXPECT: 10
    printf("%d\n", c);
    // EXPECT: 15
}

void test_comma_in_for(void) {
    int i;
    int j;
    int sum = 0;

    // Comma in for init and increment
    for (i = 0, j = 10; i < 5; i++, j--) {
        sum = sum + i + j;
    }
    // i=0,j=10: 10; i=1,j=9: 10; i=2,j=8: 10; i=3,j=7: 10; i=4,j=6: 10
    // total = 50
    printf("%d\n", sum);
    // EXPECT: 50
    printf("%d\n", i);
    // EXPECT: 5
    printf("%d\n", j);
    // EXPECT: 5
}

void test_comma_in_while(void) {
    int x = 0;
    int count = 0;

    // Comma in while condition: increments x, tests x < 5
    while (x++, x < 5) {
        count++;
    }
    // x goes 1,2,3,4 then 5 fails. count = 4
    printf("%d\n", x);
    // EXPECT: 5
    printf("%d\n", count);
    // EXPECT: 4
}

void test_comma_in_condition(void) {
    int a = 0;
    int result;

    // Comma in if condition
    if (a = 42, a > 40) {
        result = 1;
    } else {
        result = 0;
    }
    printf("%d\n", a);
    // EXPECT: 42
    printf("%d\n", result);
    // EXPECT: 1
}

void test_comma_nested(void) {
    int a;
    int b;
    int c;

    a = (b = 3, c = b * 2, b + c);
    // b=3, c=6, a=9
    printf("%d\n", a);
    // EXPECT: 9
    printf("%d\n", b);
    // EXPECT: 3
    printf("%d\n", c);
    // EXPECT: 6
}

void test_comma_in_array_index(void) {
    int arr[5] = {10, 20, 30, 40, 50};
    int idx;
    int val;

    // Comma operator in array index (parenthesized)
    val = arr[(idx = 2, idx)];
    printf("%d\n", val);
    // EXPECT: 30
    printf("%d\n", idx);
    // EXPECT: 2

    val = arr[(idx = 0, idx + 3)];
    printf("%d\n", val);
    // EXPECT: 40
}

void test_comma_evaluation_order(void) {
    int seq[6];
    int pos = 0;
    int result;

    // Each comma subexpression should execute left-to-right
    result = (seq[pos++] = 10, seq[pos++] = 20, seq[pos++] = 30, pos);
    printf("%d\n", result);
    // EXPECT: 3
    printf("%d\n", seq[0]);
    // EXPECT: 10
    printf("%d\n", seq[1]);
    // EXPECT: 20
    printf("%d\n", seq[2]);
    // EXPECT: 30
}

void test_comma_with_ternary(void) {
    int x = 5;
    int y;

    // Comma used together with ternary
    y = (x > 3) ? (x = x + 1, x * 2) : (x = x - 1, x * 3);
    // x>3 is true, so x becomes 6, y = 12
    printf("%d\n", x);
    // EXPECT: 6
    printf("%d\n", y);
    // EXPECT: 12
}

void test_comma_accumulator(void) {
    int total = 0;
    int i;
    int tmp;

    for (i = 1; i <= 5; i++) {
        tmp = (total = total + i, total * 2);
    }
    // After loop: total = 1+2+3+4+5 = 15
    // Last tmp = 15*2 = 30
    printf("%d\n", total);
    // EXPECT: 15
    printf("%d\n", tmp);
    // EXPECT: 30
}

int main(void) {
    test_basic_comma();
    test_comma_side_effects();
    test_comma_in_for();
    test_comma_in_while();
    test_comma_in_condition();
    test_comma_nested();
    test_comma_in_array_index();
    test_comma_evaluation_order();
    test_comma_with_ternary();
    test_comma_accumulator();
    return 0;
}
