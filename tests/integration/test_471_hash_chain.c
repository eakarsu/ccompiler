int printf(const char *fmt, ...);
// EXPECT: Size: 8\nGet 5: 50 (found=1)\nGet 13: 130 (found=1)\nGet 21: 210 (found=1)\nGet 99: found=0\nBucket 5 chain: 3\nBucket 3 chain: 1\nUpdated 5: 500\nSize after update: 8\nRemove 13: 1\nSize after remove: 7\nGet 13 after remove: found=0\nBucket 5 chain after remove: 2\nDone

// Hash table with separate chaining

struct HashNode {
    int key;
    int value;
    int next;
    int valid;
};

struct HashTable {
    int buckets[8];
    struct HashNode pool[32];
    int pool_next;
    int size;
};

void ht_init(struct HashTable *ht) {
    ht->pool_next = 0;
    ht->size = 0;
    int i = 0;
    while (i < 8) {
        ht->buckets[i] = -1;
        i = i + 1;
    }
    i = 0;
    while (i < 32) {
        ht->pool[i].valid = 0;
        ht->pool[i].next = -1;
        i = i + 1;
    }
}

int ht_hash(int key) {
    int h = key;
    if (h < 0) h = -h;
    return h % 8;
}

void ht_put(struct HashTable *ht, int key, int value) {
    int h = ht_hash(key);
    // Check if key exists
    int idx = ht->buckets[h];
    while (idx >= 0) {
        if (ht->pool[idx].key == key) {
            ht->pool[idx].value = value;
            return;
        }
        idx = ht->pool[idx].next;
    }
    // New entry
    int new_idx = ht->pool_next;
    ht->pool_next = ht->pool_next + 1;
    ht->pool[new_idx].key = key;
    ht->pool[new_idx].value = value;
    ht->pool[new_idx].valid = 1;
    ht->pool[new_idx].next = ht->buckets[h];
    ht->buckets[h] = new_idx;
    ht->size = ht->size + 1;
}

int ht_get(struct HashTable *ht, int key, int *found) {
    int h = ht_hash(key);
    int idx = ht->buckets[h];
    while (idx >= 0) {
        if (ht->pool[idx].key == key) {
            *found = 1;
            return ht->pool[idx].value;
        }
        idx = ht->pool[idx].next;
    }
    *found = 0;
    return 0;
}

int ht_remove(struct HashTable *ht, int key) {
    int h = ht_hash(key);
    int prev = -1;
    int idx = ht->buckets[h];
    while (idx >= 0) {
        if (ht->pool[idx].key == key) {
            if (prev < 0) {
                ht->buckets[h] = ht->pool[idx].next;
            } else {
                ht->pool[prev].next = ht->pool[idx].next;
            }
            ht->pool[idx].valid = 0;
            ht->size = ht->size - 1;
            return 1;
        }
        prev = idx;
        idx = ht->pool[idx].next;
    }
    return 0;
}

int ht_chain_length(struct HashTable *ht, int bucket) {
    int count = 0;
    int idx = ht->buckets[bucket];
    while (idx >= 0) {
        count = count + 1;
        idx = ht->pool[idx].next;
    }
    return count;
}

int main(void) {
    struct HashTable ht;
    ht_init(&ht);

    // Insert various keys
    ht_put(&ht, 5, 50);
    ht_put(&ht, 13, 130);  // same bucket as 5
    ht_put(&ht, 21, 210);  // same bucket as 5
    ht_put(&ht, 3, 30);
    ht_put(&ht, 10, 100);
    ht_put(&ht, 7, 70);
    ht_put(&ht, 15, 150);
    ht_put(&ht, 22, 220);

    printf("Size: %d\n", ht.size);

    int found;
    int v;
    v = ht_get(&ht, 5, &found);
    printf("Get 5: %d (found=%d)\n", v, found);
    v = ht_get(&ht, 13, &found);
    printf("Get 13: %d (found=%d)\n", v, found);
    v = ht_get(&ht, 21, &found);
    printf("Get 21: %d (found=%d)\n", v, found);
    v = ht_get(&ht, 99, &found);
    printf("Get 99: found=%d\n", found);

    // Chain lengths
    printf("Bucket 5 chain: %d\n", ht_chain_length(&ht, 5));
    printf("Bucket 3 chain: %d\n", ht_chain_length(&ht, 3));

    // Update
    ht_put(&ht, 5, 500);
    v = ht_get(&ht, 5, &found);
    printf("Updated 5: %d\n", v);
    printf("Size after update: %d\n", ht.size);

    // Remove
    int removed = ht_remove(&ht, 13);
    printf("Remove 13: %d\n", removed);
    printf("Size after remove: %d\n", ht.size);
    v = ht_get(&ht, 13, &found);
    printf("Get 13 after remove: found=%d\n", found);

    // Check chain after removal
    printf("Bucket 5 chain after remove: %d\n", ht_chain_length(&ht, 5));

    printf("Done\n");
    return 0;
}
