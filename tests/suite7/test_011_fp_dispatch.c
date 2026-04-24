int printf(const char *fmt, ...);

enum Op {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_COUNT
};

typedef int (*BinOp)(int, int);

int op_add(int a, int b) { return a + b; }
int op_sub(int a, int b) { return a - b; }
int op_mul(int a, int b) { return a * b; }
int op_div(int a, int b) { return a / b; }
int op_mod(int a, int b) { return a % b; }

int dispatch(BinOp table[], int op, int a, int b) {
    return table[op](a, b);
}

int apply_all(BinOp table[], int count, int a, int b, int results[]) {
    int i;
    for (i = 0; i < count; i++) {
        results[i] = table[i](a, b);
    }
    return count;
}

int find_op_result(BinOp table[], int count, int a, int b, int target) {
    int i;
    for (i = 0; i < count; i++) {
        if (table[i](a, b) == target) {
            return i;
        }
    }
    return -1;
}

int accumulate(BinOp table[], int ops[], int n, int init) {
    int result;
    int i;
    result = init;
    for (i = 0; i < n; i++) {
        result = table[ops[i]](result, i + 1);
    }
    return result;
}

int count_positive_results(BinOp table[], int count, int a, int b) {
    int pos;
    int i;
    pos = 0;
    for (i = 0; i < count; i++) {
        if (table[i](a, b) > 0) {
            pos = pos + 1;
        }
    }
    return pos;
}

int max_result(BinOp table[], int count, int a, int b) {
    int best;
    int val;
    int i;
    best = table[0](a, b);
    for (i = 1; i < count; i++) {
        val = table[i](a, b);
        if (val > best) {
            best = val;
        }
    }
    return best;
}

int main(void) {
    BinOp table[5];
    int results[5];
    int ops[4];
    int n;
    int val;

    table[OP_ADD] = op_add;
    table[OP_SUB] = op_sub;
    table[OP_MUL] = op_mul;
    table[OP_DIV] = op_div;
    table[OP_MOD] = op_mod;

    // EXPECT: add(10,3) = 13
    printf("add(10,3) = %d\n", dispatch(table, OP_ADD, 10, 3));
    // EXPECT: sub(10,3) = 7
    printf("sub(10,3) = %d\n", dispatch(table, OP_SUB, 10, 3));
    // EXPECT: mul(10,3) = 30
    printf("mul(10,3) = %d\n", dispatch(table, OP_MUL, 10, 3));
    // EXPECT: div(10,3) = 3
    printf("div(10,3) = %d\n", dispatch(table, OP_DIV, 10, 3));
    // EXPECT: mod(10,3) = 1
    printf("mod(10,3) = %d\n", dispatch(table, OP_MOD, 10, 3));

    n = apply_all(table, 5, 20, 6, results);
    // EXPECT: applied 5 ops
    printf("applied %d ops\n", n);
    // EXPECT: results: 26 14 120 3 2
    printf("results: %d %d %d %d %d\n", results[0], results[1], results[2], results[3], results[4]);

    val = find_op_result(table, 5, 20, 6, 120);
    // EXPECT: op for 120: 2
    printf("op for 120: %d\n", val);

    val = find_op_result(table, 5, 20, 6, 999);
    // EXPECT: op for 999: -1
    printf("op for 999: %d\n", val);

    ops[0] = OP_ADD;
    ops[1] = OP_MUL;
    ops[2] = OP_ADD;
    ops[3] = OP_SUB;
    val = accumulate(table, ops, 4, 0);
    // EXPECT: accumulate = 1
    printf("accumulate = %d\n", val);

    val = count_positive_results(table, 5, 7, 3);
    // EXPECT: positive results: 5
    printf("positive results: %d\n", val);

    val = count_positive_results(table, 5, 3, 7);
    // EXPECT: positive results(3,7): 3
    printf("positive results(3,7): %d\n", val);

    val = max_result(table, 5, 8, 3);
    // EXPECT: max result(8,3): 24
    printf("max result(8,3): %d\n", val);

    val = max_result(table, 5, 2, 10);
    // EXPECT: max result(2,10): 20
    printf("max result(2,10): %d\n", val);

    val = dispatch(table, OP_ADD, -5, 3);
    // EXPECT: add(-5,3) = -2
    printf("add(-5,3) = %d\n", val);

    val = find_op_result(table, 5, 12, 4, 3);
    // EXPECT: op for 12,4->3: 3
    printf("op for 12,4->3: %d\n", val);

    return 0;
}
