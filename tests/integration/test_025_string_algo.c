int printf(const char *fmt, ...);
// EXPECT: 1:5\n2:0\n3:1\n4:101\n5:97\n6:98\n7:97\n8:1\n9:0\n10:1\n11:1\n12:0\n13:0\n14:1\n15:1\n16:1\n17:3\n18:2\n19:1\n20:0\n21:5\n22:2\n23:1\n24:1\n25:2\n26:6\n27:0\n28:-1\n29:3\n30:4\n31:3\n32:104\n33:72\n34:79\n35:1\n36:0\n37:1\n38:0\n39:3\n40:1\n41:1\n42:0\n43:0\n44:2\n45:4\n46:99\n47:100\n48:3\n49:98\n50:98\n51:99
// =============================================================================
// TEST 025: String Algorithms (char arrays)
// =============================================================================
// Tests string operations on char arrays: reverse, palindrome check,
// character frequency count, substring search (brute force), string compare,
// and more. All strings represented as char arrays with null terminator.
// =============================================================================

// -----------------------------------------------------------------------------
// String length (null-terminated char array)
// -----------------------------------------------------------------------------
int str_len(char *s) {
    int i;
    i = 0;
    while (s[i] != 0) {
        i = i + 1;
    }
    return i;
}

// -----------------------------------------------------------------------------
// String reverse in place
// -----------------------------------------------------------------------------
void str_reverse(char *s) {
    int i;
    int j;
    char tmp;
    i = 0;
    j = str_len(s) - 1;
    while (i < j) {
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i = i + 1;
        j = j - 1;
    }
}

// -----------------------------------------------------------------------------
// Check if string is palindrome
// Returns 1 if palindrome, 0 otherwise
// -----------------------------------------------------------------------------
int is_palindrome(char *s) {
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

// -----------------------------------------------------------------------------
// String compare: returns 0 if equal, <0 if s1<s2, >0 if s1>s2
// -----------------------------------------------------------------------------
int str_cmp(char *s1, char *s2) {
    int i;
    i = 0;
    while (s1[i] != 0 && s2[i] != 0) {
        if (s1[i] != s2[i]) return s1[i] - s2[i];
        i = i + 1;
    }
    return s1[i] - s2[i];
}

// -----------------------------------------------------------------------------
// String copy
// -----------------------------------------------------------------------------
void str_copy(char *dest, char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dest[i] = src[i];
        i = i + 1;
    }
    dest[i] = 0;
}

// -----------------------------------------------------------------------------
// String concatenate (appends src to end of dest)
// Assumes dest has enough space
// -----------------------------------------------------------------------------
void str_cat(char *dest, char *src) {
    int i;
    int j;
    i = str_len(dest);
    j = 0;
    while (src[j] != 0) {
        dest[i] = src[j];
        i = i + 1;
        j = j + 1;
    }
    dest[i] = 0;
}

