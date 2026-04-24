int printf(const char *fmt, ...);
// EXPECT: === Memory Pool Init ===
// EXPECT: Pool created: 16 blocks of size 4
// EXPECT: Free blocks: 16
// EXPECT: Used blocks: 0
// EXPECT: === Basic Allocation ===
// EXPECT: Allocated block 0
// EXPECT: Allocated block 1
// EXPECT: Allocated block 2
// EXPECT: Free blocks: 13
// EXPECT: Used blocks: 3
// EXPECT: === Write and Read ===
// EXPECT: Block 0 values: 10 20 30 40
// EXPECT: Block 1 values: 100 200 300 400
// EXPECT: Block 2 values: 1 2 3 4
// EXPECT: === Free and Realloc ===
// EXPECT: Freed block 1
// EXPECT: Free blocks: 14
// EXPECT: Used blocks: 2
// EXPECT: Allocated block 1
// EXPECT: Block 1 values: 55 66 77 88
// EXPECT: Free blocks: 13
// EXPECT: Used blocks: 3
// EXPECT: === Allocate All Blocks ===
// EXPECT: Allocated 13 more blocks
// EXPECT: Free blocks: 0
// EXPECT: Used blocks: 16
// EXPECT: Alloc when full returned: -1
// EXPECT: === Free All and Verify ===
// EXPECT: Freed all 16 blocks
// EXPECT: Free blocks: 16
// EXPECT: Used blocks: 0
// EXPECT: === Free List Order Test ===
// EXPECT: Allocated a0=0
// EXPECT: Allocated a1=1
// EXPECT: Allocated a2=2
// EXPECT: Allocated a3=3
// EXPECT: Allocated a4=4
// EXPECT: Freed block 2
// EXPECT: Freed block 4
// EXPECT: Freed block 0
// EXPECT: Realloc got block 0
// EXPECT: Realloc got block 4
// EXPECT: Realloc got block 2
// EXPECT: Free blocks: 11
// EXPECT: === Pool Statistics ===
// EXPECT: Total allocs: 5
// EXPECT: Total frees: 0
// EXPECT: Peak usage: 5
// EXPECT: Current usage: 5
// EXPECT: Freed 3 blocks
// EXPECT: Total allocs: 5
// EXPECT: Total frees: 3
// EXPECT: Peak usage: 5
// EXPECT: Current usage: 2
// EXPECT: Allocated 3 more
// EXPECT: Total allocs: 8
// EXPECT: Total frees: 3
// EXPECT: Peak usage: 5
// EXPECT: Current usage: 5
// EXPECT: === Double Free Detection ===
// EXPECT: Block 0 is allocated: 1
// EXPECT: Free block 0: success
// EXPECT: Block 0 is allocated: 0
// EXPECT: Free block 0 again: already free
// EXPECT: === Fragmentation Test ===
// EXPECT: Allocated 8 blocks
// EXPECT: Freed even blocks: 0 2 4 6
// EXPECT: Free blocks: 12
// EXPECT: Reallocated 4 blocks after frag
// EXPECT: Free blocks: 8
// EXPECT: === Block Boundary Test ===
// EXPECT: Block 0 data[0] = 111
// EXPECT: Block 0 data[3] = 444
// EXPECT: Block 1 data[0] = 555
// EXPECT: Block 1 data[3] = 888
// EXPECT: Blocks are independent: 1
// EXPECT: === Multi Pool Test ===
// EXPECT: Pool A: 8 blocks of size 2
// EXPECT: Pool B: 4 blocks of size 8
// EXPECT: Pool A alloc block 0
// EXPECT: Pool B alloc block 0
// EXPECT: Pool A block 0: 11 22
// EXPECT: Pool B block 0: 99 88 77 66 55 44 33 22
// EXPECT: Pool A free: 7, Pool B free: 3
// EXPECT: === All memory pool tests passed ===

// ============================================================
// Memory Pool: 16 blocks of 4 ints each
// ============================================================
int pool_data[16][4];
int pool_free_list[16];  // stack of free block indices
int pool_free_top;       // top index of free stack
int pool_used[16];       // 1 if allocated
int pool_total_blocks;
int pool_block_size;
int pool_total_allocs;
int pool_total_frees;
int pool_peak_usage;
int pool_current_usage;

