int printf(const char *fmt, ...);
// EXPECT: Operations on 12 and 5:\n  add(12,5) = 17\n  sub(12,5) = 7\n  mul(12,5) = 60\n  div(12,5) = 2\n  mod(12,5) = 2\n  pow(12,5) = 248832\n  max(12,5) = 12\n  min(12,5) = 5\nApply all sum = 86\nMax of array: 9\nMin of array: 2\nSum of array: 25

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int divide(int a, int b) { return b != 0 ? a / b : 0; }
int mod(int a, int b) { return b != 0 ? a % b : 0; }
int power(int a, int b) {
    int result = 1;
    int i;
    for (i = 0; i < b; i++) result = result * a;
    return result;
}
int max_fn(int a, int b) { return a > b ? a : b; }
int min_fn(int a, int b) { return a < b ? a : b; }

typedef int (*BinOp)(int, int);

int apply_all(BinOp *ops, int count, int a, int b) {
    int total = 0;
    int i;
    for (i = 0; i < count; i++) {
        total = total + ops[i](a, b);
    }
    return total;
}

int main(void) {
    BinOp ops[8];
    ops[0] = add;
    ops[1] = sub;
    ops[2] = mul;
    ops[3] = divide;
    ops[4] = mod;
    ops[5] = power;
    ops[6] = max_fn;
    ops[7] = min_fn;

    char *names[8];
    names[0] = "add";
    names[1] = "sub";
    names[2] = "mul";
    names[3] = "div";
    names[4] = "mod";
    names[5] = "pow";
    names[6] = "max";
    names[7] = "min";

    int a = 12;
    int b = 5;
    int i;

    printf("Operations on %d and %d:\n", a, b);
    for (i = 0; i < 8; i++) {
        printf("  %s(%d,%d) = %d\n", names[i], a, b, ops[i](a, b));
    }

    printf("Apply all sum = %d\n", apply_all(ops, 4, a, b));

    int data[5];
    data[0] = 3; data[1] = 7; data[2] = 2; data[3] = 9; data[4] = 4;
    int result = data[0];
    for (i = 1; i < 5; i++) {
        result = max_fn(result, data[i]);
    }
    printf("Max of array: %d\n", result);

    result = data[0];
    for (i = 1; i < 5; i++) {
        result = min_fn(result, data[i]);
    }
    printf("Min of array: %d\n", result);

    result = 0;
    for (i = 0; i < 5; i++) {
        result = add(result, data[i]);
    }
    printf("Sum of array: %d\n", result);

    return 0;
}
