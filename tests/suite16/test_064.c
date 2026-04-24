int printf(const char *fmt, ...);

// Simple Regex Matcher
// Supports: literal chars, . (any), * (zero or more of prev), + (one or more of prev)
// Uses NFA-style state machine simulation

enum re_type {
    RE_LITERAL,
    RE_DOT,
    RE_STAR,
    RE_PLUS
};

struct re_node {
    int type;
    char ch;
};

struct regex {
    struct re_node nodes[32];
    int count;
};

void regex_compile(struct regex *re, char *pattern, int len) {
    re->count = 0;
    int i;
    for (i = 0; i < len; i++) {
        char c = pattern[i];
        if (c == 46) {
            // dot
            re->nodes[re->count].type = RE_DOT;
            re->nodes[re->count].ch = 0;
            re->count = re->count + 1;
        } else if (c == 42) {
            // star: modify previous node
            if (re->count > 0) {
                re->nodes[re->count - 1].type = RE_STAR;
            }
        } else if (c == 43) {
            // plus: modify previous node
            if (re->count > 0) {
                re->nodes[re->count - 1].type = RE_PLUS;
            }
        } else {
            re->nodes[re->count].type = RE_LITERAL;
            re->nodes[re->count].ch = c;
            re->count = re->count + 1;
        }
    }
}

int match_here(struct regex *re, int ri, char *text, int ti, int tlen) {
    if (ri >= re->count) {
        return 1; // pattern consumed = match
    }
    int type = re->nodes[ri].type;
    char ch = re->nodes[ri].ch;

    if (type == RE_LITERAL) {
        if (ti < tlen && text[ti] == ch) {
            return match_here(re, ri + 1, text, ti + 1, tlen);
        }
        return 0;
    } else if (type == RE_DOT) {
        if (ti < tlen) {
            return match_here(re, ri + 1, text, ti + 1, tlen);
        }
        return 0;
    } else if (type == RE_STAR) {
        // zero or more: try skipping first
        if (match_here(re, ri + 1, text, ti, tlen)) {
            return 1;
        }
        // try consuming one character
        while (ti < tlen) {
            if (ch == 0 || text[ti] == ch) {
                ti = ti + 1;
                if (match_here(re, ri + 1, text, ti, tlen)) {
                    return 1;
                }
            } else {
                return 0;
            }
        }
        return 0;
    } else if (type == RE_PLUS) {
        // one or more: must consume at least one
        while (ti < tlen) {
            if (ch == 0 || text[ti] == ch) {
                ti = ti + 1;
                if (match_here(re, ri + 1, text, ti, tlen)) {
                    return 1;
                }
            } else {
                return 0;
            }
        }
        return 0;
    }
    return 0;
}

int regex_match(struct regex *re, char *text, int tlen) {
    // Try matching at each position
    int i;
    for (i = 0; i <= tlen; i++) {
        if (match_here(re, 0, text, i, tlen)) {
            return 1;
        }
    }
    return 0;
}

int regex_full_match(struct regex *re, char *text, int tlen) {
    return match_here(re, 0, text, 0, tlen);
}

