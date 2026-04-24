// stress_089_crypto_hash.c - DJB2, FNV-1a, Jenkins one-at-a-time, SDBM hash
// EXPECT: djb2_empty=5381
// EXPECT: djb2_hello=210714636441
// EXPECT: djb2_world=210732791149
// EXPECT: fnv_empty=2166136261
// EXPECT: fnv_hello=1335831723
// EXPECT: fnv_world=933488787
// EXPECT: jenkins_empty=0
// EXPECT: jenkins_hello=3372029979
// EXPECT: jenkins_world=2425990116
// EXPECT: sdbm_empty=0
// EXPECT: sdbm_hello=684824882
// EXPECT: sdbm_world=4158893746
// EXPECT: djb2_diff_ab=1
// EXPECT: fnv_diff_ab=1
// EXPECT: jenkins_diff_ab=1
// EXPECT: sdbm_diff_ab=1
// EXPECT: collision_djb2=0
// EXPECT: collision_fnv=0
// EXPECT: batch_djb2_ok=1
// EXPECT: batch_fnv_ok=1
// EXPECT: batch_jenkins_ok=1
// EXPECT: batch_sdbm_ok=1
// EXPECT: avalanche_ok=1
// EXPECT: pass_done=1

int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) len++;
    return len;
}

// --- DJB2 hash ---
unsigned long djb2(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++) != 0) {
        hash = hash * 33 + c;
    }
    return hash;
}

// --- FNV-1a hash (32-bit) ---
unsigned int fnv1a(char *str) {
    unsigned int hash = 2166136261u;
    while (*str) {
        hash = hash ^ (unsigned int)(unsigned char)*str;
        hash = hash * 16777619u;
        str++;
    }
    return hash;
}

// --- Jenkins one-at-a-time ---
unsigned int jenkins(char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash += (unsigned char)*str;
        hash += hash << 10;
        hash ^= hash >> 6;
        str++;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

// --- SDBM hash ---
unsigned int sdbm(char *str) {
    unsigned int hash = 0;
    int c;
    while ((c = *str++) != 0) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

// Count number of differing bits between two 32-bit values
int hamming_distance(unsigned int a, unsigned int b) {
    unsigned int diff = a ^ b;
    int count = 0;
    while (diff) {
        count += diff & 1;
        diff >>= 1;
    }
    return count;
}

int main(void) {
    // --- Basic hash values ---
    printf("djb2_empty=%lu\n", djb2(""));
    printf("djb2_hello=%lu\n", djb2("hello"));
    printf("djb2_world=%lu\n", djb2("world"));

    printf("fnv_empty=%u\n", fnv1a(""));
    printf("fnv_hello=%u\n", fnv1a("hello"));
    printf("fnv_world=%u\n", fnv1a("world"));

    printf("jenkins_empty=%u\n", jenkins(""));
    printf("jenkins_hello=%u\n", jenkins("hello"));
    printf("jenkins_world=%u\n", jenkins("world"));

    printf("sdbm_empty=%u\n", sdbm(""));
    printf("sdbm_hello=%u\n", sdbm("hello"));
    printf("sdbm_world=%u\n", sdbm("world"));

    // --- Different strings produce different hashes ---
    int djb2_diff = (djb2("ab") != djb2("ba")) ? 1 : 0;
    printf("djb2_diff_ab=%d\n", djb2_diff);

    int fnv_diff = (fnv1a("ab") != fnv1a("ba")) ? 1 : 0;
    printf("fnv_diff_ab=%d\n", fnv_diff);

    int jenkins_diff = (jenkins("ab") != jenkins("ba")) ? 1 : 0;
    printf("jenkins_diff_ab=%d\n", jenkins_diff);

    int sdbm_diff = (sdbm("ab") != sdbm("ba")) ? 1 : 0;
    printf("sdbm_diff_ab=%d\n", sdbm_diff);

    // --- Collision resistance: hash 20 strings, count collisions ---
    char *strings[20];
    strings[0] = "alpha";    strings[1] = "beta";     strings[2] = "gamma";
    strings[3] = "delta";    strings[4] = "epsilon";  strings[5] = "zeta";
    strings[6] = "eta";      strings[7] = "theta";    strings[8] = "iota";
    strings[9] = "kappa";    strings[10] = "lambda";  strings[11] = "mu";
    strings[12] = "nu";      strings[13] = "xi";      strings[14] = "omicron";
    strings[15] = "pi";      strings[16] = "rho";     strings[17] = "sigma";
    strings[18] = "tau";     strings[19] = "upsilon";

    unsigned int djb2_hashes[20];
    unsigned int fnv_hashes[20];
    int i, j;

    for (i = 0; i < 20; i++) {
        // Use lower 32 bits of djb2
        djb2_hashes[i] = (unsigned int)djb2(strings[i]);
        fnv_hashes[i] = fnv1a(strings[i]);
    }

    int djb2_collisions = 0;
    int fnv_collisions = 0;
    for (i = 0; i < 20; i++) {
        for (j = i + 1; j < 20; j++) {
            if (djb2_hashes[i] == djb2_hashes[j]) djb2_collisions++;
            if (fnv_hashes[i] == fnv_hashes[j]) fnv_collisions++;
        }
    }
    printf("collision_djb2=%d\n", djb2_collisions);
    printf("collision_fnv=%d\n", fnv_collisions);

    // --- Batch consistency: hash same string twice gives same result ---
    int batch_djb2 = 1, batch_fnv = 1, batch_jen = 1, batch_sdbm = 1;
    for (i = 0; i < 20; i++) {
        if ((unsigned int)djb2(strings[i]) != djb2_hashes[i]) batch_djb2 = 0;
        if (fnv1a(strings[i]) != fnv_hashes[i]) batch_fnv = 0;
        if (jenkins(strings[i]) != jenkins(strings[i])) batch_jen = 0;
        if (sdbm(strings[i]) != sdbm(strings[i])) batch_sdbm = 0;
    }
    printf("batch_djb2_ok=%d\n", batch_djb2);
    printf("batch_fnv_ok=%d\n", batch_fnv);
    printf("batch_jenkins_ok=%d\n", batch_jen);
    printf("batch_sdbm_ok=%d\n", batch_sdbm);

    // --- Avalanche effect: single bit change should change many output bits ---
    // Compare "hello" and "hellp" (differ by 1 bit in last char: o=0x6F, p=0x70, differ by more than 1 bit actually)
    // Use "aa" vs "ab" (a=0x61, b=0x62, differ by 1 bit in LSB)
    unsigned int h1 = fnv1a("aa");
    unsigned int h2 = fnv1a("ab");
    int diff_bits = hamming_distance(h1, h2);
    // Good hash should change multiple bits (avalanche). Expect >= 4 bits changed.
    int avalanche_ok = (diff_bits >= 4) ? 1 : 0;
    printf("avalanche_ok=%d\n", avalanche_ok);

    printf("pass_done=1\n");
    return 0;
}
