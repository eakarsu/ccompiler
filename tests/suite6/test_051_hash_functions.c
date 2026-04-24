int printf(const char *fmt, ...);

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int djb2_hash(char *s) {
    int hash = 5381;
    int i = 0;
    while (s[i] != 0) {
        hash = hash * 33 + s[i];
        i = i + 1;
    }
    if (hash < 0) hash = -hash;
    return hash;
}

int djb2_mod(char *s, int mod) {
    int h = djb2_hash(s);
    return h % mod;
}

int simple_mod_hash(char *s, int mod) {
    int sum = 0;
    int i = 0;
    while (s[i] != 0) {
        sum = sum + s[i];
        i = i + 1;
    }
    if (sum < 0) sum = -sum;
    return sum % mod;
}

int poly_hash(char *s, int base, int mod) {
    int hash = 0;
    int i = 0;
    int power = 1;
    while (s[i] != 0) {
        int val = s[i] * power;
        if (val < 0) val = -val;
        hash = (hash + val) % mod;
        power = (power * base) % mod;
        i = i + 1;
    }
    return hash;
}

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int main(void) {
    // EXPECT: djb2 hello nonzero: 1
    int h1 = djb2_hash("hello");
    printf("djb2 hello nonzero: %d\n", h1 != 0);

    // EXPECT: djb2 world nonzero: 1
    int h2 = djb2_hash("world");
    printf("djb2 world nonzero: %d\n", h2 != 0);

    // EXPECT: djb2 different: 1
    printf("djb2 different: %d\n", h1 != h2);

    // EXPECT: djb2 same: 1
    int h3 = djb2_hash("hello");
    printf("djb2 same: %d\n", h1 == h3);

    // EXPECT: djb2 empty: 5381
    int h4 = djb2_hash("");
    printf("djb2 empty: %d\n", h4);

    // EXPECT: mod hash a mod16: 1
    int m1 = simple_mod_hash("a", 16);
    printf("mod hash a mod16: %d\n", m1);

    // EXPECT: mod hash b mod16: 2
    int m2 = simple_mod_hash("b", 16);
    printf("mod hash b mod16: %d\n", m2);

    // EXPECT: mod hash ab mod16: 3
    int m3 = simple_mod_hash("ab", 16);
    printf("mod hash ab mod16: %d\n", m3);

    // EXPECT: mod hash abc mod10: 4
    int m4 = simple_mod_hash("abc", 10);
    printf("mod hash abc mod10: %d\n", m4);

    // EXPECT: poly hash nonzero: 1
    int p1 = poly_hash("hello", 31, 1000);
    printf("poly hash nonzero: %d\n", p1 != 0);

    // EXPECT: poly same: 1
    int p2 = poly_hash("hello", 31, 1000);
    printf("poly same: %d\n", p1 == p2);

    // EXPECT: poly diff: 1
    int p3 = poly_hash("world", 31, 1000);
    printf("poly diff: %d\n", p1 != p3);

    // EXPECT: djb2 mod16 range: 1
    int dm = djb2_mod("test", 16);
    printf("djb2 mod16 range: %d\n", dm >= 0 && dm < 16);

    // EXPECT: djb2 mod8 range: 1
    int dm2 = djb2_mod("hello", 8);
    printf("djb2 mod8 range: %d\n", dm2 >= 0 && dm2 < 8);

    // EXPECT: hash distribution ok: 1
    int buckets[8];
    int bi;
    for (bi = 0; bi < 8; bi = bi + 1) buckets[bi] = 0;
    char *words[8];
    words[0] = "apple";
    words[1] = "banana";
    words[2] = "cherry";
    words[3] = "date";
    words[4] = "elder";
    words[5] = "fig";
    words[6] = "grape";
    words[7] = "honey";
    for (bi = 0; bi < 8; bi = bi + 1) {
        int b = djb2_mod(words[bi], 8);
        buckets[b] = buckets[b] + 1;
    }
    int max_bucket = 0;
    for (bi = 0; bi < 8; bi = bi + 1) {
        if (buckets[bi] > max_bucket) max_bucket = buckets[bi];
    }
    printf("hash distribution ok: %d\n", max_bucket <= 4);

    // EXPECT: single char hashes unique: 1
    int ha = djb2_mod("a", 100);
    int hb = djb2_mod("b", 100);
    int hc = djb2_mod("c", 100);
    int unique = (ha != hb) && (hb != hc) && (ha != hc);
    printf("single char hashes unique: %d\n", unique);

    // EXPECT: poly mod range: 1
    int pm = poly_hash("testing", 37, 50);
    printf("poly mod range: %d\n", pm >= 0 && pm < 50);

    // EXPECT: simple hash deterministic: 1
    int s1 = simple_mod_hash("xyz", 100);
    int s2 = simple_mod_hash("xyz", 100);
    printf("simple hash deterministic: %d\n", s1 == s2);

    // EXPECT: done
    printf("done\n");

    return 0;
}
