int printf(const char *fmt, ...);
// EXPECT: 5 0 3 1 0 1 0 1 1 0 1 3 0 4 4 3 0 3 1 111 104 1 0 1 3 0 1 -1 1 0 2 2 -1 0 99 98 72 79 2 0 2 5 2 98 293 97 1 0 3 2

// String operations: rotation check, anagram detection, longest common prefix,
// substring generation (small), string manipulation using char arrays

// String utilities operating on char arrays with null terminator

int str_len(char s[]) {
    int i;
    i = 0;
    while (s[i] != 0) {
        i = i + 1;
    }
    return i;
}

int str_equal(char a[], char b[]) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    if (a[i] != b[i]) return 0;
    return 1;
}

void str_copy(char dest[], char src[]) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dest[i] = src[i];
        i = i + 1;
    }
    dest[i] = 0;
}

// Check if s2 is a rotation of s1
// A rotation means s2 can be found in s1+s1
int is_rotation(char s1[], char s2[]) {
    int len1;
    int len2;
    char doubled[200];
    int i;
    int j;
    int k;
    int match;
    len1 = str_len(s1);
    len2 = str_len(s2);
    if (len1 != len2) return 0;
    if (len1 == 0) return 1;
    // Create s1+s1 in doubled
    for (i = 0; i < len1; i = i + 1) {
        doubled[i] = s1[i];
    }
    for (i = 0; i < len1; i = i + 1) {
        doubled[len1 + i] = s1[i];
    }
    doubled[2 * len1] = 0;
    // Check if s2 is substring of doubled
    for (i = 0; i <= 2 * len1 - len2; i = i + 1) {
        match = 1;
        for (j = 0; j < len2; j = j + 1) {
            if (doubled[i + j] != s2[j]) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    return 0;
}

// Check if two strings are anagrams (using character count)
int is_anagram(char s1[], char s2[]) {
    int count[128];
    int i;
    int len1;
    int len2;
    len1 = str_len(s1);
    len2 = str_len(s2);
    if (len1 != len2) return 0;
    for (i = 0; i < 128; i = i + 1) {
        count[i] = 0;
    }
    for (i = 0; i < len1; i = i + 1) {
        count[s1[i]] = count[s1[i]] + 1;
    }
    for (i = 0; i < len2; i = i + 1) {
        count[s2[i]] = count[s2[i]] - 1;
    }
    for (i = 0; i < 128; i = i + 1) {
        if (count[i] != 0) return 0;
    }
    return 1;
}

// Longest common prefix length of two strings
int lcp_length(char s1[], char s2[]) {
    int i;
    i = 0;
    while (s1[i] != 0 && s2[i] != 0 && s1[i] == s2[i]) {
        i = i + 1;
    }
    return i;
}

// Count substrings of length k
int count_substrings(char s[], int k) {
    int len;
    len = str_len(s);
    if (k > len) return 0;
    return len - k + 1;
}

// Count distinct characters in string
int distinct_chars(char s[]) {
    int seen[128];
    int i;
    int count;
    for (i = 0; i < 128; i = i + 1) {
        seen[i] = 0;
    }
    count = 0;
    i = 0;
    while (s[i] != 0) {
        if (seen[s[i]] == 0) {
            count = count + 1;
            seen[s[i]] = 1;
        }
        i = i + 1;
    }
    return count;
}

// Reverse a string in place
void str_reverse(char s[]) {
    int i;
    int j;
    char t;
    i = 0;
    j = str_len(s) - 1;
    while (i < j) {
        t = s[i];
        s[i] = s[j];
        s[j] = t;
        i = i + 1;
        j = j - 1;
    }
}

// Check if string is palindrome
int is_palindrome(char s[]) {
    int i;
    int j;
    i = 0;
    j = str_len(s) - 1;
    while (i < j) {
        if (s[i] != s[j]) return 0;
        i = i + 1;
        j = j - 1;
    }
    return 1;
}

// Count occurrences of char c in string
int count_char(char s[], char c) {
    int i;
    int count;
    count = 0;
    i = 0;
    while (s[i] != 0) {
        if (s[i] == c) count = count + 1;
        i = i + 1;
    }
    return count;
}

// Find first occurrence of char c in string, return index or -1
int find_char(char s[], char c) {
    int i;
    i = 0;
    while (s[i] != 0) {
        if (s[i] == c) return i;
        i = i + 1;
    }
    return -1;
}

// Check if s2 is a substring of s1
int is_substring(char s1[], char s2[]) {
    int len1;
    int len2;
    int i;
    int j;
    int match;
    len1 = str_len(s1);
    len2 = str_len(s2);
    if (len2 == 0) return 1;
    if (len2 > len1) return 0;
    for (i = 0; i <= len1 - len2; i = i + 1) {
        match = 1;
        for (j = 0; j < len2; j = j + 1) {
            if (s1[i + j] != s2[j]) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    return 0;
}

// Count number of words (separated by spaces, char 32)
int count_words(char s[]) {
    int i;
    int count;
    int in_word;
    count = 0;
    in_word = 0;
    i = 0;
    while (s[i] != 0) {
        if (s[i] != 32) {
            if (in_word == 0) {
                count = count + 1;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        i = i + 1;
    }
    return count;
}

// Compare strings lexicographically: -1, 0, 1
int str_compare(char a[], char b[]) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
        i = i + 1;
    }
    if (a[i] == 0 && b[i] == 0) return 0;
    if (a[i] == 0) return -1;
    return 1;
}

// Left rotate string by k positions
void str_left_rotate(char s[], int k) {
    int len;
    char temp[200];
    int i;
    len = str_len(s);
    if (len == 0) return;
    k = k % len;
    for (i = 0; i < len; i = i + 1) {
        temp[i] = s[(i + k) % len];
    }
    temp[len] = 0;
    str_copy(s, temp);
}

// Right rotate string by k positions
void str_right_rotate(char s[], int k) {
    int len;
    len = str_len(s);
    if (len == 0) return;
    k = k % len;
    str_left_rotate(s, len - k);
}

// Convert lowercase to uppercase (add -32 if 'a' <= c <= 'z')
void to_upper(char s[]) {
    int i;
    i = 0;
    while (s[i] != 0) {
        if (s[i] >= 97 && s[i] <= 122) {
            s[i] = s[i] - 32;
        }
        i = i + 1;
    }
}

// Convert uppercase to lowercase
void to_lower(char s[]) {
    int i;
    i = 0;
    while (s[i] != 0) {
        if (s[i] >= 65 && s[i] <= 90) {
            s[i] = s[i] + 32;
        }
        i = i + 1;
    }
}

// Longest common prefix of multiple strings (represented as 2D char array)
int multi_lcp(char strs[10][50], int n) {
    int i;
    int j;
    int len;
    int ch;
    if (n == 0) return 0;
    len = 0;
    while (1) {
        ch = strs[0][len];
        if (ch == 0) break;
        for (i = 1; i < n; i = i + 1) {
            if (strs[i][len] != ch) return len;
        }
        len = len + 1;
    }
    return len;
}

// Count vowels (a, e, i, o, u and uppercase)
int count_vowels(char s[]) {
    int i;
    int count;
    char c;
    count = 0;
    i = 0;
    while (s[i] != 0) {
        c = s[i];
        if (c == 97 || c == 101 || c == 105 || c == 111 || c == 117) {
            count = count + 1;
        }
        if (c == 65 || c == 69 || c == 73 || c == 79 || c == 85) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

// Remove all occurrences of char c from string
void remove_char(char s[], char c) {
    int i;
    int j;
    j = 0;
    i = 0;
    while (s[i] != 0) {
        if (s[i] != c) {
            s[j] = s[i];
            j = j + 1;
        }
        i = i + 1;
    }
    s[j] = 0;
}

// Replace all occurrences of char old with new
void replace_char(char s[], char old_c, char new_c) {
    int i;
    i = 0;
    while (s[i] != 0) {
        if (s[i] == old_c) {
            s[i] = new_c;
        }
        i = i + 1;
    }
}

// String hash (simple: sum of chars * position)
int str_hash(char s[]) {
    int i;
    int h;
    h = 0;
    i = 0;
    while (s[i] != 0) {
        h = h + s[i] * (i + 1);
        i = i + 1;
    }
    return h;
}

// Check if all characters are unique
int all_unique(char s[]) {
    int seen[128];
    int i;
    for (i = 0; i < 128; i = i + 1) {
        seen[i] = 0;
    }
    i = 0;
    while (s[i] != 0) {
        if (seen[s[i]]) return 0;
        seen[s[i]] = 1;
        i = i + 1;
    }
    return 1;
}

// Longest substring without repeating characters
int longest_unique_substr(char s[]) {
    int seen[128];
    int i;
    int j;
    int mx;
    int len;
    len = str_len(s);
    mx = 0;
    for (i = 0; i < len; i = i + 1) {
        for (j = 0; j < 128; j = j + 1) {
            seen[j] = 0;
        }
        j = i;
        while (j < len && seen[s[j]] == 0) {
            seen[s[j]] = 1;
            j = j + 1;
        }
        if (j - i > mx) mx = j - i;
    }
    return mx;
}

// Number of matching characters in two strings at same positions
int matching_positions(char a[], char b[]) {
    int i;
    int count;
    count = 0;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] == b[i]) count = count + 1;
        i = i + 1;
    }
    return count;
}

int main(void) {
    char s1[100];
    char s2[100];
    char s3[100];
    char strs[10][50];
    int i;

    // Test 1-3: String length
    s1[0]='h'; s1[1]='e'; s1[2]='l'; s1[3]='l'; s1[4]='o'; s1[5]=0;
    printf("%d", str_len(s1)); // 5
    s1[0]=0;
    printf(" %d", str_len(s1)); // 0
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]=0;
    printf(" %d", str_len(s1)); // 3

    // Test 4-5: String equality
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]=0;
    s2[0]='a'; s2[1]='b'; s2[2]='c'; s2[3]=0;
    printf(" %d", str_equal(s1, s2)); // 1
    s2[2]='d'; s2[3]=0;
    printf(" %d", str_equal(s1, s2)); // 0

    // Test 6-8: Rotation check
    // "abcde" rotated is "cdeab"
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]='d'; s1[4]='e'; s1[5]=0;
    s2[0]='c'; s2[1]='d'; s2[2]='e'; s2[3]='a'; s2[4]='b'; s2[5]=0;
    printf(" %d", is_rotation(s1, s2)); // 1

    s2[0]='c'; s2[1]='d'; s2[2]='e'; s2[3]='b'; s2[4]='a'; s2[5]=0;
    printf(" %d", is_rotation(s1, s2)); // 0

    // "abc" and "bca"
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]=0;
    s2[0]='b'; s2[1]='c'; s2[2]='a'; s2[3]=0;
    printf(" %d", is_rotation(s1, s2)); // 1

    // Test 9-11: Anagram check
    // "listen" and "silent"
    s1[0]='l'; s1[1]='i'; s1[2]='s'; s1[3]='t'; s1[4]='e'; s1[5]='n'; s1[6]=0;
    s2[0]='s'; s2[1]='i'; s2[2]='l'; s2[3]='e'; s2[4]='n'; s2[5]='t'; s2[6]=0;
    printf(" %d", is_anagram(s1, s2)); // 1

    // "abc" and "abd"
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]=0;
    s2[0]='a'; s2[1]='b'; s2[2]='d'; s2[3]=0;
    printf(" %d", is_anagram(s1, s2)); // 0

    // "aab" and "baa"
    s1[0]='a'; s1[1]='a'; s1[2]='b'; s1[3]=0;
    s2[0]='b'; s2[1]='a'; s2[2]='a'; s2[3]=0;
    printf(" %d", is_anagram(s1, s2)); // 1

    // Test 12-14: LCP length
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]='d'; s1[4]=0;
    s2[0]='a'; s2[1]='b'; s2[2]='c'; s2[3]='e'; s2[4]=0;
    printf(" %d", lcp_length(s1, s2)); // 3

    s1[0]='x'; s1[1]='y'; s1[2]=0;
    s2[0]='a'; s2[1]='b'; s2[2]=0;
    printf(" %d", lcp_length(s1, s2)); // 0

    s1[0]='t'; s1[1]='e'; s1[2]='s'; s1[3]='t'; s1[4]=0;
    s2[0]='t'; s2[1]='e'; s2[2]='s'; s2[3]='t'; s2[4]=0;
    printf(" %d", lcp_length(s1, s2)); // 4

    // Test 15-17: Count substrings of length k
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]='d'; s1[4]='e'; s1[5]=0;
    printf(" %d", count_substrings(s1, 2)); // 4
    printf(" %d", count_substrings(s1, 3)); // 3
    printf(" %d", count_substrings(s1, 6)); // 0

    // Test 18-19: Distinct characters
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]='a'; s1[4]='b'; s1[5]=0;
    printf(" %d", distinct_chars(s1)); // 3
    s1[0]='a'; s1[1]='a'; s1[2]='a'; s1[3]=0;
    printf(" %d", distinct_chars(s1)); // 1

    // Test 20-21: Reverse
    s1[0]='h'; s1[1]='e'; s1[2]='l'; s1[3]='l'; s1[4]='o'; s1[5]=0;
    str_reverse(s1);
    // "olleh" -> o=111, l=108, l=108, e=101, h=104
    printf(" %d", s1[0]); // 111 ('o')
    printf(" %d", s1[4]); // 104 ('h')

    // Test 22-24: Palindrome
    s1[0]='a'; s1[1]='b'; s1[2]='a'; s1[3]=0;
    printf(" %d", is_palindrome(s1)); // 1
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]=0;
    printf(" %d", is_palindrome(s1)); // 0
    s1[0]='a'; s1[1]='b'; s1[2]='b'; s1[3]='a'; s1[4]=0;
    printf(" %d", is_palindrome(s1)); // 1

    // Test 25-26: Count char
    s1[0]='a'; s1[1]='b'; s1[2]='a'; s1[3]='c'; s1[4]='a'; s1[5]=0;
    printf(" %d", count_char(s1, 'a')); // 3
    printf(" %d", count_char(s1, 'z')); // 0

    // Test 27-28: Find char
    printf(" %d", find_char(s1, 'b')); // 1
    printf(" %d", find_char(s1, 'z')); // -1

    // Test 29-30: Substring check
    s1[0]='h'; s1[1]='e'; s1[2]='l'; s1[3]='l'; s1[4]='o'; s1[5]=0;
    s2[0]='e'; s2[1]='l'; s2[2]='l'; s2[3]=0;
    printf(" %d", is_substring(s1, s2)); // 1
    s2[0]='e'; s2[1]='o'; s2[2]=0;
    printf(" %d", is_substring(s1, s2)); // 0

    // Test 31-32: Word count
    s1[0]='h'; s1[1]='e'; s1[2]='l'; s1[3]='l'; s1[4]='o'; s1[5]=32;
    s1[6]='w'; s1[7]='o'; s1[8]='r'; s1[9]='l'; s1[10]='d'; s1[11]=0;
    printf(" %d", count_words(s1)); // 2

    s1[0]=32; s1[1]='a'; s1[2]=32; s1[3]='b'; s1[4]=32; s1[5]=0;
    printf(" %d", count_words(s1)); // 2

    // Test 33-34: String compare
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]=0;
    s2[0]='a'; s2[1]='b'; s2[2]='d'; s2[3]=0;
    printf(" %d", str_compare(s1, s2)); // -1
    s2[0]='a'; s2[1]='b'; s2[2]='c'; s2[3]=0;
    printf(" %d", str_compare(s1, s2)); // 0

    // Test 35-36: Left rotate
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]='d'; s1[4]='e'; s1[5]=0;
    str_left_rotate(s1, 2);
    // "cdeab" -> c=99
    printf(" %d", s1[0]); // 99 ('c')
    printf(" %d", s1[4]); // 98 ('b')

    // Test 37-38: To upper
    s1[0]='h'; s1[1]='e'; s1[2]='l'; s1[3]='l'; s1[4]='o'; s1[5]=0;
    to_upper(s1);
    // 'H'=72, 'E'=69, 'L'=76, 'L'=76, 'O'=79
    printf(" %d", s1[0]); // 72 ('H')
    printf(" %d", s1[4]); // 79 ('O')

    // Test 39-40: Multi-string LCP
    strs[0][0]='f'; strs[0][1]='l'; strs[0][2]='o'; strs[0][3]='w'; strs[0][4]=0;
    strs[1][0]='f'; strs[1][1]='l'; strs[1][2]='i'; strs[1][3]='g'; strs[1][4]=0;
    strs[2][0]='f'; strs[2][1]='l'; strs[2][2]='a'; strs[2][3]='g'; strs[2][4]=0;
    printf(" %d", multi_lcp(strs, 3)); // 2 ("fl")

    strs[0][0]='a'; strs[0][1]=0;
    strs[1][0]='b'; strs[1][1]=0;
    printf(" %d", multi_lcp(strs, 2)); // 0

    // Test 41-42: Vowel count
    s1[0]='h'; s1[1]='e'; s1[2]='l'; s1[3]='l'; s1[4]='o'; s1[5]=0;
    printf(" %d", count_vowels(s1)); // 2 (e, o)
    s1[0]='a'; s1[1]='e'; s1[2]='i'; s1[3]='o'; s1[4]='u'; s1[5]=0;
    printf(" %d", count_vowels(s1)); // 5

    // Test 43-44: Remove char
    s1[0]='a'; s1[1]='b'; s1[2]='a'; s1[3]='c'; s1[4]='a'; s1[5]=0;
    remove_char(s1, 'a');
    printf(" %d", str_len(s1)); // 2 ("bc")
    printf(" %d", s1[0]); // 98 ('b')

    // Test 45-46: String hash
    s1[0]='a'; s1[1]='b'; s1[2]=0; // 97*1 + 98*2 = 97 + 196 = 293
    printf(" %d", str_hash(s1)); // 293
    s1[0]='a'; s1[1]=0; // 97*1 = 97
    printf(" %d", str_hash(s1)); // 97

    // Test 47-48: All unique
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]=0;
    printf(" %d", all_unique(s1)); // 1
    s1[0]='a'; s1[1]='b'; s1[2]='a'; s1[3]=0;
    printf(" %d", all_unique(s1)); // 0

    // Test 49-50: Longest unique substring, matching positions
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]='a'; s1[4]='b'; s1[5]='c'; s1[6]='b'; s1[7]='b'; s1[8]=0;
    printf(" %d", longest_unique_substr(s1)); // 3 ("abc")

    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]=0;
    s2[0]='a'; s2[1]='x'; s2[2]='c'; s2[3]=0;
    printf(" %d", matching_positions(s1, s2)); // 2

    printf("\n");
    return 0;
}
