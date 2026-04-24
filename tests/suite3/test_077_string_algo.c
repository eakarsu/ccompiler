int printf(const char *fmt, ...);

/* String algorithms: reverse, palindrome, substring search,
   character frequency, manual string compare */

/* String length */
int str_len(const char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

/* Reverse string in-place */
void str_reverse(char *s, int len) {
    int i = 0, j = len - 1;
    while (i < j) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i++; j--;
    }
}

/* Check if string is palindrome */
int is_palindrome(const char *s, int len) {
    int i = 0, j = len - 1;
    while (i < j) {
        if (s[i] != s[j]) return 0;
        i++; j--;
    }
    return 1;
}

/* Naive substring search: return index of first occurrence of pat in txt,
   or -1 if not found */
int str_find(const char *txt, int tlen, const char *pat, int plen) {
    if (plen > tlen) return -1;
    int i, j;
    for (i = 0; i <= tlen - plen; i++) {
        for (j = 0; j < plen; j++) {
            if (txt[i + j] != pat[j]) break;
        }
        if (j == plen) return i;
    }
    return -1;
}

/* Count non-overlapping occurrences of pat in txt */
int str_count(const char *txt, int tlen, const char *pat, int plen) {
    int count = 0, i = 0;
    while (i <= tlen - plen) {
        int j;
        for (j = 0; j < plen; j++) {
            if (txt[i + j] != pat[j]) break;
        }
        if (j == plen) { count++; i += plen; }
        else i++;
    }
    return count;
}

/* Character frequency: count occurrences of ch in s */
int char_freq(const char *s, int len, char ch) {
    int count = 0, i;
    for (i = 0; i < len; i++) {
        if (s[i] == ch) count++;
    }
    return count;
}

/* Manual strcmp: <0, 0, >0 */
int str_cmp(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

/* Manual strncmp */
int str_ncmp(const char *a, const char *b, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] != b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
        if (a[i] == 0) return 0;
    }
    return 0;
}

/* Copy string */
void str_copy(char *dst, const char *src) {
    while (*src) { *dst++ = *src++; }
    *dst = 0;
}

int main(void) {
    /* Length */
    printf("%d\n", str_len("hello"));       /* 5 */
    // EXPECT: 5
    printf("%d\n", str_len(""));            /* 0 */
    // EXPECT: 0
    printf("%d\n", str_len("abcdefgh"));    /* 8 */
    // EXPECT: 8

    /* Reverse */
    char buf[16];
    str_copy(buf, "hello");
    str_reverse(buf, 5);
    printf("%s\n", buf);                    /* olleh */
    // EXPECT: olleh

    str_copy(buf, "abcde");
    str_reverse(buf, 5);
    printf("%s\n", buf);                    /* edcba */
    // EXPECT: edcba

    str_copy(buf, "a");
    str_reverse(buf, 1);
    printf("%s\n", buf);                    /* a */
    // EXPECT: a

    /* Palindrome */
    printf("%d\n", is_palindrome("racecar", 7));    /* 1 */
    // EXPECT: 1
    printf("%d\n", is_palindrome("level", 5));      /* 1 */
    // EXPECT: 1
    printf("%d\n", is_palindrome("abba", 4));       /* 1 */
    // EXPECT: 1
    printf("%d\n", is_palindrome("hello", 5));      /* 0 */
    // EXPECT: 0
    printf("%d\n", is_palindrome("a", 1));          /* 1 */
    // EXPECT: 1
    printf("%d\n", is_palindrome("ab", 2));         /* 0 */
    // EXPECT: 0

    /* Substring search */
    const char *txt = "the quick brown fox jumps";
    int tlen = str_len(txt);
    printf("%d\n", str_find(txt, tlen, "quick", 5));  /* 4 */
    // EXPECT: 4
    printf("%d\n", str_find(txt, tlen, "fox", 3));    /* 16 */
    // EXPECT: 16
    printf("%d\n", str_find(txt, tlen, "the", 3));    /* 0 */
    // EXPECT: 0
    printf("%d\n", str_find(txt, tlen, "jumps", 5));  /* 20 */
    // EXPECT: 20
    printf("%d\n", str_find(txt, tlen, "cat", 3));    /* -1 */
    // EXPECT: -1
    printf("%d\n", str_find(txt, tlen, "ick", 3));    /* 6 */
    // EXPECT: 6

    /* Count occurrences */
    const char *s2 = "abcabcabc";
    printf("%d\n", str_count(s2, 9, "abc", 3));  /* 3 */
    // EXPECT: 3
    printf("%d\n", str_count(s2, 9, "ab", 2));   /* 3 */
    // EXPECT: 3
    printf("%d\n", str_count(s2, 9, "d", 1));    /* 0 */
    // EXPECT: 0

    /* Character frequency */
    const char *s3 = "mississippi";
    int l3 = str_len(s3);
    printf("%d\n", char_freq(s3, l3, 's'));  /* 4 */
    // EXPECT: 4
    printf("%d\n", char_freq(s3, l3, 'i'));  /* 4 */
    // EXPECT: 4
    printf("%d\n", char_freq(s3, l3, 'p'));  /* 2 */
    // EXPECT: 2
    printf("%d\n", char_freq(s3, l3, 'm'));  /* 1 */
    // EXPECT: 1
    printf("%d\n", char_freq(s3, l3, 'z'));  /* 0 */
    // EXPECT: 0

    /* String compare */
    printf("%d\n", str_cmp("abc", "abc") == 0 ? 1 : 0);  /* 1 */
    // EXPECT: 1
    printf("%d\n", str_cmp("abc", "abd") < 0 ? 1 : 0);   /* 1 */
    // EXPECT: 1
    printf("%d\n", str_cmp("abd", "abc") > 0 ? 1 : 0);   /* 1 */
    // EXPECT: 1
    printf("%d\n", str_cmp("", "") == 0 ? 1 : 0);         /* 1 */
    // EXPECT: 1
    printf("%d\n", str_cmp("a", "") > 0 ? 1 : 0);         /* 1 */
    // EXPECT: 1
    printf("%d\n", str_cmp("abc", "abcd") < 0 ? 1 : 0);   /* 1 */
    // EXPECT: 1

    /* Strncmp */
    printf("%d\n", str_ncmp("abcde", "abcXX", 3) == 0 ? 1 : 0); /* 1 */
    // EXPECT: 1
    printf("%d\n", str_ncmp("abcde", "abcXX", 4) == 0 ? 0 : 1); /* 1 */
    // EXPECT: 1

    return 0;
}
