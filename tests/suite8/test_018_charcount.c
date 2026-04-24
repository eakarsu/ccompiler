int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
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

int count_digits(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] >= '0' && s[i] <= '9') count = count + 1;
        i = i + 1;
    }
    return count;
}

int count_upper(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] >= 'A' && s[i] <= 'Z') count = count + 1;
        i = i + 1;
    }
    return count;
}

int count_lower(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] >= 'a' && s[i] <= 'z') count = count + 1;
        i = i + 1;
    }
    return count;
}

char most_frequent(char *s) {
    int freq[128];
    int i = 0;
    while (i < 128) {
        freq[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (s[i] != 0) {
        int idx = s[i];
        freq[idx] = freq[idx] + 1;
        i = i + 1;
    }
    int best = 0;
    char bestc = s[0];
    i = 0;
    while (i < 128) {
        if (freq[i] > best) {
            best = freq[i];
            bestc = (char)i;
        }
        i = i + 1;
    }
    return bestc;
}

int count_unique(char *s) {
    int seen[128];
    int i = 0;
    while (i < 128) {
        seen[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (s[i] != 0) {
        int idx = s[i];
        seen[idx] = 1;
        i = i + 1;
    }
    int count = 0;
    i = 0;
    while (i < 128) {
        if (seen[i]) count = count + 1;
        i = i + 1;
    }
    return count;
}

int first_non_repeating(char *s) {
    int freq[128];
    int i = 0;
    while (i < 128) {
        freq[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (s[i] != 0) {
        int idx = s[i];
        freq[idx] = freq[idx] + 1;
        i = i + 1;
    }
    i = 0;
    while (s[i] != 0) {
        int idx = s[i];
        if (freq[idx] == 1) return i;
        i = i + 1;
    }
    return -1;
}

int is_anagram(char *a, char *b) {
    int freq[128];
    int i = 0;
    while (i < 128) {
        freq[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (a[i] != 0) {
        int idx = a[i];
        freq[idx] = freq[idx] + 1;
        i = i + 1;
    }
    i = 0;
    while (b[i] != 0) {
        int idx = b[i];
        freq[idx] = freq[idx] - 1;
        i = i + 1;
    }
    i = 0;
    while (i < 128) {
        if (freq[i] != 0) return 0;
        i = i + 1;
    }
    return 1;
}

int main() {
    // EXPECT: count l in hello = 2
    printf("count l in hello = %d\n", count_char("hello", 'l'));
    // EXPECT: count o in hello world = 2
    printf("count o in hello world = %d\n", count_char("hello world", 'o'));
    // EXPECT: count z in hello = 0
    printf("count z in hello = %d\n", count_char("hello", 'z'));

    // EXPECT: digits in abc123def = 3
    printf("digits in abc123def = %d\n", count_digits("abc123def"));
    // EXPECT: digits in hello = 0
    printf("digits in hello = %d\n", count_digits("hello"));
    // EXPECT: digits in 12345 = 5
    printf("digits in 12345 = %d\n", count_digits("12345"));

    // EXPECT: upper in Hello World = 2
    printf("upper in Hello World = %d\n", count_upper("Hello World"));
    // EXPECT: lower in Hello World = 8
    printf("lower in Hello World = %d\n", count_lower("Hello World"));

    char mf = most_frequent("aabbbcc");
    // EXPECT: most freq aabbbcc = 98
    printf("most freq aabbbcc = %d\n", (int)mf);

    char mf2 = most_frequent("hello");
    // EXPECT: most freq hello = 108
    printf("most freq hello = %d\n", (int)mf2);

    // EXPECT: unique in hello = 4
    printf("unique in hello = %d\n", count_unique("hello"));
    // EXPECT: unique in aaa = 1
    printf("unique in aaa = %d\n", count_unique("aaa"));
    // EXPECT: unique in abcdef = 6
    printf("unique in abcdef = %d\n", count_unique("abcdef"));

    // EXPECT: first non-repeat in aabcc = 2
    printf("first non-repeat in aabcc = %d\n", first_non_repeating("aabcc"));
    // EXPECT: first non-repeat in abab = -1
    printf("first non-repeat in abab = %d\n", first_non_repeating("abab"));
    // EXPECT: first non-repeat in abcab = 2
    printf("first non-repeat in abcab = %d\n", first_non_repeating("abcab"));

    // EXPECT: anagram listen silent = 1
    printf("anagram listen silent = %d\n", is_anagram("listen", "silent"));
    // EXPECT: anagram hello world = 0
    printf("anagram hello world = %d\n", is_anagram("hello", "world"));
    // EXPECT: anagram abc cba = 1
    printf("anagram abc cba = %d\n", is_anagram("abc", "cba"));

    return 0;
}
