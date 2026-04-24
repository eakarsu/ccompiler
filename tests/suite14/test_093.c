int printf(const char *fmt, ...);

// Test 093: Function pointer arrays and dispatch tables

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int div_op(int a, int b) { return a / b; }

int square(int x) { return x * x; }
int negate(int x) { return -x; }
int identity(int x) { return x; }
int double_it(int x) { return x * 2; }

typedef int (*binop_fn)(int, int);
typedef int (*unary_fn)(int);

void test_basic_fptr(void) {
    binop_fn f = add;
    printf("%d\n", f(3, 4));
    // EXPECT: 7

    f = sub;
    printf("%d\n", f(10, 3));
    // EXPECT: 7

    f = mul;
    printf("%d\n", f(5, 6));
    // EXPECT: 30
}

void test_dispatch_table(void) {
    binop_fn ops[4];
    ops[0] = add;
    ops[1] = sub;
    ops[2] = mul;
    ops[3] = div_op;

    printf("%d\n", ops[0](10, 5));
    // EXPECT: 15
    printf("%d\n", ops[1](10, 5));
    // EXPECT: 5
    printf("%d\n", ops[2](10, 5));
    // EXPECT: 50
    printf("%d\n", ops[3](10, 5));
    // EXPECT: 2
}

void test_unary_dispatch(void) {
    unary_fn transforms[4];
    transforms[0] = identity;
    transforms[1] = negate;
    transforms[2] = square;
    transforms[3] = double_it;

    int val = 7;
    int i;
    for (i = 0; i < 4; i++) {
        printf("%d\n", transforms[i](val));
    }
    // EXPECT: 7
    // EXPECT: -7
    // EXPECT: 49
    // EXPECT: 14
}

int apply_op(binop_fn fn, int a, int b) {
    return fn(a, b);
}

void test_fptr_as_param(void) {
    printf("%d\n", apply_op(add, 100, 200));
    // EXPECT: 300
    printf("%d\n", apply_op(mul, 12, 11));
    // EXPECT: 132
}

int apply_chain(unary_fn f, unary_fn g, int x) {
    return f(g(x));
}

void test_composition(void) {
    // negate(square(5)) = negate(25) = -25
    printf("%d\n", apply_chain(negate, square, 5));
    // EXPECT: -25

    // square(negate(3)) = square(-3) = 9
    printf("%d\n", apply_chain(square, negate, 3));
    // EXPECT: 9

    // double(square(4)) = double(16) = 32
    printf("%d\n", apply_chain(double_it, square, 4));
    // EXPECT: 32
}

void test_reduce(void) {
    int data[5] = {1, 2, 3, 4, 5};
    binop_fn op = add;

    int result = data[0];
    int i;
    for (i = 1; i < 5; i++) {
        result = op(result, data[i]);
    }
    printf("%d\n", result);
    // EXPECT: 15

    op = mul;
    result = data[0];
    for (i = 1; i < 5; i++) {
        result = op(result, data[i]);
    }
    printf("%d\n", result);
    // EXPECT: 120
}

void test_map(void) {
    int data[4] = {3, 5, 7, 9};
    unary_fn transform = square;

    int results[4];
    int i;
    for (i = 0; i < 4; i++) {
        results[i] = transform(data[i]);
    }

    int sum = 0;
    for (i = 0; i < 4; i++) {
        sum = sum + results[i];
    }
    // 9 + 25 + 49 + 81 = 164
    printf("%d\n", sum);
    // EXPECT: 164
}

binop_fn select_op(int choice) {
    if (choice == 0) return add;
    if (choice == 1) return sub;
    if (choice == 2) return mul;
    return div_op;
}

void test_return_fptr(void) {
    binop_fn f = select_op(0);
    printf("%d\n", f(8, 2));
    // EXPECT: 10

    f = select_op(2);
    printf("%d\n", f(8, 2));
    // EXPECT: 16

    f = select_op(3);
    printf("%d\n", f(8, 2));
    // EXPECT: 4
}

int main(void) {
    test_basic_fptr();
    test_dispatch_table();
    test_unary_dispatch();
    test_fptr_as_param();
    test_composition();
    test_reduce();
    test_map();
    test_return_fptr();
    return 0;
}
