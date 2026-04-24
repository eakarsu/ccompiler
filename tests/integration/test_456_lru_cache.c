int printf(const char *fmt, ...);
// EXPECT: After 3 inserts: [3:30, 2:20, 1:10]\nGet 1: 10\nAfter get 1: [1:10, 3:30, 2:20]\nAfter more inserts: [5:50, 4:40, 1:10, 3:30, 2:20]\nGet 2: 20\nAfter get 2: [2:20, 5:50, 4:40, 1:10, 3:30]\nAfter update 3: [3:300, 2:20, 5:50, 4:40, 1:10]\nSize: 5\nGet 99: -1\nDone

// LRU cache with array-based doubly linked list

struct LRUEntry {
    int key;
    int value;
    int prev;
    int next;
    int valid;
};

struct LRUCache {
    struct LRUEntry entries[8];
    int head;
    int tail;
    int size;
    int capacity;
};

void lru_init(struct LRUCache *c) {
    c->head = -1;
    c->tail = -1;
    c->size = 0;
    c->capacity = 8;
    int i = 0;
    while (i < 8) {
        c->entries[i].valid = 0;
        c->entries[i].prev = -1;
        c->entries[i].next = -1;
        i = i + 1;
    }
}

int lru_find_slot(struct LRUCache *c) {
    int i = 0;
    while (i < c->capacity) {
        if (!c->entries[i].valid) return i;
        i = i + 1;
    }
    return -1;
}

void lru_remove_node(struct LRUCache *c, int idx) {
    int p = c->entries[idx].prev;
    int n = c->entries[idx].next;
    if (p >= 0) c->entries[p].next = n;
    else c->head = n;
    if (n >= 0) c->entries[n].prev = p;
    else c->tail = p;
}

void lru_add_front(struct LRUCache *c, int idx) {
    c->entries[idx].prev = -1;
    c->entries[idx].next = c->head;
    if (c->head >= 0) c->entries[c->head].prev = idx;
    c->head = idx;
    if (c->tail < 0) c->tail = idx;
}

int lru_get(struct LRUCache *c, int key) {
    int i = 0;
    while (i < c->capacity) {
        if (c->entries[i].valid && c->entries[i].key == key) {
            lru_remove_node(c, i);
            lru_add_front(c, i);
            return c->entries[i].value;
        }
        i = i + 1;
    }
    return -1;
}

void lru_put(struct LRUCache *c, int key, int value) {
    int i = 0;
    while (i < c->capacity) {
        if (c->entries[i].valid && c->entries[i].key == key) {
            c->entries[i].value = value;
            lru_remove_node(c, i);
            lru_add_front(c, i);
            return;
        }
        i = i + 1;
    }
    if (c->size >= c->capacity) {
        int evict = c->tail;
        lru_remove_node(c, evict);
        c->entries[evict].valid = 0;
        c->size = c->size - 1;
    }
    int slot = lru_find_slot(c);
    c->entries[slot].key = key;
    c->entries[slot].value = value;
    c->entries[slot].valid = 1;
    lru_add_front(c, slot);
    c->size = c->size + 1;
}

void lru_print_order(struct LRUCache *c) {
    int idx = c->head;
    printf("[");
    int first = 1;
    while (idx >= 0) {
        if (!first) printf(", ");
        printf("%d:%d", c->entries[idx].key, c->entries[idx].value);
        first = 0;
        idx = c->entries[idx].next;
    }
    printf("]\n");
}

int main(void) {
    struct LRUCache cache;
    lru_init(&cache);

    lru_put(&cache, 1, 10);
    lru_put(&cache, 2, 20);
    lru_put(&cache, 3, 30);
    printf("After 3 inserts: ");
    lru_print_order(&cache);

    int v = lru_get(&cache, 1);
    printf("Get 1: %d\n", v);
    printf("After get 1: ");
    lru_print_order(&cache);

    lru_put(&cache, 4, 40);
    lru_put(&cache, 5, 50);
    printf("After more inserts: ");
    lru_print_order(&cache);

    printf("Get 2: %d\n", lru_get(&cache, 2));
    printf("After get 2: ");
    lru_print_order(&cache);

    // Update existing
    lru_put(&cache, 3, 300);
    printf("After update 3: ");
    lru_print_order(&cache);

    printf("Size: %d\n", cache.size);

    // Miss
    printf("Get 99: %d\n", lru_get(&cache, 99));

    printf("Done\n");
    return 0;
}
