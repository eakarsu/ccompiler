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

void my_strncpy(char *dst, char *src, int n) {
    int i = 0;
    while (i < n && src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    while (i < n) {
        dst[i] = 0;
        i = i + 1;
    }
}

void fill_char(char *buf, int n, char c) {
    int i = 0;
    while (i < n) {
        buf[i] = c;
        i = i + 1;
    }
    buf[n] = 0;
}

void copy_range(char *dst, char *src, int start, int end) {
    int i = 0;
    int j = start;
    while (j < end && src[j] != 0) {
        dst[i] = src[j];
        i = i + 1;
        j = j + 1;
    }
    dst[i] = 0;
}

void copy_every_other(char *dst, char *src) {
    int i = 0;
    int j = 0;
    while (src[i] != 0) {
        if (i % 2 == 0) {
            dst[j] = src[i];
            j = j + 1;
        }
        i = i + 1;
    }
    dst[j] = 0;
}

void copy_filter_spaces(char *dst, char *src) {
    int i = 0;
    int j = 0;
    while (src[i] != 0) {
        if (src[i] != ' ') {
            dst[j] = src[i];
            j = j + 1;
        }
        i = i + 1;
    }
    dst[j] = 0;
}

void repeat_string(char *dst, char *src, int times) {
    int slen = my_strlen(src);
    int pos = 0;
    int t = 0;
    while (t < times) {
        int i = 0;
        while (i < slen) {
            dst[pos] = src[i];
            pos = pos + 1;
            i = i + 1;
        }
        t = t + 1;
    }
    dst[pos] = 0;
}

void pad_right(char *dst, char *src, int width, char pad) {
    int slen = my_strlen(src);
    my_strcpy(dst, src);
    int i = slen;
    while (i < width) {
        dst[i] = pad;
        i = i + 1;
    }
    dst[i] = 0;
}

int main() {
    char buf[128];

    my_strcpy(buf, "hello");
    // EXPECT: strcpy = hello
    printf("strcpy = %s\n", buf);

    my_strcpy(buf, "world");
    // EXPECT: strcpy2 = world
    printf("strcpy2 = %s\n", buf);

    my_strcpy(buf, "");
    // EXPECT: strcpy empty len = 0
    printf("strcpy empty len = %d\n", my_strlen(buf));

    my_strncpy(buf, "hello world", 5);
    buf[5] = 0;
    // EXPECT: strncpy 5 = hello
    printf("strncpy 5 = %s\n", buf);

    my_strncpy(buf, "hi", 5);
    // EXPECT: strncpy pad len = 2
    printf("strncpy pad len = %d\n", my_strlen(buf));

    fill_char(buf, 5, 'x');
    // EXPECT: fill = xxxxx
    printf("fill = %s\n", buf);

    fill_char(buf, 3, 'a');
    // EXPECT: fill2 = aaa
    printf("fill2 = %s\n", buf);

    copy_range(buf, "hello world", 6, 11);
    // EXPECT: range = world
    printf("range = %s\n", buf);

    copy_range(buf, "abcdef", 2, 5);
    // EXPECT: range2 = cde
    printf("range2 = %s\n", buf);

    copy_every_other(buf, "abcdef");
    // EXPECT: every_other = ace
    printf("every_other = %s\n", buf);

    copy_every_other(buf, "hello world");
    // EXPECT: every_other2 = hlowrd
    printf("every_other2 = %s\n", buf);

    copy_filter_spaces(buf, "hello world");
    // EXPECT: no_spaces = helloworld
    printf("no_spaces = %s\n", buf);

    copy_filter_spaces(buf, "a b c d");
    // EXPECT: no_spaces2 = abcd
    printf("no_spaces2 = %s\n", buf);

    repeat_string(buf, "ab", 3);
    // EXPECT: repeat = ababab
    printf("repeat = %s\n", buf);

    repeat_string(buf, "xyz", 2);
    // EXPECT: repeat2 = xyzxyz
    printf("repeat2 = %s\n", buf);

    pad_right(buf, "hi", 6, '.');
    // EXPECT: pad = hi....
    printf("pad = %s\n", buf);

    pad_right(buf, "test", 8, '-');
    // EXPECT: pad2 = test----
    printf("pad2 = %s\n", buf);

    return 0;
}
