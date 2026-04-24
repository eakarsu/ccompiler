int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int find_first(char *s, int ch) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == ch) return i;
        i = i + 1;
    }
    return -1;
}

int find_last(char *s, int ch) {
    int len = my_strlen(s);
    int i = len - 1;
    while (i >= 0) {
        if (s[i] == ch) return i;
        i = i - 1;
    }
    return -1;
}

int count_char(char *s, int ch) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == ch) count = count + 1;
        i = i + 1;
    }
    return count;
}

int contains_char(char *s, int ch) {
    return find_first(s, ch) >= 0 ? 1 : 0;
}

int find_nth(char *s, int ch, int n) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == ch) {
            count = count + 1;
            if (count == n) return i;
        }
        i = i + 1;
    }
    return -1;
}

int find_any(char *s, char *chars) {
    int i = 0;
    while (s[i] != 0) {
        int j = 0;
        while (chars[j] != 0) {
            if (s[i] == chars[j]) return i;
            j = j + 1;
        }
        i = i + 1;
    }
    return -1;
}

int count_any(char *s, char *chars) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        int j = 0;
        while (chars[j] != 0) {
            if (s[i] == chars[j]) {
                count = count + 1;
                break;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return count;
}

void find_all(char *s, int ch, int *positions, int *out_count) {
    int count = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == ch) {
            positions[count] = i;
            count = count + 1;
        }
        i = i + 1;
    }
    *out_count = count;
}

int main(void) {
    char str[] = "hello world";

    // EXPECT: first_l=2
    printf("first_l=%d\n", find_first(str, 'l'));

    // EXPECT: last_l=9
    printf("last_l=%d\n", find_last(str, 'l'));

    // EXPECT: count_l=3
    printf("count_l=%d\n", count_char(str, 'l'));

    // EXPECT: first_o=4
    printf("first_o=%d\n", find_first(str, 'o'));

    // EXPECT: last_o=7
    printf("last_o=%d\n", find_last(str, 'o'));

    // EXPECT: count_o=2
    printf("count_o=%d\n", count_char(str, 'o'));

    // EXPECT: contains_w=1
    printf("contains_w=%d\n", contains_char(str, 'w'));

    // EXPECT: contains_z=0
    printf("contains_z=%d\n", contains_char(str, 'z'));

    // EXPECT: first_sp=5
    printf("first_sp=%d\n", find_first(str, ' '));

    // EXPECT: last_sp=5
    printf("last_sp=%d\n", find_last(str, ' '));

    // EXPECT: find_x=-1
    printf("find_x=%d\n", find_first(str, 'x'));

    // EXPECT: nth_l_1=2
    printf("nth_l_1=%d\n", find_nth(str, 'l', 1));

    // EXPECT: nth_l_2=3
    printf("nth_l_2=%d\n", find_nth(str, 'l', 2));

    // EXPECT: nth_l_3=9
    printf("nth_l_3=%d\n", find_nth(str, 'l', 3));

    // EXPECT: nth_l_4=-1
    printf("nth_l_4=%d\n", find_nth(str, 'l', 4));

    char vowels[] = "aeiou";
    // EXPECT: find_vowel=1
    printf("find_vowel=%d\n", find_any(str, vowels));

    // EXPECT: count_vowels=3
    printf("count_vowels=%d\n", count_any(str, vowels));

    int positions[16];
    int n;
    find_all(str, 'l', positions, &n);
    // EXPECT: all_l_count=3
    printf("all_l_count=%d\n", n);

    // EXPECT: all_l_0=2
    printf("all_l_0=%d\n", positions[0]);

    // EXPECT: all_l_1=3
    printf("all_l_1=%d\n", positions[1]);

    // EXPECT: all_l_2=9
    printf("all_l_2=%d\n", positions[2]);

    char xyz[] = "xyz";
    // EXPECT: find_any_xyz=-1
    printf("find_any_xyz=%d\n", find_any("abc", xyz));

    char empty[] = "";
    // EXPECT: empty_find=-1
    printf("empty_find=%d\n", find_first(empty, 'a'));

    // EXPECT: empty_count=0
    printf("empty_count=%d\n", count_char(empty, 'a'));

    return 0;
}
