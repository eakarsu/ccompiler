int printf(const char *fmt, ...);
// EXPECT: add(3,4)=7\nsub(10,3)=7\nmul(5,6)=30\ndiv(20,4)=5\nnegate(5)=-5\ndouble(7)=14\nsquare(8)=64\npositive: 4\neven: 3\nbig: 1\nsum=259\nproduct=-180\nvals[0]=1\nvals[1]=4\nvals[2]=9\nvals[3]=16
// Test: typedef for function pointer

typedef int (*BinOp)(int, int);
typedef int (*UnaryOp)(int);
typedef int (*Predicate)(int);

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int div_op(int a, int b) { return a / b; }

int negate(int x) { return -x; }
int double_val(int x) { return x * 2; }
int square(int x) { return x * x; }

int is_positive(int x) { return x > 0; }
int is_even(int x) { return (x % 2) == 0; }
int is_big(int x) { return x > 100; }

int apply_binop(BinOp op, int a, int b) {
    return op(a, b);
}

int apply_unary(UnaryOp op, int x) {
    return op(x);
}

int count_matching(int arr[], int n, Predicate pred) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (pred(arr[i])) {
            count = count + 1;
        }
    }
    return count;
}

int reduce(int arr[], int n, BinOp op, int init) {
    int result = init;
    int i;
    for (i = 0; i < n; i++) {
        result = op(result, arr[i]);
    }
    return result;
}

void transform(int arr[], int n, UnaryOp op) {
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = op(arr[i]);
    }
}

int main(void) {
    printf("add(3,4)=%d\n", apply_binop(add, 3, 4));
    printf("sub(10,3)=%d\n", apply_binop(sub, 10, 3));
    printf("mul(5,6)=%d\n", apply_binop(mul, 5, 6));
    printf("div(20,4)=%d\n", apply_binop(div_op, 20, 4));

    printf("negate(5)=%d\n", apply_unary(negate, 5));
    printf("double(7)=%d\n", apply_unary(double_val, 7));
    printf("square(8)=%d\n", apply_unary(square, 8));

    int data[6];
    data[0] = 3; data[1] = -5; data[2] = 12;
    data[3] = 200; data[4] = -1; data[5] = 50;

    printf("positive: %d\n", count_matching(data, 6, is_positive));
    printf("even: %d\n", count_matching(data, 6, is_even));
    printf("big: %d\n", count_matching(data, 6, is_big));

    printf("sum=%d\n", reduce(data, 6, add, 0));
    printf("product=%d\n", reduce(data, 3, mul, 1));

    int vals[4];
    vals[0] = 1; vals[1] = 2; vals[2] = 3; vals[3] = 4;
    transform(vals, 4, square);
    int i;
    for (i = 0; i < 4; i++) {
        printf("vals[%d]=%d\n", i, vals[i]);
    }

    return 0;
}