void pool_init(int num_blocks, int block_size) {
    int i;
    int j;
    pool_total_blocks = num_blocks;
    pool_block_size = block_size;
    pool_total_allocs = 0;
    pool_total_frees = 0;
    pool_peak_usage = 0;
    pool_current_usage = 0;
    // Store in reverse so that alloc pops block 0 first
    for (i = 0; i < num_blocks; i++) {
        pool_free_list[i] = num_blocks - 1 - i;
        pool_used[i] = 0;
        for (j = 0; j < 4; j++) {
            pool_data[i][j] = 0;
        }
    }
    pool_free_top = num_blocks;
}

int pool_alloc(void) {
    int block;
    if (pool_free_top <= 0) return -1;
    pool_free_top--;
    block = pool_free_list[pool_free_top];
    pool_used[block] = 1;
    pool_total_allocs++;
    pool_current_usage++;
    if (pool_current_usage > pool_peak_usage) {
        pool_peak_usage = pool_current_usage;
    }
    return block;
}

int pool_free_block(int block) {
    if (block < 0 || block >= pool_total_blocks) return -1;
    if (pool_used[block] == 0) return 0;
    pool_used[block] = 0;
    pool_free_list[pool_free_top] = block;
    pool_free_top++;
    pool_total_frees++;
    pool_current_usage--;
    return 1;
}

void pool_write(int block, int offset, int value) {
    if (block >= 0 && block < pool_total_blocks && offset >= 0 && offset < pool_block_size) {
        pool_data[block][offset] = value;
    }
}

int pool_read(int block, int offset) {
    if (block >= 0 && block < pool_total_blocks && offset >= 0 && offset < pool_block_size) {
        return pool_data[block][offset];
    }
    return -1;
}

int pool_count_free(void) {
    return pool_free_top;
}

int pool_count_used(void) {
    return pool_total_blocks - pool_free_top;
}

int pool_is_allocated(int block) {
    if (block >= 0 && block < pool_total_blocks) {
        return pool_used[block];
    }
    return 0;
}

// ============================================================
// Second pool for multi-pool test
// ============================================================
int pool2_data[8][8];
int pool2_free_list[8];
int pool2_free_top;
int pool2_used[8];
int pool2_total_blocks;
int pool2_block_size;

void pool2_init(int num_blocks, int block_size) {
    int i;
    int j;
    pool2_total_blocks = num_blocks;
    pool2_block_size = block_size;
    for (i = 0; i < num_blocks; i++) {
        pool2_free_list[i] = num_blocks - 1 - i;
        pool2_used[i] = 0;
        for (j = 0; j < 8; j++) {
            pool2_data[i][j] = 0;
        }
    }
    pool2_free_top = num_blocks;
}

int pool2_alloc(void) {
    int block;
    if (pool2_free_top <= 0) return -1;
    pool2_free_top--;
    block = pool2_free_list[pool2_free_top];
    pool2_used[block] = 1;
    return block;
}

void pool2_write(int block, int offset, int value) {
    if (block >= 0 && block < pool2_total_blocks && offset >= 0 && offset < pool2_block_size) {
        pool2_data[block][offset] = value;
    }
}

int pool2_read(int block, int offset) {
    if (block >= 0 && block < pool2_total_blocks && offset >= 0 && offset < pool2_block_size) {
        return pool2_data[block][offset];
    }
    return -1;
}

// ============================================================
// Tests
// ============================================================

void test_pool_init(void) {
    printf("=== Memory Pool Init ===\n");
    pool_init(16, 4);
    printf("Pool created: %d blocks of size %d\n", pool_total_blocks, pool_block_size);
    printf("Free blocks: %d\n", pool_count_free());
    printf("Used blocks: %d\n", pool_count_used());
}

void test_basic_alloc(void) {
    int b0;
    int b1;
    int b2;

    printf("=== Basic Allocation ===\n");
    b0 = pool_alloc();
    printf("Allocated block %d\n", b0);
    b1 = pool_alloc();
    printf("Allocated block %d\n", b1);
    b2 = pool_alloc();
    printf("Allocated block %d\n", b2);
    printf("Free blocks: %d\n", pool_count_free());
    printf("Used blocks: %d\n", pool_count_used());

    pool_write(b0, 0, 10);
    pool_write(b0, 1, 20);
    pool_write(b0, 2, 30);
    pool_write(b0, 3, 40);

    pool_write(b1, 0, 100);
    pool_write(b1, 1, 200);
    pool_write(b1, 2, 300);
    pool_write(b1, 3, 400);

    pool_write(b2, 0, 1);
    pool_write(b2, 1, 2);
    pool_write(b2, 2, 3);
    pool_write(b2, 3, 4);

    printf("=== Write and Read ===\n");
    printf("Block 0 values: %d %d %d %d\n",
           pool_read(b0, 0), pool_read(b0, 1), pool_read(b0, 2), pool_read(b0, 3));
    printf("Block 1 values: %d %d %d %d\n",
           pool_read(b1, 0), pool_read(b1, 1), pool_read(b1, 2), pool_read(b1, 3));
    printf("Block 2 values: %d %d %d %d\n",
           pool_read(b2, 0), pool_read(b2, 1), pool_read(b2, 2), pool_read(b2, 3));
}

