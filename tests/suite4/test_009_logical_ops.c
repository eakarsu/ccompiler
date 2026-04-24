int printf(const char *fmt, ...);

// Test logical operators: &&, ||, !
// with various truthy/falsy values and short-circuit evaluation.

int global_counter;

int increment_and_return(int val) {
    global_counter = global_counter + 1;
    return val;
}

int logical_and(int a, int b) { return a && b; }
int logical_or(int a, int b) { return a || b; }
int logical_not(int a) { return !a; }

int both_positive(int a, int b) {
    return a > 0 && b > 0;
}

int either_zero(int a, int b) {
    return a == 0 || b == 0;
}

int none_negative(int a, int b, int c) {
    return a >= 0 && b >= 0 && c >= 0;
}

int any_negative(int a, int b, int c) {
    return a < 0 || b < 0 || c < 0;
}

int xor_logical(int a, int b) {
    return (a && !b) || (!a && b);
}

int implies(int a, int b) {
    // a implies b: !a || b
    return !a || b;
}

int exactly_one(int a, int b, int c) {
    int count = 0;
    if (a) count = count + 1;
    if (b) count = count + 1;
    if (c) count = count + 1;
    return count == 1;
}

int all_true(int a, int b, int c, int d) {
    return a && b && c && d;
}

int any_true(int a, int b, int c, int d) {
    return a || b || c || d;
}

int chain_and(int *arr, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (!arr[i]) return 0;
    }
    return 1;
}

int chain_or(int *arr, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (arr[i]) return 1;
    }
    return 0;
}

