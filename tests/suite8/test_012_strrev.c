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

void reverse_in_place(char *s) {
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

void reverse_copy(char *dst, char *src) {
    int len = my_strlen(src);
    int i = 0;
    while (i < len) {
        dst[i] = src[len - 1 - i];
        i = i + 1;
    }
    dst[len] = 0;
}

void reverse_words(char *dst, char *src) {
    int len = my_strlen(src);
    my_strcpy(dst, src);
    int start = 0;
    int i = 0;
    while (i <= len) {
        if (i == len || dst[i] == ' ') {
            int a = start;
            int b = i - 1;
            while (a < b) {
                char tmp = dst[a];
                dst[a] = dst[b];
                dst[b] = tmp;
                a = a + 1;
                b = b - 1;
            }
            start = i + 1;
        }
        i = i + 1;
    }
}

void reverse_recursive(char *s, int left, int right) {
    if (left >= right) return;
    char tmp = s[left];
    s[left] = s[right];
    s[right] = tmp;
    reverse_recursive(s, left + 1, right - 1);
}

int is_palindrome(char *s) {
    int len = my_strlen(s);
    int i = 0;
    while (i < len / 2) {
        if (s[i] != s[len - 1 - i]) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

void rotate_left(char *s, int n) {
    int len = my_strlen(s);
    if (len == 0) return;
    n = n % len;
    char temp[256];
    int i = 0;
    while (i < len) {
        temp[i] = s[(i + n) % len];
        i = i + 1;
    }
    i = 0;
    while (i < len) {
        s[i] = temp[i];
        i = i + 1;
    }
    s[len] = 0;
}

int main() {
    char buf1[64];
    char buf2[64];

    my_strcpy(buf1, "hello");
    reverse_in_place(buf1);
    // EXPECT: reverse hello = olleh
    printf("reverse hello = %s\n", buf1);

    my_strcpy(buf1, "abcd");
    reverse_in_place(buf1);
    // EXPECT: reverse abcd = dcba
    printf("reverse abcd = %s\n", buf1);

    my_strcpy(buf1, "a");
    reverse_in_place(buf1);
    // EXPECT: reverse a = a
    printf("reverse a = %s\n", buf1);

    reverse_copy(buf2, "world");
    // EXPECT: reverse_copy world = dlrow
    printf("reverse_copy world = %s\n", buf2);

    reverse_copy(buf2, "12345");
    // EXPECT: reverse_copy 12345 = 54321
    printf("reverse_copy 12345 = %s\n", buf2);

    reverse_words(buf2, "hello world");
    // EXPECT: reverse_words = olleh dlrow
    printf("reverse_words = %s\n", buf2);

    reverse_words(buf2, "abc def ghi");
    // EXPECT: reverse_words2 = cba fed ihg
    printf("reverse_words2 = %s\n", buf2);

    my_strcpy(buf1, "racecar");
    reverse_recursive(buf1, 0, my_strlen(buf1) - 1);
    // EXPECT: recursive rev racecar = racecar
    printf("recursive rev racecar = %s\n", buf1);

    my_strcpy(buf1, "test");
    reverse_recursive(buf1, 0, my_strlen(buf1) - 1);
    // EXPECT: recursive rev test = tset
    printf("recursive rev test = %s\n", buf1);

    // EXPECT: palindrome racecar = 1
    printf("palindrome racecar = %d\n", is_palindrome("racecar"));
    // EXPECT: palindrome hello = 0
    printf("palindrome hello = %d\n", is_palindrome("hello"));
    // EXPECT: palindrome aba = 1
    printf("palindrome aba = %d\n", is_palindrome("aba"));
    // EXPECT: palindrome ab = 0
    printf("palindrome ab = %d\n", is_palindrome("ab"));

    my_strcpy(buf1, "abcdef");
    rotate_left(buf1, 2);
    // EXPECT: rotate abcdef by 2 = cdefab
    printf("rotate abcdef by 2 = %s\n", buf1);

    my_strcpy(buf1, "hello");
    rotate_left(buf1, 1);
    // EXPECT: rotate hello by 1 = elloh
    printf("rotate hello by 1 = %s\n", buf1);

    return 0;
}
