int printf(const char *fmt, ...);

// Reference Counting Garbage Collector Simulation
// Objects with reference counts, automatic deallocation when count hits 0

int obj_refcount[16];   // reference count for each object
int obj_value[16];      // stored value
int obj_alive[16];      // 1 = alive, 0 = collected
int obj_refs[16];       // index of another object this one references (-1 = none)
int next_obj;
int total_collected;
int total_created;

void rc_init() {
    int i;
    for (i = 0; i < 16; i++) {
        obj_refcount[i] = 0;
        obj_value[i] = 0;
        obj_alive[i] = 0;
        obj_refs[i] = -1;
    }
    next_obj = 0;
    total_collected = 0;
    total_created = 0;
}

int rc_create(int value) {
    if (next_obj >= 16) return -1;
    int id;
    id = next_obj;
    next_obj = next_obj + 1;
    obj_refcount[id] = 1;
    obj_value[id] = value;
    obj_alive[id] = 1;
    obj_refs[id] = -1;
    total_created = total_created + 1;
    return id;
}

void rc_collect(int id);

void rc_incref(int id) {
    if (id < 0 || id >= 16) return;
    if (obj_alive[id] == 0) return;
    obj_refcount[id] = obj_refcount[id] + 1;
}

void rc_decref(int id) {
    if (id < 0 || id >= 16) return;
    if (obj_alive[id] == 0) return;
    obj_refcount[id] = obj_refcount[id] - 1;
    if (obj_refcount[id] <= 0) {
        rc_collect(id);
    }
}

void rc_collect(int id) {
    if (id < 0 || id >= 16) return;
    if (obj_alive[id] == 0) return;

    obj_alive[id] = 0;
    total_collected = total_collected + 1;

    // If this object references another, decrement that ref
    int ref;
    ref = obj_refs[id];
    if (ref >= 0 && ref < 16) {
        rc_decref(ref);
    }
    obj_refs[id] = -1;
    obj_refcount[id] = 0;
}

void rc_set_ref(int from_id, int to_id) {
    if (from_id < 0 || from_id >= 16) return;
    if (obj_alive[from_id] == 0) return;

    // Decrement old reference
    int old_ref;
    old_ref = obj_refs[from_id];
    if (old_ref >= 0) {
        rc_decref(old_ref);
    }

    // Set new reference and increment
    obj_refs[from_id] = to_id;
    if (to_id >= 0 && to_id < 16) {
        rc_incref(to_id);
    }
}

int count_alive() {
    int i;
    int count;
    count = 0;
    for (i = 0; i < 16; i++) {
        if (obj_alive[i] == 1) count = count + 1;
    }
    return count;
}

int total_refs() {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < 16; i++) {
        if (obj_alive[i] == 1) {
            sum = sum + obj_refcount[i];
        }
    }
    return sum;
}

void print_obj(int id) {
    if (id < 0 || id >= 16) return;
    printf("Obj %d: alive=%d, rc=%d, val=%d\n",
           id, obj_alive[id], obj_refcount[id], obj_value[id]);
}

int main() {
    int a;
    int b;
    int c;
    int d;

    rc_init();
    printf("Reference Counting GC\n");
    // EXPECT: Reference Counting GC

    // Create objects
    a = rc_create(100);
    b = rc_create(200);
    c = rc_create(300);
    d = rc_create(400);
    printf("Created: %d %d %d %d\n", a, b, c, d);
    // EXPECT: Created: 0 1 2 3

    printf("Alive: %d\n", count_alive());
    // EXPECT: Alive: 4

    // Add references: a -> b, b -> c
    rc_set_ref(a, b);
    rc_set_ref(b, c);
    printf("After refs: a->b, b->c\n");
    // EXPECT: After refs: a->b, b->c

    print_obj(a);
    // EXPECT: Obj 0: alive=1, rc=1, val=100
    print_obj(b);
    // EXPECT: Obj 1: alive=1, rc=2, val=200
    print_obj(c);
    // EXPECT: Obj 2: alive=1, rc=2, val=300

    // Drop the external ref to b (decref)
    rc_decref(b);
    printf("After decref b\n");
    // EXPECT: After decref b
    print_obj(b);
    // EXPECT: Obj 1: alive=1, rc=1, val=200

    // Drop external ref to a -> cascades: a dies, b refcount drops, b dies, c refcount drops
    rc_decref(a);
    printf("After decref a (cascade)\n");
    // EXPECT: After decref a (cascade)
    print_obj(a);
    // EXPECT: Obj 0: alive=0, rc=0, val=100
    print_obj(b);
    // EXPECT: Obj 1: alive=0, rc=0, val=200
    print_obj(c);
    // EXPECT: Obj 2: alive=1, rc=1, val=300

    printf("Alive: %d, Collected: %d\n", count_alive(), total_collected);
    // EXPECT: Alive: 2, Collected: 2

    // Multiple references to same object
    rc_incref(c);
    rc_incref(c);
    printf("c refcount: %d\n", obj_refcount[c]);
    // EXPECT: c refcount: 3

    rc_decref(c);
    rc_decref(c);
    printf("c refcount after 2 decrefs: %d\n", obj_refcount[c]);
    // EXPECT: c refcount after 2 decrefs: 1

    // d references c
    rc_set_ref(d, c);
    printf("c refcount after d->c: %d\n", obj_refcount[c]);
    // EXPECT: c refcount after d->c: 2

    // Free d -> c gets decref
    rc_decref(d);
    printf("After freeing d\n");
    // EXPECT: After freeing d
    print_obj(d);
    // EXPECT: Obj 3: alive=0, rc=0, val=400
    print_obj(c);
    // EXPECT: Obj 2: alive=1, rc=1, val=300

    // Final collection
    rc_decref(c);
    printf("Final alive: %d, collected: %d, created: %d\n",
           count_alive(), total_collected, total_created);
    // EXPECT: Final alive: 0, collected: 4, created: 4

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
