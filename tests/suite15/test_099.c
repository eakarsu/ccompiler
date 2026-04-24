int printf(const char *fmt, ...);

/* Suffix Array Construction (Naive)
   Build suffix array using O(n^2 log n) sorting.
   Then use it for pattern search and LCP computation. */

char text[64];
int text_len;
int sa[64];     /* suffix array: sa[i] = starting index of i-th suffix */
int rank_arr[64]; /* rank of each suffix */
int lcp[64];    /* lcp[i] = longest common prefix between sa[i] and sa[i-1] */

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

void set_text(char *s) {
    int i;
    text_len = str_len(s);
    for (i = 0; i <= text_len; i++) {
        text[i] = s[i];
    }
}

/* Compare suffixes starting at positions a and b */
int suffix_cmp(int a, int b) {
    int i = a;
    int j = b;
    while (i < text_len && j < text_len) {
        if (text[i] < text[j]) return -1;
        if (text[i] > text[j]) return 1;
        i = i + 1;
        j = j + 1;
    }
    /* Shorter suffix is "less" */
    if (i == text_len && j < text_len) return -1;
    if (j == text_len && i < text_len) return 1;
    return 0;
}

/* Bubble sort the suffix array */
void build_suffix_array(void) {
    int i;
    int j;
    int tmp;

    for (i = 0; i < text_len; i++) {
        sa[i] = i;
    }

    for (i = 0; i < text_len - 1; i++) {
        for (j = 0; j < text_len - 1 - i; j++) {
            if (suffix_cmp(sa[j], sa[j + 1]) > 0) {
                tmp = sa[j];
                sa[j] = sa[j + 1];
                sa[j + 1] = tmp;
            }
        }
    }

    /* Compute ranks */
    for (i = 0; i < text_len; i++) {
        rank_arr[sa[i]] = i;
    }
}

/* Compute LCP array (Kasai's algorithm simplified) */
void build_lcp(void) {
    int i;
    int j;
    int k;

    lcp[0] = 0;
    for (i = 1; i < text_len; i++) {
        j = sa[i];
        k = sa[i - 1];
        lcp[i] = 0;
        while (j + lcp[i] < text_len && k + lcp[i] < text_len &&
               text[j + lcp[i]] == text[k + lcp[i]]) {
            lcp[i] = lcp[i] + 1;
        }
    }
}

/* Binary search for pattern in suffix array. Returns 1 if found. */
int search_pattern(char *pat) {
    int pat_len;
    int lo;
    int hi;
    int mid;
    int cmp;
    int i;
    int s;

    pat_len = str_len(pat);
    lo = 0;
    hi = text_len - 1;

    while (lo <= hi) {
        mid = (lo + hi) / 2;
        s = sa[mid];
        cmp = 0;
        for (i = 0; i < pat_len && s + i < text_len; i++) {
            if (pat[i] < text[s + i]) { cmp = -1; break; }
            if (pat[i] > text[s + i]) { cmp = 1; break; }
        }
        if (cmp == 0 && i < pat_len) cmp = 1; /* pattern longer */

        if (cmp == 0) return 1;
        if (cmp < 0) hi = mid - 1;
        else lo = mid + 1;
    }
    return 0;
}

/* Count occurrences of pattern */
int count_pattern(char *pat) {
    int pat_len;
    int i;
    int s;
    int count;
    int j;
    int match;

    pat_len = str_len(pat);
    count = 0;

    for (i = 0; i < text_len; i++) {
        s = sa[i];
        match = 1;
        for (j = 0; j < pat_len; j++) {
            if (s + j >= text_len || text[s + j] != pat[j]) {
                match = 0;
                break;
            }
        }
        if (match) count = count + 1;
    }
    return count;
}

