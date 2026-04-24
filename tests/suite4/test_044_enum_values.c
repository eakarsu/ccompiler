int printf(const char *fmt, ...);

enum Explicit {
    A = 10,
    B = 20,
    C = 30
};

enum Negative {
    NEG3 = -3,
    NEG2 = -2,
    NEG1 = -1,
    ZERO = 0,
    POS1 = 1
};

enum NonSequential {
    VAL_FIRST = 5,
    VAL_SECOND = 100,
    VAL_THIRD = 42,
    VAL_FOURTH = 7
};

enum Mixed {
    M_START = 0,
    M_AUTO1,     // 1
    M_AUTO2,     // 2
    M_JUMP = 10,
    M_AUTO3,     // 11
    M_AUTO4      // 12
};

enum Large {
    BIG1 = 1000,
    BIG2 = 2000,
    BIG3 = 3000
};

int test_explicit_values(void) {
    return A + B + C; // 10 + 20 + 30 = 60
}

int test_negative_values(void) {
    return NEG3 + NEG2 + NEG1 + ZERO + POS1;
    // -3 + -2 + -1 + 0 + 1 = -5
}

int test_non_sequential(void) {
    return VAL_FIRST + VAL_SECOND + VAL_THIRD + VAL_FOURTH;
    // 5 + 100 + 42 + 7 = 154
}

int test_mixed_auto(void) {
    return M_START + M_AUTO1 + M_AUTO2 + M_JUMP + M_AUTO3 + M_AUTO4;
    // 0 + 1 + 2 + 10 + 11 + 12 = 36
}

int test_enum_arithmetic_add(void) {
    enum Explicit x = A; // 10
    int result = x + 5;  // 15
    return result;
}

int test_enum_arithmetic_mul(void) {
    enum Explicit x = B; // 20
    return x * 3;        // 60
}

int test_negative_switch(enum Negative n) {
    switch (n) {
        case NEG3: return 30;
        case NEG2: return 20;
        case NEG1: return 10;
        case ZERO: return 0;
        case POS1: return -10;
    }
    return -99;
}

int test_negative_comparison(void) {
    if (NEG3 < ZERO) return 1;
    return 0;
}

int test_negative_arithmetic(void) {
    int x = NEG3 * NEG2; // -3 * -2 = 6
    return x;
}

int test_enum_in_array(void) {
    int arr[5];
    arr[0] = A;    // 10
    arr[1] = B;    // 20
    arr[2] = C;    // 30
    arr[3] = NEG1; // -1
    arr[4] = POS1; // 1
    int sum = 0;
    int i;
    for (i = 0; i < 5; i = i + 1) {
        sum = sum + arr[i];
    }
    return sum; // 10 + 20 + 30 + (-1) + 1 = 60
}

int test_large_values(void) {
    return BIG1 + BIG2 + BIG3; // 1000 + 2000 + 3000 = 6000
}

int test_enum_subtract(void) {
    return C - A; // 30 - 10 = 20
}

int test_enum_modulo(void) {
    return VAL_SECOND % VAL_THIRD; // 100 % 42 = 16
}

int main(void) {
    // EXPECT: A: 10
    printf("A: %d\n", A);

    // EXPECT: B: 20
    printf("B: %d\n", B);

    // EXPECT: C: 30
    printf("C: %d\n", C);

    // EXPECT: explicit sum: 60
    printf("explicit sum: %d\n", test_explicit_values());

    // EXPECT: NEG3: -3
    printf("NEG3: %d\n", NEG3);

    // EXPECT: NEG1: -1
    printf("NEG1: %d\n", NEG1);

    // EXPECT: negative sum: -5
    printf("negative sum: %d\n", test_negative_values());

    // EXPECT: non-sequential sum: 154
    printf("non-sequential sum: %d\n", test_non_sequential());

    // EXPECT: M_AUTO1: 1
    printf("M_AUTO1: %d\n", M_AUTO1);

    // EXPECT: M_JUMP: 10
    printf("M_JUMP: %d\n", M_JUMP);

    // EXPECT: M_AUTO3: 11
    printf("M_AUTO3: %d\n", M_AUTO3);

    // EXPECT: mixed auto sum: 36
    printf("mixed auto sum: %d\n", test_mixed_auto());

    // EXPECT: enum add: 15
    printf("enum add: %d\n", test_enum_arithmetic_add());

    // EXPECT: enum mul: 60
    printf("enum mul: %d\n", test_enum_arithmetic_mul());

    // EXPECT: neg switch NEG3: 30
    printf("neg switch NEG3: %d\n", test_negative_switch(NEG3));

    // EXPECT: neg switch ZERO: 0
    printf("neg switch ZERO: %d\n", test_negative_switch(ZERO));

    // EXPECT: neg comparison: 1
    printf("neg comparison: %d\n", test_negative_comparison());

    // EXPECT: neg arithmetic: 6
    printf("neg arithmetic: %d\n", test_negative_arithmetic());

    // EXPECT: enum in array: 60
    printf("enum in array: %d\n", test_enum_in_array());

    // EXPECT: large values: 6000
    printf("large values: %d\n", test_large_values());

    // EXPECT: enum subtract: 20
    printf("enum subtract: %d\n", test_enum_subtract());

    // EXPECT: enum modulo: 16
    printf("enum modulo: %d\n", test_enum_modulo());

    return 0;
}
