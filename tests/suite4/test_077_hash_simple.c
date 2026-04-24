int printf(const char *fmt, ...);

int hash_mod(int key, int table_size) {
    int h;
    h = key % table_size;
    if (h < 0) {
        h = h + table_size;
    }
    return h;
}

int hash_mult(int key, int table_size) {
    int h;
    h = key * 2654435;
    if (h < 0) {
        h = -h;
    }
    h = h % table_size;
    return h;
}

struct Entry {
    int key;
    int value;
    int occupied;
};

struct HashTable {
    struct Entry buckets[16];
    int size;
    int count;
};

void ht_init(struct HashTable *ht) {
    int i;
    ht->size = 16;
    ht->count = 0;
    for (i = 0; i < 16; i = i + 1) {
        ht->buckets[i].key = 0;
        ht->buckets[i].value = 0;
        ht->buckets[i].occupied = 0;
    }
}

int ht_insert(struct HashTable *ht, int key, int value) {
    int idx;
    int i;
    int probes;
    idx = hash_mod(key, ht->size);
    probes = 0;
    for (i = 0; i < ht->size; i = i + 1) {
        probes = probes + 1;
        if (ht->buckets[idx].occupied == 0) {
            ht->buckets[idx].key = key;
            ht->buckets[idx].value = value;
            ht->buckets[idx].occupied = 1;
            ht->count = ht->count + 1;
            return probes;
        }
        if (ht->buckets[idx].key == key) {
            ht->buckets[idx].value = value;
            return probes;
        }
        idx = (idx + 1) % ht->size;
    }
    return -1;
}

int ht_get(struct HashTable *ht, int key) {
    int idx;
    int i;
    idx = hash_mod(key, ht->size);
    for (i = 0; i < ht->size; i = i + 1) {
        if (ht->buckets[idx].occupied == 0) {
            return -1;
        }
        if (ht->buckets[idx].key == key) {
            return ht->buckets[idx].value;
        }
        idx = (idx + 1) % ht->size;
    }
    return -1;
}

int ht_contains(struct HashTable *ht, int key) {
    int idx;
    int i;
    idx = hash_mod(key, ht->size);
    for (i = 0; i < ht->size; i = i + 1) {
        if (ht->buckets[idx].occupied == 0) {
            return 0;
        }
        if (ht->buckets[idx].key == key) {
            return 1;
        }
        idx = (idx + 1) % ht->size;
    }
    return 0;
}

int count_collisions(int *keys, int n, int table_size) {
    int i;
    int j;
    int collisions;
    int h1;
    int h2;
    collisions = 0;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            h1 = hash_mod(keys[i], table_size);
            h2 = hash_mod(keys[j], table_size);
            if (h1 == h2) {
                collisions = collisions + 1;
            }
        }
    }
    return collisions;
}

int main(void) {
    struct HashTable ht;
    int keys[8];
    int probes;

    // EXPECT: hash 10 mod 7: 3
    printf("hash 10 mod 7: %d\n", hash_mod(10, 7));
    // EXPECT: hash 0 mod 7: 0
    printf("hash 0 mod 7: %d\n", hash_mod(0, 7));
    // EXPECT: hash 7 mod 7: 0
    printf("hash 7 mod 7: %d\n", hash_mod(7, 7));
    // EXPECT: hash 15 mod 16: 15
    printf("hash 15 mod 16: %d\n", hash_mod(15, 16));

    ht_init(&ht);

    // EXPECT: count: 0
    printf("count: %d\n", ht.count);

    probes = ht_insert(&ht, 5, 50);
    // EXPECT: insert 5 probes: 1
    printf("insert 5 probes: %d\n", probes);

    probes = ht_insert(&ht, 21, 210);
    // EXPECT: insert 21 probes: 2
    printf("insert 21 probes: %d\n", probes);

    ht_insert(&ht, 10, 100);
    ht_insert(&ht, 3, 30);
    ht_insert(&ht, 7, 70);

    // EXPECT: count: 5
    printf("count: %d\n", ht.count);

    // EXPECT: get 5: 50
    printf("get 5: %d\n", ht_get(&ht, 5));
    // EXPECT: get 10: 100
    printf("get 10: %d\n", ht_get(&ht, 10));
    // EXPECT: get 21: 210
    printf("get 21: %d\n", ht_get(&ht, 21));
    // EXPECT: get 3: 30
    printf("get 3: %d\n", ht_get(&ht, 3));
    // EXPECT: get 99: -1
    printf("get 99: %d\n", ht_get(&ht, 99));

    // EXPECT: has 7: 1
    printf("has 7: %d\n", ht_contains(&ht, 7));
    // EXPECT: has 42: 0
    printf("has 42: %d\n", ht_contains(&ht, 42));

    ht_insert(&ht, 5, 55);
    // EXPECT: updated 5: 55
    printf("updated 5: %d\n", ht_get(&ht, 5));
    // EXPECT: count after update: 5
    printf("count after update: %d\n", ht.count);

    keys[0] = 0; keys[1] = 16; keys[2] = 32; keys[3] = 48;
    keys[4] = 1; keys[5] = 2; keys[6] = 3; keys[7] = 4;
    // EXPECT: collisions mod16: 6
    printf("collisions mod16: %d\n", count_collisions(keys, 8, 16));

    keys[0] = 1; keys[1] = 2; keys[2] = 3; keys[3] = 4;
    keys[4] = 5; keys[5] = 6; keys[6] = 7; keys[7] = 8;
    // EXPECT: no collisions: 0
    printf("no collisions: %d\n", count_collisions(keys, 8, 16));

    // EXPECT: done
    printf("done\n");

    return 0;
}
