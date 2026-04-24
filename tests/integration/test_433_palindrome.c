int printf(const char *fmt, ...);
// EXPECT: Longest in 'babad': bab (len=3)\nLongest in 'cbbd': bb (len=2)\nLongest in 'forgeeksskeegfor': geeksskeeg (len=10)\nLongest in 'a': len=1\nPalindromic substrings in 'abc': 3\nPalindromic substrings in 'aaa': 6\nPalindromic substrings in 'abba': 6
// Test: Longest palindrome substring

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n++;
    return n;
}

int expand(char *s, int n, int left, int right) {
    while (left >= 0 && right < n && s[left] == s[right]) {
        left--;
        right++;
    }
    return right - left - 1;
}

void longest_palindrome(char *s, int *start, int *length) {
    int n = str_len(s);
    int best_start = 0;
    int best_len = 1;
    int i;

    for (i = 0; i < n; i++) {
        int len1 = expand(s, n, i, i);
        int len2 = expand(s, n, i, i + 1);
        int len = len1;
        if (len2 > len) len = len2;

        if (len > best_len) {
            best_len = len;
            best_start = i - (len - 1) / 2;
        }
    }
    *start = best_start;
    *length = best_len;
}

int is_palindrome(char *s, int left, int right) {
    while (left < right) {
        if (s[left] != s[right]) return 0;
        left++;
        right--;
    }
    return 1;
}

int count_palindromic_substrings(char *s) {
    int n = str_len(s);
    int count = 0;
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = i; j < n; j++) {
            if (is_palindrome(s, i, j)) count++;
        }
    }
    return count;
}

int main(void) {
    int start, length;
    int i;

    char s1[] = "babad";
    longest_palindrome(s1, &start, &length);
    printf("Longest in '%s': ", s1);
    for (i = start; i < start + length; i++) printf("%c", s1[i]);
    printf(" (len=%d)\n", length);

    char s2[] = "cbbd";
    longest_palindrome(s2, &start, &length);
    printf("Longest in '%s': ", s2);
    for (i = start; i < start + length; i++) printf("%c", s2[i]);
    printf(" (len=%d)\n", length);

    char s3[] = "forgeeksskeegfor";
    longest_palindrome(s3, &start, &length);
    printf("Longest in '%s': ", s3);
    for (i = start; i < start + length; i++) printf("%c", s3[i]);
    printf(" (len=%d)\n", length);

    char s4[] = "a";
    longest_palindrome(s4, &start, &length);
    printf("Longest in '%s': len=%d\n", s4, length);

    printf("Palindromic substrings in 'abc': %d\n", count_palindromic_substrings("abc"));
    printf("Palindromic substrings in 'aaa': %d\n", count_palindromic_substrings("aaa"));
    printf("Palindromic substrings in 'abba': %d\n", count_palindromic_substrings("abba"));

    return 0;
}
