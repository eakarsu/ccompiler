int printf(const char *fmt, ...);
// EXPECT: Brute 'AABAACAADAABAABA' 'AABA': 3\nBrute 'hello world' 'world': 1\nBrute 'aaa' 'aa': 2\nReverse 'hello': olleh\nRotation 'abcde' 'cdeab': 1\nRotation 'abcde' 'abced': 0\nAnagram 'listen' 'silent': 1\nAnagram 'hello' 'world': 0\nAnagram 'abc' 'cba': 1\nStrcmp 'abc' 'abd': -1\nStrcmp 'abc' 'abc': 0
// Test: String matching algorithms (brute force + tricks)

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n++;
    return n;
}

int str_cmp(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return a[i] - b[i];
        i++;
    }
    return a[i] - b[i];
}

int brute_search(char *text, char *pat) {
    int n = str_len(text);
    int m = str_len(pat);
    int count = 0;
    int i, j;

    for (i = 0; i <= n - m; i++) {
        int match = 1;
        for (j = 0; j < m; j++) {
            if (text[i + j] != pat[j]) {
                match = 0;
                break;
            }
        }
        if (match) count++;
    }
    return count;
}

void str_reverse(char *s) {
    int n = str_len(s);
    int i;
    for (i = 0; i < n / 2; i++) {
        char t = s[i];
        s[i] = s[n - 1 - i];
        s[n - 1 - i] = t;
    }
}

int is_rotation(char *s1, char *s2) {
    int n1 = str_len(s1);
    int n2 = str_len(s2);
    if (n1 != n2) return 0;

    char doubled[128];
    int i;
    for (i = 0; i < n1; i++) doubled[i] = s1[i];
    for (i = 0; i < n1; i++) doubled[n1 + i] = s1[i];
    doubled[2 * n1] = 0;

    return brute_search(doubled, s2) > 0;
}

int is_anagram(char *a, char *b) {
    int count[128];
    int i;
    for (i = 0; i < 128; i++) count[i] = 0;
    for (i = 0; a[i] != 0; i++) count[(int)a[i]]++;
    for (i = 0; b[i] != 0; i++) count[(int)b[i]]--;
    for (i = 0; i < 128; i++) {
        if (count[i] != 0) return 0;
    }
    return 1;
}

int main(void) {
    printf("Brute 'AABAACAADAABAABA' 'AABA': %d\n",
        brute_search("AABAACAADAABAABA", "AABA"));
    printf("Brute 'hello world' 'world': %d\n",
        brute_search("hello world", "world"));
    printf("Brute 'aaa' 'aa': %d\n",
        brute_search("aaa", "aa"));

    char rev[] = "hello";
    str_reverse(rev);
    printf("Reverse 'hello': %s\n", rev);

    printf("Rotation 'abcde' 'cdeab': %d\n", is_rotation("abcde", "cdeab"));
    printf("Rotation 'abcde' 'abced': %d\n", is_rotation("abcde", "abced"));

    printf("Anagram 'listen' 'silent': %d\n", is_anagram("listen", "silent"));
    printf("Anagram 'hello' 'world': %d\n", is_anagram("hello", "world"));
    printf("Anagram 'abc' 'cba': %d\n", is_anagram("abc", "cba"));

    printf("Strcmp 'abc' 'abd': %d\n", str_cmp("abc", "abd") < 0 ? -1 : 1);
    printf("Strcmp 'abc' 'abc': %d\n", str_cmp("abc", "abc"));

    return 0;
}
