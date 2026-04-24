int printf(const char *fmt, ...);

// Test: Function pointer arrays and dispatch tables

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

int modulo(int a, int b) {
    if (b == 0) return 0;
    return a % b;
}

int negate(int a, int b) {
    return -a;
}

int identity(int a, int b) {
    return a;
}

int max_of(int a, int b) {
    if (a > b) return a;
    return b;
}

int min_of(int a, int b) {
    if (a < b) return a;
    return b;
}

typedef int (*BinOp)(int, int);

int apply(BinOp fn, int x, int y) {
    return fn(x, y);
}

int apply_all(BinOp *ops, int n, int init, int operand) {
    int result = init;
    int i;
    for (i = 0; i < n; i++) {
        result = ops[i](result, operand);
    }
    return result;
}

int dispatch(BinOp *table, int op, int a, int b) {
    return table[op](a, b);
}

int main() {
    // Array of function pointers
    BinOp ops[4];
    ops[0] = add;
    ops[1] = sub;
    ops[2] = mul;
    ops[3] = divide;

    // Call through array
    // EXPECT: ops[0](10,3) = 13
    printf("ops[0](10,3) = %d\n", ops[0](10, 3));
    // EXPECT: ops[1](10,3) = 7
    printf("ops[1](10,3) = %d\n", ops[1](10, 3));
    // EXPECT: ops[2](10,3) = 30
    printf("ops[2](10,3) = %d\n", ops[2](10, 3));
    // EXPECT: ops[3](10,3) = 3
    printf("ops[3](10,3) = %d\n", ops[3](10, 3));

    // Dispatch via index
    int idx = 2;
    // EXPECT: dispatch mul: 56
    printf("dispatch mul: %d\n", ops[idx](7, 8));

    // Use apply() with function pointer
    // EXPECT: apply add: 150
    printf("apply add: %d\n", apply(add, 100, 50));
    // EXPECT: apply sub: 50
    printf("apply sub: %d\n", apply(sub, 100, 50));
    // EXPECT: apply mul: 600
    printf("apply mul: %d\n", apply(mul, 30, 20));

    // Loop through all ops with names
    int i;
    char *names[4];
    names[0] = "add";
    names[1] = "sub";
    names[2] = "mul";
    names[3] = "div";

    for (i = 0; i < 4; i++) {
        int result = ops[i](20, 4);
        // EXPECT: op 0 add(20,4) = 24
        // EXPECT: op 1 sub(20,4) = 16
        // EXPECT: op 2 mul(20,4) = 80
        // EXPECT: op 3 div(20,4) = 5
        printf("op %d %s(20,4) = %d\n", i, names[i], result);
    }

    // Apply chain: add 10 three times starting from 0
    BinOp add_ops[3];
    add_ops[0] = add;
    add_ops[1] = add;
    add_ops[2] = add;
    int chain = apply_all(add_ops, 3, 0, 10);
    // EXPECT: chain add: 30
    printf("chain add: %d\n", chain);

    // Chain: multiply by 2 three times (1 -> 2 -> 4 -> 8)
    BinOp mul_ops[3];
    mul_ops[0] = mul;
    mul_ops[1] = mul;
    mul_ops[2] = mul;
    int mchain = apply_all(mul_ops, 3, 1, 2);
    // EXPECT: chain mul: 8
    printf("chain mul: %d\n", mchain);

    // Dispatch table with extended ops
    BinOp table[6];
    table[0] = add;
    table[1] = sub;
    table[2] = mul;
    table[3] = divide;
    table[4] = max_of;
    table[5] = min_of;

    // EXPECT: dispatch add: 15
    printf("dispatch add: %d\n", dispatch(table, 0, 10, 5));
    // EXPECT: dispatch sub: 5
    printf("dispatch sub: %d\n", dispatch(table, 1, 10, 5));
    // EXPECT: dispatch max: 10
    printf("dispatch max: %d\n", dispatch(table, 4, 10, 5));
    // EXPECT: dispatch min: 5
    printf("dispatch min: %d\n", dispatch(table, 5, 10, 5));

    // Switch-based dispatch vs function pointer dispatch
    int op_code = 1;
    int switch_result;
    switch (op_code) {
        case 0: switch_result = 50 + 25; break;
        case 1: switch_result = 50 - 25; break;
        case 2: switch_result = 50 * 25; break;
        default: switch_result = 0; break;
    }
    int fp_result = ops[op_code](50, 25);
    // EXPECT: switch result: 25
    printf("switch result: %d\n", switch_result);
    // EXPECT: fp result: 25
    printf("fp result: %d\n", fp_result);
    // EXPECT: match: 1
    printf("match: %d\n", switch_result == fp_result);

    // Conditional function pointer selection
    int use_add = 1;
    BinOp chosen;
    if (use_add) {
        chosen = add;
    } else {
        chosen = sub;
    }
    // EXPECT: chosen result: 42
    printf("chosen result: %d\n", chosen(30, 12));

    // Store result of each op in array
    int results[4];
    for (i = 0; i < 4; i++) {
        results[i] = ops[i](100, 10);
    }
    // EXPECT: results: 110 90 1000 10
    printf("results: %d %d %d %d\n", results[0], results[1], results[2], results[3]);

    // EXPECT: func_ptr_array done
    printf("func_ptr_array done\n");

    return 0;
}
