int printf(const char *fmt, ...);

// LCS-based diff algorithm
// Computes longest common subsequence and generates diff output

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return (a[i] == 0 && b[i] == 0) ? 1 : 0;
}

// Compute LCS length between two arrays of strings
// lines1[0..n1-1], lines2[0..n2-1]
// dp is (n1+1) x (n2+1)
void compute_lcs(char *lines1[16], int n1, char *lines2[16], int n2, int dp[17][17]) {
    int i;
    int j;

    for (i = 0; i <= n1; i = i + 1) {
        dp[i][0] = 0;
    }
    for (j = 0; j <= n2; j = j + 1) {
        dp[0][j] = 0;
    }

    for (i = 1; i <= n1; i = i + 1) {
        for (j = 1; j <= n2; j = j + 1) {
            if (str_eq(lines1[i - 1], lines2[j - 1])) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                if (dp[i - 1][j] >= dp[i][j - 1]) {
                    dp[i][j] = dp[i - 1][j];
                } else {
                    dp[i][j] = dp[i][j - 1];
                }
            }
        }
    }
}

// Generate diff output
// op: '+' for added, '-' for removed, ' ' for same
struct DiffEntry {
    char op;
    char *line;
};

int generate_diff(char *lines1[16], int n1, char *lines2[16], int n2, struct DiffEntry *diff) {
    int dp[17][17];
    int i;
    int j;
    int d = 0;
    // Use a stack to reverse order
    struct DiffEntry temp[32];
    int nt = 0;

    compute_lcs(lines1, n1, lines2, n2, dp);

    i = n1;
    j = n2;

    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && str_eq(lines1[i - 1], lines2[j - 1])) {
            temp[nt].op = ' ';
            temp[nt].line = lines1[i - 1];
            nt = nt + 1;
            i = i - 1;
            j = j - 1;
        } else if (j > 0 && (i == 0 || dp[i][j - 1] >= dp[i - 1][j])) {
            temp[nt].op = '+';
            temp[nt].line = lines2[j - 1];
            nt = nt + 1;
            j = j - 1;
        } else {
            temp[nt].op = '-';
            temp[nt].line = lines1[i - 1];
            nt = nt + 1;
            i = i - 1;
        }
    }

    // Reverse
    for (i = nt - 1; i >= 0; i = i - 1) {
        diff[d] = temp[i];
        d = d + 1;
    }
    return d;
}

// Count additions, deletions, unchanged
struct DiffStats {
    int added;
    int removed;
    int unchanged;
};

void compute_stats(struct DiffEntry *diff, int ndiff, struct DiffStats *stats) {
    int i;
    stats->added = 0;
    stats->removed = 0;
    stats->unchanged = 0;

    for (i = 0; i < ndiff; i = i + 1) {
        if (diff[i].op == '+') {
            stats->added = stats->added + 1;
        } else if (diff[i].op == '-') {
            stats->removed = stats->removed + 1;
        } else {
            stats->unchanged = stats->unchanged + 1;
        }
    }
}

void test_lcs() {
    int dp[17][17];
    char *a[16];
    char *b[16];
    int lcs_len;

    a[0] = "A"; a[1] = "B"; a[2] = "C"; a[3] = "D";
    b[0] = "A"; b[1] = "C"; b[2] = "D";

    compute_lcs(a, 4, b, 3, dp);
    lcs_len = dp[4][3];
    printf("LCS len: %d\n", lcs_len);
    // EXPECT: LCS len: 3

    a[0] = "X"; a[1] = "Y"; a[2] = "Z";
    b[0] = "A"; b[1] = "B"; b[2] = "C";
    compute_lcs(a, 3, b, 3, dp);
    printf("no common: %d\n", dp[3][3]);
    // EXPECT: no common: 0

    a[0] = "same"; a[1] = "same";
    b[0] = "same"; b[1] = "same";
    compute_lcs(a, 2, b, 2, dp);
    printf("identical: %d\n", dp[2][2]);
    // EXPECT: identical: 2
}

