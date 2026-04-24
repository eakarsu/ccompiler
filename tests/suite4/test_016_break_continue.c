int printf(const char *fmt, ...);

int find_first(int *arr, int len, int target) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (arr[i] == target) return i;
    }
    return -1;
}

int sum_until_negative(int *arr, int len) {
    int s = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (arr[i] < 0) break;
        s = s + arr[i];
    }
    return s;
}

int sum_positive_only(int *arr, int len) {
    int s = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (arr[i] <= 0) continue;
        s = s + arr[i];
    }
    return s;
}

int main(void) {
    // Test 1: break in for loop
    int i;
    int stopped_at = -1;
    for (i = 0; i < 100; i = i + 1) {
        if (i == 7) {
            stopped_at = i;
            break;
        }
    }
    // EXPECT: for_break: 7
    printf("for_break: %d\n", stopped_at);

    // Test 2: continue in for loop - sum odd numbers
    int odd_sum = 0;
    for (i = 0; i < 20; i = i + 1) {
        if (i % 2 == 0) continue;
        odd_sum = odd_sum + i;
    }
    // 1+3+5+7+9+11+13+15+17+19 = 100
    // EXPECT: for_continue: 100
    printf("for_continue: %d\n", odd_sum);

    // Test 3: break in while loop
    i = 0;
    while (1) {
        if (i >= 10) break;
        i = i + 1;
    }
    // EXPECT: while_break: 10
    printf("while_break: %d\n", i);

    // Test 4: continue in while loop
    int skip_count = 0;
    i = 0;
    while (i < 15) {
        i = i + 1;
        if (i % 5 == 0) continue;
        skip_count = skip_count + 1;
    }
    // 15 iterations, skip i=5,10,15 -> 12 counted
    // EXPECT: while_continue: 12
    printf("while_continue: %d\n", skip_count);

    // Test 5: break in do-while
    i = 0;
    do {
        if (i == 3) break;
        i = i + 1;
    } while (i < 100);
    // EXPECT: do_break: 3
    printf("do_break: %d\n", i);

    // Test 6: continue in do-while
    int dw_sum = 0;
    i = 0;
    do {
        i = i + 1;
        if (i % 2 == 0) continue;
        dw_sum = dw_sum + i;
    } while (i < 10);
    // odd: 1+3+5+7+9 = 25
    // EXPECT: do_continue: 25
    printf("do_continue: %d\n", dw_sum);

    // Test 7: break in nested loop (only breaks inner)
    int outer_count = 0;
    int inner_total = 0;
    int j;
    for (i = 0; i < 5; i = i + 1) {
        outer_count = outer_count + 1;
        for (j = 0; j < 100; j = j + 1) {
            if (j == 3) break;
            inner_total = inner_total + 1;
        }
    }
    // outer runs 5 times, inner breaks at j=3 each time -> 5*3 = 15
    // EXPECT: nested_break_outer: 5
    printf("nested_break_outer: %d\n", outer_count);
    // EXPECT: nested_break_inner: 15
    printf("nested_break_inner: %d\n", inner_total);

    // Test 8: continue in nested loop (only affects inner)
    int inner_sum = 0;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 10; j = j + 1) {
            if (j % 3 == 0) continue;
            inner_sum = inner_sum + 1;
        }
    }
    // inner: skip j=0,3,6,9 -> 6 counted per outer iteration, 3*6=18
    // EXPECT: nested_continue: 18
    printf("nested_continue: %d\n", inner_sum);

    // Test 9: break out of outer loop using flag
    int found_i = -1;
    int found_j = -1;
    int done = 0;
    for (i = 0; i < 10 && !done; i = i + 1) {
        for (j = 0; j < 10 && !done; j = j + 1) {
            if (i * 10 + j == 37) {
                found_i = i;
                found_j = j;
                done = 1;
            }
        }
    }
    // EXPECT: found_pos: 3 7
    printf("found_pos: %d %d\n", found_i, found_j);

    // Test 10: find_first function
    int data[8];
    data[0] = 10; data[1] = 20; data[2] = 30; data[3] = 40;
    data[4] = 50; data[5] = 30; data[6] = 60; data[7] = 70;
    // EXPECT: find_30: 2
    printf("find_30: %d\n", find_first(data, 8, 30));
    // EXPECT: find_99: -1
    printf("find_99: %d\n", find_first(data, 8, 99));

    // Test 11: sum_until_negative
    int data2[6];
    data2[0] = 5; data2[1] = 10; data2[2] = 3; data2[3] = -1;
    data2[4] = 100; data2[5] = 200;
    // EXPECT: sum_until_neg: 18
    printf("sum_until_neg: %d\n", sum_until_negative(data2, 6));

    // Test 12: sum_positive_only
    int data3[6];
    data3[0] = 5; data3[1] = -2; data3[2] = 10; data3[3] = 0;
    data3[4] = -7; data3[5] = 3;
    // 5+10+3 = 18
    // EXPECT: sum_pos: 18
    printf("sum_pos: %d\n", sum_positive_only(data3, 6));

    // Test 13: multiple breaks in switch inside loop
    int category_sum = 0;
    for (i = 0; i < 10; i = i + 1) {
        int cat;
        switch (i % 3) {
        case 0: cat = 1; break;
        case 1: cat = 2; break;
        default: cat = 3; break;
        }
        category_sum = category_sum + cat;
    }
    // i%3: 0,1,2,0,1,2,0,1,2,0 -> cats: 1,2,3,1,2,3,1,2,3,1 -> sum=19
    // 4*1 + 3*2 + 3*3 = 4+6+9 = 19
    // EXPECT: switch_in_loop: 19
    printf("switch_in_loop: %d\n", category_sum);

    // Test 14: break in while inside for
    int while_breaks = 0;
    for (i = 0; i < 5; i = i + 1) {
        j = 0;
        while (1) {
            if (j >= i + 1) break;
            j = j + 1;
            while_breaks = while_breaks + 1;
        }
    }
    // i=0: j loops 1 time; i=1: 2; i=2: 3; i=3: 4; i=4: 5 -> total=15
    // EXPECT: while_in_for: 15
    printf("while_in_for: %d\n", while_breaks);

    // Test 15: continue with accumulation pattern
    int filtered = 0;
    for (i = 1; i <= 20; i = i + 1) {
        if (i % 2 == 0) continue;
        if (i % 5 == 0) continue;
        filtered = filtered + i;
    }
    // odd and not multiple of 5: 1,3,7,9,11,13,17,19 = 80
    // EXPECT: filtered: 80
    printf("filtered: %d\n", filtered);

    return 0;
}
