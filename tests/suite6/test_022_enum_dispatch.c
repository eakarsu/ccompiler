int printf(const char *fmt, ...);

/* Enum-based dispatch with switch */
enum Op {
    OP_ADD = 0,
    OP_SUB = 1,
    OP_MUL = 2,
    OP_DIV = 3,
    OP_MOD = 4,
    OP_NEG = 5,
    OP_COUNT = 6
};

char *op_to_string(enum Op op) {
    switch (op) {
        case OP_ADD: return "ADD";
        case OP_SUB: return "SUB";
        case OP_MUL: return "MUL";
        case OP_DIV: return "DIV";
        case OP_MOD: return "MOD";
        case OP_NEG: return "NEG";
        default:     return "UNKNOWN";
    }
}

int dispatch_binary(enum Op op, int a, int b) {
    switch (op) {
        case OP_ADD: return a + b;
        case OP_SUB: return a - b;
        case OP_MUL: return a * b;
        case OP_DIV: return b != 0 ? a / b : 0;
        case OP_MOD: return b != 0 ? a % b : 0;
        default:     return 0;
    }
}

int dispatch_unary(enum Op op, int a) {
    switch (op) {
        case OP_NEG: return -a;
        default:     return a;
    }
}

int is_binary_op(enum Op op) {
    return op >= OP_ADD && op <= OP_MOD;
}

int is_commutative(enum Op op) {
    return op == OP_ADD || op == OP_MUL;
}

/* Enum arithmetic: next/previous op */
int next_op(int op) {
    if (op + 1 < OP_COUNT) return op + 1;
    return 0;
}

int prev_op(int op) {
    if (op > 0) return op - 1;
    return OP_COUNT - 1;
}

/* Apply chain of operations */
int apply_chain(int ops[], int count, int start) {
    int result = start;
    int i;
    for (i = 0; i < count; i++) {
        result = dispatch_binary(ops[i], result, i + 1);
    }
    return result;
}

/* Count how many ops produce a positive result for given a, b */
int count_positive_results(int a, int b) {
    int count = 0;
    int op;
    for (op = OP_ADD; op <= OP_MOD; op++) {
        if (dispatch_binary(op, a, b) > 0) count++;
    }
    return count;
}

int main(void) {
    /* Test op_to_string */
    // EXPECT: Op: ADD
    printf("Op: %s\n", op_to_string(OP_ADD));
    // EXPECT: Op: SUB
    printf("Op: %s\n", op_to_string(OP_SUB));
    // EXPECT: Op: MUL
    printf("Op: %s\n", op_to_string(OP_MUL));
    // EXPECT: Op: DIV
    printf("Op: %s\n", op_to_string(OP_DIV));
    // EXPECT: Op: MOD
    printf("Op: %s\n", op_to_string(OP_MOD));
    // EXPECT: Op: NEG
    printf("Op: %s\n", op_to_string(OP_NEG));

    /* Test dispatch_binary */
    // EXPECT: 10 + 3 = 13
    printf("10 + 3 = %d\n", dispatch_binary(OP_ADD, 10, 3));
    // EXPECT: 10 - 3 = 7
    printf("10 - 3 = %d\n", dispatch_binary(OP_SUB, 10, 3));
    // EXPECT: 10 * 3 = 30
    printf("10 * 3 = %d\n", dispatch_binary(OP_MUL, 10, 3));
    // EXPECT: 10 / 3 = 3
    printf("10 / 3 = %d\n", dispatch_binary(OP_DIV, 10, 3));
    // EXPECT: 10 % 3 = 1
    printf("10 %% 3 = %d\n", dispatch_binary(OP_MOD, 10, 3));

    /* Test dispatch_unary */
    // EXPECT: -42 = -42
    printf("-42 = %d\n", dispatch_unary(OP_NEG, 42));

    /* Test is_binary_op */
    // EXPECT: ADD binary: 1
    printf("ADD binary: %d\n", is_binary_op(OP_ADD));
    // EXPECT: NEG binary: 0
    printf("NEG binary: %d\n", is_binary_op(OP_NEG));

    /* Test is_commutative */
    // EXPECT: ADD commutative: 1
    printf("ADD commutative: %d\n", is_commutative(OP_ADD));
    // EXPECT: SUB commutative: 0
    printf("SUB commutative: %d\n", is_commutative(OP_SUB));
    // EXPECT: MUL commutative: 1
    printf("MUL commutative: %d\n", is_commutative(OP_MUL));

    /* Test next/prev op */
    // EXPECT: Next ADD: SUB
    printf("Next ADD: %s\n", op_to_string(next_op(OP_ADD)));
    // EXPECT: Next NEG: ADD
    printf("Next NEG: %s\n", op_to_string(next_op(OP_NEG)));
    // EXPECT: Prev ADD: NEG
    printf("Prev ADD: %s\n", op_to_string(prev_op(OP_ADD)));

    /* Test operation chain: start=0, ops=[ADD,ADD,MUL,ADD]
       0 + 1 = 1, 1 + 2 = 3, 3 * 3 = 9, 9 + 4 = 13 */
    int chain[4];
    chain[0] = OP_ADD;
    chain[1] = OP_ADD;
    chain[2] = OP_MUL;
    chain[3] = OP_ADD;
    // EXPECT: Chain result: 13
    printf("Chain result: %d\n", apply_chain(chain, 4, 0));

    /* Count positive results for 10, 3 */
    // EXPECT: Positive ops(10,3): 5
    printf("Positive ops(10,3): %d\n", count_positive_results(10, 3));

    /* Iterate all ops and print names */
    int op;
    for (op = 0; op < OP_COUNT; op++) {
        printf("Op %d: %s\n", op, op_to_string(op));
    }
    // EXPECT: Op 0: ADD
    // EXPECT: Op 1: SUB
    // EXPECT: Op 2: MUL
    // EXPECT: Op 3: DIV
    // EXPECT: Op 4: MOD
    // EXPECT: Op 5: NEG

    /* Div by zero safety */
    // EXPECT: Div0: 0
    printf("Div0: %d\n", dispatch_binary(OP_DIV, 10, 0));

    return 0;
}
