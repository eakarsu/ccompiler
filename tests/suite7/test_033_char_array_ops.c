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
        if (a[i] != b[i]) {
            return a[i] - b[i];
        }
        i = i + 1;
    }
    return a[i] - b[i];
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

void my_reverse(char *s) {
    int len = my_strlen(s);
    int i = 0;
    int j = len - 1;
    while (i < j) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i = i + 1;
        j = j - 1;
    }
}

int my_strncmp(char *a, char *b, int n) {
    int i = 0;
    while (i < n && a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) {
            return a[i] - b[i];
        }
        i = i + 1;
    }
    if (i == n) return 0;
    return a[i] - b[i];
}

void my_strncpy(char *dst, char *src, int n) {
    int i = 0;
    while (i < n && src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int main(void) {
    char buf[64];
    char buf2[64];

    buf[0] = 'h'; buf[1] = 'e'; buf[2] = 'l';
    buf[3] = 'l'; buf[4] = 'o'; buf[5] = 0;

    // EXPECT: len=5
    printf("len=%d\n", my_strlen(buf));

    my_strcpy(buf2, buf);
    // EXPECT: copy=hello
    printf("copy=%s\n", buf2);

    // EXPECT: cmp_eq=0
    printf("cmp_eq=%d\n", my_strcmp(buf, buf2));

    buf2[0] = 'z'; buf2[1] = 0;
    int c = my_strcmp(buf, buf2);
    // EXPECT: cmp_lt=1
    printf("cmp_lt=%d\n", c < 0 ? 1 : 0);

    buf2[0] = 'a'; buf2[1] = 0;
    c = my_strcmp(buf, buf2);
    // EXPECT: cmp_gt=1
    printf("cmp_gt=%d\n", c > 0 ? 1 : 0);

    char cat[64];
    cat[0] = 'f'; cat[1] = 'o'; cat[2] = 'o'; cat[3] = 0;

    char bar[4];
    bar[0] = 'b'; bar[1] = 'a'; bar[2] = 'r'; bar[3] = 0;

    my_strcat(cat, bar);
    // EXPECT: cat=foobar
    printf("cat=%s\n", cat);

    // EXPECT: catlen=6
    printf("catlen=%d\n", my_strlen(cat));

    char rev[6];
    rev[0] = 'a'; rev[1] = 'b'; rev[2] = 'c';
    rev[3] = 'd'; rev[4] = 'e'; rev[5] = 0;
    my_reverse(rev);
    // EXPECT: rev=edcba
    printf("rev=%s\n", rev);

    char single[2];
    single[0] = 'x'; single[1] = 0;
    my_reverse(single);
    // EXPECT: rev1=x
    printf("rev1=%s\n", single);

    char empty[1];
    empty[0] = 0;
    // EXPECT: empty_len=0
    printf("empty_len=%d\n", my_strlen(empty));

    // EXPECT: ncmp_eq=0
    printf("ncmp_eq=%d\n", my_strncmp("hello", "help", 3));

    int nc = my_strncmp("hello", "help", 4);
    // EXPECT: ncmp_ne=1
    printf("ncmp_ne=%d\n", nc != 0 ? 1 : 0);

    char sub[10];
    my_strncpy(sub, "abcdef", 3);
    // EXPECT: sub=abc
    printf("sub=%s\n", sub);

    // EXPECT: sublen=3
    printf("sublen=%d\n", my_strlen(sub));

    my_strcpy(buf, "world");
    // EXPECT: world=world
    printf("world=%s\n", buf);

    // EXPECT: world_len=5
    printf("world_len=%d\n", my_strlen(buf));

    my_reverse(buf);
    // EXPECT: rev_world=dlrow
    printf("rev_world=%s\n", buf);

    my_reverse(buf);
    my_strcat(buf, "!");
    // EXPECT: excl=world!
    printf("excl=%s\n", buf);

    return 0;
}
