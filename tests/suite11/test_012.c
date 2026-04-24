int printf(const char *fmt, ...);

// KMP (Knuth-Morris-Pratt) string search algorithm

void compute_lps(char *pattern, int pat_len, int *lps) {
    int len = 0;
    int i = 1;
    lps[0] = 0;

    while (i < pat_len) {
        if (pattern[i] == pattern[len]) {
            len = len + 1;
            lps[i] = len;
            i = i + 1;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i = i + 1;
            }
        }
    }
}

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) {
        n = n + 1;
    }
    return n;
}

// KMP search: returns index of first occurrence, or -1
int kmp_search(char *text, char *pattern) {
    int n = str_len(text);
    int m = str_len(pattern);
    int lps[64];
    int i;
    int j;

    if (m == 0) return 0;
    if (n == 0) return -1;

    compute_lps(pattern, m, lps);

    i = 0;
    j = 0;
    while (i < n) {
        if (text[i] == pattern[j]) {
            i = i + 1;
            j = j + 1;
        }
        if (j == m) {
            return i - j;
        }
        if (i < n && text[i] != pattern[j]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i = i + 1;
            }
        }
    }
    return -1;
}

// Count all occurrences
int kmp_count(char *text, char *pattern) {
    int n = str_len(text);
    int m = str_len(pattern);
    int lps[64];
    int i;
    int j;
    int count = 0;

    if (m == 0) return 0;

    compute_lps(pattern, m, lps);

    i = 0;
    j = 0;
    while (i < n) {
        if (text[i] == pattern[j]) {
            i = i + 1;
            j = j + 1;
        }
        if (j == m) {
            count = count + 1;
            j = lps[j - 1];
        } else if (i < n && text[i] != pattern[j]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i = i + 1;
            }
        }
    }
    return count;
}

void test_lps() {
    int lps[64];
    int i;

    compute_lps("AABAACAABAA", 11, lps);
    printf("LPS:");
    for (i = 0; i < 11; i = i + 1) {
        printf(" %d", lps[i]);
    }
    printf("\n");
    // EXPECT: LPS: 0 1 0 1 2 0 1 2 3 4 5

    compute_lps("ABCABD", 6, lps);
    printf("LPS2:");
    for (i = 0; i < 6; i = i + 1) {
        printf(" %d", lps[i]);
    }
    printf("\n");
    // EXPECT: LPS2: 0 0 0 1 2 0

    compute_lps("AAAA", 4, lps);
    printf("LPS3:");
    for (i = 0; i < 4; i = i + 1) {
        printf(" %d", lps[i]);
    }
    printf("\n");
    // EXPECT: LPS3: 0 1 2 3
}

void test_search() {
    int r;

    r = kmp_search("hello world", "world");
    printf("find world: %d\n", r);
    // EXPECT: find world: 6

    r = kmp_search("hello world", "hello");
    printf("find hello: %d\n", r);
    // EXPECT: find hello: 0

    r = kmp_search("hello world", "xyz");
    printf("find xyz: %d\n", r);
    // EXPECT: find xyz: -1

    r = kmp_search("AABAACAABAA", "AABAA");
    printf("find AABAA: %d\n", r);
    // EXPECT: find AABAA: 0

    r = kmp_search("abcabcabc", "abcabc");
    printf("find overlap: %d\n", r);
    // EXPECT: find overlap: 0

    r = kmp_search("aaaaa", "aa");
    printf("find aa: %d\n", r);
    // EXPECT: find aa: 0

    r = kmp_search("", "abc");
    printf("empty text: %d\n", r);
    // EXPECT: empty text: -1

    r = kmp_search("abc", "");
    printf("empty pat: %d\n", r);
    // EXPECT: empty pat: 0
}

void test_count() {
    int c;

    c = kmp_count("aaaa", "aa");
    printf("count aa in aaaa: %d\n", c);
    // EXPECT: count aa in aaaa: 3

    c = kmp_count("abababab", "ab");
    printf("count ab: %d\n", c);
    // EXPECT: count ab: 4

    c = kmp_count("hello world hello", "hello");
    printf("count hello: %d\n", c);
    // EXPECT: count hello: 2

    c = kmp_count("abcdef", "xyz");
    printf("count none: %d\n", c);
    // EXPECT: count none: 0

    c = kmp_count("aaa", "a");
    printf("count a in aaa: %d\n", c);
    // EXPECT: count a in aaa: 3
}

int main() {
    test_lps();
    test_search();
    test_count();

    printf("KMP tests done\n");
    // EXPECT: KMP tests done
    return 0;
}
