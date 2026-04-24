int printf(const char *fmt, ...);
// EXPECT: === Hash Function Tests ===\ndjb2 consistency: 1\nfnv1a consistency: 1\ndjb2 diff: 1\nfnv1a diff: 1\ndjb2 empty diff: 1\nfnv1a empty diff: 1\ndjb2 single a: 177670\nfnv1a single a: 3826002220\n=== XOR Checksum ===\nxor hello: 98\nxor world: 98\nxor test: 22\nxor empty: 0\nxor a: 97\nxor ab: 3\nxor abc: 96\nxor abcd: 4\nxor abcde: 97\n=== XOR Properties ===\nxor symmetric: 1\nxor length1: 1\nxor empty zero: 1\n=== Rolling Hash ===\nrolling hello window 3:\n  w0: 313\n  w1: 317\n  w2: 327\nrolling count: 3\nrolling all diff: 1\n=== Hash Distribution ===\nbuckets used: 5\nmax bucket: 3\ngood dist: 1\n=== Hash Table ===\ninserted: 10\nall found: 1\nmissing not found: 1\n=== Chain Verify ===\ndjb2 fox: 2546203896\nfnv1a fox: 2924308450\nxor fox: 11\ncombined: 965647121\ncombined nonzero: 1\ndjb2 foo: 193491849\ndjb2 bar: 193487034\ndjb2 foobar: 4259602622\nconcat diff: 1\nDone

/* --- djb2 hash function --- */
unsigned djb2(char *str) {
    unsigned hash;
    int c;
    hash = 5381;
    while ((c = *str)) {
        hash = hash * 33 + c;
        str = str + 1;
    }
    return hash;
}

/* --- FNV-1a hash function --- */
unsigned fnv1a(char *str) {
    unsigned hash;
    int c;
    hash = 0x811c9dc5;
    while ((c = *str)) {
        hash = hash ^ c;
        hash = hash * 0x01000193;
        str = str + 1;
    }
    return hash;
}

/* --- Simple XOR checksum --- */
unsigned xor_checksum(char *str) {
    unsigned hash;
    hash = 0;
    while (*str) {
        hash = hash ^ *str;
        str = str + 1;
    }
    return hash;
}

/* --- String length --- */
int str_len(char *s) {
    int n;
    n = 0;
    while (s[n]) {
        n = n + 1;
    }
    return n;
}

/* --- String equality --- */
int str_eq(char *a, char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a = a + 1;
        b = b + 1;
    }
    return *a == *b;
}

/* --- Rolling hash: sum of ASCII values in a window --- */
unsigned rolling_hash(char *str, int start, int window) {
    unsigned h;
    int i;
    h = 0;
    i = 0;
    while (i < window) {
        h = h + str[start + i];
        i = i + 1;
    }
    return h;
}

/* --- Hash table with linear probing --- */
struct HashEntry {
    int occupied;
    char *key;
    int val;
};

int ht_insert(struct HashEntry *table, int size, char *key, int val) {
    unsigned h;
    int pos;
    int steps;
    h = djb2(key) % size;
    pos = h;
    steps = 0;
    while (table[pos].occupied && steps < size) {
        pos = (pos + 1) % size;
        steps = steps + 1;
    }
    if (steps >= size) return 0;
    table[pos].occupied = 1;
    table[pos].key = key;
    table[pos].val = val;
    return 1;
}

int ht_lookup(struct HashEntry *table, int size, char *key) {
    unsigned h;
    int pos;
    int steps;
    h = djb2(key) % size;
    pos = h;
    steps = 0;
    while (table[pos].occupied && steps < size) {
        if (str_eq(table[pos].key, key)) {
            return table[pos].val;
        }
        pos = (pos + 1) % size;
        steps = steps + 1;
    }
    return -1;
}

void test_hash_functions(void) {
    unsigned h1;
    unsigned h2;

    printf("=== Hash Function Tests ===\n");

    /* Consistency: same input -> same output */
    h1 = djb2("hello");
    h2 = djb2("hello");
    printf("djb2 consistency: %d\n", h1 == h2);

    h1 = fnv1a("hello");
    h2 = fnv1a("hello");
    printf("fnv1a consistency: %d\n", h1 == h2);

    /* Different inputs -> different outputs */
    h1 = djb2("hello");
    h2 = djb2("world");
    printf("djb2 diff: %d\n", h1 != h2);

    h1 = fnv1a("hello");
    h2 = fnv1a("world");
    printf("fnv1a diff: %d\n", h1 != h2);

    /* Empty string differs from non-empty */
    h1 = djb2("");
    h2 = djb2("hello");
    printf("djb2 empty diff: %d\n", h1 != h2);

    h1 = fnv1a("");
    h2 = fnv1a("hello");
    printf("fnv1a empty diff: %d\n", h1 != h2);

    /* Known values for single character */
    printf("djb2 single a: %u\n", djb2("a"));
    printf("fnv1a single a: %u\n", fnv1a("a"));
}

void test_xor_checksum(void) {
    printf("=== XOR Checksum ===\n");

    printf("xor hello: %u\n", xor_checksum("hello"));
    printf("xor world: %u\n", xor_checksum("world"));
    printf("xor test: %u\n", xor_checksum("test"));
    printf("xor empty: %u\n", xor_checksum(""));
    printf("xor a: %u\n", xor_checksum("a"));
    printf("xor ab: %u\n", xor_checksum("ab"));
    printf("xor abc: %u\n", xor_checksum("abc"));
    printf("xor abcd: %u\n", xor_checksum("abcd"));
    printf("xor abcde: %u\n", xor_checksum("abcde"));
}

