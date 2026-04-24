int printf(const char *fmt, ...);
// EXPECT: === Hash Table Tests ===
// EXPECT: --- Create hash table ---
// EXPECT: table created with 16 buckets
// EXPECT: --- Insert and lookup ---
// EXPECT: insert(10, 100)
// EXPECT: insert(25, 250)
// EXPECT: insert(3, 30)
// EXPECT: lookup(10): 100
// EXPECT: lookup(25): 250
// EXPECT: lookup(3): 30
// EXPECT: lookup(99): -1
// EXPECT: --- Update existing key ---
// EXPECT: insert(10, 999)
// EXPECT: lookup(10): 999
// EXPECT: --- Delete ---
// EXPECT: delete(25): 1
// EXPECT: lookup(25): -1
// EXPECT: delete(25): 0
// EXPECT: --- Collision handling ---
// EXPECT: insert(0, 10)
// EXPECT: insert(16, 20)
// EXPECT: insert(32, 30)
// EXPECT: lookup(0): 10
// EXPECT: lookup(16): 20
// EXPECT: lookup(32): 30
// EXPECT: --- Delete with collisions ---
// EXPECT: delete(16): 1
// EXPECT: lookup(0): 10
// EXPECT: lookup(16): -1
// EXPECT: lookup(32): 30
// EXPECT: --- Many insertions ---
// EXPECT: inserted 50 items
// EXPECT: all lookups correct: 1
// EXPECT: --- Count and size ---
// EXPECT: count: 54
// EXPECT: --- Contains check ---
// EXPECT: contains(10): 1
// EXPECT: contains(999): 0
// EXPECT: --- Clear table ---
// EXPECT: count after clear: 0
// EXPECT: lookup(10): -1
// EXPECT: --- Rehash test ---
// EXPECT: inserted 100 items, all correct: 1
// EXPECT: --- Hash function distribution ---
// EXPECT: bucket 0: used
// EXPECT: empty_buckets <= 5: 1
// EXPECT: --- Free table ---
// EXPECT: table freed
// EXPECT: === Hash Table Done ===
void *malloc(unsigned long size);
void free(void *ptr);
// --- Create hash table ---
// table created with 16 buckets
// --- Insert and lookup ---
// insert(10, 100)
// insert(25, 250)
// insert(3, 30)
// lookup(10): 100
// lookup(25): 250
// lookup(3): 30
// lookup(99): -1
// --- Update existing key ---
// insert(10, 999)
// lookup(10): 999
// --- Delete ---
// delete(25): 1
// lookup(25): -1
// delete(25): 0
// --- Collision handling ---
// insert(0, 10)
// insert(16, 20)
// insert(32, 30)
// lookup(0): 10
// lookup(16): 20
// lookup(32): 30
// --- Delete with collisions ---
// delete(16): 1
// lookup(0): 10
// lookup(16): -1
// lookup(32): 30
// --- Many insertions ---
// inserted 50 items
// all lookups correct: 1
// --- Count and size ---
// count: 54
// --- Contains check ---
// contains(10): 1
// contains(999): 0
// --- Clear table ---
// count after clear: 0
// lookup(10): -1
// --- Rehash test ---
// inserted 100 items, all correct: 1
// --- Hash function distribution ---
// bucket 0: used
// empty_buckets <= 5: 1
// --- Free table ---
// table freed
// === Hash Table Done ===

// Simple linked-list node for chaining
struct Node {
    int key;
    int value;
    struct Node *next;
};

struct HashTable {
    struct Node **buckets;
    int size;
    int count;
};

int hash_func(int key, int size) {
    int h = key % size;
    if (h < 0) h = h + size;
    return h;
}

struct HashTable *ht_create(int size) {
    struct HashTable *ht = (struct HashTable *)malloc(sizeof(struct HashTable));
    ht->size = size;
    ht->count = 0;
    ht->buckets = (struct Node **)malloc(sizeof(struct Node *) * size);
    int i;
    for (i = 0; i < size; i++) {
        ht->buckets[i] = (struct Node *)0;
    }
    return ht;
}

void ht_insert(struct HashTable *ht, int key, int value) {
    int h = hash_func(key, ht->size);
    struct Node *cur = ht->buckets[h];
    // Check if key exists, update
    while (cur) {
        if (cur->key == key) {
            cur->value = value;
            return;
        }
        cur = cur->next;
    }
    // Insert new node at head
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->key = key;
    node->value = value;
    node->next = ht->buckets[h];
    ht->buckets[h] = node;
    ht->count = ht->count + 1;
}

int ht_lookup(struct HashTable *ht, int key) {
    int h = hash_func(key, ht->size);
    struct Node *cur = ht->buckets[h];
    while (cur) {
        if (cur->key == key) return cur->value;
        cur = cur->next;
    }
    return -1; // not found
}

int ht_contains(struct HashTable *ht, int key) {
    int h = hash_func(key, ht->size);
    struct Node *cur = ht->buckets[h];
    while (cur) {
        if (cur->key == key) return 1;
        cur = cur->next;
    }
    return 0;
}

