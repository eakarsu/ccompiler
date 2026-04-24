int printf(const char *fmt, ...);

// ============================================================
// test_085_string_ops.c
// String operations using char arrays (no string.h):
//   1. String rotation check
//   2. Longest common prefix
//   3. String compression (run-length encoding on chars)
//   4. Pattern repetition check
//   5. Various string utilities
// ============================================================

// ---- String utility: length ----
int str_len(char *s) {
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

// ---- String utility: copy ----
void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

// ---- String utility: compare ----
int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i]; // Both should be '\0'
}

// ---- String utility: print as characters ----
void str_print(char *s) {
    int i = 0;
    while (s[i] != '\0') {
        printf("%c", s[i]);
        i++;
    }
}

// ---- String utility: concatenate ----
void str_concat(char *dst, char *a, char *b) {
    int i = 0;
    int j = 0;
    while (a[i] != '\0') {
        dst[i] = a[i];
        i++;
    }
    while (b[j] != '\0') {
        dst[i] = b[j];
        i++;
        j++;
    }
    dst[i] = '\0';
}

// ---- String utility: substring ----
void str_substr(char *dst, char *src, int start, int len) {
    int i;
    for (i = 0; i < len && src[start + i] != '\0'; i++) {
        dst[i] = src[start + i];
    }
    dst[i] = '\0';
}

// ---- String utility: reverse ----
void str_reverse(char *s) {
    int len = str_len(s);
    int i = 0;
    int j = len - 1;
    while (i < j) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i++;
        j--;
    }
}

// ---- String utility: char at position ----
char str_char_at(char *s, int pos) {
    return s[pos];
}

// ---- String utility: count occurrences of a character ----
int str_count_char(char *s, char c) {
    int count = 0;
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] == c) count++;
        i++;
    }
    return count;
}

// ---- String utility: find first occurrence of char ----
int str_find_char(char *s, char c) {
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] == c) return i;
        i++;
    }
    return -1;
}

