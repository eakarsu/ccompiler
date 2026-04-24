// EXPECT: === Longest Common Subsequence ===
// EXPECT: --- Test 1 ---
// EXPECT: s1="ABCBDAB" s2="BDCAB"
// EXPECT: LCS length=4
// EXPECT: LCS="BDAB"
// EXPECT: --- Test 2 ---
// EXPECT: s1="AGGTAB" s2="GXTXAYB"
// EXPECT: LCS length=4
// EXPECT: LCS="GTAB"
// EXPECT: --- Test 3 ---
// EXPECT: s1="ABC" s2="DEF"
// EXPECT: LCS length=0
// EXPECT: LCS=""
// EXPECT: --- Test 4 ---
// EXPECT: s1="ABCD" s2="ABCD"
// EXPECT: LCS length=4
// EXPECT: LCS="ABCD"
// EXPECT: --- Test 5 ---
// EXPECT: s1="" s2="ABC"
// EXPECT: LCS length=0
// EXPECT: LCS=""
// EXPECT: --- Test 6 ---
// EXPECT: s1="A" s2="A"
// EXPECT: LCS length=1
// EXPECT: LCS="A"
// EXPECT: --- Test 7 ---
// EXPECT: s1="AABAA" s2="AAAB"
// EXPECT: LCS length=3
// EXPECT: LCS="AAA"
// EXPECT: --- Test 8: Longest Common Substring ---
// EXPECT: lcsubstr("ABCDEF","ZBCDF")=3
// EXPECT: lcsubstr("ABC","DEF")=0
// EXPECT: lcsubstr("ABAB","BABA")=3
// EXPECT: lcsubstr("HELLO","JELLO")=4
// EXPECT: --- Test 9: Edit Distance ---
// EXPECT: edit("kitten","sitting")=3
// EXPECT: edit("","abc")=3
// EXPECT: edit("abc","abc")=0
// EXPECT: edit("abc","")=3
// EXPECT: edit("sunday","saturday")=3
// EXPECT: === Done ===

int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) len = len + 1;
    return len;
}

int dp[50][50];

int lcs_length(char *s1, char *s2, int m, int n) {
    int i = 0;
    while (i <= m) {
        int j = 0;
        while (j <= n) {
            dp[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }

    i = 1;
    while (i <= m) {
        int j = 1;
        while (j <= n) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                if (dp[i - 1][j] > dp[i][j - 1]) {
                    dp[i][j] = dp[i - 1][j];
                } else {
                    dp[i][j] = dp[i][j - 1];
                }
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return dp[m][n];
}

void lcs_backtrack(char *s1, char *s2, int m, int n, char *result) {
    int len = dp[m][n];
    result[len] = 0;
    int i = m;
    int j = n;
    int idx = len - 1;
    while (i > 0 && j > 0) {
        if (s1[i - 1] == s2[j - 1]) {
            result[idx] = s1[i - 1];
            idx = idx - 1;
            i = i - 1;
            j = j - 1;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i = i - 1;
        } else {
            j = j - 1;
        }
    }
}

void test_lcs(char *s1, char *s2, int test_num) {
    int m = my_strlen(s1);
    int n = my_strlen(s2);
    printf("--- Test %d ---\n", test_num);
    printf("s1=\"%s\" s2=\"%s\"\n", s1, s2);
    int len = lcs_length(s1, s2, m, n);
    printf("LCS length=%d\n", len);

    char result[50];
    lcs_backtrack(s1, s2, m, n, result);
    printf("LCS=\"%s\"\n", result);
}

// Longest Common Substring (contiguous)
int lc_substr(char *s1, char *s2) {
    int m = my_strlen(s1);
    int n = my_strlen(s2);
    int best = 0;
    int tbl[50][50];

    int i = 0;
    while (i <= m) {
        int j = 0;
        while (j <= n) {
            tbl[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }

    i = 1;
    while (i <= m) {
        int j = 1;
        while (j <= n) {
            if (s1[i - 1] == s2[j - 1]) {
                tbl[i][j] = tbl[i - 1][j - 1] + 1;
                if (tbl[i][j] > best) best = tbl[i][j];
            } else {
                tbl[i][j] = 0;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return best;
}

// Edit distance (Levenshtein)
int edit_dist(char *s1, char *s2) {
    int m = my_strlen(s1);
    int n = my_strlen(s2);
    int ed[50][50];

    int i = 0;
    while (i <= m) {
        ed[i][0] = i;
        i = i + 1;
    }
    int j = 0;
    while (j <= n) {
        ed[0][j] = j;
        j = j + 1;
    }

    i = 1;
    while (i <= m) {
        j = 1;
        while (j <= n) {
            int cost = 0;
            if (s1[i - 1] != s2[j - 1]) cost = 1;
            int del = ed[i - 1][j] + 1;
            int ins = ed[i][j - 1] + 1;
            int rep = ed[i - 1][j - 1] + cost;
            int best = del;
            if (ins < best) best = ins;
            if (rep < best) best = rep;
            ed[i][j] = best;
            j = j + 1;
        }
        i = i + 1;
    }
    return ed[m][n];
}

int main(void) {
    printf("=== Longest Common Subsequence ===\n");

    test_lcs("ABCBDAB", "BDCAB", 1);
    test_lcs("AGGTAB", "GXTXAYB", 2);
    test_lcs("ABC", "DEF", 3);
    test_lcs("ABCD", "ABCD", 4);
    test_lcs("", "ABC", 5);
    test_lcs("A", "A", 6);
    test_lcs("AABAA", "AAAB", 7);

    printf("--- Test 8: Longest Common Substring ---\n");
    printf("lcsubstr(\"ABCDEF\",\"ZBCDF\")=%d\n", lc_substr("ABCDEF", "ZBCDF"));
    printf("lcsubstr(\"ABC\",\"DEF\")=%d\n", lc_substr("ABC", "DEF"));
    printf("lcsubstr(\"ABAB\",\"BABA\")=%d\n", lc_substr("ABAB", "BABA"));
    printf("lcsubstr(\"HELLO\",\"JELLO\")=%d\n", lc_substr("HELLO", "JELLO"));

    printf("--- Test 9: Edit Distance ---\n");
    printf("edit(\"kitten\",\"sitting\")=%d\n", edit_dist("kitten", "sitting"));
    printf("edit(\"\",\"abc\")=%d\n", edit_dist("", "abc"));
    printf("edit(\"abc\",\"abc\")=%d\n", edit_dist("abc", "abc"));
    printf("edit(\"abc\",\"\")=%d\n", edit_dist("abc", ""));
    printf("edit(\"sunday\",\"saturday\")=%d\n", edit_dist("sunday", "saturday"));

    printf("=== Done ===\n");
    return 0;
}
