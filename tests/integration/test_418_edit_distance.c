int printf(const char *fmt, ...);
// EXPECT: Edit dist('kitten','sitting'): 3\nEdit dist('sunday','saturday'): 3\nReplace 'n' with 'r'\nInsert 't'\nInsert 'a'\nTotal ops: 3\nEdit dist('abc','abc'): 0\nEdit dist(empty,'hello'): 5\nEdit dist('horse','ros'): 3
// Test: Edit distance (Levenshtein)

int dp[64][64];

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len++;
    return len;
}

int min3(int a, int b, int c) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

int edit_distance(char *a, char *b) {
    int m = str_len(a);
    int n = str_len(b);
    int i, j;

    for (i = 0; i <= m; i++) dp[i][0] = i;
    for (j = 0; j <= n; j++) dp[0][j] = j;

    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = min3(dp[i - 1][j] + 1,
                                dp[i][j - 1] + 1,
                                dp[i - 1][j - 1] + 1);
            }
        }
    }
    return dp[m][n];
}

void print_operations(char *a, char *b) {
    int m = str_len(a);
    int n = str_len(b);
    int i = m, j = n;
    int ops = 0;

    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && a[i - 1] == b[j - 1]) {
            i--;
            j--;
        } else if (i > 0 && j > 0 && dp[i][j] == dp[i - 1][j - 1] + 1) {
            printf("Replace '%c' with '%c'\n", a[i - 1], b[j - 1]);
            i--;
            j--;
            ops++;
        } else if (j > 0 && dp[i][j] == dp[i][j - 1] + 1) {
            printf("Insert '%c'\n", b[j - 1]);
            j--;
            ops++;
        } else {
            printf("Delete '%c'\n", a[i - 1]);
            i--;
            ops++;
        }
    }
    printf("Total ops: %d\n", ops);
}

int main(void) {
    char a1[] = "kitten";
    char b1[] = "sitting";
    printf("Edit dist('%s','%s'): %d\n", a1, b1, edit_distance(a1, b1));

    char a2[] = "sunday";
    char b2[] = "saturday";
    printf("Edit dist('%s','%s'): %d\n", a2, b2, edit_distance(a2, b2));
    print_operations(a2, b2);

    char a3[] = "abc";
    char b3[] = "abc";
    printf("Edit dist('%s','%s'): %d\n", a3, b3, edit_distance(a3, b3));

    char a4[] = "";
    char b4[] = "hello";
    printf("Edit dist(empty,'%s'): %d\n", b4, edit_distance(a4, b4));

    char a5[] = "horse";
    char b5[] = "ros";
    printf("Edit dist('%s','%s'): %d\n", a5, b5, edit_distance(a5, b5));

    return 0;
}
