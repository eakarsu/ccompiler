int printf(const char *fmt, ...);

// LRU cache simulation using doubly linked list + array-based hash map

struct CacheNode {
    int key;
    int value;
    int prev;
    int next;
    int used; // 1 if this slot is active
};

struct LRUCache {
    struct CacheNode nodes[16];
    int head;       // most recently used
    int tail;       // least recently used
    int size;
    int capacity;
    // Simple hash map: maps key -> node index, -1 = empty
    int map[32];
};

void lru_init(struct LRUCache *c, int cap) {
    int i;
    c->head = -1;
    c->tail = -1;
    c->size = 0;
    c->capacity = cap;
    for (i = 0; i < 16; i++) {
        c->nodes[i].used = 0;
        c->nodes[i].prev = -1;
        c->nodes[i].next = -1;
        c->nodes[i].key = -1;
        c->nodes[i].value = -1;
    }
    for (i = 0; i < 32; i++) {
        c->map[i] = -1;
    }
}

int hash_key(int key) {
    int h = key % 32;
    if (h < 0) h = h + 32;
    return h;
}

int lru_find(struct LRUCache *c, int key) {
    int h = hash_key(key);
    int i;
    // Linear probing
    for (i = 0; i < 32; i++) {
        int idx = (h + i) % 32;
        if (c->map[idx] == -1) return -1;
        if (c->map[idx] >= 0 && c->nodes[c->map[idx]].key == key && c->nodes[c->map[idx]].used) {
            return c->map[idx];
        }
    }
    return -1;
}

void map_insert(struct LRUCache *c, int key, int node_idx) {
    int h = hash_key(key);
    int i;
    for (i = 0; i < 32; i++) {
        int idx = (h + i) % 32;
        if (c->map[idx] == -1 || (c->map[idx] >= 0 && c->nodes[c->map[idx]].used == 0)) {
            c->map[idx] = node_idx;
            return;
        }
    }
}

void map_remove(struct LRUCache *c, int key) {
    int h = hash_key(key);
    int i;
    for (i = 0; i < 32; i++) {
        int idx = (h + i) % 32;
        if (c->map[idx] == -1) return;
        if (c->map[idx] >= 0 && c->nodes[c->map[idx]].key == key) {
            c->map[idx] = -1;
            return;
        }
    }
}

int alloc_node(struct LRUCache *c) {
    int i;
    for (i = 0; i < 16; i++) {
        if (c->nodes[i].used == 0) return i;
    }
    return -1;
}

// Remove node from doubly linked list
void detach_node(struct LRUCache *c, int idx) {
    int p = c->nodes[idx].prev;
    int n = c->nodes[idx].next;
    if (p != -1) c->nodes[p].next = n;
    else c->head = n;
    if (n != -1) c->nodes[n].prev = p;
    else c->tail = p;
    c->nodes[idx].prev = -1;
    c->nodes[idx].next = -1;
}

// Insert node at head (most recently used)
void push_front(struct LRUCache *c, int idx) {
    c->nodes[idx].prev = -1;
    c->nodes[idx].next = c->head;
    if (c->head != -1) c->nodes[c->head].prev = idx;
    c->head = idx;
    if (c->tail == -1) c->tail = idx;
}

// Get value for key; returns -1 if not found
int lru_get(struct LRUCache *c, int key) {
    int idx = lru_find(c, key);
    if (idx == -1) return -1;
    // Move to front
    detach_node(c, idx);
    push_front(c, idx);
    return c->nodes[idx].value;
}

// Put key-value pair; evicts LRU if at capacity
void lru_put(struct LRUCache *c, int key, int value) {
    int idx = lru_find(c, key);
    int evict_idx;
    int new_idx;

    if (idx != -1) {
        // Update existing
        c->nodes[idx].value = value;
        detach_node(c, idx);
        push_front(c, idx);
        return;
    }

    // Need to insert new
    if (c->size >= c->capacity) {
        // Evict LRU (tail)
        evict_idx = c->tail;
        detach_node(c, evict_idx);
        map_remove(c, c->nodes[evict_idx].key);
        c->nodes[evict_idx].used = 0;
        c->size--;
    }

    new_idx = alloc_node(c);
    if (new_idx == -1) return;
    c->nodes[new_idx].key = key;
    c->nodes[new_idx].value = value;
    c->nodes[new_idx].used = 1;
    push_front(c, new_idx);
    map_insert(c, key, new_idx);
    c->size++;
}

void lru_print_order(struct LRUCache *c) {
    int cur = c->head;
    int first = 1;
    while (cur != -1) {
        if (!first) printf(" ");
        printf("%d:%d", c->nodes[cur].key, c->nodes[cur].value);
        first = 0;
        cur = c->nodes[cur].next;
    }
    printf("\n");
}

int main() {
    struct LRUCache cache;
    int val;

    lru_init(&cache, 3);

    // Insert 3 items
    lru_put(&cache, 1, 100);
    lru_put(&cache, 2, 200);
    lru_put(&cache, 3, 300);

    printf("Size: %d\n", cache.size);
    // EXPECT: Size: 3

    printf("Order: ");
    lru_print_order(&cache);
    // EXPECT: Order: 3:300 2:200 1:100

    // Access key 1 -> moves to front
    val = lru_get(&cache, 1);
    printf("Get 1: %d\n", val);
    // EXPECT: Get 1: 100

    printf("After get 1: ");
    lru_print_order(&cache);
    // EXPECT: After get 1: 1:100 3:300 2:200

    // Insert key 4 -> evicts key 2 (LRU)
    lru_put(&cache, 4, 400);
    printf("After put 4: ");
    lru_print_order(&cache);
    // EXPECT: After put 4: 4:400 1:100 3:300

    val = lru_get(&cache, 2);
    printf("Get 2 (evicted): %d\n", val);
    // EXPECT: Get 2 (evicted): -1

    // Access key 3
    val = lru_get(&cache, 3);
    printf("Get 3: %d\n", val);
    // EXPECT: Get 3: 300

    printf("After get 3: ");
    lru_print_order(&cache);
    // EXPECT: After get 3: 3:300 4:400 1:100

    // Insert key 5 -> evicts key 1 (LRU)
    lru_put(&cache, 5, 500);
    printf("After put 5: ");
    lru_print_order(&cache);
    // EXPECT: After put 5: 5:500 3:300 4:400

    val = lru_get(&cache, 1);
    printf("Get 1 (evicted): %d\n", val);
    // EXPECT: Get 1 (evicted): -1

    // Update existing key 3
    lru_put(&cache, 3, 333);
    printf("After update 3: ");
    lru_print_order(&cache);
    // EXPECT: After update 3: 3:333 5:500 4:400

    val = lru_get(&cache, 3);
    printf("Get 3 updated: %d\n", val);
    // EXPECT: Get 3 updated: 333

    printf("Final size: %d\n", cache.size);
    // EXPECT: Final size: 3

    return 0;
}
