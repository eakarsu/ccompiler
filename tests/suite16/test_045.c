int printf(const char *fmt, ...);

// Suffix Array Construction (naive O(n^2 log n) approach)
// and basic suffix array queries

int str_len(char *s) {
    int i = 0;
    while (s[i] != 0) {
        i = i + 1;
    }
    return i;
}

// Compare suffix starting at i with suffix starting at j in string s
int suffix_compare(char *s, int n, int i, int j) {
    while (i < n && j < n) {
        if (s[i] < s[j]) return -1;
        if (s[i] > s[j]) return 1;
        i = i + 1;
        j = j + 1;
    }
    if (i == n && j == n) return 0;
    if (i == n) return -1;
    return 1;
}

// Build suffix array using insertion sort
void build_suffix_array(char *s, int n, int *sa) {
    int i = 0;
    while (i < n) {
        sa[i] = i;
        i = i + 1;
    }

    // Insertion sort on suffix indices
    i = 1;
    while (i < n) {
        int key = sa[i];
        int j = i - 1;
        while (j >= 0 && suffix_compare(s, n, sa[j], key) > 0) {
            sa[j + 1] = sa[j];
            j = j - 1;
        }
        sa[j + 1] = key;
        i = i + 1;
    }
}

// Count number of suffixes starting with pattern
int count_pattern(char *s, int n, int *sa, char *pat, int pn) {
    int count = 0;
    int i = 0;
    while (i < n) {
        int idx = sa[i];
        int match = 1;
        int j = 0;
        while (j < pn) {
            if (idx + j >= n || s[idx + j] != pat[j]) {
                match = 0;
                j = pn; // break
            }
            j = j + 1;
        }
        if (match) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

void test_basic_sa() {
    char s[8];
    int sa[8];
    s[0]='b'; s[1]='a'; s[2]='n'; s[3]='a'; s[4]='n'; s[5]='a'; s[6]=0;
    int n = 6;
    build_suffix_array(s, n, sa);
    // Suffixes sorted: a(5), ana(3), anana(1), banana(0), na(4), nana(2)
    printf("SA: %d %d %d %d %d %d\n", sa[0], sa[1], sa[2], sa[3], sa[4], sa[5]); // EXPECT: SA: 5 3 1 0 4 2
}

void test_suffix_compare() {
    char s[8];
    s[0]='a'; s[1]='b'; s[2]='c'; s[3]='a'; s[4]='b'; s[5]=0;
    int r1 = suffix_compare(s, 5, 0, 3); // "abcab" vs "ab"
    printf("Compare 0,3: %d\n", r1); // EXPECT: Compare 0,3: 1

    int r2 = suffix_compare(s, 5, 3, 0); // "ab" vs "abcab"
    printf("Compare 3,0: %d\n", r2); // EXPECT: Compare 3,0: -1

    int r3 = suffix_compare(s, 5, 0, 0); // same
    printf("Compare 0,0: %d\n", r3); // EXPECT: Compare 0,0: 0
}

void test_all_same() {
    char s[6];
    int sa[6];
    s[0]='a'; s[1]='a'; s[2]='a'; s[3]='a'; s[4]=0;
    build_suffix_array(s, 4, sa);
    // Suffixes: a(3), aa(2), aaa(1), aaaa(0)
    printf("All same SA: %d %d %d %d\n", sa[0], sa[1], sa[2], sa[3]); // EXPECT: All same SA: 3 2 1 0
}

void test_already_sorted() {
    char s[6];
    int sa[6];
    s[0]='a'; s[1]='b'; s[2]='c'; s[3]='d'; s[4]=0;
    build_suffix_array(s, 4, sa);
    // a(0)<b(1)<bcd(1)<cd(2)<d(3) => abcd, bcd, cd, d
    printf("Sorted SA: %d %d %d %d\n", sa[0], sa[1], sa[2], sa[3]); // EXPECT: Sorted SA: 0 1 2 3
}

void test_reverse_sorted() {
    char s[6];
    int sa[6];
    s[0]='d'; s[1]='c'; s[2]='b'; s[3]='a'; s[4]=0;
    build_suffix_array(s, 4, sa);
    // Suffixes: a(3), ba(2), cba(1), dcba(0)
    printf("Reverse SA: %d %d %d %d\n", sa[0], sa[1], sa[2], sa[3]); // EXPECT: Reverse SA: 3 2 1 0
}

void test_pattern_count() {
    char s[8];
    int sa[8];
    char pat[3];
    s[0]='b'; s[1]='a'; s[2]='n'; s[3]='a'; s[4]='n'; s[5]='a'; s[6]=0;
    build_suffix_array(s, 6, sa);
    pat[0]='a'; pat[1]='n'; pat[2]=0;
    int c1 = count_pattern(s, 6, sa, pat, 2);
    printf("Count 'an': %d\n", c1); // EXPECT: Count 'an': 2

    pat[0]='n'; pat[1]='a'; pat[2]=0;
    int c2 = count_pattern(s, 6, sa, pat, 2);
    printf("Count 'na': %d\n", c2); // EXPECT: Count 'na': 2

    pat[0]='x'; pat[1]=0;
    int c3 = count_pattern(s, 6, sa, pat, 1);
    printf("Count 'x': %d\n", c3); // EXPECT: Count 'x': 0
}

void test_single_char() {
    char s[2];
    int sa[2];
    s[0]='z'; s[1]=0;
    build_suffix_array(s, 1, sa);
    printf("Single SA: %d\n", sa[0]); // EXPECT: Single SA: 0
}

int main() {
    test_basic_sa();
    test_suffix_compare();
    test_all_same();
    test_already_sorted();
    test_reverse_sorted();
    test_pattern_count();
    test_single_char();
    printf("Suffix array done\n"); // EXPECT: Suffix array done
    return 0;
}
