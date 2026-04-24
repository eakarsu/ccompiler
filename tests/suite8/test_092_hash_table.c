int printf(const char *fmt, ...);

/* Hash table with open addressing (linear probing) */

enum { TABLE_SIZE = 32, EMPTY = 0, OCCUPIED = 1, DELETED = 2 };

struct Entry {
    int key;
    int value;
    int state;
};

struct HashTable {
    struct Entry entries[32];
    int count;
};

void ht_init(struct HashTable *ht) {
    int i;
    for (i = 0; i < TABLE_SIZE; i = i + 1) {
        ht->entries[i].key = 0;
        ht->entries[i].value = 0;
        ht->entries[i].state = EMPTY;
    }
    ht->count = 0;
}

int ht_hash(int key) {
    int h;
    h = key;
    if (h < 0) h = -h;
    return h % TABLE_SIZE;
}

int ht_insert(struct HashTable *ht, int key, int value) {
    int idx;
    int start;
    int i;
    idx = ht_hash(key);
    start = idx;
    for (i = 0; i < TABLE_SIZE; i = i + 1) {
        int pos;
        pos = (start + i) % TABLE_SIZE;
        if (ht->entries[pos].state == EMPTY || ht->entries[pos].state == DELETED) {
            ht->entries[pos].key = key;
            ht->entries[pos].value = value;
            ht->entries[pos].state = OCCUPIED;
            ht->count = ht->count + 1;
            return 1;
        }
        if (ht->entries[pos].state == OCCUPIED && ht->entries[pos].key == key) {
            ht->entries[pos].value = value;
            return 1;
        }
    }
    return 0;
}

int ht_find(struct HashTable *ht, int key, int *out_value) {
    int start;
    int i;
    start = ht_hash(key);
    for (i = 0; i < TABLE_SIZE; i = i + 1) {
        int pos;
        pos = (start + i) % TABLE_SIZE;
        if (ht->entries[pos].state == EMPTY) {
            return 0;
        }
        if (ht->entries[pos].state == OCCUPIED && ht->entries[pos].key == key) {
            *out_value = ht->entries[pos].value;
            return 1;
        }
    }
    return 0;
}

int ht_delete(struct HashTable *ht, int key) {
    int start;
    int i;
    start = ht_hash(key);
    for (i = 0; i < TABLE_SIZE; i = i + 1) {
        int pos;
        pos = (start + i) % TABLE_SIZE;
        if (ht->entries[pos].state == EMPTY) {
            return 0;
        }
        if (ht->entries[pos].state == OCCUPIED && ht->entries[pos].key == key) {
            ht->entries[pos].state = DELETED;
            ht->count = ht->count - 1;
            return 1;
        }
    }
    return 0;
}

void test_insert_find(void) {
    struct HashTable ht;
    int val;
    int found;
    ht_init(&ht);
    ht_insert(&ht, 10, 100);
    ht_insert(&ht, 20, 200);
    ht_insert(&ht, 30, 300);
    // EXPECT: count=3
    printf("count=%d\n", ht.count);
    found = ht_find(&ht, 10, &val);
    // EXPECT: find 10: found=1 val=100
    printf("find 10: found=%d val=%d\n", found, val);
    found = ht_find(&ht, 20, &val);
    // EXPECT: find 20: found=1 val=200
    printf("find 20: found=%d val=%d\n", found, val);
    found = ht_find(&ht, 99, &val);
    // EXPECT: find 99: found=0
    printf("find 99: found=%d\n", found);
}

void test_update(void) {
    struct HashTable ht;
    int val;
    ht_init(&ht);
    ht_insert(&ht, 5, 50);
    ht_insert(&ht, 5, 999);
    // EXPECT: update count=1
    printf("update count=%d\n", ht.count);
    ht_find(&ht, 5, &val);
    // EXPECT: updated val=999
    printf("updated val=%d\n", val);
}

void test_delete(void) {
    struct HashTable ht;
    int val;
    int found;
    int del;
    ht_init(&ht);
    ht_insert(&ht, 42, 420);
    ht_insert(&ht, 74, 740);
    del = ht_delete(&ht, 42);
    // EXPECT: delete 42: del=1 count=1
    printf("delete 42: del=%d count=%d\n", del, ht.count);
    found = ht_find(&ht, 42, &val);
    // EXPECT: after delete find 42: found=0
    printf("after delete find 42: found=%d\n", found);
    found = ht_find(&ht, 74, &val);
    // EXPECT: after delete find 74: found=1 val=740
    printf("after delete find 74: found=%d val=%d\n", found, val);
}

void test_collision_chain(void) {
    struct HashTable ht;
    int val;
    int found;
    ht_init(&ht);
    ht_insert(&ht, 1, 10);
    ht_insert(&ht, 33, 330);
    ht_insert(&ht, 65, 650);
    found = ht_find(&ht, 1, &val);
    // EXPECT: chain 1: found=1 val=10
    printf("chain 1: found=%d val=%d\n", found, val);
    found = ht_find(&ht, 33, &val);
    // EXPECT: chain 33: found=1 val=330
    printf("chain 33: found=%d val=%d\n", found, val);
    found = ht_find(&ht, 65, &val);
    // EXPECT: chain 65: found=1 val=650
    printf("chain 65: found=%d val=%d\n", found, val);
    // EXPECT: chain count=3
    printf("chain count=%d\n", ht.count);
}

void test_reinsert_after_delete(void) {
    struct HashTable ht;
    int val;
    ht_init(&ht);
    ht_insert(&ht, 7, 70);
    ht_delete(&ht, 7);
    ht_insert(&ht, 7, 777);
    ht_find(&ht, 7, &val);
    // EXPECT: reinsert val=777
    printf("reinsert val=%d\n", val);
}

int main(void) {
    test_insert_find();
    test_update();
    test_delete();
    test_collision_chain();
    test_reinsert_after_delete();
    // EXPECT: done
    printf("done\n");
    return 0;
}
