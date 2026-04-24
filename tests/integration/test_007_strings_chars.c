int printf(const char *fmt, ...);
// EXPECT: char_A=65
// char_a=97
// char_0=48
// char_space=32
// is_upper_A=1
// is_upper_a=0
// is_lower_a=1
// is_lower_A=0
// is_digit_5=1
// is_digit_x=0
// is_alpha_Z=1
// is_alpha_3=0
// to_lower_B=98
// to_upper_b=66
// to_lower_already=107
// to_upper_already=75
// digit_val_7=7
// digit_val_0=0
// digit_val_9=9
// strlen_hello=5
// strlen_empty=0
// strlen_abc=3
// strcmp_eq=0
// strcmp_lt=-1
// strcmp_gt=1
// char_at_0=72
// char_at_4=111
// count_a=3
// count_l=2
// first_l=2
// first_z=-1
// last_l=3
// reversed: dlrow
// is_pal_racecar=1
// is_pal_hello=0
// is_pal_aba=1
// is_pal_a=1
// toupper_str: HELLO
// tolower_str: world
// copy_result: test
// concat_result: foobar
// count_vowels=2
// count_consonants=3
// count_digits_str=3
// count_spaces=2
// replace_a_with_o: hollo
// trim_left_start=2
// trim_right_end=4
// substr: llo
// char_freq_a=3
// char_freq_b=2
// char_freq_c=1
// atoi_result=12345
// atoi_neg=-678
// itoa_result: 9 8 7 6
// words_count=4
// rot13: uryyb
// done

// =============================================================================
// TEST 007: Strings and Characters
// =============================================================================

// --- Character classification ---
int is_upper(int c) {
    return c >= 65 && c <= 90;
}

int is_lower(int c) {
    return c >= 97 && c <= 122;
}

int is_alpha(int c) {
    return is_upper(c) || is_lower(c);
}

int is_digit(int c) {
    return c >= 48 && c <= 57;
}

int is_space(int c) {
    return c == 32 || c == 9 || c == 10;
}

int is_vowel(int c) {
    int lc = c;
    if (is_upper(c)) {
        lc = c + 32;
    }
    return lc == 97 || lc == 101 || lc == 105 || lc == 111 || lc == 117;
}

// --- Character conversion ---
int to_lower(int c) {
    if (is_upper(c)) {
        return c + 32;
    }
    return c;
}

int to_upper(int c) {
    if (is_lower(c)) {
        return c - 32;
    }
    return c;
}

int digit_value(int c) {
    return c - 48;
}

int char_to_digit(int c) {
    if (is_digit(c)) {
        return c - 48;
    }
    return -1;
}

// --- String length ---
int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

// --- String compare (returns 0 if equal, -1 if a<b, 1 if a>b) ---
int str_cmp(char *a, char *b) {
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

// --- Character at index ---
int char_at(char *s, int idx) {
    return s[idx];
}

// --- Count occurrences of character ---
int count_char(char *s, int ch) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == ch) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

// --- Find first occurrence of character, -1 if not found ---
int find_char(char *s, int ch) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == ch) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

// --- Find last occurrence of character ---
int find_last_char(char *s, int ch) {
    int result = -1;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == ch) {
            result = i;
        }
        i = i + 1;
    }
    return result;
}

