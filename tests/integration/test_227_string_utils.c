int printf(const char *fmt, ...);
// EXPECT: len(hello)=5\nlen()=0\neq(abc,abc)=1\neq(abc,abd)=0\neq(ab,abc)=0\ncopy: hello\nreverse: olleh\nfind(hello,l)=2\nfind(hello,z)=-1\ncount(mississippi,s)=4\npalindrome(racecar)=1\npalindrome(hello)=0\npalindrome(aba)=1\nupper: HELLO WORLD\nlower: hello world\natoi(123)=123\natoi(-45)=-45\natoi(0)=0
// Test 227: Helper function library - string utilities

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len = len + 1;
    return len;
}

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

void str_reverse(char *s) {
    int len = str_len(s);
    int i = 0;
    int j = len - 1;
    while (i < j) {
        char t = s[i];
        s[i] = s[j];
        s[j] = t;
        i = i + 1;
        j = j - 1;
    }
}

int str_find(char *haystack, char needle) {
    int i = 0;
    while (haystack[i] != 0) {
        if (haystack[i] == needle) return i;
        i = i + 1;
    }
    return -1;
}

int str_count_char(char *s, char c) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == c) count = count + 1;
        i = i + 1;
    }
    return count;
}

int is_palindrome(char *s) {
    int len = str_len(s);
    int i = 0;
    int j = len - 1;
    while (i < j) {
        if (s[i] != s[j]) return 0;
        i = i + 1;
        j = j - 1;
    }
    return 1;
}

void str_to_upper(char *s) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] = s[i] - 32;
        }
        i = i + 1;
    }
}

void str_to_lower(char *s) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] + 32;
        }
        i = i + 1;
    }
}

int str_is_digit(char c) {
    return c >= '0' && c <= '9';
}

int str_to_int(char *s) {
    int result = 0;
    int neg = 0;
    int i = 0;
    if (s[0] == '-') { neg = 1; i = 1; }
    while (s[i] != 0 && str_is_digit(s[i])) {
        result = result * 10 + (s[i] - '0');
        i = i + 1;
    }
    if (neg) return -result;
    return result;
}

int main(void) {
    printf("len(hello)=%d\n", str_len("hello"));
    printf("len()=%d\n", str_len(""));

    printf("eq(abc,abc)=%d\n", str_equal("abc", "abc"));
    printf("eq(abc,abd)=%d\n", str_equal("abc", "abd"));
    printf("eq(ab,abc)=%d\n", str_equal("ab", "abc"));

    char buf[50];
    str_copy(buf, "hello");
    printf("copy: %s\n", buf);

    str_reverse(buf);
    printf("reverse: %s\n", buf);

    printf("find(hello,l)=%d\n", str_find("hello", 'l'));
    printf("find(hello,z)=%d\n", str_find("hello", 'z'));

    printf("count(mississippi,s)=%d\n", str_count_char("mississippi", 's'));

    printf("palindrome(racecar)=%d\n", is_palindrome("racecar"));
    printf("palindrome(hello)=%d\n", is_palindrome("hello"));
    printf("palindrome(aba)=%d\n", is_palindrome("aba"));

    str_copy(buf, "Hello World");
    str_to_upper(buf);
    printf("upper: %s\n", buf);
    str_to_lower(buf);
    printf("lower: %s\n", buf);

    printf("atoi(123)=%d\n", str_to_int("123"));
    printf("atoi(-45)=%d\n", str_to_int("-45"));
    printf("atoi(0)=%d\n", str_to_int("0"));

    return 0;
}
