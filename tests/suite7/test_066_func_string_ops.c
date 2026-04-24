int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

void my_strcpy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

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

void my_strcat(char *dst, char *src) {
    int dlen = my_strlen(dst);
    int i = 0;
    while (src[i] != 0) {
        dst[dlen + i] = src[i];
        i = i + 1;
    }
    dst[dlen + i] = 0;
}

int my_strfind(char *haystack, char needle) {
    int i = 0;
    while (haystack[i] != 0) {
        if (haystack[i] == needle) return i;
        i = i + 1;
    }
    return -1;
}

int my_count_char(char *s, char c) {
    int cnt = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == c) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

void my_to_upper(char *s) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] >= 97 && s[i] <= 122) {
            s[i] = s[i] - 32;
        }
        i = i + 1;
    }
}

void my_to_lower(char *s) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] >= 65 && s[i] <= 90) {
            s[i] = s[i] + 32;
        }
        i = i + 1;
    }
}

void my_reverse_str(char *s) {
    int len = my_strlen(s);
    int left = 0;
    int right = len - 1;
    while (left < right) {
        char tmp = s[left];
        s[left] = s[right];
        s[right] = tmp;
        left = left + 1;
        right = right - 1;
    }
}

int my_starts_with(char *s, char *prefix) {
    int i = 0;
    while (prefix[i] != 0) {
        if (s[i] == 0 || s[i] != prefix[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int my_ends_with(char *s, char *suffix) {
    int slen = my_strlen(s);
    int suflen = my_strlen(suffix);
    if (suflen > slen) return 0;
    int offset = slen - suflen;
    int i = 0;
    while (suffix[i] != 0) {
        if (s[offset + i] != suffix[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int my_is_palindrome(char *s) {
    int len = my_strlen(s);
    int left = 0;
    int right = len - 1;
    while (left < right) {
        if (s[left] != s[right]) return 0;
        left = left + 1;
        right = right - 1;
    }
    return 1;
}

void my_substr(char *dst, char *src, int start, int count) {
    int i = 0;
    while (i < count && src[start + i] != 0) {
        dst[i] = src[start + i];
        i = i + 1;
    }
    dst[i] = 0;
}

int main(void) {
    // EXPECT: len_hello: 5
    printf("len_hello: %d\n", my_strlen("hello"));

    // EXPECT: len_empty: 0
    printf("len_empty: %d\n", my_strlen(""));

    char buf[64];
    my_strcpy(buf, "world");
    // EXPECT: copy: world
    printf("copy: %s\n", buf);

    // EXPECT: cmp_eq: 0
    printf("cmp_eq: %d\n", my_strcmp("abc", "abc"));

    // EXPECT: cmp_lt: -1
    printf("cmp_lt: %d\n", my_strcmp("abc", "abd"));

    // EXPECT: cmp_gt: 1
    printf("cmp_gt: %d\n", my_strcmp("abd", "abc"));

    // EXPECT: cmp_short: -1
    printf("cmp_short: %d\n", my_strcmp("ab", "abc"));

    char cat_buf[64];
    my_strcpy(cat_buf, "hello");
    my_strcat(cat_buf, " world");
    // EXPECT: cat: hello world
    printf("cat: %s\n", cat_buf);

    // EXPECT: find_l: 2
    printf("find_l: %d\n", my_strfind("hello", 'l'));

    // EXPECT: find_z: -1
    printf("find_z: %d\n", my_strfind("hello", 'z'));

    // EXPECT: count_l: 2
    printf("count_l: %d\n", my_count_char("hello", 'l'));

    char upper_buf[64];
    my_strcpy(upper_buf, "hello");
    my_to_upper(upper_buf);
    // EXPECT: upper: HELLO
    printf("upper: %s\n", upper_buf);

    char lower_buf[64];
    my_strcpy(lower_buf, "WORLD");
    my_to_lower(lower_buf);
    // EXPECT: lower: world
    printf("lower: %s\n", lower_buf);

    char rev_buf[64];
    my_strcpy(rev_buf, "abcde");
    my_reverse_str(rev_buf);
    // EXPECT: rev: edcba
    printf("rev: %s\n", rev_buf);

    // EXPECT: starts1: 1
    printf("starts1: %d\n", my_starts_with("hello world", "hello"));

    // EXPECT: starts0: 0
    printf("starts0: %d\n", my_starts_with("hello world", "world"));

    // EXPECT: ends1: 1
    printf("ends1: %d\n", my_ends_with("hello world", "world"));

    // EXPECT: ends0: 0
    printf("ends0: %d\n", my_ends_with("hello world", "hello"));

    // EXPECT: pal1: 1
    printf("pal1: %d\n", my_is_palindrome("racecar"));

    // EXPECT: pal0: 0
    printf("pal0: %d\n", my_is_palindrome("hello"));

    char sub_buf[64];
    my_substr(sub_buf, "hello world", 6, 5);
    // EXPECT: substr: world
    printf("substr: %s\n", sub_buf);

    return 0;
}
