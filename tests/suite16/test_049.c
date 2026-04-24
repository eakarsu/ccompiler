int printf(const char *fmt, ...);

// String Hashing - polynomial hashing and substring operations

int str_len(char *s) {
    int i = 0;
    while (s[i] != 0) {
        i = i + 1;
    }
    return i;
}

// Polynomial hash of a string
int poly_hash(char *s, int n, int base, int mod) {
    int h = 0;
    int i = 0;
    while (i < n) {
        h = ((h * base) % mod + s[i]) % mod;
        i = i + 1;
    }
    return h;
}

// Precompute prefix hashes for all positions
void prefix_hashes(char *s, int n, int base, int mod, int *hashes) {
    hashes[0] = 0;
    int i = 0;
    while (i < n) {
        hashes[i + 1] = ((hashes[i] * base) % mod + s[i]) % mod;
        i = i + 1;
    }
}

// Precompute powers of base
void precompute_powers(int base, int mod, int n, int *powers) {
    powers[0] = 1;
    int i = 1;
    while (i <= n) {
        powers[i] = (powers[i - 1] * base) % mod;
        i = i + 1;
    }
}

// Get hash of substring s[l..r] using precomputed values
int substr_hash(int *hashes, int *powers, int l, int r, int mod) {
    int len = r - l + 1;
    int term = (hashes[l] * powers[len]) % mod;
    int h = ((hashes[r + 1] - term) % mod + mod) % mod;
    return h;
}

// Count distinct substrings of length k
int count_distinct_k(char *s, int n, int k) {
    int base = 31;
    int mod = 100003;
    int hashes[22];
    int powers[22];
    prefix_hashes(s, n, base, mod, hashes);
    precompute_powers(base, mod, n, powers);

    // Store seen hashes in an array (simple approach)
    int seen[100];
    int nseen = 0;
    int i = 0;
    while (i <= n - k) {
        int h = substr_hash(hashes, powers, i, i + k - 1, mod);
        // Check if already seen
        int found = 0;
        int j = 0;
        while (j < nseen) {
            if (seen[j] == h) {
                found = 1;
                j = nseen; // break
            }
            j = j + 1;
        }
        if (found == 0) {
            seen[nseen] = h;
            nseen = nseen + 1;
        }
        i = i + 1;
    }
    return nseen;
}

// Check if two substrings are equal using direct hashing
int substr_equal_hash(char *s, int n, int i1, int i2, int len) {
    int base = 31;
    int mod = 100003;
    int h1 = poly_hash(s + i1, len, base, mod);
    int h2 = poly_hash(s + i2, len, base, mod);
    return h1 == h2;
}

void test_basic_hash() {
    char s1[4]; char s2[4];
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]=0;
    s2[0]='a'; s2[1]='b'; s2[2]='c'; s2[3]=0;
    int h1 = poly_hash(s1, 3, 31, 1000003);
    int h2 = poly_hash(s2, 3, 31, 1000003);
    printf("Same hash: %d\n", h1 == h2); // EXPECT: Same hash: 1

    char s3[4];
    s3[0]='x'; s3[1]='y'; s3[2]='z'; s3[3]=0;
    int h3 = poly_hash(s3, 3, 31, 1000003);
    printf("Diff hash: %d\n", h1 != h3); // EXPECT: Diff hash: 1
}

void test_prefix_hashes() {
    char s[6];
    int hashes[7];
    int powers[7];
    s[0]='h'; s[1]='e'; s[2]='l'; s[3]='l'; s[4]='o'; s[5]=0;
    prefix_hashes(s, 5, 31, 100003, hashes);
    precompute_powers(31, 100003, 5, powers);
    printf("Prefix[0]: %d\n", hashes[0]); // EXPECT: Prefix[0]: 0

    // Verify substring hash matches direct hash
    int sh = substr_hash(hashes, powers, 0, 4, 100003);
    int dh = poly_hash(s, 5, 31, 100003);
    printf("Full match: %d\n", sh == dh); // EXPECT: Full match: 1

    // Verify substring "ell"
    char sub[4];
    sub[0]='e'; sub[1]='l'; sub[2]='l'; sub[3]=0;
    int sh2 = substr_hash(hashes, powers, 1, 3, 100003);
    int dh2 = poly_hash(sub, 3, 31, 100003);
    printf("Substr ell: %d\n", sh2 == dh2); // EXPECT: Substr ell: 1
}

void test_distinct_substrings() {
    char s1[6];
    s1[0]='a'; s1[1]='a'; s1[2]='a'; s1[3]='a'; s1[4]=0;
    int d1 = count_distinct_k(s1, 4, 2);
    printf("Distinct k=2 aaaa: %d\n", d1); // EXPECT: Distinct k=2 aaaa: 1

    char s2[6];
    s2[0]='a'; s2[1]='b'; s2[2]='c'; s2[3]='d'; s2[4]=0;
    int d2 = count_distinct_k(s2, 4, 2);
    printf("Distinct k=2 abcd: %d\n", d2); // EXPECT: Distinct k=2 abcd: 3

    char s3[8];
    s3[0]='a'; s3[1]='b'; s3[2]='a'; s3[3]='b'; s3[4]='a'; s3[5]=0;
    int d3 = count_distinct_k(s3, 5, 2);
    printf("Distinct k=2 ababa: %d\n", d3); // EXPECT: Distinct k=2 ababa: 2

    int d4 = count_distinct_k(s3, 5, 3);
    printf("Distinct k=3 ababa: %d\n", d4); // EXPECT: Distinct k=3 ababa: 2
}

void test_substr_equal() {
    char s[12];
    // "abcabc"
    s[0]='a'; s[1]='b'; s[2]='c'; s[3]='a'; s[4]='b'; s[5]='c'; s[6]=0;
    printf("Substr 0,3 len3: %d\n", substr_equal_hash(s, 6, 0, 3, 3)); // EXPECT: Substr 0,3 len3: 1
    printf("Substr 0,1 len3: %d\n", substr_equal_hash(s, 6, 0, 1, 3)); // EXPECT: Substr 0,1 len3: 0
}

void test_single_char_hash() {
    char s[2];
    s[0]='z'; s[1]=0;
    int h = poly_hash(s, 1, 31, 1000003);
    printf("Single hash z: %d\n", h); // EXPECT: Single hash z: 122
}

void test_empty_hash() {
    char s[2];
    s[0]=0;
    int h = poly_hash(s, 0, 31, 1000003);
    printf("Empty hash: %d\n", h); // EXPECT: Empty hash: 0
}

int main() {
    test_basic_hash();
    test_prefix_hashes();
    test_distinct_substrings();
    test_substr_equal();
    test_single_char_hash();
    test_empty_hash();
    printf("String hashing done\n"); // EXPECT: String hashing done
    return 0;
}
