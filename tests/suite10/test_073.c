int printf(const char *fmt, ...);

// Test 073: Cleanup-on-error with goto cleanup pattern

enum ResourceState {
    RES_UNINIT,
    RES_ALLOCATED,
    RES_OPENED,
    RES_LOCKED,
    RES_ACTIVE
};

struct Resource {
    int id;
    int state;
    int data;
};

int resource_count = 0;
int cleanup_count = 0;

void init_resource(struct Resource *r, int id) {
    r->id = id;
    r->state = RES_UNINIT;
    r->data = 0;
}

int allocate_resource(struct Resource *r) {
    if (r->state != RES_UNINIT) return -1;
    r->state = RES_ALLOCATED;
    resource_count++;
    return 0;
}

int open_resource(struct Resource *r) {
    if (r->state != RES_ALLOCATED) return -1;
    r->state = RES_OPENED;
    return 0;
}

int lock_resource(struct Resource *r) {
    if (r->state != RES_OPENED) return -1;
    r->state = RES_LOCKED;
    return 0;
}

int activate_resource(struct Resource *r, int fail_flag) {
    if (r->state != RES_LOCKED) return -1;
    if (fail_flag) return -2;
    r->state = RES_ACTIVE;
    r->data = r->id * 100;
    return 0;
}

void cleanup_resource(struct Resource *r) {
    if (r->state != RES_UNINIT) {
        cleanup_count++;
        r->state = RES_UNINIT;
        r->data = 0;
        resource_count--;
    }
}

char *state_str(int s) {
    if (s == RES_UNINIT) return "UNINIT";
    if (s == RES_ALLOCATED) return "ALLOCATED";
    if (s == RES_OPENED) return "OPENED";
    if (s == RES_LOCKED) return "LOCKED";
    if (s == RES_ACTIVE) return "ACTIVE";
    return "UNKNOWN";
}

int do_operation(int id, int fail_at_step) {
    struct Resource r1;
    struct Resource r2;
    struct Resource r3;
    int err = 0;

    init_resource(&r1, id);
    init_resource(&r2, id + 1);
    init_resource(&r3, id + 2);

    err = allocate_resource(&r1);
    if (err != 0) goto cleanup;
    if (fail_at_step == 1) { err = -1; goto cleanup; }

    err = allocate_resource(&r2);
    if (err != 0) goto cleanup;

    err = open_resource(&r1);
    if (err != 0) goto cleanup;
    if (fail_at_step == 2) { err = -2; goto cleanup; }

    err = open_resource(&r2);
    if (err != 0) goto cleanup;

    err = allocate_resource(&r3);
    if (err != 0) goto cleanup;

    err = lock_resource(&r1);
    if (err != 0) goto cleanup;
    if (fail_at_step == 3) { err = -3; goto cleanup; }

    err = lock_resource(&r2);
    if (err != 0) goto cleanup;

    err = open_resource(&r3);
    if (err != 0) goto cleanup;

    err = activate_resource(&r1, 0);
    if (err != 0) goto cleanup;

    err = activate_resource(&r2, fail_at_step == 4 ? 1 : 0);
    if (err != 0) goto cleanup;

    printf("op %d: success r1=%d r2=%d\n", id, r1.data, r2.data);

cleanup:
    if (err != 0) {
        printf("op %d: failed at step %d, cleaning up\n", id, fail_at_step);
    }
    cleanup_resource(&r3);
    cleanup_resource(&r2);
    cleanup_resource(&r1);
    return err;
}

int main() {
    int ret;

    ret = do_operation(1, 0);
    // EXPECT: op 1: success r1=100 r2=200
    printf("result=%d resources=%d cleanups=%d\n", ret, resource_count, cleanup_count);
    // EXPECT: result=0 resources=0 cleanups=3

    cleanup_count = 0;
    ret = do_operation(10, 1);
    // EXPECT: op 10: failed at step 1, cleaning up
    printf("result=%d resources=%d cleanups=%d\n", ret, resource_count, cleanup_count);
    // EXPECT: result=-1 resources=0 cleanups=1

    cleanup_count = 0;
    ret = do_operation(20, 2);
    // EXPECT: op 20: failed at step 2, cleaning up
    printf("result=%d resources=%d cleanups=%d\n", ret, resource_count, cleanup_count);
    // EXPECT: result=-2 resources=0 cleanups=2

    cleanup_count = 0;
    ret = do_operation(30, 3);
    // EXPECT: op 30: failed at step 3, cleaning up
    printf("result=%d resources=%d cleanups=%d\n", ret, resource_count, cleanup_count);
    // EXPECT: result=-3 resources=0 cleanups=3

    cleanup_count = 0;
    ret = do_operation(40, 4);
    // EXPECT: op 40: failed at step 4, cleaning up
    printf("result=%d resources=%d cleanups=%d\n", ret, resource_count, cleanup_count);
    // EXPECT: result=-2 resources=0 cleanups=3

    printf("All goto cleanup tests passed\n");
    // EXPECT: All goto cleanup tests passed
    return 0;
}
