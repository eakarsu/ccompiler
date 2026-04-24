int printf(const char *fmt, ...);

// Edit Distance (Levenshtein Distance) using dynamic programming

int str_len(char *s) {
    int i = 0;
    while (s[i] != 0) {
        i = i + 1;
    }
    return i;
}

int min2(int a, int b) {
    if (a < b) return a;
    return b;
}

int min3(int a, int b, int c) {
    return min2(min2(a, b), c);
}

int edit_distance(char *s1, int n1, char *s2, int n2) {
    // Use a flat array as 2D table: dp[(n1+1) * (n2+1)]
    // Max supported: 20x20
    int dp[441]; // 21*21
    int i;
    int j;

    i = 0;
    while (i <= n1) {
        dp[i * (n2 + 1) + 0] = i;
        i = i + 1;
    }
    j = 0;
    while (j <= n2) {
        dp[0 * (n2 + 1) + j] = j;
        j = j + 1;
    }

    i = 1;
    while (i <= n1) {
        j = 1;
        while (j <= n2) {
            int cost;
            if (s1[i - 1] == s2[j - 1]) {
                cost = 0;
            } else {
                cost = 1;
            }
            dp[i * (n2 + 1) + j] = min3(
                dp[(i - 1) * (n2 + 1) + j] + 1,
                dp[i * (n2 + 1) + (j - 1)] + 1,
                dp[(i - 1) * (n2 + 1) + (j - 1)] + cost
            );
            j = j + 1;
        }
        i = i + 1;
    }
    return dp[n1 * (n2 + 1) + n2];
}

void test_identical() {
    char s1[8];
    char s2[8];
    s1[0]='h'; s1[1]='e'; s1[2]='l'; s1[3]='l'; s1[4]='o'; s1[5]=0;
    s2[0]='h'; s2[1]='e'; s2[2]='l'; s2[3]='l'; s2[4]='o'; s2[5]=0;
    int d = edit_distance(s1, 5, s2, 5);
    printf("Identical: %d\n", d); // EXPECT: Identical: 0
}

void test_one_insert() {
    char s1[4];
    char s2[5];
    s1[0]='c'; s1[1]='a'; s1[2]='t'; s1[3]=0;
    s2[0]='c'; s2[1]='a'; s2[2]='r'; s2[3]='t'; s2[4]=0;
    int d = edit_distance(s1, 3, s2, 4);
    printf("Insert: %d\n", d); // EXPECT: Insert: 1
}

void test_one_delete() {
    char s1[5];
    char s2[4];
    s1[0]='c'; s1[1]='a'; s1[2]='r'; s1[3]='t'; s1[4]=0;
    s2[0]='c'; s2[1]='a'; s2[2]='t'; s2[3]=0;
    int d = edit_distance(s1, 4, s2, 3);
    printf("Delete: %d\n", d); // EXPECT: Delete: 1
}

void test_one_replace() {
    char s1[4];
    char s2[4];
    s1[0]='c'; s1[1]='a'; s1[2]='t'; s1[3]=0;
    s2[0]='c'; s2[1]='o'; s2[2]='t'; s2[3]=0;
    int d = edit_distance(s1, 3, s2, 3);
    printf("Replace: %d\n", d); // EXPECT: Replace: 1
}

void test_kitten_sitting() {
    char s1[8];
    char s2[8];
    s1[0]='k'; s1[1]='i'; s1[2]='t'; s1[3]='t'; s1[4]='e'; s1[5]='n'; s1[6]=0;
    s2[0]='s'; s2[1]='i'; s2[2]='t'; s2[3]='t'; s2[4]='i'; s2[5]='n'; s2[6]='g'; s2[7]=0;
    int d = edit_distance(s1, 6, s2, 7);
    printf("kitten->sitting: %d\n", d); // EXPECT: kitten->sitting: 3
}

void test_empty_strings() {
    char s1[2];
    char s2[6];
    s1[0] = 0;
    s2[0]='h'; s2[1]='e'; s2[2]='l'; s2[3]='l'; s2[4]='o'; s2[5]=0;
    int d1 = edit_distance(s1, 0, s2, 5);
    printf("Empty to hello: %d\n", d1); // EXPECT: Empty to hello: 5

    int d2 = edit_distance(s2, 5, s1, 0);
    printf("Hello to empty: %d\n", d2); // EXPECT: Hello to empty: 5

    char s3[2];
    s3[0] = 0;
    int d3 = edit_distance(s1, 0, s3, 0);
    printf("Both empty: %d\n", d3); // EXPECT: Both empty: 0
}

void test_completely_different() {
    char s1[4];
    char s2[4];
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]=0;
    s2[0]='x'; s2[1]='y'; s2[2]='z'; s2[3]=0;
    int d = edit_distance(s1, 3, s2, 3);
    printf("All different: %d\n", d); // EXPECT: All different: 3
}

void test_prefix() {
    char s1[6];
    char s2[4];
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]='d'; s1[4]='e'; s1[5]=0;
    s2[0]='a'; s2[1]='b'; s2[2]='c'; s2[3]=0;
    int d = edit_distance(s1, 5, s2, 3);
    printf("Prefix: %d\n", d); // EXPECT: Prefix: 2
}

int main() {
    test_identical();
    test_one_insert();
    test_one_delete();
    test_one_replace();
    test_kitten_sitting();
    test_empty_strings();
    test_completely_different();
    test_prefix();
    printf("Edit distance done\n"); // EXPECT: Edit distance done
    return 0;
}
