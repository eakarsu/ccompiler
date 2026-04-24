int printf(const char *fmt, ...);

// CPU cache simulation: direct-mapped cache, hit/miss, LRU eviction

struct CacheLine {
    int valid;
    int tag;
    int data;
    int last_used;
};

struct Cache {
    struct CacheLine lines[8];
    int num_lines;
    int hits;
    int misses;
    int evictions;
    int access_count;
};

void cache_init(struct Cache *c, int num_lines) {
    c->num_lines = num_lines;
    c->hits = 0;
    c->misses = 0;
    c->evictions = 0;
    c->access_count = 0;
    int i;
    for (i = 0; i < 8; i++) {
        c->lines[i].valid = 0;
        c->lines[i].tag = -1;
        c->lines[i].data = 0;
        c->lines[i].last_used = 0;
    }
}

// Direct-mapped cache access
int dm_access(struct Cache *c, int address, int data) {
    c->access_count = c->access_count + 1;
    int index = address % c->num_lines;
    int tag = address / c->num_lines;

    if (c->lines[index].valid && c->lines[index].tag == tag) {
        c->hits = c->hits + 1;
        c->lines[index].last_used = c->access_count;
        return c->lines[index].data; // hit
    }

    c->misses = c->misses + 1;
    if (c->lines[index].valid) {
        c->evictions = c->evictions + 1;
    }
    c->lines[index].valid = 1;
    c->lines[index].tag = tag;
    c->lines[index].data = data;
    c->lines[index].last_used = c->access_count;
    return -1; // miss
}

// Fully-associative cache with LRU
struct FACache {
    struct CacheLine lines[4];
    int num_lines;
    int hits;
    int misses;
    int evictions;
    int access_count;
};

void fa_init(struct FACache *c, int num_lines) {
    c->num_lines = num_lines;
    c->hits = 0;
    c->misses = 0;
    c->evictions = 0;
    c->access_count = 0;
    int i;
    for (i = 0; i < 4; i++) {
        c->lines[i].valid = 0;
        c->lines[i].tag = -1;
        c->lines[i].data = 0;
        c->lines[i].last_used = 0;
    }
}

int fa_find_lru(struct FACache *c) {
    int lru_idx = 0;
    int lru_time = c->lines[0].last_used;
    int i;
    for (i = 1; i < c->num_lines; i++) {
        if (c->lines[i].valid == 0) return i; // empty slot
        if (c->lines[i].last_used < lru_time) {
            lru_idx = i;
            lru_time = c->lines[i].last_used;
        }
    }
    // Check if slot 0 is invalid
    if (c->lines[0].valid == 0) return 0;
    return lru_idx;
}

int fa_access(struct FACache *c, int address, int data) {
    c->access_count = c->access_count + 1;
    int i;

    // Check for hit
    for (i = 0; i < c->num_lines; i++) {
        if (c->lines[i].valid && c->lines[i].tag == address) {
            c->hits = c->hits + 1;
            c->lines[i].last_used = c->access_count;
            return c->lines[i].data;
        }
    }

    // Miss
    c->misses = c->misses + 1;
    int victim = fa_find_lru(c);
    if (c->lines[victim].valid) {
        c->evictions = c->evictions + 1;
    }
    c->lines[victim].valid = 1;
    c->lines[victim].tag = address;
    c->lines[victim].data = data;
    c->lines[victim].last_used = c->access_count;
    return -1;
}

