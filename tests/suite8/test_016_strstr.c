int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int my_strstr(char *haystack, char *needle) {
    int hlen = my_strlen(haystack);
    int nlen = my_strlen(needle);
    if (nlen == 0) return 0;
    if (nlen > hlen) return -1;
    int i = 0;
    while (i <= hlen - nlen) {
        int j = 0;
        int match = 1;
        while (j < nlen) {
            if (haystack[i + j] != needle[j]) {
                match = 0;
                break;
            }
            j = j + 1;
        }
        if (match) return i;
        i = i + 1;
    }
    return -1;
}

int count_occurrences(char *haystack, char *needle) {
    int count = 0;
    int hlen = my_strlen(haystack);
    int nlen = my_strlen(needle);
    if (nlen == 0) return 0;
    int i = 0;
    while (i <= hlen - nlen) {
        int j = 0;
        int match = 1;
        while (j < nlen) {
            if (haystack[i + j] != needle[j]) {
                match = 0;
                break;
            }
            j = j + 1;
        }
        if (match) {
            count = count + 1;
            i = i + nlen;
        } else {
            i = i + 1;
        }
    }
    return count;
}

int find_last(char *haystack, char *needle) {
    int hlen = my_strlen(haystack);
    int nlen = my_strlen(needle);
    if (nlen == 0) return hlen;
    if (nlen > hlen) return -1;
    int i = hlen - nlen;
    while (i >= 0) {
        int j = 0;
        int match = 1;
        while (j < nlen) {
            if (haystack[i + j] != needle[j]) {
                match = 0;
                break;
            }
            j = j + 1;
        }
        if (match) return i;
        i = i - 1;
    }
    return -1;
}

int find_char(char *s, char c) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == c) return i;
        i = i + 1;
    }
    return -1;
}

int find_any_of(char *s, char *chars) {
    int i = 0;
    while (s[i] != 0) {
        int j = 0;
        while (chars[j] != 0) {
            if (s[i] == chars[j]) return i;
            j = j + 1;
        }
        i = i + 1;
    }
    return -1;
}

void find_all_positions(char *haystack, char c, int *positions, int *count) {
    *count = 0;
    int i = 0;
    while (haystack[i] != 0) {
        if (haystack[i] == c) {
            positions[*count] = i;
            *count = *count + 1;
        }
        i = i + 1;
    }
}

int contains_all(char *s, char *chars) {
    int i = 0;
    while (chars[i] != 0) {
        if (find_char(s, chars[i]) == -1) return 0;
        i = i + 1;
    }
    return 1;
}

int main() {
    // EXPECT: find world in hello world = 6
    printf("find world in hello world = %d\n", my_strstr("hello world", "world"));
    // EXPECT: find hello in hello world = 0
    printf("find hello in hello world = %d\n", my_strstr("hello world", "hello"));
    // EXPECT: find xyz in hello world = -1
    printf("find xyz in hello world = %d\n", my_strstr("hello world", "xyz"));
    // EXPECT: find empty in hello = 0
    printf("find empty in hello = %d\n", my_strstr("hello", ""));
    // EXPECT: find lo in hello = 3
    printf("find lo in hello = %d\n", my_strstr("hello", "lo"));

    // EXPECT: count ab in ababab = 3
    printf("count ab in ababab = %d\n", count_occurrences("ababab", "ab"));
    // EXPECT: count aa in aaaa = 2
    printf("count aa in aaaa = %d\n", count_occurrences("aaaa", "aa"));
    // EXPECT: count x in hello = 0
    printf("count x in hello = %d\n", count_occurrences("hello", "x"));
    // EXPECT: count the in the cat and the dog = 2
    printf("count the in the cat and the dog = %d\n", count_occurrences("the cat and the dog", "the"));

    // EXPECT: last ab in ababab = 4
    printf("last ab in ababab = %d\n", find_last("ababab", "ab"));
    // EXPECT: last o in hello world = 7
    printf("last o in hello world = %d\n", find_last("hello world", "o"));
    // EXPECT: last xyz in hello = -1
    printf("last xyz in hello = %d\n", find_last("hello", "xyz"));

    // EXPECT: find_char l in hello = 2
    printf("find_char l in hello = %d\n", find_char("hello", 'l'));
    // EXPECT: find_char z in hello = -1
    printf("find_char z in hello = %d\n", find_char("hello", 'z'));

    // EXPECT: find_any aeiou in hello = 1
    printf("find_any aeiou in hello = %d\n", find_any_of("hello", "aeiou"));
    // EXPECT: find_any xyz in hello = -1
    printf("find_any xyz in hello = %d\n", find_any_of("hello", "xyz"));

    int positions[32];
    int count;
    find_all_positions("hello world", 'l', positions, &count);
    // EXPECT: l positions count = 3
    printf("l positions count = %d\n", count);
    // EXPECT: l pos 0 = 2
    printf("l pos 0 = %d\n", positions[0]);
    // EXPECT: l pos 1 = 3
    printf("l pos 1 = %d\n", positions[1]);
    // EXPECT: l pos 2 = 9
    printf("l pos 2 = %d\n", positions[2]);

    // EXPECT: contains helo in hello = 1
    printf("contains helo in hello = %d\n", contains_all("hello", "helo"));
    // EXPECT: contains xyz in hello = 0
    printf("contains xyz in hello = %d\n", contains_all("hello", "xyz"));

    return 0;
}
