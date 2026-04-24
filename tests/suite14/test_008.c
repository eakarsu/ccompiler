int printf(const char *fmt, ...);

/* Regex-like pattern matcher using state tables.
   Supports: literal chars, '.' (any char), '*' (zero or more of previous),
             '+' (one or more of previous).
   Compiles pattern into a transition table then runs the DFA-like simulation. */

/* Pattern representation:
   Each pattern node has a type and a character.
   Types: LITERAL=0, DOT=1, STAR=2, PLUS=3, ACCEPT=4 */

typedef struct {
    int type;
    char ch;
} PatNode;

typedef struct {
    PatNode nodes[20];
    int len;
} Pattern;

void compile_pattern(char *pat, Pattern *p) {
    int i = 0;
    int j = 0;
    while (pat[i] != 0) {
        if (pat[i] == '.') {
            p->nodes[j].type = 1;
            p->nodes[j].ch = 0;
            j++;
        } else if (pat[i] == '*') {
            /* Modify previous node: mark it as STAR */
            if (j > 0) {
                p->nodes[j].type = 2;
                p->nodes[j].ch = p->nodes[j-1].ch;
                /* Keep previous node, star is separate */
                j++;
            }
        } else if (pat[i] == '+') {
            if (j > 0) {
                p->nodes[j].type = 3;
                p->nodes[j].ch = p->nodes[j-1].ch;
                j++;
            }
        } else {
            p->nodes[j].type = 0;
            p->nodes[j].ch = pat[i];
            j++;
        }
        i++;
    }
    p->nodes[j].type = 4; /* ACCEPT */
    p->len = j + 1;
}

int node_matches(PatNode *n, char c) {
    if (n->type == 1) return 1; /* dot matches anything */
    if (n->type == 0) return n->ch == c;
    return 0;
}

/* Simple NFA-style matching using recursive backtracking */
int match_from(Pattern *p, int pi, char *str, int si) {
    PatNode *cur;

    if (pi >= p->len) return 0;
    cur = &p->nodes[pi];

    if (cur->type == 4) {
        /* ACCEPT: pattern exhausted */
        return str[si] == 0;
    }

    /* Check if next node is STAR */
    if (pi + 1 < p->len && p->nodes[pi+1].type == 2) {
        /* Try matching zero occurrences (skip current + star) */
        if (match_from(p, pi + 2, str, si)) return 1;
        /* Try matching one or more */
        while (str[si] != 0 && node_matches(cur, str[si])) {
            si++;
            if (match_from(p, pi + 2, str, si)) return 1;
        }
        return 0;
    }

    /* Check if next node is PLUS */
    if (pi + 1 < p->len && p->nodes[pi+1].type == 3) {
        /* Must match at least one */
        while (str[si] != 0 && node_matches(cur, str[si])) {
            si++;
            if (match_from(p, pi + 2, str, si)) return 1;
        }
        return 0;
    }

    /* Regular match */
    if (str[si] == 0) return 0;
    if (node_matches(cur, str[si])) {
        return match_from(p, pi + 1, str, si + 1);
    }
    return 0;
}

int regex_match(char *pattern, char *str) {
    Pattern p;
    compile_pattern(pattern, &p);
    return match_from(&p, 0, str, 0);
}

void test_literal(void) {
    printf("=== Literal Matching ===\n");
    // EXPECT: === Literal Matching ===

    printf("abc=abc: %d\n", regex_match("abc", "abc"));
    // EXPECT: abc=abc: 1

    printf("abc=abd: %d\n", regex_match("abc", "abd"));
    // EXPECT: abc=abd: 0

    printf("abc=ab: %d\n", regex_match("abc", "ab"));
    // EXPECT: abc=ab: 0

    printf("abc=abcd: %d\n", regex_match("abc", "abcd"));
    // EXPECT: abc=abcd: 0

    printf("a=a: %d\n", regex_match("a", "a"));
    // EXPECT: a=a: 1

    printf("x=y: %d\n", regex_match("x", "y"));
    // EXPECT: x=y: 0
}

