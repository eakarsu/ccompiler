int printf(const char *fmt, ...);
// EXPECT: Text: bananas\nSuffix Array:\n  sa[0] = 1 -> ananas\n  sa[1] = 3 -> anas\n  sa[2] = 5 -> as\n  sa[3] = 0 -> bananas\n  sa[4] = 2 -> nanas\n  sa[5] = 4 -> nas\n  sa[6] = 6 -> s\nLCP Array: 3 1 0 0 2 0\nSearch 'ana': pos 3\nSearch 'nan': pos 2\nSearch 'xyz': pos -1\nDone

// Suffix array construction (naive O(n^2 log n) for small strings)

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int suffix_cmp(char *s, int i, int j, int n) {
    while (i < n && j < n) {
        if (s[i] < s[j]) return -1;
        if (s[i] > s[j]) return 1;
        i = i + 1;
        j = j + 1;
    }
    if (i < n) return 1;
    if (j < n) return -1;
    return 0;
}

void build_suffix_array(char *s, int *sa, int n) {
    int i = 0;
    while (i < n) {
        sa[i] = i;
        i = i + 1;
    }
    // Insertion sort on suffix indices
    i = 1;
    while (i < n) {
        int key_idx = sa[i];
        int j = i - 1;
        while (j >= 0 && suffix_cmp(s, sa[j], key_idx, n) > 0) {
            sa[j + 1] = sa[j];
            j = j - 1;
        }
        sa[j + 1] = key_idx;
        i = i + 1;
    }
}

void build_lcp_array(char *s, int *sa, int *lcp, int n) {
    int i = 0;
    while (i < n - 1) {
        int a = sa[i];
        int b = sa[i + 1];
        int l = 0;
        while (a + l < n && b + l < n && s[a + l] == s[b + l]) {
            l = l + 1;
        }
        lcp[i] = l;
        i = i + 1;
    }
}

int search_pattern(char *text, int *sa, int n, char *pattern) {
    int plen = str_len(pattern);
    int lo = 0;
    int hi = n - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        int cmp = 0;
        int i = 0;
        while (i < plen && sa[mid] + i < n) {
            if (text[sa[mid] + i] < pattern[i]) { cmp = -1; break; }
            if (text[sa[mid] + i] > pattern[i]) { cmp = 1; break; }
            i = i + 1;
        }
        if (cmp == 0 && i < plen) cmp = -1;
        if (cmp == 0) return sa[mid];
        if (cmp < 0) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

int main(void) {
    char text[20];
    text[0] = 'b'; text[1] = 'a'; text[2] = 'n';
    text[3] = 'a'; text[4] = 'n'; text[5] = 'a';
    text[6] = 's'; text[7] = 0;

    int n = str_len(text);
    int sa[20];
    int lcp[20];

    build_suffix_array(text, sa, n);

    printf("Text: bananas\n");
    printf("Suffix Array:\n");
    int i = 0;
    while (i < n) {
        printf("  sa[%d] = %d -> ", i, sa[i]);
        int j = sa[i];
        while (j < n) {
            printf("%c", text[j]);
            j = j + 1;
        }
        printf("\n");
        i = i + 1;
    }

    build_lcp_array(text, sa, lcp, n);
    printf("LCP Array:");
    i = 0;
    while (i < n - 1) {
        printf(" %d", lcp[i]);
        i = i + 1;
    }
    printf("\n");

    // Search patterns
    char pat1[4];
    pat1[0] = 'a'; pat1[1] = 'n'; pat1[2] = 'a'; pat1[3] = 0;
    int pos = search_pattern(text, sa, n, pat1);
    printf("Search 'ana': pos %d\n", pos);

    char pat2[4];
    pat2[0] = 'n'; pat2[1] = 'a'; pat2[2] = 'n'; pat2[3] = 0;
    pos = search_pattern(text, sa, n, pat2);
    printf("Search 'nan': pos %d\n", pos);

    char pat3[4];
    pat3[0] = 'x'; pat3[1] = 'y'; pat3[2] = 'z'; pat3[3] = 0;
    pos = search_pattern(text, sa, n, pat3);
    printf("Search 'xyz': pos %d\n", pos);

    printf("Done\n");
    return 0;
}
