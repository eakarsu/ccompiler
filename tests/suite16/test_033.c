int printf(const char *fmt, ...);

/* Free-List Allocator Simulation
 * Memory is an int array. Free blocks form a linked list.
 * Each block has a header: [size, next_free_index].
 * Allocated blocks have header: [size, -2] where -2 means "in use".
 * First-fit allocation strategy.
 */

int mem[256];
int free_list_head;
int total_allocs;
int total_frees;

void fl_init(int size) {
    /* Single free block spanning the whole memory */
    mem[0] = size;    /* size of this free block (including header) */
    mem[1] = -1;      /* next free = -1 (end of list) */
    free_list_head = 0;
    total_allocs = 0;
    total_frees = 0;
    int i;
    for (i = 2; i < size; i++) {
        mem[i] = 0;
    }
}

/* Returns address of usable memory (after header), or -1 */
int fl_alloc(int request_size) {
    int alloc_size = request_size + 2; /* include header */
    int prev = -1;
    int cur = free_list_head;

    while (cur != -1) {
        int block_size = mem[cur];
        if (block_size >= alloc_size) {
            /* Can we split? Need at least 3 slots for remainder */
            if (block_size >= alloc_size + 3) {
                /* Split: create new free block after allocation */
                int new_free = cur + alloc_size;
                mem[new_free] = block_size - alloc_size;
                mem[new_free + 1] = mem[cur + 1]; /* inherit next pointer */

                /* Update previous pointer or head */
                if (prev == -1) {
                    free_list_head = new_free;
                } else {
                    mem[prev + 1] = new_free;
                }

                mem[cur] = alloc_size;
                mem[cur + 1] = -2; /* mark as allocated */
            } else {
                /* Use entire block */
                if (prev == -1) {
                    free_list_head = mem[cur + 1];
                } else {
                    mem[prev + 1] = mem[cur + 1];
                }
                mem[cur + 1] = -2; /* mark as allocated */
            }
            total_allocs = total_allocs + 1;
            return cur + 2; /* return pointer past header */
        }
        prev = cur;
        cur = mem[cur + 1];
    }
    return -1; /* no space */
}

void fl_free(int addr) {
    if (addr < 2) return;
    int block_start = addr - 2;
    if (mem[block_start + 1] != -2) return; /* not allocated */

    /* Add to front of free list */
    mem[block_start + 1] = free_list_head;
    free_list_head = block_start;
    total_frees = total_frees + 1;
}

void fl_write(int addr, int offset, int val) {
    if (addr + offset >= 0 && addr + offset < 256) {
        mem[addr + offset] = val;
    }
}

int fl_read(int addr, int offset) {
    if (addr + offset >= 0 && addr + offset < 256) {
        return mem[addr + offset];
    }
    return -1;
}

int count_free_blocks(void) {
    int count = 0;
    int cur = free_list_head;
    while (cur != -1) {
        count = count + 1;
        cur = mem[cur + 1];
    }
    return count;
}

int total_free_space(void) {
    int total = 0;
    int cur = free_list_head;
    while (cur != -1) {
        total = total + mem[cur] - 2; /* usable space */
        cur = mem[cur + 1];
    }
    return total;
}

void test_basic(void) {
    printf("=== Free-List Basic ===\n"); // EXPECT: === Free-List Basic ===
    fl_init(64);

    int a = fl_alloc(4);
    printf("alloc 4: addr=%d\n", a); // EXPECT: alloc 4: addr=2
    fl_write(a, 0, 42);
    printf("read: %d\n", fl_read(a, 0)); // EXPECT: read: 42

    int b = fl_alloc(8);
    printf("alloc 8: addr=%d\n", b); // EXPECT: alloc 8: addr=8

    int c = fl_alloc(3);
    printf("alloc 3: addr=%d\n", c); // EXPECT: alloc 3: addr=18

    printf("free blocks=%d free space=%d\n", count_free_blocks(), total_free_space()); // EXPECT: free blocks=1 free space=41
}

void test_free_and_realloc(void) {
    printf("=== Free and Realloc ===\n"); // EXPECT: === Free and Realloc ===
    fl_init(32);

    int a = fl_alloc(4);
    int b = fl_alloc(4);
    int c = fl_alloc(4);
    printf("alloc: a=%d b=%d c=%d\n", a, b, c); // EXPECT: alloc: a=2 b=8 c=14

    fl_free(b);
    printf("after free b: free blocks=%d\n", count_free_blocks()); // EXPECT: after free b: free blocks=2

    /* Allocate something that fits in b's freed space */
    int d = fl_alloc(4);
    printf("realloc 4: addr=%d\n", d); // EXPECT: realloc 4: addr=8

    printf("free blocks=%d\n", count_free_blocks()); // EXPECT: free blocks=1
}

void test_fragmentation(void) {
    printf("=== Fragmentation ===\n"); // EXPECT: === Fragmentation ===
    fl_init(48);

    int a = fl_alloc(4);
    int b = fl_alloc(4);
    int c = fl_alloc(4);
    int d = fl_alloc(4);

    printf("alloc 4 blocks: %d %d %d %d\n", a, b, c, d); // EXPECT: alloc 4 blocks: 2 8 14 20

    /* Free alternating blocks */
    fl_free(a);
    fl_free(c);

    printf("after freeing a,c: free blocks=%d\n", count_free_blocks()); // EXPECT: after freeing a,c: free blocks=3

    /* Try to allocate something bigger than any single free block */
    int e = fl_alloc(10);
    printf("alloc 10 (fits in remainder): addr=%d\n", e); // EXPECT: alloc 10 (fits in remainder): addr=26

    printf("allocs=%d frees=%d\n", total_allocs, total_frees); // EXPECT: allocs=5 frees=2
}

void test_exhaust(void) {
    printf("=== Exhaust Memory ===\n"); // EXPECT: === Exhaust Memory ===
    fl_init(20);

    int count = 0;
    int i;
    for (i = 0; i < 10; i++) {
        int addr = fl_alloc(3);
        if (addr >= 0) {
            fl_write(addr, 0, i * 10);
            count = count + 1;
        }
    }
    printf("allocated: %d blocks\n", count); // EXPECT: allocated: 4 blocks

    int fail = fl_alloc(1);
    printf("alloc when full: %d\n", fail); // EXPECT: alloc when full: -1
}

int main(void) {
    test_basic();
    test_free_and_realloc();
    test_fragmentation();
    test_exhaust();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
