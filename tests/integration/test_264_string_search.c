int printf(const char *fmt, ...);
// EXPECT: find 'o' = 4\nfind 'w' = 6\nfind 'z' = -1\nlast 'l' = 15\nlast 'h' = 12\ncount 'l' = 5\ncount 'o' = 3\ncount ' ' = 2\nfind 'world' = 6\nfind 'hello' = 0\nfind 'xyz' = -1\nfind 'lo w' = 3\nfind '' = 0\nfind '41' in pi = 2\nfind '926' in pi = 5\ncount '5' in pi = 3
// Test: string search (find char and substring)

int my_strlen(char *s) {
    int i = 0;
    while (s[i] != '\0') i = i + 1;
    return i;
}

int find_char(char *s, char c) {
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] == c) return i;
        i = i + 1;
    }
    return -1;
}

int find_last_char(char *s, char c) {
    int len = my_strlen(s);
    int i = len - 1;
    while (i >= 0) {
        if (s[i] == c) return i;
        i = i - 1;
    }
    return -1;
}

int count_char(char *s, char c) {
    int cnt = 0;
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] == c) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

int find_substr(char *haystack, char *needle) {
    int hlen = my_strlen(haystack);
    int nlen = my_strlen(needle);
    if (nlen == 0) return 0;
    if (nlen > hlen) return -1;
    int i;
    for (i = 0; i <= hlen - nlen; i++) {
        int j;
        int match = 1;
        for (j = 0; j < nlen; j++) {
            if (haystack[i + j] != needle[j]) {
                match = 0;
                break;
            }
        }
        if (match) return i;
    }
    return -1;
}

int main(void) {
    char *text = "hello world hello";

    // Find first char
    printf("find 'o' = %d\n", find_char(text, 'o'));
    printf("find 'w' = %d\n", find_char(text, 'w'));
    printf("find 'z' = %d\n", find_char(text, 'z'));

    // Find last char
    printf("last 'l' = %d\n", find_last_char(text, 'l'));
    printf("last 'h' = %d\n", find_last_char(text, 'h'));

    // Count char
    printf("count 'l' = %d\n", count_char(text, 'l'));
    printf("count 'o' = %d\n", count_char(text, 'o'));
    printf("count ' ' = %d\n", count_char(text, ' '));

    // Find substring
    printf("find 'world' = %d\n", find_substr(text, "world"));
    printf("find 'hello' = %d\n", find_substr(text, "hello"));
    printf("find 'xyz' = %d\n", find_substr(text, "xyz"));
    printf("find 'lo w' = %d\n", find_substr(text, "lo w"));
    printf("find '' = %d\n", find_substr(text, ""));

    // Search in digits
    char *digits = "31415926535";
    printf("find '41' in pi = %d\n", find_substr(digits, "41"));
    printf("find '926' in pi = %d\n", find_substr(digits, "926"));
    printf("count '5' in pi = %d\n", count_char(digits, '5'));

    return 0;
}
