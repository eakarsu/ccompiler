int printf(const char *fmt, ...);

// Simple Regular Expression Matcher
// Supports: literal chars, '.' (any char), '*' (zero or more of previous)
// Anchored matching and search matching

int match_here(char *regexp, char *text);
int match_star(char c, char *regexp, char *text);
int match_full_helper(char *regexp, char *text);
int match_star_full(char c, char *regexp, char *text);

int match_star(char c, char *regexp, char *text) {
    while (1) {
        if (match_here(regexp, text)) return 1;
        if (*text == 0) return 0;
        if (c != '.' && *text != c) return 0;
        text = text + 1;
    }
    return 0;
}

int match_here(char *regexp, char *text) {
    if (regexp[0] == 0) return 1;
    if (regexp[1] == '*') {
        return match_star(regexp[0], regexp + 2, text);
    }
    if (*text == 0) return 0;
    if (regexp[0] == '.' || regexp[0] == *text) {
        return match_here(regexp + 1, text + 1);
    }
    return 0;
}

// Search: finds pattern anywhere in text
int match_search(char *regexp, char *text) {
    while (1) {
        if (match_here(regexp, text)) return 1;
        if (*text == 0) return 0;
        text = text + 1;
    }
    return 0;
}

// Full match variants: pattern must match entire string
int match_star_full(char c, char *regexp, char *text) {
    while (1) {
        if (match_full_helper(regexp, text)) return 1;
        if (*text == 0) return 0;
        if (c != '.' && *text != c) return 0;
        text = text + 1;
    }
    return 0;
}

int match_full_helper(char *regexp, char *text) {
    if (regexp[0] == 0) {
        return (*text == 0) ? 1 : 0;
    }
    if (regexp[1] == '*') {
        return match_star_full(regexp[0], regexp + 2, text);
    }
    if (*text == 0) return 0;
    if (regexp[0] == '.' || regexp[0] == *text) {
        return match_full_helper(regexp + 1, text + 1);
    }
    return 0;
}

int match_full(char *regexp, char *text) {
    return match_full_helper(regexp, text);
}

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len = len + 1;
    return len;
}

void test_search(char *pattern, char *text, int expected) {
    int result = match_search(pattern, text);
    if (result == expected) {
        printf("PASS: /%s/ in '%s' = %d\n", pattern, text, result);
    } else {
        printf("FAIL: /%s/ in '%s' = %d exp %d\n", pattern, text, result, expected);
    }
}

void test_full(char *pattern, char *text, int expected) {
    int result = match_full(pattern, text);
    if (result == expected) {
        printf("PASS: /%s/ full '%s' = %d\n", pattern, text, result);
    } else {
        printf("FAIL: /%s/ full '%s' = %d exp %d\n", pattern, text, result, expected);
    }
}

int main() {
    // Basic literal matching
    // EXPECT: PASS: /abc/ in 'abc' = 1
    test_search("abc", "abc", 1);
    // EXPECT: PASS: /abc/ in 'xabc' = 1
    test_search("abc", "xabc", 1);
    // EXPECT: PASS: /abc/ in 'xyz' = 0
    test_search("abc", "xyz", 0);

    // Dot matching
    // EXPECT: PASS: /a.c/ in 'abc' = 1
    test_search("a.c", "abc", 1);
    // EXPECT: PASS: /a.c/ in 'aXc' = 1
    test_search("a.c", "aXc", 1);
    // EXPECT: PASS: /a.c/ in 'ac' = 0
    test_search("a.c", "ac", 0);

    // Star matching
    // EXPECT: PASS: /ab*c/ in 'ac' = 1
    test_search("ab*c", "ac", 1);
    // EXPECT: PASS: /ab*c/ in 'abc' = 1
    test_search("ab*c", "abc", 1);
    // EXPECT: PASS: /ab*c/ in 'abbc' = 1
    test_search("ab*c", "abbc", 1);
    // EXPECT: PASS: /ab*c/ in 'abbbc' = 1
    test_search("ab*c", "abbbc", 1);

    // Dot star
    // EXPECT: PASS: /a.*c/ in 'ac' = 1
    test_search("a.*c", "ac", 1);
    // EXPECT: PASS: /a.*c/ in 'aXYZc' = 1
    test_search("a.*c", "aXYZc", 1);

    // Full matching tests
    // EXPECT: PASS: /abc/ full 'abc' = 1
    test_full("abc", "abc", 1);
    // EXPECT: PASS: /abc/ full 'abcd' = 0
    test_full("abc", "abcd", 0);
    // EXPECT: PASS: /a.*d/ full 'abcd' = 1
    test_full("a.*d", "abcd", 1);
    // EXPECT: PASS: /a.*/ full 'abc' = 1
    test_full("a.*", "abc", 1);

    // Edge cases with star
    // EXPECT: PASS: /a*/ in 'bbb' = 1
    test_search("a*", "bbb", 1);
    // EXPECT: PASS: /a*/ full 'aaa' = 1
    test_full("a*", "aaa", 1);
    // EXPECT: PASS: /a*/ full 'b' = 0
    test_full("a*", "b", 0);
    // EXPECT: PASS: /a*b/ full 'aaab' = 1
    test_full("a*b", "aaab", 1);
    // EXPECT: PASS: /.*/ full 'hello' = 1
    test_full(".*", "hello", 1);

    // Count 3-char substrings of "abcabc" matching full "a.c"
    char pool[7];
    pool[0] = 'a'; pool[1] = 'b'; pool[2] = 'c';
    pool[3] = 'a'; pool[4] = 'b'; pool[5] = 'c';
    pool[6] = 0;
    int count = 0;
    int i;
    for (i = 0; i < 4; i++) {
        char sub[4];
        sub[0] = pool[i]; sub[1] = pool[i+1]; sub[2] = pool[i+2]; sub[3] = 0;
        if (match_full("a.c", sub)) {
            count = count + 1;
        }
    }
    // EXPECT: Substrings matching /a.c/: 2
    printf("Substrings matching /a.c/: %d\n", count);

    return 0;
}
