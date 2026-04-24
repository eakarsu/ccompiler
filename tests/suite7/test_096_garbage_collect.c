int printf(const char *fmt, ...);

// Mark-and-sweep garbage collection simulation

enum {
    OBJ_FREE = 0,
    OBJ_ALLOCATED = 1,
    OBJ_MARKED = 2
};

struct GCObject {
    int id;
    int state;
    int refs[4];     // indices of objects this references (-1 = none)
    int ref_count;
    int size;
};

struct GCHeap {
    struct GCObject objects[16];
    int capacity;
    int allocated;
    int roots[4];    // root set indices
    int root_count;
    int collections;
    int total_freed;
};

void heap_init(struct GCHeap *heap) {
    int i;
    int j;
    heap->capacity = 16;
    heap->allocated = 0;
    heap->root_count = 0;
    heap->collections = 0;
    heap->total_freed = 0;
    for (i = 0; i < 16; i++) {
        heap->objects[i].id = i;
        heap->objects[i].state = OBJ_FREE;
        heap->objects[i].ref_count = 0;
        heap->objects[i].size = 0;
        for (j = 0; j < 4; j++) {
            heap->objects[i].refs[j] = -1;
        }
    }
}

int gc_alloc(struct GCHeap *heap, int size) {
    int i;
    for (i = 0; i < heap->capacity; i++) {
        if (heap->objects[i].state == OBJ_FREE) {
            heap->objects[i].state = OBJ_ALLOCATED;
            heap->objects[i].size = size;
            heap->objects[i].ref_count = 0;
            int j;
            for (j = 0; j < 4; j++) heap->objects[i].refs[j] = -1;
            heap->allocated = heap->allocated + 1;
            return i;
        }
    }
    return -1; // out of memory
}

void gc_add_ref(struct GCHeap *heap, int from, int to) {
    if (from < 0 || to < 0) return;
    struct GCObject *obj = &heap->objects[from];
    if (obj->ref_count < 4) {
        obj->refs[obj->ref_count] = to;
        obj->ref_count = obj->ref_count + 1;
    }
}

void gc_add_root(struct GCHeap *heap, int obj_idx) {
    if (heap->root_count < 4) {
        heap->roots[heap->root_count] = obj_idx;
        heap->root_count = heap->root_count + 1;
    }
}

void mark_recursive(struct GCHeap *heap, int idx) {
    if (idx < 0 || idx >= heap->capacity) return;
    if (heap->objects[idx].state == OBJ_MARKED) return;
    if (heap->objects[idx].state == OBJ_FREE) return;
    heap->objects[idx].state = OBJ_MARKED;
    int i;
    for (i = 0; i < heap->objects[idx].ref_count; i++) {
        mark_recursive(heap, heap->objects[idx].refs[i]);
    }
}

void gc_mark(struct GCHeap *heap) {
    int i;
    for (i = 0; i < heap->root_count; i++) {
        mark_recursive(heap, heap->roots[i]);
    }
}

int gc_sweep(struct GCHeap *heap) {
    int freed = 0;
    int i;
    for (i = 0; i < heap->capacity; i++) {
        if (heap->objects[i].state == OBJ_ALLOCATED) {
            heap->objects[i].state = OBJ_FREE;
            heap->objects[i].size = 0;
            heap->objects[i].ref_count = 0;
            int j;
            for (j = 0; j < 4; j++) heap->objects[i].refs[j] = -1;
            heap->allocated = heap->allocated - 1;
            freed = freed + 1;
        } else if (heap->objects[i].state == OBJ_MARKED) {
            heap->objects[i].state = OBJ_ALLOCATED; // reset mark
        }
    }
    return freed;
}

int gc_collect(struct GCHeap *heap) {
    gc_mark(heap);
    int freed = gc_sweep(heap);
    heap->collections = heap->collections + 1;
    heap->total_freed = heap->total_freed + freed;
    return freed;
}

int count_live(struct GCHeap *heap) {
    int count = 0;
    int i;
    for (i = 0; i < heap->capacity; i++) {
        if (heap->objects[i].state != OBJ_FREE) count = count + 1;
    }
    return count;
}

