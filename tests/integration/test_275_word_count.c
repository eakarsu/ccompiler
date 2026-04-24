int printf(const char *fmt, ...);
// EXPECT: 'hello world': 2 words\n'  leading spaces  ': 2 words\n'one': 1 words\nempty: 0 words\nspaces: 0 words\n'the quick brown fox jumps': 5 words, 21 chars\n  word 1: 'the' (len=3)\n  word 2: 'quick' (len=5)\n  word 3: 'brown' (len=5)\n  word 4: 'fox' (len=3)\n  word 5: 'jumps' (len=5)\nlongest word: 'quick' (5)\navg word len: 4
// Test: word counting in strings

int is_space(char c) {
    return c == ' ' || c == '\t';
}

int count_words(char *s) {
    int count = 0;
    int in_word = 0;
    int i = 0;
    while (s[i] != '\0') {
        if (is_space(s[i])) {
            in_word = 0;
        } else {
            if (!in_word) {
                count = count + 1;
                in_word = 1;
            }
        }
        i = i + 1;
    }
    return count;
}

int count_chars(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != '\0') {
        if (!is_space(s[i])) count = count + 1;
        i = i + 1;
    }
    return count;
}

// Extract nth word into buffer, return length
int get_word(char *s, int n, char *buf) {
    int count = 0;
    int in_word = 0;
    int i = 0;
    int wi = 0;
    while (s[i] != '\0') {
        if (is_space(s[i])) {
            if (in_word && count == n) {
                buf[wi] = '\0';
                return wi;
            }
            in_word = 0;
        } else {
            if (!in_word) {
                count = count + 1;
                in_word = 1;
                wi = 0;
            }
            if (count == n) {
                buf[wi] = s[i];
                wi = wi + 1;
            }
        }
        i = i + 1;
    }
    if (in_word && count == n) {
        buf[wi] = '\0';
        return wi;
    }
    buf[0] = '\0';
    return 0;
}

int my_strlen(char *s) {
    int i = 0;
    while (s[i] != '\0') i = i + 1;
    return i;
}

int main(void) {
    char *s1 = "hello world";
    printf("'%s': %d words\n", s1, count_words(s1));

    char *s2 = "  leading spaces  ";
    printf("'%s': %d words\n", s2, count_words(s2));

    char *s3 = "one";
    printf("'%s': %d words\n", s3, count_words(s3));

    char *s4 = "";
    printf("empty: %d words\n", count_words(s4));

    char *s5 = "   ";
    printf("spaces: %d words\n", count_words(s5));

    char *s6 = "the quick brown fox jumps";
    printf("'%s': %d words, %d chars\n", s6, count_words(s6), count_chars(s6));

    // Extract each word
    char buf[20];
    int nw = count_words(s6);
    int i;
    for (i = 1; i <= nw; i++) {
        int wlen = get_word(s6, i, buf);
        printf("  word %d: '%s' (len=%d)\n", i, buf, wlen);
    }

    // Find longest word
    int max_len = 0;
    char longest[20];
    longest[0] = '\0';
    for (i = 1; i <= nw; i++) {
        get_word(s6, i, buf);
        int l = my_strlen(buf);
        if (l > max_len) {
            max_len = l;
            int j;
            for (j = 0; j <= l; j++) longest[j] = buf[j];
        }
    }
    printf("longest word: '%s' (%d)\n", longest, max_len);

    // Average word length
    int total_chars = count_chars(s6);
    printf("avg word len: %d\n", total_chars / nw);

    return 0;
}
