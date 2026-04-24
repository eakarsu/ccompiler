int printf(const char *fmt, ...);

int level10(int x) {
    return x + 10;
}

int level9(int x) {
    return level10(x + 9);
}

int level8(int x) {
    return level9(x + 8);
}

int level7(int x) {
    return level8(x + 7);
}

int level6(int x) {
    return level7(x + 6);
}

int level5(int x) {
    return level6(x + 5);
}

int level4(int x) {
    return level5(x + 4);
}

int level3(int x) {
    return level4(x + 3);
}

int level2(int x) {
    return level3(x + 2);
}

int level1(int x) {
    return level2(x + 1);
}

int accumulate_down(int depth, int acc) {
    if (depth <= 0) return acc;
    return accumulate_down(depth - 1, acc + depth);
}

int chain_add(int a) {
    return a + 1;
}

int chain_mul(int a) {
    return chain_add(a) * 2;
}

int chain_sub(int a) {
    return chain_mul(a) - 3;
}

int chain_mod(int a) {
    return chain_sub(a) + 7;
}

int chain_final(int a) {
    return chain_mod(a) + 100;
}

int pass_through_1(int x) { return x; }
int pass_through_2(int x) { return pass_through_1(x); }
int pass_through_3(int x) { return pass_through_2(x); }
int pass_through_4(int x) { return pass_through_3(x); }
int pass_through_5(int x) { return pass_through_4(x); }
int pass_through_6(int x) { return pass_through_5(x); }
int pass_through_7(int x) { return pass_through_6(x); }
int pass_through_8(int x) { return pass_through_7(x); }
int pass_through_9(int x) { return pass_through_8(x); }
int pass_through_10(int x) { return pass_through_9(x); }
int pass_through_11(int x) { return pass_through_10(x); }
int pass_through_12(int x) { return pass_through_11(x); }

int deep_multiply(int depth, int val) {
    if (depth <= 0) return val;
    return deep_multiply(depth - 1, val * 2);
}

int deep_subtract(int depth, int val) {
    if (depth <= 0) return val;
    return deep_subtract(depth - 1, val - 1);
}

int combined_deep(int x) {
    int a = deep_multiply(3, x);
    int b = deep_subtract(5, a);
    return b;
}

int main(void) {
    // EXPECT: deep10: 55
    printf("deep10: %d\n", level1(0));

    // EXPECT: deep10_from5: 60
    printf("deep10_from5: %d\n", level1(5));

    // EXPECT: accum10: 55
    printf("accum10: %d\n", accumulate_down(10, 0));

    // EXPECT: accum5: 15
    printf("accum5: %d\n", accumulate_down(5, 0));

    // EXPECT: accum0: 0
    printf("accum0: %d\n", accumulate_down(0, 0));

    // EXPECT: chain3: 112
    printf("chain3: %d\n", chain_final(3));

    // EXPECT: chain0: 106
    printf("chain0: %d\n", chain_final(0));

    // EXPECT: pass42: 42
    printf("pass42: %d\n", pass_through_12(42));

    // EXPECT: pass0: 0
    printf("pass0: %d\n", pass_through_12(0));

    // EXPECT: pass_neg: -7
    printf("pass_neg: %d\n", pass_through_12(-7));

    // EXPECT: deepmul: 80
    printf("deepmul: %d\n", deep_multiply(3, 10));

    // EXPECT: deepsub: 5
    printf("deepsub: %d\n", deep_subtract(5, 10));

    // EXPECT: combined2: 11
    printf("combined2: %d\n", combined_deep(2));

    // EXPECT: combined1: 3
    printf("combined1: %d\n", combined_deep(1));

    // EXPECT: accum_start100: 155
    printf("accum_start100: %d\n", accumulate_down(10, 100));

    // EXPECT: level5_direct: 45
    printf("level5_direct: %d\n", level5(0));

    // EXPECT: deepmul0: 0
    printf("deepmul0: %d\n", deep_multiply(10, 0));

    return 0;
}
