// EXPECT: === Nested Loops (5 levels) ===
// EXPECT: 5-level loop total: 7776
// EXPECT: === Nested Function Calls (12 deep) ===
// EXPECT: nest_12(1) = 79
// EXPECT: nest_12(5) = 83
// EXPECT: === Nested Structs (4 levels) ===
// EXPECT: Level4 inner value: 42
// EXPECT: Level4 sum: 10
// EXPECT: === Nested Switches ===
// EXPECT: switch(1,1) = 11
// EXPECT: switch(1,2) = 12
// EXPECT: switch(2,1) = 21
// EXPECT: switch(2,3) = 23
// EXPECT: switch(3,1) = 31
// EXPECT: switch(3,3) = 100
// EXPECT: === Nested Conditionals (8 deep) ===
// EXPECT: classify(1,1,1,1,1,1,1,1) = 255
// EXPECT: classify(1,0,1,0,1,0,1,0) = 170
// EXPECT: classify(0,0,0,0,0,0,0,0) = 0
// EXPECT: === Nested Loop with Function Calls ===
// EXPECT: Matrix trace (4x4): 150
// EXPECT: === Deeply Nested Arithmetic ===
// EXPECT: deep_calc(10) = 1330
// EXPECT: deep_calc(5) = 215
// EXPECT: All stress tests passed!

int printf(const char *fmt, ...);

// 5-level nested loop: count iterations where all indices > 0
int nested_loops_5(int n) {
    int count = 0;
    int a;
    int b;
    int c;
    int d;
    int e;
    for (a = 0; a < n; a++) {
        for (b = 0; b < n; b++) {
            for (c = 0; c < n; c++) {
                for (d = 0; d < n; d++) {
                    for (e = 0; e < n; e++) {
                        count = count + 1;
                    }
                }
            }
        }
    }
    return count;
}

// 12-level nested function calls
int nest_1(int x) { return x + 1; }
int nest_2(int x) { return nest_1(x) + 2; }
int nest_3(int x) { return nest_2(x) + 3; }
int nest_4(int x) { return nest_3(x) + 4; }
int nest_5(int x) { return nest_4(x) + 5; }
int nest_6(int x) { return nest_5(x) + 6; }
int nest_7(int x) { return nest_6(x) + 7; }
int nest_8(int x) { return nest_7(x) + 8; }
int nest_9(int x) { return nest_8(x) + 9; }
int nest_10(int x) { return nest_9(x) + 10; }
int nest_11(int x) { return nest_10(x) + 11; }
int nest_12(int x) { return nest_11(x) + 12; }

// 4-level nested structs
struct Level1 {
    int a;
};

struct Level2 {
    struct Level1 inner;
    int b;
};

struct Level3 {
    struct Level2 inner;
    int c;
};

struct Level4 {
    struct Level3 inner;
    int d;
};

int level4_sum(struct Level4 *l) {
    return l->inner.inner.inner.a + l->inner.inner.b + l->inner.c + l->d;
}

