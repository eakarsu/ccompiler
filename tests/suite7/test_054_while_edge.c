int printf(const char *fmt, ...);

int while_zero(void) {
    int count = 0;
    while (0) {
        count = count + 1;
    }
    return count;
}

int while_one_break(int limit) {
    int i = 0;
    while (1) {
        if (i >= limit) break;
        i = i + 1;
    }
    return i;
}

int do_while_zero(void) {
    int count = 0;
    do {
        count = count + 1;
    } while (0);
    return count;
}

int do_while_runs_once(int flag) {
    int val = 0;
    do {
        val = val + 10;
        if (flag == 0) break;
        val = val + 20;
    } while (0);
    return val;
}

int complex_condition(int n) {
    int sum = 0;
    int i = 0;
    while (i < n && sum < 100) {
        sum = sum + i * i;
        i = i + 1;
    }
    return sum;
}

int or_condition(int n) {
    int i = 0;
    int found = 0;
    while (i < n || found == 0) {
        if (i >= 20) {
            found = 1;
            break;
        }
        i = i + 1;
    }
    return i;
}

int nested_while(int rows, int cols) {
    int total = 0;
    int i = 0;
    while (i < rows) {
        int j = 0;
        while (j < cols) {
            total = total + 1;
            j = j + 1;
        }
        i = i + 1;
    }
    return total;
}

int while_with_multiple_breaks(int n) {
    int i = 0;
    while (i < 1000) {
        if (i == n) break;
        if (i * i > 200) break;
        i = i + 1;
    }
    return i;
}

int do_while_countdown(int n) {
    int sum = 0;
    do {
        sum = sum + n;
        n = n - 1;
    } while (n > 0);
    return sum;
}

int while_modifying_cond(void) {
    int a = 10;
    int b = 1;
    int steps = 0;
    while (a > b) {
        a = a - 1;
        b = b + 1;
        steps = steps + 1;
    }
    return steps;
}

int do_while_continue(int n) {
    int sum = 0;
    int i = 0;
    do {
        i = i + 1;
        if (i % 3 == 0) continue;
        sum = sum + i;
    } while (i < n);
    return sum;
}

int while_flag_pattern(int arr[], int len, int target) {
    int found = -1;
    int i = 0;
    while (i < len && found == -1) {
        if (arr[i] == target) {
            found = i;
        }
        i = i + 1;
    }
    return found;
}

int main(void) {
    // EXPECT: while_zero: 0
    printf("while_zero: %d\n", while_zero());
    // EXPECT: while_one 5: 5
    printf("while_one 5: %d\n", while_one_break(5));
    // EXPECT: while_one 0: 0
    printf("while_one 0: %d\n", while_one_break(0));
    // EXPECT: do_zero: 1
    printf("do_zero: %d\n", do_while_zero());
    // EXPECT: do_once 0: 10
    printf("do_once 0: %d\n", do_while_runs_once(0));
    // EXPECT: do_once 1: 30
    printf("do_once 1: %d\n", do_while_runs_once(1));
    // EXPECT: complex 5: 30
    printf("complex 5: %d\n", complex_condition(5));
    // EXPECT: complex 20: 140
    printf("complex 20: %d\n", complex_condition(20));
    // EXPECT: or_cond 5: 20
    printf("or_cond 5: %d\n", or_condition(5));
    // EXPECT: nested 3,4: 12
    printf("nested 3,4: %d\n", nested_while(3, 4));
    // EXPECT: multibreak 5: 5
    printf("multibreak 5: %d\n", while_with_multiple_breaks(5));
    // EXPECT: multibreak 100: 15
    printf("multibreak 100: %d\n", while_with_multiple_breaks(100));
    // EXPECT: do_count 5: 15
    printf("do_count 5: %d\n", do_while_countdown(5));
    // EXPECT: mod_cond: 5
    printf("mod_cond: %d\n", while_modifying_cond());
    // EXPECT: do_continue 10: 37
    printf("do_continue 10: %d\n", do_while_continue(10));

    int arr[] = {5, 3, 8, 1, 7};
    // EXPECT: find 8: 2
    printf("find 8: %d\n", while_flag_pattern(arr, 5, 8));
    // EXPECT: find 99: -1
    printf("find 99: %d\n", while_flag_pattern(arr, 5, 99));

    return 0;
}
