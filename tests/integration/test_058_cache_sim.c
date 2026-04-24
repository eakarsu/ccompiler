int printf(const char *fmt, ...);
// EXPECT: T1:a0=0 a1=0 a2=0 a0=1 a1=1 a8=0 a0=0 a1=1 h=3 m=5 T2:pass1 h=0 m=8 pass2 h=8 m=0 T3:h=0 m=4 tag=3 T4:h=0 m=4 get10=100 h=1 m=5 get20=-1 get30=300 T5:h=6 m=3 T6:h=0 m=12 T7:h=12 m=4 T8:h=4 m=4 T9:a0=0 a8=0 a0=1 a16=0 a8=0 h=1 m=4 T10:h=8 m=8 rate=50 T11:g1=10 g2=-1 g3=30 g4=40 T12:h=16 m=16 T13:g1=10 g1=-1 g2=20 h=2 m=3 T14:h=8 m=4 T15:a0=0 a4=0 h=0 m=5 T16:h=6 m=4 T17:h=0 m=16 h2=0 m2=24 DONE

/* =========================================================================
 * Test 058: LRU Cache Simulation Using Arrays
 * =========================================================================
 * Simulates LRU cache, direct-mapped cache, counts hits/misses,
 * tests various access patterns and cache sizes.
 * ========================================================================= */

/* ---- LRU Cache ---- */
int lru_keys[16];
int lru_vals[16];
int lru_time[16];
int lru_valid[16];
int lru_cap;
int lru_clock;
int lru_hits;
int lru_misses;

void lru_init(int cap) {
    int i;
    lru_cap = cap;
    lru_clock = 0;
    lru_hits = 0;
    lru_misses = 0;
    for (i = 0; i < 16; i = i + 1) {
        lru_keys[i] = -1;
        lru_vals[i] = -1;
        lru_time[i] = 0;
        lru_valid[i] = 0;
    }
}

int lru_find(int key) {
    int i;
    for (i = 0; i < lru_cap; i = i + 1) {
        if (lru_valid[i] && lru_keys[i] == key) {
            return i;
        }
    }
    return -1;
}

int lru_find_victim(void) {
    int i;
    int min_time;
    int min_idx;

    for (i = 0; i < lru_cap; i = i + 1) {
        if (lru_valid[i] == 0) {
            return i;
        }
    }

    min_time = lru_time[0];
    min_idx = 0;
    for (i = 1; i < lru_cap; i = i + 1) {
        if (lru_time[i] < min_time) {
            min_time = lru_time[i];
            min_idx = i;
        }
    }
    return min_idx;
}

int lru_access(int key, int val) {
    int idx;
    int victim;

    lru_clock = lru_clock + 1;
    idx = lru_find(key);

    if (idx >= 0) {
        lru_hits = lru_hits + 1;
        lru_time[idx] = lru_clock;
        lru_vals[idx] = val;
        return lru_vals[idx];
    }

    lru_misses = lru_misses + 1;
    victim = lru_find_victim();
    lru_keys[victim] = key;
    lru_vals[victim] = val;
    lru_time[victim] = lru_clock;
    lru_valid[victim] = 1;
    return -1;
}

int lru_get(int key) {
    int idx;
    lru_clock = lru_clock + 1;
    idx = lru_find(key);
    if (idx >= 0) {
        lru_hits = lru_hits + 1;
        lru_time[idx] = lru_clock;
        return lru_vals[idx];
    }
    lru_misses = lru_misses + 1;
    return -1;
}

/* ---- Direct-Mapped Cache ---- */
int dm_tags[16];
int dm_valid[16];
int dm_data[16];
int dm_size;
int dm_hits;
int dm_misses;

void dm_init(int size) {
    int i;
    dm_size = size;
    dm_hits = 0;
    dm_misses = 0;
    for (i = 0; i < 16; i = i + 1) {
        dm_tags[i] = -1;
        dm_valid[i] = 0;
        dm_data[i] = 0;
    }
}

int dm_access(int addr) {
    int line;
    int tag;
    line = addr % dm_size;
    tag = addr / dm_size;

    if (dm_valid[line] && dm_tags[line] == tag) {
        dm_hits = dm_hits + 1;
        return 1;
    }

    dm_misses = dm_misses + 1;
    dm_valid[line] = 1;
    dm_tags[line] = tag;
    dm_data[line] = addr;
    return 0;
}

/* ---- 2-Way Set Associative Cache ---- */
int sa_tags[16];
int sa_valid[16];
int sa_time[16];
int sa_sets;
int sa_clock;
int sa_hits;
int sa_misses;

