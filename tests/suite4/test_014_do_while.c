int printf(const char *fmt, ...);

int count_digits_dw(int n) {
    if (n < 0) n = -n;
    int c = 0;
    do {
        c = c + 1;
        n = n / 10;
    } while (n > 0);
    return c;
}

int sum_digits_dw(int n) {
    if (n < 0) n = -n;
    int s = 0;
    do {
        s = s + n % 10;
        n = n / 10;
    } while (n > 0);
    return s;
}

int reverse_dw(int n) {
    int rev = 0;
    do {
        rev = rev * 10 + n % 10;
        n = n / 10;
    } while (n > 0);
    return rev;
}

int int_log2(int n) {
    int result = 0;
    do {
        n = n / 2;
        if (n > 0) result = result + 1;
    } while (n > 1);
    return result;
}

int main(void) {
    // Test 1: do-while runs at least once
    int ran = 0;
    do {
        ran = 1;
    } while (0);
    // EXPECT: runs_once: 1
    printf("runs_once: %d\n", ran);

    // Test 2: do-while with false condition from start
    int val = 0;
    do {
        val = 42;
    } while (val < 0);
    // EXPECT: false_start: 42
    printf("false_start: %d\n", val);

    // Test 3: do-while counting
    int count = 0;
    int i = 0;
    do {
        count = count + 1;
        i = i + 1;
    } while (i < 10);
    // EXPECT: do_count: 10
    printf("do_count: %d\n", count);

    // Test 4: do-while sum
    int sum = 0;
    i = 1;
    do {
        sum = sum + i;
        i = i + 1;
    } while (i <= 10);
    // EXPECT: do_sum: 55
    printf("do_sum: %d\n", sum);

    // Test 5: do-while with break
    i = 0;
    do {
        if (i == 5) break;
        i = i + 1;
    } while (i < 100);
    // EXPECT: do_break: 5
    printf("do_break: %d\n", i);

    // Test 6: do-while with continue
    int skip_sum = 0;
    i = 0;
    do {
        i = i + 1;
        if (i % 3 == 0) continue;
        skip_sum = skip_sum + i;
    } while (i < 9);
    // i: 1,2,3(skip),4,5,6(skip),7,8,9(skip) -> 1+2+4+5+7+8 = 27
    // EXPECT: do_continue: 27
    printf("do_continue: %d\n", skip_sum);

    // Test 7: nested do-while
    int outer = 0;
    int total = 0;
    do {
        int inner = 0;
        do {
            total = total + 1;
            inner = inner + 1;
        } while (inner < 3);
        outer = outer + 1;
    } while (outer < 4);
    // 4 * 3 = 12
    // EXPECT: nested_dw: 12
    printf("nested_dw: %d\n", total);

    // Test 8: count_digits_dw
    // EXPECT: digits_0: 1
    printf("digits_0: %d\n", count_digits_dw(0));
    // EXPECT: digits_5: 1
    printf("digits_5: %d\n", count_digits_dw(5));
    // EXPECT: digits_99: 2
    printf("digits_99: %d\n", count_digits_dw(99));
    // EXPECT: digits_1000: 4
    printf("digits_1000: %d\n", count_digits_dw(1000));

    // Test 9: sum_digits_dw
    // EXPECT: dsum_456: 15
    printf("dsum_456: %d\n", sum_digits_dw(456));
    // EXPECT: dsum_0: 0
    printf("dsum_0: %d\n", sum_digits_dw(0));

    // Test 10: reverse_dw
    // EXPECT: rev_1234: 4321
    printf("rev_1234: %d\n", reverse_dw(1234));

    // Test 11: int_log2
    // EXPECT: log2_8: 3
    printf("log2_8: %d\n", int_log2(8));
    // EXPECT: log2_16: 4
    printf("log2_16: %d\n", int_log2(16));

    // Test 12: do-while as input validation simulation
    int attempts = 0;
    int valid = 0;
    int test_vals[5];
    test_vals[0] = -1;
    test_vals[1] = -5;
    test_vals[2] = 0;
    test_vals[3] = 42;
    test_vals[4] = 10;
    do {
        if (test_vals[attempts] > 0) {
            valid = test_vals[attempts];
        }
        attempts = attempts + 1;
    } while (valid == 0 && attempts < 5);
    // First positive is test_vals[3] = 42, attempts will be 4
    // EXPECT: valid_val: 42
    printf("valid_val: %d\n", valid);
    // EXPECT: attempts: 4
    printf("attempts: %d\n", attempts);

    // Test 13: do-while computing power of 2 >= n
    int n = 50;
    int p = 1;
    do {
        p = p * 2;
    } while (p < n);
    // 1->2->4->8->16->32->64 (>=50, stop)
    // EXPECT: next_pow2: 64
    printf("next_pow2: %d\n", p);

    // Test 14: do-while with complex condition
    int x = 0;
    int y = 100;
    do {
        x = x + 3;
        y = y - 7;
    } while (x < y && y > 0);
    // x:3,y:93 | x:6,y:86 | x:9,y:79 | x:12,y:72 | x:15,y:65 | x:18,y:58
    // x:21,y:51 | x:24,y:44 | x:27,y:37 | x:30,y:30 (x<y false)
    // EXPECT: complex_x: 30
    printf("complex_x: %d\n", x);
    // EXPECT: complex_y: 30
    printf("complex_y: %d\n", y);

    return 0;
}
