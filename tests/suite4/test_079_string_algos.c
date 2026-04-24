int printf(const char *fmt, ...);

int str_len(char *s) {
    int len;
    len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
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

void str_reverse(char *s) {
    int left;
    int right;
    char temp;
    left = 0;
    right = str_len(s) - 1;
    while (left < right) {
        temp = s[left];
        s[left] = s[right];
        s[right] = temp;
        left = left + 1;
        right = right - 1;
    }
}

int str_compare(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] < b[i]) {
            return -1;
        }
        if (a[i] > b[i]) {
            return 1;
        }
        i = i + 1;
    }
    if (a[i] == 0 && b[i] == 0) {
        return 0;
    }
    if (a[i] == 0) {
        return -1;
    }
    return 1;
}

int str_find_char(char *s, char c) {
    int i;
    i = 0;
    while (s[i] != 0) {
        if (s[i] == c) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

int str_count_char(char *s, char c) {
    int i;
    int count;
    count = 0;
    i = 0;
    while (s[i] != 0) {
        if (s[i] == c) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

void str_concat(char *dst, char *a, char *b) {
    int i;
    int j;
    i = 0;
    while (a[i] != 0) {
        dst[i] = a[i];
        i = i + 1;
    }
    j = 0;
    while (b[j] != 0) {
        dst[i] = b[j];
        i = i + 1;
        j = j + 1;
    }
    dst[i] = 0;
}

int str_is_palindrome(char *s) {
    int left;
    int right;
    left = 0;
    right = str_len(s) - 1;
    while (left < right) {
        if (s[left] != s[right]) {
            return 0;
        }
        left = left + 1;
        right = right - 1;
    }
    return 1;
}

void str_to_upper(char *s) {
    int i;
    i = 0;
    while (s[i] != 0) {
        if (s[i] >= 97 && s[i] <= 122) {
            s[i] = s[i] - 32;
        }
        i = i + 1;
    }
}

int main(void) {
    char buf[64];
    char buf2[64];
    char concat[64];

    // EXPECT: len hello: 5
    printf("len hello: %d\n", str_len("hello"));
    // EXPECT: len empty: 0
    printf("len empty: %d\n", str_len(""));
    // EXPECT: len abc: 3
    printf("len abc: %d\n", str_len("abc"));

    str_copy(buf, "world");
    // EXPECT: copy: world
    printf("copy: %s\n", buf);

    str_copy(buf, "abcde");
    str_reverse(buf);
    // EXPECT: reverse: edcba
    printf("reverse: %s\n", buf);

    str_copy(buf, "a");
    str_reverse(buf);
    // EXPECT: rev single: a
    printf("rev single: %s\n", buf);

    // EXPECT: cmp equal: 0
    printf("cmp equal: %d\n", str_compare("abc", "abc"));
    // EXPECT: cmp less: -1
    printf("cmp less: %d\n", str_compare("abc", "abd"));
    // EXPECT: cmp greater: 1
    printf("cmp greater: %d\n", str_compare("abd", "abc"));
    // EXPECT: cmp prefix: -1
    printf("cmp prefix: %d\n", str_compare("ab", "abc"));
    // EXPECT: cmp longer: 1
    printf("cmp longer: %d\n", str_compare("abc", "ab"));

    // EXPECT: find l in hello: 2
    printf("find l in hello: %d\n", str_find_char("hello", 'l'));
    // EXPECT: find z in hello: -1
    printf("find z in hello: %d\n", str_find_char("hello", 'z'));
    // EXPECT: find h in hello: 0
    printf("find h in hello: %d\n", str_find_char("hello", 'h'));

    // EXPECT: count l in hello: 2
    printf("count l in hello: %d\n", str_count_char("hello", 'l'));
    // EXPECT: count o in hello: 1
    printf("count o in hello: %d\n", str_count_char("hello", 'o'));
    // EXPECT: count a in hello: 0
    printf("count a in hello: %d\n", str_count_char("hello", 'a'));

    str_concat(concat, "hello", " world");
    // EXPECT: concat: hello world
    printf("concat: %s\n", concat);

    str_concat(concat, "", "test");
    // EXPECT: concat empty: test
    printf("concat empty: %s\n", concat);

    // EXPECT: palindrome racecar: 1
    printf("palindrome racecar: %d\n", str_is_palindrome("racecar"));
    // EXPECT: palindrome hello: 0
    printf("palindrome hello: %d\n", str_is_palindrome("hello"));
    // EXPECT: palindrome aba: 1
    printf("palindrome aba: %d\n", str_is_palindrome("aba"));
    // EXPECT: palindrome a: 1
    printf("palindrome a: %d\n", str_is_palindrome("a"));

    str_copy(buf, "hello");
    str_to_upper(buf);
    // EXPECT: upper: HELLO
    printf("upper: %s\n", buf);

    str_copy(buf, "Test123");
    str_to_upper(buf);
    // EXPECT: upper mixed: TEST123
    printf("upper mixed: %s\n", buf);

    // EXPECT: done
    printf("done\n");

    return 0;
}
