int printf(const char *fmt, ...);

int continue_inner(void) {
    int sum = 0;
    int i;
    int j;
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 10; j = j + 1) {
            if (j % 2 == 0) continue;
            sum = sum + 1;
        }
    }
    return sum;
}

int continue_outer(void) {
    int sum = 0;
    int i;
    int j;
    for (i = 0; i < 10; i = i + 1) {
        if (i % 3 == 0) continue;
        for (j = 0; j < 3; j = j + 1) {
            sum = sum + 1;
        }
    }
    return sum;
}

int continue_both(void) {
    int sum = 0;
    int i;
    int j;
    for (i = 0; i < 10; i = i + 1) {
        if (i % 2 == 0) continue;
        for (j = 0; j < 10; j = j + 1) {
            if (j % 3 == 0) continue;
            sum = sum + 1;
        }
    }
    return sum;
}

int continue_while(int n) {
    int sum = 0;
    int i = 0;
    while (i < n) {
        i = i + 1;
        if (i % 4 == 0) continue;
        sum = sum + i;
    }
    return sum;
}

int continue_do_while(int n) {
    int sum = 0;
    int i = 0;
    do {
        i = i + 1;
        if (i % 3 == 0) continue;
        sum = sum + i;
    } while (i < n);
    return sum;
}

int continue_with_side_effects(int n) {
    int skipped = 0;
    int processed = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (i % 2 == 0) {
            skipped = skipped + 1;
            continue;
        }
        processed = processed + 1;
    }
    return skipped * 100 + processed;
}

int continue_inc_still_runs(void) {
    int sum = 0;
    int i;
    for (i = 0; i < 20; i = i + 1) {
        if (i < 10) continue;
        sum = sum + i;
    }
    return sum;
}

int continue_in_while_nested(void) {
    int count = 0;
    int i = 0;
    while (i < 5) {
        i = i + 1;
        int j = 0;
        while (j < 5) {
            j = j + 1;
            if (j == 3) continue;
            count = count + 1;
        }
    }
    return count;
}

int continue_after_work(int n) {
    int total = 0;
    int extras = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        total = total + 1;
        if (i % 5 == 0) continue;
        extras = extras + 1;
    }
    return total * 100 + extras;
}

int continue_complex_cond(int n) {
    int sum = 0;
    int i;
    for (i = 1; i <= n; i = i + 1) {
        if (i % 2 == 0 && i % 3 == 0) continue;
        sum = sum + i;
    }
    return sum;
}

int continue_triple_nested(void) {
    int count = 0;
    int i;
    int j;
    int k;
    for (i = 0; i < 4; i = i + 1) {
        for (j = 0; j < 4; j = j + 1) {
            if (j == 2) continue;
            for (k = 0; k < 4; k = k + 1) {
                if (k == 1) continue;
                count = count + 1;
            }
        }
    }
    return count;
}

int main(void) {
    // EXPECT: inner: 25
    printf("inner: %d\n", continue_inner());
    // EXPECT: outer: 18
    printf("outer: %d\n", continue_outer());
    // EXPECT: both: 30
    printf("both: %d\n", continue_both());
    // EXPECT: while 12: 54
    printf("while 12: %d\n", continue_while(12));
    // EXPECT: dowhile 10: 37
    printf("dowhile 10: %d\n", continue_do_while(10));
    // EXPECT: side 10: 505
    printf("side 10: %d\n", continue_with_side_effects(10));
    // EXPECT: side 7: 403
    printf("side 7: %d\n", continue_with_side_effects(7));
    // EXPECT: inc_runs: 145
    printf("inc_runs: %d\n", continue_inc_still_runs());
    // EXPECT: while_nest: 20
    printf("while_nest: %d\n", continue_in_while_nested());
    // EXPECT: afterwork 20: 2016
    printf("afterwork 20: %d\n", continue_after_work(20));
    // EXPECT: complex 12: 60
    printf("complex 12: %d\n", continue_complex_cond(12));
    // EXPECT: complex 20: 174
    printf("complex 20: %d\n", continue_complex_cond(20));
    // EXPECT: triple: 36
    printf("triple: %d\n", continue_triple_nested());
    // EXPECT: while 20: 150
    printf("while 20: %d\n", continue_while(20));
    // EXPECT: dowhile 6: 12
    printf("dowhile 6: %d\n", continue_do_while(6));

    return 0;
}
