int printf(const char *fmt, ...);

// Longest Common Subsequence (LCS) length

// Helper: string length
int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

// LCS length using DP table
int lcs_length(char *s1, char *s2) {
    int m = str_len(s1);
    int n = str_len(s2);
    // Using a flat 2D array: dp[i][j] at index i*(n+1)+j
    int dp[900];  // max 30x30
    int i;
    int j;

    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            dp[i * (n + 1) + j] = 0;
        }
    }

    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i * (n + 1) + j] = dp[(i - 1) * (n + 1) + (j - 1)] + 1;
            } else {
                int top = dp[(i - 1) * (n + 1) + j];
                int left = dp[i * (n + 1) + (j - 1)];
                if (top > left) {
                    dp[i * (n + 1) + j] = top;
                } else {
                    dp[i * (n + 1) + j] = left;
                }
            }
        }
    }

    return dp[m * (n + 1) + n];
}

// LCS length using space-optimized DP (two rows)
int lcs_optimized(char *s1, char *s2) {
    int m = str_len(s1);
    int n = str_len(s2);
    int prev[30];
    int curr[30];
    int i;
    int j;

    for (j = 0; j <= n; j++) {
        prev[j] = 0;
        curr[j] = 0;
    }

    for (i = 1; i <= m; i++) {
        curr[0] = 0;
        for (j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                curr[j] = prev[j - 1] + 1;
            } else {
                if (prev[j] > curr[j - 1]) {
                    curr[j] = prev[j];
                } else {
                    curr[j] = curr[j - 1];
                }
            }
        }
        for (j = 0; j <= n; j++) {
            prev[j] = curr[j];
        }
    }

    return prev[n];
}

// Recursive LCS (for small strings only, exponential)
int lcs_recursive(char *s1, int m, char *s2, int n) {
    if (m == 0 || n == 0) return 0;
    if (s1[m - 1] == s2[n - 1]) {
        return 1 + lcs_recursive(s1, m - 1, s2, n - 1);
    }
    int a = lcs_recursive(s1, m - 1, s2, n);
    int b = lcs_recursive(s1, m, s2, n - 1);
    if (a > b) return a;
    return b;
}

// Longest Common Substring (contiguous) length
int lc_substring(char *s1, char *s2) {
    int m = str_len(s1);
    int n = str_len(s2);
    int dp[900];
    int i;
    int j;
    int max_len = 0;

    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            dp[i * (n + 1) + j] = 0;
        }
    }

    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i * (n + 1) + j] = dp[(i - 1) * (n + 1) + (j - 1)] + 1;
                if (dp[i * (n + 1) + j] > max_len) {
                    max_len = dp[i * (n + 1) + j];
                }
            } else {
                dp[i * (n + 1) + j] = 0;
            }
        }
    }

    return max_len;
}

// Shortest Common Supersequence length
int scs_length(char *s1, char *s2) {
    int m = str_len(s1);
    int n = str_len(s2);
    int lcs = lcs_length(s1, s2);
    return m + n - lcs;
}

int main() {
    // Test basic LCS
    // EXPECT: lcs(ABCBDAB,BDCAB) = 4
    printf("lcs(ABCBDAB,BDCAB) = %d\n", lcs_length("ABCBDAB", "BDCAB"));
    // EXPECT: lcs(AGGTAB,GXTXAYB) = 4
    printf("lcs(AGGTAB,GXTXAYB) = %d\n", lcs_length("AGGTAB", "GXTXAYB"));
    // EXPECT: lcs(ABC,ABC) = 3
    printf("lcs(ABC,ABC) = %d\n", lcs_length("ABC", "ABC"));
    // EXPECT: lcs(ABC,DEF) = 0
    printf("lcs(ABC,DEF) = %d\n", lcs_length("ABC", "DEF"));
    // EXPECT: lcs(A,A) = 1
    printf("lcs(A,A) = %d\n", lcs_length("A", "A"));
    // EXPECT: lcs(ABCD,ACDF) = 3
    printf("lcs(ABCD,ACDF) = %d\n", lcs_length("ABCD", "ACDF"));

    // Test optimized version matches standard
    // EXPECT: opt(ABCBDAB,BDCAB) = 4
    printf("opt(ABCBDAB,BDCAB) = %d\n", lcs_optimized("ABCBDAB", "BDCAB"));
    // EXPECT: opt(AGGTAB,GXTXAYB) = 4
    printf("opt(AGGTAB,GXTXAYB) = %d\n", lcs_optimized("AGGTAB", "GXTXAYB"));
    // EXPECT: opt(ABC,DEF) = 0
    printf("opt(ABC,DEF) = %d\n", lcs_optimized("ABC", "DEF"));

    // Test recursive (small inputs only)
    // EXPECT: rec(ABCB,BDC) = 2
    printf("rec(ABCB,BDC) = %d\n", lcs_recursive("ABCB", 4, "BDC", 3));
    // EXPECT: rec(ABC,AC) = 2
    printf("rec(ABC,AC) = %d\n", lcs_recursive("ABC", 3, "AC", 2));

    // Test longest common substring (contiguous)
    // EXPECT: substr(ABCDEF,ZBCDF) = 3
    printf("substr(ABCDEF,ZBCDF) = %d\n", lc_substring("ABCDEF", "ZBCDF"));
    // EXPECT: substr(ABAB,BABA) = 3
    printf("substr(ABAB,BABA) = %d\n", lc_substring("ABAB", "BABA"));
    // EXPECT: substr(ABC,DEF) = 0
    printf("substr(ABC,DEF) = %d\n", lc_substring("ABC", "DEF"));

    // Test shortest common supersequence
    // EXPECT: scs(AGGTAB,GXTXAYB) = 9
    printf("scs(AGGTAB,GXTXAYB) = %d\n", scs_length("AGGTAB", "GXTXAYB"));
    // EXPECT: scs(ABC,ABC) = 3
    printf("scs(ABC,ABC) = %d\n", scs_length("ABC", "ABC"));
    // EXPECT: scs(ABC,DEF) = 6
    printf("scs(ABC,DEF) = %d\n", scs_length("ABC", "DEF"));

    // Verify all three methods agree
    int v1 = lcs_length("ABCBDAB", "BDCAB");
    int v2 = lcs_optimized("ABCBDAB", "BDCAB");
    int v3 = lcs_recursive("ABCBDAB", 7, "BDCAB", 5);
    int all_match = (v1 == v2 && v2 == v3) ? 1 : 0;
    // EXPECT: all_match = 1
    printf("all_match = %d\n", all_match);

    return 0;
}
