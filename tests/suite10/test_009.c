int printf(const char *fmt, ...);

// Language Recognizer
// Recognizes several formal languages:
// L1: a^n b^n (equal a's then b's)
// L2: palindromes over {a,b} (up to length 7)
// L3: strings with equal number of a's and b's (any order)
// L4: strings where every 'a' is immediately followed by 'b'

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len = len + 1;
    return len;
}

// L1: a^n b^n recognizer
int recognize_anbn(char *s) {
    int len = str_len(s);
    int i = 0;
    int a_count = 0;
    int b_count = 0;

    // Count a's
    while (i < len && s[i] == 'a') {
        a_count = a_count + 1;
        i = i + 1;
    }
    // Count b's
    while (i < len && s[i] == 'b') {
        b_count = b_count + 1;
        i = i + 1;
    }
    // Must have consumed entire string, counts must match, and n >= 1
    if (i != len) return 0;
    if (a_count != b_count) return 0;
    if (a_count == 0) return 0;
    return 1;
}

// L2: palindrome recognizer
int recognize_palindrome(char *s) {
    int len = str_len(s);
    int i = 0;
    int j = len - 1;
    while (i < j) {
        if (s[i] != s[j]) return 0;
        i = i + 1;
        j = j - 1;
    }
    return 1;
}

// L3: equal a's and b's (any order)
int recognize_equal_ab(char *s) {
    int len = str_len(s);
    int a_count = 0;
    int b_count = 0;
    int i;
    for (i = 0; i < len; i++) {
        if (s[i] == 'a') a_count = a_count + 1;
        else if (s[i] == 'b') b_count = b_count + 1;
        else return 0; // invalid char
    }
    return (a_count == b_count) ? 1 : 0;
}

// L4: every 'a' must be immediately followed by 'b'
int recognize_ab_pairs(char *s) {
    int len = str_len(s);
    int i = 0;
    while (i < len) {
        if (s[i] == 'a') {
            if (i + 1 >= len || s[i + 1] != 'b') return 0;
            i = i + 2; // skip the pair
        } else if (s[i] == 'b') {
            i = i + 1; // standalone b is ok
        } else {
            return 0;
        }
    }
    return 1;
}

typedef int (*Recognizer)(char *);

void test_language(char *name, Recognizer rec, char **tests, int *expected, int count) {
    int passed = 0;
    int i;
    for (i = 0; i < count; i++) {
        int result = rec(tests[i]);
        if (result == expected[i]) {
            passed = passed + 1;
        } else {
            printf("  FAIL: %s('%s')=%d expected %d\n", name, tests[i], result, expected[i]);
        }
    }
    printf("%s: %d/%d passed\n", name, passed, count);
}

int main() {
    // Test L1: a^n b^n
    char *l1_tests[8];
    int l1_exp[8];
    l1_tests[0] = "ab";      l1_exp[0] = 1;
    l1_tests[1] = "aabb";    l1_exp[1] = 1;
    l1_tests[2] = "aaabbb";  l1_exp[2] = 1;
    l1_tests[3] = "aab";     l1_exp[3] = 0;
    l1_tests[4] = "abb";     l1_exp[4] = 0;
    l1_tests[5] = "ba";      l1_exp[5] = 0;
    l1_tests[6] = "abab";    l1_exp[6] = 0;
    l1_tests[7] = "";        l1_exp[7] = 0;
    // EXPECT: L1(a^n b^n): 8/8 passed
    test_language("L1(a^n b^n)", recognize_anbn, l1_tests, l1_exp, 8);

    // Test L2: palindromes
    char *l2_tests[8];
    int l2_exp[8];
    l2_tests[0] = "aba";     l2_exp[0] = 1;
    l2_tests[1] = "abba";    l2_exp[1] = 1;
    l2_tests[2] = "a";       l2_exp[2] = 1;
    l2_tests[3] = "aa";      l2_exp[3] = 1;
    l2_tests[4] = "ab";      l2_exp[4] = 0;
    l2_tests[5] = "abc";     l2_exp[5] = 0;
    l2_tests[6] = "aabaa";   l2_exp[6] = 1;
    l2_tests[7] = "";        l2_exp[7] = 1;
    // EXPECT: L2(palindrome): 8/8 passed
    test_language("L2(palindrome)", recognize_palindrome, l2_tests, l2_exp, 8);

    // Test L3: equal a's and b's
    char *l3_tests[8];
    int l3_exp[8];
    l3_tests[0] = "ab";      l3_exp[0] = 1;
    l3_tests[1] = "ba";      l3_exp[1] = 1;
    l3_tests[2] = "aabb";    l3_exp[2] = 1;
    l3_tests[3] = "abba";    l3_exp[3] = 1;
    l3_tests[4] = "abab";    l3_exp[4] = 1;
    l3_tests[5] = "aab";     l3_exp[5] = 0;
    l3_tests[6] = "bbb";     l3_exp[6] = 0;
    l3_tests[7] = "";        l3_exp[7] = 1;
    // EXPECT: L3(equal a,b): 8/8 passed
    test_language("L3(equal a,b)", recognize_equal_ab, l3_tests, l3_exp, 8);

    // Test L4: every a followed by b
    char *l4_tests[8];
    int l4_exp[8];
    l4_tests[0] = "ab";      l4_exp[0] = 1;
    l4_tests[1] = "abab";    l4_exp[1] = 1;
    l4_tests[2] = "b";       l4_exp[2] = 1;
    l4_tests[3] = "bb";      l4_exp[3] = 1;
    l4_tests[4] = "bab";     l4_exp[4] = 1;
    l4_tests[5] = "a";       l4_exp[5] = 0;
    l4_tests[6] = "ba";      l4_exp[6] = 0;
    l4_tests[7] = "aab";     l4_exp[7] = 0;
    // EXPECT: L4(a->ab): 8/8 passed
    test_language("L4(a->ab)", recognize_ab_pairs, l4_tests, l4_exp, 8);

    // Cross-language analysis: which strings of length 4 over {a,b} are in multiple languages?
    char test_str[5];
    int multi_count = 0;
    int a; int b; int c; int d;
    char syms[2];
    syms[0] = 'a'; syms[1] = 'b';
    for (a = 0; a < 2; a++) {
        for (b = 0; b < 2; b++) {
            for (c = 0; c < 2; c++) {
                for (d = 0; d < 2; d++) {
                    test_str[0] = syms[a]; test_str[1] = syms[b];
                    test_str[2] = syms[c]; test_str[3] = syms[d];
                    test_str[4] = 0;
                    int langs = 0;
                    if (recognize_anbn(test_str)) langs = langs + 1;
                    if (recognize_palindrome(test_str)) langs = langs + 1;
                    if (recognize_equal_ab(test_str)) langs = langs + 1;
                    if (recognize_ab_pairs(test_str)) langs = langs + 1;
                    if (langs >= 2) multi_count = multi_count + 1;
                }
            }
        }
    }
    // EXPECT: 4-char strings in 2+ languages: 5
    printf("4-char strings in 2+ languages: %d\n", multi_count);

    return 0;
}
