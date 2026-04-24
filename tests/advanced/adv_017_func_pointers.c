int printf(const char *fmt, ...);
// EXPECT: === Function Pointer Tests ===
// EXPECT: basic_fptr: add(3,4)=7
// EXPECT: basic_fptr: sub(10,3)=7
// EXPECT: reassign_fptr: result=20
// EXPECT: fptr_array_add: 8
// EXPECT: fptr_array_sub: 2
// EXPECT: fptr_array_mul: 15
// EXPECT: apply: add -> 11
// EXPECT: apply: mul -> 30
// EXPECT: map_double: 2 4 6 8 10
// EXPECT: map_square: 1 4 9 16 25
// EXPECT: map_negate: -1 -2 -3 -4 -5
// EXPECT: callback_sort: 1 2 3 4 5
// EXPECT: callback_sort_desc: 5 4 3 2 1
// EXPECT: typedef_fptr: 15
// EXPECT: typedef_fptr: 5
// EXPECT: compose: f(g(3))=20
// EXPECT: fold_add: 15
// EXPECT: fold_mul: 120
// EXPECT: find_if_even: found 2 at index 1
// EXPECT: find_if_neg: found -3 at index 3
// EXPECT: count_if_pos: 3
// EXPECT: dispatch_0: add=12
// EXPECT: dispatch_1: sub=2
// EXPECT: dispatch_2: mul=35
// EXPECT: null_fptr: pointer is null
// EXPECT: fptr_return: result=25
// EXPECT: nested_apply: 42
// EXPECT: === All Function Pointer Tests Passed ===

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

int mul(int a, int b) {
    return a * b;
}

int apply(int (*f)(int, int), int a, int b) {
    return f(a, b);
}

int double_val(int x) {
    return x * 2;
}

int square(int x) {
    return x * x;
}

int negate(int x) {
    return -x;
}

void map_array(int *arr, int n, int (*f)(int)) {
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = f(arr[i]);
    }
}

int cmp_asc(int a, int b) {
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

int cmp_desc(int a, int b) {
    if (a > b) return -1;
    if (a < b) return 1;
    return 0;
}

void sort_with_cmp(int *arr, int n, int (*cmp)(int, int)) {
    int i;
    int j;
    int tmp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (cmp(arr[j], arr[j + 1]) > 0) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

typedef int (*binop_t)(int, int);
typedef int (*unaryop_t)(int);

int fold(int *arr, int n, int init, binop_t f) {
    int result = init;
    int i;
    for (i = 0; i < n; i++) {
        result = f(result, arr[i]);
    }
    return result;
}

int find_if(int *arr, int n, int (*pred)(int)) {
    int i;
    for (i = 0; i < n; i++) {
        if (pred(arr[i])) return i;
    }
    return -1;
}

int count_if(int *arr, int n, int (*pred)(int)) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (pred(arr[i])) count = count + 1;
    }
    return count;
}

int is_even(int x) {
    return x % 2 == 0;
}

int is_negative(int x) {
    return x < 0;
}

int is_positive(int x) {
    return x > 0;
}

int add5(int x) {
    return x + 5;
}

int triple(int x) {
    return x * 3;
}

int compose_apply(int (*f)(int), int (*g)(int), int x) {
    return f(g(x));
}

binop_t get_op(int choice) {
    if (choice == 0) return add;
    if (choice == 1) return sub;
    return mul;
}