void test_xor_properties(void) {
    printf("=== XOR Properties ===\n");

    /* XOR is commutative: xor("ab") == xor("ba") */
    printf("xor symmetric: %d\n", xor_checksum("ab") == xor_checksum("ba"));

    /* Single char: xor("a") == 97 */
    printf("xor length1: %d\n", xor_checksum("a") == 97);

    /* Empty string: xor("") == 0 */
    printf("xor empty zero: %d\n", xor_checksum("") == 0);
}

void test_rolling_hash(void) {
    int len;
    int window;
    int count;
    int i;
    int j;
    unsigned hashes[16];
    int all_diff;

    printf("=== Rolling Hash ===\n");

    len = str_len("hello");
    window = 3;
    count = len - window + 1;

    printf("rolling hello window 3:\n");
    i = 0;
    while (i < count) {
        hashes[i] = rolling_hash("hello", i, window);
        printf("  w%d: %u\n", i, hashes[i]);
        i = i + 1;
    }
    printf("rolling count: %d\n", count);

    /* Check all rolling hashes are different */
    all_diff = 1;
    i = 0;
    while (i < count) {
        j = i + 1;
        while (j < count) {
            if (hashes[i] == hashes[j]) {
                all_diff = 0;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    printf("rolling all diff: %d\n", all_diff);
}

void test_hash_distribution(void) {
    char *strings[10];
    int buckets[8];
    unsigned h;
    int i;
    int used;
    int max_b;

    printf("=== Hash Distribution ===\n");

    strings[0] = "alpha";
    strings[1] = "beta";
    strings[2] = "gamma";
    strings[3] = "delta";
    strings[4] = "epsilon";
    strings[5] = "zeta";
    strings[6] = "eta";
    strings[7] = "theta";
    strings[8] = "iota";
    strings[9] = "kappa";

    /* Initialize buckets to 0 */
    i = 0;
    while (i < 8) {
        buckets[i] = 0;
        i = i + 1;
    }

    /* Hash each string into 8 buckets */
    i = 0;
    while (i < 10) {
        h = djb2(strings[i]);
        buckets[h % 8] = buckets[h % 8] + 1;
        i = i + 1;
    }

    /* Count non-empty buckets */
    used = 0;
    max_b = 0;
    i = 0;
    while (i < 8) {
        if (buckets[i] > 0) {
            used = used + 1;
        }
        if (buckets[i] > max_b) {
            max_b = buckets[i];
        }
        i = i + 1;
    }

    printf("buckets used: %d\n", used);
    printf("max bucket: %d\n", max_b);
    printf("good dist: %d\n", used >= 4);
}

void test_hash_table(void) {
    struct HashEntry table[16];
    char *strings[10];
    int i;
    int count;
    int all_found;
    int val;

    printf("=== Hash Table ===\n");

    strings[0] = "alpha";
    strings[1] = "beta";
    strings[2] = "gamma";
    strings[3] = "delta";
    strings[4] = "epsilon";
    strings[5] = "zeta";
    strings[6] = "eta";
    strings[7] = "theta";
    strings[8] = "iota";
    strings[9] = "kappa";

    /* Initialize table */
    i = 0;
    while (i < 16) {
        table[i].occupied = 0;
        table[i].key = "";
        table[i].val = 0;
        i = i + 1;
    }

    /* Insert all strings */
    i = 0;
    while (i < 10) {
        ht_insert(table, 16, strings[i], i + 1);
        i = i + 1;
    }

    /* Count inserted */
    count = 0;
    i = 0;
    while (i < 16) {
        if (table[i].occupied) {
            count = count + 1;
        }
        i = i + 1;
    }
    printf("inserted: %d\n", count);

    /* Lookup all strings */
    all_found = 1;
    i = 0;
    while (i < 10) {
        val = ht_lookup(table, 16, strings[i]);
        if (val != i + 1) {
            all_found = 0;
        }
        i = i + 1;
    }
    printf("all found: %d\n", all_found);

    /* Lookup missing key */
    val = ht_lookup(table, 16, "missing");
    printf("missing not found: %d\n", val == -1);
}

void test_chain_verify(void) {
    unsigned h1;
    unsigned h2;
    unsigned x;
    unsigned combined;

    printf("=== Chain Verify ===\n");

    /* Hash a longer string with all three functions */
    h1 = djb2("The quick brown fox");
    h2 = fnv1a("The quick brown fox");
    x = xor_checksum("The quick brown fox");

    printf("djb2 fox: %u\n", h1);
    printf("fnv1a fox: %u\n", h2);
    printf("xor fox: %u\n", x);

    /* Combine all hashes with XOR */
    combined = h1 ^ h2 ^ x;
    printf("combined: %u\n", combined);
    printf("combined nonzero: %d\n", combined != 0);

    /* Verify that hash of concatenation differs from parts */
    printf("djb2 foo: %u\n", djb2("foo"));
    printf("djb2 bar: %u\n", djb2("bar"));
    printf("djb2 foobar: %u\n", djb2("foobar"));
    printf("concat diff: %d\n", djb2("foobar") != djb2("foo") && djb2("foobar") != djb2("bar"));
}

int main(void) {
    test_hash_functions();
    test_xor_checksum();
    test_xor_properties();
    test_rolling_hash();
    test_hash_distribution();
    test_hash_table();
    test_chain_verify();
    printf("Done\n");
    return 0;
}
