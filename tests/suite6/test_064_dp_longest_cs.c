int printf(const char *fmt, ...);

int lcs_length(char a[], int alen, char b[], int blen) {
    int dp[30][30];
    int i = 0;
    while (i <= alen) {
        int j = 0;
        while (j <= blen) {
            dp[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
    i = 1;
    while (i <= alen) {
        int j = 1;
        while (j <= blen) {
            if (a[i - 1] == b[j - 1]) {
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
    return dp[alen][blen];
}

int lcs_reconstruct(char a[], int alen, char b[], int blen, char result[]) {
    int dp[30][30];
    int i = 0;
    while (i <= alen) {
        int j = 0;
        while (j <= blen) {
            dp[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
    i = 1;
    while (i <= alen) {
        int j = 1;
        while (j <= blen) {
            if (a[i - 1] == b[j - 1]) {
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

    int len = dp[alen][blen];
    int idx = len;
    result[idx] = 0;
    i = alen;
    int j = blen;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            idx = idx - 1;
            result[idx] = a[i - 1];
            i = i - 1;
            j = j - 1;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i = i - 1;
        } else {
            j = j - 1;
        }
    }
    return len;
}

int lis_length(int arr[], int n) {
    int dp[50];
    int i = 0;
    while (i < n) {
        dp[i] = 1;
        i = i + 1;
    }
    i = 1;
    while (i < n) {
        int j = 0;
        while (j < i) {
            if (arr[j] < arr[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    int maxv = 0;
    i = 0;
    while (i < n) {
        if (dp[i] > maxv) maxv = dp[i];
        i = i + 1;
    }
    return maxv;
}

int strlen_manual(char s[]) {
    int len = 0;
    while (s[len] != 0) len = len + 1;
    return len;
}

int lcs_int(int a[], int alen, int b[], int blen) {
    int dp[30][30];
    int i = 0;
    while (i <= alen) {
        int j = 0;
        while (j <= blen) {
            dp[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
    i = 1;
    while (i <= alen) {
        int j = 1;
        while (j <= blen) {
            if (a[i - 1] == b[j - 1]) {
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
    return dp[alen][blen];
}

int main(void) {
    char a1[10];
    a1[0]='A'; a1[1]='B'; a1[2]='C'; a1[3]='D'; a1[4]='E'; a1[5]=0;
    char b1[10];
    b1[0]='A'; b1[1]='C'; b1[2]='E'; b1[3]=0;
    // EXPECT: lcs(ABCDE,ACE) = 3
    printf("lcs(ABCDE,ACE) = %d\n", lcs_length(a1, 5, b1, 3));

    char a2[10];
    a2[0]='A'; a2[1]='B'; a2[2]='C'; a2[3]=0;
    char b2[10];
    b2[0]='D'; b2[1]='E'; b2[2]='F'; b2[3]=0;
    // EXPECT: lcs(ABC,DEF) = 0
    printf("lcs(ABC,DEF) = %d\n", lcs_length(a2, 3, b2, 3));

    char a3[10];
    a3[0]='A'; a3[1]='G'; a3[2]='G'; a3[3]='T'; a3[4]='A'; a3[5]='B'; a3[6]=0;
    char b3[10];
    b3[0]='G'; b3[1]='X'; b3[2]='T'; b3[3]='X'; b3[4]='A'; b3[5]='Y'; b3[6]='B'; b3[7]=0;
    // EXPECT: lcs(AGGTAB,GXTXAYB) = 4
    printf("lcs(AGGTAB,GXTXAYB) = %d\n", lcs_length(a3, 6, b3, 7));

    char res[30];
    int len = lcs_reconstruct(a1, 5, b1, 3, res);
    // EXPECT: recon len=3
    printf("recon len=%d\n", len);
    // EXPECT: recon str=ACE
    printf("recon str=%s\n", res);

    len = lcs_reconstruct(a3, 6, b3, 7, res);
    // EXPECT: recon2 len=4
    printf("recon2 len=%d\n", len);
    // EXPECT: recon2 str=GTAB
    printf("recon2 str=%s\n", res);

    char same[5];
    same[0]='X'; same[1]='Y'; same[2]='Z'; same[3]=0;
    // EXPECT: lcs(same,same) = 3
    printf("lcs(same,same) = %d\n", lcs_length(same, 3, same, 3));

    int arr1[8];
    arr1[0]=10; arr1[1]=9; arr1[2]=2; arr1[3]=5;
    arr1[4]=3; arr1[5]=7; arr1[6]=101; arr1[7]=18;
    // EXPECT: lis = 4
    printf("lis = %d\n", lis_length(arr1, 8));

    int arr2[6];
    arr2[0]=0; arr2[1]=1; arr2[2]=0; arr2[3]=3; arr2[4]=2; arr2[5]=3;
    // EXPECT: lis2 = 4
    printf("lis2 = %d\n", lis_length(arr2, 6));

    int arr3[5];
    arr3[0]=5; arr3[1]=4; arr3[2]=3; arr3[3]=2; arr3[4]=1;
    // EXPECT: lis_dec = 1
    printf("lis_dec = %d\n", lis_length(arr3, 5));

    int ia[5];
    ia[0]=1; ia[1]=3; ia[2]=4; ia[3]=1; ia[4]=5;
    int ib[5];
    ib[0]=1; ib[1]=4; ib[2]=5; ib[3]=2; ib[4]=6;
    // EXPECT: lcs_int = 3
    printf("lcs_int = %d\n", lcs_int(ia, 5, ib, 5));

    // EXPECT: lcs_empty = 0
    printf("lcs_empty = %d\n", lcs_length(a1, 0, b1, 3));

    // EXPECT: lis_single = 1
    int single[1];
    single[0] = 42;
    printf("lis_single = %d\n", lis_length(single, 1));

    int arr4[6];
    arr4[0]=1; arr4[1]=2; arr4[2]=3; arr4[3]=4; arr4[4]=5; arr4[5]=6;
    // EXPECT: lis_sorted = 6
    printf("lis_sorted = %d\n", lis_length(arr4, 6));

    char x1[5];
    x1[0]='A'; x1[1]='B'; x1[2]=0;
    char x2[5];
    x2[0]='A'; x2[1]='B'; x2[2]=0;
    // EXPECT: lcs_equal = 2
    printf("lcs_equal = %d\n", lcs_length(x1, 2, x2, 2));

    return 0;
}