void test_diff_simple() {
    char *a[16];
    char *b[16];
    struct DiffEntry diff[32];
    struct DiffStats stats;
    int nd;
    int i;

    a[0] = "hello"; a[1] = "world";
    b[0] = "hello"; b[1] = "there";

    nd = generate_diff(a, 2, b, 2, diff);
    printf("simple diff (%d):\n", nd);
    // EXPECT: simple diff (3):
    for (i = 0; i < nd; i = i + 1) {
        printf(" %s\n", diff[i].line);
    }
    // EXPECT:  hello
    // EXPECT:  world
    // EXPECT:  there

    compute_stats(diff, nd, &stats);
    printf("stats: +%d -%d =%d\n", stats.added, stats.removed, stats.unchanged);
    // EXPECT: stats: +1 -1 =1
}

void test_diff_insert() {
    char *a[16];
    char *b[16];
    struct DiffEntry diff[32];
    struct DiffStats stats;
    int nd;
    int i;

    a[0] = "line1"; a[1] = "line3";
    b[0] = "line1"; b[1] = "line2"; b[2] = "line3";

    nd = generate_diff(a, 2, b, 3, diff);
    printf("insert diff:\n");
    // EXPECT: insert diff:
    for (i = 0; i < nd; i = i + 1) {
        printf("%s %s\n", diff[i].op == '+' ? "+" : (diff[i].op == '-' ? "-" : " "), diff[i].line);
    }
    // EXPECT:   line1
    // EXPECT: + line2
    // EXPECT:   line3

    compute_stats(diff, nd, &stats);
    printf("ins stats: +%d -%d =%d\n", stats.added, stats.removed, stats.unchanged);
    // EXPECT: ins stats: +1 -0 =2
}

void test_diff_delete() {
    char *a[16];
    char *b[16];
    struct DiffEntry diff[32];
    struct DiffStats stats;
    int nd;
    int i;

    a[0] = "aaa"; a[1] = "bbb"; a[2] = "ccc";
    b[0] = "aaa"; b[1] = "ccc";

    nd = generate_diff(a, 3, b, 2, diff);
    printf("delete diff:\n");
    // EXPECT: delete diff:
    for (i = 0; i < nd; i = i + 1) {
        printf("%s %s\n", diff[i].op == '+' ? "+" : (diff[i].op == '-' ? "-" : " "), diff[i].line);
    }
    // EXPECT:   aaa
    // EXPECT: - bbb
    // EXPECT:   ccc

    compute_stats(diff, nd, &stats);
    printf("del stats: +%d -%d =%d\n", stats.added, stats.removed, stats.unchanged);
    // EXPECT: del stats: +0 -1 =2
}

void test_diff_complex() {
    char *a[16];
    char *b[16];
    struct DiffEntry diff[32];
    struct DiffStats stats;
    int nd;

    a[0] = "int x"; a[1] = "x = 5"; a[2] = "print x"; a[3] = "return";
    b[0] = "int x"; b[1] = "int y"; b[2] = "x = 10"; b[3] = "print x"; b[4] = "print y"; b[5] = "return";

    nd = generate_diff(a, 4, b, 6, diff);
    compute_stats(diff, nd, &stats);
    printf("complex: +%d -%d =%d\n", stats.added, stats.removed, stats.unchanged);
    // EXPECT: complex: +3 -1 =3
}

void test_empty() {
    char *a[16];
    char *b[16];
    struct DiffEntry diff[32];
    struct DiffStats stats;
    int nd;

    // Empty to something
    b[0] = "new";
    nd = generate_diff(a, 0, b, 1, diff);
    compute_stats(diff, nd, &stats);
    printf("empty to 1: +%d -%d =%d\n", stats.added, stats.removed, stats.unchanged);
    // EXPECT: empty to 1: +1 -0 =0

    // Something to empty
    a[0] = "old";
    nd = generate_diff(a, 1, b, 0, diff);
    compute_stats(diff, nd, &stats);
    printf("1 to empty: +%d -%d =%d\n", stats.added, stats.removed, stats.unchanged);
    // EXPECT: 1 to empty: +0 -1 =0

    printf("diff tests done\n");
    // EXPECT: diff tests done
}

int main() {
    test_lcs();
    test_diff_simple();
    test_diff_insert();
    test_diff_delete();
    test_diff_complex();
    test_empty();
    return 0;
}