// -----------------------------------------------------------------------------
// Count occurrences of character in string
// -----------------------------------------------------------------------------
int char_count(char *s, char c) {
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

// -----------------------------------------------------------------------------
// Character frequency: count frequency of each ASCII char (only lowercase a-z)
// Returns via freq array (index 0=a, 1=b, ..., 25=z)
// -----------------------------------------------------------------------------
int freq[26];

void char_frequency(char *s) {
    int i;
    for (i = 0; i < 26; i = i + 1) freq[i] = 0;
    i = 0;
    while (s[i] != 0) {
        if (s[i] >= 97 && s[i] <= 122) { // 'a' to 'z'
            freq[s[i] - 97] = freq[s[i] - 97] + 1;
        }
        i = i + 1;
    }
}

// -----------------------------------------------------------------------------
// Brute force substring search
// Returns index of first occurrence, or -1 if not found
// -----------------------------------------------------------------------------
int str_find(char *text, char *pattern) {
    int tlen;
    int plen;
    int i;
    int j;
    int found;
    tlen = str_len(text);
    plen = str_len(pattern);
    if (plen == 0) return 0;
    if (plen > tlen) return -1;
    for (i = 0; i <= tlen - plen; i = i + 1) {
        found = 1;
        for (j = 0; j < plen; j = j + 1) {
            if (text[i + j] != pattern[j]) {
                found = 0;
                break;
            }
        }
        if (found) return i;
    }
    return -1;
}

// -----------------------------------------------------------------------------
// Count occurrences of pattern in text
// -----------------------------------------------------------------------------
int str_count_pattern(char *text, char *pattern) {
    int tlen;
    int plen;
    int i;
    int j;
    int count;
    int found;
    tlen = str_len(text);
    plen = str_len(pattern);
    if (plen == 0) return 0;
    count = 0;
    for (i = 0; i <= tlen - plen; i = i + 1) {
        found = 1;
        for (j = 0; j < plen; j = j + 1) {
            if (text[i + j] != pattern[j]) {
                found = 0;
                break;
            }
        }
        if (found) count = count + 1;
    }
    return count;
}

// -----------------------------------------------------------------------------
// Convert lowercase to uppercase (in place)
// -----------------------------------------------------------------------------
void to_upper(char *s) {
    int i;
    i = 0;
    while (s[i] != 0) {
        if (s[i] >= 97 && s[i] <= 122) {
            s[i] = s[i] - 32;
        }
        i = i + 1;
    }
}

// -----------------------------------------------------------------------------
// Convert uppercase to lowercase (in place)
// -----------------------------------------------------------------------------
void to_lower(char *s) {
    int i;
    i = 0;
    while (s[i] != 0) {
        if (s[i] >= 65 && s[i] <= 90) {
            s[i] = s[i] + 32;
        }
        i = i + 1;
    }
}

// -----------------------------------------------------------------------------
// Check if string contains only digits
// -----------------------------------------------------------------------------
int is_all_digits(char *s) {
    int i;
    i = 0;
    if (s[0] == 0) return 0; // empty
    while (s[i] != 0) {
        if (s[i] < 48 || s[i] > 57) return 0; // '0'=48, '9'=57
        i = i + 1;
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Check if string contains only letters (a-z, A-Z)
// -----------------------------------------------------------------------------
int is_all_alpha(char *s) {
    int i;
    i = 0;
    if (s[0] == 0) return 0;
    while (s[i] != 0) {
        if (!((s[i] >= 65 && s[i] <= 90) || (s[i] >= 97 && s[i] <= 122))) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Count words (separated by spaces, ASCII 32)
// -----------------------------------------------------------------------------
int count_words(char *s) {
    int i;
    int in_word;
    int count;
    count = 0;
    in_word = 0;
    i = 0;
    while (s[i] != 0) {
        if (s[i] == 32) {
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

// -----------------------------------------------------------------------------
// Char at index
// -----------------------------------------------------------------------------
int char_at(char *s, int idx) {
    return s[idx];
}

// -----------------------------------------------------------------------------
// Check if two strings are anagrams (lowercase only)
// -----------------------------------------------------------------------------
int are_anagrams(char *s1, char *s2) {
    int f1[26];
    int f2[26];
    int i;
    for (i = 0; i < 26; i = i + 1) {
        f1[i] = 0;
        f2[i] = 0;
    }
    i = 0;
    while (s1[i] != 0) {
        if (s1[i] >= 97 && s1[i] <= 122) {
            f1[s1[i] - 97] = f1[s1[i] - 97] + 1;
        }
        i = i + 1;
    }
    i = 0;
    while (s2[i] != 0) {
        if (s2[i] >= 97 && s2[i] <= 122) {
            f2[s2[i] - 97] = f2[s2[i] - 97] + 1;
        }
        i = i + 1;
    }
    for (i = 0; i < 26; i = i + 1) {
        if (f1[i] != f2[i]) return 0;
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Remove all occurrences of a character
// Returns new length
// -----------------------------------------------------------------------------
int remove_char(char *s, char c) {
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
    return j;
}

// =============================================================================
// Main test function
// =============================================================================
int main(void) {
    int test_num;
    int v;
    char buf1[50];
    char buf2[50];
    char buf3[50];

    test_num = 0;

    // ---- String length tests ----
    buf1[0] = 104; buf1[1] = 101; buf1[2] = 108; buf1[3] = 108; buf1[4] = 111; buf1[5] = 0; // "hello"
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, str_len(buf1)); // 5

    buf2[0] = 0; // empty
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, str_len(buf2)); // 0

    buf2[0] = 97; buf2[1] = 0; // "a"
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, str_len(buf2)); // 1

    // ---- String reverse tests ----
    // "abcde" = 97,98,99,100,101
    buf1[0] = 97; buf1[1] = 98; buf1[2] = 99; buf1[3] = 100; buf1[4] = 101; buf1[5] = 0;
    str_reverse(buf1);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, buf1[0]); // 101 ('e')

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, buf1[4]); // 97 ('a')

    // "ab" reversed
    buf1[0] = 97; buf1[1] = 98; buf1[2] = 0;
    str_reverse(buf1);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, buf1[0]); // 98 ('b')

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, buf1[1]); // 97 ('a')

    // ---- Palindrome tests ----
    // "racecar" = 114,97,99,101,99,97,114
    buf1[0]=114; buf1[1]=97; buf1[2]=99; buf1[3]=101; buf1[4]=99; buf1[5]=97; buf1[6]=114; buf1[7]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_palindrome(buf1)); // 1

    // "hello" - not palindrome
    buf1[0]=104; buf1[1]=101; buf1[2]=108; buf1[3]=108; buf1[4]=111; buf1[5]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_palindrome(buf1)); // 0

    // "aba"
    buf1[0]=97; buf1[1]=98; buf1[2]=97; buf1[3]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_palindrome(buf1)); // 1

    // "a"
    buf1[0]=97; buf1[1]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_palindrome(buf1)); // 1

    // "ab"
    buf1[0]=97; buf1[1]=98; buf1[2]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_palindrome(buf1)); // 0

    // ---- String compare tests ----
    // "abc" vs "abc"
    buf1[0]=97; buf1[1]=98; buf1[2]=99; buf1[3]=0;
    buf2[0]=97; buf2[1]=98; buf2[2]=99; buf2[3]=0;
    v = str_cmp(buf1, buf2);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, v); // 0

    // "abc" vs "abd" => negative
    buf2[2] = 100;
    v = str_cmp(buf1, buf2);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, v < 0 ? 1 : 0); // 1

    // "abd" vs "abc" => positive
    v = str_cmp(buf2, buf1);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, v > 0 ? 1 : 0); // 1

    // "ab" vs "abc" => negative (shorter)
    buf1[0]=97; buf1[1]=98; buf1[2]=0;
    buf2[0]=97; buf2[1]=98; buf2[2]=99; buf2[3]=0;
    v = str_cmp(buf1, buf2);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, v < 0 ? 1 : 0); // 1

    // ---- Char count tests ----
    // "aababc" = 97,97,98,97,98,99
    buf1[0]=97; buf1[1]=97; buf1[2]=98; buf1[3]=97; buf1[4]=98; buf1[5]=99; buf1[6]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, char_count(buf1, 97)); // 3 (count of 'a')

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, char_count(buf1, 98)); // 2 (count of 'b')

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, char_count(buf1, 99)); // 1 (count of 'c')

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, char_count(buf1, 100)); // 0 (count of 'd')

    // ---- Character frequency tests ----
    // "abracadabra"
    buf1[0]=97; buf1[1]=98; buf1[2]=114; buf1[3]=97; buf1[4]=99;
    buf1[5]=97; buf1[6]=100; buf1[7]=97; buf1[8]=98; buf1[9]=114;
    buf1[10]=97; buf1[11]=0;
    char_frequency(buf1);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, freq[0]); // 5 (a)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, freq[1]); // 2 (b)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, freq[2]); // 1 (c)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, freq[3]); // 1 (d)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, freq[17]); // 2 (r)

    // ---- Substring search tests ----
    // "hello world" = 104,101,108,108,111,32,119,111,114,108,100
    buf1[0]=104; buf1[1]=101; buf1[2]=108; buf1[3]=108; buf1[4]=111;
    buf1[5]=32; buf1[6]=119; buf1[7]=111; buf1[8]=114; buf1[9]=108;
    buf1[10]=100; buf1[11]=0;

    // Find "world" = 119,111,114,108,100
    buf2[0]=119; buf2[1]=111; buf2[2]=114; buf2[3]=108; buf2[4]=100; buf2[5]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, str_find(buf1, buf2)); // 6

    // Find "hell" = 104,101,108,108
    buf2[0]=104; buf2[1]=101; buf2[2]=108; buf2[3]=108; buf2[4]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, str_find(buf1, buf2)); // 0

    // Find "xyz" - not present
    buf2[0]=120; buf2[1]=121; buf2[2]=122; buf2[3]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, str_find(buf1, buf2)); // -1

    // Find "lo" = 108,111
    buf2[0]=108; buf2[1]=111; buf2[2]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, str_find(buf1, buf2)); // 3

    // ---- Pattern count tests ----
    // "abababab" - count "ab"
    buf1[0]=97; buf1[1]=98; buf1[2]=97; buf1[3]=98;
    buf1[4]=97; buf1[5]=98; buf1[6]=97; buf1[7]=98; buf1[8]=0;
    buf2[0]=97; buf2[1]=98; buf2[2]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, str_count_pattern(buf1, buf2)); // 4

    // Count "aba" in "abababab"
    buf2[0]=97; buf2[1]=98; buf2[2]=97; buf2[3]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, str_count_pattern(buf1, buf2)); // 3

    // ---- Case conversion tests ----
    // "Hello" = 72,101,108,108,111
    buf1[0]=72; buf1[1]=101; buf1[2]=108; buf1[3]=108; buf1[4]=111; buf1[5]=0;
    to_lower(buf1);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, buf1[0]); // 104 ('h')

    // "hello" to upper
    to_upper(buf1);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, buf1[0]); // 72 ('H')

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, buf1[4]); // 79 ('O')

    // ---- Digit/alpha checks ----
    // "12345"
    buf1[0]=49; buf1[1]=50; buf1[2]=51; buf1[3]=52; buf1[4]=53; buf1[5]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_all_digits(buf1)); // 1

    // "123a5"
    buf1[3] = 97;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_all_digits(buf1)); // 0

    // "Hello"
    buf1[0]=72; buf1[1]=101; buf1[2]=108; buf1[3]=108; buf1[4]=111; buf1[5]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_all_alpha(buf1)); // 1

    // "He1lo"
    buf1[2] = 49;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_all_alpha(buf1)); // 0

    // ---- Word count ----
    // "hello world foo" = 104,101,108,108,111,32,119,111,114,108,100,32,102,111,111
    buf1[0]=104; buf1[1]=101; buf1[2]=108; buf1[3]=108; buf1[4]=111;
    buf1[5]=32; buf1[6]=119; buf1[7]=111; buf1[8]=114; buf1[9]=108;
    buf1[10]=100; buf1[11]=32; buf1[12]=102; buf1[13]=111; buf1[14]=111;
    buf1[15]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_words(buf1)); // 3

    // Single word "test"
    buf1[0]=116; buf1[1]=101; buf1[2]=115; buf1[3]=116; buf1[4]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_words(buf1)); // 1

    // ---- Anagram tests ----
    // "listen" and "silent"
    buf1[0]=108; buf1[1]=105; buf1[2]=115; buf1[3]=116; buf1[4]=101; buf1[5]=110; buf1[6]=0;
    buf2[0]=115; buf2[1]=105; buf2[2]=108; buf2[3]=101; buf2[4]=110; buf2[5]=116; buf2[6]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, are_anagrams(buf1, buf2)); // 1

    // "abc" and "abd" - not anagrams
    buf1[0]=97; buf1[1]=98; buf1[2]=99; buf1[3]=0;
    buf2[0]=97; buf2[1]=98; buf2[2]=100; buf2[3]=0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, are_anagrams(buf1, buf2)); // 0

    // ---- String copy test ----
    buf1[0]=104; buf1[1]=105; buf1[2]=0; // "hi"
    str_copy(buf2, buf1);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, str_cmp(buf1, buf2)); // 0 (equal)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, str_len(buf2)); // 2

    // ---- String concat test ----
    buf1[0]=97; buf1[1]=98; buf1[2]=0; // "ab"
    buf2[0]=99; buf2[1]=100; buf2[2]=0; // "cd"
    str_cat(buf1, buf2);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, str_len(buf1)); // 4

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, buf1[2]); // 99 ('c')

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, buf1[3]); // 100 ('d')

    // ---- Remove char test ----
    // "aababc" remove 'a' (97)
    buf1[0]=97; buf1[1]=97; buf1[2]=98; buf1[3]=97; buf1[4]=98; buf1[5]=99; buf1[6]=0;
    v = remove_char(buf1, 97);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, v); // 3 (new length "bbc")

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, buf1[0]); // 98 ('b')

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, buf1[1]); // 98 ('b')

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, buf1[2]); // 99 ('c')

    return 0;
}
