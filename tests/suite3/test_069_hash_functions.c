int printf(const char *fmt, ...);

/* Hash functions: djb2, fnv-like, hash combining, probe sequences */

/* djb2 hash: hash = hash*33 + c, starting from 5381 */
unsigned int djb2(const char *str) {
    unsigned int hash = 5381u;
    int c;
    while ((c = *str) != 0) {
        hash = hash * 33u + (unsigned int)c;
        str++;
    }
    return hash;
}

/* FNV-1a 32-bit: offset_basis = 2166136261, prime = 16777619 */
unsigned int fnv1a(const char *str) {
    unsigned int hash = 2166136261u;
    int c;
    while ((c = *str) != 0) {
        hash = hash ^ (unsigned int)c;
        hash = hash * 16777619u;
        str++;
    }
    return hash;
}

/* Simple polynomial hash mod a prime */
unsigned int poly_hash(const char *str, unsigned int mod) {
    unsigned int hash = 0;
    unsigned int base = 31u;
    int c;
    while ((c = *str) != 0) {
        hash = (hash * base + (unsigned int)(c - 'a' + 1)) % mod;
        str++;
    }
    return hash;
}

/* Combine two hash values */
unsigned int hash_combine(unsigned int h1, unsigned int h2) {
    /* Common technique: h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2)) */
    return h1 ^ (h2 + 0x9e3779b9u + (h1 << 6) + (h1 >> 2));
}

/* Linear probe: compute slot index with wrap-around */
int probe(unsigned int hash_val, int step, int table_size) {
    return (int)((hash_val + (unsigned int)step) % (unsigned int)table_size);
}

/* Quadratic probe */
int qprobe(unsigned int hash_val, int step, int table_size) {
    unsigned int slot = (hash_val + (unsigned int)(step * step)) % (unsigned int)table_size;
    return (int)slot;
}

