int printf(const char *fmt, ...);

// Rabin-Karp string matching with rolling hash

int str_len(char *s) {
    int i = 0;
    while (s[i] != 0) {
        i = i + 1;
    }
    return i;
}

int power_mod(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp / 2;
        base = (base * base) % mod;
    }
    return result;
}

int compute_hash(char *s, int len, int base, int mod) {
    int h = 0;
    int i = 0;
    while (i < len) {
        h = (h * base + s[i]) % mod;
        i = i + 1;
    }
    return h;
}

int str_equal(char *a, int astart, char *b, int blen) {
    int i = 0;
    while (i < blen) {
        if (a[astart + i] != b[i]) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

int rabin_karp(char *text, int tn, char *pat, int pn, int *results) {
    int base = 31;
    int mod = 1000003;
    int count = 0;

    int pat_hash = compute_hash(pat, pn, base, mod);
    int text_hash = compute_hash(text, pn, base, mod);
    int h = power_mod(base, pn - 1, mod);

    if (text_hash == pat_hash) {
        if (str_equal(text, 0, pat, pn)) {
            results[count] = 0;
            count = count + 1;
        }
    }

    int i = 1;
    while (i <= tn - pn) {
        text_hash = text_hash - (text[i - 1] * h) % mod;
        if (text_hash < 0) {
            text_hash = text_hash + mod;
        }
        text_hash = (text_hash * base + text[i + pn - 1]) % mod;
        if (text_hash == pat_hash) {
            if (str_equal(text, i, pat, pn)) {
                results[count] = i;
                count = count + 1;
            }
        }
        i = i + 1;
    }
    return count;
}

void test_basic_search() {
    char text[20];
    char pat[4];
    int results[10];
    text[0]='A'; text[1]='B'; text[2]='C'; text[3]='A';
    text[4]='B'; text[5]='C'; text[6]='D'; text[7]=0;
    pat[0]='A'; pat[1]='B'; pat[2]='C'; pat[3]=0;
    int c = rabin_karp(text, 7, pat, 3, results);
    printf("Basic: %d at %d %d\n", c, results[0], results[1]); // EXPECT: Basic: 2 at 0 3
}

void test_no_match() {
    char text[8];
    char pat[4];
    int results[10];
    text[0]='H'; text[1]='E'; text[2]='L'; text[3]='L'; text[4]='O'; text[5]=0;
    pat[0]='X'; pat[1]='Y'; pat[2]=0;
    int c = rabin_karp(text, 5, pat, 2, results);
    printf("No match: %d\n", c); // EXPECT: No match: 0
}

void test_hash_function() {
    char s1[4];
    char s2[4];
    s1[0]='A'; s1[1]='B'; s1[2]='C'; s1[3]=0;
    s2[0]='A'; s2[1]='B'; s2[2]='C'; s2[3]=0;
    int h1 = compute_hash(s1, 3, 31, 1000003);
    int h2 = compute_hash(s2, 3, 31, 1000003);
    printf("Hash equal: %d\n", h1 == h2); // EXPECT: Hash equal: 1

    char s3[4];
    s3[0]='X'; s3[1]='Y'; s3[2]='Z'; s3[3]=0;
    int h3 = compute_hash(s3, 3, 31, 1000003);
    printf("Hash diff: %d\n", h1 != h3); // EXPECT: Hash diff: 1
}

void test_power_mod() {
    int r1 = power_mod(2, 10, 1000);
    printf("2^10 mod 1000: %d\n", r1); // EXPECT: 2^10 mod 1000: 24
    int r2 = power_mod(3, 5, 100);
    printf("3^5 mod 100: %d\n", r2); // EXPECT: 3^5 mod 100: 43
    int r3 = power_mod(31, 0, 1000003);
    printf("31^0: %d\n", r3); // EXPECT: 31^0: 1
}

void test_repeated_pattern() {
    char text[12];
    char pat[3];
    int results[10];
    text[0]='A'; text[1]='A'; text[2]='A'; text[3]='A';
    text[4]='A'; text[5]='A'; text[6]=0;
    pat[0]='A'; pat[1]='A'; pat[2]=0;
    int c = rabin_karp(text, 6, pat, 2, results);
    printf("Repeated: %d\n", c); // EXPECT: Repeated: 5
    printf("Positions: %d %d %d %d %d\n", results[0], results[1], results[2], results[3], results[4]); // EXPECT: Positions: 0 1 2 3 4
}

int main() {
    test_basic_search();
    test_no_match();
    test_hash_function();
    test_power_mod();
    test_repeated_pattern();
    printf("Rabin-Karp done\n"); // EXPECT: Rabin-Karp done
    return 0;
}
