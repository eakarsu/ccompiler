int printf(const char *fmt, ...);
// EXPECT: === Regex Matching Engine ===\n  /hello/ ~ "say hello world" -> MATCH\n  /^hello/ ~ "hello world" -> MATCH\n  /^hello/ ~ "say hello" -> no match\n  /world$/ ~ "hello world" -> MATCH\n  /world$/ ~ "world hello" -> no match\n  /h.llo/ ~ "hello" -> MATCH\n  /h.llo/ ~ "hallo" -> MATCH\n  /ab*c/ ~ "ac" -> MATCH\n  /ab*c/ ~ "abc" -> MATCH\n  /ab*c/ ~ "abbbbc" -> MATCH\n  /ab+c/ ~ "ac" -> no match\n  /ab+c/ ~ "abc" -> MATCH\n  /ab+c/ ~ "abbbbc" -> MATCH\n  /.*end/ ~ "start middle end" -> MATCH\n  /a.b/ ~ "aXb" -> MATCH\n  /a.b/ ~ "ab" -> no match\nMatch counts:\n  'a' in 'abracadabra': 5\n  'ab' in 'abababab': 4\n  '.' in 'hello': 5\nEdge cases:\n  empty pattern on 'test': MATCH\n  '^$' on '': MATCH\n  'x' on '': no match

int str_len(const char *s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

int match_char(char pattern, char text) {
    if (pattern == '.') return text != '\0';
    return pattern == text;
}

int match_here(const char *pattern, const char *text);

int match_star(char c, const char *pattern, const char *text) {
    do {
        if (match_here(pattern, text)) return 1;
    } while (*text != '\0' && match_char(c, *text++));
    return 0;
}

int match_plus(char c, const char *pattern, const char *text) {
    if (*text == '\0' || !match_char(c, *text)) return 0;
    text++;
    return match_star(c, pattern, text);
}

int match_here(const char *pattern, const char *text) {
    if (pattern[0] == '\0') return 1;
    if (pattern[0] == '$' && pattern[1] == '\0') return *text == '\0';
    if (pattern[1] == '*') return match_star(pattern[0], pattern + 2, text);
    if (pattern[1] == '+') return match_plus(pattern[0], pattern + 2, text);
    if (pattern[0] == '\\' && pattern[1] == 'd') {
        if (*text >= '0' && *text <= '9')
            return match_here(pattern + 2, text + 1);
        return 0;
    }
    if (pattern[0] == '\\' && pattern[1] == 'w') {
        if ((*text >= 'a' && *text <= 'z') || (*text >= 'A' && *text <= 'Z') ||
            (*text >= '0' && *text <= '9') || *text == '_')
            return match_here(pattern + 2, text + 1);
        return 0;
    }
    if (*text != '\0' && match_char(pattern[0], *text))
        return match_here(pattern + 1, text + 1);
    return 0;
}

int match(const char *pattern, const char *text) {
    if (pattern[0] == '^')
        return match_here(pattern + 1, text);
    do {
        if (match_here(pattern, text)) return 1;
    } while (*text++ != '\0');
    return 0;
}

int count_matches(const char *pattern, const char *text) {
    int count = 0;
    const char *p = text;
    while (*p != '\0') {
        if (match_here(pattern, p)) count++;
        p++;
    }
    return count;
}

int main(void) {
    printf("=== Regex Matching Engine ===\n");

    struct {
        const char *pattern;
        const char *text;
    } tests[16];

    tests[0].pattern = "hello";      tests[0].text = "say hello world";
    tests[1].pattern = "^hello";     tests[1].text = "hello world";
    tests[2].pattern = "^hello";     tests[2].text = "say hello";
    tests[3].pattern = "world$";     tests[3].text = "hello world";
    tests[4].pattern = "world$";     tests[4].text = "world hello";
    tests[5].pattern = "h.llo";      tests[5].text = "hello";
    tests[6].pattern = "h.llo";      tests[6].text = "hallo";
    tests[7].pattern = "ab*c";       tests[7].text = "ac";
    tests[8].pattern = "ab*c";       tests[8].text = "abc";
    tests[9].pattern = "ab*c";       tests[9].text = "abbbbc";
    tests[10].pattern = "ab+c";      tests[10].text = "ac";
    tests[11].pattern = "ab+c";      tests[11].text = "abc";
    tests[12].pattern = "ab+c";      tests[12].text = "abbbbc";
    tests[13].pattern = ".*end";     tests[13].text = "start middle end";
    tests[14].pattern = "a.b";       tests[14].text = "aXb";
    tests[15].pattern = "a.b";       tests[15].text = "ab";

    int i;
    for (i = 0; i < 16; i++) {
        int result = match(tests[i].pattern, tests[i].text);
        printf("  /%s/ ~ \"%s\" -> %s\n",
               tests[i].pattern, tests[i].text,
               result ? "MATCH" : "no match");
    }

    printf("Match counts:\n");
    printf("  'a' in 'abracadabra': %d\n", count_matches("a", "abracadabra"));
    printf("  'ab' in 'abababab': %d\n", count_matches("ab", "abababab"));
    printf("  '.' in 'hello': %d\n", count_matches(".", "hello"));

    printf("Edge cases:\n");
    printf("  empty pattern on 'test': %s\n", match("", "test") ? "MATCH" : "no match");
    printf("  '^$' on '': %s\n", match("^$", "") ? "MATCH" : "no match");
    printf("  'x' on '': %s\n", match("x", "") ? "MATCH" : "no match");

    return 0;
}