// ---- String utility: check if substring exists ----
int str_contains(char *haystack, char *needle) {
    int hlen = str_len(haystack);
    int nlen = str_len(needle);
    int i;
    int j;
    if (nlen > hlen) return 0;
    for (i = 0; i <= hlen - nlen; i++) {
        int match = 1;
        for (j = 0; j < nlen; j++) {
            if (haystack[i + j] != needle[j]) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    return 0;
}

// ============================================================
// 1. String Rotation Check
// ============================================================
// Check if s2 is a rotation of s1
// e.g., "abcde" and "cdeab" -> rotation by 2
int is_rotation(char *s1, char *s2) {
    int len1 = str_len(s1);
    int len2 = str_len(s2);
    if (len1 != len2) return 0;
    if (len1 == 0) return 1;

    // Concatenate s1 with itself and check if s2 is a substring
    char doubled[200];
    str_concat(doubled, s1, s1);
    return str_contains(doubled, s2);
}

// Find rotation amount: how many positions s2 is rotated from s1
int rotation_amount(char *s1, char *s2) {
    int len = str_len(s1);
    if (len == 0) return 0;
    if (!is_rotation(s1, s2)) return -1;

    int i;
    for (i = 0; i < len; i++) {
        // Check if rotating s1 by i positions gives s2
        int match = 1;
        int j;
        for (j = 0; j < len; j++) {
            if (s1[(j + i) % len] != s2[j]) {
                match = 0;
                break;
            }
        }
        if (match) return i;
    }
    return -1;
}

// Rotate a string left by k positions
void str_rotate_left(char *s, int k) {
    int len = str_len(s);
    if (len == 0) return;
    k = k % len;
    if (k == 0) return;

    // Use reversal algorithm
    // Reverse first k chars, reverse rest, reverse whole
    char tmp[200];
    str_copy(tmp, s);

    int i;
    // Manual rotation using temp
    for (i = 0; i < len; i++) {
        s[i] = tmp[(i + k) % len];
    }
}

// ============================================================
// 2. Longest Common Prefix
// ============================================================
// Find LCP of two strings
int longest_common_prefix_two(char *a, char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0' && a[i] == b[i]) {
        i++;
    }
    return i;
}

// Find LCP of an array of strings (stored as 2D char array)
int longest_common_prefix(char strings[10][50], int count) {
    if (count == 0) return 0;
    if (count == 1) return str_len(strings[0]);

    int lcp = str_len(strings[0]); // Start with length of first string
    int i;
    for (i = 1; i < count; i++) {
        int current_lcp = longest_common_prefix_two(strings[0], strings[i]);
        if (current_lcp < lcp) {
            lcp = current_lcp;
        }
    }
    return lcp;
}

// ============================================================
// 3. String Compression (Run-Length Encoding)
// ============================================================
// Compress a string using RLE
// e.g., "aaabbbcc" -> output: a3b3c2 (printed as chars and counts)
void rle_compress(char *input, char *out_chars, int *out_counts, int *out_len) {
    int in_len = str_len(input);
    if (in_len == 0) {
        *out_len = 0;
        return;
    }

    int idx = 0;
    int i = 0;
    while (i < in_len) {
        char c = input[i];
        int count = 0;
        while (i < in_len && input[i] == c) {
            count++;
            i++;
        }
        out_chars[idx] = c;
        out_counts[idx] = count;
        idx++;
    }
    *out_len = idx;
}

// Decompress RLE back to string
void rle_decompress(char *out_chars, int *out_counts, int out_len, char *result) {
    int pos = 0;
    int i;
    int j;
    for (i = 0; i < out_len; i++) {
        for (j = 0; j < out_counts[i]; j++) {
            result[pos] = out_chars[i];
            pos++;
        }
    }
    result[pos] = '\0';
}

// Check if compression is beneficial (compressed shorter than original)
int rle_is_beneficial(char *input) {
    int in_len = str_len(input);
    char chars[100];
    int counts[100];
    int rle_len;
    rle_compress(input, chars, counts, &rle_len);
    // Each RLE entry takes 2 units (char + digit), but multi-digit counts take more
    int compressed_len = 0;
    int i;
    for (i = 0; i < rle_len; i++) {
        compressed_len++; // the char
        // Count digits in count
        int c = counts[i];
        if (c == 1) {
            compressed_len++; // '1'
        } else {
            while (c > 0) {
                compressed_len++;
                c = c / 10;
            }
        }
    }
    return compressed_len < in_len;
}

// ============================================================
// 4. Pattern Repetition Check
// ============================================================
// Check if a string is made by repeating a pattern
// e.g., "abcabcabc" is "abc" repeated 3 times
int is_repeated_pattern(char *s) {
    int len = str_len(s);
    if (len <= 1) return 0;

    int pat_len;
    for (pat_len = 1; pat_len <= len / 2; pat_len++) {
        if (len % pat_len != 0) continue;

        int reps = len / pat_len;
        int valid = 1;
        int i;
        for (i = 0; i < len; i++) {
            if (s[i] != s[i % pat_len]) {
                valid = 0;
                break;
            }
        }
        if (valid) return pat_len; // Return length of smallest repeating unit
    }
    return 0; // Not a repeated pattern
}

// Find number of repetitions if pattern exists
int count_repetitions(char *s) {
    int pat_len = is_repeated_pattern(s);
    if (pat_len == 0) return 1;
    return str_len(s) / pat_len;
}

// ============================================================
// 5. Additional string operations
// ============================================================

// Check if string is palindrome
int is_palindrome(char *s) {
    int len = str_len(s);
    int i = 0;
    int j = len - 1;
    while (i < j) {
        if (s[i] != s[j]) return 0;
        i++;
        j--;
    }
    return 1;
}

// Count distinct characters
int count_distinct_chars(char *s) {
    int seen[128];
    int i;
    for (i = 0; i < 128; i++) seen[i] = 0;
    i = 0;
    while (s[i] != '\0') {
        seen[(int)s[i]] = 1;
        i++;
    }
    int count = 0;
    for (i = 0; i < 128; i++) {
        if (seen[i]) count++;
    }
    return count;
}

// Find the most frequent character (returns ASCII value)
int most_frequent_char(char *s) {
    int freq[128];
    int i;
    for (i = 0; i < 128; i++) freq[i] = 0;
    i = 0;
    while (s[i] != '\0') {
        freq[(int)s[i]]++;
        i++;
    }
    int max_freq = 0;
    int max_char = 0;
    for (i = 0; i < 128; i++) {
        if (freq[i] > max_freq) {
            max_freq = freq[i];
            max_char = i;
        }
    }
    return max_char;
}

// Check if two strings are anagrams
int is_anagram(char *a, char *b) {
    int freq_a[128];
    int freq_b[128];
    int i;
    for (i = 0; i < 128; i++) {
        freq_a[i] = 0;
        freq_b[i] = 0;
    }
    i = 0;
    while (a[i] != '\0') { freq_a[(int)a[i]]++; i++; }
    i = 0;
    while (b[i] != '\0') { freq_b[(int)b[i]]++; i++; }

    for (i = 0; i < 128; i++) {
        if (freq_a[i] != freq_b[i]) return 0;
    }
    return 1;
}

// ============================================================
// Test functions
// ============================================================

void test_rotation() {
    printf("=== Rotation Tests ===\n");

    char s1[50];
    char s2[50];

    // Test 1: "abcde" rotated to "cdeab"
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]='d'; s1[4]='e'; s1[5]='\0';
    s2[0]='c'; s2[1]='d'; s2[2]='e'; s2[3]='a'; s2[4]='b'; s2[5]='\0';
    printf("Rot1 is_rot: %d\n", is_rotation(s1, s2));
    printf("Rot1 amount: %d\n", rotation_amount(s1, s2));

    // Test 2: "abcde" and "abced" -> not rotation
    s2[0]='a'; s2[1]='b'; s2[2]='c'; s2[3]='e'; s2[4]='d'; s2[5]='\0';
    printf("Rot2 is_rot: %d\n", is_rotation(s1, s2));

    // Test 3: "aaa" and "aaa" -> rotation by 0
    s1[0]='a'; s1[1]='a'; s1[2]='a'; s1[3]='\0';
    s2[0]='a'; s2[1]='a'; s2[2]='a'; s2[3]='\0';
    printf("Rot3 is_rot: %d\n", is_rotation(s1, s2));
    printf("Rot3 amount: %d\n", rotation_amount(s1, s2));

    // Test 4: Rotate "hello" left by 2
    char hello[50];
    hello[0]='h'; hello[1]='e'; hello[2]='l'; hello[3]='l'; hello[4]='o'; hello[5]='\0';
    str_rotate_left(hello, 2);
    printf("Rot4 result: ");
    str_print(hello);
    printf("\n");

    // Test 5: Rotate "abcdef" left by 4
    char abcdef[50];
    abcdef[0]='a'; abcdef[1]='b'; abcdef[2]='c';
    abcdef[3]='d'; abcdef[4]='e'; abcdef[5]='f'; abcdef[6]='\0';
    str_rotate_left(abcdef, 4);
    printf("Rot5 result: ");
    str_print(abcdef);
    printf("\n");

    // Test 6: different lengths
    s1[0]='a'; s1[1]='b'; s1[2]='\0';
    s2[0]='a'; s2[1]='b'; s2[2]='c'; s2[3]='\0';
    printf("Rot6 diff_len: %d\n", is_rotation(s1, s2));

    // Test 7: empty strings
    s1[0]='\0';
    s2[0]='\0';
    printf("Rot7 empty: %d\n", is_rotation(s1, s2));
}