void sa_init(int sets) {
    int i;
    sa_sets = sets;
    sa_clock = 0;
    sa_hits = 0;
    sa_misses = 0;
    for (i = 0; i < 16; i = i + 1) {
        sa_tags[i] = -1;
        sa_valid[i] = 0;
        sa_time[i] = 0;
    }
}

int sa_access(int addr) {
    int set_idx;
    int tag;
    int base;
    int i;
    int victim;

    set_idx = addr % sa_sets;
    tag = addr / sa_sets;
    base = set_idx * 2;

    sa_clock = sa_clock + 1;

    for (i = 0; i < 2; i = i + 1) {
        if (sa_valid[base + i] && sa_tags[base + i] == tag) {
            sa_hits = sa_hits + 1;
            sa_time[base + i] = sa_clock;
            return 1;
        }
    }

    sa_misses = sa_misses + 1;
    victim = base;
    if (sa_valid[base] == 0) {
        victim = base;
    } else if (sa_valid[base + 1] == 0) {
        victim = base + 1;
    } else {
        if (sa_time[base] <= sa_time[base + 1]) {
            victim = base;
        } else {
            victim = base + 1;
        }
    }
    sa_tags[victim] = tag;
    sa_valid[victim] = 1;
    sa_time[victim] = sa_clock;
    return 0;
}

/* =========================================================================
 * Test functions
 * ========================================================================= */

void test_dm_basic(void) {
    printf("T1:");
    dm_init(8);

    printf("a0=%d", dm_access(0));
    printf(" a1=%d", dm_access(1));
    printf(" a2=%d", dm_access(2));
    printf(" a0=%d", dm_access(0));
    printf(" a1=%d", dm_access(1));
    printf(" a8=%d", dm_access(8));
    printf(" a0=%d", dm_access(0));
    printf(" a1=%d", dm_access(1));
    printf(" h=%d m=%d", dm_hits, dm_misses);
}

void test_dm_sequential(void) {
    int i;
    int h1;
    int m1;

    printf(" T2:");
    dm_init(8);

    for (i = 0; i < 8; i = i + 1) {
        dm_access(i);
    }
    h1 = dm_hits;
    m1 = dm_misses;
    printf("pass1 h=%d m=%d", h1, m1);

    for (i = 0; i < 8; i = i + 1) {
        dm_access(i);
    }
    printf(" pass2 h=%d m=%d", dm_hits - h1, dm_misses - m1);
}

void test_dm_conflict(void) {
    printf(" T3:");
    dm_init(4);

    dm_access(0);
    dm_access(4);
    dm_access(8);
    dm_access(12);
    printf("h=%d m=%d", dm_hits, dm_misses);
    printf(" tag=%d", dm_tags[0]);
}

void test_lru_basic(void) {
    int v;

    printf(" T4:");
    lru_init(4);

    lru_access(10, 100);
    lru_access(20, 200);
    lru_access(30, 300);
    lru_access(40, 400);

    printf("h=%d m=%d", lru_hits, lru_misses);

    v = lru_get(10);
    printf(" get10=%d", v);
    printf(" h=%d", lru_hits);

    lru_access(50, 500);
    printf(" m=%d", lru_misses);
    v = lru_get(20);
    printf(" get20=%d", v);

    v = lru_get(30);
    printf(" get30=%d", v);
}

void test_lru_repeated(void) {
    int i;
    int j;

    printf(" T5:");
    lru_init(4);

    for (j = 0; j < 3; j = j + 1) {
        for (i = 1; i <= 3; i = i + 1) {
            lru_access(i, i * 10);
        }
    }
    printf("h=%d m=%d", lru_hits, lru_misses);
}

void test_lru_thrashing(void) {
    int i;
    int j;

    printf(" T6:");
    lru_init(3);

    for (j = 0; j < 3; j = j + 1) {
        for (i = 1; i <= 4; i = i + 1) {
            lru_access(i, i * 10);
        }
    }
    printf("h=%d m=%d", lru_hits, lru_misses);
}

void test_lru_working_set(void) {
    int i;
    int j;

    printf(" T7:");
    lru_init(4);

    for (j = 0; j < 4; j = j + 1) {
        for (i = 0; i < 4; i = i + 1) {
            lru_access(i, i * 10);
        }
    }
    printf("h=%d m=%d", lru_hits, lru_misses);
}

void test_dm_stride(void) {
    int i;

    printf(" T8:");
    dm_init(8);

    for (i = 0; i < 8; i = i + 2) {
        dm_access(i);
    }
    for (i = 0; i < 8; i = i + 2) {
        dm_access(i);
    }
    printf("h=%d m=%d", dm_hits, dm_misses);
}

