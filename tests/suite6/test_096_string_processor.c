int printf(const char *fmt, ...);

int str_len(char *s) {
    int i;
    i = 0;
    while (s[i] != 0) i = i + 1;
    return i;
}

void str_reverse(char *s, char *out) {
    int len;
    int i;
    len = str_len(s);
    i = 0;
    while (i < len) {
        out[i] = s[len - 1 - i];
        i = i + 1;
    }
    out[len] = 0;
}

int str_count_char(char *s, char c) {
    int i;
    int count;
    count = 0;
    i = 0;
    while (s[i] != 0) {
        if (s[i] == c) count = count + 1;
        i = i + 1;
    }
    return count;
}

void str_replace_char(char *s, char *out, char old, char new_c) {
    int i;
    i = 0;
    while (s[i] != 0) {
        if (s[i] == old) out[i] = new_c;
        else out[i] = s[i];
        i = i + 1;
    }
    out[i] = 0;
}

int str_compare(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
        i = i + 1;
    }
    if (a[i] == 0 && b[i] == 0) return 0;
    if (a[i] == 0) return -1;
    return 1;
}

int str_eq(char *a, char *b) {
    return str_compare(a, b) == 0;
}

void str_copy(char *dst, char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

/* Extract substring from index start, length len */
void str_substr(char *s, int start, int len, char *out) {
    int i;
    int slen;
    slen = str_len(s);
    i = 0;
    while (i < len && start + i < slen) {
        out[i] = s[start + i];
        i = i + 1;
    }
    out[i] = 0;
}

int str_find(char *s, char *needle) {
    int i;
    int j;
    int match;
    i = 0;
    while (s[i] != 0) {
        j = 0;
        match = 1;
        while (needle[j] != 0) {
            if (s[i + j] == 0 || s[i + j] != needle[j]) {
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

int str_count_words(char *s) {
    int i;
    int count;
    int in_word;
    count = 0;
    in_word = 0;
    i = 0;
    while (s[i] != 0) {
        if (s[i] == ' ') {
            in_word = 0;
        } else {
            if (in_word == 0) {
                count = count + 1;
                in_word = 1;
            }
        }
        i = i + 1;
    }
    return count;
}

int is_palindrome(char *s) {
    int len;
    int i;
    len = str_len(s);
    i = 0;
    while (i < len / 2) {
        if (s[i] != s[len - 1 - i]) return 0;
        i = i + 1;
    }
    return 1;
}

void str_concat(char *a, char *b, char *out) {
    int i;
    int j;
    i = 0;
    while (a[i] != 0) {
        out[i] = a[i];
        i = i + 1;
    }
    j = 0;
    while (b[j] != 0) {
        out[i] = b[j];
        i = i + 1;
        j = j + 1;
    }
    out[i] = 0;
}

int str_starts_with(char *s, char *prefix) {
    int i;
    i = 0;
    while (prefix[i] != 0) {
        if (s[i] == 0 || s[i] != prefix[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int str_ends_with(char *s, char *suffix) {
    int slen;
    int suflen;
    int i;
    slen = str_len(s);
    suflen = str_len(suffix);
    if (suflen > slen) return 0;
    i = 0;
    while (i < suflen) {
        if (s[slen - suflen + i] != suffix[i]) return 0;
        i = i + 1;
    }
    return 1;
}

void str_to_upper(char *s, char *out) {
    int i;
    i = 0;
    while (s[i] != 0) {
        if (s[i] >= 97 && s[i] <= 122) {
            out[i] = s[i] - 32;
        } else {
            out[i] = s[i];
        }
        i = i + 1;
    }
    out[i] = 0;
}

int main(void) {
    char buf[64];
    char buf2[64];
    int result;

    /* Length */
    // EXPECT: 13
    printf("%d\n", str_len("Hello, World!"));

    /* Reverse */
    str_reverse("abcdef", buf);
    // EXPECT: fedcba
    printf("%s\n", buf);

    /* Count character */
    // EXPECT: 3
    printf("%d\n", str_count_char("banana", 'a'));

    // EXPECT: 2
    printf("%d\n", str_count_char("banana", 'n'));

    /* Replace character */
    str_replace_char("hello world", buf, 'o', '0');
    // EXPECT: hell0 w0rld
    printf("%s\n", buf);

    /* Compare */
    // EXPECT: 0
    printf("%d\n", str_compare("abc", "abc"));
    // EXPECT: -1
    printf("%d\n", str_compare("abc", "abd"));
    // EXPECT: 1
    printf("%d\n", str_compare("abd", "abc"));

    /* Substring */
    str_substr("Hello, World!", 7, 5, buf);
    // EXPECT: World
    printf("%s\n", buf);

    /* Find */
    // EXPECT: 7
    printf("%d\n", str_find("Hello, World!", "World"));
    // EXPECT: -1
    printf("%d\n", str_find("Hello, World!", "xyz"));

    /* Word count */
    // EXPECT: 4
    printf("%d\n", str_count_words("the quick brown fox"));
    // EXPECT: 1
    printf("%d\n", str_count_words("single"));

    /* Palindrome */
    // EXPECT: 1
    printf("%d\n", is_palindrome("racecar"));
    // EXPECT: 0
    printf("%d\n", is_palindrome("hello"));
    // EXPECT: 1
    printf("%d\n", is_palindrome("abba"));

    /* Concat */
    str_concat("Hello", " World", buf);
    // EXPECT: Hello World
    printf("%s\n", buf);

    /* Starts with */
    // EXPECT: 1
    printf("%d\n", str_starts_with("Hello", "Hel"));
    // EXPECT: 0
    printf("%d\n", str_starts_with("Hello", "Wor"));

    /* Ends with */
    // EXPECT: 1
    printf("%d\n", str_ends_with("Hello", "llo"));
    // EXPECT: 0
    printf("%d\n", str_ends_with("Hello", "abc"));

    /* To upper */
    str_to_upper("hello", buf);
    // EXPECT: HELLO
    printf("%s\n", buf);

    /* Combined: reverse a palindrome is same */
    str_reverse("madam", buf);
    // EXPECT: 1
    printf("%d\n", str_eq("madam", buf));

    return 0;
}
