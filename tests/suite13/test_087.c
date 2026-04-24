int printf(const char *fmt, ...);

// Longest Common Subsequence (LCS)

int str_len(char *s) {
    int len;
    len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

// LCS length
int lcs_length(char *s1, char *s2) {
    int len1;
    int len2;
    int dp[64][64];
    int i;
    int j;

    len1 = str_len(s1);
    len2 = str_len(s2);

    for (i = 0; i <= len1; i++) {
        dp[i][0] = 0;
    }
    for (j = 0; j <= len2; j++) {
        dp[0][j] = 0;
    }

    for (i = 1; i <= len1; i++) {
        for (j = 1; j <= len2; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                if (dp[i - 1][j] > dp[i][j - 1]) {
                    dp[i][j] = dp[i - 1][j];
                } else {
                    dp[i][j] = dp[i][j - 1];
                }
            }
        }
    }

    return dp[len1][len2];
}

// LCS reconstruction: stores result in out, returns length
int lcs_string(char *s1, char *s2, char *out) {
    int len1;
    int len2;
    int dp[64][64];
    int i;
    int j;
    int idx;
    char temp[64];

    len1 = str_len(s1);
    len2 = str_len(s2);

    for (i = 0; i <= len1; i++) {
        dp[i][0] = 0;
    }
    for (j = 0; j <= len2; j++) {
        dp[0][j] = 0;
    }

    for (i = 1; i <= len1; i++) {
        for (j = 1; j <= len2; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                if (dp[i - 1][j] > dp[i][j - 1]) {
                    dp[i][j] = dp[i - 1][j];
                } else {
                    dp[i][j] = dp[i][j - 1];
                }
            }
        }
    }

    // Traceback
    idx = dp[len1][len2];
    temp[idx] = 0;
    i = len1;
    j = len2;
    while (i > 0 && j > 0) {
        if (s1[i - 1] == s2[j - 1]) {
            idx = idx - 1;
            temp[idx] = s1[i - 1];
            i = i - 1;
            j = j - 1;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i = i - 1;
        } else {
            j = j - 1;
        }
    }

    // Copy to out
    idx = dp[len1][len2];
    for (i = 0; i <= idx; i++) {
        out[i] = temp[i];
    }

    return dp[len1][len2];
}

// LCS on integer arrays
int lcs_int(int *a, int na, int *b, int nb) {
    int dp[32][32];
    int i;
    int j;

    for (i = 0; i <= na; i++) dp[i][0] = 0;
    for (j = 0; j <= nb; j++) dp[0][j] = 0;

    for (i = 1; i <= na; i++) {
        for (j = 1; j <= nb; j++) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                if (dp[i - 1][j] > dp[i][j - 1]) {
                    dp[i][j] = dp[i - 1][j];
                } else {
                    dp[i][j] = dp[i][j - 1];
                }
            }
        }
    }

    return dp[na][nb];
}

// Shortest common supersequence length = len1 + len2 - lcs
int scs_length(char *s1, char *s2) {
    return str_len(s1) + str_len(s2) - lcs_length(s1, s2);
}

int main() {
    char s1[32];
    char s2[32];
    char result[64];
    int len;
    int a[8];
    int b[8];

    // "abcde" and "ace" => LCS = "ace", length 3
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]='d'; s1[4]='e'; s1[5]=0;
    s2[0]='a'; s2[1]='c'; s2[2]='e'; s2[3]=0;

    printf("%d\n", lcs_length(s1, s2));
    // EXPECT: 3

    len = lcs_string(s1, s2, result);
    printf("%s\n", result);
    // EXPECT: ace
    printf("%d\n", len);
    // EXPECT: 3

    // "AGGTAB" and "GXTXAYB" => LCS = "GTAB", length 4
    s1[0]='A'; s1[1]='G'; s1[2]='G'; s1[3]='T'; s1[4]='A'; s1[5]='B'; s1[6]=0;
    s2[0]='G'; s2[1]='X'; s2[2]='T'; s2[3]='X'; s2[4]='A'; s2[5]='Y'; s2[6]='B'; s2[7]=0;

    printf("%d\n", lcs_length(s1, s2));
    // EXPECT: 4

    len = lcs_string(s1, s2, result);
    printf("%s\n", result);
    // EXPECT: GTAB

    // Same strings: LCS = entire string
    s1[0]='h'; s1[1]='e'; s1[2]='l'; s1[3]='l'; s1[4]='o'; s1[5]=0;
    printf("%d\n", lcs_length(s1, s1));
    // EXPECT: 5

    // No common characters
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]=0;
    s2[0]='x'; s2[1]='y'; s2[2]='z'; s2[3]=0;
    printf("%d\n", lcs_length(s1, s2));
    // EXPECT: 0

    // Empty string
    s1[0]=0;
    s2[0]='a'; s2[1]='b'; s2[2]=0;
    printf("%d\n", lcs_length(s1, s2));
    // EXPECT: 0

    // LCS on integers: {1,3,4,1,2,1,3} and {3,4,1,2,1,3}
    a[0]=1; a[1]=3; a[2]=4; a[3]=1; a[4]=2; a[5]=1; a[6]=3;
    b[0]=3; b[1]=4; b[2]=1; b[3]=2; b[4]=1; b[5]=3;
    printf("%d\n", lcs_int(a, 7, b, 6));
    // EXPECT: 6

    // {1,2,3} and {4,5,6}
    a[0]=1; a[1]=2; a[2]=3;
    b[0]=4; b[1]=5; b[2]=6;
    printf("%d\n", lcs_int(a, 3, b, 3));
    // EXPECT: 0

    // {1,2,3} and {1,2,3}
    printf("%d\n", lcs_int(a, 3, a, 3));
    // EXPECT: 3

    // SCS: "abac" and "cab"
    // LCS of "abac","cab" = "ab" (length 2)
    // SCS = 4 + 3 - 2 = 5
    s1[0]='a'; s1[1]='b'; s1[2]='a'; s1[3]='c'; s1[4]=0;
    s2[0]='c'; s2[1]='a'; s2[2]='b'; s2[3]=0;
    printf("%d\n", scs_length(s1, s2));
    // EXPECT: 5

    // SCS: "AGGTAB" and "GXTXAYB"
    // LCS = 4, SCS = 6 + 7 - 4 = 9
    s1[0]='A'; s1[1]='G'; s1[2]='G'; s1[3]='T'; s1[4]='A'; s1[5]='B'; s1[6]=0;
    s2[0]='G'; s2[1]='X'; s2[2]='T'; s2[3]='X'; s2[4]='A'; s2[5]='Y'; s2[6]='B'; s2[7]=0;
    printf("%d\n", scs_length(s1, s2));
    // EXPECT: 9

    printf("done\n");
    // EXPECT: done

    return 0;
}
