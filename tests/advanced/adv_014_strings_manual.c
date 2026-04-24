int printf(const char *fmt, ...);
// EXPECT: === Manual String Tests ===
// EXPECT: strlen_hello: 5
// EXPECT: strlen_empty: 0
// EXPECT: strlen_one: 1
// EXPECT: strcmp_equal: 0
// EXPECT: strcmp_less: less
// EXPECT: strcmp_greater: greater
// EXPECT: strcmp_prefix: not equal
// EXPECT: strcpy_test: hello
// EXPECT: strcpy_overwrite: world
// EXPECT: strcat_test: helloworld
// EXPECT: strcat_space: hello world
// EXPECT: reverse_hello: olleh
// EXPECT: reverse_a: a
// EXPECT: reverse_abcd: dcba
// EXPECT: count_a: 3
// EXPECT: count_spaces: 4
// EXPECT: to_upper: HELLO
// EXPECT: to_lower: hello
// EXPECT: is_palindrome_racecar: yes
// EXPECT: is_palindrome_hello: no
// EXPECT: find_char_l: 2
// EXPECT: find_char_z: -1
// EXPECT: str_contains: yes
// EXPECT: count_words: 5
// EXPECT: trim_leading: hello
// EXPECT: char_at: l
// EXPECT: str_repeat: abcabcabc
// EXPECT: === All Manual String Tests Passed ===

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
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
        i = i + 1;
    }
    if (a[i] == 0 && b[i] == 0) return 0;
    if (a[i] == 0) return -1;
    return 1;
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

void my_reverse(char *s) {
    int len = my_strlen(s);
    int i;
    char tmp;
    for (i = 0; i < len / 2; i++) {
        tmp = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = tmp;
    }
}

int count_char(char *s, char c) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == c) count = count + 1;
        i = i + 1;
    }
    return count;
}

void to_upper(char *s) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] = s[i] - 32;
        }
        i = i + 1;
    }
}

void to_lower(char *s) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] + 32;
        }
        i = i + 1;
    }
}

int is_palindrome(char *s) {
    int len = my_strlen(s);
    int i;
    for (i = 0; i < len / 2; i++) {
        if (s[i] != s[len - 1 - i]) return 0;
    }
    return 1;
}

int find_char(char *s, char c) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == c) return i;
        i = i + 1;
    }
    return -1;
}