void test_sa_basic(void) {
    printf(" T9:");
    sa_init(4);

    printf("a0=%d", sa_access(0));
    printf(" a8=%d", sa_access(8));
    printf(" a0=%d", sa_access(0));
    printf(" a16=%d", sa_access(16));
    printf(" a8=%d", sa_access(8));
    printf(" h=%d m=%d", sa_hits, sa_misses);
}

void test_cache_hit_rate(void) {
    int i;
    int total;
    int rate;

    printf(" T10:");
    lru_init(8);

    for (i = 0; i < 8; i = i + 1) {
        lru_access(i, i);
    }
    for (i = 0; i < 8; i = i + 1) {
        lru_access(i, i);
    }

    total = lru_hits + lru_misses;
    rate = (lru_hits * 100) / total;
    printf("h=%d m=%d rate=%d", lru_hits, lru_misses, rate);
}

void test_lru_eviction_order(void) {
    int v;

    printf(" T11:");
    lru_init(3);

    lru_access(1, 10);
    lru_access(2, 20);
    lru_access(3, 30);

    lru_get(1);

    lru_access(4, 40);

    v = lru_get(1); printf("g1=%d", v);
    v = lru_get(2); printf(" g2=%d", v);
    v = lru_get(3); printf(" g3=%d", v);
    v = lru_get(4); printf(" g4=%d", v);
}

void test_dm_large_pattern(void) {
    int i;

    printf(" T12:");
    dm_init(16);

    for (i = 0; i < 16; i = i + 1) {
        dm_access(i);
    }
    for (i = 0; i < 16; i = i + 1) {
        dm_access(i);
    }
    printf("h=%d m=%d", dm_hits, dm_misses);
}

void test_lru_single_element(void) {
    int v;

    printf(" T13:");
    lru_init(1);

    lru_access(1, 10);
    v = lru_get(1); printf("g1=%d", v);
    lru_access(2, 20);
    v = lru_get(1); printf(" g1=%d", v);
    v = lru_get(2); printf(" g2=%d", v);
    printf(" h=%d m=%d", lru_hits, lru_misses);
}

void test_access_pattern_analysis(void) {
    int pattern[20];
    int i;

    printf(" T14:");

    lru_init(4);
    pattern[0]=1; pattern[1]=1; pattern[2]=1;
    pattern[3]=2; pattern[4]=2; pattern[5]=2;
    pattern[6]=3; pattern[7]=3; pattern[8]=3;
    pattern[9]=4; pattern[10]=4; pattern[11]=4;

    for (i = 0; i < 12; i = i + 1) {
        lru_access(pattern[i], pattern[i] * 10);
    }
    printf("h=%d m=%d", lru_hits, lru_misses);
}

void test_sa_conflict(void) {
    printf(" T15:");
    sa_init(4);

    sa_access(0);
    sa_access(4);
    sa_access(8);
    printf("a0=%d", sa_access(0));
    printf(" a4=%d", sa_access(4));
    printf(" h=%d m=%d", sa_hits, sa_misses);
}

void test_lru_zigzag(void) {
    printf(" T16:");
    lru_init(4);

    /* Zigzag pattern: 1,2,3,4,3,2,1,2,3,4 */
    lru_access(1, 10);
    lru_access(2, 20);
    lru_access(3, 30);
    lru_access(4, 40);
    lru_access(3, 30);
    lru_access(2, 20);
    lru_access(1, 10);
    lru_access(2, 20);
    lru_access(3, 30);
    lru_access(4, 40);
    printf("h=%d m=%d", lru_hits, lru_misses);
}

void test_dm_wrap_around(void) {
    int i;

    printf(" T17:");
    dm_init(8);

    /* Access 0..15: 0..7 are misses, 8..15 conflict with 0..7 => all miss */
    for (i = 0; i < 16; i = i + 1) {
        dm_access(i);
    }
    printf("h=%d m=%d", dm_hits, dm_misses);
    /* Access 0..7 - should miss (evicted by 8..15) */
    for (i = 0; i < 8; i = i + 1) {
        dm_access(i);
    }
    printf(" h2=%d m2=%d", dm_hits, dm_misses);
    printf(" DONE\n");
}

int main(void) {
    test_dm_basic();
    test_dm_sequential();
    test_dm_conflict();
    test_lru_basic();
    test_lru_repeated();
    test_lru_thrashing();
    test_lru_working_set();
    test_dm_stride();
    test_sa_basic();
    test_cache_hit_rate();
    test_lru_eviction_order();
    test_dm_large_pattern();
    test_lru_single_element();
    test_access_pattern_analysis();
    test_sa_conflict();
    test_lru_zigzag();
    test_dm_wrap_around();
    return 0;
}
