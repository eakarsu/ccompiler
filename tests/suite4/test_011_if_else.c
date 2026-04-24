int printf(const char *fmt, ...);

int abs_val(int x) {
    if (x < 0)
        return -x;
    return x;
}

int clamp(int x, int lo, int hi) {
    if (x < lo)
        return lo;
    else if (x > hi)
        return hi;
    else
        return x;
}

int sign(int x) {
    if (x > 0)
        return 1;
    else if (x < 0)
        return -1;
    else
        return 0;
}

int classify_age(int age) {
    if (age < 0)
        return -1;
    else if (age < 13)
        return 0;
    else if (age < 18)
        return 1;
    else if (age < 65)
        return 2;
    else
        return 3;
}

int max3(int a, int b, int c) {
    int m = a;
    if (b > m) m = b;
    if (c > m) m = c;
    return m;
}

int min3(int a, int b, int c) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

int main(void) {
    // Test 1: simple if true
    int x = 10;
    if (x > 5) {
        // EXPECT: if_true: 10
        printf("if_true: %d\n", x);
    }

    // Test 2: simple if false (no else, nothing prints)
    int printed = 0;
    if (x < 0) {
        printed = 1;
    }
    // EXPECT: not_printed: 0
    printf("not_printed: %d\n", printed);

    // Test 3: if-else
    int y = 3;
    if (y > 5) {
        y = 100;
    } else {
        y = 200;
    }
    // EXPECT: if_else: 200
    printf("if_else: %d\n", y);

    // Test 4: if-else chain
    int grade = 85;
    int letter;
    if (grade >= 90)
        letter = 65; // 'A'
    else if (grade >= 80)
        letter = 66; // 'B'
    else if (grade >= 70)
        letter = 67; // 'C'
    else
        letter = 68; // 'D'
    // EXPECT: grade_letter: 66
    printf("grade_letter: %d\n", letter);

    // Test 5: nested if-else
    int a = 10;
    int b = 20;
    int result;
    if (a > 5) {
        if (b > 15) {
            result = 1;
        } else {
            result = 2;
        }
    } else {
        if (b > 15) {
            result = 3;
        } else {
            result = 4;
        }
    }
    // EXPECT: nested_if: 1
    printf("nested_if: %d\n", result);

    // Test 6: abs_val function
    // EXPECT: abs_neg: 42
    printf("abs_neg: %d\n", abs_val(-42));
    // EXPECT: abs_pos: 7
    printf("abs_pos: %d\n", abs_val(7));
    // EXPECT: abs_zero: 0
    printf("abs_zero: %d\n", abs_val(0));

    // Test 7: clamp function
    // EXPECT: clamp_low: 0
    printf("clamp_low: %d\n", clamp(-5, 0, 100));
    // EXPECT: clamp_high: 100
    printf("clamp_high: %d\n", clamp(200, 0, 100));
    // EXPECT: clamp_mid: 50
    printf("clamp_mid: %d\n", clamp(50, 0, 100));

    // Test 8: sign function
    // EXPECT: sign_pos: 1
    printf("sign_pos: %d\n", sign(99));
    // EXPECT: sign_neg: -1
    printf("sign_neg: %d\n", sign(-42));
    // EXPECT: sign_zero: 0
    printf("sign_zero: %d\n", sign(0));

    // Test 9: classify_age
    // EXPECT: age_child: 0
    printf("age_child: %d\n", classify_age(5));
    // EXPECT: age_teen: 1
    printf("age_teen: %d\n", classify_age(15));
    // EXPECT: age_adult: 2
    printf("age_adult: %d\n", classify_age(30));
    // EXPECT: age_senior: 3
    printf("age_senior: %d\n", classify_age(70));

    // Test 10: max3 and min3
    // EXPECT: max3: 30
    printf("max3: %d\n", max3(10, 30, 20));
    // EXPECT: min3: 10
    printf("min3: %d\n", min3(10, 30, 20));

    // Test 11: complex condition in if
    int p = 5;
    int q = 10;
    if (p > 0 && q > 0 && p + q == 15) {
        // EXPECT: complex_cond: yes
        printf("complex_cond: %s\n", "yes");
    }

    // Test 12: if with logical or
    int r = -1;
    if (r < 0 || r > 100) {
        // EXPECT: or_cond: -1
        printf("or_cond: %d\n", r);
    }

    // Test 13: deeply nested if
    int d = 1;
    if (d > 0) {
        if (d < 10) {
            if (d == 1) {
                if (d + 1 == 2) {
                    // EXPECT: deep_nest: 1
                    printf("deep_nest: %d\n", d);
                }
            }
        }
    }

    // Test 14: if without braces, single statement
    int flag = 0;
    if (1 > 0)
        flag = 1;
    // EXPECT: no_brace: 1
    printf("no_brace: %d\n", flag);

    // Test 15: chained ternary (which is also if-else)
    int v = 50;
    int cat = (v < 25) ? 0 : (v < 50) ? 1 : (v < 75) ? 2 : 3;
    // EXPECT: ternary_chain: 2
    printf("ternary_chain: %d\n", cat);

    return 0;
}
