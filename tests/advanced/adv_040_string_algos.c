int printf(const char *fmt, ...);
// EXPECT: === String Algorithm Tests ===
// EXPECT: --- String length ---
// EXPECT: len("hello"): 5
// EXPECT: len(""): 0
// EXPECT: len("a"): 1
// EXPECT: len("hello world"): 11
// EXPECT: --- Palindrome check ---
// EXPECT: "racecar": 1
// EXPECT: "hello": 0
// EXPECT: "abba": 1
// EXPECT: "a": 1
// EXPECT: "ab": 0
// EXPECT: "abcba": 1
// EXPECT: "abcca": 0
// EXPECT: --- Substring search (naive) ---
// EXPECT: "hello world" find "world": 6
// EXPECT: "hello world" find "hello": 0
// EXPECT: "hello world" find "xyz": -1
// EXPECT: "aaabaaab" find "aaab": 0
// EXPECT: "abcabcabc" find "cab": 2
// EXPECT: "aaa" find "aaaa": -1
// EXPECT: "abcd" find "d": 3
// EXPECT: --- String to integer (my_atoi) ---
// EXPECT: "123": 123
// EXPECT: "-456": -456
// EXPECT: "0": 0
// EXPECT: "2147483647": 2147483647
// EXPECT: "42abc": 42
// EXPECT: "  789": 789
// EXPECT: "+99": 99
// EXPECT: --- Integer to string (my_itoa) ---
// EXPECT: 123: "123"
// EXPECT: -456: "-456"
// EXPECT: 0: "0"
// EXPECT: 9999: "9999"
// EXPECT: -1: "-1"
// EXPECT: 100: "100"
// EXPECT: --- String reverse ---
// EXPECT: "hello" -> "olleh"
// EXPECT: "abcd" -> "dcba"
// EXPECT: "a" -> "a"
// EXPECT: "ab" -> "ba"
// EXPECT: --- String compare ---
// EXPECT: "abc" vs "abc": 0
// EXPECT: "abc" vs "abd": neg
// EXPECT: "abd" vs "abc": pos
// EXPECT: "abc" vs "ab": pos
// EXPECT: "ab" vs "abc": neg
// EXPECT: --- String copy ---
// EXPECT: copy of "testing": "testing"
// EXPECT: --- Count char occurrences ---
// EXPECT: 'l' in "hello world": 3
// EXPECT: 'o' in "hello world": 2
// EXPECT: 'z' in "hello world": 0
// EXPECT: --- Reverse words ---
// EXPECT: "hello world" -> "world hello"
// EXPECT: "one two three" -> "three two one"
// EXPECT: "single" -> "single"
// EXPECT: === String Algorithm Done ===
// --- String length ---
// len("hello"): 5
// len(""): 0
// len("a"): 1
// len("hello world"): 11
// --- Palindrome check ---
// "racecar": 1
// "hello": 0
// "abba": 1
// "a": 1
// "ab": 0
// "abcba": 1
// "abcca": 0
// --- Substring search (naive) ---
// "hello world" find "world": 6
// "hello world" find "hello": 0
// "hello world" find "xyz": -1
// "aaabaaab" find "aaab": 0
// "abcabcabc" find "cab": 2
// "aaa" find "aaaa": -1
// "abcd" find "d": 3
// --- String to integer (my_atoi) ---
// "123": 123
// "-456": -456
// "0": 0
// "2147483647": 2147483647
// "42abc": 42
// "  789": 789
// "+99": 99
// --- Integer to string (my_itoa) ---
// 123: "123"
// -456: "-456"
// 0: "0"
// 9999: "9999"
// -1: "-1"
// 100: "100"
// --- String reverse ---
// "hello" -> "olleh"
// "abcd" -> "dcba"
// "a" -> "a"
// "ab" -> "ba"
// --- String compare ---
// "abc" vs "abc": 0
// "abc" vs "abd": neg
// "abd" vs "abc": pos
// "abc" vs "ab": pos
// "ab" vs "abc": neg
// --- String copy ---
// copy of "testing": "testing"
// --- Count char occurrences ---
// 'l' in "hello world": 3
// 'o' in "hello world": 2
// 'z' in "hello world": 0
// --- Reverse words ---
// "hello world" -> "world hello"
// "one two three" -> "three two one"
// "single" -> "single"
// === String Algorithm Done ===

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int is_palindrome(char *s) {
    int len = my_strlen(s);
    int i = 0;
    int j = len - 1;
    while (i < j) {
        if (s[i] != s[j]) return 0;
        i = i + 1;
        j = j - 1;
    }
    return 1;
}

