int printf(const char *fmt, ...);

// Longest Common Subsequence (LCS) using dynamic programming

int max2(int a, int b) {
    if (a > b) return a;
    return b;
}

int lcs_length(char *s1, int n1, char *s2, int n2) {
    // dp table: (n1+1)*(n2+1), max 21*21=441
    int dp[441];
    int i;
    int j;

    i = 0;
    while (i <= n1) {
        dp[i * (n2 + 1) + 0] = 0;
        i = i + 1;
    }
    j = 0;
    while (j <= n2) {
        dp[0 * (n2 + 1) + j] = 0;
        j = j + 1;
    }

    i = 1;
    while (i <= n1) {
        j = 1;
        while (j <= n2) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i * (n2 + 1) + j] = dp[(i - 1) * (n2 + 1) + (j - 1)] + 1;
            } else {
                dp[i * (n2 + 1) + j] = max2(
                    dp[(i - 1) * (n2 + 1) + j],
                    dp[i * (n2 + 1) + (j - 1)]
                );
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return dp[n1 * (n2 + 1) + n2];
}

void lcs_reconstruct(char *s1, int n1, char *s2, int n2, char *out, int *out_len) {
    int dp[441];
    int i;
    int j;

    i = 0;
    while (i <= n1) {
        dp[i * (n2 + 1) + 0] = 0;
        i = i + 1;
    }
    j = 0;
    while (j <= n2) {
        dp[0 * (n2 + 1) + j] = 0;
        j = j + 1;
    }

    i = 1;
    while (i <= n1) {
        j = 1;
        while (j <= n2) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i * (n2 + 1) + j] = dp[(i - 1) * (n2 + 1) + (j - 1)] + 1;
            } else {
                dp[i * (n2 + 1) + j] = max2(
                    dp[(i - 1) * (n2 + 1) + j],
                    dp[i * (n2 + 1) + (j - 1)]
                );
            }
            j = j + 1;
        }
        i = i + 1;
    }

    // Backtrack to find the LCS string
    int len = dp[n1 * (n2 + 1) + n2];
    int idx = len - 1;
    i = n1;
    j = n2;
    while (i > 0 && j > 0) {
        if (s1[i - 1] == s2[j - 1]) {
            out[idx] = s1[i - 1];
            idx = idx - 1;
            i = i - 1;
            j = j - 1;
        } else if (dp[(i - 1) * (n2 + 1) + j] > dp[i * (n2 + 1) + (j - 1)]) {
            i = i - 1;
        } else {
            j = j - 1;
        }
    }
    out[len] = 0;
    *out_len = len;
}

void test_basic_lcs() {
    char s1[8];
    char s2[8];
    s1[0]='A'; s1[1]='B'; s1[2]='C'; s1[3]='D'; s1[4]='E'; s1[5]=0;
    s2[0]='A'; s2[1]='C'; s2[2]='E'; s2[3]=0;
    int len = lcs_length(s1, 5, s2, 3);
    printf("LCS length: %d\n", len); // EXPECT: LCS length: 3
}

void test_identical() {
    char s1[6];
    char s2[6];
    s1[0]='H'; s1[1]='E'; s1[2]='L'; s1[3]='L'; s1[4]='O'; s1[5]=0;
    s2[0]='H'; s2[1]='E'; s2[2]='L'; s2[3]='L'; s2[4]='O'; s2[5]=0;
    int len = lcs_length(s1, 5, s2, 5);
    printf("Identical LCS: %d\n", len); // EXPECT: Identical LCS: 5
}

void test_no_common() {
    char s1[4];
    char s2[4];
    s1[0]='A'; s1[1]='B'; s1[2]='C'; s1[3]=0;
    s2[0]='X'; s2[1]='Y'; s2[2]='Z'; s2[3]=0;
    int len = lcs_length(s1, 3, s2, 3);
    printf("No common: %d\n", len); // EXPECT: No common: 0
}

void test_reconstruct() {
    char s1[8];
    char s2[7];
    char result[20];
    int rlen;
    s1[0]='A'; s1[1]='G'; s1[2]='G'; s1[3]='T'; s1[4]='A'; s1[5]='B'; s1[6]=0;
    s2[0]='G'; s2[1]='X'; s2[2]='T'; s2[3]='X'; s2[4]='A'; s2[5]='B'; s2[6]=0;
    lcs_reconstruct(s1, 6, s2, 6, result, &rlen);
    printf("LCS string: %s\n", result); // EXPECT: LCS string: GTAB
    printf("LCS len: %d\n", rlen); // EXPECT: LCS len: 4
}

void test_one_empty() {
    char s1[4];
    char s2[2];
    s1[0]='A'; s1[1]='B'; s1[2]='C'; s1[3]=0;
    s2[0]=0;
    int len = lcs_length(s1, 3, s2, 0);
    printf("One empty: %d\n", len); // EXPECT: One empty: 0
}

void test_single_match() {
    char s1[4];
    char s2[4];
    s1[0]='A'; s1[1]='B'; s1[2]='C'; s1[3]=0;
    s2[0]='X'; s2[1]='B'; s2[2]='Z'; s2[3]=0;
    int len = lcs_length(s1, 3, s2, 3);
    printf("Single match: %d\n", len); // EXPECT: Single match: 1
}

void test_longer_strings() {
    char s1[10];
    char s2[10];
    // ABCBDAB vs BDCABA
    s1[0]='A'; s1[1]='B'; s1[2]='C'; s1[3]='B';
    s1[4]='D'; s1[5]='A'; s1[6]='B'; s1[7]=0;
    s2[0]='B'; s2[1]='D'; s2[2]='C'; s2[3]='A';
    s2[4]='B'; s2[5]='A'; s2[6]=0;
    int len = lcs_length(s1, 7, s2, 6);
    printf("Longer LCS: %d\n", len); // EXPECT: Longer LCS: 4

    char result[20];
    int rlen;
    lcs_reconstruct(s1, 7, s2, 6, result, &rlen);
    printf("Longer LCS str: %s\n", result); // EXPECT: Longer LCS str: BDAB
}

int main() {
    test_basic_lcs();
    test_identical();
    test_no_common();
    test_reconstruct();
    test_one_empty();
    test_single_match();
    test_longer_strings();
    printf("LCS done\n"); // EXPECT: LCS done
    return 0;
}
