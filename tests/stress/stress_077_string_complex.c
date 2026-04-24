// stress_077_string_complex.c - Complex string operations
// EXPECT: kmp_abc_count=4
// EXPECT: kmp_ababc_count=3
// EXPECT: kmp_aa_in_aaaaaa=5
// EXPECT: kmp_not_found=0
// EXPECT: split_count=5
// EXPECT: split_0=hello
// EXPECT: split_2=foo
// EXPECT: split_4=baz
// EXPECT: joined=hello-world-foo-bar-baz
// EXPECT: replaced=a cat sat on a mat
// EXPECT: replace_expand=bbbbbb
// EXPECT: lev_kitten_sitting=3
// EXPECT: lev_same=0
// EXPECT: lev_empty=3
// EXPECT: lev_flaw_lawn=2
// EXPECT: anagram_listen_silent=1
// EXPECT: anagram_hello_world=0
// EXPECT: anagram_abc_cba=1
// EXPECT: formatted=score=42
// EXPECT: formatted_neg=neg=-7

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

int my_strlen(char *s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

void my_strcpy(char *dst, char *src) {
    int i = 0;
    while (src[i]) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

int my_strcmp(char *a, char *b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return *a - *b;
}

void my_strcat(char *dst, char *src) {
    int dlen = my_strlen(dst);
    int i = 0;
    while (src[i]) { dst[dlen + i] = src[i]; i++; }
    dst[dlen + i] = 0;
}

// KMP pattern search
void compute_lps(char *pat, int m, int *lps) {
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

int kmp_search(char *text, char *pattern) {
    int n = my_strlen(text);
    int m = my_strlen(pattern);
    if (m == 0) return 0;
    int lps[64];
    compute_lps(pattern, m, lps);
    int count = 0;
    int i = 0;
    int j = 0;
    while (i < n) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
        }
        if (j == m) {
            count++;
            j = lps[j - 1];
        } else if (i < n && text[i] != pattern[j]) {
            if (j != 0)
                j = lps[j - 1];
            else
                i++;
        }
    }
    return count;
}

// String split by delimiter (returns count, fills tokens array)
int str_split(char *s, char delim, char tokens[10][32]) {
    int count = 0;
    int ti = 0;
    int i = 0;
    while (s[i]) {
        if (s[i] == delim) {
            tokens[count][ti] = 0;
            count++;
            ti = 0;
        } else {
            tokens[count][ti++] = s[i];
        }
        i++;
    }
    tokens[count][ti] = 0;
    count++;
    return count;
}

// Join strings with separator
void str_join(char tokens[10][32], int count, char *sep, char *result) {
    result[0] = 0;
    int i;
    for (i = 0; i < count; i++) {
        if (i > 0) my_strcat(result, sep);
        my_strcat(result, tokens[i]);
    }
}

// String replace all occurrences
void str_replace_all(char *src, char *find, char *replace, char *result) {
    int flen = my_strlen(find);
    int rlen = my_strlen(replace);
    int ri = 0;
    int si = 0;
    int slen = my_strlen(src);
    while (si < slen) {
        // Check if find starts here
        int match = 1;
        int k;
        if (si + flen <= slen) {
            for (k = 0; k < flen; k++) {
                if (src[si + k] != find[k]) { match = 0; break; }
            }
        } else {
            match = 0;
        }
        if (match) {
            for (k = 0; k < rlen; k++) result[ri++] = replace[k];
            si += flen;
        } else {
            result[ri++] = src[si++];
        }
    }
    result[ri] = 0;
}

// Levenshtein distance
int levenshtein(char *s, char *t) {
    int m = my_strlen(s);
    int n = my_strlen(t);
    int dp[16][16]; // small strings only
    int i;
    int j;
    for (i = 0; i <= m; i++) dp[i][0] = i;
    for (j = 0; j <= n; j++) dp[0][j] = j;
    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            int cost = (s[i - 1] != t[j - 1]) ? 1 : 0;
            int del = dp[i - 1][j] + 1;
            int ins = dp[i][j - 1] + 1;
            int sub = dp[i - 1][j - 1] + cost;
            int min = del;
            if (ins < min) min = ins;
            if (sub < min) min = sub;
            dp[i][j] = min;
        }
    }
    return dp[m][n];
}

// Anagram check: count character frequencies
int is_anagram(char *a, char *b) {
    int freq[128];
    int i;
    for (i = 0; i < 128; i++) freq[i] = 0;
    while (*a) { freq[(int)*a]++; a++; }
    while (*b) { freq[(int)*b]--; b++; }
    for (i = 0; i < 128; i++) {
        if (freq[i] != 0) return 0;
    }
    return 1;
}

// Format string builder
void format_int(char *buf, int val) {
    if (val == 0) { buf[0] = '0'; buf[1] = 0; return; }
    int neg = 0;
    if (val < 0) { neg = 1; val = -val; }
    char tmp[16];
    int ti = 0;
    while (val > 0) {
        tmp[ti++] = '0' + (val % 10);
        val /= 10;
    }
    int bi = 0;
    if (neg) buf[bi++] = '-';
    int j;
    for (j = ti - 1; j >= 0; j--) buf[bi++] = tmp[j];
    buf[bi] = 0;
}

void build_formatted(char *result, char *name, int val) {
    // Build "name=val" string
    my_strcpy(result, name);
    my_strcat(result, "=");
    char num[16];
    format_int(num, val);
    my_strcat(result, num);
}

int main(void) {
    // KMP search
    char *text = "ababcababcababcabc";
    int kmp1 = kmp_search(text, "abc");
    printf("kmp_abc_count=%d\n", kmp1);

    int kmp2 = kmp_search(text, "ababc");
    printf("kmp_ababc_count=%d\n", kmp2);

    int kmp3 = kmp_search("aaaaaa", "aa");
    printf("kmp_aa_in_aaaaaa=%d\n", kmp3);

    int kmp4 = kmp_search("hello world", "xyz");
    printf("kmp_not_found=%d\n", kmp4);

    // String split
    char tokens[10][32];
    int ntok = str_split("hello,world,foo,bar,baz", ',', tokens);
    printf("split_count=%d\n", ntok);
    printf("split_0=%s\n", tokens[0]);
    printf("split_2=%s\n", tokens[2]);
    printf("split_4=%s\n", tokens[4]);

    // String join
    char joined[256];
    str_join(tokens, ntok, "-", joined);
    printf("joined=%s\n", joined);

    // String replace all
    char replaced[256];
    str_replace_all("the cat sat on the mat", "the", "a", replaced);
    printf("replaced=%s\n", replaced);

    char replaced2[256];
    str_replace_all("aaa", "a", "bb", replaced2);
    printf("replace_expand=%s\n", replaced2);

    // Levenshtein distance
    printf("lev_kitten_sitting=%d\n", levenshtein("kitten", "sitting"));
    printf("lev_same=%d\n", levenshtein("hello", "hello"));
    printf("lev_empty=%d\n", levenshtein("", "abc"));
    printf("lev_flaw_lawn=%d\n", levenshtein("flaw", "lawn"));

    // Anagram check
    printf("anagram_listen_silent=%d\n", is_anagram("listen", "silent"));
    printf("anagram_hello_world=%d\n", is_anagram("hello", "world"));
    printf("anagram_abc_cba=%d\n", is_anagram("abc", "cba"));

    // Format string builder
    char fmt_buf[64];
    build_formatted(fmt_buf, "score", 42);
    printf("formatted=%s\n", fmt_buf);
    build_formatted(fmt_buf, "neg", -7);
    printf("formatted_neg=%s\n", fmt_buf);

    return 0;
}
