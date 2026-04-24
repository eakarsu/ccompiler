int printf(const char *fmt, ...);

// Deeply nested function calls (10+ deep)

int depth10(int x) {
    return x + 10;
}

int depth9(int x) {
    return depth10(x * 2) + 9;
}

int depth8(int x) {
    return depth9(x + 1) + 8;
}

int depth7(int x) {
    return depth8(x * 3) + 7;
}

int depth6(int x) {
    return depth7(x - 2) + 6;
}

int depth5(int x) {
    return depth6(x + 5) + 5;
}

int depth4(int x) {
    return depth5(x * 2) + 4;
}

int depth3(int x) {
    return depth4(x + 3) + 3;
}

int depth2(int x) {
    return depth3(x - 1) + 2;
}

int depth1(int x) {
    return depth2(x + 4) + 1;
}

int depth0(int x) {
    return depth1(x) + 0;
}

// Another chain with accumulator
int chain_a(int x, int acc);
int chain_b(int x, int acc);
int chain_c(int x, int acc);
int chain_d(int x, int acc);
int chain_e(int x, int acc);
int chain_f(int x, int acc);
int chain_g(int x, int acc);
int chain_h(int x, int acc);
int chain_i(int x, int acc);
int chain_j(int x, int acc);
int chain_k(int x, int acc);
int chain_l(int x, int acc);

int chain_l(int x, int acc) {
    return acc + x;
}

int chain_k(int x, int acc) {
    return chain_l(x + 1, acc + x);
}

int chain_j(int x, int acc) {
    return chain_k(x + 1, acc + x);
}

int chain_i(int x, int acc) {
    return chain_j(x + 1, acc + x);
}

int chain_h(int x, int acc) {
    return chain_i(x + 1, acc + x);
}

int chain_g(int x, int acc) {
    return chain_h(x + 1, acc + x);
}

int chain_f(int x, int acc) {
    return chain_g(x + 1, acc + x);
}

int chain_e(int x, int acc) {
    return chain_f(x + 1, acc + x);
}

int chain_d(int x, int acc) {
    return chain_e(x + 1, acc + x);
}

int chain_c(int x, int acc) {
    return chain_d(x + 1, acc + x);
}

int chain_b(int x, int acc) {
    return chain_c(x + 1, acc + x);
}

int chain_a(int x, int acc) {
    return chain_b(x + 1, acc + x);
}

// Recursive deep nesting
int recursive_deep(int n, int acc) {
    if (n <= 0) return acc;
    return recursive_deep(n - 1, acc + n);
}

// Mutual recursion
int even_check(int n);
int odd_check(int n);

int even_check(int n) {
    if (n == 0) return 1;
    return odd_check(n - 1);
}

int odd_check(int n) {
    if (n == 0) return 0;
    return even_check(n - 1);
}

int main() {
    int r1 = depth0(1);
    printf("depth chain 1: %d\n", r1);
    // EXPECT: depth chain 1: 159

    int r2 = depth0(0);
    printf("depth chain 0: %d\n", r2);
    // EXPECT: depth chain 0: 147

    int r3 = chain_a(1, 0);
    printf("chain acc 1: %d\n", r3);
    // EXPECT: chain acc 1: 78

    int r4 = chain_a(0, 0);
    printf("chain acc 0: %d\n", r4);
    // EXPECT: chain acc 0: 66

    int r5 = chain_a(5, 10);
    printf("chain acc 5,10: %d\n", r5);
    // EXPECT: chain acc 5,10: 136

    int r6 = recursive_deep(15, 0);
    printf("recursive 15: %d\n", r6);
    // EXPECT: recursive 15: 120

    int r7 = recursive_deep(100, 0);
    printf("recursive 100: %d\n", r7);
    // EXPECT: recursive 100: 5050

    int r8 = even_check(10);
    printf("even 10: %d\n", r8);
    // EXPECT: even 10: 1

    int r9 = odd_check(7);
    printf("odd 7: %d\n", r9);
    // EXPECT: odd 7: 1

    int r10 = even_check(3);
    printf("even 3: %d\n", r10);
    // EXPECT: even 3: 0

    return 0;
}
