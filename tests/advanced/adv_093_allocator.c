// EXPECT: === Pool Allocator Init ===
// EXPECT: Pool: 16 blocks of 64 bytes each (1024 total)
// EXPECT: Free blocks: 16
// EXPECT: === Allocation Tests ===
// EXPECT: Alloc block 0: OK
// EXPECT: Alloc block 1: OK
// EXPECT: Alloc block 2: OK
// EXPECT: Alloc block 3: OK
// EXPECT: Free blocks after 4 allocs: 12
// EXPECT: === Write/Read Tests ===
// EXPECT: Block 0 data: 42
// EXPECT: Block 1 data: 99
// EXPECT: Block 2 data: 7
// EXPECT: Block 3 data: 255
// EXPECT: === Free Tests ===
// EXPECT: Freed block 1
// EXPECT: Freed block 3
// EXPECT: Free blocks after 2 frees: 14
// EXPECT: === Reallocation Tests ===
// EXPECT: Realloc into freed slot: OK
// EXPECT: Block 0 still has: 42
// EXPECT: New block data: 123
// EXPECT: === Fragmentation Test ===
// EXPECT: Allocated 12 blocks
// EXPECT: Freed even blocks (0,2,4,6,8,10)
// EXPECT: Free blocks: 10, used blocks: 6
// EXPECT: Reallocated 6 blocks into freed slots
// EXPECT: Free blocks: 4, used blocks: 12
// EXPECT: All allocator tests passed!

int printf(const char *fmt, ...);

// Pool configuration
enum {
    BLOCK_SIZE = 64,
    NUM_BLOCKS = 16,
    POOL_SIZE = 1024  // BLOCK_SIZE * NUM_BLOCKS
};

// Memory pool
static char pool_memory[1024];  // raw storage
static int block_used[16];      // 1 = used, 0 = free

struct PoolAllocator {
    char *memory;
    int *used;
    int block_size;
    int num_blocks;
    int free_count;
};

void pool_init(struct PoolAllocator *p) {
    int i;
    p->memory = pool_memory;
    p->used = block_used;
    p->block_size = BLOCK_SIZE;
    p->num_blocks = NUM_BLOCKS;
    p->free_count = NUM_BLOCKS;
    for (i = 0; i < NUM_BLOCKS; i++) {
        p->used[i] = 0;
    }
    // Zero out memory
    for (i = 0; i < POOL_SIZE; i++) {
        pool_memory[i] = 0;
    }
}

// Allocate one block, return block index or -1
int pool_alloc(struct PoolAllocator *p) {
    int i;
    for (i = 0; i < p->num_blocks; i++) {
        if (p->used[i] == 0) {
            p->used[i] = 1;
            p->free_count = p->free_count - 1;
            return i;
        }
    }
    return -1;
}

// Free a block by index
int pool_free(struct PoolAllocator *p, int block) {
    if (block < 0 || block >= p->num_blocks) return -1;
    if (p->used[block] == 0) return -1;
    p->used[block] = 0;
    p->free_count = p->free_count + 1;
    return 0;
}

// Get pointer to block data
char *pool_get(struct PoolAllocator *p, int block) {
    if (block < 0 || block >= p->num_blocks) return 0;
    return p->memory + (block * p->block_size);
}

// Write an int at start of block
void pool_write_int(struct PoolAllocator *p, int block, int value) {
    char *ptr = pool_get(p, block);
    if (ptr) {
        ptr[0] = (char)(value & 255);
        ptr[1] = (char)((value >> 8) & 255);
        ptr[2] = (char)((value >> 16) & 255);
        ptr[3] = (char)((value >> 24) & 255);
    }
}

// Read an int from start of block
int pool_read_int(struct PoolAllocator *p, int block) {
    char *ptr = pool_get(p, block);
    if (ptr) {
        int v = (ptr[0] & 255);
        v = v | ((ptr[1] & 255) << 8);
        v = v | ((ptr[2] & 255) << 16);
        v = v | ((ptr[3] & 255) << 24);
        return v;
    }
    return -1;
}

int count_used(struct PoolAllocator *p) {
    return p->num_blocks - p->free_count;
}

int main(void) {
    struct PoolAllocator pool;
    int blocks[16];
    int i;
    int b;
    int val;

    // Init
    printf("=== Pool Allocator Init ===\n");
    pool_init(&pool);
    printf("Pool: %d blocks of %d bytes each (%d total)\n",
           pool.num_blocks, pool.block_size, pool.num_blocks * pool.block_size);
    printf("Free blocks: %d\n", pool.free_count);

    // Basic allocation
    printf("=== Allocation Tests ===\n");
    for (i = 0; i < 4; i++) {
        blocks[i] = pool_alloc(&pool);
        printf("Alloc block %d: %s\n", i, blocks[i] >= 0 ? "OK" : "FAIL");
    }
    printf("Free blocks after 4 allocs: %d\n", pool.free_count);

    // Write and read data
    printf("=== Write/Read Tests ===\n");
    pool_write_int(&pool, blocks[0], 42);
    pool_write_int(&pool, blocks[1], 99);
    pool_write_int(&pool, blocks[2], 7);
    pool_write_int(&pool, blocks[3], 255);

    printf("Block 0 data: %d\n", pool_read_int(&pool, blocks[0]));
    printf("Block 1 data: %d\n", pool_read_int(&pool, blocks[1]));
    printf("Block 2 data: %d\n", pool_read_int(&pool, blocks[2]));
    printf("Block 3 data: %d\n", pool_read_int(&pool, blocks[3]));

    // Free some blocks
    printf("=== Free Tests ===\n");
    pool_free(&pool, blocks[1]);
    printf("Freed block 1\n");
    pool_free(&pool, blocks[3]);
    printf("Freed block 3\n");
    printf("Free blocks after 2 frees: %d\n", pool.free_count);

    // Reallocate into freed slots
    printf("=== Reallocation Tests ===\n");
    b = pool_alloc(&pool);
    printf("Realloc into freed slot: %s\n", b >= 0 ? "OK" : "FAIL");
    printf("Block 0 still has: %d\n", pool_read_int(&pool, blocks[0]));
    pool_write_int(&pool, b, 123);
    printf("New block data: %d\n", pool_read_int(&pool, b));

    // Fragmentation test: alloc 12, free every other, realloc
    printf("=== Fragmentation Test ===\n");
    pool_init(&pool);
    for (i = 0; i < 12; i++) {
        blocks[i] = pool_alloc(&pool);
    }
    printf("Allocated 12 blocks\n");

    // Free even-indexed blocks
    for (i = 0; i < 12; i = i + 2) {
        pool_free(&pool, blocks[i]);
    }
    printf("Freed even blocks (0,2,4,6,8,10)\n");
    printf("Free blocks: %d, used blocks: %d\n", pool.free_count, count_used(&pool));

    // Reallocate into the freed slots
    for (i = 0; i < 6; i++) {
        b = pool_alloc(&pool);
    }
    printf("Reallocated 6 blocks into freed slots\n");
    printf("Free blocks: %d, used blocks: %d\n", pool.free_count, count_used(&pool));

    printf("All allocator tests passed!\n");
    return 0;
}
