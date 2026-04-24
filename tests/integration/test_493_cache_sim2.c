int printf(const char *fmt, ...);
// EXPECT: === Cache Simulation (LRU) ===\nConfig: 4 sets, 2 ways\nAccess  0 (addr= 0 set=0 tag=0): MISS\nAccess  1 (addr= 4 set=1 tag=0): MISS\nAccess  2 (addr= 8 set=2 tag=0): MISS\nAccess  3 (addr=12 set=3 tag=0): MISS\nAccess  4 (addr=16 set=0 tag=1): MISS\nAccess  5 (addr= 0 set=0 tag=0): HIT\nAccess  6 (addr= 4 set=1 tag=0): HIT\nAccess  7 (addr=20 set=1 tag=1): MISS\nAccess  8 (addr=24 set=2 tag=1): MISS\nAccess  9 (addr= 0 set=0 tag=0): HIT\nAccess 10 (addr=28 set=3 tag=1): MISS\nAccess 11 (addr=32 set=0 tag=2): MISS\nAccess 12 (addr= 4 set=1 tag=0): HIT\nAccess 13 (addr= 8 set=2 tag=0): HIT\nAccess 14 (addr= 0 set=0 tag=0): HIT\nAccess 15 (addr=16 set=0 tag=1): MISS\nAccess 16 (addr=36 set=1 tag=2): MISS\nAccess 17 (addr= 0 set=0 tag=0): HIT\nAccess 18 (addr= 4 set=1 tag=0): HIT\nAccess 19 (addr=40 set=2 tag=2): MISS\n--- Statistics ---\nHits: 8\nMisses: 12\nEvictions: 4\nHit rate: 40%\nCache state:\n  Set 0: [tag=1 lu=16] [tag=0 lu=18]\n  Set 1: [tag=2 lu=17] [tag=0 lu=19]\n  Set 2: [tag=2 lu=20] [tag=0 lu=14]\n  Set 3: [tag=1 lu=11] [tag=0 lu=4]

struct CacheLine {
    int tag;
    int valid;
    int data;
    int last_used;
};

struct CacheSet {
    struct CacheLine lines[4];
    int ways;
};

struct Cache {
    struct CacheSet sets[4];
    int num_sets;
    int num_ways;
    int hits;
    int misses;
    int evictions;
    int access_count;
};

void init_cache(struct Cache *c, int sets, int ways) {
    c->num_sets = sets;
    c->num_ways = ways;
    c->hits = 0;
    c->misses = 0;
    c->evictions = 0;
    c->access_count = 0;

    int i, j;
    for (i = 0; i < 4; i++) {
        c->sets[i].ways = ways;
        for (j = 0; j < 4; j++) {
            c->sets[i].lines[j].tag = -1;
            c->sets[i].lines[j].valid = 0;
            c->sets[i].lines[j].data = 0;
            c->sets[i].lines[j].last_used = 0;
        }
    }
}

int find_lru(struct CacheSet *set) {
    int lru_idx = 0;
    int lru_time = set->lines[0].last_used;
    int i;
    for (i = 1; i < set->ways; i++) {
        if (!set->lines[i].valid) return i;
        if (set->lines[i].last_used < lru_time) {
            lru_time = set->lines[i].last_used;
            lru_idx = i;
        }
    }
    return lru_idx;
}

int access_cache(struct Cache *c, int address) {
    c->access_count++;
    int set_idx = (address / 4) % c->num_sets;
    int tag = address / (4 * c->num_sets);

    struct CacheSet *set = &c->sets[set_idx];
    int i;
    for (i = 0; i < set->ways; i++) {
        if (set->lines[i].valid && set->lines[i].tag == tag) {
            set->lines[i].last_used = c->access_count;
            c->hits++;
            return 1;
        }
    }

    c->misses++;
    int victim = find_lru(set);
    if (set->lines[victim].valid) {
        c->evictions++;
    }
    set->lines[victim].tag = tag;
    set->lines[victim].valid = 1;
    set->lines[victim].data = address;
    set->lines[victim].last_used = c->access_count;
    return 0;
}

int main(void) {
    struct Cache cache;
    init_cache(&cache, 4, 2);

    printf("=== Cache Simulation (LRU) ===\n");
    printf("Config: %d sets, %d ways\n", cache.num_sets, cache.num_ways);

    int addresses[20];
    addresses[0] = 0;   addresses[1] = 4;   addresses[2] = 8;   addresses[3] = 12;
    addresses[4] = 16;  addresses[5] = 0;   addresses[6] = 4;   addresses[7] = 20;
    addresses[8] = 24;  addresses[9] = 0;   addresses[10] = 28; addresses[11] = 32;
    addresses[12] = 4;  addresses[13] = 8;  addresses[14] = 0;  addresses[15] = 16;
    addresses[16] = 36; addresses[17] = 0;  addresses[18] = 4;  addresses[19] = 40;

    int i;
    for (i = 0; i < 20; i++) {
        int hit = access_cache(&cache, addresses[i]);
        printf("Access %2d (addr=%2d set=%d tag=%d): %s\n",
               i, addresses[i],
               (addresses[i] / 4) % cache.num_sets,
               addresses[i] / (4 * cache.num_sets),
               hit ? "HIT" : "MISS");
    }

    printf("--- Statistics ---\n");
    printf("Hits: %d\n", cache.hits);
    printf("Misses: %d\n", cache.misses);
    printf("Evictions: %d\n", cache.evictions);
    printf("Hit rate: %d%%\n", cache.hits * 100 / cache.access_count);

    printf("Cache state:\n");
    int s, w;
    for (s = 0; s < cache.num_sets; s++) {
        printf("  Set %d:", s);
        for (w = 0; w < cache.num_ways; w++) {
            if (cache.sets[s].lines[w].valid) {
                printf(" [tag=%d lu=%d]", cache.sets[s].lines[w].tag,
                       cache.sets[s].lines[w].last_used);
            } else {
                printf(" [empty]");
            }
        }
        printf("\n");
    }

    return 0;
}