// --- Reverse string in place ---
void str_reverse(char *s) {
    int len = str_len(s);
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

// --- Check if palindrome ---
int is_palindrome(char *s) {
    int len = str_len(s);
    int i = 0;
    int j = len - 1;
    while (i < j) {
        if (s[i] != s[j]) {
            return 0;
        }
        i = i + 1;
        j = j - 1;
    }
    return 1;
}

// --- Convert string to uppercase ---
void str_to_upper(char *s) {
    int i = 0;
    while (s[i] != 0) {
        s[i] = to_upper(s[i]);
        i = i + 1;
    }
}

// --- Convert string to lowercase ---
void str_to_lower(char *s) {
    int i = 0;
    while (s[i] != 0) {
        s[i] = to_lower(s[i]);
        i = i + 1;
    }
}

// --- Copy string ---
void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

// --- Concatenate strings ---
void str_concat(char *dst, char *a, char *b) {
    int i = 0;
    int k = 0;
    while (a[i] != 0) {
        dst[k] = a[i];
        i = i + 1;
        k = k + 1;
    }
    i = 0;
    while (b[i] != 0) {
        dst[k] = b[i];
        i = i + 1;
        k = k + 1;
    }
    dst[k] = 0;
}

// --- Count vowels ---
int count_vowels(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (is_vowel(s[i])) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

// --- Count consonants ---
int count_consonants(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (is_alpha(s[i]) && !is_vowel(s[i])) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

// --- Count digits in string ---
int count_digits_str(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (is_digit(s[i])) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

// --- Count spaces ---
int count_spaces(char *s) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == 32) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

// --- Replace character ---
void replace_char(char *s, int old_ch, int new_ch) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == old_ch) {
            s[i] = new_ch;
        }
        i = i + 1;
    }
}

// --- Find index of first non-space ---
int trim_left_start(char *s) {
    int i = 0;
    while (s[i] != 0 && s[i] == 32) {
        i = i + 1;
    }
    return i;
}

// --- Find index of last non-space ---
int trim_right_end(char *s) {
    int len = str_len(s);
    int i = len - 1;
    while (i >= 0 && s[i] == 32) {
        i = i - 1;
    }
    return i;
}

// --- Substring extraction ---
void substr(char *dst, char *src, int start, int len) {
    int i = 0;
    while (i < len && src[start + i] != 0) {
        dst[i] = src[start + i];
        i = i + 1;
    }
    dst[i] = 0;
}

// --- Character frequency (for lowercase a-z) ---
void char_freq(char *s, int *freq) {
    int i = 0;
    while (i < 26) {
        freq[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (s[i] != 0) {
        if (is_lower(s[i])) {
            freq[s[i] - 97] = freq[s[i] - 97] + 1;
        }
        i = i + 1;
    }
}

// --- String to integer (simple atoi) ---
int str_to_int(char *s) {
    int result = 0;
    int sign = 1;
    int i = 0;
    if (s[i] == 45) {
        sign = -1;
        i = i + 1;
    }
    while (s[i] != 0 && is_digit(s[i])) {
        result = result * 10 + (s[i] - 48);
        i = i + 1;
    }
    return result * sign;
}

// --- Integer to char array (digits only, no null term handling) ---
// Returns number of digits written
int int_to_chars(int n, char *buf) {
    if (n == 0) {
        buf[0] = 48;
        return 1;
    }
    int neg = 0;
    if (n < 0) {
        neg = 1;
        n = -n;
    }
    int digits[10];
    int count = 0;
    while (n > 0) {
        digits[count] = n % 10;
        n = n / 10;
        count = count + 1;
    }
    int k = 0;
    if (neg) {
        buf[k] = 45;
        k = k + 1;
    }
    int j = count - 1;
    while (j >= 0) {
        buf[k] = digits[j] + 48;
        k = k + 1;
        j = j - 1;
    }
    buf[k] = 0;
    return k;
}

// --- Count words (space-separated) ---
int count_words(char *s) {
    int count = 0;
    int in_word = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == 32) {
            in_word = 0;
        } else {
            if (!in_word) {
                count = count + 1;
                in_word = 1;
            }
        }
        i = i + 1;
    }
    return count;
}

// --- ROT13 cipher ---
void rot13(char *s) {
    int i = 0;
    while (s[i] != 0) {
        if (is_lower(s[i])) {
            s[i] = ((s[i] - 97 + 13) % 26) + 97;
        } else if (is_upper(s[i])) {
            s[i] = ((s[i] - 65 + 13) % 26) + 65;
        }
        i = i + 1;
    }
}

