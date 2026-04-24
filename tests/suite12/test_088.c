int printf(const char *fmt, ...);

// Cache Simulator: Direct-Mapped and Set-Associative
// Simulates L1 cache behavior with hit/miss tracking

// Direct-mapped cache: 8 lines, block size 4 words
int dm_valid[8];
int dm_tag[8];
int dm_data[8];  // simplified: just one data word per line
int dm_hits;
int dm_misses;

// 2-way set-associative cache: 4 sets, 2 ways each
int sa_valid[8];     // [set*2 + way]
int sa_tag[8];
int sa_data[8];
int sa_lru[4];       // LRU bit per set: 0 = way 0 is LRU, 1 = way 1 is LRU
int sa_hits;
int sa_misses;

void dm_init() {
    int i;
    for (i = 0; i < 8; i++) {
        dm_valid[i] = 0;
        dm_tag[i] = -1;
        dm_data[i] = 0;
    }
    dm_hits = 0;
    dm_misses = 0;
}

void sa_init() {
    int i;
    for (i = 0; i < 8; i++) {
        sa_valid[i] = 0;
        sa_tag[i] = -1;
        sa_data[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        sa_lru[i] = 0;
    }
    sa_hits = 0;
    sa_misses = 0;
}

// Direct-mapped: address -> index = (addr / 4) % 8, tag = addr / 32
int dm_access(int addr) {
    int block;
    int index;
    int tag;

    block = addr / 4;
    index = block % 8;
    tag = block / 8;

    if (dm_valid[index] == 1 && dm_tag[index] == tag) {
        dm_hits = dm_hits + 1;
        return 1; // hit
    }

    dm_misses = dm_misses + 1;
    dm_valid[index] = 1;
    dm_tag[index] = tag;
    dm_data[index] = addr; // store addr as data for simplicity
    return 0; // miss
}

// 2-way set-associative: set = (addr / 4) % 4
int sa_access(int addr) {
    int block;
    int set;
    int tag;
    int way0_idx;
    int way1_idx;

    block = addr / 4;
    set = block % 4;
    tag = block / 4;

    way0_idx = set * 2;
    way1_idx = set * 2 + 1;

    // Check way 0
    if (sa_valid[way0_idx] == 1 && sa_tag[way0_idx] == tag) {
        sa_hits = sa_hits + 1;
        sa_lru[set] = 1; // way 1 is now LRU
        return 1;
    }

    // Check way 1
    if (sa_valid[way1_idx] == 1 && sa_tag[way1_idx] == tag) {
        sa_hits = sa_hits + 1;
        sa_lru[set] = 0; // way 0 is now LRU
        return 1;
    }

    // Miss - evict LRU way
    sa_misses = sa_misses + 1;
    int victim;
    if (sa_lru[set] == 0) {
        victim = way0_idx;
        sa_lru[set] = 1;
    } else {
        victim = way1_idx;
        sa_lru[set] = 0;
    }
    sa_valid[victim] = 1;
    sa_tag[victim] = tag;
    sa_data[victim] = addr;
    return 0;
}

int dm_hit_rate_pct() {
    int total;
    total = dm_hits + dm_misses;
    if (total == 0) return 0;
    return (dm_hits * 100) / total;
}

int sa_hit_rate_pct() {
    int total;
    total = sa_hits + sa_misses;
    if (total == 0) return 0;
    return (sa_hits * 100) / total;
}

int main() {
    int i;
    int result;

    dm_init();
    sa_init();
    printf("Cache Simulator\n");
    // EXPECT: Cache Simulator

    // Sequential access pattern: addresses 0, 4, 8, ..., 28 (8 blocks)
    printf("--- Sequential Access ---\n");
    // EXPECT: --- Sequential Access ---
    for (i = 0; i < 8; i++) {
        result = dm_access(i * 4);
        sa_access(i * 4);
    }
    printf("DM: hits=%d misses=%d\n", dm_hits, dm_misses);
    // EXPECT: DM: hits=0 misses=8
    printf("SA: hits=%d misses=%d\n", sa_hits, sa_misses);
    // EXPECT: SA: hits=0 misses=8

    // Access same addresses again - should all hit
    for (i = 0; i < 8; i++) {
        dm_access(i * 4);
        sa_access(i * 4);
    }
    printf("After repeat - DM: hits=%d misses=%d\n", dm_hits, dm_misses);
    // EXPECT: After repeat - DM: hits=8 misses=8
    printf("After repeat - SA: hits=%d misses=%d\n", sa_hits, sa_misses);
    // EXPECT: After repeat - SA: hits=8 misses=8

    // Conflict pattern for direct-mapped: addr 0 and addr 32 map to same index
    dm_init();
    sa_init();
    printf("--- Conflict Pattern ---\n");
    // EXPECT: --- Conflict Pattern ---

    // Alternating access: 0, 32, 0, 32, 0, 32 -> all misses for DM (thrashing)
    for (i = 0; i < 6; i++) {
        if (i % 2 == 0) {
            dm_access(0);
            sa_access(0);
        } else {
            dm_access(32);
            sa_access(32);
        }
    }
    printf("DM conflict: hits=%d misses=%d\n", dm_hits, dm_misses);
    // EXPECT: DM conflict: hits=0 misses=6
    printf("SA conflict: hits=%d misses=%d\n", sa_hits, sa_misses);
    // EXPECT: SA conflict: hits=4 misses=2

    // Stride pattern
    dm_init();
    sa_init();
    printf("--- Stride Pattern ---\n");
    // EXPECT: --- Stride Pattern ---

    // Access with stride 16: 0, 16, 32, 48, 64, 80, 96, 112
    for (i = 0; i < 8; i++) {
        dm_access(i * 16);
        sa_access(i * 16);
    }
    // Repeat
    for (i = 0; i < 8; i++) {
        dm_access(i * 16);
        sa_access(i * 16);
    }
    printf("DM stride: hits=%d misses=%d rate=%d\n", dm_hits, dm_misses, dm_hit_rate_pct());
    // EXPECT: DM stride: hits=0 misses=16 rate=0
    printf("SA stride: hits=%d misses=%d rate=%d\n", sa_hits, sa_misses, sa_hit_rate_pct());
    // EXPECT: SA stride: hits=0 misses=16 rate=0

    // Working set test
    dm_init();
    sa_init();
    printf("--- Working Set ---\n");
    // EXPECT: --- Working Set ---

    // Repeated small working set: 0, 4, 8, 12 (4 blocks, fits in both caches)
    int round;
    for (round = 0; round < 5; round++) {
        for (i = 0; i < 4; i++) {
            dm_access(i * 4);
            sa_access(i * 4);
        }
    }
    printf("DM working set: hits=%d misses=%d rate=%d\n", dm_hits, dm_misses, dm_hit_rate_pct());
    // EXPECT: DM working set: hits=16 misses=4 rate=80
    printf("SA working set: hits=%d misses=%d rate=%d\n", sa_hits, sa_misses, sa_hit_rate_pct());
    // EXPECT: SA working set: hits=16 misses=4 rate=80

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
