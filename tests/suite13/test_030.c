int printf(const char *fmt, ...);

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

char to_lower(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

void get_frequency(char *s, int *freq) {
    int i = 0;
    while (i < 26) {
        freq[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (s[i] != 0) {
        char c = to_lower(s[i]);
        if (c >= 'a' && c <= 'z') {
            freq[c - 'a'] = freq[c - 'a'] + 1;
        }
        i = i + 1;
    }
}

int are_anagrams(char *a, char *b) {
    int fa[26];
    int fb[26];
    get_frequency(a, fa);
    get_frequency(b, fb);
    int i = 0;
    while (i < 26) {
        if (fa[i] != fb[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void sort_chars(char *s) {
    int len = str_len(s);
    int i = 0;
    while (i < len - 1) {
        int j = 0;
        while (j < len - 1 - i) {
            if (s[j] > s[j + 1]) {
                char tmp = s[j];
                s[j] = s[j + 1];
                s[j + 1] = tmp;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

void canonical_form(char *src, char *dst) {
    int i = 0;
    int di = 0;
    while (src[i] != 0) {
        char c = to_lower(src[i]);
        if (c >= 'a' && c <= 'z') {
            dst[di] = c;
            di = di + 1;
        }
        i = i + 1;
    }
    dst[di] = 0;
    sort_chars(dst);
}

void group_anagrams(char words[8][20], int n, int *groups, int *num_groups) {
    int assigned[8];
    int i = 0;
    while (i < n) {
        assigned[i] = 0;
        groups[i] = -1;
        i = i + 1;
    }
    *num_groups = 0;
    i = 0;
    while (i < n) {
        if (assigned[i] == 0) {
            groups[i] = *num_groups;
            assigned[i] = 1;
            int j = i + 1;
            while (j < n) {
                if (assigned[j] == 0 && are_anagrams(words[i], words[j])) {
                    groups[j] = *num_groups;
                    assigned[j] = 1;
                }
                j = j + 1;
            }
            *num_groups = *num_groups + 1;
        }
        i = i + 1;
    }
}

int count_anagram_pairs(char words[8][20], int n) {
    int count = 0;
    int i = 0;
    while (i < n) {
        int j = i + 1;
        while (j < n) {
            if (are_anagrams(words[i], words[j])) {
                count = count + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return count;
}

int main() {
    printf("listen vs silent: %d\n", are_anagrams("listen", "silent"));
    // EXPECT: listen vs silent: 1

    printf("hello vs world: %d\n", are_anagrams("hello", "world"));
    // EXPECT: hello vs world: 0

    printf("abc vs cba: %d\n", are_anagrams("abc", "cba"));
    // EXPECT: abc vs cba: 1

    printf("abc vs abcd: %d\n", are_anagrams("abc", "abcd"));
    // EXPECT: abc vs abcd: 0

    printf("aab vs aba: %d\n", are_anagrams("aab", "aba"));
    // EXPECT: aab vs aba: 1

    printf("a vs a: %d\n", are_anagrams("a", "a"));
    // EXPECT: a vs a: 1

    printf("Abc vs cba: %d\n", are_anagrams("Abc", "cba"));
    // EXPECT: Abc vs cba: 1

    char can[50];
    canonical_form("listen", can);
    printf("canonical listen: %s\n", can);
    // EXPECT: canonical listen: eilnst

    canonical_form("silent", can);
    printf("canonical silent: %s\n", can);
    // EXPECT: canonical silent: eilnst

    canonical_form("abc", can);
    printf("canonical abc: %s\n", can);
    // EXPECT: canonical abc: abc

    canonical_form("cba", can);
    printf("canonical cba: %s\n", can);
    // EXPECT: canonical cba: abc

    canonical_form("Hello", can);
    printf("canonical Hello: %s\n", can);
    // EXPECT: canonical Hello: ehllo

    char words[8][20];
    int groups[8];
    int num_groups;

    str_copy(words[0], "eat");
    str_copy(words[1], "tea");
    str_copy(words[2], "tan");
    str_copy(words[3], "ate");
    str_copy(words[4], "nat");
    str_copy(words[5], "bat");

    group_anagrams(words, 6, groups, &num_groups);
    printf("num groups: %d\n", num_groups);
    // EXPECT: num groups: 3

    printf("eat group: %d\n", groups[0]);
    // EXPECT: eat group: 0
    printf("tea group: %d\n", groups[1]);
    // EXPECT: tea group: 0
    printf("tan group: %d\n", groups[2]);
    // EXPECT: tan group: 1
    printf("ate group: %d\n", groups[3]);
    // EXPECT: ate group: 0
    printf("nat group: %d\n", groups[4]);
    // EXPECT: nat group: 1
    printf("bat group: %d\n", groups[5]);
    // EXPECT: bat group: 2

    int pairs = count_anagram_pairs(words, 6);
    printf("anagram pairs: %d\n", pairs);
    // EXPECT: anagram pairs: 4

    str_copy(words[0], "abc");
    str_copy(words[1], "def");
    str_copy(words[2], "ghi");
    group_anagrams(words, 3, groups, &num_groups);
    printf("no anagram groups: %d\n", num_groups);
    // EXPECT: no anagram groups: 3

    str_copy(words[0], "abc");
    str_copy(words[1], "bca");
    str_copy(words[2], "cab");
    group_anagrams(words, 3, groups, &num_groups);
    printf("all anagram groups: %d\n", num_groups);
    // EXPECT: all anagram groups: 1

    pairs = count_anagram_pairs(words, 3);
    printf("all anagram pairs: %d\n", pairs);
    // EXPECT: all anagram pairs: 3

    printf("done\n");
    // EXPECT: done

    return 0;
}
