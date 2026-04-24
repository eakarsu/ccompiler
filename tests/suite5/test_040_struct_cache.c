int printf(const char *fmt, ...);

struct CacheEntry {
    int key;
    int value;
    int valid;
    int access_time;
};

struct LRUCache {
    struct CacheEntry entries[8];
    int capacity;
    int size;
    int hits;
    int misses;
    int evictions;
    int timer;
};

void cache_init(struct LRUCache *c, int cap) {
    if (cap > 8) cap = 8;
    c->capacity = cap;
    c->size = 0;
    c->hits = 0;
    c->misses = 0;
    c->evictions = 0;
    c->timer = 0;
    int i = 0;
    while (i < 8) {
        c->entries[i].key = 0;
        c->entries[i].value = 0;
        c->entries[i].valid = 0;
        c->entries[i].access_time = 0;
        i = i + 1;
    }
}

int cache_find(struct LRUCache *c, int key) {
    int i = 0;
    while (i < c->capacity) {
        if (c->entries[i].valid && c->entries[i].key == key) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

int cache_get(struct LRUCache *c, int key) {
    c->timer = c->timer + 1;
    int idx = cache_find(c, key);
    if (idx >= 0) {
        c->hits = c->hits + 1;
        c->entries[idx].access_time = c->timer;
        return c->entries[idx].value;
    }
    c->misses = c->misses + 1;
    return -1;
}

int cache_find_lru(struct LRUCache *c) {
    int min_time = 999999;
    int min_idx = 0;
    int i = 0;
    while (i < c->capacity) {
        if (c->entries[i].valid) {
            if (c->entries[i].access_time < min_time) {
                min_time = c->entries[i].access_time;
                min_idx = i;
            }
        }
        i = i + 1;
    }
    return min_idx;
}

int cache_find_empty(struct LRUCache *c) {
    int i = 0;
    while (i < c->capacity) {
        if (c->entries[i].valid == 0) return i;
        i = i + 1;
    }
    return -1;
}

void cache_put(struct LRUCache *c, int key, int value) {
    c->timer = c->timer + 1;
    int idx = cache_find(c, key);
    if (idx >= 0) {
        c->entries[idx].value = value;
        c->entries[idx].access_time = c->timer;
        return;
    }
    int slot = cache_find_empty(c);
    if (slot >= 0) {
        c->entries[slot].key = key;
        c->entries[slot].value = value;
        c->entries[slot].valid = 1;
        c->entries[slot].access_time = c->timer;
        c->size = c->size + 1;
        return;
    }
    slot = cache_find_lru(c);
    c->entries[slot].key = key;
    c->entries[slot].value = value;
    c->entries[slot].access_time = c->timer;
    c->evictions = c->evictions + 1;
}

int cache_total_value(struct LRUCache *c) {
    int total = 0;
    int i = 0;
    while (i < c->capacity) {
        if (c->entries[i].valid) {
            total = total + c->entries[i].value;
        }
        i = i + 1;
    }
    return total;
}

int cache_hit_rate_pct(struct LRUCache *c) {
    int total = c->hits + c->misses;
    if (total == 0) return 0;
    return (c->hits * 100) / total;
}

int main(void) {
    struct LRUCache cache;
    cache_init(&cache, 4);

    // EXPECT: cap=4
    printf("cap=%d\n", cache.capacity);
    // EXPECT: init_size=0
    printf("init_size=%d\n", cache.size);

    cache_put(&cache, 1, 100);
    cache_put(&cache, 2, 200);
    cache_put(&cache, 3, 300);

    // EXPECT: size=3
    printf("size=%d\n", cache.size);

    // EXPECT: get1=100
    printf("get1=%d\n", cache_get(&cache, 1));
    // EXPECT: get2=200
    printf("get2=%d\n", cache_get(&cache, 2));
    // EXPECT: miss=-1
    printf("miss=%d\n", cache_get(&cache, 99));

    // EXPECT: hits=2
    printf("hits=%d\n", cache.hits);
    // EXPECT: misses=1
    printf("misses=%d\n", cache.misses);

    cache_put(&cache, 4, 400);
    // EXPECT: size2=4
    printf("size2=%d\n", cache.size);
    // EXPECT: evictions0=0
    printf("evictions0=%d\n", cache.evictions);

    cache_put(&cache, 5, 500);
    // EXPECT: evictions1=1
    printf("evictions1=%d\n", cache.evictions);
    // EXPECT: size_still4=4
    printf("size_still4=%d\n", cache.size);

    // EXPECT: get3_evicted=-1
    printf("get3_evicted=%d\n", cache_get(&cache, 3));
    // EXPECT: get5=500
    printf("get5=%d\n", cache_get(&cache, 5));

    // EXPECT: total_val=1200
    printf("total_val=%d\n", cache_total_value(&cache));

    cache_put(&cache, 1, 150);
    // EXPECT: updated1=150
    printf("updated1=%d\n", cache_get(&cache, 1));

    cache_put(&cache, 6, 600);
    // EXPECT: evictions2=2
    printf("evictions2=%d\n", cache.evictions);

    int hit_rate = cache_hit_rate_pct(&cache);
    // EXPECT: hit_pct=66
    printf("hit_pct=%d\n", hit_rate);

    struct LRUCache c2;
    cache_init(&c2, 2);
    cache_put(&c2, 10, 1000);
    cache_put(&c2, 20, 2000);
    cache_put(&c2, 30, 3000);
    // EXPECT: c2_evict=1
    printf("c2_evict=%d\n", c2.evictions);
    // EXPECT: c2_total=5000
    printf("c2_total=%d\n", cache_total_value(&c2));

    return 0;
}
