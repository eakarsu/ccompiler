int printf(const char *fmt, ...);

int global_counter;

int increment(void) {
    global_counter = global_counter + 1;
    return global_counter;
}

int check_range(int x, int lo, int hi) {
    return x >= lo && x <= hi;
}

int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int all_positive(int a, int b, int c, int d) {
    return a > 0 && b > 0 && c > 0 && d > 0;
}

int any_zero(int a, int b, int c) {
    return a == 0 || b == 0 || c == 0;
}

int main(void) {
    // Test 1: simple AND
    int a = 5;
    int b = 10;
    int r = (a > 0 && b > 0);
    // EXPECT: simple_and: 1
    printf("simple_and: %d\n", r);

    // Test 2: simple OR
    r = (a < 0 || b > 0);
    // EXPECT: simple_or: 1
    printf("simple_or: %d\n", r);

    // Test 3: NOT
    r = !(a > 10);
    // EXPECT: simple_not: 1
    printf("simple_not: %d\n", r);

    // Test 4: AND short-circuit (false && ...)
    r = (0 && (1 / 0));
    // The 1/0 should not execute due to short-circuit
    // EXPECT: and_short: 0
    printf("and_short: %d\n", r);

    // Test 5: OR short-circuit (true || ...)
    r = (1 || (1 / 0));
    // EXPECT: or_short: 1
    printf("or_short: %d\n", r);

    // Test 6: AND short-circuit with side effects
    global_counter = 0;
    r = (0 && increment());
    // increment should NOT be called
    // EXPECT: and_side: 0 0
    printf("and_side: %d %d\n", r, global_counter);

    // Test 7: OR short-circuit with side effects
    global_counter = 0;
    r = (1 || increment());
    // increment should NOT be called
    // EXPECT: or_side: 1 0
    printf("or_side: %d %d\n", r, global_counter);

    // Test 8: AND that does evaluate second operand
    global_counter = 0;
    r = (1 && increment());
    // increment IS called
    // EXPECT: and_eval: 1 1
    printf("and_eval: %d %d\n", r, global_counter);

    // Test 9: OR that does evaluate second operand
    global_counter = 0;
    r = (0 || increment());
    // increment IS called
    // EXPECT: or_eval: 1 1
    printf("or_eval: %d %d\n", r, global_counter);

    // Test 10: compound AND chain
    int x = 15;
    r = (x > 0 && x < 100 && x % 3 == 0 && x % 5 == 0);
    // EXPECT: and_chain: 1
    printf("and_chain: %d\n", r);

    // Test 11: compound OR chain
    x = 7;
    r = (x == 3 || x == 5 || x == 7 || x == 11);
    // EXPECT: or_chain: 1
    printf("or_chain: %d\n", r);

    // Test 12: mixed AND/OR
    // AND has higher precedence than OR
    // a > 0 && b > 5 is true (5>0 && 10>5)
    // So (false || true) = true
    r = (a < 0 || a > 0 && b > 5);
    // EXPECT: mixed_and_or: 1
    printf("mixed_and_or: %d\n", r);

    // Test 13: parenthesized grouping changes result
    // (a < 0 || a > 0) = true, then true && (b > 100) = false
    r = ((a < 0 || a > 0) && b > 100);
    // EXPECT: grouped: 0
    printf("grouped: %d\n", r);

    // Test 14: check_range function
    // EXPECT: range_in: 1
    printf("range_in: %d\n", check_range(50, 1, 100));
    // EXPECT: range_out: 0
    printf("range_out: %d\n", check_range(150, 1, 100));
    // EXPECT: range_edge: 1
    printf("range_edge: %d\n", check_range(1, 1, 100));

    // Test 15: is_leap_year function
    // EXPECT: leap_2000: 1
    printf("leap_2000: %d\n", is_leap_year(2000));
    // EXPECT: leap_1900: 0
    printf("leap_1900: %d\n", is_leap_year(1900));
    // EXPECT: leap_2024: 1
    printf("leap_2024: %d\n", is_leap_year(2024));
    // EXPECT: leap_2023: 0
    printf("leap_2023: %d\n", is_leap_year(2023));

    // Test 16: all_positive function
    // EXPECT: all_pos_yes: 1
    printf("all_pos_yes: %d\n", all_positive(1, 2, 3, 4));
    // EXPECT: all_pos_no: 0
    printf("all_pos_no: %d\n", all_positive(1, -2, 3, 4));

    // Test 17: any_zero function
    // EXPECT: any_zero_yes: 1
    printf("any_zero_yes: %d\n", any_zero(1, 0, 3));
    // EXPECT: any_zero_no: 0
    printf("any_zero_no: %d\n", any_zero(1, 2, 3));

    // Test 18: nested negation
    r = !!42;
    // EXPECT: double_not: 1
    printf("double_not: %d\n", r);
    r = !!!0;
    // EXPECT: triple_not: 1
    printf("triple_not: %d\n", r);

    // Test 19: complex condition in loop
    int sum = 0;
    int i;
    for (i = 1; i <= 50; i = i + 1) {
        if ((i % 2 == 0 && i % 3 == 0) || i % 7 == 0) {
            sum = sum + i;
        }
    }
    // multiples of 6 up to 50: 6,12,18,24,30,36,42,48 = 216
    // multiples of 7 up to 50 not already counted: 7,14,21,28,35,49 = 154
    // 42 is both (6*7), already in mult of 6 list
    // total = 216 + 154 = 370
    // EXPECT: complex_loop: 370
    printf("complex_loop: %d\n", sum);

    // Test 20: ternary with complex conditions
    x = 42;
    int cat = (x > 0 && x < 10) ? 1 :
              (x >= 10 && x < 50) ? 2 :
              (x >= 50 && x < 100) ? 3 : 4;
    // EXPECT: ternary_cond: 2
    printf("ternary_cond: %d\n", cat);

    // Test 21: chained comparisons via AND
    int v = 5;
    r = (1 < v && v < 10);
    // EXPECT: chained_cmp: 1
    printf("chained_cmp: %d\n", r);

    // Test 22: De Morgan's law verification
    int p = 1;
    int q = 0;
    int dm1 = !(p && q);
    int dm2 = (!p || !q);
    // EXPECT: demorgan: 1 1
    printf("demorgan: %d %d\n", dm1, dm2);

    // Test 23: condition controlling multiple side effects
    global_counter = 0;
    int vals[5];
    vals[0] = 0; vals[1] = 1; vals[2] = 0; vals[3] = 1; vals[4] = 1;
    int true_count = 0;
    for (i = 0; i < 5; i = i + 1) {
        if (vals[i] && increment()) {
            true_count = true_count + 1;
        }
    }
    // vals[0]=0: short-circuit, no increment
    // vals[1]=1: increment called (gc=1), true_count=1
    // vals[2]=0: short-circuit, no increment
    // vals[3]=1: increment called (gc=2), true_count=2
    // vals[4]=1: increment called (gc=3), true_count=3
    // EXPECT: cond_effects: 3 3
    printf("cond_effects: %d %d\n", true_count, global_counter);

    return 0;
}
