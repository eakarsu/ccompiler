int printf(const char *fmt, ...);

/* test_024_string_ops.c
   Tests: string literals, char arrays, manual string length, copy, compare,
          string scanning, character access
*/

int my_strlen(const char *s) {
    int n = 0;
    while (s[n] != '\0') n++;
    return n;
}

/* returns 0 if equal, <0 if s<t, >0 if s>t */
int my_strcmp(const char *s, const char *t) {
    int i = 0;
    while (s[i] != '\0' && s[i] == t[i]) i++;
    return (int)(s[i]) - (int)(t[i]);
}

void my_strcpy(char *dst, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

/* concatenate src onto end of dst (dst has enough space) */
void my_strcat(char *dst, const char *src) {
    int i = 0;
    while (dst[i] != '\0') i++;
    int j = 0;
    while (src[j] != '\0') {
        dst[i] = src[j];
        i++;
        j++;
    }
    dst[i] = '\0';
}

/* count occurrences of c in s */
int count_char(const char *s, char c) {
    int n = 0;
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] == c) n++;
    }
    return n;
}

/* find first occurrence of c, return index or -1 */
int find_char(const char *s, char c) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] == c) return i;
    }
    return -1;
}

/* reverse a string in place */
void my_reverse(char *s) {
    int len = my_strlen(s);
    int i;
    for (i = 0; i < len / 2; i++) {
        char tmp = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = tmp;
    }
}

int main(void) {
    /* string literal length */
    const char *hello = "hello";
    printf("%d\n", my_strlen(hello));
    // EXPECT: 5

    const char *world = "world";
    printf("%d\n", my_strlen(world));
    // EXPECT: 5

    const char *empty = "";
    printf("%d\n", my_strlen(empty));
    // EXPECT: 0

    const char *longer = "abcdefghij";
    printf("%d\n", my_strlen(longer));
    // EXPECT: 10

    /* strcmp: equal */
    printf("%d\n", my_strcmp("abc", "abc"));
    // EXPECT: 0

    /* strcmp: less than */
    int r = my_strcmp("abc", "abd");
    if (r < 0) printf("lt\n");
    // EXPECT: lt

    /* strcmp: greater than */
    r = my_strcmp("abd", "abc");
    if (r > 0) printf("gt\n");
    // EXPECT: gt

    /* strcmp: prefix */
    r = my_strcmp("ab", "abc");
    if (r < 0) printf("prefix\n");
    // EXPECT: prefix

    /* strcpy */
    char buf[20];
    my_strcpy(buf, "hello");
    printf("%s\n", buf);
    // EXPECT: hello
    printf("%d\n", my_strlen(buf));
    // EXPECT: 5

    /* strcpy overwrites */
    my_strcpy(buf, "hi");
    printf("%s\n", buf);
    // EXPECT: hi

    /* strcat */
    char cat_buf[40];
    my_strcpy(cat_buf, "foo");
    my_strcat(cat_buf, "bar");
    printf("%s\n", cat_buf);
    // EXPECT: foobar

    my_strcpy(cat_buf, "hello");
    my_strcat(cat_buf, " ");
    my_strcat(cat_buf, "world");
    printf("%s\n", cat_buf);
    // EXPECT: hello world

    /* char array manipulation */
    char arr[6] = {'h', 'e', 'l', 'l', 'o', '\0'};
    printf("%s\n", arr);
    // EXPECT: hello
    printf("%c\n", arr[0]);
    // EXPECT: h
    printf("%c\n", arr[4]);
    // EXPECT: o

    arr[0] = 'H';
    printf("%s\n", arr);
    // EXPECT: Hello

    /* count_char */
    printf("%d\n", count_char("hello world", 'l'));
    // EXPECT: 3
    printf("%d\n", count_char("aaaaaa", 'a'));
    // EXPECT: 6
    printf("%d\n", count_char("abc", 'z'));
    // EXPECT: 0

    /* find_char */
    printf("%d\n", find_char("hello", 'e'));
    // EXPECT: 1
    printf("%d\n", find_char("hello", 'o'));
    // EXPECT: 4
    printf("%d\n", find_char("hello", 'x'));
    // EXPECT: -1
    printf("%d\n", find_char("hello", 'h'));
    // EXPECT: 0

    /* reverse */
    char rev[10];
    my_strcpy(rev, "abcde");
    my_reverse(rev);
    printf("%s\n", rev);
    // EXPECT: edcba

    my_strcpy(rev, "a");
    my_reverse(rev);
    printf("%s\n", rev);
    // EXPECT: a

    my_strcpy(rev, "ab");
    my_reverse(rev);
    printf("%s\n", rev);
    // EXPECT: ba

    my_strcpy(rev, "abcd");
    my_reverse(rev);
    printf("%s\n", rev);
    // EXPECT: dcba

    /* string literal indexing */
    const char *s = "ABCDE";
    printf("%c\n", s[0]);
    // EXPECT: A
    printf("%c\n", s[4]);
    // EXPECT: E

    /* compare after copy */
    char orig[20];
    my_strcpy(orig, "compare");
    printf("%d\n", my_strcmp(orig, "compare"));
    // EXPECT: 0
    printf("%d\n", my_strcmp(orig, "comparf"));
    // EXPECT: -1

    return 0;
}
