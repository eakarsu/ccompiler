int printf(const char *fmt, ...);

// Static memory pool
int pool[256];
int pool_offset;

int pool_alloc(int count) {
    int start = pool_offset;
    pool_offset = pool_offset + count;
    return start;
}

void pool_reset(void) {
    pool_offset = 0;
}

int main(void) {
    pool_offset = 0;

    // Initialize pool to zero
    int i = 0;
    while (i < 256) {
        pool[i] = 0;
        i = i + 1;
    }

    // Allocate a block of 5 ints
    int block1 = pool_alloc(5);
    // EXPECT: block1 start: 0
    printf("block1 start: %d\n", block1);

    // Write to block1
    pool[block1 + 0] = 10;
    pool[block1 + 1] = 20;
    pool[block1 + 2] = 30;
    pool[block1 + 3] = 40;
    pool[block1 + 4] = 50;

    // Allocate another block of 3
    int block2 = pool_alloc(3);
    // EXPECT: block2 start: 5
    printf("block2 start: %d\n", block2);

    // Write to block2
    pool[block2 + 0] = 100;
    pool[block2 + 1] = 200;
    pool[block2 + 2] = 300;

    // Read back block1
    // EXPECT: block1[0]: 10
    printf("block1[0]: %d\n", pool[block1 + 0]);
    // EXPECT: block1[4]: 50
    printf("block1[4]: %d\n", pool[block1 + 4]);

    // Read back block2
    // EXPECT: block2[0]: 100
    printf("block2[0]: %d\n", pool[block2 + 0]);
    // EXPECT: block2[2]: 300
    printf("block2[2]: %d\n", pool[block2 + 2]);

    // Allocate more blocks
    int block3 = pool_alloc(4);
    // EXPECT: block3 start: 8
    printf("block3 start: %d\n", block3);

    pool[block3 + 0] = 1000;
    pool[block3 + 1] = 2000;
    pool[block3 + 2] = 3000;
    pool[block3 + 3] = 4000;

    // Sum all elements in block3
    int sum3 = 0;
    i = 0;
    while (i < 4) {
        sum3 = sum3 + pool[block3 + i];
        i = i + 1;
    }
    // EXPECT: block3 sum: 10000
    printf("block3 sum: %d\n", sum3);

    // Current pool usage
    // EXPECT: pool used: 12
    printf("pool used: %d\n", pool_offset);

    // Reset the pool
    pool_reset();
    // EXPECT: after reset: 0
    printf("after reset: %d\n", pool_offset);

    // Re-allocate after reset
    int rblock1 = pool_alloc(10);
    // EXPECT: realloc start: 0
    printf("realloc start: %d\n", rblock1);

    // Fill with pattern
    i = 0;
    while (i < 10) {
        pool[rblock1 + i] = i * i;
        i = i + 1;
    }
    // EXPECT: pattern[3]: 9
    printf("pattern[3]: %d\n", pool[rblock1 + 3]);
    // EXPECT: pattern[9]: 81
    printf("pattern[9]: %d\n", pool[rblock1 + 9]);

    // Aligned allocation: round up to multiple of 4
    pool_reset();
    int a1 = pool_alloc(3);
    // Align to 4 boundary
    int padding = 4 - (pool_offset - (pool_offset / 4) * 4);
    if (padding < 4) {
        pool_alloc(padding);
    }
    int a2 = pool_alloc(5);
    // EXPECT: aligned a1: 0
    printf("aligned a1: %d\n", a1);
    // EXPECT: aligned a2: 4
    printf("aligned a2: %d\n", a2);

    // Use pool as a stack-like structure
    pool_reset();
    int stack_base = pool_alloc(16);
    int sp = 0;

    // Push values
    pool[stack_base + sp] = 11;
    sp = sp + 1;
    pool[stack_base + sp] = 22;
    sp = sp + 1;
    pool[stack_base + sp] = 33;
    sp = sp + 1;

    // EXPECT: stack top: 33
    printf("stack top: %d\n", pool[stack_base + sp - 1]);

    // Pop
    sp = sp - 1;
    int popped = pool[stack_base + sp];
    // EXPECT: popped: 33
    printf("popped: %d\n", popped);

    // EXPECT: new top: 22
    printf("new top: %d\n", pool[stack_base + sp - 1]);

    // EXPECT: stack size: 2
    printf("stack size: %d\n", sp);

    return 0;
}
