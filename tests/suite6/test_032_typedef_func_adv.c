int printf(const char *fmt, ...);

typedef int (*UnaryOp)(int);
typedef int (*BinaryOp)(int, int);
typedef int (*Predicate)(int);
typedef void (*Action)(int);

int double_it(int x) { return x * 2; }
int triple_it(int x) { return x * 3; }
int square_it(int x) { return x * x; }
int negate_it(int x) { return -x; }
int increment(int x) { return x + 1; }

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
int max_op(int a, int b) { if (a > b) return a; return b; }
int min_op(int a, int b) { if (a < b) return a; return b; }

int is_positive(int x) { return x > 0; }
int is_even(int x) { return (x % 2) == 0; }
int is_greater_than_5(int x) { return x > 5; }

int apply_unary(UnaryOp op, int x) {
    return op(x);
}

int apply_binary(BinaryOp op, int a, int b) {
    return op(a, b);
}

int apply_twice(UnaryOp op, int x) {
    return op(op(x));
}

int compose(UnaryOp f, UnaryOp g, int x) {
    return f(g(x));
}

int reduce(BinaryOp op, int arr[], int len, int init) {
    int result = init;
    int i;
    for (i = 0; i < len; i = i + 1) {
        result = op(result, arr[i]);
    }
    return result;
}

int count_if(Predicate pred, int arr[], int len) {
    int count = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (pred(arr[i])) {
            count = count + 1;
        }
    }
    return count;
}

int find_first(Predicate pred, int arr[], int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (pred(arr[i])) return arr[i];
    }
    return -1;
}

int transform_and_sum(UnaryOp op, int arr[], int len) {
    int total = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        total = total + op(arr[i]);
    }
    return total;
}

UnaryOp select_unary(int choice) {
    if (choice == 0) return double_it;
    if (choice == 1) return triple_it;
    if (choice == 2) return square_it;
    return negate_it;
}

BinaryOp select_binary(int choice) {
    if (choice == 0) return add;
    if (choice == 1) return subtract;
    return multiply;
}

int chain3(UnaryOp a, UnaryOp b, UnaryOp c, int x) {
    return a(b(c(x)));
}

int main(void) {
    // EXPECT: double 5: 10
    printf("double 5: %d\n", apply_unary(double_it, 5));

    // EXPECT: triple 4: 12
    printf("triple 4: %d\n", apply_unary(triple_it, 4));

    // EXPECT: square 6: 36
    printf("square 6: %d\n", apply_unary(square_it, 6));

    // EXPECT: add 3 7: 10
    printf("add 3 7: %d\n", apply_binary(add, 3, 7));

    // EXPECT: sub 10 3: 7
    printf("sub 10 3: %d\n", apply_binary(subtract, 10, 3));

    // EXPECT: mul 4 5: 20
    printf("mul 4 5: %d\n", apply_binary(multiply, 4, 5));

    // EXPECT: double twice 3: 12
    printf("double twice 3: %d\n", apply_twice(double_it, 3));

    // EXPECT: inc twice 10: 12
    printf("inc twice 10: %d\n", apply_twice(increment, 10));

    // EXPECT: compose double+triple 2: 12
    printf("compose double+triple 2: %d\n", compose(double_it, triple_it, 2));

    // EXPECT: compose square+double 3: 36
    printf("compose square+double 3: %d\n", compose(square_it, double_it, 3));

    int data[5];
    data[0] = 1; data[1] = 2; data[2] = 3; data[3] = 4; data[4] = 5;

    // EXPECT: sum reduce: 15
    printf("sum reduce: %d\n", reduce(add, data, 5, 0));

    // EXPECT: product reduce: 120
    printf("product reduce: %d\n", reduce(multiply, data, 5, 1));

    // EXPECT: max reduce: 5
    printf("max reduce: %d\n", reduce(max_op, data, 5, 0));

    // EXPECT: count positive: 5
    printf("count positive: %d\n", count_if(is_positive, data, 5));

    // EXPECT: count even: 2
    printf("count even: %d\n", count_if(is_even, data, 5));

    // EXPECT: count >5: 0
    printf("count >5: %d\n", count_if(is_greater_than_5, data, 5));

    // EXPECT: transform double sum: 30
    printf("transform double sum: %d\n", transform_and_sum(double_it, data, 5));

    // EXPECT: transform square sum: 55
    printf("transform square sum: %d\n", transform_and_sum(square_it, data, 5));

    UnaryOp op = select_unary(0);
    // EXPECT: selected double 7: 14
    printf("selected double 7: %d\n", op(7));

    op = select_unary(2);
    // EXPECT: selected square 4: 16
    printf("selected square 4: %d\n", op(4));

    BinaryOp bop = select_binary(2);
    // EXPECT: selected mul 3 8: 24
    printf("selected mul 3 8: %d\n", bop(3, 8));

    // EXPECT: chain3 inc+double+triple 2: 13
    printf("chain3 inc+double+triple 2: %d\n", chain3(increment, double_it, triple_it, 2));

    // EXPECT: find first >5: -1
    printf("find first >5: %d\n", find_first(is_greater_than_5, data, 5));

    int data2[5];
    data2[0] = 3; data2[1] = 7; data2[2] = 1; data2[3] = 9; data2[4] = 4;
    // EXPECT: find first >5 in data2: 7
    printf("find first >5 in data2: %d\n", find_first(is_greater_than_5, data2, 5));

    // EXPECT: min reduce data2: 1
    printf("min reduce data2: %d\n", reduce(min_op, data2, 5, 999));

    return 0;
}
