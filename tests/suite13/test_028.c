int printf(const char *fmt, ...);

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

void str_copy_n(char *dst, char *src, int start, int n) {
    int i = 0;
    while (i < n) {
        dst[i] = src[start + i];
        i = i + 1;
    }
    dst[n] = 0;
}

int lcs_length(char *a, char *b) {
    int alen = str_len(a);
    int blen = str_len(b);
    int best = 0;
    int table[21][21];

    int i = 0;
    while (i <= alen) {
        int j = 0;
        while (j <= blen) {
            table[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }

    i = 1;
    while (i <= alen) {
        int j = 1;
        while (j <= blen) {
            if (a[i - 1] == b[j - 1]) {
                table[i][j] = table[i - 1][j - 1] + 1;
                if (table[i][j] > best) {
                    best = table[i][j];
                }
            } else {
                table[i][j] = 0;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return best;
}

void lcs_find(char *a, char *b, char *result) {
    int alen = str_len(a);
    int blen = str_len(b);
    int best = 0;
    int best_end_i = 0;
    int table[21][21];

    int i = 0;
    while (i <= alen) {
        int j = 0;
        while (j <= blen) {
            table[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }

    i = 1;
    while (i <= alen) {
        int j = 1;
        while (j <= blen) {
            if (a[i - 1] == b[j - 1]) {
                table[i][j] = table[i - 1][j - 1] + 1;
                if (table[i][j] > best) {
                    best = table[i][j];
                    best_end_i = i;
                }
            } else {
                table[i][j] = 0;
            }
            j = j + 1;
        }
        i = i + 1;
    }

    if (best > 0) {
        str_copy_n(result, a, best_end_i - best, best);
    } else {
        result[0] = 0;
    }
}

int count_common_substrings(char *a, char *b, int min_len) {
    int alen = str_len(a);
    int blen = str_len(b);
    int count = 0;
    int i = 0;
    while (i < alen) {
        int j = 0;
        while (j < blen) {
            int k = 0;
            while (i + k < alen && j + k < blen && a[i + k] == b[j + k]) {
                k = k + 1;
            }
            if (k >= min_len) {
                count = count + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return count;
}

int has_common_substring(char *a, char *b, int min_len) {
    return lcs_length(a, b) >= min_len;
}

int main() {
    char result[50];

    int len = lcs_length("abcdef", "abcxyz");
    printf("lcs len abcdef vs abcxyz: %d\n", len);
    // EXPECT: lcs len abcdef vs abcxyz: 3

    lcs_find("abcdef", "abcxyz", result);
    printf("lcs: %s\n", result);
    // EXPECT: lcs: abc

    len = lcs_length("xyzabc", "defabc");
    printf("lcs len xyzabc vs defabc: %d\n", len);
    // EXPECT: lcs len xyzabc vs defabc: 3

    lcs_find("xyzabc", "defabc", result);
    printf("lcs: %s\n", result);
    // EXPECT: lcs: abc

    len = lcs_length("abcde", "fghij");
    printf("lcs len no common: %d\n", len);
    // EXPECT: lcs len no common: 0

    lcs_find("abcde", "fghij", result);
    printf("lcs empty: %d\n", str_len(result));
    // EXPECT: lcs empty: 0

    len = lcs_length("abab", "baba");
    printf("lcs len abab vs baba: %d\n", len);
    // EXPECT: lcs len abab vs baba: 3

    lcs_find("abab", "baba", result);
    printf("lcs: %s\n", result);
    // EXPECT: lcs: aba

    len = lcs_length("aaaa", "aa");
    printf("lcs len aaaa vs aa: %d\n", len);
    // EXPECT: lcs len aaaa vs aa: 2

    len = lcs_length("hello", "hello");
    printf("lcs len identical: %d\n", len);
    // EXPECT: lcs len identical: 5

    lcs_find("hello", "hello", result);
    printf("lcs identical: %s\n", result);
    // EXPECT: lcs identical: hello

    len = lcs_length("a", "a");
    printf("lcs single char: %d\n", len);
    // EXPECT: lcs single char: 1

    len = lcs_length("a", "b");
    printf("lcs diff single: %d\n", len);
    // EXPECT: lcs diff single: 0

    int hcs = has_common_substring("programming", "grammar", 3);
    printf("programming vs grammar min 3: %d\n", hcs);
    // EXPECT: programming vs grammar min 3: 1

    hcs = has_common_substring("abc", "xyz", 1);
    printf("abc vs xyz min 1: %d\n", hcs);
    // EXPECT: abc vs xyz min 1: 0

    int ccount = count_common_substrings("abc", "abc", 1);
    printf("common substrings abc vs abc min 1: %d\n", ccount);
    // EXPECT: common substrings abc vs abc min 1: 3

    ccount = count_common_substrings("ab", "ab", 2);
    printf("common substrings ab vs ab min 2: %d\n", ccount);
    // EXPECT: common substrings ab vs ab min 2: 1

    lcs_find("abcxyzdef", "xyzabcdef", result);
    printf("lcs overlap: %s\n", result);
    // EXPECT: lcs overlap: abc

    printf("done\n");
    // EXPECT: done

    return 0;
}
