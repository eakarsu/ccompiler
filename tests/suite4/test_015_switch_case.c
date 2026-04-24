int printf(const char *fmt, ...);

int day_type(int day) {
    int result;
    switch (day) {
    case 0:
    case 6:
        result = 0; // weekend
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        result = 1; // weekday
        break;
    default:
        result = -1;
        break;
    }
    return result;
}

int month_days(int month) {
    int days;
    switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        days = 31;
        break;
    case 4: case 6: case 9: case 11:
        days = 30;
        break;
    case 2:
        days = 28;
        break;
    default:
        days = -1;
        break;
    }
    return days;
}

int classify_char(int c) {
    // Return: 1=digit, 2=lower vowel, 3=lower consonant, 0=other
    switch (c) {
    case 48: case 49: case 50: case 51: case 52:
    case 53: case 54: case 55: case 56: case 57:
        return 1;
    case 97: case 101: case 105: case 111: case 117: // a e i o u
        return 2;
    case 98: case 99: case 100: case 102: case 103:
    case 104: case 106: case 107: case 108: case 109:
    case 110: case 112: case 113: case 114: case 115:
    case 116: case 118: case 119: case 120: case 121:
    case 122:
        return 3;
    default:
        return 0;
    }
}

int main(void) {
    // Test 1: basic switch
    int x = 2;
    int result = 0;
    switch (x) {
    case 1: result = 10; break;
    case 2: result = 20; break;
    case 3: result = 30; break;
    default: result = -1; break;
    }
    // EXPECT: basic_switch: 20
    printf("basic_switch: %d\n", result);

    // Test 2: switch with default
    x = 99;
    switch (x) {
    case 1: result = 10; break;
    case 2: result = 20; break;
    default: result = -1; break;
    }
    // EXPECT: default_case: -1
    printf("default_case: %d\n", result);

    // Test 3: switch fallthrough (no break)
    int fall = 0;
    x = 1;
    switch (x) {
    case 1: fall = fall + 1;
    case 2: fall = fall + 10;
    case 3: fall = fall + 100;
        break;
    case 4: fall = fall + 1000;
        break;
    }
    // Falls through 1->2->3: 1+10+100 = 111
    // EXPECT: fallthrough: 111
    printf("fallthrough: %d\n", fall);

    // Test 4: switch fallthrough from middle
    fall = 0;
    x = 2;
    switch (x) {
    case 1: fall = fall + 1;
    case 2: fall = fall + 10;
    case 3: fall = fall + 100;
        break;
    case 4: fall = fall + 1000;
        break;
    }
    // Falls through 2->3: 10+100 = 110
    // EXPECT: fallthrough2: 110
    printf("fallthrough2: %d\n", fall);

    // Test 5: switch with only default
    x = 42;
    result = 0;
    switch (x) {
    default:
        result = 999;
        break;
    }
    // EXPECT: only_default: 999
    printf("only_default: %d\n", result);

    // Test 6: switch with no matching case and no default
    x = 50;
    result = 77;
    switch (x) {
    case 1: result = 1; break;
    case 2: result = 2; break;
    }
    // EXPECT: no_match: 77
    printf("no_match: %d\n", result);

    // Test 7: nested switch
    int outer = 1;
    int inner = 2;
    result = 0;
    switch (outer) {
    case 1:
        switch (inner) {
        case 1: result = 11; break;
        case 2: result = 12; break;
        case 3: result = 13; break;
        }
        break;
    case 2:
        switch (inner) {
        case 1: result = 21; break;
        case 2: result = 22; break;
        }
        break;
    }
    // EXPECT: nested_switch: 12
    printf("nested_switch: %d\n", result);

    // Test 8: day_type function
    // EXPECT: day_sat: 0
    printf("day_sat: %d\n", day_type(6));
    // EXPECT: day_mon: 1
    printf("day_mon: %d\n", day_type(1));
    // EXPECT: day_invalid: -1
    printf("day_invalid: %d\n", day_type(7));

    // Test 9: month_days function
    // EXPECT: jan_days: 31
    printf("jan_days: %d\n", month_days(1));
    // EXPECT: apr_days: 30
    printf("apr_days: %d\n", month_days(4));
    // EXPECT: feb_days: 28
    printf("feb_days: %d\n", month_days(2));

    // Test 10: classify_char
    // 48 = '0'
    // EXPECT: char_digit: 1
    printf("char_digit: %d\n", classify_char(48));
    // 97 = 'a'
    // EXPECT: char_vowel: 2
    printf("char_vowel: %d\n", classify_char(97));
    // 98 = 'b'
    // EXPECT: char_consonant: 3
    printf("char_consonant: %d\n", classify_char(98));
    // 64 = '@'
    // EXPECT: char_other: 0
    printf("char_other: %d\n", classify_char(64));

    // Test 11: switch in a loop
    int counts[4];
    counts[0] = 0; counts[1] = 0; counts[2] = 0; counts[3] = 0;
    int data[8];
    data[0] = 0; data[1] = 1; data[2] = 2; data[3] = 1;
    data[4] = 0; data[5] = 3; data[6] = 2; data[7] = 1;
    int i;
    for (i = 0; i < 8; i = i + 1) {
        switch (data[i]) {
        case 0: counts[0] = counts[0] + 1; break;
        case 1: counts[1] = counts[1] + 1; break;
        case 2: counts[2] = counts[2] + 1; break;
        case 3: counts[3] = counts[3] + 1; break;
        }
    }
    // data has: two 0s, three 1s, two 2s, one 3
    // EXPECT: freq: 2 3 2 1
    printf("freq: %d %d %d %d\n", counts[0], counts[1], counts[2], counts[3]);

    // Test 12: switch with expression
    int a = 3;
    int b = 4;
    switch (a + b) {
    case 5: result = 5; break;
    case 6: result = 6; break;
    case 7: result = 7; break;
    case 8: result = 8; break;
    default: result = 0; break;
    }
    // EXPECT: switch_expr: 7
    printf("switch_expr: %d\n", result);

    return 0;
}
