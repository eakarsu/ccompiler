int printf(const char *fmt, ...);
// EXPECT: size: 5\nget(1): 100\nget(2): 200\nget(17): 1700\nget(33): 3300\nget(5): 500\nget(99): -1\ncontains(1): 1\ncontains(99): 0\nget(1) updated: 999\nsize unchanged: 5\nfinal size: 15\nget(105): 1050
// Test: hash map entry with structs

struct Entry {
    int key;
    int value;
    int occupied;
};

struct HashMap {
    struct Entry buckets[16];
    int size;
};

int hash_func(int key) {
    int h = key;
    if (h < 0) h = -h;
    return h % 16;
}

void map_init(struct HashMap *m) {
    int i;
    for (i = 0; i < 16; i++) {
        m->buckets[i].occupied = 0;
    }
    m->size = 0;
}

void map_put(struct HashMap *m, int key, int value) {
    int idx = hash_func(key);
    int i;
    for (i = 0; i < 16; i++) {
        int pos = (idx + i) % 16;
        if (!m->buckets[pos].occupied) {
            m->buckets[pos].key = key;
            m->buckets[pos].value = value;
            m->buckets[pos].occupied = 1;
            m->size = m->size + 1;
            return;
        }
        if (m->buckets[pos].key == key) {
            m->buckets[pos].value = value;
            return;
        }
    }
}

int map_get(struct HashMap *m, int key) {
    int idx = hash_func(key);
    int i;
    for (i = 0; i < 16; i++) {
        int pos = (idx + i) % 16;
        if (!m->buckets[pos].occupied) return -1;
        if (m->buckets[pos].key == key) {
            return m->buckets[pos].value;
        }
    }
    return -1;
}

int map_contains(struct HashMap *m, int key) {
    int idx = hash_func(key);
    int i;
    for (i = 0; i < 16; i++) {
        int pos = (idx + i) % 16;
        if (!m->buckets[pos].occupied) return 0;
        if (m->buckets[pos].key == key) return 1;
    }
    return 0;
}

int main(void) {
    struct HashMap m;
    map_init(&m);

    map_put(&m, 1, 100);
    map_put(&m, 2, 200);
    map_put(&m, 17, 1700);
    map_put(&m, 33, 3300);
    map_put(&m, 5, 500);

    printf("size: %d\n", m.size);
    printf("get(1): %d\n", map_get(&m, 1));
    printf("get(2): %d\n", map_get(&m, 2));
    printf("get(17): %d\n", map_get(&m, 17));
    printf("get(33): %d\n", map_get(&m, 33));
    printf("get(5): %d\n", map_get(&m, 5));
    printf("get(99): %d\n", map_get(&m, 99));
    printf("contains(1): %d\n", map_contains(&m, 1));
    printf("contains(99): %d\n", map_contains(&m, 99));

    map_put(&m, 1, 999);
    printf("get(1) updated: %d\n", map_get(&m, 1));
    printf("size unchanged: %d\n", m.size);

    int i;
    for (i = 100; i < 110; i++) {
        map_put(&m, i, i * 10);
    }
    printf("final size: %d\n", m.size);
    printf("get(105): %d\n", map_get(&m, 105));

    return 0;
}
