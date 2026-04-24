int printf(const char *fmt, ...);

int global_val = 42;

void set_value(int *p, int v) {
    *p = v;
}

int read_value(int *p) {
    return *p;
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void increment(int *p) {
    *p = *p + 1;
}

void zero_out(int *p, int count) {
    int i;
    for (i = 0; i < count; i = i + 1) {
        *(p + i) = 0;
    }
}

int sum_via_ptr(int *arr, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        total = total + *(arr + i);
    }
    return total;
}

int main(void) {
    int x = 10;
    int *p = &x;

    // EXPECT: 10
    printf("%d\n", *p);

    *p = 20;
    // EXPECT: 20
    printf("%d\n", x);

    // EXPECT: 20
    printf("%d\n", *p);

    int y = 30;
    p = &y;
    // EXPECT: 30
    printf("%d\n", *p);

    // EXPECT: 42
    printf("%d\n", global_val);

    int *gp = &global_val;
    *gp = 100;
    // EXPECT: 100
    printf("%d\n", global_val);

    int a = 5;
    int b = 15;
    swap(&a, &b);
    // EXPECT: 15
    printf("%d\n", a);
    // EXPECT: 5
    printf("%d\n", b);

    int val = 99;
    set_value(&val, 200);
    // EXPECT: 200
    printf("%d\n", val);

    // EXPECT: 200
    printf("%d\n", read_value(&val));

    increment(&val);
    // EXPECT: 201
    printf("%d\n", val);

    int arr[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;

    int *ap = arr;
    // EXPECT: 10
    printf("%d\n", *ap);

    // EXPECT: 30
    printf("%d\n", *(ap + 2));

    // EXPECT: 50
    printf("%d\n", *(ap + 4));

    // EXPECT: 150
    printf("%d\n", sum_via_ptr(arr, 5));

    zero_out(arr, 5);
    // EXPECT: 0
    printf("%d\n", arr[0]);
    // EXPECT: 0
    printf("%d\n", arr[4]);

    // EXPECT: 0
    printf("%d\n", sum_via_ptr(arr, 5));

    int c = 7;
    int *pc = &c;
    int **ppc = &pc;
    // EXPECT: 7
    printf("%d\n", **ppc);

    **ppc = 77;
    // EXPECT: 77
    printf("%d\n", c);

    int same = (&x == &x);
    // EXPECT: 1
    printf("%d\n", same);

    int diff = (&x == &y);
    // EXPECT: 0
    printf("%d\n", diff);

    return 0;
}
