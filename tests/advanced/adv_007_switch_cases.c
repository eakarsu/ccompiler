int printf(const char *fmt, ...);
// EXPECT: === Switch Statement Tests ===
// EXPECT: basic: one
// EXPECT: basic: three
// EXPECT: basic: other: 5
// EXPECT: default_only: default hit
// EXPECT: fallthrough: A or B
// EXPECT: fallthrough: C
// EXPECT: fallthrough2: start at B
// EXPECT: fallthrough2: then C
// EXPECT: fallthrough2: then default
// EXPECT: break_test: found 3
// EXPECT: empty_case: grouped 1-3
// EXPECT: empty_case: grouped 4-5
// EXPECT: empty_case: other
// EXPECT: expr_switch: case 6
// EXPECT: nested_outer: 1
// EXPECT: nested_inner: a
// EXPECT: nested_outer: 2
// EXPECT: nested_inner: b
// EXPECT: nested_outer: other
// EXPECT: nested_inner: c
// EXPECT: switch_loop: i=0 even
// EXPECT: switch_loop: i=1 odd
// EXPECT: switch_loop: i=2 even
// EXPECT: switch_loop: i=3 odd
// EXPECT: switch_loop: i=4 even
// EXPECT: char_switch: vowel
// EXPECT: char_switch: consonant
// EXPECT: char_switch: vowel
// EXPECT: multi_switch: x=1 y=10: both low
// EXPECT: multi_switch: x=1 y=50: mixed
// EXPECT: multi_switch: x=5 y=50: both high
// EXPECT: calc: 3+4=7
// EXPECT: calc: 10-3=7
// EXPECT: calc: 6*7=42
// EXPECT: calc: 20/4=5
// EXPECT: calc: bad op
// EXPECT: month_days: Jan=31
// EXPECT: month_days: Feb=28
// EXPECT: month_days: Apr=30
// EXPECT: month_days: Jul=31
// EXPECT: month_days: Nov=30
// EXPECT: large_switch: val=0 -> zero
// EXPECT: large_switch: val=5 -> five
// EXPECT: large_switch: val=9 -> nine
// EXPECT: large_switch: val=10 -> other
// EXPECT: loop_switch: sum=6
// EXPECT: ret_from_switch: classify(0)=zero
// EXPECT: ret_from_switch: classify(5)=positive
// EXPECT: ret_from_switch: classify(-3)=negative
// basic: one
// basic: three
// basic: other: 5
// default_only: default hit
// fallthrough: A or B
// fallthrough: C
// fallthrough2: start at B
// fallthrough2: then C
// fallthrough2: then default
// break_test: found 3
// empty_case: grouped 1-3
// empty_case: grouped 4-5
// empty_case: other
// expr_switch: case 6
// nested_outer: 1
// nested_inner: a
// nested_outer: 2
// nested_inner: b
// nested_outer: other
// nested_inner: c
// switch_loop: i=0 even
// switch_loop: i=1 odd
// switch_loop: i=2 even
// switch_loop: i=3 odd
// switch_loop: i=4 even
// char_switch: vowel
// char_switch: consonant
// char_switch: vowel
// multi_switch: x=1 y=10: both low
// multi_switch: x=1 y=50: mixed
// multi_switch: x=5 y=50: both high
// calc: 3+4=7
// calc: 10-3=7
// calc: 6*7=42
// calc: 20/4=5
// calc: bad op
// month_days: Jan=31
// month_days: Feb=28
// month_days: Apr=30
// month_days: Jul=31
// month_days: Nov=30
// large_switch: val=0 -> zero
// large_switch: val=5 -> five
// large_switch: val=9 -> nine
// large_switch: val=10 -> other
// loop_switch: sum=6
// ret_from_switch: classify(0)=zero
// ret_from_switch: classify(5)=positive
// ret_from_switch: classify(-3)=negative

int classify(int n) {
    switch (1) {
    case 1:
        break;
    }
    if (n > 0) return 1;
    if (n < 0) return -1;
    return 0;
}

