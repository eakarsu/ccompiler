int printf(const char *fmt, ...);

/* KMP String Matching Algorithm
   Build failure function, then search for pattern in text.
   Uses char arrays, no string.h. */

int fail[64];  /* failure function */

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

void build_failure(char *pattern, int m) {
    int i;
    int j;

    fail[0] = 0;
    j = 0;
    i = 1;

    while (i < m) {
        if (pattern[i] == pattern[j]) {
            j = j + 1;
            fail[i] = j;
            i = i + 1;
        } else {
            if (j != 0) {
                j = fail[j - 1];
            } else {
                fail[i] = 0;
                i = i + 1;
            }
        }
    }
}

/* KMP search: returns index of first occurrence, or -1 */
int kmp_search(char *text, char *pattern) {
    int n;
    int m;
    int i;
    int j;

    n = str_len(text);
    m = str_len(pattern);

    if (m == 0) return 0;

    build_failure(pattern, m);

    i = 0;
    j = 0;

    while (i < n) {
        if (text[i] == pattern[j]) {
            i = i + 1;
            j = j + 1;
            if (j == m) {
                return i - m;
            }
        } else {
            if (j != 0) {
                j = fail[j - 1];
            } else {
                i = i + 1;
            }
        }
    }
    return -1;
}

/* Count all occurrences */
int kmp_count(char *text, char *pattern) {
    int n;
    int m;
    int i;
    int j;
    int count;

    n = str_len(text);
    m = str_len(pattern);

    if (m == 0) return 0;

    build_failure(pattern, m);

    i = 0;
    j = 0;
    count = 0;

    while (i < n) {
        if (text[i] == pattern[j]) {
            i = i + 1;
            j = j + 1;
            if (j == m) {
                count = count + 1;
                j = fail[j - 1];
            }
        } else {
            if (j != 0) {
                j = fail[j - 1];
            } else {
                i = i + 1;
            }
        }
    }
    return count;
}

void test_failure_function(void) {
    char pat1[16];
    char pat2[16];
    int m;

    printf("=== Failure Function ===\n");
    // EXPECT: === Failure Function ===

    /* "ABABC" -> fail = [0, 0, 1, 2, 0] */
    pat1[0]='A'; pat1[1]='B'; pat1[2]='A'; pat1[3]='B'; pat1[4]='C'; pat1[5]=0;
    m = str_len(pat1);
    build_failure(pat1, m);
    printf("ABABC: %d %d %d %d %d\n", fail[0], fail[1], fail[2], fail[3], fail[4]);
    // EXPECT: ABABC: 0 0 1 2 0

    /* "AABAAAB" -> fail = [0, 1, 0, 1, 2, 2, 3] */
    pat2[0]='A'; pat2[1]='A'; pat2[2]='B'; pat2[3]='A'; pat2[4]='A';
    pat2[5]='A'; pat2[6]='B'; pat2[7]=0;
    m = str_len(pat2);
    build_failure(pat2, m);
    printf("AABAAAB: %d %d %d %d %d %d %d\n",
           fail[0], fail[1], fail[2], fail[3], fail[4], fail[5], fail[6]);
    // EXPECT: AABAAAB: 0 1 0 1 2 2 3
}

void test_search_basic(void) {
    char text[32];
    char pat[16];
    int idx;

    printf("=== KMP Search Basic ===\n");
    // EXPECT: === KMP Search Basic ===

    /* "ABCABDABABC" search "ABABC" -> found at 6 */
    text[0]='A'; text[1]='B'; text[2]='C'; text[3]='A'; text[4]='B';
    text[5]='D'; text[6]='A'; text[7]='B'; text[8]='A'; text[9]='B';
    text[10]='C'; text[11]=0;

    pat[0]='A'; pat[1]='B'; pat[2]='A'; pat[3]='B'; pat[4]='C'; pat[5]=0;

    idx = kmp_search(text, pat);
    printf("Found at: %d\n", idx);
    // EXPECT: Found at: 6

    /* Search for "XYZ" -> not found */
    pat[0]='X'; pat[1]='Y'; pat[2]='Z'; pat[3]=0;
    idx = kmp_search(text, pat);
    printf("Not found: %d\n", idx);
    // EXPECT: Not found: -1
}

void test_search_beginning(void) {
    char text[32];
    char pat[16];
    int idx;

    printf("=== KMP Search at Beginning ===\n");
    // EXPECT: === KMP Search at Beginning ===

    /* "HELLO WORLD" search "HELLO" -> found at 0 */
    text[0]='H'; text[1]='E'; text[2]='L'; text[3]='L'; text[4]='O';
    text[5]=' '; text[6]='W'; text[7]='O'; text[8]='R'; text[9]='L';
    text[10]='D'; text[11]=0;

    pat[0]='H'; pat[1]='E'; pat[2]='L'; pat[3]='L'; pat[4]='O'; pat[5]=0;

    idx = kmp_search(text, pat);
    printf("Found at: %d\n", idx);
    // EXPECT: Found at: 0
}

void test_count_overlapping(void) {
    char text[32];
    char pat[16];
    int cnt;

    printf("=== KMP Count Overlapping ===\n");
    // EXPECT: === KMP Count Overlapping ===

    /* "AAAAAAA" count "AAA" -> overlapping: positions 0,1,2,3,4 = 5 */
    text[0]='A'; text[1]='A'; text[2]='A'; text[3]='A'; text[4]='A';
    text[5]='A'; text[6]='A'; text[7]=0;

    pat[0]='A'; pat[1]='A'; pat[2]='A'; pat[3]=0;

    cnt = kmp_count(text, pat);
    printf("Count AAA in AAAAAAA: %d\n", cnt);
    // EXPECT: Count AAA in AAAAAAA: 5
}

void test_count_multiple(void) {
    char text[32];
    char pat[16];
    int cnt;

    printf("=== KMP Count Multiple ===\n");
    // EXPECT: === KMP Count Multiple ===

    /* "ABABABAB" count "AB" -> 4 times at 0,2,4,6 */
    text[0]='A'; text[1]='B'; text[2]='A'; text[3]='B';
    text[4]='A'; text[5]='B'; text[6]='A'; text[7]='B'; text[8]=0;

    pat[0]='A'; pat[1]='B'; pat[2]=0;

    cnt = kmp_count(text, pat);
    printf("Count AB in ABABABAB: %d\n", cnt);
    // EXPECT: Count AB in ABABABAB: 4
}

void test_single_char(void) {
    char text[32];
    char pat[16];
    int idx;
    int cnt;

    printf("=== Single Char ===\n");
    // EXPECT: === Single Char ===

    text[0]='X'; text[1]='Y'; text[2]='Z'; text[3]='X'; text[4]=0;
    pat[0]='X'; pat[1]=0;

    idx = kmp_search(text, pat);
    printf("First X: %d\n", idx);
    // EXPECT: First X: 0

    cnt = kmp_count(text, pat);
    printf("Count X: %d\n", cnt);
    // EXPECT: Count X: 2
}

int main(void) {
    test_failure_function();
    test_search_basic();
    test_search_beginning();
    test_count_overlapping();
    test_count_multiple();
    test_single_char();

    printf("All KMP tests passed!\n");
    // EXPECT: All KMP tests passed!
    return 0;
}
