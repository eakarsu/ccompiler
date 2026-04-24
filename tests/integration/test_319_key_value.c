int printf(const char *fmt, ...);
// EXPECT: count: 4\nget(1): 100\nget(3): 300\nget(4): -1\nhas(2): 1\nhas(9): 0\nsum: 1100\nget(2) after update: 999\ncount after remove: 3\nhas(3) after remove: 0\nsum after changes: 1599
// Test: struct for key-value pairs

struct KV {
    int key;
    int value;
};

struct KVStore {
    struct KV items[20];
    int count;
};

void store_init(struct KVStore *s) {
    s->count = 0;
}

void store_put(struct KVStore *s, int key, int value) {
    int i;
    for (i = 0; i < s->count; i++) {
        if (s->items[i].key == key) {
            s->items[i].value = value;
            return;
        }
    }
    if (s->count < 20) {
        s->items[s->count].key = key;
        s->items[s->count].value = value;
        s->count = s->count + 1;
    }
}

int store_get(struct KVStore *s, int key) {
    int i;
    for (i = 0; i < s->count; i++) {
        if (s->items[i].key == key) {
            return s->items[i].value;
        }
    }
    return -1;
}

int store_has(struct KVStore *s, int key) {
    int i;
    for (i = 0; i < s->count; i++) {
        if (s->items[i].key == key) return 1;
    }
    return 0;
}

int store_sum_values(struct KVStore *s) {
    int sum = 0;
    int i;
    for (i = 0; i < s->count; i++) {
        sum = sum + s->items[i].value;
    }
    return sum;
}

void store_remove(struct KVStore *s, int key) {
    int i;
    for (i = 0; i < s->count; i++) {
        if (s->items[i].key == key) {
            int j;
            for (j = i; j < s->count - 1; j++) {
                s->items[j] = s->items[j + 1];
            }
            s->count = s->count - 1;
            return;
        }
    }
}

int main(void) {
    struct KVStore store;
    store_init(&store);

    store_put(&store, 1, 100);
    store_put(&store, 2, 200);
    store_put(&store, 3, 300);
    store_put(&store, 5, 500);

    printf("count: %d\n", store.count);
    printf("get(1): %d\n", store_get(&store, 1));
    printf("get(3): %d\n", store_get(&store, 3));
    printf("get(4): %d\n", store_get(&store, 4));
    printf("has(2): %d\n", store_has(&store, 2));
    printf("has(9): %d\n", store_has(&store, 9));
    printf("sum: %d\n", store_sum_values(&store));

    store_put(&store, 2, 999);
    printf("get(2) after update: %d\n", store_get(&store, 2));

    store_remove(&store, 3);
    printf("count after remove: %d\n", store.count);
    printf("has(3) after remove: %d\n", store_has(&store, 3));
    printf("sum after changes: %d\n", store_sum_values(&store));

    return 0;
}
