int printf(const char *fmt, ...);

// Futures/Promises pattern simulation (single-threaded)
// Simulates asynchronous computation with resolve/reject/then

struct Future {
    int state;      // 0=pending, 1=resolved, 2=rejected
    int value;
    int error_code;
    int id;
    int depends_on; // id of future this depends on, -1 if none
};

struct FuturePool {
    struct Future futures[16];
    int count;
    int next_id;
    int resolved_count;
    int rejected_count;
};

void fpool_init(struct FuturePool *fp) {
    int i;
    fp->count = 0;
    fp->next_id = 1;
    fp->resolved_count = 0;
    fp->rejected_count = 0;
    for (i = 0; i < 16; i = i + 1) {
        fp->futures[i].state = 0;
        fp->futures[i].value = 0;
        fp->futures[i].error_code = 0;
        fp->futures[i].id = 0;
        fp->futures[i].depends_on = -1;
    }
}

int fpool_create(struct FuturePool *fp) {
    int idx;
    if (fp->count >= 16) return -1;
    idx = fp->count;
    fp->futures[idx].id = fp->next_id;
    fp->futures[idx].state = 0;
    fp->futures[idx].value = 0;
    fp->futures[idx].error_code = 0;
    fp->futures[idx].depends_on = -1;
    fp->next_id = fp->next_id + 1;
    fp->count = fp->count + 1;
    return fp->futures[idx].id;
}

int fpool_find(struct FuturePool *fp, int id) {
    int i;
    for (i = 0; i < fp->count; i = i + 1) {
        if (fp->futures[i].id == id) return i;
    }
    return -1;
}

int fpool_resolve(struct FuturePool *fp, int id, int value) {
    int idx;
    idx = fpool_find(fp, id);
    if (idx < 0) return 0;
    if (fp->futures[idx].state != 0) return 0;
    fp->futures[idx].state = 1;
    fp->futures[idx].value = value;
    fp->resolved_count = fp->resolved_count + 1;
    return 1;
}

int fpool_reject(struct FuturePool *fp, int id, int err) {
    int idx;
    idx = fpool_find(fp, id);
    if (idx < 0) return 0;
    if (fp->futures[idx].state != 0) return 0;
    fp->futures[idx].state = 2;
    fp->futures[idx].error_code = err;
    fp->rejected_count = fp->rejected_count + 1;
    return 1;
}

int fpool_get_state(struct FuturePool *fp, int id) {
    int idx;
    idx = fpool_find(fp, id);
    if (idx < 0) return -1;
    return fp->futures[idx].state;
}

int fpool_get_value(struct FuturePool *fp, int id) {
    int idx;
    idx = fpool_find(fp, id);
    if (idx < 0) return -999;
    if (fp->futures[idx].state != 1) return -999;
    return fp->futures[idx].value;
}

// then: create a new future that depends on another
int fpool_then(struct FuturePool *fp, int dep_id) {
    int new_id;
    int idx;
    new_id = fpool_create(fp);
    if (new_id < 0) return -1;
    idx = fpool_find(fp, new_id);
    fp->futures[idx].depends_on = dep_id;
    return new_id;
}

// Try to propagate: if dependency is resolved, resolve this with transformed value
int fpool_propagate(struct FuturePool *fp, int id, int transform) {
    int idx;
    int dep_idx;
    int dep_id;
    int dep_val;
    idx = fpool_find(fp, id);
    if (idx < 0) return 0;
    dep_id = fp->futures[idx].depends_on;
    if (dep_id < 0) return 0;
    dep_idx = fpool_find(fp, dep_id);
    if (dep_idx < 0) return 0;
    if (fp->futures[dep_idx].state == 1) {
        dep_val = fp->futures[dep_idx].value;
        // Transform: 0=same, 1=double, 2=add100, 3=negate
        if (transform == 0) fpool_resolve(fp, id, dep_val);
        else if (transform == 1) fpool_resolve(fp, id, dep_val * 2);
        else if (transform == 2) fpool_resolve(fp, id, dep_val + 100);
        else fpool_resolve(fp, id, -dep_val);
        return 1;
    }
    if (fp->futures[dep_idx].state == 2) {
        fpool_reject(fp, id, fp->futures[dep_idx].error_code);
        return 1;
    }
    return 0;
}

