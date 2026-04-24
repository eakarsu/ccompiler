int printf(const char *fmt, ...);

// Wildcard pattern matching with * and ?
// * matches zero or more characters, ? matches exactly one character

int wildcard_match(char *pattern, char *text) {
    // Use iterative approach with backtracking
    int pi = 0;
    int ti = 0;
    int star_pi = -1;
    int star_ti = -1;

    while (text[ti] != 0) {
        if (pattern[pi] == text[ti] || pattern[pi] == '?') {
            pi = pi + 1;
            ti = ti + 1;
        } else if (pattern[pi] == '*') {
            star_pi = pi;
            star_ti = ti;
            pi = pi + 1;
        } else if (star_pi >= 0) {
            pi = star_pi + 1;
            star_ti = star_ti + 1;
            ti = star_ti;
        } else {
            return 0;
        }
    }

    while (pattern[pi] == '*') {
        pi = pi + 1;
    }

    if (pattern[pi] == 0) {
        return 1;
    }
    return 0;
}

void test_basic_match() {
    int r;
    r = wildcard_match("hello", "hello");
    printf("exact match: %d\n", r);
    // EXPECT: exact match: 1

    r = wildcard_match("hello", "world");
    printf("no match: %d\n", r);
    // EXPECT: no match: 0

    r = wildcard_match("", "");
    printf("empty match: %d\n", r);
    // EXPECT: empty match: 1

    r = wildcard_match("", "abc");
    printf("empty pat: %d\n", r);
    // EXPECT: empty pat: 0
}

void test_question_mark() {
    int r;
    r = wildcard_match("h?llo", "hello");
    printf("? middle: %d\n", r);
    // EXPECT: ? middle: 1

    r = wildcard_match("?ello", "hello");
    printf("? start: %d\n", r);
    // EXPECT: ? start: 1

    r = wildcard_match("hell?", "hello");
    printf("? end: %d\n", r);
    // EXPECT: ? end: 1

    r = wildcard_match("?", "h");
    printf("single ?: %d\n", r);
    // EXPECT: single ?: 1

    r = wildcard_match("?", "");
    printf("? empty: %d\n", r);
    // EXPECT: ? empty: 0

    r = wildcard_match("???", "ab");
    printf("??? short: %d\n", r);
    // EXPECT: ??? short: 0

    r = wildcard_match("???", "abc");
    printf("??? exact: %d\n", r);
    // EXPECT: ??? exact: 1
}

void test_star() {
    int r;
    r = wildcard_match("*", "anything");
    printf("star all: %d\n", r);
    // EXPECT: star all: 1

    r = wildcard_match("*", "");
    printf("star empty: %d\n", r);
    // EXPECT: star empty: 1

    r = wildcard_match("h*o", "hello");
    printf("star mid: %d\n", r);
    // EXPECT: star mid: 1

    r = wildcard_match("h*o", "ho");
    printf("star zero: %d\n", r);
    // EXPECT: star zero: 1

    r = wildcard_match("h*", "hello");
    printf("star end: %d\n", r);
    // EXPECT: star end: 1

    r = wildcard_match("*o", "hello");
    printf("star start: %d\n", r);
    // EXPECT: star start: 1

    r = wildcard_match("*o", "hellox");
    printf("star start fail: %d\n", r);
    // EXPECT: star start fail: 0
}

void test_complex() {
    int r;
    r = wildcard_match("*l*", "hello");
    printf("star l star: %d\n", r);
    // EXPECT: star l star: 1

    r = wildcard_match("h?l*o", "hello");
    printf("mixed: %d\n", r);
    // EXPECT: mixed: 1

    r = wildcard_match("**", "test");
    printf("double star: %d\n", r);
    // EXPECT: double star: 1

    r = wildcard_match("*?*", "a");
    printf("star?star single: %d\n", r);
    // EXPECT: star?star single: 1

    r = wildcard_match("*?*", "");
    printf("star?star empty: %d\n", r);
    // EXPECT: star?star empty: 0

    r = wildcard_match("a*b*c", "aXXbYYc");
    printf("multi star: %d\n", r);
    // EXPECT: multi star: 1

    r = wildcard_match("a*b*c", "aXXbYY");
    printf("multi star fail: %d\n", r);
    // EXPECT: multi star fail: 0

    r = wildcard_match("t?s*ng", "testing");
    printf("complex mix: %d\n", r);
    // EXPECT: complex mix: 1
}

int main() {
    test_basic_match();
    test_question_mark();
    test_star();
    test_complex();

    printf("all wildcard tests done\n");
    // EXPECT: all wildcard tests done
    return 0;
}
