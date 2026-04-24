int printf(const char *fmt, ...);

// Anagram checking and related operations

int str_len(char *s) {
    int i = 0;
    while (s[i] != 0) {
        i = i + 1;
    }
    return i;
}

// Check if two strings are anagrams using character frequency counting
int is_anagram(char *a, int na, char *b, int nb) {
    if (na != nb) return 0;

    int freq[128];
    int i = 0;
    while (i < 128) {
        freq[i] = 0;
        i = i + 1;
    }

    i = 0;
    while (i < na) {
        freq[(int)a[i]] = freq[(int)a[i]] + 1;
        freq[(int)b[i]] = freq[(int)b[i]] - 1;
        i = i + 1;
    }

    i = 0;
    while (i < 128) {
        if (freq[i] != 0) return 0;
        i = i + 1;
    }
    return 1;
}

// Sort characters in a string (bubble sort) for canonical form
void sort_chars(char *s, int n, char *out) {
    int i = 0;
    while (i < n) {
        out[i] = s[i];
        i = i + 1;
    }
    out[n] = 0;

    i = 0;
    while (i < n - 1) {
        int j = 0;
        while (j < n - 1 - i) {
            if (out[j] > out[j + 1]) {
                char tmp = out[j];
                out[j] = out[j + 1];
                out[j + 1] = tmp;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

// Count anagram pairs in a group of words
// words stored as flat array with lengths
int count_anagram_pairs(char words[6][10], int lens[6], int nw) {
    int count = 0;
    int i = 0;
    while (i < nw) {
        int j = i + 1;
        while (j < nw) {
            if (is_anagram(words[i], lens[i], words[j], lens[j])) {
                count = count + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return count;
}

// Find minimum number of character changes to make anagram
int min_changes_anagram(char *a, int na, char *b, int nb) {
    if (na != nb) return -1;

    int freq[128];
    int i = 0;
    while (i < 128) {
        freq[i] = 0;
        i = i + 1;
    }

    i = 0;
    while (i < na) {
        freq[(int)a[i]] = freq[(int)a[i]] + 1;
        i = i + 1;
    }
    i = 0;
    while (i < nb) {
        freq[(int)b[i]] = freq[(int)b[i]] - 1;
        i = i + 1;
    }

    int changes = 0;
    i = 0;
    while (i < 128) {
        if (freq[i] > 0) {
            changes = changes + freq[i];
        }
        i = i + 1;
    }
    return changes;
}

void test_basic_anagram() {
    char a[8]; char b[8];
    a[0]='l'; a[1]='i'; a[2]='s'; a[3]='t'; a[4]='e'; a[5]='n'; a[6]=0;
    b[0]='s'; b[1]='i'; b[2]='l'; b[3]='e'; b[4]='n'; b[5]='t'; b[6]=0;
    printf("listen-silent: %d\n", is_anagram(a, 6, b, 6)); // EXPECT: listen-silent: 1
}

void test_not_anagram() {
    char a[4]; char b[4];
    a[0]='a'; a[1]='b'; a[2]='c'; a[3]=0;
    b[0]='a'; b[1]='b'; b[2]='d'; b[3]=0;
    printf("abc-abd: %d\n", is_anagram(a, 3, b, 3)); // EXPECT: abc-abd: 0
}

void test_diff_length() {
    char a[4]; char b[5];
    a[0]='a'; a[1]='b'; a[2]=0;
    b[0]='a'; b[1]='b'; b[2]='c'; b[3]=0;
    printf("Diff len: %d\n", is_anagram(a, 2, b, 3)); // EXPECT: Diff len: 0
}

void test_sort_canonical() {
    char s[8]; char out[8];
    s[0]='d'; s[1]='c'; s[2]='b'; s[3]='a'; s[4]=0;
    sort_chars(s, 4, out);
    printf("Sorted dcba: %s\n", out); // EXPECT: Sorted dcba: abcd

    char s2[8]; char out2[8];
    s2[0]='l'; s2[1]='i'; s2[2]='s'; s2[3]='t'; s2[4]='e'; s2[5]='n'; s2[6]=0;
    sort_chars(s2, 6, out2);
    printf("Sorted listen: %s\n", out2); // EXPECT: Sorted listen: eilnst

    char s3[8]; char out3[8];
    s3[0]='s'; s3[1]='i'; s3[2]='l'; s3[3]='e'; s3[4]='n'; s3[5]='t'; s3[6]=0;
    sort_chars(s3, 6, out3);
    printf("Sorted silent: %s\n", out3); // EXPECT: Sorted silent: eilnst
}

void test_anagram_pairs() {
    char words[6][10];
    int lens[6];

    // "abc", "bca", "xyz", "cab", "zyx", "def"
    words[0][0]='a'; words[0][1]='b'; words[0][2]='c'; words[0][3]=0; lens[0]=3;
    words[1][0]='b'; words[1][1]='c'; words[1][2]='a'; words[1][3]=0; lens[1]=3;
    words[2][0]='x'; words[2][1]='y'; words[2][2]='z'; words[2][3]=0; lens[2]=3;
    words[3][0]='c'; words[3][1]='a'; words[3][2]='b'; words[3][3]=0; lens[3]=3;
    words[4][0]='z'; words[4][1]='y'; words[4][2]='x'; words[4][3]=0; lens[4]=3;
    words[5][0]='d'; words[5][1]='e'; words[5][2]='f'; words[5][3]=0; lens[5]=3;

    int pairs = count_anagram_pairs(words, lens, 6);
    printf("Anagram pairs: %d\n", pairs); // EXPECT: Anagram pairs: 4
}

void test_min_changes() {
    char a[4]; char b[4];
    a[0]='a'; a[1]='b'; a[2]='c'; a[3]=0;
    b[0]='b'; b[1]='c'; b[2]='d'; b[3]=0;
    printf("Changes abc->bcd: %d\n", min_changes_anagram(a, 3, b, 3)); // EXPECT: Changes abc->bcd: 1

    char c[4]; char d[4];
    c[0]='a'; c[1]='a'; c[2]='a'; c[3]=0;
    d[0]='b'; d[1]='b'; d[2]='b'; d[3]=0;
    printf("Changes aaa->bbb: %d\n", min_changes_anagram(c, 3, d, 3)); // EXPECT: Changes aaa->bbb: 3

    printf("Changes abc->abc: %d\n", min_changes_anagram(a, 3, a, 3)); // EXPECT: Changes abc->abc: 0
}

void test_single_char() {
    char a[2]; char b[2];
    a[0]='x'; a[1]=0;
    b[0]='x'; b[1]=0;
    printf("Same single: %d\n", is_anagram(a, 1, b, 1)); // EXPECT: Same single: 1
    b[0]='y';
    printf("Diff single: %d\n", is_anagram(a, 1, b, 1)); // EXPECT: Diff single: 0
}

int main() {
    test_basic_anagram();
    test_not_anagram();
    test_diff_length();
    test_sort_canonical();
    test_anagram_pairs();
    test_min_changes();
    test_single_char();
    printf("Anagram done\n"); // EXPECT: Anagram done
    return 0;
}
