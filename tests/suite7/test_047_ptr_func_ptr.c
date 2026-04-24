int printf(const char *fmt, ...);

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

int mul(int a, int b) {
    return a * b;
}

int square(int x) {
    return x * x;
}

int dbl(int x) {
    return x + x;
}

int negate(int x) {
    return -x;
}

int identity(int x) {
    return x;
}

int apply(int (*f)(int), int val) {
    return f(val);
}

int apply2(int (*f)(int, int), int a, int b) {
    return f(a, b);
}

int main(void) {
    // Basic function pointer
    int (*fp)(int, int) = add;
    // EXPECT: fp_add = 15
    printf("fp_add = %d\n", fp(10, 5));

    fp = sub;
    // EXPECT: fp_sub = 5
    printf("fp_sub = %d\n", fp(10, 5));

    fp = mul;
    // EXPECT: fp_mul = 50
    printf("fp_mul = %d\n", fp(10, 5));

    // Array of function pointers (binary ops)
    int (*ops[3])(int, int);
    ops[0] = add;
    ops[1] = sub;
    ops[2] = mul;

    // EXPECT: ops[0](3,4) = 7
    printf("ops[0](3,4) = %d\n", ops[0](3, 4));

    // EXPECT: ops[1](10,3) = 7
    printf("ops[1](10,3) = %d\n", ops[1](10, 3));

    // EXPECT: ops[2](6,7) = 42
    printf("ops[2](6,7) = %d\n", ops[2](6, 7));

    // Loop over function pointer array
    int i;
    int results[3];
    for (i = 0; i < 3; i++) {
        results[i] = ops[i](8, 2);
    }
    // EXPECT: 8+2 = 10
    printf("8+2 = %d\n", results[0]);

    // EXPECT: 8-2 = 6
    printf("8-2 = %d\n", results[1]);

    // EXPECT: 8*2 = 16
    printf("8*2 = %d\n", results[2]);

    // Array of unary function pointers
    int (*unary[4])(int);
    unary[0] = square;
    unary[1] = dbl;
    unary[2] = negate;
    unary[3] = identity;

    // EXPECT: square(5) = 25
    printf("square(5) = %d\n", unary[0](5));

    // EXPECT: dbl(5) = 10
    printf("dbl(5) = %d\n", unary[1](5));

    // EXPECT: negate(5) = -5
    printf("negate(5) = %d\n", unary[2](5));

    // EXPECT: identity(5) = 5
    printf("identity(5) = %d\n", unary[3](5));

    // Apply through higher-order function
    // EXPECT: apply_square = 49
    printf("apply_square = %d\n", apply(square, 7));

    // EXPECT: apply_dbl = 14
    printf("apply_dbl = %d\n", apply(dbl, 7));

    // EXPECT: apply2_add = 30
    printf("apply2_add = %d\n", apply2(add, 10, 20));

    // EXPECT: apply2_mul = 200
    printf("apply2_mul = %d\n", apply2(mul, 10, 20));

    // Pointer to function pointer
    int (*fp2)(int) = square;
    int (**pfp)(int) = &fp2;
    // EXPECT: pfp_call = 64
    printf("pfp_call = %d\n", (*pfp)(8));

    // Change what it points to
    fp2 = negate;
    // EXPECT: pfp_changed = -8
    printf("pfp_changed = %d\n", (*pfp)(8));

    // Accumulate using function pointer array
    int acc = 0;
    for (i = 0; i < 4; i++) {
        acc = acc + unary[i](3);
    }
    // 9 + 6 + (-3) + 3 = 15
    // EXPECT: accumulated = 15
    printf("accumulated = %d\n", acc);

    // Select function at runtime
    int choice = 2;
    int (*selected)(int, int) = ops[choice];
    // EXPECT: selected = 35
    printf("selected = %d\n", selected(5, 7));

    return 0;
}
