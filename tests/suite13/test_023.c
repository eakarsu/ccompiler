int printf(const char *fmt, ...);

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int brute_force_search(char *text, char *pattern) {
    int tlen = str_len(text);
    int plen = str_len(pattern);
    int i = 0;
    while (i <= tlen - plen) {
        int j = 0;
        while (j < plen && text[i + j] == pattern[j]) {
            j = j + 1;
        }
        if (j == plen) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

int count_occurrences(char *text, char *pattern) {
    int tlen = str_len(text);
    int plen = str_len(pattern);
    int count = 0;
    int i = 0;
    while (i <= tlen - plen) {
        int j = 0;
        while (j < plen && text[i + j] == pattern[j]) {
            j = j + 1;
        }
        if (j == plen) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

void find_all(char *text, char *pattern, int *positions, int *count) {
    int tlen = str_len(text);
    int plen = str_len(pattern);
    *count = 0;
    int i = 0;
    while (i <= tlen - plen) {
        int j = 0;
        while (j < plen && text[i + j] == pattern[j]) {
            j = j + 1;
        }
        if (j == plen) {
            positions[*count] = i;
            *count = *count + 1;
        }
        i = i + 1;
    }
}

int match_with_wildcard(char *text, char *pattern) {
    int ti = 0;
    int pi = 0;
    int tlen = str_len(text);
    int plen = str_len(pattern);
    while (ti < tlen && pi < plen) {
        if (pattern[pi] == '?') {
            ti = ti + 1;
            pi = pi + 1;
        } else if (pattern[pi] == text[ti]) {
            ti = ti + 1;
            pi = pi + 1;
        } else {
            return 0;
        }
    }
    return (ti == tlen && pi == plen);
}

int starts_with(char *text, char *prefix) {
    int i = 0;
    while (prefix[i] != 0) {
        if (text[i] == 0 || text[i] != prefix[i]) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

int ends_with(char *text, char *suffix) {
    int tlen = str_len(text);
    int slen = str_len(suffix);
    if (slen > tlen) return 0;
    int offset = tlen - slen;
    int i = 0;
    while (i < slen) {
        if (text[offset + i] != suffix[i]) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

int main() {
    int pos;

    pos = brute_force_search("hello world", "world");
    printf("find world: %d\n", pos);
    // EXPECT: find world: 6

    pos = brute_force_search("hello world", "hello");
    printf("find hello: %d\n", pos);
    // EXPECT: find hello: 0

    pos = brute_force_search("hello world", "xyz");
    printf("find xyz: %d\n", pos);
    // EXPECT: find xyz: -1

    pos = brute_force_search("abababab", "abab");
    printf("find abab: %d\n", pos);
    // EXPECT: find abab: 0

    pos = brute_force_search("aaa", "aaaa");
    printf("find aaaa in aaa: %d\n", pos);
    // EXPECT: find aaaa in aaa: -1

    int cnt = count_occurrences("abababab", "ab");
    printf("count ab in abababab: %d\n", cnt);
    // EXPECT: count ab in abababab: 4

    cnt = count_occurrences("aaaaaa", "aa");
    printf("count aa in aaaaaa: %d\n", cnt);
    // EXPECT: count aa in aaaaaa: 5

    cnt = count_occurrences("hello", "xyz");
    printf("count xyz in hello: %d\n", cnt);
    // EXPECT: count xyz in hello: 0

    int positions[20];
    int num;
    find_all("abcabcabc", "abc", positions, &num);
    printf("found %d matches\n", num);
    // EXPECT: found 3 matches
    int k = 0;
    while (k < num) {
        printf("  at %d\n", positions[k]);
        k = k + 1;
    }
    // EXPECT:   at 0
    // EXPECT:   at 3
    // EXPECT:   at 6

    printf("match h?llo with hello: %d\n", match_with_wildcard("hello", "h?llo"));
    // EXPECT: match h?llo with hello: 1

    printf("match h?llo with hxllo: %d\n", match_with_wildcard("hxllo", "h?llo"));
    // EXPECT: match h?llo with hxllo: 1

    printf("match ????? with abcde: %d\n", match_with_wildcard("abcde", "?????"));
    // EXPECT: match ????? with abcde: 1

    printf("match h?llo with hllo: %d\n", match_with_wildcard("hllo", "h?llo"));
    // EXPECT: match h?llo with hllo: 0

    printf("starts hello with he: %d\n", starts_with("hello", "he"));
    // EXPECT: starts hello with he: 1

    printf("starts hello with lo: %d\n", starts_with("hello", "lo"));
    // EXPECT: starts hello with lo: 0

    printf("ends hello with lo: %d\n", ends_with("hello", "lo"));
    // EXPECT: ends hello with lo: 1

    printf("ends hello with he: %d\n", ends_with("hello", "he"));
    // EXPECT: ends hello with he: 0

    printf("done\n");
    // EXPECT: done

    return 0;
}
