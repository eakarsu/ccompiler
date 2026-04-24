int printf(const char *fmt, ...);

int max(int a, int b) {
    if (a > b) return a;
    return b;
}

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

int lps_length(char s[], int n) {
    int dp[30][30];
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < n) {
            dp[i][j] = 0;
            j = j + 1;
        }
        dp[i][i] = 1;
        i = i + 1;
    }

    int len = 2;
    while (len <= n) {
        i = 0;
        while (i <= n - len) {
            int j = i + len - 1;
            if (s[i] == s[j] && len == 2) {
                dp[i][j] = 2;
            } else if (s[i] == s[j]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
            } else {
                dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
            }
            i = i + 1;
        }
        len = len + 1;
    }
    return dp[0][n - 1];
}

int is_palindrome_range(char s[], int lo, int hi) {
    while (lo < hi) {
        if (s[lo] != s[hi]) return 0;
        lo = lo + 1;
        hi = hi - 1;
    }
    return 1;
}

int min_palindrome_cuts(char s[], int n) {
    int pal[30][30];
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < n) {
            pal[i][j] = 0;
            j = j + 1;
        }
        pal[i][i] = 1;
        i = i + 1;
    }

    int len = 2;
    while (len <= n) {
        i = 0;
        while (i <= n - len) {
            int j = i + len - 1;
            if (len == 2) {
                pal[i][j] = (s[i] == s[j]);
            } else {
                pal[i][j] = (s[i] == s[j]) && pal[i + 1][j - 1];
            }
            i = i + 1;
        }
        len = len + 1;
    }

    int cuts[30];
    i = 0;
    while (i < n) {
        if (pal[0][i]) {
            cuts[i] = 0;
        } else {
            cuts[i] = 99999;
            int j = 0;
            while (j < i) {
                if (pal[j + 1][i] && cuts[j] + 1 < cuts[i]) {
                    cuts[i] = cuts[j] + 1;
                }
                j = j + 1;
            }
        }
        i = i + 1;
    }
    return cuts[n - 1];
}

int longest_palindrome_substr(char s[], int n) {
    int dp[30][30];
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < n) {
            dp[i][j] = 0;
            j = j + 1;
        }
        dp[i][i] = 1;
        i = i + 1;
    }

    int maxlen = 1;
    i = 0;
    while (i < n - 1) {
        if (s[i] == s[i + 1]) {
            dp[i][i + 1] = 1;
            maxlen = 2;
        }
        i = i + 1;
    }

    int len = 3;
    while (len <= n) {
        i = 0;
        while (i <= n - len) {
            int j = i + len - 1;
            if (s[i] == s[j] && dp[i + 1][j - 1]) {
                dp[i][j] = 1;
                if (len > maxlen) maxlen = len;
            }
            i = i + 1;
        }
        len = len + 1;
    }
    return maxlen;
}

int count_palindromic_substrings(char s[], int n) {
    int count = 0;
    int i = 0;
    while (i < n) {
        int lo = i;
        int hi = i;
        while (lo >= 0 && hi < n && s[lo] == s[hi]) {
            count = count + 1;
            lo = lo - 1;
            hi = hi + 1;
        }
        lo = i;
        hi = i + 1;
        while (lo >= 0 && hi < n && s[lo] == s[hi]) {
            count = count + 1;
            lo = lo - 1;
            hi = hi + 1;
        }
        i = i + 1;
    }
    return count;
}

int main(void) {
    char s1[10];
    s1[0]='b'; s1[1]='b'; s1[2]='b'; s1[3]='a'; s1[4]='b'; s1[5]=0;
    // EXPECT: lps(bbbab) = 4
    printf("lps(bbbab) = %d\n", lps_length(s1, 5));

    char s2[10];
    s2[0]='c'; s2[1]='b'; s2[2]='b'; s2[3]='d'; s2[4]=0;
    // EXPECT: lps(cbbd) = 2
    printf("lps(cbbd) = %d\n", lps_length(s2, 4));

    char s3[5];
    s3[0]='a'; s3[1]=0;
    // EXPECT: lps(a) = 1
    printf("lps(a) = %d\n", lps_length(s3, 1));

    char s4[10];
    s4[0]='a'; s4[1]='b'; s4[2]='c'; s4[3]='b'; s4[4]='a'; s4[5]=0;
    // EXPECT: lps(abcba) = 5
    printf("lps(abcba) = %d\n", lps_length(s4, 5));

    char p1[10];
    p1[0]='a'; p1[1]='a'; p1[2]='b'; p1[3]=0;
    // EXPECT: cuts(aab) = 1
    printf("cuts(aab) = %d\n", min_palindrome_cuts(p1, 3));

    char p2[10];
    p2[0]='a'; p2[1]='b'; p2[2]='c'; p2[3]='b'; p2[4]='m'; p2[5]=0;
    // EXPECT: cuts(abcbm) = 2
    printf("cuts(abcbm) = %d\n", min_palindrome_cuts(p2, 5));

    char p3[5];
    p3[0]='a'; p3[1]='b'; p3[2]='a'; p3[3]=0;
    // EXPECT: cuts(aba) = 0
    printf("cuts(aba) = %d\n", min_palindrome_cuts(p3, 3));

    char p4[10];
    p4[0]='a'; p4[1]='b'; p4[2]='c'; p4[3]='d'; p4[4]='e'; p4[5]=0;
    // EXPECT: cuts(abcde) = 4
    printf("cuts(abcde) = %d\n", min_palindrome_cuts(p4, 5));

    // EXPECT: lpsub(babad) = 3
    char ls1[10];
    ls1[0]='b'; ls1[1]='a'; ls1[2]='b'; ls1[3]='a'; ls1[4]='d'; ls1[5]=0;
    printf("lpsub(babad) = %d\n", longest_palindrome_substr(ls1, 5));

    // EXPECT: lpsub(cbbd) = 2
    printf("lpsub(cbbd) = %d\n", longest_palindrome_substr(s2, 4));

    // EXPECT: lpsub(abcba) = 5
    printf("lpsub(abcba) = %d\n", longest_palindrome_substr(s4, 5));

    // EXPECT: count_pal(abc) = 3
    char cp1[5];
    cp1[0]='a'; cp1[1]='b'; cp1[2]='c'; cp1[3]=0;
    printf("count_pal(abc) = %d\n", count_palindromic_substrings(cp1, 3));

    // EXPECT: count_pal(aaa) = 6
    char cp2[5];
    cp2[0]='a'; cp2[1]='a'; cp2[2]='a'; cp2[3]=0;
    printf("count_pal(aaa) = %d\n", count_palindromic_substrings(cp2, 3));

    // EXPECT: is_pal(abcba) = 1
    printf("is_pal(abcba) = %d\n", is_palindrome_range(s4, 0, 4));

    // EXPECT: is_pal(abc) = 0
    printf("is_pal(abc) = %d\n", is_palindrome_range(cp1, 0, 2));

    return 0;
}
