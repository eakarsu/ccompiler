int printf(const char *fmt, ...);

// Resource Pool Manager
// Manages a pool of resources with acquire/release operations

struct Resource {
    int id;
    int in_use;
    int owner;
    int acquire_count;
};

struct ResourcePool {
    struct Resource resources[8];
    int size;
    int total_acquires;
    int total_releases;
    int max_concurrent;
    int current_in_use;
};

void pool_init(struct ResourcePool *pool, int size) {
    int i;
    pool->size = size;
    pool->total_acquires = 0;
    pool->total_releases = 0;
    pool->max_concurrent = 0;
    pool->current_in_use = 0;
    for (i = 0; i < size; i++) {
        pool->resources[i].id = i;
        pool->resources[i].in_use = 0;
        pool->resources[i].owner = -1;
        pool->resources[i].acquire_count = 0;
    }
}

int pool_acquire(struct ResourcePool *pool, int owner_id) {
    int i;
    for (i = 0; i < pool->size; i++) {
        if (pool->resources[i].in_use == 0) {
            pool->resources[i].in_use = 1;
            pool->resources[i].owner = owner_id;
            pool->resources[i].acquire_count = pool->resources[i].acquire_count + 1;
            pool->total_acquires = pool->total_acquires + 1;
            pool->current_in_use = pool->current_in_use + 1;
            if (pool->current_in_use > pool->max_concurrent) {
                pool->max_concurrent = pool->current_in_use;
            }
            return i;
        }
    }
    return -1;
}

int pool_release(struct ResourcePool *pool, int resource_id) {
    if (resource_id < 0 || resource_id >= pool->size) return -1;
    if (pool->resources[resource_id].in_use == 0) return -1;
    pool->resources[resource_id].in_use = 0;
    pool->resources[resource_id].owner = -1;
    pool->total_releases = pool->total_releases + 1;
    pool->current_in_use = pool->current_in_use - 1;
    return 0;
}

int pool_available(struct ResourcePool *pool) {
    int count = 0;
    int i;
    for (i = 0; i < pool->size; i++) {
        if (pool->resources[i].in_use == 0) {
            count = count + 1;
        }
    }
    return count;
}

int pool_owner_of(struct ResourcePool *pool, int resource_id) {
    if (resource_id < 0 || resource_id >= pool->size) return -1;
    return pool->resources[resource_id].owner;
}

void pool_print_status(struct ResourcePool *pool) {
    int i;
    for (i = 0; i < pool->size; i++) {
        if (pool->resources[i].in_use) {
            printf("  R%d: owner=%d uses=%d\n", i, pool->resources[i].owner,
                   pool->resources[i].acquire_count);
        } else {
            printf("  R%d: free uses=%d\n", i, pool->resources[i].acquire_count);
        }
    }
}

int main() {
    struct ResourcePool pool;
    int r0;
    int r1;
    int r2;
    int r3;

    // EXPECT: === Resource Pool Manager ===
    printf("=== Resource Pool Manager ===\n");

    pool_init(&pool, 4);

    // EXPECT: Pool size: 4 available: 4
    printf("Pool size: %d available: %d\n", pool.size, pool_available(&pool));

    r0 = pool_acquire(&pool, 100);
    r1 = pool_acquire(&pool, 101);
    // EXPECT: Acquired R0 for owner 100
    printf("Acquired R%d for owner %d\n", r0, pool_owner_of(&pool, r0));
    // EXPECT: Acquired R1 for owner 101
    printf("Acquired R%d for owner %d\n", r1, pool_owner_of(&pool, r1));
    // EXPECT: Available: 2
    printf("Available: %d\n", pool_available(&pool));

    r2 = pool_acquire(&pool, 102);
    r3 = pool_acquire(&pool, 103);
    // EXPECT: Available after 4 acquires: 0
    printf("Available after 4 acquires: %d\n", pool_available(&pool));

    int r4 = pool_acquire(&pool, 104);
    // EXPECT: Acquire when full: -1
    printf("Acquire when full: %d\n", r4);

    pool_release(&pool, r1);
    // EXPECT: After releasing R1, available: 1
    printf("After releasing R1, available: %d\n", pool_available(&pool));

    int r5 = pool_acquire(&pool, 105);
    // EXPECT: Re-acquired slot: 1 owner: 105
    printf("Re-acquired slot: %d owner: %d\n", r5, pool_owner_of(&pool, r5));

    pool_release(&pool, r0);
    pool_release(&pool, r2);
    pool_release(&pool, r3);
    pool_release(&pool, r5);

    // EXPECT: After releasing all, available: 4
    printf("After releasing all, available: %d\n", pool_available(&pool));
    // EXPECT: Total acquires: 5 releases: 5
    printf("Total acquires: %d releases: %d\n", pool.total_acquires, pool.total_releases);
    // EXPECT: Max concurrent usage: 4
    printf("Max concurrent usage: %d\n", pool.max_concurrent);

    // EXPECT: Pool status:
    printf("Pool status:\n");
    pool_print_status(&pool);
    // EXPECT:   R0: free uses=1
    // EXPECT:   R1: free uses=2
    // EXPECT:   R2: free uses=1
    // EXPECT:   R3: free uses=1

    int bad_release = pool_release(&pool, 10);
    // EXPECT: Bad release result: -1
    printf("Bad release result: %d\n", bad_release);

    int double_release = pool_release(&pool, 0);
    // EXPECT: Double release result: -1
    printf("Double release result: %d\n", double_release);

    // EXPECT: All resource pool tests passed
    printf("All resource pool tests passed\n");
    return 0;
}
