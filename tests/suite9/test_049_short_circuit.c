int printf(const char *fmt, ...);

int call_count;

int track(int val) {
    call_count = call_count + 1;
    return val;
}

int safe_div(int a, int b) {
    return (b != 0 && a / b > 5) ? 1 : 0;
}

int in_range(int x, int lo, int hi) {
    return x >= lo && x <= hi;
}

int any_positive(int a, int b, int c) {
    return a > 0 || b > 0 || c > 0;
}

int all_nonzero(int a, int b, int c, int d) {
    return a && b && c && d;
}

int complex_pred(int x, int y) {
    return (x > 0 && y > 0) || (x < 0 && y < 0) || (x == 0 && y == 0);
}

void test_and_short_circuit() {
    call_count = 0;
    int r = track(0) && track(1) && track(1);
    printf("and sc result: %d, calls: %d\n", r, call_count);

    call_count = 0;
    r = track(1) && track(0) && track(1);
    printf("and sc2 result: %d, calls: %d\n", r, call_count);

    call_count = 0;
    r = track(1) && track(1) && track(1);
    printf("and sc3 result: %d, calls: %d\n", r, call_count);
}

void test_or_short_circuit() {
    call_count = 0;
    int r = track(1) || track(1) || track(1);
    printf("or sc result: %d, calls: %d\n", r, call_count);

    call_count = 0;
    r = track(0) || track(1) || track(1);
    printf("or sc2 result: %d, calls: %d\n", r, call_count);

    call_count = 0;
    r = track(0) || track(0) || track(0);
    printf("or sc3 result: %d, calls: %d\n", r, call_count);
}

void test_mixed_short_circuit() {
    call_count = 0;
    int r = (track(1) && track(0)) || (track(1) && track(1));
    printf("mixed sc result: %d, calls: %d\n", r, call_count);

    call_count = 0;
    r = (track(1) || track(0)) && (track(0) || track(1));
    printf("mixed sc2 result: %d, calls: %d\n", r, call_count);

    call_count = 0;
    r = (track(0) || track(0)) && (track(1) || track(0));
    printf("mixed sc3 result: %d, calls: %d\n", r, call_count);
}

void test_side_effect_accumulation() {
    int acc = 0;
    int i;
    for (i = 0; i < 10; i = i + 1) {
        if ((i > 2 && i < 7) || i == 9) {
            acc = acc + i;
        }
    }
    printf("accumulated: %d\n", acc);
}

void test_nested_conditions() {
    int vals[] = {3, -2, 0, 5, -1, 7, 0, 4};
    int pos_count = 0;
    int neg_count = 0;
    int zero_count = 0;
    int i;
    for (i = 0; i < 8; i = i + 1) {
        if (vals[i] > 0 && (vals[i] % 2 != 0 || vals[i] > 4)) {
            pos_count = pos_count + 1;
        } else if (vals[i] < 0 || (vals[i] == 0 && i > 5)) {
            neg_count = neg_count + 1;
        } else {
            zero_count = zero_count + 1;
        }
    }
    printf("pos: %d, neg: %d, zero: %d\n", pos_count, neg_count, zero_count);
}

int main() {
    // EXPECT: safe_div 30 5: 1
    printf("safe_div 30 5: %d\n", safe_div(30, 5));
    // EXPECT: safe_div 10 0: 0
    printf("safe_div 10 0: %d\n", safe_div(10, 0));
    // EXPECT: safe_div 3 1: 0
    printf("safe_div 3 1: %d\n", safe_div(3, 1));

    // EXPECT: range 5: 1
    printf("range 5: %d\n", in_range(5, 1, 10));
    // EXPECT: range 15: 0
    printf("range 15: %d\n", in_range(15, 1, 10));

    // EXPECT: any_pos -1 -2 3: 1
    printf("any_pos -1 -2 3: %d\n", any_positive(-1, -2, 3));
    // EXPECT: any_pos -1 -2 -3: 0
    printf("any_pos -1 -2 -3: %d\n", any_positive(-1, -2, -3));

    // EXPECT: all_nz 1 2 3 4: 1
    printf("all_nz 1 2 3 4: %d\n", all_nonzero(1, 2, 3, 4));
    // EXPECT: all_nz 1 0 3 4: 0
    printf("all_nz 1 0 3 4: %d\n", all_nonzero(1, 0, 3, 4));

    // EXPECT: complex 3 5: 1
    printf("complex 3 5: %d\n", complex_pred(3, 5));
    // EXPECT: complex -3 -5: 1
    printf("complex -3 -5: %d\n", complex_pred(-3, -5));
    // EXPECT: complex 0 0: 1
    printf("complex 0 0: %d\n", complex_pred(0, 0));
    // EXPECT: complex 3 -5: 0
    printf("complex 3 -5: %d\n", complex_pred(3, -5));

    // EXPECT: and sc result: 0, calls: 1
    // EXPECT: and sc2 result: 0, calls: 2
    // EXPECT: and sc3 result: 1, calls: 3
    test_and_short_circuit();
    // EXPECT: or sc result: 1, calls: 1
    // EXPECT: or sc2 result: 1, calls: 2
    // EXPECT: or sc3 result: 0, calls: 3
    test_or_short_circuit();
    // EXPECT: mixed sc result: 1, calls: 4
    // EXPECT: mixed sc2 result: 1, calls: 3
    // EXPECT: mixed sc3 result: 0, calls: 2
    test_mixed_short_circuit();
    // EXPECT: accumulated: 27
    test_side_effect_accumulation();
    // EXPECT: pos: 3, neg: 3, zero: 2
    test_nested_conditions();

    return 0;
}