// all: check if all futures in range are resolved
int fpool_all_resolved(struct FuturePool *fp, int start, int end) {
    int i;
    for (i = start; i < end && i < fp->count; i = i + 1) {
        if (fp->futures[i].state != 1) return 0;
    }
    return 1;
}

int main() {
    struct FuturePool fp;
    fpool_init(&fp);

    // Create futures
    int f1 = fpool_create(&fp);
    int f2 = fpool_create(&fp);
    int f3 = fpool_create(&fp);
    printf("created: %d %d %d\n", f1, f2, f3); // EXPECT: created: 1 2 3
    printf("count: %d\n", fp.count); // EXPECT: count: 3

    // All pending
    printf("f1 state: %d\n", fpool_get_state(&fp, f1)); // EXPECT: f1 state: 0
    printf("f1 value: %d\n", fpool_get_value(&fp, f1)); // EXPECT: f1 value: -999

    // Resolve f1
    printf("resolve f1: %d\n", fpool_resolve(&fp, f1, 42)); // EXPECT: resolve f1: 1
    printf("f1 state: %d\n", fpool_get_state(&fp, f1)); // EXPECT: f1 state: 1
    printf("f1 value: %d\n", fpool_get_value(&fp, f1)); // EXPECT: f1 value: 42

    // Cannot resolve twice
    printf("re-resolve: %d\n", fpool_resolve(&fp, f1, 99)); // EXPECT: re-resolve: 0

    // Reject f2
    printf("reject f2: %d\n", fpool_reject(&fp, f2, 404)); // EXPECT: reject f2: 1
    printf("f2 state: %d\n", fpool_get_state(&fp, f2)); // EXPECT: f2 state: 2

    // then chaining: f4 depends on f1 (double the value)
    int f4 = fpool_then(&fp, f1);
    printf("f4 id: %d\n", f4); // EXPECT: f4 id: 4
    printf("f4 state: %d\n", fpool_get_state(&fp, f4)); // EXPECT: f4 state: 0

    int prop = fpool_propagate(&fp, f4, 1);
    printf("propagate f4: %d\n", prop); // EXPECT: propagate f4: 1
    printf("f4 value: %d\n", fpool_get_value(&fp, f4)); // EXPECT: f4 value: 84

    // Chain further: f5 depends on f4, add 100
    int f5 = fpool_then(&fp, f4);
    fpool_propagate(&fp, f5, 2);
    printf("f5 value: %d\n", fpool_get_value(&fp, f5)); // EXPECT: f5 value: 184

    // Rejection propagation: f6 depends on f2 (rejected)
    int f6 = fpool_then(&fp, f2);
    fpool_propagate(&fp, f6, 0);
    printf("f6 state: %d\n", fpool_get_state(&fp, f6)); // EXPECT: f6 state: 2

    // Resolve f3, test all_resolved
    fpool_resolve(&fp, f3, 10);
    printf("all 0-3: %d\n", fpool_all_resolved(&fp, 0, 3)); // EXPECT: all 0-3: 0
    printf("f1+f3: %d\n", fpool_all_resolved(&fp, 0, 1)); // EXPECT: f1+f3: 1

    printf("resolved: %d\n", fp.resolved_count); // EXPECT: resolved: 4
    printf("rejected: %d\n", fp.rejected_count); // EXPECT: rejected: 2

    // Test negate transform
    int f7 = fpool_create(&fp);
    fpool_resolve(&fp, f7, 50);
    int f8 = fpool_then(&fp, f7);
    fpool_propagate(&fp, f8, 3);
    printf("negated: %d\n", fpool_get_value(&fp, f8)); // EXPECT: negated: -50

    printf("total futures: %d\n", fp.count); // EXPECT: total futures: 8

    return 0;
}
