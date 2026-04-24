int printf(const char *fmt, ...);

// LRU Cache simulation using arrays
// Doubly linked list (array-based) + hash map for O(1)-ish lookup

int cache_keys[32];
int cache_vals[32];
int cache_prev[32];   // doubly linked list prev
int cache_next[32];   // doubly linked list next
int cache_used[32];   // 0=free, 1=used
int cache_cap;
int cache_size;
int head;  // most recently used
int tail;  // least recently used

// Hash map: key -> index in cache arrays
int hm_keys[64];
int hm_vals[64];
int hm_used[64];

void hm_init(void) {
    int i;
    for (i = 0; i < 64; i++) {
        hm_keys[i] = -1;
        hm_vals[i] = -1;
        hm_used[i] = 0;
    }
}

void hm_put(int key, int val) {
    int idx;
    int start;
    idx = key % 64;
    if (idx < 0) idx = -idx;
    start = idx;
    while (1) {
        if (hm_used[idx] == 0 || hm_keys[idx] == key) {
            hm_keys[idx] = key;
            hm_vals[idx] = val;
            hm_used[idx] = 1;
            return;
        }
        idx = (idx + 1) % 64;
        if (idx == start) return;
    }
}

int hm_get(int key) {
    int idx;
    int start;
    idx = key % 64;
    if (idx < 0) idx = -idx;
    start = idx;
    while (1) {
        if (hm_used[idx] == 0) return -1;
        if (hm_keys[idx] == key) return hm_vals[idx];
        idx = (idx + 1) % 64;
        if (idx == start) return -1;
    }
    return -1;
}

void hm_remove(int key) {
    int idx;
    int start;
    idx = key % 64;
    if (idx < 0) idx = -idx;
    start = idx;
    while (1) {
        if (hm_used[idx] == 0) return;
        if (hm_keys[idx] == key) {
            hm_used[idx] = 0;
            hm_keys[idx] = -1;
            hm_vals[idx] = -1;
            return;
        }
        idx = (idx + 1) % 64;
        if (idx == start) return;
    }
}

void lru_init(int cap) {
    int i;
    cache_cap = cap;
    cache_size = 0;
    head = -1;
    tail = -1;
    for (i = 0; i < 32; i++) {
        cache_keys[i] = 0;
        cache_vals[i] = 0;
        cache_prev[i] = -1;
        cache_next[i] = -1;
        cache_used[i] = 0;
    }
    hm_init();
}

int find_free_slot(void) {
    int i;
    for (i = 0; i < 32; i++) {
        if (cache_used[i] == 0) return i;
    }
    return -1;
}

void move_to_front(int idx) {
    int p;
    int n;
    if (idx == head) return;
    p = cache_prev[idx];
    n = cache_next[idx];
    // Unlink
    if (p != -1) cache_next[p] = n;
    if (n != -1) cache_prev[n] = p;
    if (idx == tail) tail = p;
    // Put at head
    cache_prev[idx] = -1;
    cache_next[idx] = head;
    if (head != -1) cache_prev[head] = idx;
    head = idx;
    if (tail == -1) tail = idx;
}

void lru_put(int key, int val) {
    int idx;
    int slot;
    int evict_key;
    // Check if key exists
    idx = hm_get(key);
    if (idx != -1) {
        cache_vals[idx] = val;
        move_to_front(idx);
        return;
    }
    // Need new slot
    if (cache_size < cache_cap) {
        slot = find_free_slot();
        cache_keys[slot] = key;
        cache_vals[slot] = val;
        cache_used[slot] = 1;
        cache_prev[slot] = -1;
        cache_next[slot] = head;
        if (head != -1) cache_prev[head] = slot;
        head = slot;
        if (tail == -1) tail = slot;
        hm_put(key, slot);
        cache_size = cache_size + 1;
    } else {
        // Evict LRU (tail)
        evict_key = cache_keys[tail];
        slot = tail;
        // Remove from list
        if (cache_prev[tail] != -1) {
            cache_next[cache_prev[tail]] = -1;
        }
        tail = cache_prev[tail];
        hm_remove(evict_key);
        // Reuse slot
        cache_keys[slot] = key;
        cache_vals[slot] = val;
        cache_prev[slot] = -1;
        cache_next[slot] = head;
        if (head != -1) cache_prev[head] = slot;
        head = slot;
        hm_put(key, slot);
    }
}

int lru_get(int key) {
    int idx;
    idx = hm_get(key);
    if (idx == -1) return -1;
    move_to_front(idx);
    return cache_vals[idx];
}

int main(void) {
    lru_init(3);  // capacity 3
    // EXPECT: size=0
    printf("size=%d\n", cache_size);

    lru_put(1, 10);
    lru_put(2, 20);
    lru_put(3, 30);
    // EXPECT: size=3
    printf("size=%d\n", cache_size);

    // EXPECT: get 1=10
    printf("get 1=%d\n", lru_get(1));
    // EXPECT: get 2=20
    printf("get 2=%d\n", lru_get(2));
    // EXPECT: get 3=30
    printf("get 3=%d\n", lru_get(3));

    // Access key 1 to make it MRU, then add key 4 to evict LRU (key 2)
    lru_get(1);
    lru_put(4, 40);
    // EXPECT: get 2 after evict=-1
    printf("get 2 after evict=%d\n", lru_get(2));
    // EXPECT: get 4=40
    printf("get 4=%d\n", lru_get(4));
    // EXPECT: get 1 still=10
    printf("get 1 still=%d\n", lru_get(1));

    // Add 5, should evict 3 (LRU now)
    lru_put(5, 50);
    // EXPECT: get 3 evicted=-1
    printf("get 3 evicted=%d\n", lru_get(3));
    // EXPECT: get 5=50
    printf("get 5=%d\n", lru_get(5));

    // Update existing key
    lru_put(4, 400);
    // EXPECT: get 4 updated=400
    printf("get 4 updated=%d\n", lru_get(4));

    // EXPECT: final size=3
    printf("final size=%d\n", cache_size);

    // EXPECT: done
    printf("done\n");

    return 0;
}
