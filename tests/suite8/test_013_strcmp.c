int printf(const char *fmt, ...);

int my_strcmp(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
        i = i + 1;
    }
    if (a[i] == 0 && b[i] == 0) return 0;
    if (a[i] == 0) return -1;
    return 1;
}

int my_strncmp(char *a, char *b, int n) {
    int i = 0;
    while (i < n && a[i] != 0 && b[i] != 0) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
        i = i + 1;
    }
    if (i == n) return 0;
    if (a[i] == 0 && b[i] == 0) return 0;
    if (a[i] == 0) return -1;
    return 1;
}

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int str_starts_with(char *s, char *prefix) {
    int i = 0;
    while (prefix[i] != 0) {
        if (s[i] == 0 || s[i] != prefix[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int str_ends_with(char *s, char *suffix) {
    int slen = my_strlen(s);
    int sufflen = my_strlen(suffix);
    if (sufflen > slen) return 0;
    int offset = slen - sufflen;
    int i = 0;
    while (suffix[i] != 0) {
        if (s[offset + i] != suffix[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int case_insensitive_cmp(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        char ca = a[i];
        char cb = b[i];
        if (ca >= 'A' && ca <= 'Z') ca = ca + 32;
        if (cb >= 'A' && cb <= 'Z') cb = cb + 32;
        if (ca < cb) return -1;
        if (ca > cb) return 1;
        i = i + 1;
    }
    if (a[i] == 0 && b[i] == 0) return 0;
    if (a[i] == 0) return -1;
    return 1;
}

void sort_strings(char *arr[], int n) {
    int i = 0;
    while (i < n - 1) {
        int j = 0;
        while (j < n - 1 - i) {
            if (my_strcmp(arr[j], arr[j + 1]) > 0) {
                char *tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

int main() {
    // EXPECT: cmp hello hello = 0
    printf("cmp hello hello = %d\n", my_strcmp("hello", "hello"));
    // EXPECT: cmp abc abd = -1
    printf("cmp abc abd = %d\n", my_strcmp("abc", "abd"));
    // EXPECT: cmp abd abc = 1
    printf("cmp abd abc = %d\n", my_strcmp("abd", "abc"));
    // EXPECT: cmp abc abcd = -1
    printf("cmp abc abcd = %d\n", my_strcmp("abc", "abcd"));
    // EXPECT: cmp abcd abc = 1
    printf("cmp abcd abc = %d\n", my_strcmp("abcd", "abc"));
    // EXPECT: cmp empty empty = 0
    printf("cmp empty empty = %d\n", my_strcmp("", ""));

    // EXPECT: ncmp hello hello 3 = 0
    printf("ncmp hello hello 3 = %d\n", my_strncmp("hello", "hello", 3));
    // EXPECT: ncmp hello help 3 = 0
    printf("ncmp hello help 3 = %d\n", my_strncmp("hello", "help", 3));
    // EXPECT: ncmp hello help 4 = -1
    printf("ncmp hello help 4 = %d\n", my_strncmp("hello", "help", 4));

    // EXPECT: starts hello hel = 1
    printf("starts hello hel = %d\n", str_starts_with("hello", "hel"));
    // EXPECT: starts hello world = 0
    printf("starts hello world = %d\n", str_starts_with("hello", "world"));
    // EXPECT: starts hello empty = 1
    printf("starts hello empty = %d\n", str_starts_with("hello", ""));

    // EXPECT: ends hello llo = 1
    printf("ends hello llo = %d\n", str_ends_with("hello", "llo"));
    // EXPECT: ends hello world = 0
    printf("ends hello world = %d\n", str_ends_with("hello", "world"));
    // EXPECT: ends hi hello = 0
    printf("ends hi hello = %d\n", str_ends_with("hi", "hello"));

    // EXPECT: icmp Hello hello = 0
    printf("icmp Hello hello = %d\n", case_insensitive_cmp("Hello", "hello"));
    // EXPECT: icmp ABC abc = 0
    printf("icmp ABC abc = %d\n", case_insensitive_cmp("ABC", "abc"));
    // EXPECT: icmp Hello world = -1
    printf("icmp Hello world = %d\n", case_insensitive_cmp("Hello", "world"));

    char *words[4];
    words[0] = "delta";
    words[1] = "alpha";
    words[2] = "charlie";
    words[3] = "bravo";
    sort_strings(words, 4);
    // EXPECT: sorted: alpha bravo charlie delta
    printf("sorted: %s %s %s %s\n", words[0], words[1], words[2], words[3]);

    return 0;
}