void test_banana(void) {
    char s[16];
    char p[8];

    printf("=== Suffix Array: banana ===\n");
    // EXPECT: === Suffix Array: banana ===

    s[0]='b'; s[1]='a'; s[2]='n'; s[3]='a'; s[4]='n'; s[5]='a'; s[6]=0;
    set_text(s);
    build_suffix_array();
    build_lcp();

    /* "banana" suffixes sorted:
       a       (pos 5)
       ana     (pos 3)
       anana   (pos 1)
       banana  (pos 0)
       na      (pos 4)
       nana    (pos 2)
    */
    printf("SA: %d %d %d %d %d %d\n", sa[0], sa[1], sa[2], sa[3], sa[4], sa[5]);
    // EXPECT: SA: 5 3 1 0 4 2

    /* LCP: [0, 1, 3, 0, 0, 2]
       lcp[0]=0 (first)
       lcp[1]=1 (a vs ana -> "a")
       lcp[2]=3 (ana vs anana -> "ana")
       lcp[3]=0 (anana vs banana -> "")
       lcp[4]=0 (banana vs na -> "")
       lcp[5]=2 (na vs nana -> "na")
    */
    printf("LCP: %d %d %d %d %d %d\n", lcp[0], lcp[1], lcp[2], lcp[3], lcp[4], lcp[5]);
    // EXPECT: LCP: 0 1 3 0 0 2

    /* Search for patterns */
    p[0]='a'; p[1]='n'; p[2]='a'; p[3]=0;
    printf("Search ana: %d\n", search_pattern(p));
    // EXPECT: Search ana: 1

    printf("Count ana: %d\n", count_pattern(p));
    // EXPECT: Count ana: 2

    p[0]='n'; p[1]='a'; p[2]=0;
    printf("Search na: %d\n", search_pattern(p));
    // EXPECT: Search na: 1

    printf("Count na: %d\n", count_pattern(p));
    // EXPECT: Count na: 2

    p[0]='x'; p[1]=0;
    printf("Search x: %d\n", search_pattern(p));
    // EXPECT: Search x: 0
}

void test_abcabc(void) {
    char s[16];
    char p[8];

    printf("=== Suffix Array: abcabc ===\n");
    // EXPECT: === Suffix Array: abcabc ===

    s[0]='a'; s[1]='b'; s[2]='c'; s[3]='a'; s[4]='b'; s[5]='c'; s[6]=0;
    set_text(s);
    build_suffix_array();

    /* Suffixes sorted:
       abc     (pos 3)
       abcabc  (pos 0)
       bc      (pos 4)
       bcabc   (pos 1)
       c       (pos 5)
       cabc    (pos 2)
    */
    printf("SA: %d %d %d %d %d %d\n", sa[0], sa[1], sa[2], sa[3], sa[4], sa[5]);
    // EXPECT: SA: 3 0 4 1 5 2

    p[0]='a'; p[1]='b'; p[2]='c'; p[3]=0;
    printf("Count abc: %d\n", count_pattern(p));
    // EXPECT: Count abc: 2

    p[0]='b'; p[1]='c'; p[2]=0;
    printf("Count bc: %d\n", count_pattern(p));
    // EXPECT: Count bc: 2

    p[0]='c'; p[1]='a'; p[2]=0;
    printf("Count ca: %d\n", count_pattern(p));
    // EXPECT: Count ca: 1
}

void test_aaa(void) {
    char s[8];
    char p[8];

    printf("=== Suffix Array: aaaa ===\n");
    // EXPECT: === Suffix Array: aaaa ===

    s[0]='a'; s[1]='a'; s[2]='a'; s[3]='a'; s[4]=0;
    set_text(s);
    build_suffix_array();
    build_lcp();

    /* Suffixes sorted: a(3), aa(2), aaa(1), aaaa(0) */
    printf("SA: %d %d %d %d\n", sa[0], sa[1], sa[2], sa[3]);
    // EXPECT: SA: 3 2 1 0
    printf("LCP: %d %d %d %d\n", lcp[0], lcp[1], lcp[2], lcp[3]);
    // EXPECT: LCP: 0 1 2 3

    p[0]='a'; p[1]='a'; p[2]=0;
    printf("Count aa: %d\n", count_pattern(p));
    // EXPECT: Count aa: 3

    /* Number of distinct substrings = sum(len - sa[i]) - sum(lcp)
       = (4-3)+(4-2)+(4-1)+(4-0) - (0+1+2+3)
       = 1+2+3+4 - 6 = 10 - 6 = 4
       Distinct: a, aa, aaa, aaaa
    */
    {
        int total = 0;
        int lcp_sum = 0;
        int i;
        for (i = 0; i < text_len; i++) {
            total = total + (text_len - sa[i]);
            lcp_sum = lcp_sum + lcp[i];
        }
        printf("Distinct substrings: %d\n", total - lcp_sum);
        // EXPECT: Distinct substrings: 4
    }
}

int main(void) {
    test_banana();
    test_abcabc();
    test_aaa();

    printf("All suffix array tests passed!\n");
    // EXPECT: All suffix array tests passed!
    return 0;
}