void test_lcp() {
    printf("=== LCP Tests ===\n");

    char strings[10][50];

    // Test 1: Common prefix "fl"
    strings[0][0]='f'; strings[0][1]='l'; strings[0][2]='o'; strings[0][3]='w';
    strings[0][4]='e'; strings[0][5]='r'; strings[0][6]='\0';
    strings[1][0]='f'; strings[1][1]='l'; strings[1][2]='o'; strings[1][3]='w';
    strings[1][4]='\0';
    strings[2][0]='f'; strings[2][1]='l'; strings[2][2]='i'; strings[2][3]='g';
    strings[2][4]='h'; strings[2][5]='t'; strings[2][6]='\0';
    printf("LCP1: %d\n", longest_common_prefix(strings, 3));

    // Test 2: No common prefix
    strings[0][0]='d'; strings[0][1]='o'; strings[0][2]='g'; strings[0][3]='\0';
    strings[1][0]='c'; strings[1][1]='a'; strings[1][2]='t'; strings[1][3]='\0';
    strings[2][0]='b'; strings[2][1]='i'; strings[2][2]='r'; strings[2][3]='d';
    strings[2][4]='\0';
    printf("LCP2: %d\n", longest_common_prefix(strings, 3));

    // Test 3: All same
    strings[0][0]='a'; strings[0][1]='b'; strings[0][2]='c'; strings[0][3]='\0';
    strings[1][0]='a'; strings[1][1]='b'; strings[1][2]='c'; strings[1][3]='\0';
    printf("LCP3: %d\n", longest_common_prefix(strings, 2));

    // Test 4: Two strings comparison
    char a1[50];
    char b1[50];
    a1[0]='i'; a1[1]='n'; a1[2]='t'; a1[3]='e'; a1[4]='r'; a1[5]='\0';
    b1[0]='i'; b1[1]='n'; b1[2]='t'; b1[3]='r'; b1[4]='o'; b1[5]='\0';
    printf("LCP4: %d\n", longest_common_prefix_two(a1, b1));

    // Test 5: Single string
    strings[0][0]='h'; strings[0][1]='e'; strings[0][2]='l'; strings[0][3]='l';
    strings[0][4]='o'; strings[0][5]='\0';
    printf("LCP5: %d\n", longest_common_prefix(strings, 1));
}

