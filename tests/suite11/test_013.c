int printf(const char *fmt, ...);

// Simplified Boyer-Moore string search (bad character heuristic only)

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) {
        n = n + 1;
    }
    return n;
}

// Bad character table: for ASCII subset (printable chars 32-126)
// Store last occurrence of each character in pattern
void build_bad_char(char *pattern, int m, int *bad_char) {
    int i;
    // Initialize all to -1
    for (i = 0; i < 128; i = i + 1) {
        bad_char[i] = -1;
    }
    // Fill with last occurrence
    for (i = 0; i < m; i = i + 1) {
        bad_char[(int)pattern[i]] = i;
    }
}

int bm_search(char *text, char *pattern) {
    int n = str_len(text);
    int m = str_len(pattern);
    int bad_char[128];
    int shift;
    int j;

    if (m == 0) return 0;
    if (n < m) return -1;

    build_bad_char(pattern, m, bad_char);

    shift = 0;
    while (shift <= n - m) {
        j = m - 1;

        // Match from right to left
        while (j >= 0 && pattern[j] == text[shift + j]) {
            j = j - 1;
        }

        if (j < 0) {
            // Pattern found
            return shift;
        } else {
            // Shift using bad character heuristic
            int bc = bad_char[(int)text[shift + j]];
            int move = j - bc;
            if (move < 1) move = 1;
            shift = shift + move;
        }
    }
    return -1;
}

// Count all occurrences
int bm_count(char *text, char *pattern) {
    int n = str_len(text);
    int m = str_len(pattern);
    int bad_char[128];
    int shift;
    int j;
    int count = 0;

    if (m == 0) return 0;
    if (n < m) return 0;

    build_bad_char(pattern, m, bad_char);

    shift = 0;
    while (shift <= n - m) {
        j = m - 1;

        while (j >= 0 && pattern[j] == text[shift + j]) {
            j = j - 1;
        }

        if (j < 0) {
            count = count + 1;
            shift = shift + 1;
        } else {
            int bc = bad_char[(int)text[shift + j]];
            int move = j - bc;
            if (move < 1) move = 1;
            shift = shift + move;
        }
    }
    return count;
}

void test_basic() {
    int r;

    r = bm_search("hello world", "world");
    printf("find world: %d\n", r);
    // EXPECT: find world: 6

    r = bm_search("hello world", "hello");
    printf("find hello: %d\n", r);
    // EXPECT: find hello: 0

    r = bm_search("abcdefgh", "efg");
    printf("find efg: %d\n", r);
    // EXPECT: find efg: 4

    r = bm_search("abcdefgh", "xyz");
    printf("find xyz: %d\n", r);
    // EXPECT: find xyz: -1

    r = bm_search("a", "ab");
    printf("too short: %d\n", r);
    // EXPECT: too short: -1

    r = bm_search("abc", "");
    printf("empty pat: %d\n", r);
    // EXPECT: empty pat: 0
}

void test_repeated() {
    int r;

    r = bm_search("aaaaaa", "aaa");
    printf("rep aaa: %d\n", r);
    // EXPECT: rep aaa: 0

    r = bm_search("abababab", "abab");
    printf("rep abab: %d\n", r);
    // EXPECT: rep abab: 0

    r = bm_search("xyzxyzxyz", "xyz");
    printf("rep xyz: %d\n", r);
    // EXPECT: rep xyz: 0
}

void test_count() {
    int c;

    c = bm_count("abcabcabc", "abc");
    printf("count abc: %d\n", c);
    // EXPECT: count abc: 3

    c = bm_count("aaaa", "aa");
    printf("count aa: %d\n", c);
    // EXPECT: count aa: 3

    c = bm_count("hello", "xyz");
    printf("count none: %d\n", c);
    // EXPECT: count none: 0

    c = bm_count("the cat sat on the mat", "the");
    printf("count the: %d\n", c);
    // EXPECT: count the: 2

    c = bm_count("banana", "ana");
    printf("count ana: %d\n", c);
    // EXPECT: count ana: 2
}

void test_special() {
    int r;

    r = bm_search("ABCABD", "ABD");
    printf("find ABD: %d\n", r);
    // EXPECT: find ABD: 3

    r = bm_search("this is a test string", "test");
    printf("find test: %d\n", r);
    // EXPECT: find test: 10

    r = bm_search("pattern matching", "matching");
    printf("find matching: %d\n", r);
    // EXPECT: find matching: 8

    r = bm_search("aab", "ab");
    printf("find ab: %d\n", r);
    // EXPECT: find ab: 1

    printf("BM tests complete\n");
    // EXPECT: BM tests complete
}

int main() {
    test_basic();
    test_repeated();
    test_count();
    test_special();

    return 0;
}
