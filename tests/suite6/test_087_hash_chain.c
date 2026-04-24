int printf(const char *fmt, ...);

int hash_func(int val) {
    int h = val;
    h = h ^ (h >> 4);
    h = (h * 2654435) & 65535;
    h = h ^ (h >> 8);
    return h & 255;
}

int hash_chain(int val, int depth) {
    int h = val;
    int i;
    for (i = 0; i < depth; i = i + 1) {
        h = hash_func(h);
    }
    return h;
}

int hash_combine(int a, int b) {
    return hash_func(a ^ (b * 37 + 17));
}

int merkle_hash(int *data, int len) {
    if (len == 1) return hash_func(data[0]);

    int hashes[32];
    int i;
    int n = len;

    for (i = 0; i < n; i = i + 1) {
        hashes[i] = hash_func(data[i]);
    }

    while (n > 1) {
        int new_n = 0;
        for (i = 0; i + 1 < n; i = i + 2) {
            hashes[new_n] = hash_combine(hashes[i], hashes[i + 1]);
            new_n = new_n + 1;
        }
        if (n % 2 == 1) {
            hashes[new_n] = hashes[n - 1];
            new_n = new_n + 1;
        }
        n = new_n;
    }
    return hashes[0];
}

int verify_chain(int start, int end, int depth) {
    return hash_chain(start, depth) == end;
}

int main(void) {
    int h0 = hash_func(0);
    // EXPECT: Hash(0): 0
    printf("Hash(0): %d\n", h0);

    int h1 = hash_func(1);
    // EXPECT: Hash(1): 99
    printf("Hash(1): %d\n", h1);

    int h42 = hash_func(42);
    // EXPECT: Hash(42): 91
    printf("Hash(42): %d\n", h42);

    int h255 = hash_func(255);
    // EXPECT: Hash(255): 4
    printf("Hash(255): %d\n", h255);

    int c1 = hash_chain(42, 1);
    // EXPECT: Chain(42,1): 91
    printf("Chain(42,1): %d\n", c1);

    int c2 = hash_chain(42, 2);
    // EXPECT: Chain(42,2): 9
    printf("Chain(42,2): %d\n", c2);

    int c3 = hash_chain(42, 3);
    // EXPECT: Chain(42,3): 124
    printf("Chain(42,3): %d\n", c3);

    int c0 = hash_chain(42, 0);
    // EXPECT: Chain(42,0): 42
    printf("Chain(42,0): %d\n", c0);

    int v = verify_chain(42, c3, 3);
    // EXPECT: Verify chain: 1
    printf("Verify chain: %d\n", v);

    int vf = verify_chain(42, 99, 3);
    // EXPECT: Verify fail: 0
    printf("Verify fail: %d\n", vf);

    int hc = hash_combine(10, 20);
    // EXPECT: Combine(10,20): 14
    printf("Combine(10,20): %d\n", hc);

    int data4[4];
    data4[0] = 1; data4[1] = 2; data4[2] = 3; data4[3] = 4;
    int m4 = merkle_hash(data4, 4);
    // EXPECT: Merkle 4 elems: 71
    printf("Merkle 4 elems: %d\n", m4);

    int data2[2];
    data2[0] = 1; data2[1] = 2;
    int m2 = merkle_hash(data2, 2);
    // EXPECT: Merkle 2 elems: 208
    printf("Merkle 2 elems: %d\n", m2);

    int data1[1];
    data1[0] = 42;
    int m1 = merkle_hash(data1, 1);
    // EXPECT: Merkle 1 elem: 91
    printf("Merkle 1 elem: %d\n", m1);

    int data3[3];
    data3[0] = 10; data3[1] = 20; data3[2] = 30;
    int m3 = merkle_hash(data3, 3);
    // EXPECT: Merkle 3 elems: 246
    printf("Merkle 3 elems: %d\n", m3);

    int diff = (hash_func(0) != hash_func(1)) ? 1 : 0;
    // EXPECT: Different inputs diff hash: 1
    printf("Different inputs diff hash: %d\n", diff);

    int range_ok = 1;
    int i;
    for (i = 0; i < 20; i = i + 1) {
        int hh = hash_func(i);
        if (hh < 0 || hh > 255) range_ok = 0;
    }
    // EXPECT: All hashes in range: 1
    printf("All hashes in range: %d\n", range_ok);

    return 0;
}
