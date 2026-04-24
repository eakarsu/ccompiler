int printf(const char *fmt, ...);

/* Segregated Free-List Allocator Simulation
 * Multiple size classes, each with its own free list.
 * Size classes: 0=2 slots, 1=4 slots, 2=8 slots, 3=16 slots
 * Each class has a region of memory and a free list.
 * Supports coalescing within a class on free.
 */

int mem[512];

/* Per-class state */
int class_base[4];     /* base offset in mem[] */
int class_slot_size[4]; /* slot size for this class */
int class_count[4];    /* number of slots in this class */
int class_free[4];     /* head of free list (-1 = empty) */
int class_alloc_count[4]; /* number of allocated blocks */

/* Block header (1 slot): next_free index within class, or -2 if allocated */

void seg_init(void) {
    int i;
    for (i = 0; i < 512; i++) mem[i] = 0;

    /* Class 0: 2-slot blocks, 32 blocks starting at offset 0 */
    class_base[0] = 0;
    class_slot_size[0] = 2;
    class_count[0] = 32;
    class_alloc_count[0] = 0;

    /* Class 1: 4-slot blocks, 16 blocks starting at offset 64 */
    class_base[1] = 64;
    class_slot_size[1] = 4;
    class_count[1] = 16;
    class_alloc_count[1] = 0;

    /* Class 2: 8-slot blocks, 8 blocks starting at offset 128 */
    class_base[2] = 128;
    class_slot_size[2] = 8;
    class_count[2] = 8;
    class_alloc_count[2] = 0;

    /* Class 3: 16-slot blocks, 4 blocks starting at offset 192 */
    class_base[3] = 192;
    class_slot_size[3] = 16;
    class_count[3] = 4;
    class_alloc_count[3] = 0;

    /* Initialize free lists for each class */
    int c;
    for (c = 0; c < 4; c++) {
        int base = class_base[c];
        int sz = class_slot_size[c];
        int cnt = class_count[c];
        int j;
        for (j = 0; j < cnt; j++) {
            int off = base + j * sz;
            if (j < cnt - 1) {
                mem[off] = j + 1; /* next free block index */
            } else {
                mem[off] = -1;    /* end of free list */
            }
        }
        class_free[c] = 0;
    }
}

/* Find the right size class for a request */
int find_class(int size) {
    if (size <= 1) return 0;  /* 2-slot (1 usable) */
    if (size <= 3) return 1;  /* 4-slot (3 usable) */
    if (size <= 7) return 2;  /* 8-slot (7 usable) */
    if (size <= 15) return 3; /* 16-slot (15 usable) */
    return -1; /* too large */
}

/* Returns absolute offset of usable data, or -1 */
int seg_alloc(int size) {
    int c = find_class(size);
    if (c < 0) return -1;

    int head = class_free[c];
    if (head == -1) return -1;

    int base = class_base[c];
    int sz = class_slot_size[c];
    int off = base + head * sz;

    class_free[c] = mem[off]; /* follow free list */
    mem[off] = -2; /* mark as allocated */
    class_alloc_count[c] = class_alloc_count[c] + 1;

    return off + 1; /* skip header */
}

void seg_free(int addr) {
    if (addr <= 0) return;
    int header_addr = addr - 1;

    /* Find which class this belongs to */
    int c;
    for (c = 0; c < 4; c++) {
        int base = class_base[c];
        int sz = class_slot_size[c];
        int end = base + class_count[c] * sz;
        if (header_addr >= base && header_addr < end) {
            int block_idx = (header_addr - base) / sz;
            mem[header_addr] = class_free[c];
            class_free[c] = block_idx;
            class_alloc_count[c] = class_alloc_count[c] - 1;
            return;
        }
    }
}

void seg_write(int addr, int offset, int val) {
    int idx = addr + offset;
    if (idx >= 0 && idx < 512) mem[idx] = val;
}

int seg_read(int addr, int offset) {
    int idx = addr + offset;
    if (idx >= 0 && idx < 512) return mem[idx];
    return -1;
}

int seg_free_count(int c) {
    int count = 0;
    int cur = class_free[c];
    while (cur != -1) {
        count = count + 1;
        int off = class_base[c] + cur * class_slot_size[c];
        cur = mem[off];
    }
    return count;
}

void test_basic(void) {
    printf("=== Segregated Basic ===\n"); // EXPECT: === Segregated Basic ===
    seg_init();

    int a = seg_alloc(1); /* class 0 */
    int b = seg_alloc(3); /* class 1 */
    int c = seg_alloc(7); /* class 2 */
    int d = seg_alloc(15); /* class 3 */

    printf("a=%d b=%d c=%d d=%d\n", a, b, c, d); // EXPECT: a=1 b=65 c=129 d=193

    seg_write(a, 0, 10);
    seg_write(b, 0, 20);
    seg_write(c, 0, 30);
    seg_write(d, 0, 40);

    printf("data: %d %d %d %d\n", seg_read(a, 0), seg_read(b, 0), seg_read(c, 0), seg_read(d, 0)); // EXPECT: data: 10 20 30 40
}