int ht_delete(struct HashTable *ht, int key) {
    int h = hash_func(key, ht->size);
    struct Node *cur = ht->buckets[h];
    struct Node *prev = (struct Node *)0;
    while (cur) {
        if (cur->key == key) {
            if (prev) {
                prev->next = cur->next;
            } else {
                ht->buckets[h] = cur->next;
            }
            free(cur);
            ht->count = ht->count - 1;
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

void ht_clear(struct HashTable *ht) {
    int i;
    for (i = 0; i < ht->size; i++) {
        struct Node *cur = ht->buckets[i];
        while (cur) {
            struct Node *tmp = cur;
            cur = cur->next;
            free(tmp);
        }
        ht->buckets[i] = (struct Node *)0;
    }
    ht->count = 0;
}

void ht_free(struct HashTable *ht) {
    ht_clear(ht);
    free(ht->buckets);
    free(ht);
}

int main() {
    printf("=== Hash Table Tests ===\n");

    // Create
    printf("--- Create hash table ---\n");
    struct HashTable *ht = ht_create(16);
    printf("table created with %d buckets\n", ht->size);

    // Insert and lookup
    printf("--- Insert and lookup ---\n");
    printf("insert(10, 100)\n");
    ht_insert(ht, 10, 100);
    printf("insert(25, 250)\n");
    ht_insert(ht, 25, 250);
    printf("insert(3, 30)\n");
    ht_insert(ht, 3, 30);
    printf("lookup(10): %d\n", ht_lookup(ht, 10));
    printf("lookup(25): %d\n", ht_lookup(ht, 25));
    printf("lookup(3): %d\n", ht_lookup(ht, 3));
    printf("lookup(99): %d\n", ht_lookup(ht, 99));

    // Update existing key
    printf("--- Update existing key ---\n");
    printf("insert(10, 999)\n");
    ht_insert(ht, 10, 999);
    printf("lookup(10): %d\n", ht_lookup(ht, 10));

    // Delete
    printf("--- Delete ---\n");
    printf("delete(25): %d\n", ht_delete(ht, 25));
    printf("lookup(25): %d\n", ht_lookup(ht, 25));
    printf("delete(25): %d\n", ht_delete(ht, 25));

    // Collision handling (keys 0, 16, 32 all hash to bucket 0 with size=16)
    printf("--- Collision handling ---\n");
    printf("insert(0, 10)\n");
    ht_insert(ht, 0, 10);
    printf("insert(16, 20)\n");
    ht_insert(ht, 16, 20);
    printf("insert(32, 30)\n");
    ht_insert(ht, 32, 30);
    printf("lookup(0): %d\n", ht_lookup(ht, 0));
    printf("lookup(16): %d\n", ht_lookup(ht, 16));
    printf("lookup(32): %d\n", ht_lookup(ht, 32));

    // Delete with collisions
    printf("--- Delete with collisions ---\n");
    printf("delete(16): %d\n", ht_delete(ht, 16));
    printf("lookup(0): %d\n", ht_lookup(ht, 0));
    printf("lookup(16): %d\n", ht_lookup(ht, 16));
    printf("lookup(32): %d\n", ht_lookup(ht, 32));

    // Many insertions
    printf("--- Many insertions ---\n");
    int i;
    for (i = 100; i < 150; i++) {
        ht_insert(ht, i, i * 10);
    }
    printf("inserted 50 items\n");
    int all_correct = 1;
    for (i = 100; i < 150; i++) {
        if (ht_lookup(ht, i) != i * 10) {
            all_correct = 0;
        }
    }
    printf("all lookups correct: %d\n", all_correct);

    // Count
    printf("--- Count and size ---\n");
    printf("count: %d\n", ht->count);

    // Contains
    printf("--- Contains check ---\n");
    printf("contains(10): %d\n", ht_contains(ht, 10));
    printf("contains(999): %d\n", ht_contains(ht, 999));

    // Clear
    printf("--- Clear table ---\n");
    ht_clear(ht);
    printf("count after clear: %d\n", ht->count);
    printf("lookup(10): %d\n", ht_lookup(ht, 10));

    // Rehash test - insert many items into fresh table
    printf("--- Rehash test ---\n");
    for (i = 0; i < 100; i++) {
        ht_insert(ht, i, i * 7);
    }
    all_correct = 1;
    for (i = 0; i < 100; i++) {
        if (ht_lookup(ht, i) != i * 7) {
            all_correct = 0;
        }
    }
    printf("inserted 100 items, all correct: %d\n", all_correct);

    // Hash function distribution
    printf("--- Hash function distribution ---\n");
    int empty = 0;
    for (i = 0; i < ht->size; i++) {
        if (ht->buckets[i] == (struct Node *)0) {
            empty = empty + 1;
        }
    }
    if (ht->buckets[0] != (struct Node *)0) {
        printf("bucket 0: used\n");
    }
    printf("empty_buckets <= 5: %d\n", empty <= 5);

    // Free
    printf("--- Free table ---\n");
    ht_free(ht);
    printf("table freed\n");

    printf("=== Hash Table Done ===\n");
    return 0;
}