void test_rle() {
    printf("=== RLE Tests ===\n");

    char input[100];
    char rle_chars[100];
    int rle_counts[100];
    int rle_len;
    char decompressed[100];

    // Test 1: "aaabbbcc"
    input[0]='a'; input[1]='a'; input[2]='a'; input[3]='b'; input[4]='b';
    input[5]='b'; input[6]='c'; input[7]='c'; input[8]='\0';
    rle_compress(input, rle_chars, rle_counts, &rle_len);
    printf("RLE1 groups: %d\n", rle_len);
    int i;
    for (i = 0; i < rle_len; i++) {
        printf("RLE1 %c: %d\n", rle_chars[i], rle_counts[i]);
    }
    rle_decompress(rle_chars, rle_counts, rle_len, decompressed);
    printf("RLE1 roundtrip: %d\n", str_equal(input, decompressed));
    printf("RLE1 beneficial: %d\n", rle_is_beneficial(input));

    // Test 2: "aaaaaa"
    input[0]='a'; input[1]='a'; input[2]='a'; input[3]='a'; input[4]='a';
    input[5]='a'; input[6]='\0';
    rle_compress(input, rle_chars, rle_counts, &rle_len);
    printf("RLE2 groups: %d\n", rle_len);
    printf("RLE2 char: %c count: %d\n", rle_chars[0], rle_counts[0]);
    printf("RLE2 beneficial: %d\n", rle_is_beneficial(input));

    // Test 3: "abcdef" (no repetitions)
    input[0]='a'; input[1]='b'; input[2]='c'; input[3]='d'; input[4]='e';
    input[5]='f'; input[6]='\0';
    rle_compress(input, rle_chars, rle_counts, &rle_len);
    printf("RLE3 groups: %d\n", rle_len);
    printf("RLE3 beneficial: %d\n", rle_is_beneficial(input));
    rle_decompress(rle_chars, rle_counts, rle_len, decompressed);
    printf("RLE3 roundtrip: %d\n", str_equal(input, decompressed));

    // Test 4: "aabbccddee"
    input[0]='a'; input[1]='a'; input[2]='b'; input[3]='b'; input[4]='c';
    input[5]='c'; input[6]='d'; input[7]='d'; input[8]='e'; input[9]='e';
    input[10]='\0';
    rle_compress(input, rle_chars, rle_counts, &rle_len);
    printf("RLE4 groups: %d\n", rle_len);
    rle_decompress(rle_chars, rle_counts, rle_len, decompressed);
    printf("RLE4 roundtrip: %d\n", str_equal(input, decompressed));

    // Test 5: Single character
    input[0]='x'; input[1]='\0';
    rle_compress(input, rle_chars, rle_counts, &rle_len);
    printf("RLE5 groups: %d\n", rle_len);
    printf("RLE5 char: %c count: %d\n", rle_chars[0], rle_counts[0]);
}