// Naive substring search: return index of first occurrence of pattern in text, or -1
int str_find(char *text, char *pattern) {
    int tlen = my_strlen(text);
    int plen = my_strlen(pattern);
    int i;
    int j;
    if (plen == 0) return 0;
    if (plen > tlen) return -1;
    for (i = 0; i <= tlen - plen; i++) {
        int match = 1;
        for (j = 0; j < plen; j++) {
            if (text[i + j] != pattern[j]) {
                match = 0;
                break;
            }
        }
        if (match) return i;
    }
    return -1;
}

int my_atoi(char *s) {
    int i = 0;
    int sign = 1;
    int result = 0;
    // Skip whitespace
    while (s[i] == ' ') i = i + 1;
    // Check sign
    if (s[i] == '-') {
        sign = -1;
        i = i + 1;
    } else if (s[i] == '+') {
        i = i + 1;
    }
    // Read digits
    while (s[i] >= '0' && s[i] <= '9') {
        result = result * 10 + (s[i] - '0');
        i = i + 1;
    }
    return sign * result;
}

void my_itoa(int num, char *buf) {
    int i = 0;
    int j;
    int neg = 0;
    char tmp;

    if (num == 0) {
        buf[0] = '0';
        buf[1] = 0;
        return;
    }
    if (num < 0) {
        neg = 1;
        num = -num;
    }
    while (num > 0) {
        buf[i] = '0' + (num % 10);
        num = num / 10;
        i = i + 1;
    }
    if (neg) {
        buf[i] = '-';
        i = i + 1;
    }
    buf[i] = 0;
    // Reverse
    j = 0;
    i = i - 1;
    while (j < i) {
        tmp = buf[j];
        buf[j] = buf[i];
        buf[i] = tmp;
        j = j + 1;
        i = i - 1;
    }
}

void str_reverse(char *s) {
    int len = my_strlen(s);
    int i = 0;
    int j = len - 1;
    char tmp;
    while (i < j) {
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i = i + 1;
        j = j - 1;
    }
}

int my_strcmp(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return a[i] - b[i];
        i = i + 1;
    }
    return a[i] - b[i];
}

void my_strcpy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int count_char(char *s, char c) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == c) count = count + 1;
        i = i + 1;
    }
    return count;
}

// Reverse a portion of a char array from index lo to hi
void reverse_range(char *s, int lo, int hi) {
    char tmp;
    while (lo < hi) {
        tmp = s[lo];
        s[lo] = s[hi];
        s[hi] = tmp;
        lo = lo + 1;
        hi = hi - 1;
    }
}

// Reverse words in a string in-place
void reverse_words(char *s) {
    int len = my_strlen(s);
    // First reverse entire string
    reverse_range(s, 0, len - 1);
    // Then reverse each word
    int start = 0;
    int i;
    for (i = 0; i <= len; i++) {
        if (s[i] == ' ' || s[i] == 0) {
            reverse_range(s, start, i - 1);
            start = i + 1;
        }
    }
}

