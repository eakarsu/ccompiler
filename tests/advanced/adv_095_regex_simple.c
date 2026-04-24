// EXPECT: === Literal Matching ===
// EXPECT: match("hello", "hello") = 1
// EXPECT: match("hello", "world") = 0
// EXPECT: match("abc", "xabcy") = 1
// EXPECT: match("abc", "xabyc") = 0
// EXPECT: === Dot Matching ===
// EXPECT: match("h.llo", "hello") = 1
// EXPECT: match("h.llo", "hxllo") = 1
// EXPECT: match("...", "abc") = 1
// EXPECT: match("....", "abc") = 0
// EXPECT: === Star Matching ===
// EXPECT: match("ab*c", "ac") = 1
// EXPECT: match("ab*c", "abc") = 1
// EXPECT: match("ab*c", "abbc") = 1
// EXPECT: match("ab*c", "abbbc") = 1
// EXPECT: match("ab*c", "adc") = 0
// EXPECT: === Dot-Star Matching ===
// EXPECT: match("a.*b", "ab") = 1
// EXPECT: match("a.*b", "axb") = 1
// EXPECT: match("a.*b", "axxxb") = 1
// EXPECT: match("a.*b", "axxxc") = 0
// EXPECT: === Anchor Tests ===
// EXPECT: match_full("abc", "abc") = 1
// EXPECT: match_full("abc", "abcd") = 0
// EXPECT: match_full("a.*d", "abcd") = 1
// EXPECT: match_full("a.c", "abc") = 1
// EXPECT: match_full("a.c", "abbc") = 0
// EXPECT: === Complex Patterns ===
// EXPECT: match("a.*b.*c", "abc") = 1
// EXPECT: match("a.*b.*c", "aXbYc") = 1
// EXPECT: match("a.*b.*c", "aXYc") = 0
// EXPECT: match("x*y*z", "z") = 1
// EXPECT: match("x*y*z", "xxyyyz") = 1
// EXPECT: All regex tests passed!

int printf(const char *fmt, ...);

// Match a pattern at the beginning of text
// Supports: literal chars, '.' (any char), 'c*' (zero or more of c), '.*' (zero or more of any)
int match_here(const char *pattern, const char *text);

int match_star(char c, const char *pattern, const char *text) {
    // c* matches zero or more occurrences of c
    // Try matching zero first, then one, then two, etc.
    int i = 0;
    while (1) {
        if (match_here(pattern, text + i)) return 1;
        if (text[i] == 0) return 0;
        if (c != '.' && text[i] != c) return 0;
        i = i + 1;
    }
    return 0;
}

int match_here(const char *pattern, const char *text) {
    if (pattern[0] == 0) return 1;  // empty pattern matches

    // Check for star: c* where c is pattern[0]
    if (pattern[1] == '*') {
        return match_star(pattern[0], pattern + 2, text);
    }

    // End of text but pattern remains
    if (text[0] == 0) return 0;

    // Dot matches any character
    if (pattern[0] == '.' || pattern[0] == text[0]) {
        return match_here(pattern + 1, text + 1);
    }

    return 0;
}

// Search for pattern anywhere in text
int match(const char *pattern, const char *text) {
    int i = 0;
    // Try matching at every position in text
    while (1) {
        if (match_here(pattern, text + i)) return 1;
        if (text[i] == 0) return 0;
        i = i + 1;
    }
    return 0;
}

// Full match: pattern must match entire text (anchored at both ends)
int match_full_impl(const char *pattern, const char *text) {
    if (pattern[0] == 0) {
        return text[0] == 0;
    }

    if (pattern[1] == '*') {
        // c* : try zero or more
        int i = 0;
        while (1) {
            if (match_full_impl(pattern + 2, text + i)) return 1;
            if (text[i] == 0) return 0;
            if (pattern[0] != '.' && text[i] != pattern[0]) return 0;
            i = i + 1;
        }
        return 0;
    }

    if (text[0] == 0) return 0;

    if (pattern[0] == '.' || pattern[0] == text[0]) {
        return match_full_impl(pattern + 1, text + 1);
    }

    return 0;
}

int main(void) {
    printf("=== Literal Matching ===\n");
    printf("match(\"hello\", \"hello\") = %d\n", match("hello", "hello"));
    printf("match(\"hello\", \"world\") = %d\n", match("hello", "world"));
    printf("match(\"abc\", \"xabcy\") = %d\n", match("abc", "xabcy"));
    printf("match(\"abc\", \"xabyc\") = %d\n", match("abc", "xabyc"));

    printf("=== Dot Matching ===\n");
    printf("match(\"h.llo\", \"hello\") = %d\n", match("h.llo", "hello"));
    printf("match(\"h.llo\", \"hxllo\") = %d\n", match("h.llo", "hxllo"));
    printf("match(\"...\", \"abc\") = %d\n", match("...", "abc"));
    printf("match(\"....\", \"abc\") = %d\n", match("....", "abc"));

    printf("=== Star Matching ===\n");
    printf("match(\"ab*c\", \"ac\") = %d\n", match("ab*c", "ac"));
    printf("match(\"ab*c\", \"abc\") = %d\n", match("ab*c", "abc"));
    printf("match(\"ab*c\", \"abbc\") = %d\n", match("ab*c", "abbc"));
    printf("match(\"ab*c\", \"abbbc\") = %d\n", match("ab*c", "abbbc"));
    printf("match(\"ab*c\", \"adc\") = %d\n", match("ab*c", "adc"));

    printf("=== Dot-Star Matching ===\n");
    printf("match(\"a.*b\", \"ab\") = %d\n", match("a.*b", "ab"));
    printf("match(\"a.*b\", \"axb\") = %d\n", match("a.*b", "axb"));
    printf("match(\"a.*b\", \"axxxb\") = %d\n", match("a.*b", "axxxb"));
    printf("match(\"a.*b\", \"axxxc\") = %d\n", match("a.*b", "axxxc"));

    printf("=== Anchor Tests ===\n");
    printf("match_full(\"abc\", \"abc\") = %d\n", match_full_impl("abc", "abc"));
    printf("match_full(\"abc\", \"abcd\") = %d\n", match_full_impl("abc", "abcd"));
    printf("match_full(\"a.*d\", \"abcd\") = %d\n", match_full_impl("a.*d", "abcd"));
    printf("match_full(\"a.c\", \"abc\") = %d\n", match_full_impl("a.c", "abc"));
    printf("match_full(\"a.c\", \"abbc\") = %d\n", match_full_impl("a.c", "abbc"));

    printf("=== Complex Patterns ===\n");
    printf("match(\"a.*b.*c\", \"abc\") = %d\n", match("a.*b.*c", "abc"));
    printf("match(\"a.*b.*c\", \"aXbYc\") = %d\n", match("a.*b.*c", "aXbYc"));
    printf("match(\"a.*b.*c\", \"aXYc\") = %d\n", match("a.*b.*c", "aXYc"));
    printf("match(\"x*y*z\", \"z\") = %d\n", match("x*y*z", "z"));
    printf("match(\"x*y*z\", \"xxyyyz\") = %d\n", match("x*y*z", "xxyyyz"));

    printf("All regex tests passed!\n");
    return 0;
}
