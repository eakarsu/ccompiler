int printf(const char *fmt, ...);
// EXPECT: Searching 'GEEK' in 'GEEKS FOR GEEKS':\nMatch at 0\nMatch at 10\nTotal: 2\n\nSearching 'AABA' in 'AABAACAADAABAABA':\nMatch at 0\nMatch at 9\nMatch at 12\nTotal: 3\n\nSearching 'ABAB' in 'ABABABAB':\nMatch at 0\nMatch at 2\nMatch at 4\nTotal: 3\n\nHash 'hello': 99162322\nHash 'world': 113318802\nSame hash: 1
// Test: Rabin-Karp hash-based string matching

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n++;
    return n;
}

int rabin_karp(char *text, char *pat) {
    int n = str_len(text);
    int m = str_len(pat);
    int base = 31;
    int mod = 1000000007;
    int count = 0;
    int i, j;

    if (m > n) return 0;

    int pat_hash = 0;
    int text_hash = 0;
    int power = 1;

    for (i = 0; i < m - 1; i++) {
        power = (power * base) % mod;
    }

    for (i = 0; i < m; i++) {
        pat_hash = (pat_hash * base + pat[i]) % mod;
        text_hash = (text_hash * base + text[i]) % mod;
    }

    for (i = 0; i <= n - m; i++) {
        if (pat_hash == text_hash) {
            int match = 1;
            for (j = 0; j < m; j++) {
                if (text[i + j] != pat[j]) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                printf("Match at %d\n", i);
                count++;
            }
        }
        if (i < n - m) {
            text_hash = text_hash - (text[i] * power) % mod;
            if (text_hash < 0) text_hash = text_hash + mod;
            text_hash = (text_hash * base + text[i + m]) % mod;
        }
    }
    return count;
}

int simple_hash(char *s) {
    int h = 0;
    int i = 0;
    while (s[i] != 0) {
        h = (h * 31 + s[i]) % 1000000007;
        i++;
    }
    return h;
}

int main(void) {
    char t1[] = "GEEKS FOR GEEKS";
    char p1[] = "GEEK";
    printf("Searching '%s' in '%s':\n", p1, t1);
    int c1 = rabin_karp(t1, p1);
    printf("Total: %d\n\n", c1);

    char t2[] = "AABAACAADAABAABA";
    char p2[] = "AABA";
    printf("Searching '%s' in '%s':\n", p2, t2);
    int c2 = rabin_karp(t2, p2);
    printf("Total: %d\n\n", c2);

    char t3[] = "ABABABAB";
    char p3[] = "ABAB";
    printf("Searching '%s' in '%s':\n", p3, t3);
    int c3 = rabin_karp(t3, p3);
    printf("Total: %d\n\n", c3);

    printf("Hash 'hello': %d\n", simple_hash("hello"));
    printf("Hash 'world': %d\n", simple_hash("world"));
    printf("Same hash: %d\n", simple_hash("abc") == simple_hash("abc"));

    return 0;
}