int main(void) {
    // Basic AND
    // EXPECT: 1 && 1 = 1
    printf("%d && %d = %d\n", 1, 1, logical_and(1, 1));

    // EXPECT: 1 && 0 = 0
    printf("%d && %d = %d\n", 1, 0, logical_and(1, 0));

    // EXPECT: 0 && 1 = 0
    printf("%d && %d = %d\n", 0, 1, logical_and(0, 1));

    // EXPECT: 0 && 0 = 0
    printf("%d && %d = %d\n", 0, 0, logical_and(0, 0));

    // Non-zero values as truthy
    // EXPECT: 5 && 3 = 1
    printf("%d && %d = %d\n", 5, 3, logical_and(5, 3));

    // EXPECT: -1 && 1 = 1
    printf("%d && %d = %d\n", -1, 1, logical_and(-1, 1));

    // Basic OR
    // EXPECT: 1 || 0 = 1
    printf("%d || %d = %d\n", 1, 0, logical_or(1, 0));

    // EXPECT: 0 || 0 = 0
    printf("%d || %d = %d\n", 0, 0, logical_or(0, 0));

    // EXPECT: 0 || 5 = 1
    printf("%d || %d = %d\n", 0, 5, logical_or(0, 5));

    // Basic NOT
    // EXPECT: !0 = 1
    printf("!%d = %d\n", 0, logical_not(0));

    // EXPECT: !1 = 0
    printf("!%d = %d\n", 1, logical_not(1));

    // EXPECT: !42 = 0
    printf("!%d = %d\n", 42, logical_not(42));

    // EXPECT: !-1 = 0
    printf("!%d = %d\n", -1, logical_not(-1));

    // Double negation
    // EXPECT: !!0 = 0
    printf("!!%d = %d\n", 0, !(!0));

    // EXPECT: !!5 = 1
    printf("!!%d = %d\n", 5, !(!5));

    // Short-circuit evaluation for AND:
    // If left side is 0, right side should NOT be evaluated
    global_counter = 0;
    int r1 = 0 && increment_and_return(1);
    // EXPECT: short_and: counter=0 result=0
    printf("short_and: counter=%d result=%d\n", global_counter, r1);

    // If left side is 1, right side IS evaluated
    global_counter = 0;
    int r2 = 1 && increment_and_return(1);
    // EXPECT: eval_and: counter=1 result=1
    printf("eval_and: counter=%d result=%d\n", global_counter, r2);

    // Short-circuit evaluation for OR:
    // If left side is 1, right side should NOT be evaluated
    global_counter = 0;
    int r3 = 1 || increment_and_return(1);
    // EXPECT: short_or: counter=0 result=1
    printf("short_or: counter=%d result=%d\n", global_counter, r3);

    // If left side is 0, right side IS evaluated
    global_counter = 0;
    int r4 = 0 || increment_and_return(1);
    // EXPECT: eval_or: counter=1 result=1
    printf("eval_or: counter=%d result=%d\n", global_counter, r4);

    // Both positive
    // EXPECT: both_pos(3, 5) = 1
    printf("both_pos(%d, %d) = %d\n", 3, 5, both_positive(3, 5));

    // EXPECT: both_pos(-1, 5) = 0
    printf("both_pos(%d, %d) = %d\n", -1, 5, both_positive(-1, 5));

    // Either zero
    // EXPECT: either_zero(0, 5) = 1
    printf("either_zero(%d, %d) = %d\n", 0, 5, either_zero(0, 5));

    // EXPECT: either_zero(3, 4) = 0
    printf("either_zero(%d, %d) = %d\n", 3, 4, either_zero(3, 4));

    // None negative
    // EXPECT: none_neg(1,2,3) = 1
    printf("none_neg(%d,%d,%d) = %d\n", 1, 2, 3, none_negative(1, 2, 3));

    // EXPECT: none_neg(1,-1,3) = 0
    printf("none_neg(%d,%d,%d) = %d\n", 1, -1, 3, none_negative(1, -1, 3));

    // Any negative
    // EXPECT: any_neg(1,2,3) = 0
    printf("any_neg(%d,%d,%d) = %d\n", 1, 2, 3, any_negative(1, 2, 3));

    // EXPECT: any_neg(1,-1,3) = 1
    printf("any_neg(%d,%d,%d) = %d\n", 1, -1, 3, any_negative(1, -1, 3));

    // Logical XOR
    // EXPECT: xor(1, 0) = 1
    printf("xor(%d, %d) = %d\n", 1, 0, xor_logical(1, 0));

    // EXPECT: xor(1, 1) = 0
    printf("xor(%d, %d) = %d\n", 1, 1, xor_logical(1, 1));

    // EXPECT: xor(0, 0) = 0
    printf("xor(%d, %d) = %d\n", 0, 0, xor_logical(0, 0));

    // Implies
    // EXPECT: implies(0, 0) = 1
    printf("implies(%d, %d) = %d\n", 0, 0, implies(0, 0));

    // EXPECT: implies(1, 1) = 1
    printf("implies(%d, %d) = %d\n", 1, 1, implies(1, 1));

    // EXPECT: implies(1, 0) = 0
    printf("implies(%d, %d) = %d\n", 1, 0, implies(1, 0));

    // Exactly one true
    // EXPECT: exactly_one(1,0,0) = 1
    printf("exactly_one(%d,%d,%d) = %d\n", 1, 0, 0, exactly_one(1, 0, 0));

    // EXPECT: exactly_one(1,1,0) = 0
    printf("exactly_one(%d,%d,%d) = %d\n", 1, 1, 0, exactly_one(1, 1, 0));

    // All true / any true
    // EXPECT: all(1,1,1,1) = 1
    printf("all(%d,%d,%d,%d) = %d\n", 1, 1, 1, 1, all_true(1, 1, 1, 1));

    // EXPECT: all(1,1,0,1) = 0
    printf("all(%d,%d,%d,%d) = %d\n", 1, 1, 0, 1, all_true(1, 1, 0, 1));

    // EXPECT: any(0,0,0,1) = 1
    printf("any(%d,%d,%d,%d) = %d\n", 0, 0, 0, 1, any_true(0, 0, 0, 1));

    // EXPECT: any(0,0,0,0) = 0
    printf("any(%d,%d,%d,%d) = %d\n", 0, 0, 0, 0, any_true(0, 0, 0, 0));

    // Chain AND/OR with arrays
    int all_nonzero[4];
    all_nonzero[0] = 1;
    all_nonzero[1] = 2;
    all_nonzero[2] = 3;
    all_nonzero[3] = 4;
    // EXPECT: chain_and = 1
    printf("chain_and = %d\n", chain_and(all_nonzero, 4));

    int has_zero[4];
    has_zero[0] = 1;
    has_zero[1] = 0;
    has_zero[2] = 3;
    has_zero[3] = 4;
    // EXPECT: chain_and_z = 0
    printf("chain_and_z = %d\n", chain_and(has_zero, 4));

    // EXPECT: chain_or_z = 1
    printf("chain_or_z = %d\n", chain_or(has_zero, 4));

    int all_zero[3];
    all_zero[0] = 0;
    all_zero[1] = 0;
    all_zero[2] = 0;
    // EXPECT: chain_or_0 = 0
    printf("chain_or_0 = %d\n", chain_or(all_zero, 3));

    return 0;
}