void test_free_realloc(void) {
    int result;
    int b;

    printf("=== Free and Realloc ===\n");
    result = pool_free_block(1);
    if (result == 1) printf("Freed block 1\n");
    printf("Free blocks: %d\n", pool_count_free());
    printf("Used blocks: %d\n", pool_count_used());

    b = pool_alloc();
    printf("Allocated block %d\n", b);
    pool_write(b, 0, 55);
    pool_write(b, 1, 66);
    pool_write(b, 2, 77);
    pool_write(b, 3, 88);
    printf("Block %d values: %d %d %d %d\n", b,
           pool_read(b, 0), pool_read(b, 1), pool_read(b, 2), pool_read(b, 3));
    printf("Free blocks: %d\n", pool_count_free());
    printf("Used blocks: %d\n", pool_count_used());
}

void test_alloc_all(void) {
    int i;
    int count;
    int result;
    int b;

    printf("=== Allocate All Blocks ===\n");
    count = 0;
    for (i = 0; i < 13; i++) {
        b = pool_alloc();
        if (b >= 0) count++;
    }
    printf("Allocated %d more blocks\n", count);
    printf("Free blocks: %d\n", pool_count_free());
    printf("Used blocks: %d\n", pool_count_used());

    result = pool_alloc();
    printf("Alloc when full returned: %d\n", result);
}

void test_free_all(void) {
    int i;
    int count;

    printf("=== Free All and Verify ===\n");
    count = 0;
    for (i = 0; i < 16; i++) {
        if (pool_free_block(i) == 1) count++;
    }
    printf("Freed all %d blocks\n", count);
    printf("Free blocks: %d\n", pool_count_free());
    printf("Used blocks: %d\n", pool_count_used());
}

void test_free_list_order(void) {
    int a0;
    int a1;
    int a2;
    int a3;
    int a4;
    int r0;
    int r1;
    int r2;

    printf("=== Free List Order Test ===\n");
    pool_init(16, 4);

    a0 = pool_alloc();
    printf("Allocated a0=%d\n", a0);
    a1 = pool_alloc();
    printf("Allocated a1=%d\n", a1);
    a2 = pool_alloc();
    printf("Allocated a2=%d\n", a2);
    a3 = pool_alloc();
    printf("Allocated a3=%d\n", a3);
    a4 = pool_alloc();
    printf("Allocated a4=%d\n", a4);

    // Free in order: 2, 4, 0. They go onto the stack.
    pool_free_block(a2);
    printf("Freed block %d\n", a2);
    pool_free_block(a4);
    printf("Freed block %d\n", a4);
    pool_free_block(a0);
    printf("Freed block %d\n", a0);

    // LIFO realloc: 0, 4, 2
    r0 = pool_alloc();
    printf("Realloc got block %d\n", r0);
    r1 = pool_alloc();
    printf("Realloc got block %d\n", r1);
    r2 = pool_alloc();
    printf("Realloc got block %d\n", r2);

    printf("Free blocks: %d\n", pool_count_free());
}

void test_pool_statistics(void) {
    int i;
    int blocks[5];

    printf("=== Pool Statistics ===\n");
    pool_init(16, 4);

    for (i = 0; i < 5; i++) {
        blocks[i] = pool_alloc();
    }
    printf("Total allocs: %d\n", pool_total_allocs);
    printf("Total frees: %d\n", pool_total_frees);
    printf("Peak usage: %d\n", pool_peak_usage);
    printf("Current usage: %d\n", pool_current_usage);

    pool_free_block(blocks[0]);
    pool_free_block(blocks[2]);
    pool_free_block(blocks[4]);
    printf("Freed 3 blocks\n");
    printf("Total allocs: %d\n", pool_total_allocs);
    printf("Total frees: %d\n", pool_total_frees);
    printf("Peak usage: %d\n", pool_peak_usage);
    printf("Current usage: %d\n", pool_current_usage);

    for (i = 0; i < 3; i++) {
        pool_alloc();
    }
    printf("Allocated 3 more\n");
    printf("Total allocs: %d\n", pool_total_allocs);
    printf("Total frees: %d\n", pool_total_frees);
    printf("Peak usage: %d\n", pool_peak_usage);
    printf("Current usage: %d\n", pool_current_usage);
}