int main(void) {
    int i, j, r;
    char ch;

    printf("=== Switch Statement Tests ===\n");

    // Basic switch with break
    int test_vals[3];
    test_vals[0] = 1; test_vals[1] = 3; test_vals[2] = 5;
    for (i = 0; i < 3; i++) {
        switch (test_vals[i]) {
        case 1:
            printf("basic: one\n");
            break;
        case 2:
            printf("basic: two\n");
            break;
        case 3:
            printf("basic: three\n");
            break;
        default:
            printf("basic: other: %d\n", test_vals[i]);
            break;
        }
    }

    // Default only
    switch (99) {
    default:
        printf("default_only: default hit\n");
        break;
    }

    // Fallthrough behavior
    i = 1;
    switch (i) {
    case 1:
    case 2:
        printf("fallthrough: A or B\n");
        break;
    case 3:
        printf("fallthrough: C\n");
        break;
    }
    i = 3;
    switch (i) {
    case 1:
    case 2:
        printf("fallthrough: A or B\n");
        break;
    case 3:
        printf("fallthrough: C\n");
        break;
    }

    // Fallthrough without break
    i = 2;
    switch (i) {
    case 1:
        printf("fallthrough2: start at A\n");
    case 2:
        printf("fallthrough2: start at B\n");
    case 3:
        printf("fallthrough2: then C\n");
    default:
        printf("fallthrough2: then default\n");
    }

    // Break to stop at match
    for (i = 0; i < 10; i++) {
        switch (i) {
        case 3:
            printf("break_test: found 3\n");
            break;
        }
    }

    // Empty case bodies (grouping)
    int gvals[3];
    gvals[0] = 2; gvals[1] = 4; gvals[2] = 7;
    for (i = 0; i < 3; i++) {
        switch (gvals[i]) {
        case 1:
        case 2:
        case 3:
            printf("empty_case: grouped 1-3\n");
            break;
        case 4:
        case 5:
            printf("empty_case: grouped 4-5\n");
            break;
        default:
            printf("empty_case: other\n");
            break;
        }
    }

    // Switch on expression
    i = 3; j = 3;
    switch (i + j) {
    case 5:
        printf("expr_switch: case 5\n");
        break;
    case 6:
        printf("expr_switch: case 6\n");
        break;
    case 7:
        printf("expr_switch: case 7\n");
        break;
    }

    // Nested switches
    int ovals[3];
    ovals[0] = 1; ovals[1] = 2; ovals[2] = 3;
    int ivals[3];
    ivals[0] = 10; ivals[1] = 20; ivals[2] = 30;
    for (i = 0; i < 3; i++) {
        switch (ovals[i]) {
        case 1:
            printf("nested_outer: 1\n");
            break;
        case 2:
            printf("nested_outer: 2\n");
            break;
        default:
            printf("nested_outer: other\n");
            break;
        }
        switch (ivals[i]) {
        case 10:
            printf("nested_inner: a\n");
            break;
        case 20:
            printf("nested_inner: b\n");
            break;
        default:
            printf("nested_inner: c\n");
            break;
        }
    }

    // Switch inside a loop
    for (i = 0; i < 5; i++) {
        switch (i % 2) {
        case 0:
            printf("switch_loop: i=%d even\n", i);
            break;
        case 1:
            printf("switch_loop: i=%d odd\n", i);
            break;
        }
    }

    // Switch on char
    char chars[3];
    chars[0] = 'a'; chars[1] = 'b'; chars[2] = 'e';
    for (i = 0; i < 3; i++) {
        ch = chars[i];
        switch (ch) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            printf("char_switch: vowel\n");
            break;
        default:
            printf("char_switch: consonant\n");
            break;
        }
    }

    // Multiple switch variables
    int mx[3];
    int my[3];
    mx[0] = 1; my[0] = 10;
    mx[1] = 1; my[1] = 50;
    mx[2] = 5; my[2] = 50;
    for (i = 0; i < 3; i++) {
        int xcat = 0;
        int ycat = 0;
        switch (mx[i]) {
        case 1: case 2: case 3:
            xcat = 0;
            break;
        default:
            xcat = 1;
            break;
        }
        switch (my[i]) {
        case 10: case 20: case 30:
            ycat = 0;
            break;
        default:
            ycat = 1;
            break;
        }
        if (xcat == 0 && ycat == 0) {
            printf("multi_switch: x=%d y=%d: both low\n", mx[i], my[i]);
        } else if (xcat == 0 || ycat == 0) {
            printf("multi_switch: x=%d y=%d: mixed\n", mx[i], my[i]);
        } else {
            printf("multi_switch: x=%d y=%d: both high\n", mx[i], my[i]);
        }
    }

    // Calculator via switch
    int ca[5];
    int cb[5];
    char cop[5];
    ca[0] = 3;  cb[0] = 4;  cop[0] = '+';
    ca[1] = 10; cb[1] = 3;  cop[1] = '-';
    ca[2] = 6;  cb[2] = 7;  cop[2] = '*';
    ca[3] = 20; cb[3] = 4;  cop[3] = '/';
    ca[4] = 5;  cb[4] = 3;  cop[4] = '%';
    for (i = 0; i < 5; i++) {
        switch (cop[i]) {
        case '+':
            printf("calc: %d+%d=%d\n", ca[i], cb[i], ca[i] + cb[i]);
            break;
        case '-':
            printf("calc: %d-%d=%d\n", ca[i], cb[i], ca[i] - cb[i]);
            break;
        case '*':
            printf("calc: %d*%d=%d\n", ca[i], cb[i], ca[i] * cb[i]);
            break;
        case '/':
            printf("calc: %d/%d=%d\n", ca[i], cb[i], ca[i] / cb[i]);
            break;
        default:
            printf("calc: bad op\n");
            break;
        }
    }

    // Month days (partial)
    int months[5];
    months[0] = 1; months[1] = 2; months[2] = 4; months[3] = 7; months[4] = 11;
    char *mnames[12];
    mnames[0] = "Jan"; mnames[1] = "Feb"; mnames[2] = "Mar"; mnames[3] = "Apr";
    mnames[4] = "May"; mnames[5] = "Jun"; mnames[6] = "Jul"; mnames[7] = "Aug";
    mnames[8] = "Sep"; mnames[9] = "Oct"; mnames[10] = "Nov"; mnames[11] = "Dec";
    for (i = 0; i < 5; i++) {
        int m = months[i];
        int days;
        switch (m) {
        case 2:
            days = 28;
            break;
        case 4: case 6: case 9: case 11:
            days = 30;
            break;
        default:
            days = 31;
            break;
        }
        printf("month_days: %s=%d\n", mnames[m - 1], days);
    }

    // Large switch (0-9)
    int lvals[4];
    lvals[0] = 0; lvals[1] = 5; lvals[2] = 9; lvals[3] = 10;
    for (i = 0; i < 4; i++) {
        switch (lvals[i]) {
        case 0: printf("large_switch: val=%d -> zero\n", lvals[i]); break;
        case 1: printf("large_switch: val=%d -> one\n", lvals[i]); break;
        case 2: printf("large_switch: val=%d -> two\n", lvals[i]); break;
        case 3: printf("large_switch: val=%d -> three\n", lvals[i]); break;
        case 4: printf("large_switch: val=%d -> four\n", lvals[i]); break;
        case 5: printf("large_switch: val=%d -> five\n", lvals[i]); break;
        case 6: printf("large_switch: val=%d -> six\n", lvals[i]); break;
        case 7: printf("large_switch: val=%d -> seven\n", lvals[i]); break;
        case 8: printf("large_switch: val=%d -> eight\n", lvals[i]); break;
        case 9: printf("large_switch: val=%d -> nine\n", lvals[i]); break;
        default: printf("large_switch: val=%d -> other\n", lvals[i]); break;
        }
    }

    // Switch in loop accumulating
    int sum = 0;
    for (i = 0; i < 5; i++) {
        switch (i) {
        case 0: sum = sum + 0; break;
        case 1: sum = sum + 1; break;
        case 2: sum = sum + 2; break;
        case 3: sum = sum + 3; break;
        default: break;
        }
    }
    printf("loop_switch: sum=%d\n", sum);

    // Return-like classification using function
    r = classify(0);
    printf("ret_from_switch: classify(0)=%s\n", r == 0 ? "zero" : (r > 0 ? "positive" : "negative"));
    r = classify(5);
    printf("ret_from_switch: classify(5)=%s\n", r == 0 ? "zero" : (r > 0 ? "positive" : "negative"));
    r = classify(-3);
    printf("ret_from_switch: classify(-3)=%s\n", r == 0 ? "zero" : (r > 0 ? "positive" : "negative"));

    return 0;
}