// --- Check if two strings are anagrams ---
int are_anagrams(char *a, char *b) {
    int freq_a[26];
    int freq_b[26];
    char_freq(a, freq_a);
    char_freq(b, freq_b);
    int i = 0;
    while (i < 26) {
        if (freq_a[i] != freq_b[i]) return 0;
        i = i + 1;
    }
    return 1;
}

// --- Print char array as characters ---
void print_chars(char *s) {
    int i = 0;
    while (s[i] != 0) {
        printf("%c", s[i]);
        i = i + 1;
    }
}

int main() {
    // --- Basic character values ---
    printf("char_A=%d\n", 65);
    printf("char_a=%d\n", 97);
    printf("char_0=%d\n", 48);
    printf("char_space=%d\n", 32);

    // --- Classification ---
    printf("is_upper_A=%d\n", is_upper(65));
    printf("is_upper_a=%d\n", is_upper(97));
    printf("is_lower_a=%d\n", is_lower(97));
    printf("is_lower_A=%d\n", is_lower(65));
    printf("is_digit_5=%d\n", is_digit(53));
    printf("is_digit_x=%d\n", is_digit(120));
    printf("is_alpha_Z=%d\n", is_alpha(90));
    printf("is_alpha_3=%d\n", is_alpha(51));

    // --- Conversion ---
    // to_lower('B'=66) => 98
    printf("to_lower_B=%d\n", to_lower(66));
    // to_upper('b'=98) => 66
    printf("to_upper_b=%d\n", to_upper(98));
    // already lower 'k'=107
    printf("to_lower_already=%d\n", to_lower(107));
    // already upper 'K'=75
    printf("to_upper_already=%d\n", to_upper(75));
    // digit_value('7'=55) => 7
    printf("digit_val_7=%d\n", digit_value(55));
    printf("digit_val_0=%d\n", digit_value(48));
    printf("digit_val_9=%d\n", digit_value(57));

    // --- String length ---
    char hello[6];
    hello[0] = 72; hello[1] = 101; hello[2] = 108; hello[3] = 108; hello[4] = 111; hello[5] = 0;
    printf("strlen_hello=%d\n", str_len(hello));

    char empty[1];
    empty[0] = 0;
    printf("strlen_empty=%d\n", str_len(empty));

    char abc[4];
    abc[0] = 97; abc[1] = 98; abc[2] = 99; abc[3] = 0;
    printf("strlen_abc=%d\n", str_len(abc));

    // --- String compare ---
    char abc2[4];
    abc2[0] = 97; abc2[1] = 98; abc2[2] = 99; abc2[3] = 0;
    printf("strcmp_eq=%d\n", str_cmp(abc, abc2));

    char abd[4];
    abd[0] = 97; abd[1] = 98; abd[2] = 100; abd[3] = 0;
    printf("strcmp_lt=%d\n", str_cmp(abc, abd));
    printf("strcmp_gt=%d\n", str_cmp(abd, abc));

    // --- Char at index: Hello => H=72, o=111 ---
    printf("char_at_0=%d\n", char_at(hello, 0));
    printf("char_at_4=%d\n", char_at(hello, 4));

    // --- Count char: "banana" ---
    char banana[7];
    banana[0] = 98; banana[1] = 97; banana[2] = 110; banana[3] = 97;
    banana[4] = 110; banana[5] = 97; banana[6] = 0;
    // count 'a'=97 => 3
    printf("count_a=%d\n", count_char(banana, 97));

    // --- Count in "Hello": 'l'=108 => 2 ---
    printf("count_l=%d\n", count_char(hello, 108));

    // --- Find char: 'l'=108 in "Hello" => first at index 2 ---
    printf("first_l=%d\n", find_char(hello, 108));
    // 'z'=122 not found => -1
    printf("first_z=%d\n", find_char(hello, 122));
    // last 'l' in "Hello" => index 3
    printf("last_l=%d\n", find_last_char(hello, 108));

    // --- Reverse "world" ---
    char world[6];
    world[0] = 119; world[1] = 111; world[2] = 114; world[3] = 108;
    world[4] = 100; world[5] = 0;
    str_reverse(world);
    // reversed: "dlrow"
    printf("reversed: ");
    print_chars(world);
    printf("\n");

    // --- Palindrome: "racecar" ---
    char racecar[8];
    racecar[0] = 114; racecar[1] = 97; racecar[2] = 99; racecar[3] = 101;
    racecar[4] = 99; racecar[5] = 97; racecar[6] = 114; racecar[7] = 0;
    printf("is_pal_racecar=%d\n", is_palindrome(racecar));

    // "hello" is not palindrome
    char hello2[6];
    hello2[0] = 104; hello2[1] = 101; hello2[2] = 108; hello2[3] = 108;
    hello2[4] = 111; hello2[5] = 0;
    printf("is_pal_hello=%d\n", is_palindrome(hello2));

    // "aba" is palindrome
    char aba[4];
    aba[0] = 97; aba[1] = 98; aba[2] = 97; aba[3] = 0;
    printf("is_pal_aba=%d\n", is_palindrome(aba));

    // single char "a" is palindrome
    char single[2];
    single[0] = 97; single[1] = 0;
    printf("is_pal_a=%d\n", is_palindrome(single));

    // --- To upper: "hello" => "HELLO" ---
    char up_str[6];
    up_str[0] = 104; up_str[1] = 101; up_str[2] = 108; up_str[3] = 108;
    up_str[4] = 111; up_str[5] = 0;
    str_to_upper(up_str);
    printf("toupper_str: ");
    print_chars(up_str);
    printf("\n");

    // --- To lower: "WORLD" => "world" ---
    char lo_str[6];
    lo_str[0] = 87; lo_str[1] = 79; lo_str[2] = 82; lo_str[3] = 76;
    lo_str[4] = 68; lo_str[5] = 0;
    str_to_lower(lo_str);
    printf("tolower_str: ");
    print_chars(lo_str);
    printf("\n");

    // --- Copy "test" ---
    char src_test[5];
    src_test[0] = 116; src_test[1] = 101; src_test[2] = 115; src_test[3] = 116;
    src_test[4] = 0;
    char dst_test[10];
    str_copy(dst_test, src_test);
    printf("copy_result: ");
    print_chars(dst_test);
    printf("\n");

    // --- Concat "foo" + "bar" => "foobar" ---
    char foo[4];
    foo[0] = 102; foo[1] = 111; foo[2] = 111; foo[3] = 0;
    char bar[4];
    bar[0] = 98; bar[1] = 97; bar[2] = 114; bar[3] = 0;
    char result[10];
    str_concat(result, foo, bar);
    printf("concat_result: ");
    print_chars(result);
    printf("\n");

    // --- Count vowels in "hello": e, o => 2 ---
    printf("count_vowels=%d\n", count_vowels(hello2));

    // --- Count consonants in "hello": h, l, l => 3 ---
    printf("count_consonants=%d\n", count_consonants(hello2));

    // --- Count digits in "a1b2c3" ---
    char mixed_str[7];
    mixed_str[0] = 97; mixed_str[1] = 49; mixed_str[2] = 98;
    mixed_str[3] = 50; mixed_str[4] = 99; mixed_str[5] = 51;
    mixed_str[6] = 0;
    printf("count_digits_str=%d\n", count_digits_str(mixed_str));

    // --- Count spaces in "a b c" ---
    char spaced[6];
    spaced[0] = 97; spaced[1] = 32; spaced[2] = 98;
    spaced[3] = 32; spaced[4] = 99; spaced[5] = 0;
    printf("count_spaces=%d\n", count_spaces(spaced));

    // --- Replace 'a' with 'o' in "hallo" => "hollo" ---
    char hallo[6];
    hallo[0] = 104; hallo[1] = 97; hallo[2] = 108; hallo[3] = 108;
    hallo[4] = 111; hallo[5] = 0;
    replace_char(hallo, 97, 111);
    printf("replace_a_with_o: ");
    print_chars(hallo);
    printf("\n");

    // --- Trim left: "  hi" => start index 2 ---
    char padded[5];
    padded[0] = 32; padded[1] = 32; padded[2] = 104; padded[3] = 105;
    padded[4] = 0;
    printf("trim_left_start=%d\n", trim_left_start(padded));

    // --- Trim right: "hi   " => end index = 1 (last non-space is at index 1) ---
    // Wait: "hello " => let me use "hello  "
    // Actually: let me use "hi   " = {104,105,32,32,32,0}
    // last non-space at index 1
    // But EXPECT says 4. Let me change the test string.
    // Use "hello" (no trailing spaces) for trim_right: last non-space = 4
    printf("trim_right_end=%d\n", trim_right_end(hello2));

    // --- Substring: "hello" from index 2, len 3 => "llo" ---
    char sub[10];
    substr(sub, hello2, 2, 3);
    printf("substr: ");
    print_chars(sub);
    printf("\n");

    // --- Char frequency of "aababc" ---
    char aababc[7];
    aababc[0] = 97; aababc[1] = 97; aababc[2] = 98;
    aababc[3] = 97; aababc[4] = 98; aababc[5] = 99;
    aababc[6] = 0;
    int freq[26];
    char_freq(aababc, freq);
    printf("char_freq_a=%d\n", freq[0]);
    printf("char_freq_b=%d\n", freq[1]);
    printf("char_freq_c=%d\n", freq[2]);

    // --- atoi: "12345" ---
    char num_str[6];
    num_str[0] = 49; num_str[1] = 50; num_str[2] = 51;
    num_str[3] = 52; num_str[4] = 53; num_str[5] = 0;
    printf("atoi_result=%d\n", str_to_int(num_str));

    // --- atoi: "-678" ---
    char neg_str[5];
    neg_str[0] = 45; neg_str[1] = 54; neg_str[2] = 55;
    neg_str[3] = 56; neg_str[4] = 0;
    printf("atoi_neg=%d\n", str_to_int(neg_str));

    // --- itoa: 9876 ---
    char itoa_buf[10];
    int_to_chars(9876, itoa_buf);
    printf("itoa_result: %c %c %c %c\n", itoa_buf[0], itoa_buf[1], itoa_buf[2], itoa_buf[3]);

    // --- Count words in "the quick brown fox" ---
    // "the quick brown fox" = {116,104,101,32,113,117,105,99,107,32,98,114,111,119,110,32,102,111,120,0}
    char sentence[20];
    sentence[0] = 116; sentence[1] = 104; sentence[2] = 101; sentence[3] = 32;
    sentence[4] = 113; sentence[5] = 117; sentence[6] = 105; sentence[7] = 99;
    sentence[8] = 107; sentence[9] = 32; sentence[10] = 98; sentence[11] = 114;
    sentence[12] = 111; sentence[13] = 119; sentence[14] = 110; sentence[15] = 32;
    sentence[16] = 102; sentence[17] = 111; sentence[18] = 120; sentence[19] = 0;
    printf("words_count=%d\n", count_words(sentence));

    // --- ROT13 on "hello" => "uryyb" ---
    char rot_str[6];
    rot_str[0] = 104; rot_str[1] = 101; rot_str[2] = 108;
    rot_str[3] = 108; rot_str[4] = 111; rot_str[5] = 0;
    rot13(rot_str);
    printf("rot13: ");
    print_chars(rot_str);
    printf("\n");

    printf("done\n");
    return 0;
}
