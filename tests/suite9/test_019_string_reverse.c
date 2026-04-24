int printf(const char *fmt, ...);

// Recursive string operations

int str_len(char *s) {
    if (s[0] == 0) return 0;
    return 1 + str_len(s + 1);
}

void reverse_helper(char *s, int left, int right) {
    char tmp;
    if (left >= right) return;
    tmp = s[left];
    s[left] = s[right];
    s[right] = tmp;
    reverse_helper(s, left + 1, right - 1);
}

void reverse_string(char *s) {
    int len;
    len = str_len(s);
    if (len <= 1) return;
    reverse_helper(s, 0, len - 1);
}

int is_palindrome(char *s, int left, int right) {
    if (left >= right) return 1;
    if (s[left] != s[right]) return 0;
    return is_palindrome(s, left + 1, right - 1);
}

int str_compare(char *a, char *b) {
    if (a[0] == 0 && b[0] == 0) return 0;
    if (a[0] == 0) return -1;
    if (b[0] == 0) return 1;
    if (a[0] < b[0]) return -1;
    if (a[0] > b[0]) return 1;
    return str_compare(a + 1, b + 1);
}

void str_copy(char *dst, char *src) {
    if (src[0] == 0) {
        dst[0] = 0;
        return;
    }
    dst[0] = src[0];
    str_copy(dst + 1, src + 1);
}

int count_char(char *s, char c) {
    if (s[0] == 0) return 0;
    if (s[0] == c) return 1 + count_char(s + 1, c);
    return count_char(s + 1, c);
}

int str_index_of(char *s, char c, int pos) {
    if (s[pos] == 0) return -1;
    if (s[pos] == c) return pos;
    return str_index_of(s, c, pos + 1);
}

void to_upper(char *s) {
    if (s[0] == 0) return;
    if (s[0] >= 97 && s[0] <= 122) {
        s[0] = s[0] - 32;
    }
    to_upper(s + 1);
}

int power(int base, int exp) {
    if (exp == 0) return 1;
    return base * power(base, exp - 1);
}

int recursive_atoi(char *s, int index, int len) {
    int digit;
    if (index >= len) return 0;
    digit = s[index] - 48;
    return digit * power(10, len - index - 1) + recursive_atoi(s, index + 1, len);
}

int main() {
    char buf[64];
    int len;
    int pal;
    int cmp;
    int cnt;
    int idx;
    int val;

    // Test str_len
    len = str_len("hello");
    // EXPECT: len("hello") = 5
    printf("len(\"hello\") = %d\n", len);

    len = str_len("");
    // EXPECT: len("") = 0
    printf("len(\"\") = %d\n", len);

    // Test reverse
    str_copy(buf, "abcdef");
    reverse_string(buf);
    // EXPECT: reverse("abcdef") = fedcba
    printf("reverse(\"abcdef\") = %s\n", buf);

    str_copy(buf, "a");
    reverse_string(buf);
    // EXPECT: reverse("a") = a
    printf("reverse(\"a\") = %s\n", buf);

    str_copy(buf, "abba");
    reverse_string(buf);
    // EXPECT: reverse("abba") = abba
    printf("reverse(\"abba\") = %s\n", buf);

    // Test palindrome
    pal = is_palindrome("racecar", 0, 6);
    // EXPECT: palindrome("racecar") = 1
    printf("palindrome(\"racecar\") = %d\n", pal);

    pal = is_palindrome("hello", 0, 4);
    // EXPECT: palindrome("hello") = 0
    printf("palindrome(\"hello\") = %d\n", pal);

    pal = is_palindrome("abcba", 0, 4);
    // EXPECT: palindrome("abcba") = 1
    printf("palindrome(\"abcba\") = %d\n", pal);

    // Test str_compare
    cmp = str_compare("abc", "abd");
    // EXPECT: compare("abc","abd") = -1
    printf("compare(\"abc\",\"abd\") = %d\n", cmp);

    cmp = str_compare("abc", "abc");
    // EXPECT: compare("abc","abc") = 0
    printf("compare(\"abc\",\"abc\") = %d\n", cmp);

    cmp = str_compare("abd", "abc");
    // EXPECT: compare("abd","abc") = 1
    printf("compare(\"abd\",\"abc\") = %d\n", cmp);

    // Test count_char
    cnt = count_char("mississippi", 's');
    // EXPECT: count 's' in "mississippi" = 4
    printf("count 's' in \"mississippi\" = %d\n", cnt);

    cnt = count_char("aaa", 'a');
    // EXPECT: count 'a' in "aaa" = 3
    printf("count 'a' in \"aaa\" = %d\n", cnt);

    // Test str_index_of
    idx = str_index_of("hello world", 'w', 0);
    // EXPECT: index of 'w' in "hello world" = 6
    printf("index of 'w' in \"hello world\" = %d\n", idx);

    idx = str_index_of("hello", 'z', 0);
    // EXPECT: index of 'z' in "hello" = -1
    printf("index of 'z' in \"hello\" = %d\n", idx);

    // Test to_upper
    str_copy(buf, "hello");
    to_upper(buf);
    // EXPECT: upper("hello") = HELLO
    printf("upper(\"hello\") = %s\n", buf);

    // Test recursive_atoi
    val = recursive_atoi("12345", 0, 5);
    // EXPECT: atoi("12345") = 12345
    printf("atoi(\"12345\") = %d\n", val);

    val = recursive_atoi("007", 0, 3);
    // EXPECT: atoi("007") = 7
    printf("atoi(\"007\") = %d\n", val);

    // EXPECT: String reverse done
    printf("String reverse done\n");

    return 0;
}
