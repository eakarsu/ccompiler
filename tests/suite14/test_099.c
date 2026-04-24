int printf(const char *fmt, ...);

// Test 099: Complex switch/case patterns (fallthrough, nested)

void test_basic_fallthrough(void) {
    int x = 2;
    int result = 0;

    switch (x) {
    case 1:
        result = result + 1;
        // fallthrough
    case 2:
        result = result + 2;
        // fallthrough
    case 3:
        result = result + 4;
        // fallthrough
    case 4:
        result = result + 8;
        break;
    }
    // x=2: hits case 2 (+2), falls to case 3 (+4), falls to case 4 (+8) = 14
    printf("%d\n", result);
    // EXPECT: 14
}

void test_fallthrough_from_top(void) {
    int result = 0;
    int x = 1;

    switch (x) {
    case 1:
        result = result + 1;
    case 2:
        result = result + 2;
    case 3:
        result = result + 4;
    }
    // x=1: all three hit. 1+2+4 = 7
    printf("%d\n", result);
    // EXPECT: 7
}

void test_default_fallthrough(void) {
    int x = 99;
    int result = 0;

    switch (x) {
    case 1:
        result = 10;
        break;
    default:
        result = 50;
        // fallthrough
    case 2:
        result = result + 5;
        break;
    }
    // x=99: hits default (result=50), falls to case 2 (result=55)
    printf("%d\n", result);
    // EXPECT: 55
}

void test_nested_switch(void) {
    int a = 2;
    int b = 1;
    int result = 0;

    switch (a) {
    case 1:
        result = 10;
        break;
    case 2:
        switch (b) {
        case 1:
            result = 21;
            break;
        case 2:
            result = 22;
            break;
        }
        break;
    case 3:
        result = 30;
        break;
    }
    printf("%d\n", result);
    // EXPECT: 21
}

void test_switch_in_loop(void) {
    int sum = 0;
    int i;

    for (i = 0; i < 6; i++) {
        switch (i) {
        case 0:
            sum = sum + 1;
            break;
        case 1:
            sum = sum + 10;
            break;
        case 2:
            sum = sum + 100;
            break;
        default:
            sum = sum + 1000;
            break;
        }
    }
    // i=0: +1, i=1: +10, i=2: +100, i=3: +1000, i=4: +1000, i=5: +1000
    // total = 3111
    printf("%d\n", sum);
    // EXPECT: 3111
}

void test_case_grouping(void) {
    int results[6];
    int i;

    for (i = 0; i < 6; i++) {
        switch (i) {
        case 0:
        case 1:
        case 2:
            results[i] = 1;
            break;
        case 3:
        case 4:
            results[i] = 2;
            break;
        default:
            results[i] = 3;
            break;
        }
    }

    for (i = 0; i < 6; i++) {
        printf("%d\n", results[i]);
    }
    // EXPECT: 1
    // EXPECT: 1
    // EXPECT: 1
    // EXPECT: 2
    // EXPECT: 2
    // EXPECT: 3
}

void test_switch_with_computation(void) {
    int val = 3;
    int result;

    switch (val * 2 - 1) {  // switch on expression: 3*2-1 = 5
    case 3:
        result = 30;
        break;
    case 5:
        result = 50;
        break;
    case 7:
        result = 70;
        break;
    default:
        result = -1;
        break;
    }
    printf("%d\n", result);
    // EXPECT: 50
}

void test_switch_negative(void) {
    int x = -2;
    int result;

    switch (x) {
    case -3:
        result = 1;
        break;
    case -2:
        result = 2;
        break;
    case -1:
        result = 3;
        break;
    case 0:
        result = 4;
        break;
    default:
        result = 5;
        break;
    }
    printf("%d\n", result);
    // EXPECT: 2
}

void test_partial_fallthrough(void) {
    int x;
    int a_count = 0;
    int b_count = 0;
    int c_count = 0;

    for (x = 1; x <= 5; x++) {
        switch (x) {
        case 1:
        case 3:
        case 5:
            a_count++;
            break;
        case 2:
            b_count++;
            // fallthrough
        case 4:
            c_count++;
            break;
        }
    }
    // x=1: a_count=1
    // x=2: b_count=1, c_count=1 (fallthrough)
    // x=3: a_count=2
    // x=4: c_count=2
    // x=5: a_count=3
    printf("%d\n", a_count);
    // EXPECT: 3
    printf("%d\n", b_count);
    // EXPECT: 1
    printf("%d\n", c_count);
    // EXPECT: 2
}

void test_nested_switch_fallthrough(void) {
    int outer = 1;
    int inner = 2;
    int result = 0;

    switch (outer) {
    case 1:
        result = result + 100;
        switch (inner) {
        case 1:
            result = result + 10;
            break;
        case 2:
            result = result + 20;
            // fallthrough into case 3
        case 3:
            result = result + 30;
            break;
        }
        // break from inner switch; continue in outer case 1
        result = result + 1;
        break;
    case 2:
        result = result + 200;
        break;
    }
    // outer=1: +100, inner=2: +20, falls to inner case 3: +30, then +1
    // total = 100+20+30+1 = 151
    printf("%d\n", result);
    // EXPECT: 151
}

int main(void) {
    test_basic_fallthrough();
    test_fallthrough_from_top();
    test_default_fallthrough();
    test_nested_switch();
    test_switch_in_loop();
    test_case_grouping();
    test_switch_with_computation();
    test_switch_negative();
    test_partial_fallthrough();
    test_nested_switch_fallthrough();
    return 0;
}
