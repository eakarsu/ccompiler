int printf(const char *fmt, ...);

/* Pool Allocator Simulation
 * Fixed-size block pool. Each block is the same size.
 * Free list is maintained via indices stored in the first slot of each free block.
 */

int pool[128];
int block_size;
int num_blocks;
int free_head;   /* index of first free block, -1 if none */
int alloc_count;

void pool_init(int bsize, int count) {
    block_size = bsize;
    num_blocks = count;
    alloc_count = 0;
    int i;
    /* Initialize free list: each block points to the next */
    for (i = 0; i < count; i++) {
        int base = i * bsize;
        if (i < count - 1) {
            pool[base] = i + 1; /* next free block index */
        } else {
            pool[base] = -1;    /* end of free list */
        }
        /* Clear remaining slots */
        int j;
        for (j = 1; j < bsize; j++) {
            pool[base + j] = 0;
        }
    }
    free_head = 0;
}

int pool_alloc(void) {
    if (free_head == -1) {
        return -1;
    }
    int block_idx = free_head;
    int base = block_idx * block_size;
    free_head = pool[base]; /* follow free list */
    /* Clear the block */
    int i;
    for (i = 0; i < block_size; i++) {
        pool[base + i] = 0;
    }
    alloc_count = alloc_count + 1;
    return block_idx;
}

void pool_free(int block_idx) {
    if (block_idx < 0 || block_idx >= num_blocks) {
        return;
    }
    int base = block_idx * block_size;
    pool[base] = free_head;
    free_head = block_idx;
    alloc_count = alloc_count - 1;
}

void pool_write(int block_idx, int offset, int val) {
    if (block_idx < 0 || block_idx >= num_blocks) return;
    if (offset < 0 || offset >= block_size) return;
    pool[block_idx * block_size + offset] = val;
}

int pool_read(int block_idx, int offset) {
    if (block_idx < 0 || block_idx >= num_blocks) return -1;
    if (offset < 0 || offset >= block_size) return -1;
    return pool[block_idx * block_size + offset];
}

int pool_free_count(void) {
    int count = 0;
    int cur = free_head;
    while (cur != -1) {
        count = count + 1;
        cur = pool[cur * block_size];
    }
    return count;
}

void test_basic(void) {
    printf("=== Pool Allocator Basic ===\n"); // EXPECT: === Pool Allocator Basic ===
    pool_init(4, 8);

    int a = pool_alloc();
    int b = pool_alloc();
    int c = pool_alloc();
    printf("alloc: %d %d %d\n", a, b, c); // EXPECT: alloc: 0 1 2
    printf("alloc_count=%d free=%d\n", alloc_count, pool_free_count()); // EXPECT: alloc_count=3 free=5

    pool_write(a, 0, 10);
    pool_write(a, 1, 20);
    pool_write(b, 0, 30);
    pool_write(c, 2, 40);
    printf("a[0]=%d a[1]=%d b[0]=%d c[2]=%d\n",
           pool_read(a, 0), pool_read(a, 1), pool_read(b, 0), pool_read(c, 2)); // EXPECT: a[0]=10 a[1]=20 b[0]=30 c[2]=40
}

void test_free_reuse(void) {
    printf("=== Free and Reuse ===\n"); // EXPECT: === Free and Reuse ===
    pool_init(4, 4);

    int a = pool_alloc();
    int b = pool_alloc();
    int c = pool_alloc();
    int d = pool_alloc();
    printf("all allocated: %d %d %d %d\n", a, b, c, d); // EXPECT: all allocated: 0 1 2 3
    printf("free count=%d\n", pool_free_count()); // EXPECT: free count=0

    int e = pool_alloc();
    printf("alloc when full: %d\n", e); // EXPECT: alloc when full: -1

    pool_free(b);
    pool_free(d);
    printf("after freeing b,d: free=%d\n", pool_free_count()); // EXPECT: after freeing b,d: free=2

    int f = pool_alloc();
    int g = pool_alloc();
    printf("reuse: %d %d\n", f, g); // EXPECT: reuse: 3 1

    int h = pool_alloc();
    printf("alloc again when full: %d\n", h); // EXPECT: alloc again when full: -1
}

void test_all_free(void) {
    printf("=== Free All ===\n"); // EXPECT: === Free All ===
    pool_init(2, 6);

    int blocks[6];
    int i;
    for (i = 0; i < 6; i++) {
        blocks[i] = pool_alloc();
    }
    printf("allocated %d blocks\n", alloc_count); // EXPECT: allocated 6 blocks

    for (i = 0; i < 6; i++) {
        pool_free(blocks[i]);
    }
    printf("after free all: count=%d free=%d\n", alloc_count, pool_free_count()); // EXPECT: after free all: count=0 free=6

    /* Reallocate all */
    for (i = 0; i < 6; i++) {
        blocks[i] = pool_alloc();
        pool_write(blocks[i], 0, (i + 1) * 100);
    }
    int sum = 0;
    for (i = 0; i < 6; i++) {
        sum = sum + pool_read(blocks[i], 0);
    }
    printf("sum after realloc: %d\n", sum); // EXPECT: sum after realloc: 2100
}

void test_fifo_pattern(void) {
    printf("=== FIFO Pattern ===\n"); // EXPECT: === FIFO Pattern ===
    pool_init(3, 5);

    int total_ops = 0;
    int i;
    for (i = 0; i < 10; i++) {
        int blk = pool_alloc();
        if (blk >= 0) {
            pool_write(blk, 0, i);
            total_ops = total_ops + 1;
            pool_free(blk);
        }
    }
    printf("total ops: %d\n", total_ops); // EXPECT: total ops: 10
    printf("final free count: %d\n", pool_free_count()); // EXPECT: final free count: 5
}

int main(void) {
    test_basic();
    test_free_reuse();
    test_all_free();
    test_fifo_pattern();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
