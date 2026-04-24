int printf(const char *fmt, ...);

int hash_mod(int x, int mod) {
    if (x < 0) x = -x;
    return x % mod;
}

int hash_mult(int x, int mod) {
    int h = x * 2654435;
    if (h < 0) h = -h;
    return h % mod;
}

int hash_xor(int x, int mod) {
    int h = x ^ (x * 7);
    if (h < 0) h = -h;
    return h % mod;
}

int hash_djb2_int(int x, int mod) {
    int h = 5381;
    h = h * 33 + (x & 255);
    h = h * 33 + ((x >> 8) & 255);
    h = h * 33 + ((x >> 16) & 255);
    if (h < 0) h = -h;
    return h % mod;
}

int count_collisions(int data[], int n, int num_buckets, int which_hash) {
    int buckets[32];
    int i;
    for (i = 0; i < num_buckets; i = i + 1) buckets[i] = 0;
    for (i = 0; i < n; i = i + 1) {
        int h;
        if (which_hash == 0) h = hash_mod(data[i], num_buckets);
        else if (which_hash == 1) h = hash_mult(data[i], num_buckets);
        else if (which_hash == 2) h = hash_xor(data[i], num_buckets);
        else h = hash_djb2_int(data[i], num_buckets);
        buckets[h] = buckets[h] + 1;
    }
    int collisions = 0;
    for (i = 0; i < num_buckets; i = i + 1) {
        if (buckets[i] > 1) {
            collisions = collisions + (buckets[i] - 1);
        }
    }
    return collisions;
}

int max_chain_len(int data[], int n, int num_buckets, int which_hash) {
    int buckets[32];
    int i;
    for (i = 0; i < num_buckets; i = i + 1) buckets[i] = 0;
    for (i = 0; i < n; i = i + 1) {
        int h;
        if (which_hash == 0) h = hash_mod(data[i], num_buckets);
        else if (which_hash == 1) h = hash_mult(data[i], num_buckets);
        else if (which_hash == 2) h = hash_xor(data[i], num_buckets);
        else h = hash_djb2_int(data[i], num_buckets);
        buckets[h] = buckets[h] + 1;
    }
    int mx = 0;
    for (i = 0; i < num_buckets; i = i + 1) {
        if (buckets[i] > mx) mx = buckets[i];
    }
    return mx;
}

int empty_buckets(int data[], int n, int num_buckets, int which_hash) {
    int buckets[32];
    int i;
    for (i = 0; i < num_buckets; i = i + 1) buckets[i] = 0;
    for (i = 0; i < n; i = i + 1) {
        int h;
        if (which_hash == 0) h = hash_mod(data[i], num_buckets);
        else if (which_hash == 1) h = hash_mult(data[i], num_buckets);
        else if (which_hash == 2) h = hash_xor(data[i], num_buckets);
        else h = hash_djb2_int(data[i], num_buckets);
        buckets[h] = buckets[h] + 1;
    }
    int empty = 0;
    for (i = 0; i < num_buckets; i = i + 1) {
        if (buckets[i] == 0) empty = empty + 1;
    }
    return empty;
}

int main(void) {
    int data[16];
    data[0] = 5; data[1] = 10; data[2] = 15; data[3] = 20;
    data[4] = 25; data[5] = 30; data[6] = 35; data[7] = 40;
    data[8] = 3; data[9] = 8; data[10] = 13; data[11] = 18;
    data[12] = 23; data[13] = 28; data[14] = 33; data[15] = 38;

    // EXPECT: mod collisions 8 buckets: 8
    int c0 = count_collisions(data, 16, 8, 0);
    printf("mod collisions 8 buckets: %d\n", c0);

    // EXPECT: mult collisions 8 buckets nonneg: 1
    int c1 = count_collisions(data, 16, 8, 1);
    printf("mult collisions 8 buckets nonneg: %d\n", c1 >= 0);

    // EXPECT: xor collisions 8 buckets nonneg: 1
    int c2 = count_collisions(data, 16, 8, 2);
    printf("xor collisions 8 buckets nonneg: %d\n", c2 >= 0);

    // EXPECT: djb2 collisions 8 nonneg: 1
    int c3 = count_collisions(data, 16, 8, 3);
    printf("djb2 collisions 8 nonneg: %d\n", c3 >= 0);

    // EXPECT: mod max chain 8: 2
    int mc0 = max_chain_len(data, 16, 8, 0);
    printf("mod max chain 8: %d\n", mc0);

    // EXPECT: mult max chain 8 pos: 1
    int mc1 = max_chain_len(data, 16, 8, 1);
    printf("mult max chain 8 pos: %d\n", mc1 > 0);

    // EXPECT: mod empty 8: 0
    int e0 = empty_buckets(data, 16, 8, 0);
    printf("mod empty 8: %d\n", e0);

    int seq[10];
    int si;
    for (si = 0; si < 10; si = si + 1) seq[si] = si + 1;

    // EXPECT: seq mod coll 16: 0
    int sc0 = count_collisions(seq, 10, 16, 0);
    printf("seq mod coll 16: %d\n", sc0);

    // EXPECT: seq mod max 16: 1
    int sm0 = max_chain_len(seq, 10, 16, 0);
    printf("seq mod max 16: %d\n", sm0);

    // EXPECT: seq mod empty 16: 6
    int se0 = empty_buckets(seq, 10, 16, 0);
    printf("seq mod empty 16: %d\n", se0);

    int same[8];
    for (si = 0; si < 8; si = si + 1) same[si] = 42;

    // EXPECT: same mod coll 8: 7
    int sc1 = count_collisions(same, 8, 8, 0);
    printf("same mod coll 8: %d\n", sc1);

    // EXPECT: same max chain: 8
    int sm1 = max_chain_len(same, 8, 8, 0);
    printf("same max chain: %d\n", sm1);

    // EXPECT: same empty 8: 7
    int se1 = empty_buckets(same, 8, 8, 0);
    printf("same empty 8: %d\n", se1);

    // EXPECT: more buckets fewer collisions: 1
    int c16 = count_collisions(data, 16, 16, 0);
    int c8 = count_collisions(data, 16, 8, 0);
    printf("more buckets fewer collisions: %d\n", c16 <= c8);

    // EXPECT: 16 buckets max chain: 2
    int mc16 = max_chain_len(data, 16, 16, 0);
    printf("16 buckets max chain: %d\n", mc16);

    // EXPECT: done
    printf("done\n");

    return 0;
}
