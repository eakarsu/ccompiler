int printf(const char *fmt, ...);
// EXPECT: 1: 0 1 2 -1\n2: 0 1 2 -1\n3: 4 4 4 4\n4: 4 4 3\n5: 5 5 5:0 1 2 3 4 \n6: 0 0 1 2 \n6b: 0 1 2 3 \n6c: 0 0 0 0 \n7: 5 0 2 4 6 8 \n8: 4 0 2 4 6 \n9: 12 10 14 0\n10: 1 7 1 6 1 6\n11: 0 1 2 -1\n12: 4 1\n13: -1 -1 0 0\n14: 8 7 10\n15: 4 4 3 4 \n16: 0 3 2 5\n17: 0 1 0 1 2 3 4 5 2 \n18: -1 -1 0 0\n19: 0 1 1\n20: 4 4 4 12
// ============================================================================
// String Matching - KMP-like Pattern Matching on Char Arrays
// ============================================================================
// This file implements pattern matching algorithms on character arrays:
// 1. Brute force pattern matching
// 2. KMP (Knuth-Morris-Pratt) pattern matching
// 3. Occurrence counting
// 4. Multiple pattern matching
// All strings are represented as char arrays with null terminators.
// ============================================================================

#define MAX_TEXT 200
#define MAX_PATTERN 50
#define MAX_FAIL 50