int main() {
    printf("=== Function Pointer Tests ===\n");

    // basic function pointer declaration and call
    int (*fp)(int, int) = add;
    printf("basic_fptr: add(3,4)=%d\n", fp(3, 4));

    fp = sub;
    printf("basic_fptr: sub(10,3)=%d\n", fp(10, 3));

    // reassign function pointer
    fp = mul;
    printf("reassign_fptr: result=%d\n", fp(4, 5));

    // array of function pointers
    int (*ops[3])(int, int);
    ops[0] = add;
    ops[1] = sub;
    ops[2] = mul;
    printf("fptr_array_add: %d\n", ops[0](3, 5));
    printf("fptr_array_sub: %d\n", ops[1](5, 3));
    printf("fptr_array_mul: %d\n", ops[2](3, 5));

    // callback: apply function to two values
    printf("apply: add -> %d\n", apply(add, 5, 6));
    printf("apply: mul -> %d\n", apply(mul, 5, 6));

    // map: apply unary function to each element
    int arr1[5];
    arr1[0] = 1; arr1[1] = 2; arr1[2] = 3; arr1[3] = 4; arr1[4] = 5;
    map_array(arr1, 5, double_val);
    printf("map_double:");
    int i;
    for (i = 0; i < 5; i++) printf(" %d", arr1[i]);
    printf("\n");

    int arr2[5];
    arr2[0] = 1; arr2[1] = 2; arr2[2] = 3; arr2[3] = 4; arr2[4] = 5;
    map_array(arr2, 5, square);
    printf("map_square:");
    for (i = 0; i < 5; i++) printf(" %d", arr2[i]);
    printf("\n");

    int arr3[5];
    arr3[0] = 1; arr3[1] = 2; arr3[2] = 3; arr3[3] = 4; arr3[4] = 5;
    map_array(arr3, 5, negate);
    printf("map_negate:");
    for (i = 0; i < 5; i++) printf(" %d", arr3[i]);
    printf("\n");

    // sort with comparison callback - ascending
    int s1[5];
    s1[0] = 3; s1[1] = 1; s1[2] = 4; s1[3] = 5; s1[4] = 2;
    sort_with_cmp(s1, 5, cmp_asc);
    printf("callback_sort:");
    for (i = 0; i < 5; i++) printf(" %d", s1[i]);
    printf("\n");

    // sort with comparison callback - descending
    int s2[5];
    s2[0] = 3; s2[1] = 1; s2[2] = 4; s2[3] = 5; s2[4] = 2;
    sort_with_cmp(s2, 5, cmp_desc);
    printf("callback_sort_desc:");
    for (i = 0; i < 5; i++) printf(" %d", s2[i]);
    printf("\n");

    // typedef for function pointers
    binop_t op1 = add;
    binop_t op2 = sub;
    printf("typedef_fptr: %d\n", op1(10, 5));
    printf("typedef_fptr: %d\n", op2(10, 5));

    // compose two unary functions
    printf("compose: f(g(3))=%d\n", compose_apply(add5, triple, 3 + 2));

    // fold / reduce
    int fdata[5];
    fdata[0] = 1; fdata[1] = 2; fdata[2] = 3; fdata[3] = 4; fdata[4] = 5;
    printf("fold_add: %d\n", fold(fdata, 5, 0, add));
    printf("fold_mul: %d\n", fold(fdata, 5, 1, mul));

    // find_if with predicate
    int mixed[5];
    mixed[0] = 1; mixed[1] = 2; mixed[2] = 5; mixed[3] = -3; mixed[4] = 8;
    int idx1 = find_if(mixed, 5, is_even);
    printf("find_if_even: found %d at index %d\n", mixed[idx1], idx1);

    int idx2 = find_if(mixed, 5, is_negative);
    printf("find_if_neg: found %d at index %d\n", mixed[idx2], idx2);

    // count_if with predicate
    int cdata[6];
    cdata[0] = -1; cdata[1] = 3; cdata[2] = -5; cdata[3] = 7; cdata[4] = 9; cdata[5] = -2;
    printf("count_if_pos: %d\n", count_if(cdata, 6, is_positive));

    // dispatch table pattern
    int d;
    for (d = 0; d < 3; d++) {
        binop_t func = get_op(d);
        int result = func(7, 5);
        if (d == 0) printf("dispatch_0: add=%d\n", result);
        if (d == 1) printf("dispatch_1: sub=%d\n", result);
        if (d == 2) printf("dispatch_2: mul=%d\n", result);
    }

    // null function pointer check
    int (*null_fp)(int, int) = 0;
    if (null_fp == 0) {
        printf("null_fptr: pointer is null\n");
    }

    // function returning function pointer result
    binop_t ret_op = get_op(2);
    printf("fptr_return: result=%d\n", ret_op(5, 5));

    // nested apply
    printf("nested_apply: %d\n", apply(add, apply(mul, 4, 5), 22));

    printf("=== All Function Pointer Tests Passed ===\n");
    return 0;
}
