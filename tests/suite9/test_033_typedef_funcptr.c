int printf(const char *fmt, ...);

typedef int (*BinOp)(int, int);
typedef int (*UnaryOp)(int);
typedef void (*PrintOp)(int);

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

int mul(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    if (b == 0) return 0;
    return a / b;
}

int negate(int x) {
    return -x;
}

int double_val(int x) {
    return x * 2;
}

int square(int x) {
    return x * x;
}

void print_val(int v) {
    printf("val=%d\n", v);
}

void test_binop() {
    BinOp op;
    int r;

    op = add;
    r = op(10, 20);
    // EXPECT: add=30
    printf("add=%d\n", r);

    op = sub;
    r = op(50, 15);
    // EXPECT: sub=35
    printf("sub=%d\n", r);

    op = mul;
    r = op(7, 8);
    // EXPECT: mul=56
    printf("mul=%d\n", r);

    op = divide;
    r = op(100, 4);
    // EXPECT: div=25
    printf("div=%d\n", r);
}

void test_unary() {
    UnaryOp uop;
    int r;

    uop = negate;
    r = uop(42);
    // EXPECT: neg=-42
    printf("neg=%d\n", r);

    uop = double_val;
    r = uop(21);
    // EXPECT: dbl=42
    printf("dbl=%d\n", r);

    uop = square;
    r = uop(9);
    // EXPECT: sq=81
    printf("sq=%d\n", r);
}

void test_print_op() {
    PrintOp pop;
    pop = print_val;
    // EXPECT: val=42
    pop(42);
    // EXPECT: val=100
    pop(100);
}

void test_array_of_funcptrs() {
    BinOp ops[4];
    ops[0] = add;
    ops[1] = sub;
    ops[2] = mul;
    ops[3] = divide;

    char *names[4];
    names[0] = "add";
    names[1] = "sub";
    names[2] = "mul";
    names[3] = "div";

    int i;
    // EXPECT: ops[add](10,3)=13
    // EXPECT: ops[sub](10,3)=7
    // EXPECT: ops[mul](10,3)=30
    // EXPECT: ops[div](10,3)=3
    for (i = 0; i < 4; i++) {
        int r = ops[i](10, 3);
        printf("ops[%s](10,3)=%d\n", names[i], r);
    }
}

int apply_twice(UnaryOp f, int x) {
    return f(f(x));
}

void test_higher_order() {
    int r1 = apply_twice(double_val, 5);
    // EXPECT: twice_dbl=20
    printf("twice_dbl=%d\n", r1);

    int r2 = apply_twice(square, 3);
    // EXPECT: twice_sq=81
    printf("twice_sq=%d\n", r2);

    int r3 = apply_twice(negate, 7);
    // EXPECT: twice_neg=7
    printf("twice_neg=%d\n", r3);
}

int reduce(BinOp f, int *arr, int n) {
    int result = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        result = f(result, arr[i]);
    }
    return result;
}

void test_reduce() {
    int data[5];
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    data[3] = 4;
    data[4] = 5;

    int sum = reduce(add, data, 5);
    // EXPECT: reduce_add=15
    printf("reduce_add=%d\n", sum);

    int product = reduce(mul, data, 5);
    // EXPECT: reduce_mul=120
    printf("reduce_mul=%d\n", product);
}

int main() {
    test_binop();
    test_unary();
    test_print_op();
    test_array_of_funcptrs();
    test_higher_order();
    test_reduce();
    // EXPECT: done
    printf("done\n");
    return 0;
}
