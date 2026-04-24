int printf(const char *fmt, ...);

// Catalan numbers and their applications

int catalan_recursive(int n) {
    int result;
    int i;
    if (n <= 1) return 1;
    result = 0;
    for (i = 0; i < n; i++) {
        result = result + catalan_recursive(i) * catalan_recursive(n - 1 - i);
    }
    return result;
}

int catalan_dp(int n) {
    int cat[20];
    int i;
    int j;
    cat[0] = 1;
    cat[1] = 1;
    for (i = 2; i <= n; i++) {
        cat[i] = 0;
        for (j = 0; j < i; j++) {
            cat[i] = cat[i] + cat[j] * cat[i - 1 - j];
        }
    }
    return cat[n];
}

int catalan_binomial(int n) {
    int result;
    int i;
    result = 1;
    for (i = 0; i < n; i++) {
        result = result * (2 * n - i);
        result = result / (i + 1);
    }
    result = result / (n + 1);
    return result;
}

int count_balanced_parens(int n) {
    return catalan_dp(n);
}

int count_bst(int n) {
    return catalan_dp(n);
}

int count_triangulations(int n) {
    if (n < 3) return 0;
    return catalan_dp(n - 2);
}

int count_paths(int n) {
    return catalan_dp(n);
}

int ballot_problem(int n) {
    if (n == 0) return 1;
    return catalan_dp(n);
}

void print_catalan_sequence(int count) {
    int i;
    int first;
    first = 1;
    for (i = 0; i < count; i++) {
        if (first) {
            printf("%d", catalan_dp(i));
            first = 0;
        } else {
            printf(" %d", catalan_dp(i));
        }
    }
    printf("\n");
}

int sum_catalan(int n) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i <= n; i++) {
        sum = sum + catalan_dp(i);
    }
    return sum;
}

int catalan_ratio_approx(int n) {
    int cn;
    int cn1;
    if (n == 0) return 0;
    cn = catalan_dp(n);
    cn1 = catalan_dp(n - 1);
    if (cn1 == 0) return 0;
    return (cn * 100) / cn1;
}

int verify_recurrence(int n) {
    int cn;
    int computed;
    int i;
    cn = catalan_dp(n);
    computed = 0;
    for (i = 0; i < n; i++) {
        computed = computed + catalan_dp(i) * catalan_dp(n - 1 - i);
    }
    return (cn == computed) ? 1 : 0;
}

int motzkin(int n) {
    int m[20];
    int i;
    int j;
    m[0] = 1;
    m[1] = 1;
    for (i = 2; i <= n; i++) {
        m[i] = m[i - 1];
        for (j = 0; j <= i - 2; j++) {
            m[i] = m[i] + m[j] * m[i - 2 - j];
        }
    }
    return m[n];
}

int main() {
    int i;
    int cr;
    int cd;
    int cb;
    int bp;
    int tp;
    int paths;
    int sc;
    int ratio;
    int ver;
    int mz;

    // EXPECT: First 11 Catalan numbers:
    printf("First 11 Catalan numbers:\n");
    // EXPECT: 1 1 2 5 14 42 132 429 1430 4862 16796
    print_catalan_sequence(11);

    cr = catalan_recursive(5);
    // EXPECT: C(5) recursive: 42
    printf("C(5) recursive: %d\n", cr);

    cd = catalan_dp(7);
    // EXPECT: C(7) dp: 429
    printf("C(7) dp: %d\n", cd);

    cb = catalan_binomial(6);
    // EXPECT: C(6) binomial: 132
    printf("C(6) binomial: %d\n", cb);

    // EXPECT: Balanced parenthesizations for 4 pairs: 14
    printf("Balanced parenthesizations for 4 pairs: %d\n", count_balanced_parens(4));

    // EXPECT: BSTs with 5 nodes: 42
    printf("BSTs with 5 nodes: %d\n", count_bst(5));

    tp = count_triangulations(6);
    // EXPECT: Triangulations of hexagon: 14
    printf("Triangulations of hexagon: %d\n", tp);

    paths = count_paths(4);
    // EXPECT: Monotonic paths in 4x4 grid: 14
    printf("Monotonic paths in 4x4 grid: %d\n", paths);

    bp = ballot_problem(3);
    // EXPECT: Ballot sequences of length 3: 5
    printf("Ballot sequences of length 3: %d\n", bp);

    sc = sum_catalan(6);
    // EXPECT: Sum C(0)..C(6): 197
    printf("Sum C(0)..C(6): %d\n", sc);

    ratio = catalan_ratio_approx(5);
    // EXPECT: C(5)/C(4) * 100: 300
    printf("C(5)/C(4) * 100: %d\n", ratio);

    ver = verify_recurrence(6);
    // EXPECT: Recurrence verified for n=6: 1
    printf("Recurrence verified for n=6: %d\n", ver);
    ver = verify_recurrence(8);
    // EXPECT: Recurrence verified for n=8: 1
    printf("Recurrence verified for n=8: %d\n", ver);

    mz = motzkin(5);
    // EXPECT: Motzkin(5): 21
    printf("Motzkin(5): %d\n", mz);
    mz = motzkin(6);
    // EXPECT: Motzkin(6): 51
    printf("Motzkin(6): %d\n", mz);

    // EXPECT: Catalan comparison (recursive vs dp):
    printf("Catalan comparison (recursive vs dp):\n");
    for (i = 0; i <= 9; i++) {
        // EXPECT: C(0): 1 == 1
        // EXPECT: C(1): 1 == 1
        // EXPECT: C(2): 2 == 2
        // EXPECT: C(3): 5 == 5
        // EXPECT: C(4): 14 == 14
        // EXPECT: C(5): 42 == 42
        // EXPECT: C(6): 132 == 132
        // EXPECT: C(7): 429 == 429
        // EXPECT: C(8): 1430 == 1430
        // EXPECT: C(9): 4862 == 4862
        printf("C(%d): %d == %d\n", i, catalan_recursive(i), catalan_dp(i));
    }

    return 0;
}
