int printf(const char *fmt, ...);

/* Reference Counting Memory Manager Simulation
 * Each object: [refcount, data0, data1, ref_to_obj]
 * Object size = 4 slots. Max 20 objects.
 * When refcount drops to 0, the object is freed and its references are decremented.
 */

int mem[80]; /* 20 objects * 4 slots */
int obj_alive[20];
int total_objects;

void rc_init(void) {
    int i;
    for (i = 0; i < 80; i++) mem[i] = 0;
    for (i = 0; i < 20; i++) obj_alive[i] = 0;
    total_objects = 0;
}

int rc_alloc(int d0, int d1) {
    int i;
    for (i = 0; i < 20; i++) {
        if (obj_alive[i] == 0) {
            obj_alive[i] = 1;
            int base = i * 4;
            mem[base] = 1;      /* refcount starts at 1 */
            mem[base + 1] = d0;
            mem[base + 2] = d1;
            mem[base + 3] = -1; /* no reference */
            total_objects = total_objects + 1;
            return i;
        }
    }
    return -1;
}

void rc_release(int obj); /* forward declaration */

void rc_incref(int obj) {
    if (obj < 0 || obj >= 20) return;
    if (obj_alive[obj] == 0) return;
    mem[obj * 4] = mem[obj * 4] + 1;
}

void rc_release(int obj) {
    if (obj < 0 || obj >= 20) return;
    if (obj_alive[obj] == 0) return;
    int base = obj * 4;
    mem[base] = mem[base] - 1;
    if (mem[base] <= 0) {
        /* Free this object */
        int child = mem[base + 3];
        obj_alive[obj] = 0;
        total_objects = total_objects - 1;
        /* Release referenced object */
        if (child >= 0) {
            rc_release(child);
        }
    }
}

void rc_set_ref(int from_obj, int to_obj) {
    if (from_obj < 0 || from_obj >= 20) return;
    if (obj_alive[from_obj] == 0) return;
    int base = from_obj * 4;

    /* Release old reference */
    int old_ref = mem[base + 3];
    if (old_ref >= 0) {
        rc_release(old_ref);
    }

    /* Set new reference and increment */
    mem[base + 3] = to_obj;
    if (to_obj >= 0) {
        rc_incref(to_obj);
    }
}

int rc_refcount(int obj) {
    if (obj < 0 || obj >= 20 || obj_alive[obj] == 0) return -1;
    return mem[obj * 4];
}

int rc_data(int obj, int slot) {
    if (obj < 0 || obj >= 20 || obj_alive[obj] == 0) return -1;
    return mem[obj * 4 + 1 + slot];
}

int count_live(void) {
    int c = 0;
    int i;
    for (i = 0; i < 20; i++) {
        if (obj_alive[i]) c = c + 1;
    }
    return c;
}

void test_basic_refcount(void) {
    printf("=== Basic Reference Counting ===\n"); // EXPECT: === Basic Reference Counting ===
    rc_init();

    int a = rc_alloc(10, 20);
    int b = rc_alloc(30, 40);
    printf("alloc: a=%d b=%d\n", a, b); // EXPECT: alloc: a=0 b=1
    printf("refcount: a=%d b=%d\n", rc_refcount(a), rc_refcount(b)); // EXPECT: refcount: a=1 b=1

    rc_incref(a);
    printf("after incref a: %d\n", rc_refcount(a)); // EXPECT: after incref a: 2

    rc_release(a);
    printf("after release a: %d alive=%d\n", rc_refcount(a), count_live()); // EXPECT: after release a: 1 alive=2

    rc_release(a);
    printf("after 2nd release a: alive=%d\n", count_live()); // EXPECT: after 2nd release a: alive=1

    rc_release(b);
    printf("after release b: alive=%d\n", count_live()); // EXPECT: after release b: alive=0
}

