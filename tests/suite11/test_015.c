int printf(const char *fmt, ...);

// Simple regex NFA simulation
// Supports: literal chars, . (any), * (zero or more of prev), + (one or more of prev)
// Uses Thompson NFA construction with on-the-fly simulation

enum RegexType {
    RE_LITERAL,
    RE_DOT,
    RE_STAR,
    RE_PLUS
};

struct RegexNode {
    int type;
    char ch;
};

// Parse regex pattern into node array, return count
int parse_regex(char *pattern, struct RegexNode *nodes) {
    int i = 0;
    int n = 0;

    while (pattern[i] != 0) {
        if (pattern[i] == '.') {
            nodes[n].type = RE_DOT;
            nodes[n].ch = '.';
            n = n + 1;
        } else if (pattern[i] == '*') {
            // Modify previous node to STAR
            if (n > 0) {
                nodes[n - 1].type = RE_STAR;
            }
        } else if (pattern[i] == '+') {
            // One or more: duplicate previous as literal, then make star
            if (n > 0) {
                nodes[n].type = RE_STAR;
                nodes[n].ch = nodes[n - 1].ch;
                n = n + 1;
            }
        } else {
            nodes[n].type = RE_LITERAL;
            nodes[n].ch = pattern[i];
            n = n + 1;
        }
        i = i + 1;
    }
    return n;
}

// Check if a single node matches a character
int node_matches(struct RegexNode *node, char c) {
    if (node->type == RE_DOT || node->type == RE_STAR) {
        if (node->ch == '.') return 1;
        return (node->ch == c) ? 1 : 0;
    }
    if (node->type == RE_LITERAL) {
        return (node->ch == c) ? 1 : 0;
    }
    return 0;
}

// Recursive matching from position in text and regex nodes
// full_match: if 1, requires all text consumed; if 0, partial match ok
int match_at(struct RegexNode *nodes, int nnodes, int ni, char *text, int ti, int full_match) {
    if (ni == nnodes) {
        if (full_match) {
            if (text[ti] == 0) return 1;
            return 0;
        }
        return 1; // partial match: regex consumed is success
    }

    if (nodes[ni].type == RE_STAR) {
        // Try zero occurrences first
        if (match_at(nodes, nnodes, ni + 1, text, ti, full_match)) {
            return 1;
        }
        // Try one or more occurrences
        while (text[ti] != 0 && node_matches(&nodes[ni], text[ti])) {
            ti = ti + 1;
            if (match_at(nodes, nnodes, ni + 1, text, ti, full_match)) {
                return 1;
            }
        }
        return 0;
    }

    if (text[ti] == 0) return 0;

    if (node_matches(&nodes[ni], text[ti])) {
        return match_at(nodes, nnodes, ni + 1, text, ti + 1, full_match);
    }

    return 0;
}

// Full match: pattern must match entire text
int regex_match(char *pattern, char *text) {
    struct RegexNode nodes[64];
    int n = parse_regex(pattern, nodes);
    return match_at(nodes, n, 0, text, 0, 1);
}

// Search: find if pattern occurs anywhere in text
int regex_search(char *pattern, char *text) {
    int i = 0;
    struct RegexNode nodes[64];
    int n = parse_regex(pattern, nodes);

    while (text[i] != 0) {
        if (match_at(nodes, n, 0, text, i, 0)) {
            return i;
        }
        i = i + 1;
    }
    // Also check empty match at end
    if (match_at(nodes, n, 0, text, i, 0)) {
        return i;
    }
    return -1;
}

void test_literal() {
    printf("lit abc-abc: %d\n", regex_match("abc", "abc"));
    // EXPECT: lit abc-abc: 1
    printf("lit abc-abd: %d\n", regex_match("abc", "abd"));
    // EXPECT: lit abc-abd: 0
    printf("lit abc-ab: %d\n", regex_match("abc", "ab"));
    // EXPECT: lit abc-ab: 0
    printf("lit a-a: %d\n", regex_match("a", "a"));
    // EXPECT: lit a-a: 1
}

void test_dot() {
    printf("dot a.c-abc: %d\n", regex_match("a.c", "abc"));
    // EXPECT: dot a.c-abc: 1
    printf("dot a.c-axc: %d\n", regex_match("a.c", "axc"));
    // EXPECT: dot a.c-axc: 1
    printf("dot a.c-ac: %d\n", regex_match("a.c", "ac"));
    // EXPECT: dot a.c-ac: 0
    printf("dot ...-abc: %d\n", regex_match("...", "abc"));
    // EXPECT: dot ...-abc: 1
    printf("dot ...-ab: %d\n", regex_match("...", "ab"));
    // EXPECT: dot ...-ab: 0
}

void test_star() {
    printf("star a*-empty: %d\n", regex_match("a*", ""));
    // EXPECT: star a*-empty: 1
    printf("star a*-a: %d\n", regex_match("a*", "a"));
    // EXPECT: star a*-a: 1
    printf("star a*-aaa: %d\n", regex_match("a*", "aaa"));
    // EXPECT: star a*-aaa: 1
    printf("star a*b-b: %d\n", regex_match("a*b", "b"));
    // EXPECT: star a*b-b: 1
    printf("star a*b-ab: %d\n", regex_match("a*b", "ab"));
    // EXPECT: star a*b-ab: 1
    printf("star a*b-aab: %d\n", regex_match("a*b", "aab"));
    // EXPECT: star a*b-aab: 1
    printf("star .*-xyz: %d\n", regex_match(".*", "xyz"));
    // EXPECT: star .*-xyz: 1
    printf("star a*b*c-abc: %d\n", regex_match("a*b*c", "abc"));
    // EXPECT: star a*b*c-abc: 1
    printf("star a*b*c-c: %d\n", regex_match("a*b*c", "c"));
    // EXPECT: star a*b*c-c: 1
}

void test_plus() {
    printf("plus a+-a: %d\n", regex_match("a+", "a"));
    // EXPECT: plus a+-a: 1
    printf("plus a+-aaa: %d\n", regex_match("a+", "aaa"));
    // EXPECT: plus a+-aaa: 1
    printf("plus a+-empty: %d\n", regex_match("a+", ""));
    // EXPECT: plus a+-empty: 0
    printf("plus a+b-ab: %d\n", regex_match("a+b", "ab"));
    // EXPECT: plus a+b-ab: 1
    printf("plus a+b-b: %d\n", regex_match("a+b", "b"));
    // EXPECT: plus a+b-b: 0
}

void test_search() {
    int r;
    r = regex_search("abc", "xyzabcdef");
    printf("search abc: %d\n", r);
    // EXPECT: search abc: 3

    r = regex_search("a.c", "xxaxcyy");
    printf("search a.c: %d\n", r);
    // EXPECT: search a.c: 2

    r = regex_search("xyz", "hello");
    printf("search xyz: %d\n", r);
    // EXPECT: search xyz: -1

    printf("regex tests done\n");
    // EXPECT: regex tests done
}

int main() {
    test_literal();
    test_dot();
    test_star();
    test_plus();
    test_search();
    return 0;
}
