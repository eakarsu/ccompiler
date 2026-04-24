int printf(const char *fmt, ...);

// Diff algorithm: Longest Common Subsequence (LCS) based
// Compare two strings character by character and produce LCS length,
// the LCS itself, and a simple diff output

int str_len(char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

// dp table stored as flat array: dp[i * (n+1) + j]
// max dimensions: strings up to 20 chars each
int dp[441]; // 21*21

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int lcs_length(char *a, char *b) {
    int m = str_len(a);
    int n = str_len(b);
    int i;
    int j;
    int cols = n + 1;

    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                dp[i * cols + j] = 0;
            } else if (a[i - 1] == b[j - 1]) {
                dp[i * cols + j] = dp[(i - 1) * cols + (j - 1)] + 1;
            } else {
                int top = dp[(i - 1) * cols + j];
                int left = dp[i * cols + (j - 1)];
                dp[i * cols + j] = max(top, left);
            }
        }
    }
    return dp[m * cols + n];
}

// Backtrack to find the actual LCS string
void lcs_string(char *a, char *b, char *result) {
    int m = str_len(a);
    int n = str_len(b);
    int cols = n + 1;
    int len = dp[m * cols + n];
    result[len] = 0;

    int i = m;
    int j = n;
    int k = len - 1;

    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            result[k] = a[i - 1];
            k--;
            i--;
            j--;
        } else if (dp[(i - 1) * cols + j] > dp[i * cols + (j - 1)]) {
            i--;
        } else {
            j--;
        }
    }
}

// Compute edit distance (insertions + deletions to transform a to b)
// Based on LCS: edits = len(a) + len(b) - 2 * lcs
int edit_distance_from_lcs(char *a, char *b) {
    int m = str_len(a);
    int n = str_len(b);
    int lcs = lcs_length(a, b);
    return m + n - 2 * lcs;
}

// Generate diff markers: for each char in a, mark as kept (=) or removed (-)
// For each char in b not in LCS, mark as added (+)
// Writes diff info to arrays
void generate_diff(char *a, char *b, char *diff_ops, char *diff_chars, int *diff_len) {
    int m = str_len(a);
    int n = str_len(b);
    int cols = n + 1;
    int idx = 0;

    // Must have dp table computed already
    int i = m;
    int j = n;

    // Build diff in reverse
    char rev_ops[64];
    char rev_chars[64];
    int rlen = 0;

    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && a[i - 1] == b[j - 1]) {
            rev_ops[rlen] = '=';
            rev_chars[rlen] = a[i - 1];
            rlen++;
            i--; j--;
        } else if (j > 0 && (i == 0 || dp[i * cols + (j - 1)] >= dp[(i - 1) * cols + j])) {
            rev_ops[rlen] = '+';
            rev_chars[rlen] = b[j - 1];
            rlen++;
            j--;
        } else {
            rev_ops[rlen] = '-';
            rev_chars[rlen] = a[i - 1];
            rlen++;
            i--;
        }
    }

    // Reverse
    int k;
    for (k = 0; k < rlen; k++) {
        diff_ops[k] = rev_ops[rlen - 1 - k];
        diff_chars[k] = rev_chars[rlen - 1 - k];
    }
    diff_ops[rlen] = 0;
    diff_chars[rlen] = 0;
    *diff_len = rlen;
}

int count_op(char *ops, int len, char op) {
    int c = 0;
    int i;
    for (i = 0; i < len; i++) {
        if (ops[i] == op) c++;
    }
    return c;
}

int main() {
    // Test 1: basic LCS
    int l1 = lcs_length("ABCBDAB", "BDCAB");
    printf("%d\n", l1);
    // EXPECT: 4

    char lcs1[32];
    lcs_string("ABCBDAB", "BDCAB", lcs1);
    printf("%s\n", lcs1);
    // EXPECT: BDAB

    // Test 2: identical strings
    int l2 = lcs_length("hello", "hello");
    printf("%d\n", l2);
    // EXPECT: 5

    // Test 3: completely different
    int l3 = lcs_length("abc", "xyz");
    printf("%d\n", l3);
    // EXPECT: 0

    // Test 4: one empty
    int l4 = lcs_length("abc", "");
    printf("%d\n", l4);
    // EXPECT: 0

    // Test 5: subsequence
    int l5 = lcs_length("ace", "abcde");
    printf("%d\n", l5);
    // EXPECT: 3

    char lcs5[32];
    lcs_string("ace", "abcde", lcs5);
    printf("%s\n", lcs5);
    // EXPECT: ace

    // Test 6: edit distance
    int ed1 = edit_distance_from_lcs("kitten", "sitting");
    printf("%d\n", ed1);
    // EXPECT: 5

    int ed2 = edit_distance_from_lcs("abc", "abc");
    printf("%d\n", ed2);
    // EXPECT: 0

    int ed3 = edit_distance_from_lcs("ab", "cd");
    printf("%d\n", ed3);
    // EXPECT: 4

    // Test 7: diff generation
    lcs_length("ABCD", "ACBD");
    char ops[64];
    char chars[64];
    int dlen;
    generate_diff("ABCD", "ACBD", ops, chars, &dlen);

    printf("%d\n", dlen);
    // EXPECT: 5

    int kept = count_op(ops, dlen, '=');
    int added = count_op(ops, dlen, '+');
    int removed = count_op(ops, dlen, '-');
    printf("%d\n", kept);
    // EXPECT: 3
    printf("%d\n", added);
    // EXPECT: 1
    printf("%d\n", removed);
    // EXPECT: 1

    // Test 8: diff of identical
    lcs_length("abc", "abc");
    generate_diff("abc", "abc", ops, chars, &dlen);
    printf("%d\n", dlen);
    // EXPECT: 3
    printf("%d\n", count_op(ops, dlen, '='));
    // EXPECT: 3
    printf("%d\n", count_op(ops, dlen, '+'));
    // EXPECT: 0

    // Test 9: diff where one is prefix
    lcs_length("abc", "abcde");
    generate_diff("abc", "abcde", ops, chars, &dlen);
    printf("%d\n", dlen);
    // EXPECT: 5
    printf("%d\n", count_op(ops, dlen, '='));
    // EXPECT: 3
    printf("%d\n", count_op(ops, dlen, '+'));
    // EXPECT: 2

    // Test 10: LCS of longer strings
    int l10 = lcs_length("AGGTAB", "GXTXAYB");
    printf("%d\n", l10);
    // EXPECT: 4

    char lcs10[32];
    lcs_string("AGGTAB", "GXTXAYB", lcs10);
    printf("%s\n", lcs10);
    // EXPECT: GTAB

    return 0;
}