void test_cascading_free(void) {
    printf("=== Cascading Free ===\n"); // EXPECT: === Cascading Free ===
    rc_init();

    int a = rc_alloc(1, 0);
    int b = rc_alloc(2, 0);
    int c = rc_alloc(3, 0);

    rc_set_ref(a, b); /* a -> b, b refcount = 2 */
    rc_set_ref(b, c); /* b -> c, c refcount = 2 */

    printf("refcounts: a=%d b=%d c=%d\n", rc_refcount(a), rc_refcount(b), rc_refcount(c)); // EXPECT: refcounts: a=1 b=2 c=2

    /* Releasing a should cascade: a frees, b goes to 1 */
    rc_release(a);
    printf("after release a: alive=%d\n", count_live()); // EXPECT: after release a: alive=2
    printf("b refcount=%d c refcount=%d\n", rc_refcount(b), rc_refcount(c)); // EXPECT: b refcount=1 c refcount=2

    /* Releasing b should cascade further: b frees, c goes to 1 */
    rc_release(b);
    printf("after release b: alive=%d c refcount=%d\n", count_live(), rc_refcount(c)); // EXPECT: after release b: alive=1 c refcount=1

    rc_release(c);
    printf("after release c: alive=%d\n", count_live()); // EXPECT: after release c: alive=0
}

void test_shared_refs(void) {
    printf("=== Shared References ===\n"); // EXPECT: === Shared References ===
    rc_init();

    int shared = rc_alloc(42, 0);
    int a = rc_alloc(1, 0);
    int b = rc_alloc(2, 0);
    int c = rc_alloc(3, 0);

    rc_set_ref(a, shared);
    rc_set_ref(b, shared);
    rc_set_ref(c, shared);

    printf("shared refcount=%d\n", rc_refcount(shared)); // EXPECT: shared refcount=4

    rc_release(a);
    printf("after free a: shared refcount=%d alive=%d\n", rc_refcount(shared), count_live()); // EXPECT: after free a: shared refcount=3 alive=3

    rc_release(b);
    printf("after free b: shared refcount=%d alive=%d\n", rc_refcount(shared), count_live()); // EXPECT: after free b: shared refcount=2 alive=2

    rc_release(c);
    printf("after free c: shared refcount=%d alive=%d\n", rc_refcount(shared), count_live()); // EXPECT: after free c: shared refcount=1 alive=1

    rc_release(shared);
    printf("after free shared: alive=%d\n", count_live()); // EXPECT: after free shared: alive=0
}

void test_replace_ref(void) {
    printf("=== Replace Reference ===\n"); // EXPECT: === Replace Reference ===
    rc_init();

    int a = rc_alloc(1, 0);
    int b = rc_alloc(2, 0);
    int c = rc_alloc(3, 0);

    rc_set_ref(a, b);
    printf("b refcount=%d\n", rc_refcount(b)); // EXPECT: b refcount=2

    /* Replace a's ref from b to c */
    rc_set_ref(a, c);
    printf("after replace: b refcount=%d c refcount=%d\n", rc_refcount(b), rc_refcount(c)); // EXPECT: after replace: b refcount=1 c refcount=2

    printf("alive=%d\n", count_live()); // EXPECT: alive=3
    rc_release(a);
    rc_release(b);
    rc_release(c);
    printf("final alive=%d\n", count_live()); // EXPECT: final alive=0
}

void test_stress_refcount(void) {
    printf("=== Stress Refcount ===\n"); // EXPECT: === Stress Refcount ===
    rc_init();

    int i;
    int objects[10];
    for (i = 0; i < 10; i++) {
        objects[i] = rc_alloc(i * 10, i);
    }
    printf("allocated: %d\n", count_live()); // EXPECT: allocated: 10

    /* Chain them: 0->1->2->...->9 */
    for (i = 0; i < 9; i++) {
        rc_set_ref(objects[i], objects[i + 1]);
    }
    printf("obj[9] refcount=%d\n", rc_refcount(objects[9])); // EXPECT: obj[9] refcount=2

    /* Release from front: each cascades one decrement */
    rc_release(objects[0]);
    printf("after release 0: alive=%d\n", count_live()); // EXPECT: after release 0: alive=9

    /* Release remaining (each has refcount 1 from chain + 1 from alloc) */
    for (i = 1; i < 10; i++) {
        rc_release(objects[i]);
    }
    printf("final alive=%d\n", count_live()); // EXPECT: final alive=0
}

int main(void) {
    test_basic_refcount();
    test_cascading_free();
    test_shared_refs();
    test_replace_ref();
    test_stress_refcount();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
