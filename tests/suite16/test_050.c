int printf(const char *fmt, ...);

// Longest Common Prefix, Z-algorithm, and string comparison utilities

int str_len(char *s) {
    int i = 0;
    while (s[i] != 0) {
        i = i + 1;
    }
    return i;
}

// Longest common prefix of two strings
int lcp(char *a, int na, char *b, int nb) {
    int i = 0;
    int limit = na;
    if (nb < limit) limit = nb;
    while (i < limit && a[i] == b[i]) {
        i = i + 1;
    }
    return i;
}

// Z-array: z[i] = length of longest substring starting at i
// that matches a prefix of the string
void z_function(char *s, int n, int *z) {
    z[0] = n;
    int l = 0;
    int r = 0;
    int i = 1;
    while (i < n) {
        if (i < r) {
            int k = z[i - l];
            if (k < r - i) {
                z[i] = k;
            } else {
                int j = r - i;
                while (i + j < n && s[j] == s[i + j]) {
                    j = j + 1;
                }
                z[i] = j;
                l = i;
                r = i + j;
            }
        } else {
            int j = 0;
            while (i + j < n && s[j] == s[i + j]) {
                j = j + 1;
            }
            z[i] = j;
            if (j > 0) {
                l = i;
                r = i + j;
            }
        }
        i = i + 1;
    }
}

// Use Z-function for pattern matching
int z_match(char *text, int tn, char *pat, int pn, int *results) {
    // Build "pattern$text" conceptually, but use combined array
    char combined[42];
    int ci = 0;
    int i = 0;
    while (i < pn) {
        combined[ci] = pat[i];
        ci = ci + 1;
        i = i + 1;
    }
    combined[ci] = '$';
    ci = ci + 1;
    i = 0;
    while (i < tn) {
        combined[ci] = text[i];
        ci = ci + 1;
        i = i + 1;
    }
    combined[ci] = 0;
    int total = ci;

    int z[42];
    z_function(combined, total, z);

    int count = 0;
    i = pn + 1;
    while (i < total) {
        if (z[i] == pn) {
            results[count] = i - pn - 1;
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

// Lexicographic comparison
int str_compare(char *a, char *b) {
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

// Find lexicographically smallest rotation
int smallest_rotation(char *s, int n) {
    int best = 0;
    char rot1[22];
    char rot2[22];
    int k = 1;
    while (k < n) {
        // Build rotation at 'best' and at 'k'
        int i = 0;
        while (i < n) {
            rot1[i] = s[(i + best) % n];
            rot2[i] = s[(i + k) % n];
            i = i + 1;
        }
        rot1[n] = 0;
        rot2[n] = 0;
        if (str_compare(rot2, rot1) < 0) {
            best = k;
        }
        k = k + 1;
    }
    return best;
}

void test_lcp_basic() {
    char a[8]; char b[8];
    a[0]='a'; a[1]='b'; a[2]='c'; a[3]='d'; a[4]=0;
    b[0]='a'; b[1]='b'; b[2]='x'; b[3]='y'; b[4]=0;
    printf("LCP abcd,abxy: %d\n", lcp(a, 4, b, 4)); // EXPECT: LCP abcd,abxy: 2

    b[0]='x'; b[1]='y'; b[2]='z'; b[3]=0;
    printf("LCP abcd,xyz: %d\n", lcp(a, 4, b, 3)); // EXPECT: LCP abcd,xyz: 0

    printf("LCP same: %d\n", lcp(a, 4, a, 4)); // EXPECT: LCP same: 4
}

void test_z_function() {
    char s[10];
    int z[10];
    // "aabxaab"
    s[0]='a'; s[1]='a'; s[2]='b'; s[3]='x';
    s[4]='a'; s[5]='a'; s[6]='b'; s[7]=0;
    z_function(s, 7, z);
    printf("Z: %d %d %d %d %d %d %d\n", z[0], z[1], z[2], z[3], z[4], z[5], z[6]); // EXPECT: Z: 7 1 0 0 3 1 0
}

void test_z_all_same() {
    char s[6];
    int z[6];
    s[0]='a'; s[1]='a'; s[2]='a'; s[3]='a'; s[4]=0;
    z_function(s, 4, z);
    printf("Z aaaa: %d %d %d %d\n", z[0], z[1], z[2], z[3]); // EXPECT: Z aaaa: 4 3 2 1
}

void test_z_match() {
    char text[12]; char pat[4];
    int results[10];
    // text: "ABCABCAB", pat: "ABC"
    text[0]='A'; text[1]='B'; text[2]='C'; text[3]='A';
    text[4]='B'; text[5]='C'; text[6]='A'; text[7]='B'; text[8]=0;
    pat[0]='A'; pat[1]='B'; pat[2]='C'; pat[3]=0;
    int c = z_match(text, 8, pat, 3, results);
    printf("Z-match: %d at %d %d\n", c, results[0], results[1]); // EXPECT: Z-match: 2 at 0 3
}

void test_str_compare() {
    char a[4]; char b[4];
    a[0]='a'; a[1]='b'; a[2]='c'; a[3]=0;
    b[0]='a'; b[1]='b'; b[2]='d'; b[3]=0;
    printf("abc < abd: %d\n", str_compare(a, b)); // EXPECT: abc < abd: -1

    b[0]='a'; b[1]='b'; b[2]='b'; b[3]=0;
    printf("abc > abb: %d\n", str_compare(a, b)); // EXPECT: abc > abb: 1

    b[0]='a'; b[1]='b'; b[2]='c'; b[3]=0;
    printf("abc == abc: %d\n", str_compare(a, b)); // EXPECT: abc == abc: 0

    char c[3];
    c[0]='a'; c[1]='b'; c[2]=0;
    printf("abc > ab: %d\n", str_compare(a, c)); // EXPECT: abc > ab: 1
}

void test_smallest_rotation() {
    char s[8];
    s[0]='c'; s[1]='a'; s[2]='b'; s[3]=0;
    int r = smallest_rotation(s, 3);
    printf("Smallest rot cab: %d\n", r); // EXPECT: Smallest rot cab: 1

    // "dcba" -> rotations: dcba, cbad, badc, adcb
    s[0]='d'; s[1]='c'; s[2]='b'; s[3]='a'; s[4]=0;
    r = smallest_rotation(s, 4);
    printf("Smallest rot dcba: %d\n", r); // EXPECT: Smallest rot dcba: 3

    // "abcd" already smallest
    s[0]='a'; s[1]='b'; s[2]='c'; s[3]='d'; s[4]=0;
    r = smallest_rotation(s, 4);
    printf("Smallest rot abcd: %d\n", r); // EXPECT: Smallest rot abcd: 0
}

void test_z_no_match() {
    char text[6]; char pat[4];
    int results[10];
    text[0]='X'; text[1]='Y'; text[2]='Z'; text[3]=0;
    pat[0]='A'; pat[1]='B'; pat[2]=0;
    int c = z_match(text, 3, pat, 2, results);
    printf("Z no match: %d\n", c); // EXPECT: Z no match: 0
}

int main() {
    test_lcp_basic();
    test_z_function();
    test_z_all_same();
    test_z_match();
    test_str_compare();
    test_smallest_rotation();
    test_z_no_match();
    printf("String utils done\n"); // EXPECT: String utils done
    return 0;
}
