int printf(const char *fmt, ...);

/* Slab Allocator Simulation
 * Multiple "caches" for different object sizes.
 * Each cache has a slab (contiguous memory region) with fixed-size objects.
 * Free objects are tracked via a freelist within each cache.
 */

/* Memory pool - 512 slots total */
int memory[512];

/* Cache descriptors (max 4 caches) */
int cache_base[4];     /* starting offset in memory[] */
int cache_obj_size[4]; /* object size in slots */
int cache_capacity[4]; /* max objects */
int cache_free_head[4]; /* free list head (-1 = empty) */
int cache_used[4];     /* number of allocated objects */
int num_caches;

void slab_init(void) {
    num_caches = 0;
    int i;
    for (i = 0; i < 512; i++) memory[i] = 0;
    for (i = 0; i < 4; i++) {
        cache_base[i] = 0;
        cache_obj_size[i] = 0;
        cache_capacity[i] = 0;
        cache_free_head[i] = -1;
        cache_used[i] = 0;
    }
}

int next_base;

int slab_create_cache(int obj_size, int capacity) {
    if (num_caches >= 4) return -1;
    int c = num_caches;
    num_caches = num_caches + 1;

    cache_base[c] = next_base;
    cache_obj_size[c] = obj_size;
    cache_capacity[c] = capacity;
    cache_used[c] = 0;

    /* Build free list: each object's first slot holds next free index */
    int i;
    for (i = 0; i < capacity; i++) {
        int off = next_base + i * obj_size;
        if (i < capacity - 1) {
            memory[off] = i + 1;
        } else {
            memory[off] = -1;
        }
    }
    cache_free_head[c] = 0;

    next_base = next_base + capacity * obj_size;
    return c;
}

/* Returns absolute offset in memory[], or -1 */
int slab_alloc(int cache_id) {
    if (cache_id < 0 || cache_id >= num_caches) return -1;
    int head = cache_free_head[cache_id];
    if (head == -1) return -1;

    int base = cache_base[cache_id];
    int osize = cache_obj_size[cache_id];
    int off = base + head * osize;

    cache_free_head[cache_id] = memory[off];
    cache_used[cache_id] = cache_used[cache_id] + 1;

    /* Clear the object */
    int i;
    for (i = 0; i < osize; i++) {
        memory[off + i] = 0;
    }
    return off;
}

void slab_free(int cache_id, int offset) {
    if (cache_id < 0 || cache_id >= num_caches) return;
    int base = cache_base[cache_id];
    int osize = cache_obj_size[cache_id];
    int obj_idx = (offset - base) / osize;

    memory[offset] = cache_free_head[cache_id];
    cache_free_head[cache_id] = obj_idx;
    cache_used[cache_id] = cache_used[cache_id] - 1;
}

void mem_write(int off, int slot, int val) {
    if (off + slot >= 0 && off + slot < 512) {
        memory[off + slot] = val;
    }
}

int mem_read(int off, int slot) {
    if (off + slot >= 0 && off + slot < 512) {
        return memory[off + slot];
    }
    return -1;
}

void test_single_cache(void) {
    printf("=== Single Cache ===\n"); // EXPECT: === Single Cache ===
    slab_init();
    next_base = 0;

    int c = slab_create_cache(4, 8);
    printf("cache created: %d\n", c); // EXPECT: cache created: 0

    int a = slab_alloc(c);
    int b = slab_alloc(c);
    int d = slab_alloc(c);
    printf("alloc: %d %d %d\n", a, b, d); // EXPECT: alloc: 0 4 8

    mem_write(a, 0, 10);
    mem_write(a, 1, 20);
    mem_write(b, 0, 30);
    printf("read: %d %d %d\n", mem_read(a, 0), mem_read(a, 1), mem_read(b, 0)); // EXPECT: read: 10 20 30

    printf("used=%d\n", cache_used[c]); // EXPECT: used=3
}

void test_multi_cache(void) {
    printf("=== Multiple Caches ===\n"); // EXPECT: === Multiple Caches ===
    slab_init();
    next_base = 0;

    int small = slab_create_cache(2, 10);  /* small objects: 2 slots each */
    int medium = slab_create_cache(4, 8);  /* medium: 4 slots each */
    int large = slab_create_cache(8, 4);   /* large: 8 slots each */

    printf("caches: %d %d %d\n", small, medium, large); // EXPECT: caches: 0 1 2

    int s1 = slab_alloc(small);
    int s2 = slab_alloc(small);
    int m1 = slab_alloc(medium);
    int l1 = slab_alloc(large);

    printf("s1=%d s2=%d m1=%d l1=%d\n", s1, s2, m1, l1); // EXPECT: s1=0 s2=2 m1=20 l1=52

    mem_write(s1, 0, 1);
    mem_write(m1, 0, 2);
    mem_write(l1, 0, 3);
    printf("values: %d %d %d\n", mem_read(s1, 0), mem_read(m1, 0), mem_read(l1, 0)); // EXPECT: values: 1 2 3
}

void test_free_reuse(void) {
    printf("=== Slab Free/Reuse ===\n"); // EXPECT: === Slab Free/Reuse ===
    slab_init();
    next_base = 0;

    int c = slab_create_cache(3, 4);

    int a = slab_alloc(c);
    int b = slab_alloc(c);
    int d = slab_alloc(c);
    int e = slab_alloc(c);
    printf("all alloc: %d %d %d %d\n", a, b, d, e); // EXPECT: all alloc: 0 3 6 9

    int f = slab_alloc(c);
    printf("full: %d\n", f); // EXPECT: full: -1

    slab_free(c, b);
    slab_free(c, e);
    printf("used after free: %d\n", cache_used[c]); // EXPECT: used after free: 2

    int g = slab_alloc(c);
    int h = slab_alloc(c);
    printf("reuse: %d %d\n", g, h); // EXPECT: reuse: 9 3

    printf("used final: %d\n", cache_used[c]); // EXPECT: used final: 4
}

void test_stress(void) {
    printf("=== Slab Stress ===\n"); // EXPECT: === Slab Stress ===
    slab_init();
    next_base = 0;

    int c = slab_create_cache(2, 16);

    /* Allocate all, write values, then free all, then reallocate */
    int addrs[16];
    int i;
    for (i = 0; i < 16; i++) {
        addrs[i] = slab_alloc(c);
        mem_write(addrs[i], 0, i * 10);
    }
    printf("used=%d\n", cache_used[c]); // EXPECT: used=16

    int sum = 0;
    for (i = 0; i < 16; i++) {
        sum = sum + mem_read(addrs[i], 0);
    }
    printf("sum=%d\n", sum); // EXPECT: sum=1200

    for (i = 0; i < 16; i++) {
        slab_free(c, addrs[i]);
    }
    printf("after free all: used=%d\n", cache_used[c]); // EXPECT: after free all: used=0

    int count = 0;
    for (i = 0; i < 16; i++) {
        int addr = slab_alloc(c);
        if (addr >= 0) count = count + 1;
    }
    printf("reallocated: %d\n", count); // EXPECT: reallocated: 16
}

int main(void) {
    test_single_cache();
    test_multi_cache();
    test_free_reuse();
    test_stress();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