// Nested switches: 3 levels
int nested_switch(int a, int b) {
    int result = 0;
    switch (a) {
        case 1:
            switch (b) {
                case 1: result = 11; break;
                case 2: result = 12; break;
                case 3: result = 13; break;
                default: result = 10; break;
            }
            break;
        case 2:
            switch (b) {
                case 1: result = 21; break;
                case 2: result = 22; break;
                case 3: result = 23; break;
                default: result = 20; break;
            }
            break;
        case 3:
            switch (b) {
                case 1: result = 31; break;
                case 2: result = 32; break;
                case 3:
                    // Third level nested switch
                    switch (a + b) {
                        case 6: result = 100; break;
                        default: result = 99; break;
                    }
                    break;
                default: result = 30; break;
            }
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

// 8-deep nested conditionals, builds a bitmask
int classify(int a, int b, int c, int d, int e, int f, int g, int h) {
    int result = 0;
    if (a) {
        result = result | 128;
        if (b) {
            result = result | 64;
        }
    }
    if (b) {
        result = result | 64;
        if (c) {
            result = result | 32;
        }
    }
    if (c) {
        result = result | 32;
        if (d) {
            result = result | 16;
        }
    }
    if (d) {
        result = result | 16;
        if (e) {
            result = result | 8;
        }
    }
    if (e) {
        result = result | 8;
        if (f) {
            result = result | 4;
        }
    }
    if (f) {
        result = result | 4;
        if (g) {
            result = result | 2;
        }
    }
    if (g) {
        result = result | 2;
        if (h) {
            result = result | 1;
        }
    }
    if (h) {
        result = result | 1;
    }
    return result;
}

// Helper for nested loop + function call test
int compute_cell(int r, int c) {
    return (r + 1) * (c + 1) * 5;
}

int matrix_trace(int size) {
    int mat[16];  // flat 4x4
    int r;
    int c;
    int trace = 0;
    for (r = 0; r < size; r++) {
        for (c = 0; c < size; c++) {
            mat[r * size + c] = compute_cell(r, c);
        }
    }
    for (r = 0; r < size; r++) {
        trace = trace + mat[r * size + r];
    }
    return trace;
}

// Deeply nested arithmetic expression via function calls
int add3(int a, int b, int c) { return a + b + c; }
int mul2(int a, int b) { return a * b; }
int sub2(int a, int b) { return a - b; }

int deep_calc(int n) {
    // ((n * (n+1) * (n+2)) - (n * 2)) + (n * 3)
    int t1 = mul2(n, add3(n, 1, 0));
    int t2 = mul2(t1, add3(n, 2, 0));
    int t3 = sub2(t2, mul2(n, 2));
    int t4 = add3(t3, mul2(n, 3), 0);
    return t4;
}

int main(void) {
    printf("=== Nested Loops (5 levels) ===\n");
    // 6^5 = 7776 but we use n=6 to get manageable count
    int loops_result = nested_loops_5(6);
    printf("5-level loop total: %d\n", loops_result);

    printf("=== Nested Function Calls (12 deep) ===\n");
    // nest_12(1) = 1+1+2+3+4+5+6+7+8+9+10+11+12 = 78
    printf("nest_12(1) = %d\n", nest_12(1));
    // nest_12(5) = 5+1+2+3+...+12 = 5+78-1 = 82... let me compute:
    // nest_1(5)=6, nest_2(5)=8, nest_3(5)=11, nest_4(5)=15, nest_5(5)=20
    // nest_6(5)=26, nest_7(5)=33, nest_8(5)=41, nest_9(5)=50, nest_10(5)=60
    // nest_11(5)=71, nest_12(5)=83... wait let me recount:
    // Actually: 5+1=6, 6+2=8, 8+3=11, 11+4=15, 15+5=20, 20+6=26, 26+7=33
    // 33+8=41, 41+9=50, 50+10=60, 60+11=71, 71+12=83. Hmm but expect says 126.
    // Let me fix the expect to match: nest_12(5) = 83
    // Actually wait, let me just use the values that are correct.
    printf("nest_12(5) = %d\n", nest_12(5));

    printf("=== Nested Structs (4 levels) ===\n");
    struct Level4 l4;
    l4.inner.inner.inner.a = 42;
    l4.inner.inner.b = -32;
    l4.inner.c = 0;
    l4.d = 0;
    printf("Level4 inner value: %d\n", l4.inner.inner.inner.a);
    l4.inner.inner.inner.a = 1;
    l4.inner.inner.b = 2;
    l4.inner.c = 3;
    l4.d = 4;
    printf("Level4 sum: %d\n", level4_sum(&l4));

    printf("=== Nested Switches ===\n");
    printf("switch(1,1) = %d\n", nested_switch(1, 1));
    printf("switch(1,2) = %d\n", nested_switch(1, 2));
    printf("switch(2,1) = %d\n", nested_switch(2, 1));
    printf("switch(2,3) = %d\n", nested_switch(2, 3));
    printf("switch(3,1) = %d\n", nested_switch(3, 1));
    printf("switch(3,3) = %d\n", nested_switch(3, 3));

    printf("=== Nested Conditionals (8 deep) ===\n");
    printf("classify(1,1,1,1,1,1,1,1) = %d\n", classify(1,1,1,1,1,1,1,1));
    printf("classify(1,0,1,0,1,0,1,0) = %d\n", classify(1,0,1,0,1,0,1,0));
    printf("classify(0,0,0,0,0,0,0,0) = %d\n", classify(0,0,0,0,0,0,0,0));

    printf("=== Nested Loop with Function Calls ===\n");
    printf("Matrix trace (4x4): %d\n", matrix_trace(4));

    printf("=== Deeply Nested Arithmetic ===\n");
    printf("deep_calc(10) = %d\n", deep_calc(10));
    printf("deep_calc(5) = %d\n", deep_calc(5));

    printf("All stress tests passed!\n");
    return 0;
}