void test_pattern_repetition() {
    printf("=== Pattern Repetition Tests ===\n");

    char s[100];

    // Test 1: "abcabcabc" -> pattern "abc" repeated 3 times
    s[0]='a'; s[1]='b'; s[2]='c'; s[3]='a'; s[4]='b';
    s[5]='c'; s[6]='a'; s[7]='b'; s[8]='c'; s[9]='\0';
    printf("Pat1 unit_len: %d\n", is_repeated_pattern(s));
    printf("Pat1 reps: %d\n", count_repetitions(s));

    // Test 2: "aaaa" -> pattern "a" repeated 4 times
    s[0]='a'; s[1]='a'; s[2]='a'; s[3]='a'; s[4]='\0';
    printf("Pat2 unit_len: %d\n", is_repeated_pattern(s));
    printf("Pat2 reps: %d\n", count_repetitions(s));

    // Test 3: "abab" -> pattern "ab" repeated 2 times
    s[0]='a'; s[1]='b'; s[2]='a'; s[3]='b'; s[4]='\0';
    printf("Pat3 unit_len: %d\n", is_repeated_pattern(s));
    printf("Pat3 reps: %d\n", count_repetitions(s));

    // Test 4: "abcde" -> no repetition
    s[0]='a'; s[1]='b'; s[2]='c'; s[3]='d'; s[4]='e'; s[5]='\0';
    printf("Pat4 unit_len: %d\n", is_repeated_pattern(s));
    printf("Pat4 reps: %d\n", count_repetitions(s));

    // Test 5: "abcabd" -> no repetition
    s[0]='a'; s[1]='b'; s[2]='c'; s[3]='a'; s[4]='b'; s[5]='d'; s[6]='\0';
    printf("Pat5 unit_len: %d\n", is_repeated_pattern(s));
    printf("Pat5 reps: %d\n", count_repetitions(s));

    // Test 6: "xyzxyz" -> pattern "xyz" repeated 2 times
    s[0]='x'; s[1]='y'; s[2]='z'; s[3]='x'; s[4]='y'; s[5]='z'; s[6]='\0';
    printf("Pat6 unit_len: %d\n", is_repeated_pattern(s));
    printf("Pat6 reps: %d\n", count_repetitions(s));
}

void test_string_utils() {
    printf("=== String Utility Tests ===\n");

    char s[100];

    // Palindrome tests
    s[0]='r'; s[1]='a'; s[2]='c'; s[3]='e'; s[4]='c';
    s[5]='a'; s[6]='r'; s[7]='\0';
    printf("Palindrome1: %d\n", is_palindrome(s));

    s[0]='h'; s[1]='e'; s[2]='l'; s[3]='l'; s[4]='o'; s[5]='\0';
    printf("Palindrome2: %d\n", is_palindrome(s));

    s[0]='a'; s[1]='b'; s[2]='b'; s[3]='a'; s[4]='\0';
    printf("Palindrome3: %d\n", is_palindrome(s));

    // Distinct chars
    s[0]='h'; s[1]='e'; s[2]='l'; s[3]='l'; s[4]='o'; s[5]='\0';
    printf("Distinct1: %d\n", count_distinct_chars(s));

    s[0]='a'; s[1]='a'; s[2]='a'; s[3]='\0';
    printf("Distinct2: %d\n", count_distinct_chars(s));

    s[0]='a'; s[1]='b'; s[2]='c'; s[3]='d'; s[4]='\0';
    printf("Distinct3: %d\n", count_distinct_chars(s));

    // Most frequent char
    s[0]='a'; s[1]='b'; s[2]='a'; s[3]='c'; s[4]='a'; s[5]='\0';
    int mf = most_frequent_char(s);
    printf("MostFreq1: %c\n", (char)mf);

    // Anagram tests
    char a[50];
    char b[50];
    a[0]='l'; a[1]='i'; a[2]='s'; a[3]='t'; a[4]='e'; a[5]='n'; a[6]='\0';
    b[0]='s'; b[1]='i'; b[2]='l'; b[3]='e'; b[4]='n'; b[5]='t'; b[6]='\0';
    printf("Anagram1: %d\n", is_anagram(a, b));

    a[0]='h'; a[1]='e'; a[2]='l'; a[3]='l'; a[4]='o'; a[5]='\0';
    b[0]='w'; b[1]='o'; b[2]='r'; b[3]='l'; b[4]='d'; b[5]='\0';
    printf("Anagram2: %d\n", is_anagram(a, b));

    // Contains
    char hay[50];
    char needle[50];
    hay[0]='h'; hay[1]='e'; hay[2]='l'; hay[3]='l'; hay[4]='o'; hay[5]='\0';
    needle[0]='l'; needle[1]='l'; needle[2]='\0';
    printf("Contains1: %d\n", str_contains(hay, needle));

    needle[0]='x'; needle[1]='y'; needle[2]='\0';
    printf("Contains2: %d\n", str_contains(hay, needle));

    // Reverse
    char rev[50];
    rev[0]='a'; rev[1]='b'; rev[2]='c'; rev[3]='d'; rev[4]='e'; rev[5]='\0';
    str_reverse(rev);
    printf("Reverse: ");
    str_print(rev);
    printf("\n");

    // Count char
    s[0]='m'; s[1]='i'; s[2]='s'; s[3]='s'; s[4]='i'; s[5]='s'; s[6]='s';
    s[7]='i'; s[8]='p'; s[9]='p'; s[10]='i'; s[11]='\0';
    printf("Count_s: %d\n", str_count_char(s, 's'));
    printf("Count_i: %d\n", str_count_char(s, 'i'));
    printf("Count_p: %d\n", str_count_char(s, 'p'));

    // Find char
    printf("Find_s: %d\n", str_find_char(s, 's'));
    printf("Find_p: %d\n", str_find_char(s, 'p'));
    printf("Find_z: %d\n", str_find_char(s, 'z'));
}

