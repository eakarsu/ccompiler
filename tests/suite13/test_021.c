int printf(const char *fmt, ...);

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

void swap_chars(char *a, char *b) {
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

void reverse_iterative(char *s) {
    int len = str_len(s);
    int i = 0;
    while (i < len / 2) {
        swap_chars(&s[i], &s[len - 1 - i]);
        i = i + 1;
    }
}

void reverse_recursive(char *s, int left, int right) {
    if (left >= right) {
        return;
    }
    swap_chars(&s[left], &s[right]);
    reverse_recursive(s, left + 1, right - 1);
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void reverse_words(char *s) {
    int len = str_len(s);
    int start = 0;
    int end;
    int i = 0;
    while (i <= len) {
        if (i == len || s[i] == ' ') {
            end = i - 1;
            while (start < end) {
                swap_chars(&s[start], &s[end]);
                start = start + 1;
                end = end - 1;
            }
            start = i + 1;
        }
        i = i + 1;
    }
    reverse_recursive(s, 0, len - 1);
}

void reverse_segment(char *s, int from, int to) {
    while (from < to) {
        swap_chars(&s[from], &s[to]);
        from = from + 1;
        to = to - 1;
    }
}

int main() {
    char buf1[20];
    char buf2[20];
    char buf3[40];

    str_copy(buf1, "hello");
    reverse_iterative(buf1);
    printf("%s\n", buf1);
    // EXPECT: olleh

    str_copy(buf1, "abcdef");
    reverse_iterative(buf1);
    printf("%s\n", buf1);
    // EXPECT: fedcba

    str_copy(buf1, "a");
    reverse_iterative(buf1);
    printf("%s\n", buf1);
    // EXPECT: a

    str_copy(buf2, "world");
    reverse_recursive(buf2, 0, str_len(buf2) - 1);
    printf("%s\n", buf2);
    // EXPECT: dlrow

    str_copy(buf2, "ab");
    reverse_recursive(buf2, 0, str_len(buf2) - 1);
    printf("%s\n", buf2);
    // EXPECT: ba

    str_copy(buf2, "12345");
    reverse_recursive(buf2, 0, str_len(buf2) - 1);
    printf("%s\n", buf2);
    // EXPECT: 54321

    str_copy(buf3, "hello world");
    reverse_words(buf3);
    printf("%s\n", buf3);
    // EXPECT: world hello

    str_copy(buf3, "one two three");
    reverse_words(buf3);
    printf("%s\n", buf3);
    // EXPECT: three two one

    str_copy(buf3, "abc");
    reverse_words(buf3);
    printf("%s\n", buf3);
    // EXPECT: abc

    str_copy(buf1, "racecar");
    reverse_iterative(buf1);
    printf("%s\n", buf1);
    // EXPECT: racecar

    str_copy(buf1, "madam");
    char buf4[20];
    str_copy(buf4, buf1);
    reverse_recursive(buf4, 0, str_len(buf4) - 1);
    int eq = str_equal(buf1, buf4);
    printf("madam reversed equals: %d\n", eq);
    // EXPECT: madam reversed equals: 1

    str_copy(buf1, "hello");
    str_copy(buf4, buf1);
    reverse_recursive(buf4, 0, str_len(buf4) - 1);
    eq = str_equal(buf1, buf4);
    printf("hello reversed equals: %d\n", eq);
    // EXPECT: hello reversed equals: 0

    str_copy(buf1, "ABCDE");
    reverse_segment(buf1, 1, 3);
    printf("%s\n", buf1);
    // EXPECT: ADCBE

    str_copy(buf1, "xyz");
    reverse_segment(buf1, 0, 2);
    printf("%s\n", buf1);
    // EXPECT: zyx

    printf("done\n");
    // EXPECT: done

    return 0;
}
