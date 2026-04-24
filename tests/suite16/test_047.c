int printf(const char *fmt, ...);

// Palindrome Detection - various palindrome algorithms

int str_len(char *s) {
    int i = 0;
    while (s[i] != 0) {
        i = i + 1;
    }
    return i;
}

int is_palindrome(char *s, int n) {
    int left = 0;
    int right = n - 1;
    while (left < right) {
        if (s[left] != s[right]) return 0;
        left = left + 1;
        right = right - 1;
    }
    return 1;
}

// Find longest palindromic substring length centered at i
// (odd-length palindromes)
int expand_odd(char *s, int n, int center) {
    int left = center;
    int right = center;
    while (left >= 0 && right < n && s[left] == s[right]) {
        left = left - 1;
        right = right + 1;
    }
    return right - left - 1;
}

// Find longest palindromic substring length centered between i and i+1
// (even-length palindromes)
int expand_even(char *s, int n, int center) {
    int left = center;
    int right = center + 1;
    while (left >= 0 && right < n && s[left] == s[right]) {
        left = left - 1;
        right = right + 1;
    }
    return right - left - 1;
}

int max2(int a, int b) {
    if (a > b) return a;
    return b;
}

int longest_palindrome_len(char *s, int n) {
    int best = 0;
    int i = 0;
    while (i < n) {
        int odd = expand_odd(s, n, i);
        int even = expand_even(s, n, i);
        int cur = max2(odd, even);
        if (cur > best) best = cur;
        i = i + 1;
    }
    return best;
}

// Count total number of palindromic substrings
int count_palindromes(char *s, int n) {
    int count = 0;
    int i = 0;
    while (i < n) {
        // Odd-length palindromes centered at i
        int left = i;
        int right = i;
        while (left >= 0 && right < n && s[left] == s[right]) {
            count = count + 1;
            left = left - 1;
            right = right + 1;
        }
        // Even-length palindromes centered between i and i+1
        left = i;
        right = i + 1;
        while (left >= 0 && right < n && s[left] == s[right]) {
            count = count + 1;
            left = left - 1;
            right = right + 1;
        }
        i = i + 1;
    }
    return count;
}

// Min deletions to make palindrome (using LCS with reverse)
int min_deletions_palindrome(char *s, int n) {
    // LCS of s and reverse of s
    char rev[22];
    int i = 0;
    while (i < n) {
        rev[i] = s[n - 1 - i];
        i = i + 1;
    }
    rev[n] = 0;

    // DP for LCS
    int dp[441]; // 21*21
    i = 0;
    while (i <= n) {
        dp[i * (n + 1) + 0] = 0;
        i = i + 1;
    }
    int j = 0;
    while (j <= n) {
        dp[0 * (n + 1) + j] = 0;
        j = j + 1;
    }
    i = 1;
    while (i <= n) {
        j = 1;
        while (j <= n) {
            if (s[i - 1] == rev[j - 1]) {
                dp[i * (n + 1) + j] = dp[(i - 1) * (n + 1) + (j - 1)] + 1;
            } else {
                int a = dp[(i - 1) * (n + 1) + j];
                int b = dp[i * (n + 1) + (j - 1)];
                dp[i * (n + 1) + j] = max2(a, b);
            }
            j = j + 1;
        }
        i = i + 1;
    }
    int lps = dp[n * (n + 1) + n];
    return n - lps;
}

void test_basic_palindrome() {
    char s1[8];
    s1[0]='r'; s1[1]='a'; s1[2]='c'; s1[3]='e'; s1[4]='c'; s1[5]='a'; s1[6]='r'; s1[7]=0;
    printf("racecar: %d\n", is_palindrome(s1, 7)); // EXPECT: racecar: 1

    char s2[6];
    s2[0]='h'; s2[1]='e'; s2[2]='l'; s2[3]='l'; s2[4]='o'; s2[5]=0;
    printf("hello: %d\n", is_palindrome(s2, 5)); // EXPECT: hello: 0

    char s3[2];
    s3[0]='a'; s3[1]=0;
    printf("single: %d\n", is_palindrome(s3, 1)); // EXPECT: single: 1

    char s4[5];
    s4[0]='a'; s4[1]='b'; s4[2]='b'; s4[3]='a'; s4[4]=0;
    printf("abba: %d\n", is_palindrome(s4, 4)); // EXPECT: abba: 1
}

void test_longest_palindrome() {
    char s1[10];
    // "babad"
    s1[0]='b'; s1[1]='a'; s1[2]='b'; s1[3]='a'; s1[4]='d'; s1[5]=0;
    printf("Longest in babad: %d\n", longest_palindrome_len(s1, 5)); // EXPECT: Longest in babad: 3

    char s2[6];
    s2[0]='c'; s2[1]='b'; s2[2]='b'; s2[3]='d'; s2[4]=0;
    printf("Longest in cbbd: %d\n", longest_palindrome_len(s2, 4)); // EXPECT: Longest in cbbd: 2

    char s3[2];
    s3[0]='a'; s3[1]=0;
    printf("Longest in a: %d\n", longest_palindrome_len(s3, 1)); // EXPECT: Longest in a: 1
}

void test_count_palindromes() {
    char s1[4];
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]=0;
    printf("Count abc: %d\n", count_palindromes(s1, 3)); // EXPECT: Count abc: 3

    char s2[4];
    s2[0]='a'; s2[1]='a'; s2[2]='a'; s2[3]=0;
    printf("Count aaa: %d\n", count_palindromes(s2, 3)); // EXPECT: Count aaa: 6

    char s3[5];
    s3[0]='a'; s3[1]='b'; s3[2]='b'; s3[3]='a'; s3[4]=0;
    printf("Count abba: %d\n", count_palindromes(s3, 4)); // EXPECT: Count abba: 6
}

void test_min_deletions() {
    char s1[6];
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]='d'; s1[4]='a'; s1[5]=0;
    printf("Del abcda: %d\n", min_deletions_palindrome(s1, 5)); // EXPECT: Del abcda: 2

    char s2[8];
    s2[0]='r'; s2[1]='a'; s2[2]='c'; s2[3]='e'; s2[4]='c'; s2[5]='a'; s2[6]='r'; s2[7]=0;
    printf("Del racecar: %d\n", min_deletions_palindrome(s2, 7)); // EXPECT: Del racecar: 0
}

int main() {
    test_basic_palindrome();
    test_longest_palindrome();
    test_count_palindromes();
    test_min_deletions();
    printf("Palindrome done\n"); // EXPECT: Palindrome done
    return 0;
}
