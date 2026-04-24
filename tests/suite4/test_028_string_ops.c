int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
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

void my_strcpy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

void my_strcat(char *dst, char *src) {
    int i = 0;
    while (dst[i] != 0) {
        i = i + 1;
    }
    int j = 0;
    while (src[j] != 0) {
        dst[i] = src[j];
        i = i + 1;
        j = j + 1;
    }
    dst[i] = 0;
}

int my_strncmp(char *a, char *b, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (a[i] != b[i]) {
            return a[i] - b[i];
        }
        if (a[i] == 0) {
            return 0;
        }
    }
    return 0;
}

int count_char(char *s, char c) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == c) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int find_char(char *s, char c) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == c) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

void reverse_string(char *s) {
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

int main(void) {
    char *hello = "Hello";
    // EXPECT: Hello
    printf("%s\n", hello);

    // EXPECT: 5
    printf("%d\n", my_strlen(hello));

    char *world = "World";
    // EXPECT: 5
    printf("%d\n", my_strlen(world));

    /* strcmp: "Hello" vs "Hello" => 0 */
    // EXPECT: 0
    printf("%d\n", my_strcmp(hello, hello));

    /* "Hello" vs "World": 'H'(72) - 'W'(87) = -15 */
    int cmp = my_strcmp(hello, world);
    int neg = (cmp < 0) ? 1 : 0;
    // EXPECT: 1
    printf("%d\n", neg);

    /* strcpy test */
    char buf[20];
    my_strcpy(buf, hello);
    // EXPECT: Hello
    printf("%s\n", buf);

    /* strcat test */
    char cat_buf[20];
    my_strcpy(cat_buf, "Hi");
    my_strcat(cat_buf, "There");
    // EXPECT: HiThere
    printf("%s\n", cat_buf);
    // EXPECT: 7
    printf("%d\n", my_strlen(cat_buf));

    /* strncmp test */
    // EXPECT: 0
    printf("%d\n", my_strncmp("Hello", "Help", 3));

    int sncmp = my_strncmp("Hello", "Help", 4);
    int is_neg = (sncmp < 0) ? 1 : 0;
    // EXPECT: 1
    printf("%d\n", is_neg);

    /* count_char: "banana" has 'a' at positions 1,3,5 = 3 */
    // EXPECT: 3
    printf("%d\n", count_char("banana", 'a'));

    /* 'n' appears 2 times, 'b' appears 1 time, total 3 */
    // EXPECT: 3
    printf("%d\n", count_char("banana", 'n') + count_char("banana", 'b'));

    /* find_char */
    // EXPECT: 0
    printf("%d\n", find_char("abcdef", 'a'));
    // EXPECT: 3
    printf("%d\n", find_char("abcdef", 'd'));
    // EXPECT: -1
    printf("%d\n", find_char("abcdef", 'z'));

    /* reverse_string */
    char rev[10];
    my_strcpy(rev, "abcde");
    reverse_string(rev);
    // EXPECT: edcba
    printf("%s\n", rev);

    /* reverse palindrome check */
    char pal[10];
    my_strcpy(pal, "racecar");
    char pal_copy[10];
    my_strcpy(pal_copy, pal);
    reverse_string(pal_copy);
    // EXPECT: 0
    printf("%d\n", my_strcmp(pal, pal_copy));

    /* Empty string */
    char *empty = "";
    // EXPECT: 0
    printf("%d\n", my_strlen(empty));

    /* Single char */
    char *single = "X";
    // EXPECT: 1
    printf("%d\n", my_strlen(single));

    /* Char array init */
    char arr[4];
    arr[0] = 'A';
    arr[1] = 'B';
    arr[2] = 'C';
    arr[3] = 0;
    // EXPECT: ABC
    printf("%s\n", arr);
    // EXPECT: 3
    printf("%d\n", my_strlen(arr));

    return 0;
}
