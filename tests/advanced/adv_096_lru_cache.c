// EXPECT: === LRU Cache Tests ===
// EXPECT: Cache capacity: 4
// EXPECT: === Insert and Get ===
// EXPECT: Put(1,10) Put(2,20) Put(3,30) Put(4,40)
// EXPECT: Get(1) = 10 (hit)
// EXPECT: Get(2) = 20 (hit)
// EXPECT: Get(3) = 30 (hit)
// EXPECT: Get(4) = 40 (hit)
// EXPECT: Get(5) = -1 (miss, evicted)
// EXPECT: Hits: 4, Misses: 1
// EXPECT: === Eviction Test ===
// EXPECT: Put(5,50) - should evict key 1
// EXPECT: Get(1) = -1 (miss, evicted)
// EXPECT: Get(5) = 50 (hit)
// EXPECT: Get(2) = 20 (hit)
// EXPECT: === LRU Order Test ===
// EXPECT: Access order: get(3), get(4), get(5), get(2)
// EXPECT: Put(6,60) - should evict key 3 (least recent)
// EXPECT: Get(3) = -1 (miss, evicted)
// EXPECT: Get(6) = 60 (hit)
// EXPECT: Get(4) = 40 (hit)
// EXPECT: === Update Test ===
// EXPECT: Put(4,400) - update existing
// EXPECT: Get(4) = 400 (hit, updated)
// EXPECT: === Stats ===
// EXPECT: Total hits: 13, total misses: 3
// EXPECT: Hit rate: 81%
// EXPECT: All LRU cache tests passed!

int printf(const char *fmt, ...);

enum { CACHE_CAP = 4 };

struct CacheEntry {
    int key;
    int value;
    int valid;
    int last_access;  // timestamp for LRU ordering
};

struct LRUCache {
    struct CacheEntry entries[4];
    int capacity;
    int size;
    int timer;
    int hits;
    int misses;
};

void cache_init(struct LRUCache *c) {
    int i;
    c->capacity = CACHE_CAP;
    c->size = 0;
    c->timer = 0;
    c->hits = 0;
    c->misses = 0;
    for (i = 0; i < CACHE_CAP; i++) {
        c->entries[i].key = 0;
        c->entries[i].value = 0;
        c->entries[i].valid = 0;
        c->entries[i].last_access = 0;
    }
}

// Find entry by key, return index or -1
int cache_find(struct LRUCache *c, int key) {
    int i;
    for (i = 0; i < CACHE_CAP; i++) {
        if (c->entries[i].valid && c->entries[i].key == key) {
            return i;
        }
    }
    return -1;
}

// Find the LRU (least recently used) entry index
int cache_find_lru(struct LRUCache *c) {
    int min_time = 2147483647;
    int min_idx = 0;
    int i;
    for (i = 0; i < CACHE_CAP; i++) {
        if (c->entries[i].valid && c->entries[i].last_access < min_time) {
            min_time = c->entries[i].last_access;
            min_idx = i;
        }
    }
    return min_idx;
}

// Find an empty slot or -1
int cache_find_empty(struct LRUCache *c) {
    int i;
    for (i = 0; i < CACHE_CAP; i++) {
        if (!c->entries[i].valid) return i;
    }
    return -1;
}

int cache_get(struct LRUCache *c, int key) {
    int idx = cache_find(c, key);
    if (idx < 0) {
        c->misses = c->misses + 1;
        return -1;
    }
    c->hits = c->hits + 1;
    c->timer = c->timer + 1;
    c->entries[idx].last_access = c->timer;
    return c->entries[idx].value;
}

void cache_put(struct LRUCache *c, int key, int value) {
    int idx;
    c->timer = c->timer + 1;

    // Check if key already exists (update)
    idx = cache_find(c, key);
    if (idx >= 0) {
        c->entries[idx].value = value;
        c->entries[idx].last_access = c->timer;
        return;
    }

    // Find empty slot
    idx = cache_find_empty(c);
    if (idx < 0) {
        // Evict LRU
        idx = cache_find_lru(c);
    } else {
        c->size = c->size + 1;
    }

    c->entries[idx].key = key;
    c->entries[idx].value = value;
    c->entries[idx].valid = 1;
    c->entries[idx].last_access = c->timer;
}

