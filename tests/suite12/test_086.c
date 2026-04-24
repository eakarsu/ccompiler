int printf(const char *fmt, ...);

// Compacting Garbage Collector Simulation
// After mark-and-sweep, compact live objects to eliminate fragmentation

int obj_value[24];
int obj_alive[24];
int obj_marked[24];
int obj_size[24];       // size of each object in "units"
int obj_addr[24];       // simulated memory address (offset)
int obj_ref[24];        // single reference to another object (-1 = none)
int num_objects;

int roots[4];
int num_roots;

int heap_size;          // total heap size in units
int gc_compactions;

void compact_init() {
    int i;
    for (i = 0; i < 24; i++) {
        obj_value[i] = 0;
        obj_alive[i] = 0;
        obj_marked[i] = 0;
        obj_size[i] = 0;
        obj_addr[i] = 0;
        obj_ref[i] = -1;
    }
    for (i = 0; i < 4; i++) {
        roots[i] = -1;
    }
    num_objects = 0;
    num_roots = 0;
    heap_size = 256;
    gc_compactions = 0;
}

int next_addr;

int compact_alloc(int value, int size) {
    if (num_objects >= 24) return -1;
    int id;
    id = num_objects;
    obj_value[id] = value;
    obj_alive[id] = 1;
    obj_size[id] = size;
    obj_addr[id] = next_addr;
    obj_ref[id] = -1;
    next_addr = next_addr + size;
    num_objects = num_objects + 1;
    return id;
}

void compact_add_root(int id) {
    if (num_roots < 4) {
        roots[num_roots] = id;
        num_roots = num_roots + 1;
    }
}

void compact_mark(int id) {
    if (id < 0 || id >= 24) return;
    if (obj_alive[id] == 0) return;
    if (obj_marked[id] == 1) return;
    obj_marked[id] = 1;
    compact_mark(obj_ref[id]);
}

int compact_sweep() {
    int i;
    int swept;
    swept = 0;
    for (i = 0; i < num_objects; i++) {
        if (obj_alive[i] == 1 && obj_marked[i] == 0) {
            obj_alive[i] = 0;
            swept = swept + 1;
        }
        obj_marked[i] = 0;
    }
    return swept;
}

int compute_fragmentation() {
    // Count number of gaps between live objects
    int i;
    int gaps;
    int last_was_dead;
    int found_live;
    gaps = 0;
    last_was_dead = 0;
    found_live = 0;

    for (i = 0; i < num_objects; i++) {
        if (obj_alive[i] == 1) {
            if (last_was_dead == 1 && found_live == 1) {
                gaps = gaps + 1;
            }
            found_live = 1;
            last_was_dead = 0;
        } else {
            last_was_dead = 1;
        }
    }
    return gaps;
}

int compute_wasted_space() {
    int i;
    int wasted;
    wasted = 0;
    for (i = 0; i < num_objects; i++) {
        if (obj_alive[i] == 0) {
            wasted = wasted + obj_size[i];
        }
    }
    return wasted;
}

void compact() {
    // Move all live objects to the beginning of heap
    int new_addr;
    int i;
    int old_addrs[24];

    // Save old addresses
    for (i = 0; i < num_objects; i++) {
        old_addrs[i] = obj_addr[i];
    }

    // Compute new addresses for live objects
    new_addr = 0;
    for (i = 0; i < num_objects; i++) {
        if (obj_alive[i] == 1) {
            obj_addr[i] = new_addr;
            new_addr = new_addr + obj_size[i];
        }
    }
    next_addr = new_addr;
    gc_compactions = gc_compactions + 1;
}

int total_live_space() {
    int i;
    int total;
    total = 0;
    for (i = 0; i < num_objects; i++) {
        if (obj_alive[i] == 1) {
            total = total + obj_size[i];
        }
    }
    return total;
}

int count_alive() {
    int i;
    int c;
    c = 0;
    for (i = 0; i < num_objects; i++) {
        if (obj_alive[i] == 1) c = c + 1;
    }
    return c;
}

int gc_collect_and_compact() {
    int i;
    int swept;

    // Mark from roots
    for (i = 0; i < num_roots; i++) {
        if (roots[i] >= 0) compact_mark(roots[i]);
    }

    // Sweep
    swept = compact_sweep();

    // Compact
    if (swept > 0) {
        compact();
    }
    return swept;
}

int main() {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int swept;

    compact_init();
    next_addr = 0;
    printf("Compacting GC Simulation\n");
    // EXPECT: Compacting GC Simulation

    // Allocate objects with varying sizes
    a = compact_alloc(10, 8);
    b = compact_alloc(20, 16);
    c = compact_alloc(30, 4);
    d = compact_alloc(40, 32);
    e = compact_alloc(50, 8);
    f = compact_alloc(60, 16);
    g = compact_alloc(70, 12);

    printf("Allocated %d objects, total space: %d\n", num_objects, next_addr);
    // EXPECT: Allocated 7 objects, total space: 96

    // Print addresses before GC
    printf("Addrs: %d %d %d %d %d %d %d\n",
           obj_addr[a], obj_addr[b], obj_addr[c], obj_addr[d],
           obj_addr[e], obj_addr[f], obj_addr[g]);
    // EXPECT: Addrs: 0 8 24 28 60 68 84

    // Set refs: a->c, c->e, f->g
    obj_ref[a] = c;
    obj_ref[c] = e;
    obj_ref[f] = g;

    // Roots: a and f
    compact_add_root(a);
    compact_add_root(f);

    // Collect - b, d should die (unreachable)
    swept = gc_collect_and_compact();
    printf("Swept: %d, alive: %d\n", swept, count_alive());
    // EXPECT: Swept: 2, alive: 5

    printf("b alive: %d, d alive: %d\n", obj_alive[b], obj_alive[d]);
    // EXPECT: b alive: 0, d alive: 0

    // After compaction, addresses should be contiguous
    printf("Post-compact addrs: a=%d c=%d e=%d f=%d g=%d\n",
           obj_addr[a], obj_addr[c], obj_addr[e], obj_addr[f], obj_addr[g]);
    // EXPECT: Post-compact addrs: a=0 c=8 e=12 f=20 g=36

    printf("Live space: %d, next_addr: %d\n", total_live_space(), next_addr);
    // EXPECT: Live space: 48, next_addr: 48

    int frags;
    frags = compute_fragmentation();
    printf("Fragmentation gaps: %d\n", frags);
    // EXPECT: Fragmentation gaps: 2

    printf("Wasted space: %d\n", compute_wasted_space());
    // EXPECT: Wasted space: 48

    // Remove root f -> g, f become garbage
    num_roots = 1; // keep only root a
    swept = gc_collect_and_compact();
    printf("Swept: %d, alive: %d\n", swept, count_alive());
    // EXPECT: Swept: 2, alive: 3

    printf("Live space: %d, next_addr: %d\n", total_live_space(), next_addr);
    // EXPECT: Live space: 20, next_addr: 20

    printf("Compactions: %d\n", gc_compactions);
    // EXPECT: Compactions: 2

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
