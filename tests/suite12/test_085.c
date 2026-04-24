int printf(const char *fmt, ...);

// Mark-and-Sweep Garbage Collector Simulation
// Objects form a graph. GC marks reachable objects from roots, sweeps unreachable.

int obj_value[20];
int obj_alive[20];
int obj_marked[20];
// Each object can reference up to 3 other objects
int obj_ref0[20];
int obj_ref1[20];
int obj_ref2[20];
int num_objects;

// Root set: indices of root objects
int roots[5];
int num_roots;

int gc_mark_count;
int gc_sweep_count;
int gc_cycles;

void gc_init() {
    int i;
    for (i = 0; i < 20; i++) {
        obj_value[i] = 0;
        obj_alive[i] = 0;
        obj_marked[i] = 0;
        obj_ref0[i] = -1;
        obj_ref1[i] = -1;
        obj_ref2[i] = -1;
    }
    for (i = 0; i < 5; i++) {
        roots[i] = -1;
    }
    num_objects = 0;
    num_roots = 0;
    gc_mark_count = 0;
    gc_sweep_count = 0;
    gc_cycles = 0;
}

int gc_alloc(int value) {
    if (num_objects >= 20) return -1;
    int id;
    id = num_objects;
    num_objects = num_objects + 1;
    obj_value[id] = value;
    obj_alive[id] = 1;
    obj_ref0[id] = -1;
    obj_ref1[id] = -1;
    obj_ref2[id] = -1;
    return id;
}

void gc_add_root(int id) {
    if (num_roots < 5 && id >= 0) {
        roots[num_roots] = id;
        num_roots = num_roots + 1;
    }
}

void gc_remove_root(int id) {
    int i;
    int j;
    for (i = 0; i < num_roots; i++) {
        if (roots[i] == id) {
            // Shift remaining roots
            for (j = i; j < num_roots - 1; j++) {
                roots[j] = roots[j + 1];
            }
            roots[num_roots - 1] = -1;
            num_roots = num_roots - 1;
            return;
        }
    }
}

void gc_set_ref(int from_id, int slot, int to_id) {
    if (from_id < 0 || from_id >= 20) return;
    if (slot == 0) obj_ref0[from_id] = to_id;
    else if (slot == 1) obj_ref1[from_id] = to_id;
    else if (slot == 2) obj_ref2[from_id] = to_id;
}

void gc_mark(int id) {
    if (id < 0 || id >= 20) return;
    if (obj_alive[id] == 0) return;
    if (obj_marked[id] == 1) return;

    obj_marked[id] = 1;
    gc_mark_count = gc_mark_count + 1;

    // Recursively mark references
    gc_mark(obj_ref0[id]);
    gc_mark(obj_ref1[id]);
    gc_mark(obj_ref2[id]);
}

int gc_sweep() {
    int i;
    int swept;
    swept = 0;
    for (i = 0; i < num_objects; i++) {
        if (obj_alive[i] == 1 && obj_marked[i] == 0) {
            obj_alive[i] = 0;
            obj_ref0[i] = -1;
            obj_ref1[i] = -1;
            obj_ref2[i] = -1;
            swept = swept + 1;
        }
        obj_marked[i] = 0; // clear marks for next cycle
    }
    gc_sweep_count = gc_sweep_count + swept;
    return swept;
}

int gc_collect() {
    int i;
    int swept;

    gc_mark_count = 0;

    // Mark phase: start from all roots
    for (i = 0; i < num_roots; i++) {
        if (roots[i] >= 0) {
            gc_mark(roots[i]);
        }
    }

    // Sweep phase
    swept = gc_sweep();
    gc_cycles = gc_cycles + 1;
    return swept;
}

int count_alive() {
    int i;
    int count;
    count = 0;
    for (i = 0; i < num_objects; i++) {
        if (obj_alive[i] == 1) count = count + 1;
    }
    return count;
}

int main() {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int swept;

    gc_init();
    printf("Mark-and-Sweep GC Simulation\n");
    // EXPECT: Mark-and-Sweep GC Simulation

    // Create object graph
    a = gc_alloc(10);
    b = gc_alloc(20);
    c = gc_alloc(30);
    d = gc_alloc(40);
    e = gc_alloc(50);
    f = gc_alloc(60);
    printf("Created %d objects\n", num_objects);
    // EXPECT: Created 6 objects

    // Build graph: a -> b -> c, a -> d, e and f are unreachable
    gc_set_ref(a, 0, b);
    gc_set_ref(b, 0, c);
    gc_set_ref(a, 1, d);
    printf("Graph: a->b->c, a->d, e and f floating\n");
    // EXPECT: Graph: a->b->c, a->d, e and f floating

    // Add a as root
    gc_add_root(a);
    printf("Root: a (obj %d)\n", a);
    // EXPECT: Root: a (obj 0)

    // First GC cycle
    swept = gc_collect();
    printf("GC cycle 1: swept %d, marked %d\n", swept, gc_mark_count);
    // EXPECT: GC cycle 1: swept 2, marked 4
    printf("Alive: %d\n", count_alive());
    // EXPECT: Alive: 4

    // Check which survived
    printf("a alive: %d\n", obj_alive[a]);
    // EXPECT: a alive: 1
    printf("e alive: %d\n", obj_alive[e]);
    // EXPECT: e alive: 0
    printf("f alive: %d\n", obj_alive[f]);
    // EXPECT: f alive: 0

    // Remove ref from a to d
    gc_set_ref(a, 1, -1);
    swept = gc_collect();
    printf("GC cycle 2: swept %d\n", swept);
    // EXPECT: GC cycle 2: swept 1
    printf("d alive: %d\n", obj_alive[d]);
    // EXPECT: d alive: 0
    printf("Alive: %d\n", count_alive());
    // EXPECT: Alive: 3

    // Create a cycle: c -> a (cycle detection is a weakness of mark-sweep, but it handles it)
    gc_set_ref(c, 0, a);
    printf("Added cycle c->a\n");
    // EXPECT: Added cycle c->a

    // GC with cycle should still work (all reachable from root)
    swept = gc_collect();
    printf("GC cycle 3 with cycle: swept %d\n", swept);
    // EXPECT: GC cycle 3 with cycle: swept 0
    printf("Alive: %d\n", count_alive());
    // EXPECT: Alive: 3

    // Remove root -> everything becomes garbage
    gc_remove_root(a);
    printf("Removed root, roots: %d\n", num_roots);
    // EXPECT: Removed root, roots: 0

    swept = gc_collect();
    printf("GC cycle 4 no roots: swept %d\n", swept);
    // EXPECT: GC cycle 4 no roots: swept 3
    printf("Alive: %d\n", count_alive());
    // EXPECT: Alive: 0

    // Stats
    printf("Total cycles: %d, total swept: %d\n", gc_cycles, gc_sweep_count);
    // EXPECT: Total cycles: 4, total swept: 6

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