void test_dot(void) {
    printf("=== Dot Matching ===\n");
    // EXPECT: === Dot Matching ===

    printf("a.c=abc: %d\n", regex_match("a.c", "abc"));
    // EXPECT: a.c=abc: 1

    printf("a.c=axc: %d\n", regex_match("a.c", "axc"));
    // EXPECT: a.c=axc: 1

    printf("a.c=ac: %d\n", regex_match("a.c", "ac"));
    // EXPECT: a.c=ac: 0

    printf("...=abc: %d\n", regex_match("...", "abc"));
    // EXPECT: ...=abc: 1

    printf("...=ab: %d\n", regex_match("...", "ab"));
    // EXPECT: ...=ab: 0
}

void test_star(void) {
    printf("=== Star Matching ===\n");
    // EXPECT: === Star Matching ===

    printf("a*=empty: %d\n", regex_match("a*", ""));
    // EXPECT: a*=empty: 1

    printf("a*=a: %d\n", regex_match("a*", "a"));
    // EXPECT: a*=a: 1

    printf("a*=aaa: %d\n", regex_match("a*", "aaa"));
    // EXPECT: a*=aaa: 1

    printf("a*b=b: %d\n", regex_match("a*b", "b"));
    // EXPECT: a*b=b: 1

    printf("a*b=aab: %d\n", regex_match("a*b", "aab"));
    // EXPECT: a*b=aab: 1

    printf("a*b=aac: %d\n", regex_match("a*b", "aac"));
    // EXPECT: a*b=aac: 0

    printf("xa*b=xb: %d\n", regex_match("xa*b", "xb"));
    // EXPECT: xa*b=xb: 1

    printf("xa*b=xaaab: %d\n", regex_match("xa*b", "xaaab"));
    // EXPECT: xa*b=xaaab: 1
}

void test_plus(void) {
    printf("=== Plus Matching ===\n");
    // EXPECT: === Plus Matching ===

    printf("a+=empty: %d\n", regex_match("a+", ""));
    // EXPECT: a+=empty: 0

    printf("a+=a: %d\n", regex_match("a+", "a"));
    // EXPECT: a+=a: 1

    printf("a+=aaa: %d\n", regex_match("a+", "aaa"));
    // EXPECT: a+=aaa: 1

    printf("a+b=b: %d\n", regex_match("a+b", "b"));
    // EXPECT: a+b=b: 0

    printf("a+b=ab: %d\n", regex_match("a+b", "ab"));
    // EXPECT: a+b=ab: 1

    printf("a+b=aaab: %d\n", regex_match("a+b", "aaab"));
    // EXPECT: a+b=aaab: 1
}

void test_combined(void) {
    printf("=== Combined Patterns ===\n");
    // EXPECT: === Combined Patterns ===

    printf("a.*b=ab: %d\n", regex_match("a.*b", "ab"));
    // EXPECT: a.*b=ab: 1

    printf("a.*b=axxb: %d\n", regex_match("a.*b", "axxb"));
    // EXPECT: a.*b=axxb: 1

    printf("a.*b=a: %d\n", regex_match("a.*b", "a"));
    // EXPECT: a.*b=a: 0

    printf(".+=x: %d\n", regex_match(".+", "x"));
    // EXPECT: .+=x: 1

    printf(".+=xy: %d\n", regex_match(".+", "xy"));
    // EXPECT: .+=xy: 1

    printf(".+=empty: %d\n", regex_match(".+", ""));
    // EXPECT: .+=empty: 0
}

int main(void) {
    test_literal();
    test_dot();
    test_star();
    test_plus();
    test_combined();

    printf("Regex matcher done.\n");
    // EXPECT: Regex matcher done.
    return 0;
}
