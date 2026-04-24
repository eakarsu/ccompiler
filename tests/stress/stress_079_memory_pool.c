// stress_079_memory_pool.c - Memory pool: fixed-size allocator
// EXPECT: init_free=64
// EXPECT: alloc_all_ok=1
// EXPECT: after_alloc_free=0
// EXPECT: overflow_alloc=-1
// EXPECT: read_back_ok=1
// EXPECT: after_free_all=64
// EXPECT: double_free_caught=1
// EXPECT: mid_alloc_free=32
// EXPECT: after_free_odd=48
// EXPECT: realloc_ok=1
// EXPECT: after_realloc_free=32
// EXPECT: clean_slate=64
// EXPECT: pattern_no_corrupt=1
// EXPECT: rapid_stress_ok=1
// EXPECT: final_free=64

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

// Fixed-size memory pool: 64 blocks of 32 bytes each
// Total pool size: 64 * 32 = 2048 bytes
typedef struct {
    char memory[2048];  // 64 * 32
    int free_bitmap[64]; // 1 = free, 0 = allocated
    int total_blocks;
    int block_size;
    int free_count;
} MemPool;

void pool_init(MemPool *p) {
    p->total_blocks = 64;
    p->block_size = 32;
    p->free_count = 64;
    int i;
    for (i = 0; i < 64; i++) p->free_bitmap[i] = 1;
    // Zero out memory
    for (i = 0; i < 2048; i++) p->memory[i] = 0;
}

// Allocate a block, returns block index (-1 on failure)
int pool_alloc(MemPool *p) {
    int i;
    for (i = 0; i < p->total_blocks; i++) {
        if (p->free_bitmap[i]) {
            p->free_bitmap[i] = 0;
            p->free_count--;
            return i;
        }
    }
    return -1;
}

// Free a block
int pool_free(MemPool *p, int block) {
    if (block < 0 || block >= p->total_blocks) return 0;
    if (p->free_bitmap[block]) return 0; // double free
    p->free_bitmap[block] = 1;
    p->free_count++;
    return 1;
}

// Get pointer to block's memory
char *pool_get(MemPool *p, int block) {
    return &p->memory[block * p->block_size];
}

// Write an int at the start of a block
void pool_write_int(MemPool *p, int block, int val) {
    char *ptr = pool_get(p, block);
    ptr[0] = (char)(val & 0xFF);
    ptr[1] = (char)((val >> 8) & 0xFF);
    ptr[2] = (char)((val >> 16) & 0xFF);
    ptr[3] = (char)((val >> 24) & 0xFF);
}

// Read an int from start of a block
int pool_read_int(MemPool *p, int block) {
    char *ptr = pool_get(p, block);
    int val = (ptr[0] & 0xFF)
            | ((ptr[1] & 0xFF) << 8)
            | ((ptr[2] & 0xFF) << 16)
            | ((ptr[3] & 0xFF) << 24);
    return val;
}

// Fill a block with a byte pattern
void pool_fill(MemPool *p, int block, char pattern) {
    char *ptr = pool_get(p, block);
    int i;
    for (i = 0; i < p->block_size; i++) ptr[i] = pattern;
}

// Verify a block has expected pattern
int pool_verify_fill(MemPool *p, int block, char pattern) {
    char *ptr = pool_get(p, block);
    int i;
    for (i = 0; i < p->block_size; i++) {
        if (ptr[i] != pattern) return 0;
    }
    return 1;
}

int main(void) {
    MemPool pool;
    pool_init(&pool);

    printf("init_free=%d\n", pool.free_count);

    // Allocate all 64 blocks
    int blocks[64];
    int i;
    int alloc_ok = 1;
    for (i = 0; i < 64; i++) {
        blocks[i] = pool_alloc(&pool);
        if (blocks[i] < 0) alloc_ok = 0;
    }
    printf("alloc_all_ok=%d\n", alloc_ok);
    printf("after_alloc_free=%d\n", pool.free_count);

    // Try to allocate one more (should fail)
    int overflow = pool_alloc(&pool);
    printf("overflow_alloc=%d\n", overflow);

    // Write unique values to each block
    for (i = 0; i < 64; i++) {
        pool_write_int(&pool, blocks[i], (i + 1) * 100);
    }

    // Read back and verify
    int read_ok = 1;
    for (i = 0; i < 64; i++) {
        if (pool_read_int(&pool, blocks[i]) != (i + 1) * 100) read_ok = 0;
    }
    printf("read_back_ok=%d\n", read_ok);

    // Free all blocks
    for (i = 0; i < 64; i++) {
        pool_free(&pool, blocks[i]);
    }
    printf("after_free_all=%d\n", pool.free_count);

    // Double free should fail
    int dbl_free = pool_free(&pool, 0);
    printf("double_free_caught=%d\n", dbl_free == 0);

    // Interleaved alloc/free pattern
    // Allocate 32, free odd-indexed ones, allocate 16 more
    for (i = 0; i < 32; i++) {
        blocks[i] = pool_alloc(&pool);
    }
    printf("mid_alloc_free=%d\n", pool.free_count);

    for (i = 1; i < 32; i += 2) {
        pool_free(&pool, blocks[i]);
    }
    printf("after_free_odd=%d\n", pool.free_count);

    // Allocate 16 more into the freed slots
    int new_blocks[16];
    int new_alloc_ok = 1;
    for (i = 0; i < 16; i++) {
        new_blocks[i] = pool_alloc(&pool);
        if (new_blocks[i] < 0) new_alloc_ok = 0;
    }
    printf("realloc_ok=%d\n", new_alloc_ok);
    printf("after_realloc_free=%d\n", pool.free_count);

    // Fill pattern test: write different patterns to adjacent blocks
    // Free everything first
    for (i = 0; i < 32; i += 2) pool_free(&pool, blocks[i]);
    for (i = 0; i < 16; i++) pool_free(&pool, new_blocks[i]);
    printf("clean_slate=%d\n", pool.free_count);

    // Allocate 4 adjacent blocks, fill with different patterns
    int b0 = pool_alloc(&pool);
    int b1 = pool_alloc(&pool);
    int b2 = pool_alloc(&pool);
    int b3 = pool_alloc(&pool);

    pool_fill(&pool, b0, 'A');
    pool_fill(&pool, b1, 'B');
    pool_fill(&pool, b2, 'C');
    pool_fill(&pool, b3, 'D');

    // Verify no corruption between blocks
    int pat_ok = pool_verify_fill(&pool, b0, 'A')
              && pool_verify_fill(&pool, b1, 'B')
              && pool_verify_fill(&pool, b2, 'C')
              && pool_verify_fill(&pool, b3, 'D');
    printf("pattern_no_corrupt=%d\n", pat_ok);

    // Stress: allocate/free rapidly 100 times
    pool_free(&pool, b0);
    pool_free(&pool, b1);
    pool_free(&pool, b2);
    pool_free(&pool, b3);

    int stress_ok = 1;
    for (i = 0; i < 100; i++) {
        int s1 = pool_alloc(&pool);
        int s2 = pool_alloc(&pool);
        if (s1 < 0 || s2 < 0) { stress_ok = 0; break; }
        pool_write_int(&pool, s1, i * 7);
        pool_write_int(&pool, s2, i * 13);
        if (pool_read_int(&pool, s1) != i * 7) stress_ok = 0;
        if (pool_read_int(&pool, s2) != i * 13) stress_ok = 0;
        pool_free(&pool, s1);
        pool_free(&pool, s2);
    }
    printf("rapid_stress_ok=%d\n", stress_ok);
    printf("final_free=%d\n", pool.free_count);

    return 0;
}
