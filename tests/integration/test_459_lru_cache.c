int printf(const char *fmt, ...);
// EXPECT: Cache: [(4:40),(3:30),(2:20),(1:10)]\nGet 2: 20\nCache: [(2:20),(4:40),(3:30),(1:10)]\nAfter adding 5 (evicts 1): [(5:50),(2:20),(4:40),(3:30)]\nGet 1: -1\nGet 3: 30\nCache: [(3:30),(5:50),(2:20),(4:40)]\nHits: 2, Misses: 1
// LRU cache simulation with arrays

struct LRUEntry {
    int key;
    int value;
    int prev;
    int next;
    int active;
};

struct LRUCache {
    struct LRUEntry entries[8];
    int capacity;
    int size;
    int head;
    int tail;
    int hits;
    int misses;
};

void lru_init(struct LRUCache *c, int cap) {
    int i;
    c->capacity = cap;
    c->size = 0;
    c->head = -1;
    c->tail = -1;
    c->hits = 0;
    c->misses = 0;
    for (i = 0; i < 8; i++) {
        c->entries[i].key = -1;
        c->entries[i].value = 0;
        c->entries[i].prev = -1;
        c->entries[i].next = -1;
        c->entries[i].active = 0;
    }
}

void lru_remove_node(struct LRUCache *c, int idx) {
    int p = c->entries[idx].prev;
    int n = c->entries[idx].next;
    if (p != -1) c->entries[p].next = n;
    else c->head = n;
    if (n != -1) c->entries[n].prev = p;
    else c->tail = p;
}

void lru_add_to_front(struct LRUCache *c, int idx) {
    c->entries[idx].prev = -1;
    c->entries[idx].next = c->head;
    if (c->head != -1) c->entries[c->head].prev = idx;
    c->head = idx;
    if (c->tail == -1) c->tail = idx;
}

int lru_find_slot(struct LRUCache *c) {
    int i;
    for (i = 0; i < 8; i++) {
        if (!c->entries[i].active) return i;
    }
    return -1;
}

int lru_get(struct LRUCache *c, int key) {
    int i;
    for (i = 0; i < 8; i++) {
        if (c->entries[i].active && c->entries[i].key == key) {
            c->hits++;
            lru_remove_node(c, i);
            lru_add_to_front(c, i);
            return c->entries[i].value;
        }
    }
    c->misses++;
    return -1;
}

void lru_put(struct LRUCache *c, int key, int value) {
    int i;
    int slot;
    int evict;
    for (i = 0; i < 8; i++) {
        if (c->entries[i].active && c->entries[i].key == key) {
            c->entries[i].value = value;
            lru_remove_node(c, i);
            lru_add_to_front(c, i);
            return;
        }
    }
    if (c->size >= c->capacity) {
        evict = c->tail;
        lru_remove_node(c, evict);
        c->entries[evict].active = 0;
        c->size--;
    }
    slot = lru_find_slot(c);
    c->entries[slot].key = key;
    c->entries[slot].value = value;
    c->entries[slot].active = 1;
    lru_add_to_front(c, slot);
    c->size++;
}

void lru_print(struct LRUCache *c) {
    int cur = c->head;
    printf("[");
    while (cur != -1) {
        printf("(%d:%d)", c->entries[cur].key, c->entries[cur].value);
        cur = c->entries[cur].next;
        if (cur != -1) printf(",");
    }
    printf("]\n");
}

int main(void) {
    struct LRUCache c;
    lru_init(&c, 4);

    lru_put(&c, 1, 10);
    lru_put(&c, 2, 20);
    lru_put(&c, 3, 30);
    lru_put(&c, 4, 40);
    printf("Cache: ");
    lru_print(&c);

    printf("Get 2: %d\n", lru_get(&c, 2));
    printf("Cache: ");
    lru_print(&c);

    lru_put(&c, 5, 50);
    printf("After adding 5 (evicts 1): ");
    lru_print(&c);

    printf("Get 1: %d\n", lru_get(&c, 1));
    printf("Get 3: %d\n", lru_get(&c, 3));
    printf("Cache: ");
    lru_print(&c);

    printf("Hits: %d, Misses: %d\n", c.hits, c.misses);

    return 0;
}
