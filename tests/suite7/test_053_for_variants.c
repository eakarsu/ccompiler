int printf(const char *fmt, ...);

int empty_init(int start, int limit) {
    int i = start;
    int sum = 0;
    for (; i < limit; i = i + 1) {
        sum = sum + i;
    }
    return sum;
}

int empty_cond(int n) {
    int sum = 0;
    int i;
    for (i = 0; ; i = i + 1) {
        if (i >= n) break;
        sum = sum + i;
    }
    return sum;
}

int empty_inc(int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; ) {
        sum = sum + i;
        i = i + 1;
    }
    return sum;
}

int all_empty(int n) {
    int sum = 0;
    int i = 0;
    for (;;) {
        if (i >= n) break;
        sum = sum + i;
        i = i + 1;
    }
    return sum;
}

int comma_init_inc(int n) {
    int sum = 0;
    int i;
    int j;
    for (i = 0, j = n; i < j; i = i + 1, j = j - 1) {
        sum = sum + i + j;
    }
    return sum;
}

int nested_for_vars(int rows, int cols) {
    int total = 0;
    int i;
    int j;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            total = total + i * cols + j;
        }
    }
    return total;
}

int for_single_iter(void) {
    int count = 0;
    int i;
    for (i = 0; i < 1; i = i + 1) {
        count = count + 1;
    }
    return count;
}

int for_zero_iter(void) {
    int count = 0;
    int i;
    for (i = 0; i < 0; i = i + 1) {
        count = count + 1;
    }
    return count;
}

int countdown(int n) {
    int sum = 0;
    int i;
    for (i = n; i > 0; i = i - 1) {
        sum = sum + i;
    }
    return sum;
}

int for_with_continue(int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (i % 2 == 0) continue;
        sum = sum + i;
    }
    return sum;
}

int triple_for(int n) {
    int count = 0;
    int i;
    int j;
    int k;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            for (k = 0; k < n; k = k + 1) {
                count = count + 1;
            }
        }
    }
    return count;
}

int comma_in_condition(void) {
    int sum = 0;
    int i;
    int tmp;
    for (i = 0; (tmp = i * 2, tmp < 20); i = i + 1) {
        sum = sum + tmp;
    }
    return sum;
}

int main(void) {
    // EXPECT: empty_init 3,7: 18
    printf("empty_init 3,7: %d\n", empty_init(3, 7));
    // EXPECT: empty_cond 5: 10
    printf("empty_cond 5: %d\n", empty_cond(5));
    // EXPECT: empty_inc 5: 10
    printf("empty_inc 5: %d\n", empty_inc(5));
    // EXPECT: all_empty 5: 10
    printf("all_empty 5: %d\n", all_empty(5));
    // EXPECT: comma 10: 50
    printf("comma 10: %d\n", comma_init_inc(10));
    // EXPECT: nested 3,4: 66
    printf("nested 3,4: %d\n", nested_for_vars(3, 4));
    // EXPECT: single: 1
    printf("single: %d\n", for_single_iter());
    // EXPECT: zero: 0
    printf("zero: %d\n", for_zero_iter());
    // EXPECT: countdown 5: 15
    printf("countdown 5: %d\n", countdown(5));
    // EXPECT: continue 10: 25
    printf("continue 10: %d\n", for_with_continue(10));
    // EXPECT: triple 3: 27
    printf("triple 3: %d\n", triple_for(3));
    // EXPECT: triple 4: 64
    printf("triple 4: %d\n", triple_for(4));
    // EXPECT: comma_cond: 90
    printf("comma_cond: %d\n", comma_in_condition());
    // EXPECT: empty_init 0,5: 10
    printf("empty_init 0,5: %d\n", empty_init(0, 5));
    // EXPECT: countdown 10: 55
    printf("countdown 10: %d\n", countdown(10));

    return 0;
}
