int printf(const char *fmt, ...);

// KMP Pattern Matching - compute failure function and search

void compute_lps(char *pattern, int m, int *lps) {
    int len = 0;
    int i = 1;
    lps[0] = 0;
    while (i < m) {
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

int kmp_search(char *text, int n, char *pat, int m, int *lps, int *results) {
    int i = 0;
    int j = 0;
    int count = 0;
    while (i < n) {
        if (pat[j] == text[i]) {
            i = i + 1;
            j = j + 1;
        }
        if (j == m) {
            results[count] = i - j;
            count = count + 1;
            j = lps[j - 1];
        } else if (i < n && pat[j] != text[i]) {
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
    char pat1[8];
    int lps1[8];
    pat1[0] = 'A'; pat1[1] = 'B'; pat1[2] = 'A'; pat1[3] = 'B';
    pat1[4] = 'C'; pat1[5] = 'A'; pat1[6] = 'B'; pat1[7] = 0;
    compute_lps(pat1, 7, lps1);
    printf("LPS: %d %d %d %d %d %d %d\n", lps1[0], lps1[1], lps1[2], lps1[3], lps1[4], lps1[5], lps1[6]); // EXPECT: LPS: 0 0 1 2 0 1 2

    char pat2[5];
    int lps2[5];
    pat2[0] = 'A'; pat2[1] = 'A'; pat2[2] = 'A'; pat2[3] = 'A'; pat2[4] = 0;
    compute_lps(pat2, 4, lps2);
    printf("LPS2: %d %d %d %d\n", lps2[0], lps2[1], lps2[2], lps2[3]); // EXPECT: LPS2: 0 1 2 3

    char pat3[7];
    int lps3[7];
    pat3[0] = 'A'; pat3[1] = 'B'; pat3[2] = 'C'; pat3[3] = 'D';
    pat3[4] = 'E'; pat3[5] = 'F'; pat3[6] = 0;
    compute_lps(pat3, 6, lps3);
    printf("LPS3: %d %d %d %d %d %d\n", lps3[0], lps3[1], lps3[2], lps3[3], lps3[4], lps3[5]); // EXPECT: LPS3: 0 0 0 0 0 0
}

void test_kmp_basic() {
    char text[16];
    char pat[4];
    int lps[4];
    int results[10];
    text[0] = 'A'; text[1] = 'B'; text[2] = 'C'; text[3] = 'A';
    text[4] = 'B'; text[5] = 'C'; text[6] = 'D'; text[7] = 0;
    pat[0] = 'A'; pat[1] = 'B'; pat[2] = 'C'; pat[3] = 0;
    compute_lps(pat, 3, lps);
    int count = kmp_search(text, 7, pat, 3, lps, results);
    printf("Matches: %d at %d %d\n", count, results[0], results[1]); // EXPECT: Matches: 2 at 0 3
}

void test_kmp_overlap() {
    char text[12];
    char pat[4];
    int lps[4];
    int results[10];
    text[0] = 'A'; text[1] = 'A'; text[2] = 'A'; text[3] = 'A';
    text[4] = 'A'; text[5] = 0;
    pat[0] = 'A'; pat[1] = 'A'; pat[2] = 0;
    compute_lps(pat, 2, lps);
    int count = kmp_search(text, 5, pat, 2, lps, results);
    printf("Overlap: %d at %d %d %d %d\n", count, results[0], results[1], results[2], results[3]); // EXPECT: Overlap: 4 at 0 1 2 3
}

void test_kmp_no_match() {
    char text[8];
    char pat[4];
    int lps[4];
    int results[10];
    text[0] = 'A'; text[1] = 'B'; text[2] = 'C'; text[3] = 0;
    pat[0] = 'X'; pat[1] = 'Y'; pat[2] = 0;
    compute_lps(pat, 2, lps);
    int count = kmp_search(text, 3, pat, 2, lps, results);
    printf("No match count: %d\n", count); // EXPECT: No match count: 0
}

void test_kmp_single_char() {
    char text[8];
    char pat[2];
    int lps[2];
    int results[10];
    text[0] = 'A'; text[1] = 'B'; text[2] = 'A'; text[3] = 'C';
    text[4] = 'A'; text[5] = 0;
    pat[0] = 'A'; pat[1] = 0;
    compute_lps(pat, 1, lps);
    int count = kmp_search(text, 5, pat, 1, lps, results);
    printf("Single: %d at %d %d %d\n", count, results[0], results[1], results[2]); // EXPECT: Single: 3 at 0 2 4
}

int main() {
    test_lps();
    test_kmp_basic();
    test_kmp_overlap();
    test_kmp_no_match();
    test_kmp_single_char();
    printf("KMP test done\n"); // EXPECT: KMP test done
    return 0;
}