int slen(char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

int main() {
    struct regex re;

    printf("=== Regex Matcher ===\n"); // EXPECT: === Regex Matcher ===

    // Test 1: literal match "abc"
    char p1[4]; p1[0]='a'; p1[1]='b'; p1[2]='c'; p1[3]=0;
    regex_compile(&re, p1, 3);
    printf("Nodes: %d\n", re.count); // EXPECT: Nodes: 3

    char t1[4]; t1[0]='a'; t1[1]='b'; t1[2]='c'; t1[3]=0;
    printf("Match abc: %d\n", regex_full_match(&re, t1, 3)); // EXPECT: Match abc: 1

    char t2[4]; t2[0]='a'; t2[1]='b'; t2[2]='d'; t2[3]=0;
    printf("Match abd: %d\n", regex_full_match(&re, t2, 3)); // EXPECT: Match abd: 0

    // Test 2: dot match "a.c"
    char p2[4]; p2[0]='a'; p2[1]=46; p2[2]='c'; p2[3]=0;
    regex_compile(&re, p2, 3);
    char t3[4]; t3[0]='a'; t3[1]='x'; t3[2]='c'; t3[3]=0;
    printf("Match axc: %d\n", regex_full_match(&re, t3, 3)); // EXPECT: Match axc: 1
    char t4[4]; t4[0]='a'; t4[1]='b'; t4[2]='d'; t4[3]=0;
    printf("Match abd: %d\n", regex_full_match(&re, t4, 3)); // EXPECT: Match abd: 0

    // Test 3: star "ab*c" matches "ac", "abc", "abbc"
    char p3[4]; p3[0]='a'; p3[1]='b'; p3[2]=42; p3[3]='c';
    regex_compile(&re, p3, 4);
    printf("Nodes: %d\n", re.count); // EXPECT: Nodes: 3

    char t5[3]; t5[0]='a'; t5[1]='c'; t5[2]=0;
    printf("Match ac: %d\n", regex_full_match(&re, t5, 2));   // EXPECT: Match ac: 1

    char t6[4]; t6[0]='a'; t6[1]='b'; t6[2]='c'; t6[3]=0;
    printf("Match abc: %d\n", regex_full_match(&re, t6, 3));  // EXPECT: Match abc: 1

    char t7[5]; t7[0]='a'; t7[1]='b'; t7[2]='b'; t7[3]='c'; t7[4]=0;
    printf("Match abbc: %d\n", regex_full_match(&re, t7, 4)); // EXPECT: Match abbc: 1

    char t8[2]; t8[0]='a'; t8[1]=0;
    printf("Match a: %d\n", regex_full_match(&re, t8, 1));    // EXPECT: Match a: 0

    // Test 4: plus "ab+c" matches "abc", "abbc" but not "ac"
    char p4[4]; p4[0]='a'; p4[1]='b'; p4[2]=43; p4[3]='c';
    regex_compile(&re, p4, 4);
    printf("Match ac: %d\n", regex_full_match(&re, t5, 2));   // EXPECT: Match ac: 0
    printf("Match abc: %d\n", regex_full_match(&re, t6, 3));  // EXPECT: Match abc: 1
    printf("Match abbc: %d\n", regex_full_match(&re, t7, 4)); // EXPECT: Match abbc: 1

    // Test 5: substring search with regex_match
    char p5[3]; p5[0]='b'; p5[1]='c'; p5[2]=0;
    regex_compile(&re, p5, 2);
    char t9[5]; t9[0]='x'; t9[1]='b'; t9[2]='c'; t9[3]='y'; t9[4]=0;
    printf("Search bc in xbcy: %d\n", regex_match(&re, t9, 4)); // EXPECT: Search bc in xbcy: 1

    char t10[4]; t10[0]='x'; t10[1]='y'; t10[2]='z'; t10[3]=0;
    printf("Search bc in xyz: %d\n", regex_match(&re, t10, 3)); // EXPECT: Search bc in xyz: 0

    // Test 6: dot star "a.*b"
    char p6[4]; p6[0]='a'; p6[1]=46; p6[2]=42; p6[3]='b';
    regex_compile(&re, p6, 4);
    char t11[5]; t11[0]='a'; t11[1]='x'; t11[2]='y'; t11[3]='b'; t11[4]=0;
    printf("Match axyb: %d\n", regex_full_match(&re, t11, 4)); // EXPECT: Match axyb: 1

    char t12[3]; t12[0]='a'; t12[1]='b'; t12[2]=0;
    printf("Match ab: %d\n", regex_full_match(&re, t12, 2));   // EXPECT: Match ab: 1

    printf("Done\n"); // EXPECT: Done
    return 0;
}
