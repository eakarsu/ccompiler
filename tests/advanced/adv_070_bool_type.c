// EXPECT: bool_true=1
// EXPECT: bool_false=0
// EXPECT: from_int=1
// EXPECT: from_zero=0
// EXPECT: from_neg=1
// EXPECT: from_large=1
// EXPECT: from_char=1
// EXPECT: from_null_ptr=0
// EXPECT: cond_true=yes
// EXPECT: cond_false=no
// EXPECT: bool_and=0
// EXPECT: bool_or=1
// EXPECT: bool_not=0
// EXPECT: arith_add=2
// EXPECT: arith_mul=0
// EXPECT: bool_eq=1
// EXPECT: bool_neq=1
// EXPECT: arr: 1 0 1 1 0
// EXPECT: arr_sum=3
// EXPECT: struct_flags: a=1 b=0 c=1
// EXPECT: toggle=0
// EXPECT: chain_assign=1
// EXPECT: ternary_bool=1
// EXPECT: loop_count=5
// EXPECT: compare_bools=1
// EXPECT: all_passed=1

int printf(const char *fmt, ...);

struct BoolFlags {
    _Bool active;
    _Bool visible;
    _Bool enabled;
};

_Bool is_positive(int x) {
    return x > 0;
}

_Bool is_even(int x) {
    return (x % 2) == 0;
}

int count_true(_Bool *arr, int n) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i]) count = count + 1;
    }
    return count;
}

int main(void) {
    int passed = 1;

    // Basic _Bool assignment
    _Bool bt = 1;
    _Bool bf = 0;
    printf("bool_true=%d\n", bt);
    if (bt != 1) passed = 0;
    printf("bool_false=%d\n", bf);
    if (bf != 0) passed = 0;

    // Assignment from int (non-zero becomes 1)
    _Bool from_int = 42;
    printf("from_int=%d\n", from_int);
    if (from_int != 1) passed = 0;

    _Bool from_zero = 0;
    printf("from_zero=%d\n", from_zero);
    if (from_zero != 0) passed = 0;

    _Bool from_neg = -5;
    printf("from_neg=%d\n", from_neg);
    if (from_neg != 1) passed = 0;

    _Bool from_large = 1000000;
    printf("from_large=%d\n", from_large);
    if (from_large != 1) passed = 0;

    // Assignment from char
    _Bool from_char = 'A';
    printf("from_char=%d\n", from_char);
    if (from_char != 1) passed = 0;

    // Assignment from null pointer (0)
    _Bool from_null_ptr = 0;
    printf("from_null_ptr=%d\n", from_null_ptr);
    if (from_null_ptr != 0) passed = 0;

    // Bool in conditions
    _Bool cond = 1;
    if (cond) {
        printf("cond_true=yes\n");
    } else {
        printf("cond_true=no\n");
        passed = 0;
    }

    cond = 0;
    if (cond) {
        printf("cond_false=yes\n");
        passed = 0;
    } else {
        printf("cond_false=no\n");
    }

    // Bool logical operations
    _Bool a = 1;
    _Bool b = 0;
    _Bool and_result = a && b;
    printf("bool_and=%d\n", and_result);
    if (and_result != 0) passed = 0;

    _Bool or_result = a || b;
    printf("bool_or=%d\n", or_result);
    if (or_result != 1) passed = 0;

    _Bool not_result = !a;
    printf("bool_not=%d\n", not_result);
    if (not_result != 0) passed = 0;

    // Bool arithmetic (promoted to int)
    _Bool x = 1;
    _Bool y = 1;
    int add_result = x + y;
    printf("arith_add=%d\n", add_result);
    if (add_result != 2) passed = 0;

    _Bool z = 0;
    int mul_result = x * z;
    printf("arith_mul=%d\n", mul_result);
    if (mul_result != 0) passed = 0;

    // Bool comparison
    _Bool eq = (x == y);
    printf("bool_eq=%d\n", eq);
    if (eq != 1) passed = 0;

    _Bool neq = (x != z);
    printf("bool_neq=%d\n", neq);
    if (neq != 1) passed = 0;

    // Bool arrays
    _Bool arr[5];
    arr[0] = 1;
    arr[1] = 0;
    arr[2] = 1;
    arr[3] = 1;
    arr[4] = 0;
    printf("arr: %d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4]);
    if (arr[0] != 1 || arr[1] != 0 || arr[2] != 1) passed = 0;

    int arr_sum = count_true(arr, 5);
    printf("arr_sum=%d\n", arr_sum);
    if (arr_sum != 3) passed = 0;

    // Bool in structs
    struct BoolFlags flags;
    flags.active = 1;
    flags.visible = 0;
    flags.enabled = 1;
    printf("struct_flags: a=%d b=%d c=%d\n",
           flags.active, flags.visible, flags.enabled);
    if (flags.active != 1 || flags.visible != 0 || flags.enabled != 1) passed = 0;

    // Toggle pattern
    _Bool toggle = 1;
    toggle = !toggle;
    printf("toggle=%d\n", toggle);
    if (toggle != 0) passed = 0;

    // Chain assignment
    _Bool c1, c2, c3;
    c1 = c2 = c3 = 1;
    printf("chain_assign=%d\n", c1 && c2 && c3);
    if (!(c1 && c2 && c3)) passed = 0;

    // Ternary with bool
    int val = 10;
    _Bool tb = val > 5 ? 1 : 0;
    printf("ternary_bool=%d\n", tb);
    if (tb != 1) passed = 0;

    // Bool in loop condition
    int count = 0;
    _Bool running = 1;
    while (running) {
        count = count + 1;
        if (count >= 5) running = 0;
    }
    printf("loop_count=%d\n", count);
    if (count != 5) passed = 0;

    // Function returning _Bool
    _Bool p1 = is_positive(10);
    _Bool p2 = is_positive(-3);
    _Bool e1 = is_even(4);
    _Bool e2 = is_even(7);
    int cmp = (p1 == 1 && p2 == 0 && e1 == 1 && e2 == 0) ? 1 : 0;
    printf("compare_bools=%d\n", cmp);
    if (cmp != 1) passed = 0;

    printf("all_passed=%d\n", passed);
    return passed ? 0 : 1;
}