int main(void) {
    /* --- DJB2 HASH --- */
    /* hash("") = 5381 */
    printf("%u\n", djb2(""));                    // EXPECT: 5381

    /* hash("a") = 5381*33 + 97 = 177670 */
    printf("%u\n", djb2("a"));                   // EXPECT: 177670

    /* hash("b") = 5381*33 + 98 = 177671 */
    printf("%u\n", djb2("b"));                   // EXPECT: 177671

    /* Two strings that should hash differently */
    unsigned int h1 = djb2("hello");
    unsigned int h2 = djb2("world");
    printf("%d\n", h1 != h2 ? 1 : 0);           // EXPECT: 1

    /* Known djb2 value for "hello" (32-bit wrapped) */
    printf("%u\n", djb2("hello"));               // EXPECT: 261238937

    /* --- FNV-1A HASH --- */
    /* fnv1a("") = 2166136261 (just the offset basis) */
    printf("%u\n", fnv1a(""));                   // EXPECT: 2166136261

    /* fnv1a known values */
    unsigned int fh1 = fnv1a("hello");
    unsigned int fh2 = fnv1a("hello");
    printf("%d\n", fh1 == fh2 ? 1 : 0);         // EXPECT: 1
    /* same string -> same hash */

    unsigned int fh3 = fnv1a("world");
    printf("%d\n", fh1 != fh3 ? 1 : 0);         // EXPECT: 1

    /* --- POLYNOMIAL HASH MOD PRIME --- */
    /* poly_hash("abc", 1000000007): a=1, b=2, c=3 */
    /* hash = 0; h = (0*31+1) = 1; h = (1*31+2) = 33; h = (33*31+3) = 1026 */
    printf("%u\n", poly_hash("abc", 1000000007u)); // EXPECT: 1026

    /* poly_hash("a", large_prime) = 1 */
    printf("%u\n", poly_hash("a", 1000000007u));   // EXPECT: 1

    /* poly_hash("b", large_prime) = 2 */
    printf("%u\n", poly_hash("b", 1000000007u));   // EXPECT: 2

    /* poly_hash("z", large_prime) = 26 */
    printf("%u\n", poly_hash("z", 1000000007u));   // EXPECT: 26

    /* poly_hash with small modulus */
    printf("%u\n", poly_hash("abc", 97u));               // EXPECT: 56
    /* 1026 % 97 = 1026 - 10*97 = 1026 - 970 = 56 */

    /* --- HASH AVALANCHE: small change -> different hash --- */
    unsigned int ha = djb2("test");
    unsigned int hb = djb2("uest");
    printf("%d\n", ha != hb ? 1 : 0);             // EXPECT: 1

    /* --- LINEAR PROBE SEQUENCE --- */
    /* table size 16, initial hash = 5 */
    printf("%d\n", probe(5u, 0, 16));              // EXPECT: 5
    printf("%d\n", probe(5u, 1, 16));              // EXPECT: 6
    printf("%d\n", probe(5u, 7, 16));              // EXPECT: 12
    printf("%d\n", probe(5u, 11, 16));             // EXPECT: 0
    /* (5+11) % 16 = 16 % 16 = 0 */
    printf("%d\n", probe(5u, 12, 16));             // EXPECT: 1
    printf("%d\n", probe(14u, 3, 16));             // EXPECT: 1
    /* (14+3) % 16 = 17 % 16 = 1 */

    /* --- QUADRATIC PROBE SEQUENCE --- */
    printf("%d\n", qprobe(5u, 0, 16));             // EXPECT: 5
    printf("%d\n", qprobe(5u, 1, 16));             // EXPECT: 6
    /* (5 + 1) % 16 = 6 */
    printf("%d\n", qprobe(5u, 2, 16));             // EXPECT: 9
    /* (5 + 4) % 16 = 9 */
    printf("%d\n", qprobe(5u, 3, 16));             // EXPECT: 14
    /* (5 + 9) % 16 = 14 */
    printf("%d\n", qprobe(5u, 4, 16));             // EXPECT: 5
    /* (5 + 16) % 16 = 5 */
    printf("%d\n", qprobe(5u, 5, 16));             // EXPECT: 14
    /* (5 + 25) % 16 = 30 % 16 = 14 */

    /* --- HASH DISTRIBUTION: simple mod-based bucket --- */
    int buckets = 10;
    /* Simulate inserting several keys and checking bucket distribution */
    unsigned int keys[5];
    keys[0] = djb2("apple");
    keys[1] = djb2("banana");
    keys[2] = djb2("cherry");
    keys[3] = djb2("date");
    keys[4] = djb2("elderberry");

    /* Check that all keys land in valid buckets [0, 9] */
    int b0 = (int)(keys[0] % (unsigned int)buckets);
    int b1 = (int)(keys[1] % (unsigned int)buckets);
    int b2 = (int)(keys[2] % (unsigned int)buckets);
    int b3 = (int)(keys[3] % (unsigned int)buckets);
    int b4 = (int)(keys[4] % (unsigned int)buckets);
    printf("%d\n", b0 >= 0 && b0 < buckets ? 1 : 0);   // EXPECT: 1
    printf("%d\n", b1 >= 0 && b1 < buckets ? 1 : 0);   // EXPECT: 1
    printf("%d\n", b2 >= 0 && b2 < buckets ? 1 : 0);   // EXPECT: 1
    printf("%d\n", b3 >= 0 && b3 < buckets ? 1 : 0);   // EXPECT: 1
    printf("%d\n", b4 >= 0 && b4 < buckets ? 1 : 0);   // EXPECT: 1

    /* --- HASH COMBINING --- */
    unsigned int ch1 = hash_combine(0u, 0u);
    printf("%u\n", ch1);                           // EXPECT: 2654435769
    /* 0 ^ (0 + 0x9e3779b9 + 0 + 0) = 0x9e3779b9 = 2654435769 */

    unsigned int ch2 = hash_combine(1u, 1u);
    /* 1 ^ (1 + 0x9e3779b9 + (1<<6) + (1>>2)) */
    /* = 1 ^ (1 + 2654435769 + 64 + 0) = 1 ^ 2654435834 */
    /* 2654435834 = 0x9E3779FA; 1 ^ 0x9E3779FA = 0x9E3779FB = 2654435835 */
    printf("%u\n", ch2);                           // EXPECT: 2654435835

    return 0;
}