int str_contains(char *haystack, char *needle) {
    int hlen = my_strlen(haystack);
    int nlen = my_strlen(needle);
    int i;
    int j;
    if (nlen == 0) return 1;
    for (i = 0; i <= hlen - nlen; i++) {
        int match = 1;
        for (j = 0; j < nlen; j++) {
            if (haystack[i + j] != needle[j]) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    return 0;
}

int count_words(char *s) {
    int count = 0;
    int in_word = 0;
    int i = 0;
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

void trim_leading(char *dst, char *src) {
    int i = 0;
    while (src[i] == ' ') {
        i = i + 1;
    }
    my_strcpy(dst, src + i);
}

void str_repeat(char *dst, char *src, int times) {
    dst[0] = 0;
    int t;
    for (t = 0; t < times; t++) {
        my_strcat(dst, src);
    }
}

int main() {
    printf("=== Manual String Tests ===\n");

    // strlen
    char h[6];
    h[0] = 'h'; h[1] = 'e'; h[2] = 'l'; h[3] = 'l'; h[4] = 'o'; h[5] = 0;
    printf("strlen_hello: %d\n", my_strlen(h));

    char empty[1];
    empty[0] = 0;
    printf("strlen_empty: %d\n", my_strlen(empty));

    char one[2];
    one[0] = 'x'; one[1] = 0;
    printf("strlen_one: %d\n", my_strlen(one));

    // strcmp
    char s1[6];
    s1[0] = 'h'; s1[1] = 'e'; s1[2] = 'l'; s1[3] = 'l'; s1[4] = 'o'; s1[5] = 0;
    char s2[6];
    s2[0] = 'h'; s2[1] = 'e'; s2[2] = 'l'; s2[3] = 'l'; s2[4] = 'o'; s2[5] = 0;
    printf("strcmp_equal: %d\n", my_strcmp(s1, s2));

    char sa[4];
    sa[0] = 'a'; sa[1] = 'b'; sa[2] = 'c'; sa[3] = 0;
    char sb[4];
    sb[0] = 'a'; sb[1] = 'b'; sb[2] = 'd'; sb[3] = 0;
    printf("strcmp_less: %s\n", my_strcmp(sa, sb) < 0 ? "less" : "not less");

    printf("strcmp_greater: %s\n", my_strcmp(sb, sa) > 0 ? "greater" : "not greater");

    char sp[3];
    sp[0] = 'a'; sp[1] = 'b'; sp[2] = 0;
    char sl[4];
    sl[0] = 'a'; sl[1] = 'b'; sl[2] = 'c'; sl[3] = 0;
    printf("strcmp_prefix: %s\n", my_strcmp(sp, sl) == 0 ? "equal" : "not equal");

    // strcpy
    char dst1[10];
    my_strcpy(dst1, h);
    printf("strcpy_test: %s\n", dst1);

    char w[6];
    w[0] = 'w'; w[1] = 'o'; w[2] = 'r'; w[3] = 'l'; w[4] = 'd'; w[5] = 0;
    my_strcpy(dst1, w);
    printf("strcpy_overwrite: %s\n", dst1);

    // strcat
    char cat1[20];
    my_strcpy(cat1, h);
    my_strcat(cat1, w);
    printf("strcat_test: %s\n", cat1);

    char cat2[20];
    char space[2];
    space[0] = ' '; space[1] = 0;
    my_strcpy(cat2, h);
    my_strcat(cat2, space);
    my_strcat(cat2, w);
    printf("strcat_space: %s\n", cat2);

    // reverse
    char rev1[6];
    my_strcpy(rev1, h);
    my_reverse(rev1);
    printf("reverse_hello: %s\n", rev1);

    char rev2[2];
    rev2[0] = 'a'; rev2[1] = 0;
    my_reverse(rev2);
    printf("reverse_a: %s\n", rev2);

    char rev3[5];
    rev3[0] = 'a'; rev3[1] = 'b'; rev3[2] = 'c'; rev3[3] = 'd'; rev3[4] = 0;
    my_reverse(rev3);
    printf("reverse_abcd: %s\n", rev3);

    // count characters
    char cstr[10];
    cstr[0] = 'b'; cstr[1] = 'a'; cstr[2] = 'n'; cstr[3] = 'a';
    cstr[4] = 'n'; cstr[5] = 'a'; cstr[6] = 0;
    printf("count_a: %d\n", count_char(cstr, 'a'));

    char spc[20];
    spc[0] = 'a'; spc[1] = ' '; spc[2] = 'b'; spc[3] = ' ';
    spc[4] = 'c'; spc[5] = ' '; spc[6] = 'd'; spc[7] = ' ';
    spc[8] = 'e'; spc[9] = 0;
    printf("count_spaces: %d\n", count_char(spc, ' '));

    // to_upper
    char up[6];
    my_strcpy(up, h);
    to_upper(up);
    printf("to_upper: %s\n", up);

    // to_lower
    char lo[6];
    lo[0] = 'H'; lo[1] = 'E'; lo[2] = 'L'; lo[3] = 'L'; lo[4] = 'O'; lo[5] = 0;
    to_lower(lo);
    printf("to_lower: %s\n", lo);

    // palindrome check
    char pal1[8];
    pal1[0] = 'r'; pal1[1] = 'a'; pal1[2] = 'c'; pal1[3] = 'e';
    pal1[4] = 'c'; pal1[5] = 'a'; pal1[6] = 'r'; pal1[7] = 0;
    printf("is_palindrome_racecar: %s\n", is_palindrome(pal1) ? "yes" : "no");

    printf("is_palindrome_hello: %s\n", is_palindrome(h) ? "yes" : "no");

    // find character
    printf("find_char_l: %d\n", find_char(h, 'l'));
    printf("find_char_z: %d\n", find_char(h, 'z'));

    // string contains substring
    char hay[12];
    hay[0] = 'h'; hay[1] = 'e'; hay[2] = 'l'; hay[3] = 'l';
    hay[4] = 'o'; hay[5] = 'w'; hay[6] = 'o'; hay[7] = 'r';
    hay[8] = 'l'; hay[9] = 'd'; hay[10] = 0;
    char needle[4];
    needle[0] = 'w'; needle[1] = 'o'; needle[2] = 'r'; needle[3] = 0;
    printf("str_contains: %s\n", str_contains(hay, needle) ? "yes" : "no");

    // count words
    char words[30];
    words[0] = 'o'; words[1] = 'n'; words[2] = 'e'; words[3] = ' ';
    words[4] = 't'; words[5] = 'w'; words[6] = 'o'; words[7] = ' ';
    words[8] = 't'; words[9] = 'h'; words[10] = 'r'; words[11] = 'e';
    words[12] = 'e'; words[13] = ' '; words[14] = 'f'; words[15] = 'o';
    words[16] = 'u'; words[17] = 'r'; words[18] = ' '; words[19] = 'f';
    words[20] = 'i'; words[21] = 'v'; words[22] = 'e'; words[23] = 0;
    printf("count_words: %d\n", count_words(words));

    // trim leading spaces
    char trimmed[20];
    char with_spaces[20];
    with_spaces[0] = ' '; with_spaces[1] = ' '; with_spaces[2] = ' ';
    with_spaces[3] = 'h'; with_spaces[4] = 'e'; with_spaces[5] = 'l';
    with_spaces[6] = 'l'; with_spaces[7] = 'o'; with_spaces[8] = 0;
    trim_leading(trimmed, with_spaces);
    printf("trim_leading: %s\n", trimmed);

    // char at index
    printf("char_at: %c\n", h[2]);

    // string repeat
    char rep[20];
    char abc[4];
    abc[0] = 'a'; abc[1] = 'b'; abc[2] = 'c'; abc[3] = 0;
    str_repeat(rep, abc, 3);
    printf("str_repeat: %s\n", rep);

    printf("=== All Manual String Tests Passed ===\n");
    return 0;
}
