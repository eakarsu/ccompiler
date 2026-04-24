int printf(const char *fmt, ...);

// Garbage collector mark phase (array-based)
// Objects are stored in an array, each with mark bit and references

struct GCObject {
    int id;
    int marked;
    int refs[4];
    int num_refs;
    int alive;
};

struct GCHeap {
    struct GCObject objects[16];
    int count;
    int roots[4];
    int num_roots;
    int mark_stack[32];
    int mark_sp;
};

void heap_init(struct GCHeap *h) {
    h->count = 0;
    h->num_roots = 0;
    h->mark_sp = 0;
    int i;
    int j;
    for (i = 0; i < 16; i++) {
        h->objects[i].id = i;
        h->objects[i].marked = 0;
        h->objects[i].num_refs = 0;
        h->objects[i].alive = 0;
        for (j = 0; j < 4; j++) {
            h->objects[i].refs[j] = -1;
        }
    }
    for (i = 0; i < 4; i++) h->roots[i] = -1;
}

int heap_alloc(struct GCHeap *h) {
    int idx = h->count;
    h->objects[idx].alive = 1;
    h->objects[idx].marked = 0;
    h->objects[idx].num_refs = 0;
    h->count = h->count + 1;
    return idx;
}

void heap_add_ref(struct GCHeap *h, int from, int to) {
    int n = h->objects[from].num_refs;
    h->objects[from].refs[n] = to;
    h->objects[from].num_refs = n + 1;
}

void heap_add_root(struct GCHeap *h, int obj) {
    h->roots[h->num_roots] = obj;
    h->num_roots = h->num_roots + 1;
}

void gc_mark(struct GCHeap *h) {
    // Clear all marks
    int i;
    for (i = 0; i < h->count; i++) {
        h->objects[i].marked = 0;
    }

    // Push roots onto mark stack
    h->mark_sp = 0;
    for (i = 0; i < h->num_roots; i++) {
        h->mark_stack[h->mark_sp] = h->roots[i];
        h->mark_sp = h->mark_sp + 1;
    }

    // Process mark stack
    while (h->mark_sp > 0) {
        h->mark_sp = h->mark_sp - 1;
        int obj = h->mark_stack[h->mark_sp];

        if (obj < 0 || obj >= h->count) continue;
        if (h->objects[obj].marked) continue;

        h->objects[obj].marked = 1;

        // Push all references
        int j;
        for (j = 0; j < h->objects[obj].num_refs; j++) {
            int ref = h->objects[obj].refs[j];
            if (ref >= 0 && !h->objects[ref].marked) {
                h->mark_stack[h->mark_sp] = ref;
                h->mark_sp = h->mark_sp + 1;
            }
        }
    }
}

int gc_sweep_count(struct GCHeap *h) {
    int swept = 0;
    int i;
    for (i = 0; i < h->count; i++) {
        if (h->objects[i].alive && !h->objects[i].marked) {
            h->objects[i].alive = 0;
            swept = swept + 1;
        }
    }
    return swept;
}

void print_marks(struct GCHeap *h) {
    int i;
    for (i = 0; i < h->count; i++) {
        printf("obj%d: %s\n", i, h->objects[i].marked ? "marked" : "unmarked");
    }
}

int main() {
    struct GCHeap heap;
    heap_init(&heap);

    // Create object graph:
    // root -> obj0 -> obj1 -> obj2
    //                  |-> obj3
    // obj4 is unreachable (garbage)
    // obj5 -> obj6 (both unreachable)

    int o0 = heap_alloc(&heap);  // 0
    int o1 = heap_alloc(&heap);  // 1
    int o2 = heap_alloc(&heap);  // 2
    int o3 = heap_alloc(&heap);  // 3
    int o4 = heap_alloc(&heap);  // 4
    int o5 = heap_alloc(&heap);  // 5
    int o6 = heap_alloc(&heap);  // 6

    heap_add_ref(&heap, o0, o1);
    heap_add_ref(&heap, o1, o2);
    heap_add_ref(&heap, o1, o3);
    heap_add_ref(&heap, o5, o6);

    heap_add_root(&heap, o0);

    printf("total objects: %d\n", heap.count);
    // EXPECT: total objects: 7

    gc_mark(&heap);
    print_marks(&heap);
    // EXPECT: obj0: marked
    // EXPECT: obj1: marked
    // EXPECT: obj2: marked
    // EXPECT: obj3: marked
    // EXPECT: obj4: unmarked
    // EXPECT: obj5: unmarked
    // EXPECT: obj6: unmarked

    int swept = gc_sweep_count(&heap);
    printf("swept: %d\n", swept);
    // EXPECT: swept: 3

    // Test 2: cycle detection
    heap_init(&heap);
    o0 = heap_alloc(&heap);
    o1 = heap_alloc(&heap);
    o2 = heap_alloc(&heap);
    heap_add_ref(&heap, o0, o1);
    heap_add_ref(&heap, o1, o2);
    heap_add_ref(&heap, o2, o0);  // cycle!
    heap_add_root(&heap, o0);

    gc_mark(&heap);
    printf("cycle all marked: %d\n",
           heap.objects[0].marked &&
           heap.objects[1].marked &&
           heap.objects[2].marked);
    // EXPECT: cycle all marked: 1

    // Test 3: multiple roots
    heap_init(&heap);
    o0 = heap_alloc(&heap);
    o1 = heap_alloc(&heap);
    o2 = heap_alloc(&heap);
    o3 = heap_alloc(&heap);
    heap_add_ref(&heap, o0, o2);
    heap_add_ref(&heap, o1, o3);
    heap_add_root(&heap, o0);
    heap_add_root(&heap, o1);

    gc_mark(&heap);
    int all_marked = 1;
    int idx;
    for (idx = 0; idx < 4; idx++) {
        if (!heap.objects[idx].marked) all_marked = 0;
    }
    printf("multi-root all marked: %d\n", all_marked);
    // EXPECT: multi-root all marked: 1

    printf("gc done\n");
    // EXPECT: gc done

    return 0;
}
