int printf(const char *fmt, ...);
// EXPECT: len(hello) idx=5 ptr=5 rec=5\nlen(empty) = 0\nlen(x) = 1\nlen(abcdefghijklmnop) = 16\nalphabet len = 26\nalphabet = abcdefghijklmnopqrstuvwxyz\nlen(a) = 1\nlen(abc) = 3\nlen(hello) = 5\nlen(hi) = 2\nlongest: hello (5)\ntotal chars = 11
// Test: string length (manual strlen) with various approaches

int strlen_index(char *s) {
    int i = 0;
    while (s[i] != '\0') i = i + 1;
    return i;
}

int strlen_ptr(char *s) {
    char *p = s;
    while (*p != '\0') p = p + 1;
    return (int)(p - s);
}

int strlen_recursive(char *s) {
    if (*s == '\0') return 0;
    return 1 + strlen_recursive(s + 1);
}

int main(void) {
    // Basic length
    char *s1 = "hello";
    printf("len(hello) idx=%d ptr=%d rec=%d\n",
           strlen_index(s1), strlen_ptr(s1), strlen_recursive(s1));

    // Empty string
    char empty[1];
    empty[0] = '\0';
    printf("len(empty) = %d\n", strlen_index(empty));

    // Single char
    char one[2];
    one[0] = 'x';
    one[1] = '\0';
    printf("len(x) = %d\n", strlen_index(one));

    // Longer string
    char *long_str = "abcdefghijklmnop";
    printf("len(abcdefghijklmnop) = %d\n", strlen_index(long_str));

    // Build string and measure
    char buf[50];
    int i;
    for (i = 0; i < 26; i++) {
        buf[i] = 'a' + i;
    }
    buf[26] = '\0';
    printf("alphabet len = %d\n", strlen_index(buf));
    printf("alphabet = %s\n", buf);

    // Compare lengths
    char *words[4];
    char w0[2]; w0[0]='a'; w0[1]='\0';
    char w1[4]; w1[0]='a'; w1[1]='b'; w1[2]='c'; w1[3]='\0';
    char w2[6]; w2[0]='h'; w2[1]='e'; w2[2]='l'; w2[3]='l'; w2[4]='o'; w2[5]='\0';
    char w3[3]; w3[0]='h'; w3[1]='i'; w3[2]='\0';
    words[0] = w0; words[1] = w1; words[2] = w2; words[3] = w3;

    // Find longest
    int max_len = 0;
    int max_idx = 0;
    for (i = 0; i < 4; i++) {
        int l = strlen_index(words[i]);
        printf("len(%s) = %d\n", words[i], l);
        if (l > max_len) {
            max_len = l;
            max_idx = i;
        }
    }
    printf("longest: %s (%d)\n", words[max_idx], max_len);

    // Total characters
    int total = 0;
    for (i = 0; i < 4; i++) {
        total = total + strlen_index(words[i]);
    }
    printf("total chars = %d\n", total);

    return 0;
}
