int printf(const char *fmt, ...);

/* Mark-Sweep Garbage Collector Simulation
 * Objects are in an array. Each object has:
 *   slot 0: mark bit (0/1)
 *   slot 1: ref count of pointers to other objects (max 3)
 *   slot 2..4: indices of referenced objects (-1 = null)
 *   slot 5: data value
 * Object size = 6 slots. Max 16 objects.
 */

int heap[96]; /* 16 objects * 6 slots */
int allocated[16]; /* 1 if object slot is in use, 0 if free */
int roots[4]; /* root set: indices of root objects (-1 = unused) */
int num_roots;

void gc_init(void) {
    int i;
    for (i = 0; i < 96; i++) heap[i] = 0;
    for (i = 0; i < 16; i++) allocated[i] = 0;
    for (i = 0; i < 4; i++) roots[i] = -1;
    num_roots = 0;
}

int gc_alloc(int data_val) {
    int i;
    for (i = 0; i < 16; i++) {
        if (allocated[i] == 0) {
            allocated[i] = 1;
            int base = i * 6;
            heap[base] = 0;     /* mark bit */
            heap[base + 1] = 0; /* ref count */
            heap[base + 2] = -1;
            heap[base + 3] = -1;
            heap[base + 4] = -1;
            heap[base + 5] = data_val;
            return i;
        }
    }
    return -1;
}

void gc_add_ref(int from_obj, int to_obj) {
    if (from_obj < 0 || from_obj >= 16) return;
    if (to_obj < 0 || to_obj >= 16) return;
    int base = from_obj * 6;
    int refs = heap[base + 1];
    if (refs < 3) {
        heap[base + 2 + refs] = to_obj;
        heap[base + 1] = refs + 1;
    }
}

void gc_add_root(int obj) {
    if (num_roots < 4 && obj >= 0) {
        roots[num_roots] = obj;
        num_roots = num_roots + 1;
    }
}

int gc_get_data(int obj) {
    if (obj < 0 || obj >= 16) return -1;
    return heap[obj * 6 + 5];
}

void gc_set_data(int obj, int val) {
    if (obj < 0 || obj >= 16) return;
    heap[obj * 6 + 5] = val;
}

void gc_mark(int obj) {
    if (obj < 0 || obj >= 16) return;
    if (allocated[obj] == 0) return;
    int base = obj * 6;
    if (heap[base] == 1) return; /* already marked */
    heap[base] = 1;

    int refs = heap[base + 1];
    int i;
    for (i = 0; i < refs; i++) {
        int child = heap[base + 2 + i];
        if (child >= 0) {
            gc_mark(child);
        }
    }
}

int gc_sweep(void) {
    int freed = 0;
    int i;
    for (i = 0; i < 16; i++) {
        if (allocated[i] == 1) {
            int base = i * 6;
            if (heap[base] == 0) {
                /* Not marked - garbage */
                allocated[i] = 0;
                freed = freed + 1;
            } else {
                /* Clear mark for next cycle */
                heap[base] = 0;
            }
        }
    }
    return freed;
}

int gc_collect(void) {
    /* Mark phase */
    int i;
    for (i = 0; i < num_roots; i++) {
        if (roots[i] >= 0) {
            gc_mark(roots[i]);
        }
    }
    /* Sweep phase */
    return gc_sweep();
}

int count_alive(void) {
    int c = 0;
    int i;
    for (i = 0; i < 16; i++) {
        if (allocated[i]) c = c + 1;
    }
    return c;
}

void test_basic_gc(void) {
    printf("=== Basic GC ===\n"); // EXPECT: === Basic GC ===
    gc_init();

    int a = gc_alloc(10);
    int b = gc_alloc(20);
    int c = gc_alloc(30);
    printf("alloc: %d %d %d alive=%d\n", a, b, c, count_alive()); // EXPECT: alloc: 0 1 2 alive=3

    gc_add_root(a);
    gc_add_ref(a, b); /* a -> b */

    int freed = gc_collect();
    printf("freed=%d alive=%d\n", freed, count_alive()); // EXPECT: freed=1 alive=2
    printf("a data=%d b data=%d\n", gc_get_data(a), gc_get_data(b)); // EXPECT: a data=10 b data=20
}

void test_chain(void) {
    printf("=== Chain References ===\n"); // EXPECT: === Chain References ===
    gc_init();

    int a = gc_alloc(1);
    int b = gc_alloc(2);
    int c = gc_alloc(3);
    int d = gc_alloc(4);

    gc_add_root(a);
    gc_add_ref(a, b);
    gc_add_ref(b, c);
    /* d is unreachable */

    int freed = gc_collect();
    printf("freed=%d alive=%d\n", freed, count_alive()); // EXPECT: freed=1 alive=3
    printf("data: %d %d %d\n", gc_get_data(a), gc_get_data(b), gc_get_data(c)); // EXPECT: data: 1 2 3
}

void test_cycle(void) {
    printf("=== Cycle Detection ===\n"); // EXPECT: === Cycle Detection ===
    gc_init();

    int a = gc_alloc(100);
    int b = gc_alloc(200);
    int c = gc_alloc(300);

    gc_add_ref(a, b);
    gc_add_ref(b, c);
    gc_add_ref(c, a); /* cycle */

    /* No roots - everything should be collected */
    int freed = gc_collect();
    printf("no roots: freed=%d alive=%d\n", freed, count_alive()); // EXPECT: no roots: freed=3 alive=0

    /* Now with a root */
    gc_init();
    a = gc_alloc(100);
    b = gc_alloc(200);
    c = gc_alloc(300);
    gc_add_ref(a, b);
    gc_add_ref(b, c);
    gc_add_ref(c, a);
    gc_add_root(a);

    freed = gc_collect();
    printf("with root: freed=%d alive=%d\n", freed, count_alive()); // EXPECT: with root: freed=0 alive=3
}

void test_multi_collect(void) {
    printf("=== Multiple Collections ===\n"); // EXPECT: === Multiple Collections ===
    gc_init();

    int a = gc_alloc(1);
    gc_add_root(a);

    int i;
    int total_freed = 0;
    for (i = 0; i < 5; i++) {
        gc_alloc(i * 10 + 100);
        gc_alloc(i * 10 + 200);
        int freed = gc_collect();
        total_freed = total_freed + freed;
    }
    printf("total freed=%d alive=%d\n", total_freed, count_alive()); // EXPECT: total freed=10 alive=1
    printf("root data=%d\n", gc_get_data(a)); // EXPECT: root data=1
}

void test_tree_structure(void) {
    printf("=== Tree Structure ===\n"); // EXPECT: === Tree Structure ===
    gc_init();

    /*       r
     *      / \
     *     a   b
     *    / \
     *   c   d
     */
    int r = gc_alloc(0);
    int a = gc_alloc(1);
    int b = gc_alloc(2);
    int c = gc_alloc(3);
    int d = gc_alloc(4);
    int orphan = gc_alloc(99);

    gc_add_root(r);
    gc_add_ref(r, a);
    gc_add_ref(r, b);
    gc_add_ref(a, c);
    gc_add_ref(a, d);

    int freed = gc_collect();
    printf("freed=%d alive=%d\n", freed, count_alive()); // EXPECT: freed=1 alive=5

    int sum = gc_get_data(r) + gc_get_data(a) + gc_get_data(b) + gc_get_data(c) + gc_get_data(d);
    printf("sum of live data=%d\n", sum); // EXPECT: sum of live data=10
}

int main(void) {
    test_basic_gc();
    test_chain();
    test_cycle();
    test_multi_collect();
    test_tree_structure();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