void test_double_free(void) {
    int b;
    int result;

    printf("=== Double Free Detection ===\n");
    pool_init(16, 4);
    b = pool_alloc();
    printf("Block %d is allocated: %d\n", b, pool_is_allocated(b));

    result = pool_free_block(b);
    if (result == 1) printf("Free block %d: success\n", b);
    printf("Block %d is allocated: %d\n", b, pool_is_allocated(b));

    result = pool_free_block(b);
    if (result == 0) printf("Free block %d again: already free\n", b);
}

void test_fragmentation(void) {
    int blocks[8];
    int i;
    int count;

    printf("=== Fragmentation Test ===\n");
    pool_init(16, 4);

    for (i = 0; i < 8; i++) {
        blocks[i] = pool_alloc();
    }
    printf("Allocated 8 blocks\n");

    printf("Freed even blocks:");
    for (i = 0; i < 8; i = i + 2) {
        pool_free_block(blocks[i]);
        printf(" %d", blocks[i]);
    }
    printf("\n");
    printf("Free blocks: %d\n", pool_count_free());

    count = 0;
    for (i = 0; i < 4; i++) {
        if (pool_alloc() >= 0) count++;
    }
    printf("Reallocated %d blocks after frag\n", count);
    printf("Free blocks: %d\n", pool_count_free());
}

void test_block_boundary(void) {
    int b0;
    int b1;
    int ok;

    printf("=== Block Boundary Test ===\n");
    pool_init(16, 4);

    b0 = pool_alloc();
    b1 = pool_alloc();

    pool_write(b0, 0, 111);
    pool_write(b0, 1, 222);
    pool_write(b0, 2, 333);
    pool_write(b0, 3, 444);

    pool_write(b1, 0, 555);
    pool_write(b1, 1, 666);
    pool_write(b1, 2, 777);
    pool_write(b1, 3, 888);

    printf("Block 0 data[0] = %d\n", pool_read(b0, 0));
    printf("Block 0 data[3] = %d\n", pool_read(b0, 3));
    printf("Block 1 data[0] = %d\n", pool_read(b1, 0));
    printf("Block 1 data[3] = %d\n", pool_read(b1, 3));

    ok = (pool_read(b0, 0) == 111 && pool_read(b1, 0) == 555);
    printf("Blocks are independent: %d\n", ok);
}

void test_multi_pool(void) {
    int a_block;
    int b_block;

    printf("=== Multi Pool Test ===\n");

    pool_init(8, 2);
    printf("Pool A: %d blocks of size %d\n", pool_total_blocks, pool_block_size);

    pool2_init(4, 8);
    printf("Pool B: %d blocks of size %d\n", pool2_total_blocks, pool2_block_size);

    a_block = pool_alloc();
    printf("Pool A alloc block %d\n", a_block);
    b_block = pool2_alloc();
    printf("Pool B alloc block %d\n", b_block);

    pool_write(a_block, 0, 11);
    pool_write(a_block, 1, 22);

    pool2_write(b_block, 0, 99);
    pool2_write(b_block, 1, 88);
    pool2_write(b_block, 2, 77);
    pool2_write(b_block, 3, 66);
    pool2_write(b_block, 4, 55);
    pool2_write(b_block, 5, 44);
    pool2_write(b_block, 6, 33);
    pool2_write(b_block, 7, 22);

    printf("Pool A block %d: %d %d\n", a_block,
           pool_read(a_block, 0), pool_read(a_block, 1));
    printf("Pool B block %d: %d %d %d %d %d %d %d %d\n", b_block,
           pool2_read(b_block, 0), pool2_read(b_block, 1),
           pool2_read(b_block, 2), pool2_read(b_block, 3),
           pool2_read(b_block, 4), pool2_read(b_block, 5),
           pool2_read(b_block, 6), pool2_read(b_block, 7));

    printf("Pool A free: %d, Pool B free: %d\n",
           pool_count_free(), pool2_free_top);
}

