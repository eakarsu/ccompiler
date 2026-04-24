int printf(const char *fmt, ...);

typedef int (*BinOp)(int, int);
typedef int (*UnaryOp)(int);
typedef int (*NoArgOp)(void);
typedef void (*VoidOp)(int *);

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    return a / b;
}

int negate(int x) {
    return -x;
}

int double_it(int x) {
    return x * 2;
}

int square(int x) {
    return x * x;
}

int get_zero(void) {
    return 0;
}

int get_forty_two(void) {
    return 42;
}

int get_hundred(void) {
    return 100;
}

void increment(int *p) {
    *p = *p + 1;
}

void triple(int *p) {
    *p = *p * 3;
}

int apply_binop(BinOp op, int a, int b) {
    return op(a, b);
}

int apply_unary(UnaryOp op, int x) {
    return op(x);
}

int apply_noarg(NoArgOp op) {
    return op();
}

void apply_void(VoidOp op, int *val) {
    op(val);
}

int test_basic_funcptr(void) {
    BinOp op = add;
    return op(3, 4); // 7
}

int test_subtract_ptr(void) {
    BinOp op = subtract;
    return op(10, 3); // 7
}

int test_multiply_ptr(void) {
    BinOp op = multiply;
    return op(6, 7); // 42
}

int test_apply(void) {
    return apply_binop(add, 10, 20); // 30
}

int test_apply_subtract(void) {
    return apply_binop(subtract, 50, 25); // 25
}

int test_unary(void) {
    return apply_unary(negate, 5); // -5
}

int test_double(void) {
    return apply_unary(double_it, 21); // 42
}

int test_square(void) {
    return apply_unary(square, 7); // 49
}

int test_noarg(void) {
    return apply_noarg(get_forty_two); // 42
}

int test_noarg_hundred(void) {
    return apply_noarg(get_hundred); // 100
}

int test_void_op(void) {
    int x = 10;
    apply_void(increment, &x);
    return x; // 11
}

int test_void_triple(void) {
    int x = 5;
    apply_void(triple, &x);
    return x; // 15
}

int test_array_of_binops(void) {
    BinOp ops[4];
    ops[0] = add;
    ops[1] = subtract;
    ops[2] = multiply;
    ops[3] = divide;
    int sum = 0;
    sum = sum + ops[0](10, 5);  // 15
    sum = sum + ops[1](10, 5);  // 5
    sum = sum + ops[2](10, 5);  // 50
    sum = sum + ops[3](10, 5);  // 2
    return sum; // 72
}

int test_switch_funcptr(int choice) {
    BinOp op;
    switch (choice) {
        case 0: op = add; break;
        case 1: op = subtract; break;
        case 2: op = multiply; break;
        default: op = add; break;
    }
    return op(6, 3);
}

int test_conditional_funcptr(int flag) {
    UnaryOp op = flag ? double_it : negate;
    return op(10);
}

int main(void) {
    // EXPECT: basic funcptr: 7
    printf("basic funcptr: %d\n", test_basic_funcptr());

    // EXPECT: subtract ptr: 7
    printf("subtract ptr: %d\n", test_subtract_ptr());

    // EXPECT: multiply ptr: 42
    printf("multiply ptr: %d\n", test_multiply_ptr());

    // EXPECT: apply add: 30
    printf("apply add: %d\n", test_apply());

    // EXPECT: apply subtract: 25
    printf("apply subtract: %d\n", test_apply_subtract());

    // EXPECT: unary negate: -5
    printf("unary negate: %d\n", test_unary());

    // EXPECT: unary double: 42
    printf("unary double: %d\n", test_double());

    // EXPECT: unary square: 49
    printf("unary square: %d\n", test_square());

    // EXPECT: noarg 42: 42
    printf("noarg 42: %d\n", test_noarg());

    // EXPECT: noarg 100: 100
    printf("noarg 100: %d\n", test_noarg_hundred());

    // EXPECT: void increment: 11
    printf("void increment: %d\n", test_void_op());

    // EXPECT: void triple: 15
    printf("void triple: %d\n", test_void_triple());

    // EXPECT: array of binops: 72
    printf("array of binops: %d\n", test_array_of_binops());

    // EXPECT: switch add: 9
    printf("switch add: %d\n", test_switch_funcptr(0));

    // EXPECT: switch subtract: 3
    printf("switch subtract: %d\n", test_switch_funcptr(1));

    // EXPECT: switch multiply: 18
    printf("switch multiply: %d\n", test_switch_funcptr(2));

    // EXPECT: conditional double: 20
    printf("conditional double: %d\n", test_conditional_funcptr(1));

    // EXPECT: conditional negate: -10
    printf("conditional negate: %d\n", test_conditional_funcptr(0));

    return 0;
}
