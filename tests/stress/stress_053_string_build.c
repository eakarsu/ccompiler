// stress_053_string_build.c - String building stress test
// Construct strings char-by-char, concatenate, reverse, compare, extract substrings.
// EXPECT: build=HELLO WORLD
// EXPECT: concat=ABCDEFGHIJ
// EXPECT: reverse1=DLROW OLLEH
// EXPECT: reverse2=HELLO WORLD
// EXPECT: cmp_eq=1
// EXPECT: cmp_ne=1
// EXPECT: cmp_lt=1
// EXPECT: substr=ELLO
// EXPECT: fill_pattern=ABABABABAB
// EXPECT: to_lower=hello world
// EXPECT: count_vowels=2
// EXPECT: replace_char=HELLX WXRLD
// EXPECT: is_palindrome=1
// EXPECT: build_num=12345
// EXPECT: string_tests_passed=15

int printf(const char *fmt, ...);

int str_len(char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

void str_cpy(char *dst, char *src) {
    int i = 0;
    while (src[i]) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = 0;
}

void str_cat(char *dst, char *src) {
    int d = str_len(dst);
    int i = 0;
    while (src[i]) {
        dst[d + i] = src[i];
        i++;
    }
    dst[d + i] = 0;
}

void str_reverse(char *s) {
    int len = str_len(s);
    int i;
    for (i = 0; i < len / 2; i++) {
        char tmp = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = tmp;
    }
}

int str_cmp(char *a, char *b) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
        i++;
    }
    if (a[i]) return 1;
    if (b[i]) return -1;
    return 0;
}

void str_substr(char *dst, char *src, int start, int len) {
    int i;
    for (i = 0; i < len && src[start + i]; i++) {
        dst[i] = src[start + i];
    }
    dst[i] = 0;
}

void str_fill_pattern(char *dst, char *pat, int count) {
    int plen = str_len(pat);
    int d = 0;
    int i;
    for (i = 0; i < count; i++) {
        int j;
        for (j = 0; j < plen; j++) {
            dst[d++] = pat[j];
        }
    }
    dst[d] = 0;
}

void str_to_lower(char *dst, char *src) {
    int i = 0;
    while (src[i]) {
        if (src[i] >= 'A' && src[i] <= 'Z')
            dst[i] = src[i] + 32;
        else
            dst[i] = src[i];
        i++;
    }
    dst[i] = 0;
}

int count_vowels(char *s) {
    int count = 0;
    int i = 0;
    while (s[i]) {
        char c = s[i];
        if (c >= 'A' && c <= 'Z') c = c + 32;
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
            count++;
        i++;
    }
    return count;
}

void replace_char(char *dst, char *src, char old, char new_c) {
    int i = 0;
    while (src[i]) {
        if (src[i] == old)
            dst[i] = new_c;
        else
            dst[i] = src[i];
        i++;
    }
    dst[i] = 0;
}

int is_palindrome(char *s) {
    int len = str_len(s);
    int i;
    for (i = 0; i < len / 2; i++) {
        if (s[i] != s[len - 1 - i]) return 0;
    }
    return 1;
}

void int_to_str(char *buf, int n) {
    char tmp[20];
    int i = 0;
    if (n == 0) {
        buf[0] = '0'; buf[1] = 0;
        return;
    }
    while (n > 0) {
        tmp[i++] = '0' + (n % 10);
        n = n / 10;
    }
    int j;
    for (j = 0; j < i; j++) {
        buf[j] = tmp[i - 1 - j];
    }
    buf[i] = 0;
}

int main(void) {
    int passed = 0;
    char buf[128];


    // Build string char by char
    buf[0] = 'H'; buf[1] = 'E'; buf[2] = 'L'; buf[3] = 'L'; buf[4] = 'O';
    buf[5] = ' ';
    buf[6] = 'W'; buf[7] = 'O'; buf[8] = 'R'; buf[9] = 'L'; buf[10] = 'D';
    buf[11] = 0;
    printf("build=%s\n", buf);
    passed++;

    // Concatenation
    char a[64]; char b[64];
    a[0] = 'A'; a[1] = 'B'; a[2] = 'C'; a[3] = 'D'; a[4] = 'E'; a[5] = 0;
    b[0] = 'F'; b[1] = 'G'; b[2] = 'H'; b[3] = 'I'; b[4] = 'J'; b[5] = 0;
    str_cat(a, b);
    printf("concat=%s\n", a);
    passed++;

    // Reverse
    char rev[64];
    str_cpy(rev, buf);
    str_reverse(rev);
    printf("reverse1=%s\n", rev);
    passed++;

    // Reverse again => original
    str_reverse(rev);
    printf("reverse2=%s\n", rev);
    passed++;

    // Comparison
    printf("cmp_eq=%d\n", str_eq(rev, buf));
    passed++;

    printf("cmp_ne=%d\n", !str_eq(buf, a));
    passed++;

    printf("cmp_lt=%d\n", str_cmp("ABC", "ABD") < 0 ? 1 : 0);
    passed++;

    // Substring
    char sub[64];
    str_substr(sub, buf, 1, 4);
    printf("substr=%s\n", sub);
    passed++;

    // Fill pattern
    char pat[64];
    str_fill_pattern(pat, "AB", 5);
    printf("fill_pattern=%s\n", pat);
    passed++;

    // To lower
    char lower[64];
    str_to_lower(lower, buf);
    printf("to_lower=%s\n", lower);
    passed++;

    // Count vowels in "HELLO"
    char hello[6];
    hello[0]='H'; hello[1]='E'; hello[2]='L'; hello[3]='L'; hello[4]='O'; hello[5]=0;
    printf("count_vowels=%d\n", count_vowels(hello));
    passed++;

    // Replace char
    char replaced[64];
    replace_char(replaced, buf, 'O', 'X');
    printf("replace_char=%s\n", replaced);
    passed++;

    // Palindrome check
    char pal[10];
    pal[0]='r'; pal[1]='a'; pal[2]='c'; pal[3]='e'; pal[4]='c';
    pal[5]='a'; pal[6]='r'; pal[7]=0;
    printf("is_palindrome=%d\n", is_palindrome(pal));
    passed++;

    // Build number string
    char numstr[20];
    int_to_str(numstr, 12345);
    printf("build_num=%s\n", numstr);
    passed++;

    passed++; // for final line
    printf("string_tests_passed=%d\n", passed);

    return 0;
}
