int printf(const char *fmt, ...);

typedef int (*IntOp)(int, int);

int op_add(int a, int b) { return a + b; }
int op_sub(int a, int b) { return a - b; }
int op_mul(int a, int b) { return a * b; }
int op_div(int a, int b) {
    if (b == 0) return 0;
    return a / b;
}
int op_mod(int a, int b) {
    if (b == 0) return 0;
    return a % b;
}
int op_max(int a, int b) {
    if (a > b) return a;
    return b;
}
int op_min(int a, int b) {
    if (a < b) return a;
    return b;
}

int dispatch(int opcode, int a, int b) {
    IntOp table[7];
    table[0] = op_add;
    table[1] = op_sub;
    table[2] = op_mul;
    table[3] = op_div;
    table[4] = op_mod;
    table[5] = op_max;
    table[6] = op_min;
    if (opcode < 0 || opcode > 6) return -1;
    return table[opcode](a, b);
}

typedef int (*UnaryOp)(int);

int square(int x) { return x * x; }
int negate(int x) { return -x; }
int double_it(int x) { return x + x; }
int identity(int x) { return x; }
int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int apply_chain(UnaryOp ops[], int n, int val) {
    int i = 0;
    while (i < n) {
        val = ops[i](val);
        i = i + 1;
    }
    return val;
}

int transform_array(int arr[], int n, UnaryOp fn) {
    int sum = 0;
    int i = 0;
    while (i < n) {
        arr[i] = fn(arr[i]);
        sum = sum + arr[i];
        i = i + 1;
    }
    return sum;
}

typedef int (*Predicate)(int);

int is_positive(int x) { return x > 0; }
int is_even(int x) { return (x % 2) == 0; }
int is_gt5(int x) { return x > 5; }

int count_if(int arr[], int n, Predicate pred) {
    int cnt = 0;
    int i = 0;
    while (i < n) {
        if (pred(arr[i])) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

int filter_sum(int arr[], int n, Predicate pred) {
    int s = 0;
    int i = 0;
    while (i < n) {
        if (pred(arr[i])) s = s + arr[i];
        i = i + 1;
    }
    return s;
}

int calculator(int program[], int n) {
    int acc = 0;
    int i = 0;
    while (i + 1 < n) {
        int op = program[i];
        int val = program[i + 1];
        acc = dispatch(op, acc, val);
        i = i + 2;
    }
    return acc;
}

int main(void) {
    // EXPECT: add=15
    printf("add=%d\n", dispatch(0, 10, 5));
    // EXPECT: sub=5
    printf("sub=%d\n", dispatch(1, 10, 5));
    // EXPECT: mul=50
    printf("mul=%d\n", dispatch(2, 10, 5));
    // EXPECT: div=2
    printf("div=%d\n", dispatch(3, 10, 5));
    // EXPECT: mod=0
    printf("mod=%d\n", dispatch(4, 10, 5));
    // EXPECT: max=10
    printf("max=%d\n", dispatch(5, 10, 5));
    // EXPECT: min=5
    printf("min=%d\n", dispatch(6, 10, 5));

    UnaryOp chain[3];
    chain[0] = double_it;
    chain[1] = negate;
    chain[2] = abs_val;
    // EXPECT: chain=10
    printf("chain=%d\n", apply_chain(chain, 3, 5));

    int arr1[4] = {1, 2, 3, 4};
    // EXPECT: sqsum=30
    printf("sqsum=%d\n", transform_array(arr1, 4, square));

    int arr2[6] = {-3, 5, -1, 8, 0, 2};
    // EXPECT: poscount=3
    printf("poscount=%d\n", count_if(arr2, 6, is_positive));
    // EXPECT: evencount=3
    printf("evencount=%d\n", count_if(arr2, 6, is_even));
    // EXPECT: gt5count=1
    printf("gt5count=%d\n", count_if(arr2, 6, is_gt5));
    // EXPECT: fsum=15
    printf("fsum=%d\n", filter_sum(arr2, 6, is_positive));

    int prog[8] = {0,10, 2,3, 1,5, 0,2};
    // EXPECT: calc=27
    printf("calc=%d\n", calculator(prog, 8));

    // EXPECT: bad=-1
    printf("bad=%d\n", dispatch(99, 1, 2));

    return 0;
}
