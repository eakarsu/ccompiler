int printf(const char *fmt, ...);

// Simple functions to take pointers to
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
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

int identity(int x) {
    return x;
}

int zero(int x) {
    return 0;
}

// Typedef for function pointer types
typedef int (*BinOp)(int, int);
typedef int (*UnaryOp)(int);

// Function that uses a function pointer
int apply_binop(BinOp op, int a, int b) {
    return op(a, b);
}

int apply_unary(UnaryOp op, int x) {
    return op(x);
}

// Transform an array with a unary function pointer
void transform(int *arr, int n, UnaryOp op) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        arr[i] = op(arr[i]);
    }
}

// Accumulate using a binary operation
int accumulate(int *arr, int n, int init, BinOp op) {
    int result = init;
    int i;
    for (i = 0; i < n; i = i + 1) {
        result = op(result, arr[i]);
    }
    return result;
}

// Function that returns different behavior based on a selector
int select_and_apply(int sel, int a, int b) {
    BinOp op;
    if (sel == 0) {
        op = add;
    } else if (sel == 1) {
        op = subtract;
    } else {
        op = multiply;
    }
    return op(a, b);
}

// Compare two values using a custom comparator
int max_by(int a, int b, int (*cmp)(int, int)) {
    if (cmp(a, b) > 0) return a;
    return b;
}

int compare_asc(int a, int b) {
    return a - b;
}

int compare_desc(int a, int b) {
    return b - a;
}

int main(void) {
    // Basic function pointer declaration and call
    int (*fp)(int, int);
    fp = add;
    // EXPECT: 15
    printf("%d\n", fp(10, 5));

    fp = subtract;
    // EXPECT: 5
    printf("%d\n", fp(10, 5));

    fp = multiply;
    // EXPECT: 50
    printf("%d\n", fp(10, 5));

    // Using typedef
    BinOp binop = add;
    // EXPECT: 30
    printf("%d\n", binop(10, 20));

    UnaryOp unop = negate;
    // EXPECT: -7
    printf("%d\n", unop(7));

    unop = double_val;
    // EXPECT: 14
    printf("%d\n", unop(7));

    unop = square;
    // EXPECT: 49
    printf("%d\n", unop(7));

    // Reassignment
    binop = multiply;
    // EXPECT: 200
    printf("%d\n", binop(10, 20));

    // Passing function pointer to another function
    // EXPECT: 13
    printf("%d\n", apply_binop(add, 6, 7));
    // EXPECT: -1
    printf("%d\n", apply_binop(subtract, 6, 7));
    // EXPECT: 42
    printf("%d\n", apply_binop(multiply, 6, 7));

    // EXPECT: 25
    printf("%d\n", apply_unary(square, 5));
    // EXPECT: -5
    printf("%d\n", apply_unary(negate, 5));

    // Transform array
    int arr[] = {1, 2, 3, 4, 5};
    transform(arr, 5, double_val);
    // EXPECT: 2 4 6 8 10
    printf("%d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4]);

    int arr2[] = {3, 4, 5};
    transform(arr2, 3, square);
    // EXPECT: 9 16 25
    printf("%d %d %d\n", arr2[0], arr2[1], arr2[2]);

    // Accumulate
    int vals[] = {1, 2, 3, 4, 5};
    // sum = 0 + 1 + 2 + 3 + 4 + 5 = 15
    // EXPECT: 15
    printf("%d\n", accumulate(vals, 5, 0, add));
    // product = 1 * 1 * 2 * 3 * 4 * 5 = 120
    // EXPECT: 120
    printf("%d\n", accumulate(vals, 5, 1, multiply));

    // Select and apply
    // EXPECT: 15
    printf("%d\n", select_and_apply(0, 10, 5));
    // EXPECT: 5
    printf("%d\n", select_and_apply(1, 10, 5));
    // EXPECT: 50
    printf("%d\n", select_and_apply(2, 10, 5));

    // Max by custom comparator
    // EXPECT: 10
    printf("%d\n", max_by(10, 5, compare_asc));
    // compare_desc: b - a, so cmp(10,5) = 5-10 = -5 < 0, return b=5
    // EXPECT: 5
    printf("%d\n", max_by(10, 5, compare_desc));

    // Function pointer to identity and zero
    UnaryOp f = identity;
    // EXPECT: 42
    printf("%d\n", f(42));
    f = zero;
    // EXPECT: 0
    printf("%d\n", f(42));

    return 0;
}
