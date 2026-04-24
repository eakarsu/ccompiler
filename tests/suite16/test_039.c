int printf(const char *fmt, ...);

/* Memory Compaction Allocator Simulation
 * Objects are allocated in a heap with handles (indirection table).
 * Handles allow objects to be moved during compaction.
 * Each object: [size, data0, data1, ...]
 * Handle table maps handle -> current heap offset.
 */

int heap[200];
int heap_top;

/* Handle table: maps handle_id -> heap offset, -1 = free handle */
int handles[20];
int handle_sizes[20]; /* size of each allocation */
int handle_alive[20];
int num_handles;

void compact_init(void) {
    heap_top = 0;
    num_handles = 0;
    int i;
    for (i = 0; i < 200; i++) heap[i] = 0;
    for (i = 0; i < 20; i++) {
        handles[i] = -1;
        handle_sizes[i] = 0;
        handle_alive[i] = 0;
    }
}

int compact_alloc(int size) {
    if (heap_top + size > 200) return -1;
    /* Find free handle */
    int h;
    for (h = 0; h < 20; h++) {
        if (handle_alive[h] == 0) {
            handles[h] = heap_top;
            handle_sizes[h] = size;
            handle_alive[h] = 1;
            heap_top = heap_top + size;
            num_handles = num_handles + 1;
            return h;
        }
    }
    return -1;
}

void compact_free(int handle) {
    if (handle < 0 || handle >= 20) return;
    if (handle_alive[handle] == 0) return;
    handle_alive[handle] = 0;
    num_handles = num_handles - 1;
    /* Don't reclaim memory until compaction */
}

void compact_write(int handle, int offset, int val) {
    if (handle < 0 || handle >= 20) return;
    if (handle_alive[handle] == 0) return;
    if (offset < 0 || offset >= handle_sizes[handle]) return;
    heap[handles[handle] + offset] = val;
}

int compact_read(int handle, int offset) {
    if (handle < 0 || handle >= 20) return -1;
    if (handle_alive[handle] == 0) return -1;
    if (offset < 0 || offset >= handle_sizes[handle]) return -1;
    return heap[handles[handle] + offset];
}

/* Compact the heap: move all live objects to the front */
int compact_gc(void) {
    int new_top = 0;
    int moved = 0;
    int h;

    /* Process handles in order of their current heap position */
    /* Simple approach: iterate through handles sorted by position */
    int order[20];
    int order_count = 0;

    for (h = 0; h < 20; h++) {
        if (handle_alive[h]) {
            order[order_count] = h;
            order_count = order_count + 1;
        }
    }

    /* Sort by heap position (simple insertion sort) */
    int i;
    int j;
    for (i = 1; i < order_count; i++) {
        int key = order[i];
        j = i - 1;
        while (j >= 0 && handles[order[j]] > handles[key]) {
            order[j + 1] = order[j];
            j = j - 1;
        }
        order[j + 1] = key;
    }

    /* Move objects */
    for (i = 0; i < order_count; i++) {
        h = order[i];
        int old_pos = handles[h];
        int size = handle_sizes[h];
        if (old_pos != new_top) {
            /* Move data */
            int k;
            for (k = 0; k < size; k++) {
                heap[new_top + k] = heap[old_pos + k];
            }
            handles[h] = new_top;
            moved = moved + 1;
        }
        new_top = new_top + size;
    }

    heap_top = new_top;
    return moved;
}

int live_count(void) {
    int c = 0;
    int i;
    for (i = 0; i < 20; i++) {
        if (handle_alive[i]) c = c + 1;
    }
    return c;
}

void test_basic(void) {
    printf("=== Compaction Basic ===\n"); // EXPECT: === Compaction Basic ===
    compact_init();

    int a = compact_alloc(4);
    int b = compact_alloc(4);
    int c = compact_alloc(4);
    printf("alloc: %d %d %d top=%d\n", a, b, c, heap_top); // EXPECT: alloc: 0 1 2 top=12

    compact_write(a, 0, 10);
    compact_write(b, 0, 20);
    compact_write(c, 0, 30);

    printf("data: %d %d %d\n", compact_read(a, 0), compact_read(b, 0), compact_read(c, 0)); // EXPECT: data: 10 20 30
}