int main() {
    printf("=== String Algorithm Tests ===\n");

    // String length
    printf("--- String length ---\n");
    printf("len(\"hello\"): %d\n", my_strlen("hello"));
    printf("len(\"\"): %d\n", my_strlen(""));
    printf("len(\"a\"): %d\n", my_strlen("a"));
    printf("len(\"hello world\"): %d\n", my_strlen("hello world"));

    // Palindrome check
    printf("--- Palindrome check ---\n");
    printf("\"racecar\": %d\n", is_palindrome("racecar"));
    printf("\"hello\": %d\n", is_palindrome("hello"));
    printf("\"abba\": %d\n", is_palindrome("abba"));
    printf("\"a\": %d\n", is_palindrome("a"));
    printf("\"ab\": %d\n", is_palindrome("ab"));
    printf("\"abcba\": %d\n", is_palindrome("abcba"));
    printf("\"abcca\": %d\n", is_palindrome("abcca"));

    // Substring search
    printf("--- Substring search (naive) ---\n");
    printf("\"hello world\" find \"world\": %d\n", str_find("hello world", "world"));
    printf("\"hello world\" find \"hello\": %d\n", str_find("hello world", "hello"));
    printf("\"hello world\" find \"xyz\": %d\n", str_find("hello world", "xyz"));
    printf("\"aaabaaab\" find \"aaab\": %d\n", str_find("aaabaaab", "aaab"));
    printf("\"abcabcabc\" find \"cab\": %d\n", str_find("abcabcabc", "cab"));
    printf("\"aaa\" find \"aaaa\": %d\n", str_find("aaa", "aaaa"));
    printf("\"abcd\" find \"d\": %d\n", str_find("abcd", "d"));

    // String to integer
    printf("--- String to integer (my_atoi) ---\n");
    printf("\"123\": %d\n", my_atoi("123"));
    printf("\"-456\": %d\n", my_atoi("-456"));
    printf("\"0\": %d\n", my_atoi("0"));
    printf("\"2147483647\": %d\n", my_atoi("2147483647"));
    printf("\"42abc\": %d\n", my_atoi("42abc"));
    printf("\"  789\": %d\n", my_atoi("  789"));
    printf("\"+99\": %d\n", my_atoi("+99"));

    // Integer to string
    printf("--- Integer to string (my_itoa) ---\n");
    char buf[32];
    my_itoa(123, buf);
    printf("123: \"%s\"\n", buf);
    my_itoa(-456, buf);
    printf("-456: \"%s\"\n", buf);
    my_itoa(0, buf);
    printf("0: \"%s\"\n", buf);
    my_itoa(9999, buf);
    printf("9999: \"%s\"\n", buf);
    my_itoa(-1, buf);
    printf("-1: \"%s\"\n", buf);
    my_itoa(100, buf);
    printf("100: \"%s\"\n", buf);

    // String reverse
    printf("--- String reverse ---\n");
    char s1[16];
    my_strcpy(s1, "hello");
    str_reverse(s1);
    printf("\"hello\" -> \"%s\"\n", s1);
    my_strcpy(s1, "abcd");
    str_reverse(s1);
    printf("\"abcd\" -> \"%s\"\n", s1);
    my_strcpy(s1, "a");
    str_reverse(s1);
    printf("\"a\" -> \"%s\"\n", s1);
    my_strcpy(s1, "ab");
    str_reverse(s1);
    printf("\"ab\" -> \"%s\"\n", s1);

    // String compare
    printf("--- String compare ---\n");
    int cmp;
    cmp = my_strcmp("abc", "abc");
    printf("\"abc\" vs \"abc\": %d\n", cmp);
    cmp = my_strcmp("abc", "abd");
    printf("\"abc\" vs \"abd\": %s\n", cmp < 0 ? "neg" : "pos");
    cmp = my_strcmp("abd", "abc");
    printf("\"abd\" vs \"abc\": %s\n", cmp > 0 ? "pos" : "neg");
    cmp = my_strcmp("abc", "ab");
    printf("\"abc\" vs \"ab\": %s\n", cmp > 0 ? "pos" : "neg");
    cmp = my_strcmp("ab", "abc");
    printf("\"ab\" vs \"abc\": %s\n", cmp < 0 ? "neg" : "pos");

    // String copy
    printf("--- String copy ---\n");
    char dst[32];
    my_strcpy(dst, "testing");
    printf("copy of \"testing\": \"%s\"\n", dst);

    // Count char occurrences
    printf("--- Count char occurrences ---\n");
    printf("'l' in \"hello world\": %d\n", count_char("hello world", 'l'));
    printf("'o' in \"hello world\": %d\n", count_char("hello world", 'o'));
    printf("'z' in \"hello world\": %d\n", count_char("hello world", 'z'));

    // Reverse words
    printf("--- Reverse words ---\n");
    char w1[32];
    my_strcpy(w1, "hello world");
    reverse_words(w1);
    printf("\"hello world\" -> \"%s\"\n", w1);

    char w2[32];
    my_strcpy(w2, "one two three");
    reverse_words(w2);
    printf("\"one two three\" -> \"%s\"\n", w2);

    char w3[32];
    my_strcpy(w3, "single");
    reverse_words(w3);
    printf("\"single\" -> \"%s\"\n", w3);

    printf("=== String Algorithm Done ===\n");
    return 0;
}