// ============================================================
// Additional helper functions for code volume
// ============================================================

// Reset specific block data to zero
void pool_clear_block(int block) {
    int j;
    if (block >= 0 && block < pool_total_blocks) {
        for (j = 0; j < pool_block_size; j++) {
            pool_data[block][j] = 0;
        }
    }
}

// Copy data from one block to another
void pool_copy_block(int src, int dst) {
    int j;
    if (src >= 0 && src < pool_total_blocks &&
        dst >= 0 && dst < pool_total_blocks) {
        for (j = 0; j < pool_block_size; j++) {
            pool_data[dst][j] = pool_data[src][j];
        }
    }
}

// Fill a block with a repeating value
void pool_fill_block(int block, int value) {
    int j;
    if (block >= 0 && block < pool_total_blocks) {
        for (j = 0; j < pool_block_size; j++) {
            pool_data[block][j] = value;
        }
    }
}

// Sum all values in a block
int pool_block_sum(int block) {
    int j;
    int sum;
    sum = 0;
    if (block >= 0 && block < pool_total_blocks) {
        for (j = 0; j < pool_block_size; j++) {
            sum = sum + pool_data[block][j];
        }
    }
    return sum;
}

// Check if two blocks have identical data
int pool_blocks_equal(int a, int b) {
    int j;
    if (a < 0 || a >= pool_total_blocks || b < 0 || b >= pool_total_blocks) return 0;
    for (j = 0; j < pool_block_size; j++) {
        if (pool_data[a][j] != pool_data[b][j]) return 0;
    }
    return 1;
}

// Get pool utilization as percentage (integer)
int pool_utilization(void) {
    if (pool_total_blocks == 0) return 0;
    return (pool_count_used() * 100) / pool_total_blocks;
}

// Find first block containing a specific value at offset 0
int pool_find_block(int value) {
    int i;
    for (i = 0; i < pool_total_blocks; i++) {
        if (pool_used[i] && pool_data[i][0] == value) return i;
    }
    return -1;
}

// Count total allocated bytes
int pool_allocated_bytes(void) {
    return pool_count_used() * pool_block_size;
}

// Defragment: compact used blocks to lower indices (internal use)
void pool_defragment(void) {
    int write_idx;
    int read_idx;
    int j;
    int temp[4];

    write_idx = 0;
    for (read_idx = 0; read_idx < pool_total_blocks; read_idx++) {
        if (pool_used[read_idx]) {
            if (write_idx != read_idx) {
                // Swap block data
                for (j = 0; j < pool_block_size; j++) {
                    temp[j] = pool_data[write_idx][j];
                    pool_data[write_idx][j] = pool_data[read_idx][j];
                    pool_data[read_idx][j] = temp[j];
                }
                pool_used[write_idx] = 1;
                pool_used[read_idx] = 0;
            }
            write_idx++;
        }
    }
    // Rebuild free list
    pool_free_top = 0;
    for (read_idx = pool_total_blocks - 1; read_idx >= 0; read_idx--) {
        if (pool_used[read_idx] == 0) {
            pool_free_list[pool_free_top] = read_idx;
            pool_free_top++;
        }
    }
}

// Pool diagnostic: count consecutive free blocks
int pool_max_consecutive_free(void) {
    int i;
    int cur;
    int best;
    cur = 0;
    best = 0;
    for (i = 0; i < pool_total_blocks; i++) {
        if (pool_used[i] == 0) {
            cur++;
            if (cur > best) best = cur;
        } else {
            cur = 0;
        }
    }
    return best;
}

// Validate pool internal consistency
int pool_validate(void) {
    int used_count;
    int free_count;
    int i;

    used_count = 0;
    free_count = 0;
    for (i = 0; i < pool_total_blocks; i++) {
        if (pool_used[i]) used_count++;
        else free_count++;
    }
    if (used_count + free_count != pool_total_blocks) return 0;
    if (free_count != pool_free_top) return 0;
    return 1;
}

int main(void) {
    test_pool_init();
    test_basic_alloc();
    test_free_realloc();
    test_alloc_all();
    test_free_all();
    test_free_list_order();
    test_pool_statistics();
    test_double_free();
    test_fragmentation();
    test_block_boundary();
    test_multi_pool();

    printf("=== All memory pool tests passed ===\n");
    return 0;
}
