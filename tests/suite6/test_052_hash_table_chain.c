int printf(const char *fmt, ...);

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int hash_str(char *s, int mod) {
    int hash = 5381;
    int i = 0;
    while (s[i] != 0) {
        hash = hash * 33 + s[i];
        i = i + 1;
    }
    if (hash < 0) hash = -hash;
    return hash % mod;
}

struct Node {
    char *key;
    int value;
    int next;
};

struct Node pool[64];
int pool_next;

int buckets[16];

void ht_init(void) {
    int i;
    for (i = 0; i < 16; i = i + 1) {
        buckets[i] = -1;
    }
    pool_next = 0;
}

void ht_insert(char *key, int value) {
    int b = hash_str(key, 16);
    int idx = buckets[b];
    while (idx != -1) {
        if (str_equal(pool[idx].key, key)) {
            pool[idx].value = value;
            return;
        }
        idx = pool[idx].next;
    }
    int n = pool_next;
    pool_next = pool_next + 1;
    pool[n].key = key;
    pool[n].value = value;
    pool[n].next = buckets[b];
    buckets[b] = n;
}

int ht_find(char *key, int *found) {
    int b = hash_str(key, 16);
    int idx = buckets[b];
    while (idx != -1) {
        if (str_equal(pool[idx].key, key)) {
            *found = 1;
            return pool[idx].value;
        }
        idx = pool[idx].next;
    }
    *found = 0;
    return -1;
}

int ht_delete(char *key) {
    int b = hash_str(key, 16);
    int prev = -1;
    int idx = buckets[b];
    while (idx != -1) {
        if (str_equal(pool[idx].key, key)) {
            if (prev == -1) {
                buckets[b] = pool[idx].next;
            } else {
                pool[prev].next = pool[idx].next;
            }
            return 1;
        }
        prev = idx;
        idx = pool[idx].next;
    }
    return 0;
}

int ht_count(void) {
    int count = 0;
    int i;
    for (i = 0; i < 16; i = i + 1) {
        int idx = buckets[i];
        while (idx != -1) {
            count = count + 1;
            idx = pool[idx].next;
        }
    }
    return count;
}

int main(void) {
    int found;
    int val;

    ht_init();

    // EXPECT: count after init: 0
    printf("count after init: %d\n", ht_count());

    ht_insert("apple", 10);
    // EXPECT: count after 1 insert: 1
    printf("count after 1 insert: %d\n", ht_count());

    ht_insert("banana", 20);
    ht_insert("cherry", 30);
    // EXPECT: count after 3 inserts: 3
    printf("count after 3 inserts: %d\n", ht_count());

    // EXPECT: find apple: 10
    val = ht_find("apple", &found);
    printf("find apple: %d\n", val);

    // EXPECT: apple found: 1
    printf("apple found: %d\n", found);

    // EXPECT: find banana: 20
    val = ht_find("banana", &found);
    printf("find banana: %d\n", val);

    // EXPECT: find cherry: 30
    val = ht_find("cherry", &found);
    printf("find cherry: %d\n", val);

    // EXPECT: find missing: 0
    ht_find("date", &found);
    printf("find missing: %d\n", found);

    ht_insert("apple", 99);
    // EXPECT: update apple: 99
    val = ht_find("apple", &found);
    printf("update apple: %d\n", val);

    // EXPECT: count after update: 3
    printf("count after update: %d\n", ht_count());

    // EXPECT: delete banana: 1
    int del = ht_delete("banana");
    printf("delete banana: %d\n", del);

    // EXPECT: count after delete: 2
    printf("count after delete: %d\n", ht_count());

    // EXPECT: banana gone: 0
    ht_find("banana", &found);
    printf("banana gone: %d\n", found);

    // EXPECT: delete missing: 0
    del = ht_delete("xyz");
    printf("delete missing: %d\n", del);

    ht_insert("date", 40);
    ht_insert("elder", 50);
    ht_insert("fig", 60);
    // EXPECT: count after more inserts: 5
    printf("count after more inserts: %d\n", ht_count());

    // EXPECT: find elder: 50
    val = ht_find("elder", &found);
    printf("find elder: %d\n", val);

    // EXPECT: find fig: 60
    val = ht_find("fig", &found);
    printf("find fig: %d\n", val);

    // EXPECT: cherry still there: 30
    val = ht_find("cherry", &found);
    printf("cherry still there: %d\n", val);

    // EXPECT: done
    printf("done\n");

    return 0;
}