void test_multi_alloc_per_class(void) {
    printf("=== Multiple Per Class ===\n"); // EXPECT: === Multiple Per Class ===
    seg_init();

    /* Allocate multiple from class 0 */
    int a1 = seg_alloc(1);
    int a2 = seg_alloc(1);
    int a3 = seg_alloc(1);
    printf("class 0: %d %d %d\n", a1, a2, a3); // EXPECT: class 0: 1 3 5

    /* Allocate multiple from class 1 */
    int b1 = seg_alloc(2);
    int b2 = seg_alloc(3);
    printf("class 1: %d %d\n", b1, b2); // EXPECT: class 1: 65 69

    printf("class 0 free: %d\n", seg_free_count(0)); // EXPECT: class 0 free: 29
    printf("class 1 free: %d\n", seg_free_count(1)); // EXPECT: class 1 free: 14
}

void test_free_reuse(void) {
    printf("=== Free and Reuse ===\n"); // EXPECT: === Free and Reuse ===
    seg_init();

    int a = seg_alloc(1);
    int b = seg_alloc(1);
    int c = seg_alloc(1);

    seg_write(a, 0, 111);
    seg_write(b, 0, 222);
    seg_write(c, 0, 333);

    seg_free(b);
    printf("after free b: class 0 alloc=%d\n", class_alloc_count[0]); // EXPECT: after free b: class 0 alloc=2

    int d = seg_alloc(1);
    printf("reuse: d=%d\n", d); // EXPECT: reuse: d=3
    seg_write(d, 0, 444);

    printf("data: a=%d d=%d c=%d\n", seg_read(a, 0), seg_read(d, 0), seg_read(c, 0)); // EXPECT: data: a=111 d=444 c=333
}

void test_exhaust_class(void) {
    printf("=== Exhaust Class ===\n"); // EXPECT: === Exhaust Class ===
    seg_init();

    /* Exhaust class 3 (16-slot, 4 blocks) */
    int count = 0;
    int i;
    for (i = 0; i < 10; i++) {
        int addr = seg_alloc(15);
        if (addr >= 0) {
            count = count + 1;
            seg_write(addr, 0, i);
        }
    }
    printf("class 3 allocated: %d\n", count); // EXPECT: class 3 allocated: 4
    printf("class 3 free: %d\n", seg_free_count(3)); // EXPECT: class 3 free: 0
}

void test_mixed_sizes(void) {
    printf("=== Mixed Sizes ===\n"); // EXPECT: === Mixed Sizes ===
    seg_init();

    int addrs[12];
    int sizes[12];
    sizes[0] = 1; sizes[1] = 3; sizes[2] = 7; sizes[3] = 1;
    sizes[4] = 15; sizes[5] = 3; sizes[6] = 1; sizes[7] = 7;
    sizes[8] = 1; sizes[9] = 3; sizes[10] = 15; sizes[11] = 7;

    int i;
    int total = 0;
    for (i = 0; i < 12; i++) {
        addrs[i] = seg_alloc(sizes[i]);
        if (addrs[i] >= 0) {
            seg_write(addrs[i], 0, sizes[i] * 10);
            total = total + 1;
        }
    }
    printf("allocated: %d\n", total); // EXPECT: allocated: 12

    /* Free half */
    for (i = 0; i < 12; i = i + 2) {
        seg_free(addrs[i]);
    }

    int sum = 0;
    for (i = 1; i < 12; i = i + 2) {
        sum = sum + seg_read(addrs[i], 0);
    }
    printf("sum of surviving: %d\n", sum); // EXPECT: sum of surviving: 240

    printf("class allocs: %d %d %d %d\n",
           class_alloc_count[0], class_alloc_count[1],
           class_alloc_count[2], class_alloc_count[3]); // EXPECT: class allocs: 1 3 2 0
}

void test_stats(void) {
    printf("=== Allocator Stats ===\n"); // EXPECT: === Allocator Stats ===
    seg_init();

    int total_capacity = 0;
    int i;
    for (i = 0; i < 4; i++) {
        int usable = class_slot_size[i] - 1;
        total_capacity = total_capacity + class_count[i] * usable;
    }
    printf("total usable capacity: %d\n", total_capacity); // EXPECT: total usable capacity: 196

    printf("class sizes: %d %d %d %d\n",
           class_slot_size[0], class_slot_size[1],
           class_slot_size[2], class_slot_size[3]); // EXPECT: class sizes: 2 4 8 16

    printf("class counts: %d %d %d %d\n",
           class_count[0], class_count[1],
           class_count[2], class_count[3]); // EXPECT: class counts: 32 16 8 4
}

int main(void) {
    test_basic();
    test_multi_alloc_per_class();
    test_free_reuse();
    test_exhaust_class();
    test_mixed_sizes();
    test_stats();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