// EXPECT: === Rotation Tests ===
// EXPECT: Rot1 is_rot: 1
// EXPECT: Rot1 amount: 2
// EXPECT: Rot2 is_rot: 0
// EXPECT: Rot3 is_rot: 1
// EXPECT: Rot3 amount: 0
// EXPECT: Rot4 result: llohe
// EXPECT: Rot5 result: efabcd
// EXPECT: Rot6 diff_len: 0
// EXPECT: Rot7 empty: 1
// EXPECT: === LCP Tests ===
// EXPECT: LCP1: 2
// EXPECT: LCP2: 0
// EXPECT: LCP3: 3
// EXPECT: LCP4: 3
// EXPECT: LCP5: 5
// EXPECT: === RLE Tests ===
// EXPECT: RLE1 groups: 3
// EXPECT: RLE1 a: 3
// EXPECT: RLE1 b: 3
// EXPECT: RLE1 c: 2
// EXPECT: RLE1 roundtrip: 1
// EXPECT: RLE1 beneficial: 1
// EXPECT: RLE2 groups: 1
// EXPECT: RLE2 char: a count: 6
// EXPECT: RLE2 beneficial: 1
// EXPECT: RLE3 groups: 6
// EXPECT: RLE3 beneficial: 0
// EXPECT: RLE3 roundtrip: 1
// EXPECT: RLE4 groups: 5
// EXPECT: RLE4 roundtrip: 1
// EXPECT: RLE5 groups: 1
// EXPECT: RLE5 char: x count: 1
// EXPECT: === Pattern Repetition Tests ===
// EXPECT: Pat1 unit_len: 3
// EXPECT: Pat1 reps: 3
// EXPECT: Pat2 unit_len: 1
// EXPECT: Pat2 reps: 4
// EXPECT: Pat3 unit_len: 2
// EXPECT: Pat3 reps: 2
// EXPECT: Pat4 unit_len: 0
// EXPECT: Pat4 reps: 1
// EXPECT: Pat5 unit_len: 0
// EXPECT: Pat5 reps: 1
// EXPECT: Pat6 unit_len: 3
// EXPECT: Pat6 reps: 2
// EXPECT: === String Utility Tests ===
// EXPECT: Palindrome1: 1
// EXPECT: Palindrome2: 0
// EXPECT: Palindrome3: 1
// EXPECT: Distinct1: 4
// EXPECT: Distinct2: 1
// EXPECT: Distinct3: 4
// EXPECT: MostFreq1: a
// EXPECT: Anagram1: 1
// EXPECT: Anagram2: 0
// EXPECT: Contains1: 1
// EXPECT: Contains2: 0
// EXPECT: Reverse: edcba
// EXPECT: Count_s: 4
// EXPECT: Count_i: 4
// EXPECT: Count_p: 2
// EXPECT: Find_s: 2
// EXPECT: Find_p: 8
// EXPECT: Find_z: -1

int main() {
    test_rotation();
    test_lcp();
    test_rle();
    test_pattern_repetition();
    test_string_utils();
    return 0;
}
