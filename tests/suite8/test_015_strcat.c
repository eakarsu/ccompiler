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

void my_strcat(char *dst, char *src) {
    int dlen = my_strlen(dst);
    int i = 0;
    while (src[i] != 0) {
        dst[dlen + i] = src[i];
        i = i + 1;
    }
    dst[dlen + i] = 0;
}

void my_strncat(char *dst, char *src, int n) {
    int dlen = my_strlen(dst);
    int i = 0;
    while (i < n && src[i] != 0) {
        dst[dlen + i] = src[i];
        i = i + 1;
    }
    dst[dlen + i] = 0;
}

void join_strings(char *dst, char *strings[], int count, char *sep) {
    dst[0] = 0;
    int i = 0;
    while (i < count) {
        if (i > 0) {
            my_strcat(dst, sep);
        }
        my_strcat(dst, strings[i]);
        i = i + 1;
    }
}

void concat_with_space(char *dst, char *a, char *b) {
    my_strcpy(dst, a);
    my_strcat(dst, " ");
    my_strcat(dst, b);
}

void surround(char *dst, char *s, char left, char right) {
    dst[0] = left;
    dst[1] = 0;
    my_strcat(dst, s);
    int len = my_strlen(dst);
    dst[len] = right;
    dst[len + 1] = 0;
}

void int_to_str(char *buf, int n) {
    if (n == 0) {
        buf[0] = '0';
        buf[1] = 0;
        return;
    }
    int neg = 0;
    if (n < 0) {
        neg = 1;
        n = -n;
    }
    char tmp[32];
    int i = 0;
    while (n > 0) {
        tmp[i] = '0' + (n % 10);
        n = n / 10;
        i = i + 1;
    }
    int pos = 0;
    if (neg) {
        buf[pos] = '-';
        pos = pos + 1;
    }
    int j = i - 1;
    while (j >= 0) {
        buf[pos] = tmp[j];
        pos = pos + 1;
        j = j - 1;
    }
    buf[pos] = 0;
}

void concat_int(char *dst, char *prefix, int val) {
    char numbuf[32];
    int_to_str(numbuf, val);
    my_strcpy(dst, prefix);
    my_strcat(dst, numbuf);
}

int main() {
    char buf[256];

    my_strcpy(buf, "hello");
    my_strcat(buf, " world");
    // EXPECT: cat = hello world
    printf("cat = %s\n", buf);

    my_strcpy(buf, "");
    my_strcat(buf, "test");
    // EXPECT: cat empty = test
    printf("cat empty = %s\n", buf);

    my_strcpy(buf, "abc");
    my_strcat(buf, "");
    // EXPECT: cat with empty = abc
    printf("cat with empty = %s\n", buf);

    my_strcpy(buf, "foo");
    my_strncat(buf, "barbaz", 3);
    // EXPECT: ncat = foobar
    printf("ncat = %s\n", buf);

    my_strcpy(buf, "hello");
    my_strncat(buf, "xx", 5);
    // EXPECT: ncat short = helloxx
    printf("ncat short = %s\n", buf);

    char *parts[3];
    parts[0] = "alpha";
    parts[1] = "beta";
    parts[2] = "gamma";
    join_strings(buf, parts, 3, ", ");
    // EXPECT: join = alpha, beta, gamma
    printf("join = %s\n", buf);

    join_strings(buf, parts, 3, "-");
    // EXPECT: join dash = alpha-beta-gamma
    printf("join dash = %s\n", buf);

    join_strings(buf, parts, 1, ", ");
    // EXPECT: join one = alpha
    printf("join one = %s\n", buf);

    concat_with_space(buf, "hello", "world");
    // EXPECT: space = hello world
    printf("space = %s\n", buf);

    concat_with_space(buf, "good", "morning");
    // EXPECT: space2 = good morning
    printf("space2 = %s\n", buf);

    surround(buf, "hello", '[', ']');
    // EXPECT: surround = [hello]
    printf("surround = %s\n", buf);

    surround(buf, "test", '(', ')');
    // EXPECT: surround2 = (test)
    printf("surround2 = %s\n", buf);

    char numbuf[32];
    int_to_str(numbuf, 42);
    // EXPECT: itos 42 = 42
    printf("itos 42 = %s\n", numbuf);

    int_to_str(numbuf, 0);
    // EXPECT: itos 0 = 0
    printf("itos 0 = %s\n", numbuf);

    int_to_str(numbuf, -123);
    // EXPECT: itos -123 = -123
    printf("itos -123 = %s\n", numbuf);

    concat_int(buf, "count=", 99);
    // EXPECT: concat_int = count=99
    printf("concat_int = %s\n", buf);

    concat_int(buf, "val=", -5);
    // EXPECT: concat_int2 = val=-5
    printf("concat_int2 = %s\n", buf);

    return 0;
}