int main(void) {
    struct LRUCache cache;
    int val;

    printf("=== LRU Cache Tests ===\n");
    cache_init(&cache);
    printf("Cache capacity: %d\n", cache.capacity);

    // Basic insert and get
    printf("=== Insert and Get ===\n");
    cache_put(&cache, 1, 10);
    cache_put(&cache, 2, 20);
    cache_put(&cache, 3, 30);
    cache_put(&cache, 4, 40);
    printf("Put(1,10) Put(2,20) Put(3,30) Put(4,40)\n");

    val = cache_get(&cache, 1);
    printf("Get(1) = %d (%s)\n", val, val >= 0 ? "hit" : "miss");
    val = cache_get(&cache, 2);
    printf("Get(2) = %d (%s)\n", val, val >= 0 ? "hit" : "miss");
    val = cache_get(&cache, 3);
    printf("Get(3) = %d (%s)\n", val, val >= 0 ? "hit" : "miss");
    val = cache_get(&cache, 4);
    printf("Get(4) = %d (%s)\n", val, val >= 0 ? "hit" : "miss");
    val = cache_get(&cache, 5);
    printf("Get(5) = %d (%s)\n", val, val >= 0 ? "hit" : "miss, evicted");
    printf("Hits: %d, Misses: %d\n", cache.hits, cache.misses);

    // Eviction test: adding 5th element should evict LRU (key=1, oldest access)
    printf("=== Eviction Test ===\n");
    // key 1 was accessed most recently among 1,2,3,4. Let's reset to make 1 the LRU.
    // Actually after gets above, access order is 1,2,3,4,miss5. So 1 is LRU.
    // But we did get(1) first so its timestamp is oldest among the 4 successful gets.
    cache_put(&cache, 5, 50);
    printf("Put(5,50) - should evict key 1\n");
    val = cache_get(&cache, 1);
    printf("Get(1) = %d (%s)\n", val, val >= 0 ? "hit" : "miss, evicted");
    val = cache_get(&cache, 5);
    printf("Get(5) = %d (%s)\n", val, val >= 0 ? "hit" : "miss");
    val = cache_get(&cache, 2);
    printf("Get(2) = %d (%s)\n", val, val >= 0 ? "hit" : "miss");

    // LRU order test
    printf("=== LRU Order Test ===\n");
    // Current state: keys 2,3,4,5. Most recent access: 2 (just accessed), 5, then 3 and 4
    // Access 3,4,5,2 to set a known order
    cache_get(&cache, 3);
    cache_get(&cache, 4);
    cache_get(&cache, 5);
    cache_get(&cache, 2);
    printf("Access order: get(3), get(4), get(5), get(2)\n");
    // Now LRU is key 3 (accessed first in this sequence)
    cache_put(&cache, 6, 60);
    printf("Put(6,60) - should evict key 3 (least recent)\n");
    val = cache_get(&cache, 3);
    printf("Get(3) = %d (%s)\n", val, val >= 0 ? "hit" : "miss, evicted");
    val = cache_get(&cache, 6);
    printf("Get(6) = %d (%s)\n", val, val >= 0 ? "hit" : "miss");
    val = cache_get(&cache, 4);
    printf("Get(4) = %d (%s)\n", val, val >= 0 ? "hit" : "miss");

    // Update test
    printf("=== Update Test ===\n");
    cache_put(&cache, 4, 400);
    printf("Put(4,400) - update existing\n");
    val = cache_get(&cache, 4);
    printf("Get(4) = %d (%s)\n", val, val >= 0 ? "hit, updated" : "miss");

    // Stats
    printf("=== Stats ===\n");
    printf("Total hits: %d, total misses: %d\n", cache.hits, cache.misses);
    int total = cache.hits + cache.misses;
    int rate = (cache.hits * 100) / total;
    printf("Hit rate: %d%%\n", rate);

    printf("All LRU cache tests passed!\n");
    return 0;
}