// ============================================================================
// Utility: compute string length
// ============================================================================
int str_len(char *s) {
    int len;
    len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

// ============================================================================
// Utility: copy string from src to dst
// ============================================================================
void str_copy(char *dst, char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

// ============================================================================
// Utility: compare two strings, return 1 if equal, 0 otherwise
// ============================================================================
int str_equal(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    if (a[i] != b[i]) return 0;
    return 1;
}

// ============================================================================
// Utility: set a string from individual characters
// ============================================================================
void set_str(char *s, int c0, int c1, int c2, int c3, int c4,
             int c5, int c6, int c7, int c8, int c9) {
    s[0] = c0; s[1] = c1; s[2] = c2; s[3] = c3; s[4] = c4;
    s[5] = c5; s[6] = c6; s[7] = c7; s[8] = c8; s[9] = c9;
}

// ============================================================================
// Brute force pattern matching
// Returns the index of the first occurrence, or -1 if not found
// ============================================================================
int brute_force_search(char *text, char *pattern) {
    int n;
    int m;
    int i;
    int j;
    int found;

    n = str_len(text);
    m = str_len(pattern);

    if (m == 0) return 0;
    if (m > n) return -1;

    for (i = 0; i <= n - m; i = i + 1) {
        found = 1;
        for (j = 0; j < m; j = j + 1) {
            if (text[i + j] != pattern[j]) {
                found = 0;
                j = m; // break
            }
        }
        if (found) return i;
    }
    return -1;
}

// ============================================================================
// Brute force: count all occurrences of pattern in text
// ============================================================================
int brute_force_count(char *text, char *pattern) {
    int n;
    int m;
    int i;
    int j;
    int count;
    int found;

    n = str_len(text);
    m = str_len(pattern);
    count = 0;

    if (m == 0) return 0;
    if (m > n) return 0;

    for (i = 0; i <= n - m; i = i + 1) {
        found = 1;
        for (j = 0; j < m; j = j + 1) {
            if (text[i + j] != pattern[j]) {
                found = 0;
                j = m; // break
            }
        }
        if (found) {
            count = count + 1;
        }
    }
    return count;
}

// ============================================================================
// Compute KMP failure function (partial match table)
// fail[i] = length of longest proper prefix of pattern[0..i] that is also a suffix
// ============================================================================
int fail_table[MAX_FAIL];

void compute_failure(char *pattern, int m) {
    int i;
    int j;

    fail_table[0] = 0;
    j = 0;
    i = 1;

    while (i < m) {
        if (pattern[i] == pattern[j]) {
            j = j + 1;
            fail_table[i] = j;
            i = i + 1;
        } else {
            if (j != 0) {
                j = fail_table[j - 1];
            } else {
                fail_table[i] = 0;
                i = i + 1;
            }
        }
    }
}

// ============================================================================
// KMP pattern matching: find first occurrence
// Returns index of first match, or -1 if not found
// ============================================================================
int kmp_search(char *text, char *pattern) {
    int n;
    int m;
    int i;
    int j;

    n = str_len(text);
    m = str_len(pattern);

    if (m == 0) return 0;
    if (m > n) return -1;

    compute_failure(pattern, m);

    i = 0;
    j = 0;
    while (i < n) {
        if (text[i] == pattern[j]) {
            i = i + 1;
            j = j + 1;
            if (j == m) {
                return i - m;
            }
        } else {
            if (j != 0) {
                j = fail_table[j - 1];
            } else {
                i = i + 1;
            }
        }
    }
    return -1;
}

// ============================================================================
// KMP: count all occurrences of pattern in text
// ============================================================================
int kmp_count(char *text, char *pattern) {
    int n;
    int m;
    int i;
    int j;
    int count;

    n = str_len(text);
    m = str_len(pattern);
    count = 0;

    if (m == 0) return 0;
    if (m > n) return 0;

    compute_failure(pattern, m);

    i = 0;
    j = 0;
    while (i < n) {
        if (text[i] == pattern[j]) {
            i = i + 1;
            j = j + 1;
            if (j == m) {
                count = count + 1;
                j = fail_table[j - 1];
            }
        } else {
            if (j != 0) {
                j = fail_table[j - 1];
            } else {
                i = i + 1;
            }
        }
    }
    return count;
}

// ============================================================================
// Find all occurrence positions using KMP, store in positions array
// Returns the number of occurrences found
// ============================================================================
int positions[MAX_TEXT];

int kmp_find_all(char *text, char *pattern) {
    int n;
    int m;
    int i;
    int j;
    int count;

    n = str_len(text);
    m = str_len(pattern);
    count = 0;

    if (m == 0) return 0;
    if (m > n) return 0;

    compute_failure(pattern, m);

    i = 0;
    j = 0;
    while (i < n) {
        if (text[i] == pattern[j]) {
            i = i + 1;
            j = j + 1;
            if (j == m) {
                positions[count] = i - m;
                count = count + 1;
                j = fail_table[j - 1];
            }
        } else {
            if (j != 0) {
                j = fail_table[j - 1];
            } else {
                i = i + 1;
            }
        }
    }
    return count;
}

// ============================================================================
// Check if pattern is a substring starting at position pos
// ============================================================================
int check_at_position(char *text, char *pattern, int pos) {
    int m;
    int i;
    m = str_len(pattern);
    for (i = 0; i < m; i = i + 1) {
        if (text[pos + i] != pattern[i]) return 0;
    }
    return 1;
}

// ============================================================================
// Build a text string character by character
// ============================================================================
void build_text_abcabc(char *text) {
    // "abcabcabcabc"
    int i;
    for (i = 0; i < 4; i = i + 1) {
        text[i * 3] = 'a';
        text[i * 3 + 1] = 'b';
        text[i * 3 + 2] = 'c';
    }
    text[12] = 0;
}

// ============================================================================
// Build text: "aaaaaa"
// ============================================================================
void build_text_aaaa(char *text, int count) {
    int i;
    for (i = 0; i < count; i = i + 1) {
        text[i] = 'a';
    }
    text[count] = 0;
}

// ============================================================================
// Build text: "ababababab"
// ============================================================================
void build_text_abab(char *text, int pairs) {
    int i;
    for (i = 0; i < pairs; i = i + 1) {
        text[i * 2] = 'a';
        text[i * 2 + 1] = 'b';
    }
    text[pairs * 2] = 0;
}

// ============================================================================
// Build pattern from individual characters (short patterns)
// ============================================================================
void build_pattern(char *pat, int c0, int c1, int c2, int c3) {
    pat[0] = c0;
    if (c1 == 0) { pat[1] = 0; return; }
    pat[1] = c1;
    if (c2 == 0) { pat[2] = 0; return; }
    pat[2] = c2;
    if (c3 == 0) { pat[3] = 0; return; }
    pat[3] = c3;
    pat[4] = 0;
}

// ============================================================================
// Test Case 1: Basic brute force search
// ============================================================================
void test_case_1(void) {
    char text[MAX_TEXT];
    char pattern[MAX_PATTERN];
    int result;

    printf("%d", 1);
    printf("%c", ':');
    printf("%c", ' ');

    build_text_abcabc(text);

    // Search for "abc"
    build_pattern(pattern, 'a', 'b', 'c', 0);
    result = brute_force_search(text, pattern);
    printf("%d", result);
    printf("%c", ' ');

    // Search for "bca"
    build_pattern(pattern, 'b', 'c', 'a', 0);
    result = brute_force_search(text, pattern);
    printf("%d", result);
    printf("%c", ' ');

    // Search for "cab"
    build_pattern(pattern, 'c', 'a', 'b', 0);
    result = brute_force_search(text, pattern);
    printf("%d", result);
    printf("%c", ' ');

    // Search for "xyz" (not found)
    build_pattern(pattern, 'x', 'y', 'z', 0);
    result = brute_force_search(text, pattern);
    printf("%d", result);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 2: KMP search on same text
// ============================================================================
void test_case_2(void) {
    char text[MAX_TEXT];
    char pattern[MAX_PATTERN];
    int result;

    printf("%d", 2);
    printf("%c", ':');
    printf("%c", ' ');

    build_text_abcabc(text);

    build_pattern(pattern, 'a', 'b', 'c', 0);
    result = kmp_search(text, pattern);
    printf("%d", result);
    printf("%c", ' ');

    build_pattern(pattern, 'b', 'c', 'a', 0);
    result = kmp_search(text, pattern);
    printf("%d", result);
    printf("%c", ' ');

    build_pattern(pattern, 'c', 'a', 'b', 0);
    result = kmp_search(text, pattern);
    printf("%d", result);
    printf("%c", ' ');

    build_pattern(pattern, 'x', 'y', 'z', 0);
    result = kmp_search(text, pattern);
    printf("%d", result);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 3: Occurrence counting with brute force
// ============================================================================
void test_case_3(void) {
    char text[MAX_TEXT];
    char pattern[MAX_PATTERN];

    printf("%d", 3);
    printf("%c", ':');
    printf("%c", ' ');

    build_text_abcabc(text);

    build_pattern(pattern, 'a', 'b', 'c', 0);
    printf("%d", brute_force_count(text, pattern));
    printf("%c", ' ');

    build_pattern(pattern, 'a', 0, 0, 0);
    printf("%d", brute_force_count(text, pattern));
    printf("%c", ' ');

    build_pattern(pattern, 'b', 0, 0, 0);
    printf("%d", brute_force_count(text, pattern));
    printf("%c", ' ');

    build_pattern(pattern, 'c', 0, 0, 0);
    printf("%d", brute_force_count(text, pattern));
    printf("%c", '\n');
}

// ============================================================================
// Test Case 4: KMP occurrence counting
// ============================================================================
void test_case_4(void) {
    char text[MAX_TEXT];
    char pattern[MAX_PATTERN];

    printf("%d", 4);
    printf("%c", ':');
    printf("%c", ' ');

    build_text_abcabc(text);

    build_pattern(pattern, 'a', 'b', 'c', 0);
    printf("%d", kmp_count(text, pattern));
    printf("%c", ' ');

    build_pattern(pattern, 'a', 0, 0, 0);
    printf("%d", kmp_count(text, pattern));
    printf("%c", ' ');

    build_pattern(pattern, 'c', 'a', 0, 0);
    printf("%d", kmp_count(text, pattern));
    printf("%c", '\n');
}

// ============================================================================
// Test Case 5: Overlapping pattern matching with "aa" in "aaaa"
// ============================================================================
void test_case_5(void) {
    char text[MAX_TEXT];
    char pattern[MAX_PATTERN];
    int count;
    int pos_count;
    int i;

    printf("%d", 5);
    printf("%c", ':');
    printf("%c", ' ');

    build_text_aaaa(text, 6);
    build_pattern(pattern, 'a', 'a', 0, 0);

    count = kmp_count(text, pattern);
    printf("%d", count);
    printf("%c", ' ');

    count = brute_force_count(text, pattern);
    printf("%d", count);
    printf("%c", ' ');

    // Find all positions
    pos_count = kmp_find_all(text, pattern);
    printf("%d", pos_count);
    printf("%c", ':');
    for (i = 0; i < pos_count; i = i + 1) {
        printf("%d", positions[i]);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 6: Failure function values
// ============================================================================
void test_case_6(void) {
    char pattern[MAX_PATTERN];
    int m;
    int i;

    printf("%d", 6);
    printf("%c", ':');
    printf("%c", ' ');

    // Pattern "abab"
    build_pattern(pattern, 'a', 'b', 'a', 'b');
    m = str_len(pattern);
    compute_failure(pattern, m);
    for (i = 0; i < m; i = i + 1) {
        printf("%d", fail_table[i]);
        printf("%c", ' ');
    }
    printf("%c", '\n');

    // Pattern "aaaa"
    printf("%d", 6);
    printf("%c", 'b');
    printf("%c", ':');
    printf("%c", ' ');
    build_pattern(pattern, 'a', 'a', 'a', 'a');
    m = str_len(pattern);
    compute_failure(pattern, m);
    for (i = 0; i < m; i = i + 1) {
        printf("%d", fail_table[i]);
        printf("%c", ' ');
    }
    printf("%c", '\n');

    // Pattern "abcd"
    printf("%d", 6);
    printf("%c", 'c');
    printf("%c", ':');
    printf("%c", ' ');
    build_pattern(pattern, 'a', 'b', 'c', 'd');
    m = str_len(pattern);
    compute_failure(pattern, m);
    for (i = 0; i < m; i = i + 1) {
        printf("%d", fail_table[i]);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 7: Pattern "ab" in "ababababab"
// ============================================================================
void test_case_7(void) {
    char text[MAX_TEXT];
    char pattern[MAX_PATTERN];
    int count;
    int pos_count;
    int i;

    printf("%d", 7);
    printf("%c", ':');
    printf("%c", ' ');

    build_text_abab(text, 5);  // "ababababab"
    build_pattern(pattern, 'a', 'b', 0, 0);

    count = kmp_count(text, pattern);
    printf("%d", count);
    printf("%c", ' ');

    pos_count = kmp_find_all(text, pattern);
    for (i = 0; i < pos_count; i = i + 1) {
        printf("%d", positions[i]);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 8: Pattern "aba" in "ababababab" (overlapping)
// ============================================================================
void test_case_8(void) {
    char text[MAX_TEXT];
    char pattern[MAX_PATTERN];
    int count;
    int pos_count;
    int i;

    printf("%d", 8);
    printf("%c", ':');
    printf("%c", ' ');

    build_text_abab(text, 5);  // "ababababab"
    build_pattern(pattern, 'a', 'b', 'a', 0);

    count = kmp_count(text, pattern);
    printf("%d", count);
    printf("%c", ' ');

    pos_count = kmp_find_all(text, pattern);
    for (i = 0; i < pos_count; i = i + 1) {
        printf("%d", positions[i]);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 9: String length tests
// ============================================================================
void test_case_9(void) {
    char text[MAX_TEXT];

    printf("%d", 9);
    printf("%c", ':');
    printf("%c", ' ');

    build_text_abcabc(text);
    printf("%d", str_len(text));
    printf("%c", ' ');

    build_text_aaaa(text, 10);
    printf("%d", str_len(text));
    printf("%c", ' ');

    build_text_abab(text, 7);
    printf("%d", str_len(text));
    printf("%c", ' ');

    text[0] = 0;
    printf("%d", str_len(text));
    printf("%c", '\n');
}

// ============================================================================
// Test Case 10: Brute force vs KMP consistency check
// ============================================================================
void test_case_10(void) {
    char text[MAX_TEXT];
    char pattern[MAX_PATTERN];
    int bf;
    int kmp;
    int match;

    printf("%d", 10);
    printf("%c", ':');
    printf("%c", ' ');

    // Build a longer text
    // "abcabcabcabcabcabcabc" (7 repetitions = 21 chars)
    int i;
    for (i = 0; i < 7; i = i + 1) {
        text[i * 3] = 'a';
        text[i * 3 + 1] = 'b';
        text[i * 3 + 2] = 'c';
    }
    text[21] = 0;

    // Test multiple patterns
    build_pattern(pattern, 'a', 'b', 'c', 0);
    bf = brute_force_count(text, pattern);
    kmp = kmp_count(text, pattern);
    match = (bf == kmp) ? 1 : 0;
    printf("%d", match);
    printf("%c", ' ');
    printf("%d", bf);
    printf("%c", ' ');

    build_pattern(pattern, 'c', 'a', 'b', 0);
    bf = brute_force_count(text, pattern);
    kmp = kmp_count(text, pattern);
    match = (bf == kmp) ? 1 : 0;
    printf("%d", match);
    printf("%c", ' ');
    printf("%d", bf);
    printf("%c", ' ');

    build_pattern(pattern, 'b', 'c', 'a', 0);
    bf = brute_force_count(text, pattern);
    kmp = kmp_count(text, pattern);
    match = (bf == kmp) ? 1 : 0;
    printf("%d", match);
    printf("%c", ' ');
    printf("%d", bf);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 11: Empty and single character patterns
// ============================================================================
void test_case_11(void) {
    char text[MAX_TEXT];
    char pattern[MAX_PATTERN];

    printf("%d", 11);
    printf("%c", ':');
    printf("%c", ' ');

    build_text_abcabc(text);

    // Single character patterns
    build_pattern(pattern, 'a', 0, 0, 0);
    printf("%d", kmp_search(text, pattern));
    printf("%c", ' ');

    build_pattern(pattern, 'b', 0, 0, 0);
    printf("%d", kmp_search(text, pattern));
    printf("%c", ' ');

    build_pattern(pattern, 'c', 0, 0, 0);
    printf("%d", kmp_search(text, pattern));
    printf("%c", ' ');

    build_pattern(pattern, 'd', 0, 0, 0);
    printf("%d", kmp_search(text, pattern));
    printf("%c", '\n');
}

// ============================================================================
// Test Case 12: Verify positions of all occurrences
// ============================================================================
void test_case_12(void) {
    char text[MAX_TEXT];
    char pattern[MAX_PATTERN];
    int count;
    int i;
    int all_valid;

    printf("%d", 12);
    printf("%c", ':');
    printf("%c", ' ');

    build_text_abcabc(text); // "abcabcabcabc"
    build_pattern(pattern, 'a', 'b', 'c', 0);

    count = kmp_find_all(text, pattern);
    printf("%d", count);
    printf("%c", ' ');

    // Verify each position
    all_valid = 1;
    for (i = 0; i < count; i = i + 1) {
        if (check_at_position(text, pattern, positions[i]) == 0) {
            all_valid = 0;
        }
    }
    printf("%d", all_valid);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 13: Pattern longer than text
// ============================================================================
void test_case_13(void) {
    char text[MAX_TEXT];
    char pattern[MAX_PATTERN];

    printf("%d", 13);
    printf("%c", ':');
    printf("%c", ' ');

    build_pattern(text, 'a', 'b', 0, 0);  // text = "ab"
    build_pattern(pattern, 'a', 'b', 'c', 'd');  // pattern = "abcd"

    printf("%d", brute_force_search(text, pattern));
    printf("%c", ' ');
    printf("%d", kmp_search(text, pattern));
    printf("%c", ' ');
    printf("%d", brute_force_count(text, pattern));
    printf("%c", ' ');
    printf("%d", kmp_count(text, pattern));
    printf("%c", '\n');
}

// ============================================================================
// Test Case 14: Pattern matching with repeated characters
// ============================================================================
void test_case_14(void) {
    char text[MAX_TEXT];
    char pattern[MAX_PATTERN];
    int count;

    printf("%d", 14);
    printf("%c", ':');
    printf("%c", ' ');

    build_text_aaaa(text, 10); // "aaaaaaaaaa"

    build_pattern(pattern, 'a', 'a', 'a', 0);
    count = kmp_count(text, pattern);
    printf("%d", count);
    printf("%c", ' ');

    build_pattern(pattern, 'a', 'a', 'a', 'a');
    count = kmp_count(text, pattern);
    printf("%d", count);
    printf("%c", ' ');

    build_pattern(pattern, 'a', 0, 0, 0);
    count = kmp_count(text, pattern);
    printf("%d", count);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 15: Multi-pattern searching
// ============================================================================
void test_case_15(void) {
    char text[MAX_TEXT];
    char patterns[4][MAX_PATTERN];
    int counts[4];
    int i;

    printf("%d", 15);
    printf("%c", ':');
    printf("%c", ' ');

    // Text: "abcabcabcabc"
    build_text_abcabc(text);

    // Pattern 0: "ab"
    build_pattern(patterns[0], 'a', 'b', 0, 0);
    // Pattern 1: "bc"
    build_pattern(patterns[1], 'b', 'c', 0, 0);
    // Pattern 2: "ca"
    build_pattern(patterns[2], 'c', 'a', 0, 0);
    // Pattern 3: "abc"
    build_pattern(patterns[3], 'a', 'b', 'c', 0);

    for (i = 0; i < 4; i = i + 1) {
        counts[i] = kmp_count(text, patterns[i]);
        printf("%d", counts[i]);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 16: First occurrence positions for multiple patterns
// ============================================================================
void test_case_16(void) {
    char text[MAX_TEXT];
    char pattern[MAX_PATTERN];
    int i;

    printf("%d", 16);
    printf("%c", ':');
    printf("%c", ' ');

    // Build a specific text: "xyzabcxyz"
    text[0] = 'x'; text[1] = 'y'; text[2] = 'z';
    text[3] = 'a'; text[4] = 'b'; text[5] = 'c';
    text[6] = 'x'; text[7] = 'y'; text[8] = 'z';
    text[9] = 0;

    build_pattern(pattern, 'x', 'y', 'z', 0);
    printf("%d", kmp_search(text, pattern));
    printf("%c", ' ');

    build_pattern(pattern, 'a', 'b', 'c', 0);
    printf("%d", kmp_search(text, pattern));
    printf("%c", ' ');

    build_pattern(pattern, 'z', 'a', 0, 0);
    printf("%d", kmp_search(text, pattern));
    printf("%c", ' ');

    build_pattern(pattern, 'c', 'x', 0, 0);
    printf("%d", kmp_search(text, pattern));
    printf("%c", '\n');
}

// ============================================================================
// Test Case 17: Longer failure function test
// ============================================================================
void test_case_17(void) {
    char pattern[MAX_PATTERN];
    int m;
    int i;

    printf("%d", 17);
    printf("%c", ':');
    printf("%c", ' ');

    // Pattern "aabaabaaa"
    pattern[0] = 'a'; pattern[1] = 'a'; pattern[2] = 'b';
    pattern[3] = 'a'; pattern[4] = 'a'; pattern[5] = 'b';
    pattern[6] = 'a'; pattern[7] = 'a'; pattern[8] = 'a';
    pattern[9] = 0;

    m = str_len(pattern);
    compute_failure(pattern, m);
    for (i = 0; i < m; i = i + 1) {
        printf("%d", fail_table[i]);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 18: Search in text with no matches at all
// ============================================================================
void test_case_18(void) {
    char text[MAX_TEXT];
    char pattern[MAX_PATTERN];

    printf("%d", 18);
    printf("%c", ':');
    printf("%c", ' ');

    build_text_aaaa(text, 8);
    build_pattern(pattern, 'b', 0, 0, 0);
    printf("%d", kmp_search(text, pattern));
    printf("%c", ' ');
    printf("%d", brute_force_search(text, pattern));
    printf("%c", ' ');
    printf("%d", kmp_count(text, pattern));
    printf("%c", ' ');
    printf("%d", brute_force_count(text, pattern));
    printf("%c", '\n');
}

// ============================================================================
// Test Case 19: Exact full string match
// ============================================================================
void test_case_19(void) {
    char text[MAX_TEXT];
    char pattern[MAX_PATTERN];

    printf("%d", 19);
    printf("%c", ':');
    printf("%c", ' ');

    build_pattern(text, 'a', 'b', 'c', 'd');
    build_pattern(pattern, 'a', 'b', 'c', 'd');

    printf("%d", kmp_search(text, pattern));
    printf("%c", ' ');
    printf("%d", kmp_count(text, pattern));
    printf("%c", ' ');
    printf("%d", str_equal(text, pattern));
    printf("%c", '\n');
}

// ============================================================================
// Test Case 20: Character frequency counting (simulated using matching)
// ============================================================================
void test_case_20(void) {
    char text[MAX_TEXT];
    char pattern[MAX_PATTERN];
    int freq_a;
    int freq_b;
    int freq_c;

    printf("%d", 20);
    printf("%c", ':');
    printf("%c", ' ');

    // "abcabcabcabc" has 4 a's, 4 b's, 4 c's
    build_text_abcabc(text);

    build_pattern(pattern, 'a', 0, 0, 0);
    freq_a = kmp_count(text, pattern);

    build_pattern(pattern, 'b', 0, 0, 0);
    freq_b = kmp_count(text, pattern);

    build_pattern(pattern, 'c', 0, 0, 0);
    freq_c = kmp_count(text, pattern);

    printf("%d", freq_a);
    printf("%c", ' ');
    printf("%d", freq_b);
    printf("%c", ' ');
    printf("%d", freq_c);
    printf("%c", ' ');
    printf("%d", freq_a + freq_b + freq_c);
    printf("%c", '\n');
}

// ============================================================================
// Main function
// ============================================================================

// 2: 0 1 2 -1
// 3: 4 4 4 4
// 4: 4 4 3
// 5: 5 5 5:0 1 2 3 4
// 6: 0 0 1 2
// 6b: 0 1 2 3
// 6c: 0 0 0 0
// 7: 5 0 2 4 6 8
// 8: 4 0 2 4 6
// 9: 12 10 14 0
// 10: 1 7 1 6 1 6
// 11: 0 1 2 -1
// 12: 4 1
// 13: -1 -1 0 0
// 14: 8 7 10
// 15: 4 4 3 4
// 16: 0 3 2 5
// 17: 0 1 0 1 2 3 4 5 2
// 18: -1 -1 0 0
// 19: 0 1 1
// 20: 4 4 4 12

int main(void) {
    test_case_1();
    test_case_2();
    test_case_3();
    test_case_4();
    test_case_5();
    test_case_6();
    test_case_7();
    test_case_8();
    test_case_9();
    test_case_10();
    test_case_11();
    test_case_12();
    test_case_13();
    test_case_14();
    test_case_15();
    test_case_16();
    test_case_17();
    test_case_18();
    test_case_19();
    test_case_20();
    return 0;
}
