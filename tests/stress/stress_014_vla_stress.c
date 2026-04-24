// VLA stress: computed sizes, loops, function params, sizeof, nesting, structs
int printf(const char *fmt, ...);

int get_size(int base) {
    return base + 3;
}

void fill_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = i * i;
    }
}

int sum_array(int *arr, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) {
        s = s + arr[i];
    }
    return s;
}

void test_basic_vla(void) {
    int n = 10;
    int arr[n];
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    int sum = 0;
    for (i = 0; i < n; i++) {
        sum = sum + arr[i];
    }
    printf("basic sum=%d\n", sum);
    // EXPECT: basic sum=55
}

void test_vla_from_func(void) {
    int n = get_size(7);
    int arr[n];
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = i;
    }
    printf("func size=%d sum=%d\n", n, sum_array(arr, n));
    // EXPECT: func size=10 sum=45
}

void test_vla_in_loop(void) {
    int i;
    for (i = 1; i <= 5; i++) {
        int arr[i];
        int j;
        for (j = 0; j < i; j++) {
            arr[j] = j + 1;
        }
        int s = sum_array(arr, i);
        printf("i=%d sum=%d\n", i, s);
    }
    // EXPECT: i=1 sum=1
    // EXPECT: i=2 sum=3
    // EXPECT: i=3 sum=6
    // EXPECT: i=4 sum=10
    // EXPECT: i=5 sum=15
}

void test_vla_passed_to_func(void) {
    int n = 8;
    int arr[n];
    fill_array(arr, n);
    int s = sum_array(arr, n);
    printf("passed sum=%d\n", s);
    // EXPECT: passed sum=140
}

void test_sizeof_vla(void) {
    int n = 6;
    int arr[n];
    unsigned long sz = sizeof(arr);
    unsigned long expected = (unsigned long)n * sizeof(int);
    printf("sizeof=%d expected=%d match=%d\n", (int)sz, (int)expected, sz == expected);
    // EXPECT: sizeof=24 expected=24 match=1
}

void test_sizeof_vla_dynamic(void) {
    int i;
    for (i = 1; i <= 4; i++) {
        int arr[i * 3];
        int sz = (int)sizeof(arr);
        int expected = i * 3 * (int)sizeof(int);
        printf("n=%d sizeof=%d ok=%d\n", i * 3, sz, sz == expected);
    }
    // EXPECT: n=3 sizeof=12 ok=1
    // EXPECT: n=6 sizeof=24 ok=1
    // EXPECT: n=9 sizeof=36 ok=1
    // EXPECT: n=12 sizeof=48 ok=1
}

void test_vla_char(void) {
    int n = 20;
    char buf[n];
    int i;
    for (i = 0; i < n - 1; i++) {
        buf[i] = 'A' + (i % 26);
    }
    buf[n - 1] = 0;
    printf("str=%s\n", buf);
    // EXPECT: str=ABCDEFGHIJKLMNOPQRS
}

void test_nested_vla(void) {
    int n = 4;
    int m = 3;
    int a[n];
    int b[m];
    int i;
    for (i = 0; i < n; i++) a[i] = i * 10;
    for (i = 0; i < m; i++) b[i] = i * 100;
    printf("a: %d %d %d %d\n", a[0], a[1], a[2], a[3]);
    printf("b: %d %d %d\n", b[0], b[1], b[2]);
    // EXPECT: a: 0 10 20 30
    // EXPECT: b: 0 100 200
}

void test_vla_large(void) {
    int n = 100;
    int arr[n];
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = i;
    }
    int sum = 0;
    for (i = 0; i < n; i++) {
        sum = sum + arr[i];
    }
    printf("large sum=%d\n", sum);
    // EXPECT: large sum=4950
}

void test_vla_after_fixed(void) {
    int fixed[5] = {1, 2, 3, 4, 5};
    int n = 5;
    int vla[n];
    int i;
    for (i = 0; i < n; i++) {
        vla[i] = fixed[i] * 10;
    }
    printf("fixed+vla: %d %d %d %d %d\n", vla[0], vla[1], vla[2], vla[3], vla[4]);
    // EXPECT: fixed+vla: 10 20 30 40 50
}

void test_vla_conditional_size(void) {
    int cond = 1;
    int n = cond ? 8 : 4;
    int arr[n];
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = i;
    }
    printf("cond n=%d last=%d\n", n, arr[n - 1]);
    // EXPECT: cond n=8 last=7
}

void test_vla_sum_two(void) {
    int a = 3;
    int b = 4;
    int n = a + b;
    int arr[n];
    int i;
    for (i = 0; i < n; i++) arr[i] = 1;
    printf("sum_two=%d\n", sum_array(arr, n));
    // EXPECT: sum_two=7
}

int main(void) {
    test_basic_vla();
    test_vla_from_func();
    test_vla_in_loop();
    test_vla_passed_to_func();
    test_sizeof_vla();
    test_sizeof_vla_dynamic();
    test_vla_char();
    test_nested_vla();
    test_vla_large();
    test_vla_after_fixed();
    test_vla_conditional_size();
    test_vla_sum_two();
    return 0;
}
