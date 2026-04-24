int printf(const char *fmt, ...);
// EXPECT: Text: AABAACAADAABAABA\nPattern: AABA\nFound at index 0\nFound at index 9\nFound at index 12\nOccurrences: 3\n\nText: ABABDABACDABABCABAB\nPattern: ABABCABAB\nFound at index 10\nOccurrences: 1\n\nText: AAAAAA\nPattern: AA\nFound at index 0\nFound at index 1\nFound at index 2\nFound at index 3\nFound at index 4\nOccurrences: 5\n\nText: ABCDEFG\nPattern: XYZ\nOccurrences: 0
// Test: KMP string matching algorithm

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n++;
    return n;
}

void compute_lps(char *pat, int m, int lps[]) {
    int len = 0;
    int i = 1;
    lps[0] = 0;

    while (i < m) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

int kmp_search(char *text, char *pat) {
    int n = str_len(text);
    int m = str_len(pat);
    int lps[64];
    int count = 0;
    int i = 0, j = 0;

    compute_lps(pat, m, lps);

    while (i < n) {
        if (pat[j] == text[i]) {
            i++;
            j++;
        }
        if (j == m) {
            printf("Found at index %d\n", i - j);
            count++;
            j = lps[j - 1];
        } else if (i < n && pat[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return count;
}

int main(void) {
    char t1[] = "AABAACAADAABAABA";
    char p1[] = "AABA";
    printf("Text: %s\nPattern: %s\n", t1, p1);
    int c1 = kmp_search(t1, p1);
    printf("Occurrences: %d\n\n", c1);

    char t2[] = "ABABDABACDABABCABAB";
    char p2[] = "ABABCABAB";
    printf("Text: %s\nPattern: %s\n", t2, p2);
    int c2 = kmp_search(t2, p2);
    printf("Occurrences: %d\n\n", c2);

    char t3[] = "AAAAAA";
    char p3[] = "AA";
    printf("Text: %s\nPattern: %s\n", t3, p3);
    int c3 = kmp_search(t3, p3);
    printf("Occurrences: %d\n\n", c3);

    char t4[] = "ABCDEFG";
    char p4[] = "XYZ";
    printf("Text: %s\nPattern: %s\n", t4, p4);
    int c4 = kmp_search(t4, p4);
    printf("Occurrences: %d\n", c4);

    return 0;
}