void test_compact(void) {
    printf("=== Compaction After Free ===\n"); // EXPECT: === Compaction After Free ===
    compact_init();

    int a = compact_alloc(4);
    int b = compact_alloc(4);
    int c = compact_alloc(4);

    compact_write(a, 0, 100);
    compact_write(a, 1, 101);
    compact_write(b, 0, 200);
    compact_write(c, 0, 300);
    compact_write(c, 1, 301);

    printf("before free: top=%d\n", heap_top); // EXPECT: before free: top=12

    compact_free(b); /* free middle block */
    printf("after free b: live=%d top=%d\n", live_count(), heap_top); // EXPECT: after free b: live=2 top=12

    int moved = compact_gc();
    printf("after compact: moved=%d top=%d\n", moved, heap_top); // EXPECT: after compact: moved=1 top=8

    /* Data should be preserved */
    printf("a data: %d %d\n", compact_read(a, 0), compact_read(a, 1)); // EXPECT: a data: 100 101
    printf("c data: %d %d\n", compact_read(c, 0), compact_read(c, 1)); // EXPECT: c data: 300 301
}

void test_heavy_fragmentation(void) {
    printf("=== Heavy Fragmentation ===\n"); // EXPECT: === Heavy Fragmentation ===
    compact_init();

    int objs[10];
    int i;
    for (i = 0; i < 10; i++) {
        objs[i] = compact_alloc(5);
        compact_write(objs[i], 0, (i + 1) * 10);
    }
    printf("allocated 10 objects: top=%d\n", heap_top); // EXPECT: allocated 10 objects: top=50

    /* Free every other object */
    for (i = 0; i < 10; i = i + 2) {
        compact_free(objs[i]);
    }
    printf("after freeing 5: live=%d top=%d\n", live_count(), heap_top); // EXPECT: after freeing 5: live=5 top=50

    int moved = compact_gc();
    printf("after compact: moved=%d top=%d\n", moved, heap_top); // EXPECT: after compact: moved=5 top=25

    /* Verify surviving data */
    int sum = 0;
    for (i = 1; i < 10; i = i + 2) {
        sum = sum + compact_read(objs[i], 0);
    }
    printf("sum of surviving data: %d\n", sum); // EXPECT: sum of surviving data: 300
}

void test_reuse_after_compact(void) {
    printf("=== Reuse After Compact ===\n"); // EXPECT: === Reuse After Compact ===
    compact_init();

    int a = compact_alloc(10);
    int b = compact_alloc(10);
    compact_write(a, 0, 42);
    compact_write(b, 0, 99);
    printf("initial top=%d\n", heap_top); // EXPECT: initial top=20

    compact_free(a);
    compact_gc();
    printf("after free a + compact: top=%d\n", heap_top); // EXPECT: after free a + compact: top=10

    /* Now allocate more */
    int c = compact_alloc(8);
    compact_write(c, 0, 77);
    printf("new alloc c at handle %d: top=%d\n", c, heap_top); // EXPECT: new alloc c at handle 0: top=18

    printf("b data=%d c data=%d\n", compact_read(b, 0), compact_read(c, 0)); // EXPECT: b data=99 c data=77
}

void test_compact_nothing(void) {
    printf("=== Compact With Nothing To Move ===\n"); // EXPECT: === Compact With Nothing To Move ===
    compact_init();

    int a = compact_alloc(5);
    int b = compact_alloc(5);
    compact_write(a, 0, 1);
    compact_write(b, 0, 2);

    int moved = compact_gc();
    printf("moved=%d top=%d\n", moved, heap_top); // EXPECT: moved=0 top=10
    printf("data: %d %d\n", compact_read(a, 0), compact_read(b, 0)); // EXPECT: data: 1 2
}

int main(void) {
    test_basic();
    test_compact();
    test_heavy_fragmentation();
    test_reuse_after_compact();
    test_compact_nothing();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
