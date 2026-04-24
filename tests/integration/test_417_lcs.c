int printf(const char *fmt, ...);
// EXPECT: LCS length: 4\nLCS: BDAB\nLCS length: 4\nLCS: GTAB\nLCS length: 6\nLCS: ABCDEF\nLCS length: 0
// Test: Longest common subsequence

int dp[64][64];

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len++;
    return len;
}

int lcs_length(char *a, char *b) {
    int m = str_len(a);
    int n = str_len(b);
    int i, j;

    for (i = 0; i <= m; i++) dp[i][0] = 0;
    for (j = 0; j <= n; j++) dp[0][j] = 0;

    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = dp[i - 1][j];
                if (dp[i][j - 1] > dp[i][j])
                    dp[i][j] = dp[i][j - 1];
            }
        }
    }
    return dp[m][n];
}

void print_lcs(char *a, char *b) {
    int m = str_len(a);
    int n = str_len(b);
    int len = dp[m][n];
    char result[64];
    result[len] = 0;

    int i = m, j = n;
    int idx = len - 1;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            result[idx] = a[i - 1];
            idx--;
            i--;
            j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }
    printf("%s", result);
}

int main(void) {
    char a1[] = "ABCBDAB";
    char b1[] = "BDCAB";
    int len1 = lcs_length(a1, b1);
    printf("LCS length: %d\n", len1);
    printf("LCS: ");
    print_lcs(a1, b1);
    printf("\n");

    char a2[] = "AGGTAB";
    char b2[] = "GXTXAYB";
    int len2 = lcs_length(a2, b2);
    printf("LCS length: %d\n", len2);
    printf("LCS: ");
    print_lcs(a2, b2);
    printf("\n");

    char a3[] = "ABCDEF";
    char b3[] = "ABCDEF";
    int len3 = lcs_length(a3, b3);
    printf("LCS length: %d\n", len3);
    printf("LCS: ");
    print_lcs(a3, b3);
    printf("\n");

    char a4[] = "ABC";
    char b4[] = "DEF";
    int len4 = lcs_length(a4, b4);
    printf("LCS length: %d\n", len4);

    return 0;
}
