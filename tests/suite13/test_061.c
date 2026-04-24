int printf(const char *fmt, ...);

// Open addressing hash table with linear probing

enum { TABLE_SIZE = 16, EMPTY = -1, DELETED = -2 };

struct Entry {
    int key;
    int value;
};

struct HashTable {
    struct Entry entries[16];
    int count;
};

int hash_func(int key) {
    int h = key;
    if (h < 0) h = -h;
    return h % TABLE_SIZE;
}

void ht_init(struct HashTable *ht) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        ht->entries[i].key = EMPTY;
        ht->entries[i].value = 0;
    }
    ht->count = 0;
}

int ht_insert(struct HashTable *ht, int key, int value) {
    int idx;
    int start;
    if (ht->count >= TABLE_SIZE) return 0;
    idx = hash_func(key);
    start = idx;
    while (1) {
        if (ht->entries[idx].key == EMPTY || ht->entries[idx].key == DELETED) {
            ht->entries[idx].key = key;
            ht->entries[idx].value = value;
            ht->count = ht->count + 1;
            return 1;
        }
        if (ht->entries[idx].key == key) {
            ht->entries[idx].value = value;
            return 1;
        }
        idx = (idx + 1) % TABLE_SIZE;
        if (idx == start) return 0;
    }
    return 0;
}

int ht_get(struct HashTable *ht, int key, int *out) {
    int idx;
    int start;
    idx = hash_func(key);
    start = idx;
    while (1) {
        if (ht->entries[idx].key == EMPTY) return 0;
        if (ht->entries[idx].key == key) {
            *out = ht->entries[idx].value;
            return 1;
        }
        idx = (idx + 1) % TABLE_SIZE;
        if (idx == start) return 0;
    }
    return 0;
}

int ht_delete(struct HashTable *ht, int key) {
    int idx;
    int start;
    idx = hash_func(key);
    start = idx;
    while (1) {
        if (ht->entries[idx].key == EMPTY) return 0;
        if (ht->entries[idx].key == key) {
            ht->entries[idx].key = DELETED;
            ht->entries[idx].value = 0;
            ht->count = ht->count - 1;
            return 1;
        }
        idx = (idx + 1) % TABLE_SIZE;
        if (idx == start) return 0;
    }
    return 0;
}

int ht_contains(struct HashTable *ht, int key) {
    int val;
    return ht_get(ht, key, &val);
}

int main() {
    struct HashTable ht;
    int val;
    int found;
    int i;

    ht_init(&ht);

    printf("%d\n", ht.count);
    // EXPECT: 0

    ht_insert(&ht, 5, 50);
    ht_insert(&ht, 21, 210);
    ht_insert(&ht, 37, 370);

    printf("%d\n", ht.count);
    // EXPECT: 3

    found = ht_get(&ht, 5, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 50

    found = ht_get(&ht, 21, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 210

    found = ht_get(&ht, 37, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 370

    found = ht_get(&ht, 99, &val);
    printf("%d\n", found);
    // EXPECT: 0

    ht_insert(&ht, 5, 55);
    found = ht_get(&ht, 5, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 55

    printf("%d\n", ht.count);
    // EXPECT: 3

    ht_delete(&ht, 21);
    printf("%d\n", ht.count);
    // EXPECT: 2

    found = ht_get(&ht, 21, &val);
    printf("%d\n", found);
    // EXPECT: 0

    found = ht_get(&ht, 37, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 370

    for (i = 0; i < 10; i++) {
        ht_insert(&ht, i * 16, i * 100);
    }
    printf("%d\n", ht.count);
    // EXPECT: 12

    found = ht_get(&ht, 48, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 300

    found = ht_get(&ht, 144, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 900

    printf("%d\n", ht_contains(&ht, 5));
    // EXPECT: 1

    printf("%d\n", ht_contains(&ht, 999));
    // EXPECT: 0

    ht_delete(&ht, 0);
    found = ht_get(&ht, 16, &val);
    printf("%d %d\n", found, val);
    // EXPECT: 1 100

    printf("%d\n", ht.count);
    // EXPECT: 11

    return 0;
}
