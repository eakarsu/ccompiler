int printf(const char *fmt, ...);

int rec_strlen(char *s) {
    if (s[0] == 0) return 0;
    return 1 + rec_strlen(s + 1);
}

int rec_strcmp(char *a, char *b) {
    if (a[0] == 0 && b[0] == 0) return 0;
    if (a[0] == 0) return -1;
    if (b[0] == 0) return 1;
    if (a[0] < b[0]) return -1;
    if (a[0] > b[0]) return 1;
    return rec_strcmp(a + 1, b + 1);
}

int is_palindrome_helper(char *s, int left, int right) {
    if (left >= right) return 1;
    if (s[left] != s[right]) return 0;
    return is_palindrome_helper(s, left + 1, right - 1);
}

int is_palindrome(char *s) {
    return is_palindrome_helper(s, 0, rec_strlen(s) - 1);
}

void reverse_string(char *s, int left, int right) {
    if (left >= right) return;
    char tmp = s[left];
    s[left] = s[right];
    s[right] = tmp;
    reverse_string(s, left + 1, right - 1);
}

int rec_strncmp(char *a, char *b, int n) {
    if (n == 0) return 0;
    if (a[0] == 0 && b[0] == 0) return 0;
    if (a[0] == 0) return -1;
    if (b[0] == 0) return 1;
    if (a[0] < b[0]) return -1;
    if (a[0] > b[0]) return 1;
    return rec_strncmp(a + 1, b + 1, n - 1);
}

int rec_count_char(char *s, char c) {
    if (s[0] == 0) return 0;
    int count = rec_count_char(s + 1, c);
    if (s[0] == c) count = count + 1;
    return count;
}

int rec_find_char(char *s, char c) {
    if (s[0] == 0) return -1;
    if (s[0] == c) return 0;
    int sub = rec_find_char(s + 1, c);
    if (sub == -1) return -1;
    return sub + 1;
}

void rec_copy(char *dst, char *src) {
    dst[0] = src[0];
    if (src[0] == 0) return;
    rec_copy(dst + 1, src + 1);
}

int rec_is_alpha(char c) {
    if (c >= 65 && c <= 90) return 1;
    if (c >= 97 && c <= 122) return 1;
    return 0;
}

int rec_count_alpha(char *s) {
    if (s[0] == 0) return 0;
    int count = rec_count_alpha(s + 1);
    if (rec_is_alpha(s[0])) count = count + 1;
    return count;
}

int rec_starts_with(char *s, char *prefix) {
    if (prefix[0] == 0) return 1;
    if (s[0] == 0) return 0;
    if (s[0] != prefix[0]) return 0;
    return rec_starts_with(s + 1, prefix + 1);
}

int rec_ends_with(char *s, char *suffix) {
    int slen = rec_strlen(s);
    int suflen = rec_strlen(suffix);
    if (suflen > slen) return 0;
    return rec_strcmp(s + slen - suflen, suffix) == 0 ? 1 : 0;
}

int main(void) {
    // EXPECT: strlen(hello)=5
    printf("strlen(hello)=%d\n", rec_strlen("hello"));
    // EXPECT: strlen()=0
    printf("strlen()=%d\n", rec_strlen(""));
    // EXPECT: strlen(abcdefghij)=10
    printf("strlen(abcdefghij)=%d\n", rec_strlen("abcdefghij"));

    // EXPECT: strcmp(abc,abd)=-1
    printf("strcmp(abc,abd)=%d\n", rec_strcmp("abc", "abd"));
    // EXPECT: strcmp(xyz,xyz)=0
    printf("strcmp(xyz,xyz)=%d\n", rec_strcmp("xyz", "xyz"));
    // EXPECT: strcmp(b,a)=1
    printf("strcmp(b,a)=%d\n", rec_strcmp("b", "a"));

    // EXPECT: pal(racecar)=1
    printf("pal(racecar)=%d\n", is_palindrome("racecar"));
    // EXPECT: pal(hello)=0
    printf("pal(hello)=%d\n", is_palindrome("hello"));
    // EXPECT: pal(abba)=1
    printf("pal(abba)=%d\n", is_palindrome("abba"));
    // EXPECT: pal(a)=1
    printf("pal(a)=%d\n", is_palindrome("a"));

    char buf1[20];
    buf1[0]='h'; buf1[1]='e'; buf1[2]='l'; buf1[3]='l'; buf1[4]='o';
    buf1[5]=0;
    reverse_string(buf1, 0, 4);
    // EXPECT: rev(hello)=olleh
    printf("rev(hello)=%s\n", buf1);

    char buf2[20];
    buf2[0]='a'; buf2[1]='b'; buf2[2]='c'; buf2[3]='d';
    buf2[4]='e'; buf2[5]='f'; buf2[6]=0;
    reverse_string(buf2, 0, 5);
    // EXPECT: rev(abcdef)=fedcba
    printf("rev(abcdef)=%s\n", buf2);

    // EXPECT: strncmp(abc,abd,2)=0
    printf("strncmp(abc,abd,2)=%d\n", rec_strncmp("abc", "abd", 2));
    // EXPECT: strncmp(abc,abd,3)=-1
    printf("strncmp(abc,abd,3)=%d\n", rec_strncmp("abc", "abd", 3));

    // EXPECT: count(l,hello world)=3
    printf("count(l,hello world)=%d\n", rec_count_char("hello world", 'l'));
    // EXPECT: count(z,hello)=0
    printf("count(z,hello)=%d\n", rec_count_char("hello", 'z'));

    // EXPECT: find(l,hello)=2
    printf("find(l,hello)=%d\n", rec_find_char("hello", 'l'));
    // EXPECT: find(z,hello)=-1
    printf("find(z,hello)=%d\n", rec_find_char("hello", 'z'));

    char dst[20];
    rec_copy(dst, "copy me");
    // EXPECT: copy=copy me
    printf("copy=%s\n", dst);

    // EXPECT: alpha(hello 123)=5
    printf("alpha(hello 123)=%d\n", rec_count_alpha("hello 123"));

    // EXPECT: starts(hello,hel)=1
    printf("starts(hello,hel)=%d\n", rec_starts_with("hello", "hel"));
    // EXPECT: starts(hello,xyz)=0
    printf("starts(hello,xyz)=%d\n", rec_starts_with("hello", "xyz"));

    // EXPECT: ends(hello,llo)=1
    printf("ends(hello,llo)=%d\n", rec_ends_with("hello", "llo"));
    // EXPECT: ends(hello,xyz)=0
    printf("ends(hello,xyz)=%d\n", rec_ends_with("hello", "xyz"));

    return 0;
}
