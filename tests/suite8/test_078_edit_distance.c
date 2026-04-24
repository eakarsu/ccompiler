int printf(const char *fmt, ...);

// Edit Distance (Levenshtein Distance)

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int min3(int a, int b, int c) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

// Standard edit distance: insert, delete, replace all cost 1
int edit_distance(char *s1, char *s2) {
    int m = str_len(s1);
    int n = str_len(s2);
    // dp[i*(n+1)+j]
    int dp[900];  // max 30x30
    int i;
    int j;

    for (i = 0; i <= m; i++) {
        dp[i * (n + 1)] = i;
    }
    for (j = 0; j <= n; j++) {
        dp[j] = j;
    }

    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i * (n + 1) + j] = dp[(i - 1) * (n + 1) + (j - 1)];
            } else {
                dp[i * (n + 1) + j] = 1 + min3(
                    dp[(i - 1) * (n + 1) + j],       // delete
                    dp[i * (n + 1) + (j - 1)],       // insert
                    dp[(i - 1) * (n + 1) + (j - 1)]  // replace
                );
            }
        }
    }

    return dp[m * (n + 1) + n];
}

// Edit distance with weighted operations
int weighted_edit(char *s1, char *s2, int w_ins, int w_del, int w_rep) {
    int m = str_len(s1);
    int n = str_len(s2);
    int dp[900];
    int i;
    int j;

    for (i = 0; i <= m; i++) {
        dp[i * (n + 1)] = i * w_del;
    }
    for (j = 0; j <= n; j++) {
        dp[j] = j * w_ins;
    }

    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i * (n + 1) + j] = dp[(i - 1) * (n + 1) + (j - 1)];
            } else {
                dp[i * (n + 1) + j] = min3(
                    dp[(i - 1) * (n + 1) + j] + w_del,
                    dp[i * (n + 1) + (j - 1)] + w_ins,
                    dp[(i - 1) * (n + 1) + (j - 1)] + w_rep
                );
            }
        }
    }

    return dp[m * (n + 1) + n];
}

// Recursive edit distance (for small inputs)
int edit_recursive(char *s1, int m, char *s2, int n) {
    if (m == 0) return n;
    if (n == 0) return m;
    if (s1[m - 1] == s2[n - 1]) {
        return edit_recursive(s1, m - 1, s2, n - 1);
    }
    int d = edit_recursive(s1, m - 1, s2, n);       // delete
    int ins = edit_recursive(s1, m, s2, n - 1);     // insert
    int r = edit_recursive(s1, m - 1, s2, n - 1);   // replace
    return 1 + min3(d, ins, r);
}

// Check if two strings are within edit distance k
int within_distance(char *s1, char *s2, int k) {
    int d = edit_distance(s1, s2);
    return d <= k ? 1 : 0;
}

// Hamming distance (only for equal length strings)
int hamming_distance(char *s1, char *s2) {
    int dist = 0;
    int i = 0;
    while (s1[i] != 0 && s2[i] != 0) {
        if (s1[i] != s2[i]) {
            dist = dist + 1;
        }
        i = i + 1;
    }
    return dist;
}

// Longest common prefix length
int lcp_length(char *s1, char *s2) {
    int i = 0;
    while (s1[i] != 0 && s2[i] != 0 && s1[i] == s2[i]) {
        i = i + 1;
    }
    return i;
}

int main() {
    // Basic edit distance tests
    // EXPECT: ed(kitten,sitting) = 3
    printf("ed(kitten,sitting) = %d\n", edit_distance("kitten", "sitting"));
    // EXPECT: ed(saturday,sunday) = 3
    printf("ed(saturday,sunday) = %d\n", edit_distance("saturday", "sunday"));
    // EXPECT: ed(abc,abc) = 0
    printf("ed(abc,abc) = %d\n", edit_distance("abc", "abc"));
    // EXPECT: ed(abc,) = 3
    printf("ed(abc,) = %d\n", edit_distance("abc", ""));
    // EXPECT: ed(,xyz) = 3
    printf("ed(,xyz) = %d\n", edit_distance("", "xyz"));
    // EXPECT: ed(a,b) = 1
    printf("ed(a,b) = %d\n", edit_distance("a", "b"));

    // More complex cases
    // EXPECT: ed(intention,execution) = 5
    printf("ed(intention,execution) = %d\n", edit_distance("intention", "execution"));
    // EXPECT: ed(horse,ros) = 3
    printf("ed(horse,ros) = %d\n", edit_distance("horse", "ros"));

    // Verify recursive matches DP
    int d1 = edit_distance("kitten", "sitting");
    int d2 = edit_recursive("kitten", 6, "sitting", 7);
    // EXPECT: rec_match = 1
    printf("rec_match = %d\n", d1 == d2 ? 1 : 0);

    int d3 = edit_distance("horse", "ros");
    int d4 = edit_recursive("horse", 5, "ros", 3);
    // EXPECT: rec_match2 = 1
    printf("rec_match2 = %d\n", d3 == d4 ? 1 : 0);

    // Weighted edit distance
    // With all weights = 1, should match standard
    // EXPECT: w_ed(abc,axc) = 1
    printf("w_ed(abc,axc) = %d\n", weighted_edit("abc", "axc", 1, 1, 1));
    // With higher replacement cost
    // EXPECT: w_ed2(abc,axc) = 2
    printf("w_ed2(abc,axc) = %d\n", weighted_edit("abc", "axc", 1, 1, 2));
    // With higher insert cost
    // EXPECT: w_ed3(abc,abcd) = 3
    printf("w_ed3(abc,abcd) = %d\n", weighted_edit("abc", "abcd", 3, 1, 1));

    // Within distance test
    // EXPECT: within(kitten,sitting,3) = 1
    printf("within(kitten,sitting,3) = %d\n", within_distance("kitten", "sitting", 3));
    // EXPECT: within(kitten,sitting,2) = 0
    printf("within(kitten,sitting,2) = %d\n", within_distance("kitten", "sitting", 2));
    // EXPECT: within(abc,abc,0) = 1
    printf("within(abc,abc,0) = %d\n", within_distance("abc", "abc", 0));

    // Hamming distance
    // EXPECT: hamming(abc,axc) = 1
    printf("hamming(abc,axc) = %d\n", hamming_distance("abc", "axc"));
    // EXPECT: hamming(abcd,abcd) = 0
    printf("hamming(abcd,abcd) = %d\n", hamming_distance("abcd", "abcd"));
    // EXPECT: hamming(abcd,wxyz) = 4
    printf("hamming(abcd,wxyz) = %d\n", hamming_distance("abcd", "wxyz"));

    // Longest common prefix
    // EXPECT: lcp(abcdef,abcxyz) = 3
    printf("lcp(abcdef,abcxyz) = %d\n", lcp_length("abcdef", "abcxyz"));
    // EXPECT: lcp(hello,hello) = 5
    printf("lcp(hello,hello) = %d\n", lcp_length("hello", "hello"));
    // EXPECT: lcp(abc,xyz) = 0
    printf("lcp(abc,xyz) = %d\n", lcp_length("abc", "xyz"));

    return 0;
}