int total_size(struct GCHeap *heap) {
    int total = 0;
    int i;
    for (i = 0; i < heap->capacity; i++) {
        if (heap->objects[i].state != OBJ_FREE) total = total + heap->objects[i].size;
    }
    return total;
}

int main(void) {
    struct GCHeap heap;
    int a;
    int b;
    int c;
    int d;
    int e;
    int freed;

    heap_init(&heap);
    // EXPECT: Heap capacity=16 allocated=0
    printf("Heap capacity=%d allocated=%d\n", heap.capacity, heap.allocated);

    // Allocate objects
    a = gc_alloc(&heap, 10);
    b = gc_alloc(&heap, 20);
    c = gc_alloc(&heap, 30);
    d = gc_alloc(&heap, 40);
    e = gc_alloc(&heap, 50);

    // EXPECT: Allocated 5 objects: 0 1 2 3 4
    printf("Allocated 5 objects: %d %d %d %d %d\n", a, b, c, d, e);

    // EXPECT: Live objects: 5
    printf("Live objects: %d\n", count_live(&heap));

    // EXPECT: Total size: 150
    printf("Total size: %d\n", total_size(&heap));

    // Build reference graph: root -> a -> b -> c, d and e are unreachable
    gc_add_root(&heap, a);
    gc_add_ref(&heap, a, b);
    gc_add_ref(&heap, b, c);

    // Collect - should free d and e
    freed = gc_collect(&heap);
    // EXPECT: Freed: 2
    printf("Freed: %d\n", freed);

    // EXPECT: After GC live: 3
    printf("After GC live: %d\n", count_live(&heap));

    // EXPECT: After GC allocated: 3
    printf("After GC allocated: %d\n", heap.allocated);

    // EXPECT: After GC size: 60
    printf("After GC size: %d\n", total_size(&heap));

    // d and e slots are now free
    // EXPECT: Obj 3 state=0 Obj 4 state=0
    printf("Obj 3 state=%d Obj 4 state=%d\n", heap.objects[3].state, heap.objects[4].state);

    // Allocate new objects in freed slots
    d = gc_alloc(&heap, 15);
    e = gc_alloc(&heap, 25);
    // EXPECT: Reused slots: 3 4
    printf("Reused slots: %d %d\n", d, e);

    // Make d reachable from c, e remains unreachable
    gc_add_ref(&heap, c, d);

    freed = gc_collect(&heap);
    // EXPECT: Second GC freed: 1
    printf("Second GC freed: %d\n", freed);

    // EXPECT: After second GC live: 4
    printf("After second GC live: %d\n", count_live(&heap));

    // Test cyclic references
    {
        struct GCHeap heap2;
        int x;
        int y;
        int z;
        int w;

        heap_init(&heap2);
        x = gc_alloc(&heap2, 5);
        y = gc_alloc(&heap2, 5);
        z = gc_alloc(&heap2, 5);
        w = gc_alloc(&heap2, 5);

        // Cycle: x -> y -> z -> x, w is isolated
        gc_add_ref(&heap2, x, y);
        gc_add_ref(&heap2, y, z);
        gc_add_ref(&heap2, z, x);
        gc_add_root(&heap2, x);

        freed = gc_collect(&heap2);
        // EXPECT: Cycle GC freed: 1
        printf("Cycle GC freed: %d\n", freed);

        // EXPECT: Cycle live: 3
        printf("Cycle live: %d\n", count_live(&heap2));
    }

    // Test all unreachable (no roots)
    {
        struct GCHeap heap3;
        heap_init(&heap3);
        gc_alloc(&heap3, 10);
        gc_alloc(&heap3, 20);
        gc_alloc(&heap3, 30);
        freed = gc_collect(&heap3);
        // EXPECT: No roots freed: 3
        printf("No roots freed: %d\n", freed);

        // EXPECT: No roots live: 0
        printf("No roots live: %d\n", count_live(&heap3));
    }

    // Collection count
    // EXPECT: Collections: 2
    printf("Collections: %d\n", heap.collections);

    // EXPECT: Total freed: 3
    printf("Total freed: %d\n", heap.total_freed);

    // EXPECT: GC sim complete
    printf("GC sim complete\n");

    return 0;
}