int main(void) {
    struct Cache dm;
    struct FACache fa;
    int result;

    // --- Direct-mapped cache tests (4 lines) ---
    cache_init(&dm, 4);
    // EXPECT: DM init: lines=4 hits=0 misses=0
    printf("DM init: lines=%d hits=%d misses=%d\n", dm.num_lines, dm.hits, dm.misses);

    // Access address 0 (miss)
    result = dm_access(&dm, 0, 100);
    // EXPECT: Addr 0: result=-1 miss
    printf("Addr 0: result=%d miss\n", result);

    // Access address 0 again (hit)
    result = dm_access(&dm, 0, 100);
    // EXPECT: Addr 0 again: result=100 hit
    printf("Addr 0 again: result=%d hit\n", result);

    // Access addresses 1, 2, 3 (all miss, fill cache)
    dm_access(&dm, 1, 101);
    dm_access(&dm, 2, 102);
    dm_access(&dm, 3, 103);
    // EXPECT: After fill: hits=1 misses=4
    printf("After fill: hits=%d misses=%d\n", dm.hits, dm.misses);

    // Access address 4 (maps to index 0, evicts address 0)
    result = dm_access(&dm, 4, 200);
    // EXPECT: Addr 4: evictions=1
    printf("Addr 4: evictions=%d\n", dm.evictions);

    // Access address 0 again (miss, evicted)
    result = dm_access(&dm, 0, 100);
    // EXPECT: Addr 0 evicted: result=-1 evictions=2
    printf("Addr 0 evicted: result=%d evictions=%d\n", result, dm.evictions);

    // Conflict pattern: alternating 0 and 4 (same index)
    dm_access(&dm, 4, 200);
    dm_access(&dm, 0, 100);
    dm_access(&dm, 4, 200);
    // EXPECT: Conflict: misses=9 evictions=5
    printf("Conflict: misses=%d evictions=%d\n", dm.misses, dm.evictions);

    // Hit rate calculation (x100 for integer math)
    {
        int total = dm.hits + dm.misses;
        int rate = (dm.hits * 100) / total;
        // EXPECT: DM hit rate: 10
        printf("DM hit rate: %d\n", rate);
    }

    // --- Fully-associative cache with LRU (4 lines) ---
    fa_init(&fa, 4);
    // EXPECT: FA init: lines=4
    printf("FA init: lines=%d\n", fa.num_lines);

    // Fill all 4 slots
    fa_access(&fa, 10, 110);
    fa_access(&fa, 20, 120);
    fa_access(&fa, 30, 130);
    fa_access(&fa, 40, 140);
    // EXPECT: FA fill: misses=4 evictions=0
    printf("FA fill: misses=%d evictions=%d\n", fa.misses, fa.evictions);

    // Hit on address 10
    result = fa_access(&fa, 10, 110);
    // EXPECT: FA hit 10: result=110
    printf("FA hit 10: result=%d\n", result);

    // Access new address 50 - should evict LRU (address 20)
    fa_access(&fa, 50, 150);
    // EXPECT: FA after 50: evictions=1
    printf("FA after 50: evictions=%d\n", fa.evictions);

    // Address 20 should be evicted (miss)
    result = fa_access(&fa, 20, 120);
    // EXPECT: FA 20 evicted: result=-1
    printf("FA 20 evicted: result=%d\n", result);

    // LRU order test: access 30, 40, 10, 50 then add new -> evicts 20 (just loaded, but LRU)
    // Actually 20 was just loaded so it is fresh. Let's access all but 30, then add new.
    fa_access(&fa, 40, 140);
    fa_access(&fa, 10, 110);
    fa_access(&fa, 50, 150);
    // Now LRU is 20 (accessed earliest at step 7), then 30 at step...
    // Actually 20 was loaded at step 7, 30 originally at step 3 but not hit since.
    // 30's last_used=3, 20's last_used=7. So 30 is LRU.
    fa_access(&fa, 60, 160);
    // EXPECT: FA evictions after 60: 3
    printf("FA evictions after 60: %d\n", fa.evictions);

    // Verify 30 was evicted
    result = fa_access(&fa, 30, 130);
    // EXPECT: FA 30 evicted: result=-1
    printf("FA 30 evicted: result=%d\n", result);

    // EXPECT: FA total: hits=4 misses=8
    printf("FA total: hits=%d misses=%d\n", fa.hits, fa.misses);

    // EXPECT: Cache sim complete
    printf("Cache sim complete\n");

    return 0;
}
