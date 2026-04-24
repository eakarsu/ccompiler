int printf(const char *fmt, ...);

int call_count = 0;

void init_array_in_block(int *out, int n) {
    {
        int arr[5];
        int i;
        for (i = 0; i < 5; i = i + 1) {
            arr[i] = n + i;
        }
        *out = arr[0] + arr[1] + arr[2] + arr[3] + arr[4];
    }
}

void test_local_reinit(void) {
    int r1;
    int r2;
    init_array_in_block(&r1, 10);
    // EXPECT: first call result = 60
    printf("first call result = %d\n", r1);
    init_array_in_block(&r2, 100);
    // EXPECT: second call result = 510
    printf("second call result = %d\n", r2);
}

int sum_local_array(int base) {
    int arr[4];
    arr[0] = base;
    arr[1] = base + 1;
    arr[2] = base + 2;
    arr[3] = base + 3;
    return arr[0] + arr[1] + arr[2] + arr[3];
}

void test_repeated_calls(void) {
    int s1 = sum_local_array(0);
    // EXPECT: sum(0) = 6
    printf("sum(0) = %d\n", s1);
    int s2 = sum_local_array(10);
    // EXPECT: sum(10) = 46
    printf("sum(10) = %d\n", s2);
    int s3 = sum_local_array(100);
    // EXPECT: sum(100) = 406
    printf("sum(100) = %d\n", s3);
}

int counter_func(void) {
    int local_val = 0;
    call_count = call_count + 1;
    local_val = call_count;
    return local_val;
}

void test_no_persistence(void) {
    int r1 = counter_func();
    // EXPECT: counter call 1 = 1
    printf("counter call 1 = %d\n", r1);
    int r2 = counter_func();
    // EXPECT: counter call 2 = 2
    printf("counter call 2 = %d\n", r2);
    int r3 = counter_func();
    // EXPECT: counter call 3 = 3
    printf("counter call 3 = %d\n", r3);
}

void fill_and_check(int id, int fill_val) {
    int buf[8];
    int i;
    for (i = 0; i < 8; i = i + 1) {
        buf[i] = fill_val;
    }
    int sum = 0;
    for (i = 0; i < 8; i = i + 1) {
        sum = sum + buf[i];
    }
    if (id == 1) {
        // EXPECT: fill1 sum = 40
        printf("fill1 sum = %d\n", sum);
    } else if (id == 2) {
        // EXPECT: fill2 sum = 160
        printf("fill2 sum = %d\n", sum);
    } else {
        // EXPECT: fill3 sum = 800
        printf("fill3 sum = %d\n", sum);
    }
}

void test_array_lifetime(void) {
    fill_and_check(1, 5);
    fill_and_check(2, 20);
    fill_and_check(3, 100);
}

int block_local_sum(int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        {
            int temp = i * i;
            total = total + temp;
        }
    }
    return total;
}

void test_block_local(void) {
    int r1 = block_local_sum(4);
    // EXPECT: block sum(4) = 14
    printf("block sum(4) = %d\n", r1);
    int r2 = block_local_sum(5);
    // EXPECT: block sum(5) = 30
    printf("block sum(5) = %d\n", r2);
}

void nested_array_blocks(void) {
    int outer_sum = 0;
    {
        int a[3];
        a[0] = 1; a[1] = 2; a[2] = 3;
        outer_sum = outer_sum + a[0] + a[1] + a[2];
    }
    {
        int a[3];
        a[0] = 10; a[1] = 20; a[2] = 30;
        outer_sum = outer_sum + a[0] + a[1] + a[2];
    }
    // EXPECT: nested array sum = 66
    printf("nested array sum = %d\n", outer_sum);
}

int main(void) {
    test_local_reinit();
    test_repeated_calls();
    test_no_persistence();
    test_array_lifetime();
    test_block_local();
    nested_array_blocks();

    // EXPECT: total calls = 3
    printf("total calls = %d\n", call_count);

    return 0;
}
