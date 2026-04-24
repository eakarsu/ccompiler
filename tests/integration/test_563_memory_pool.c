int printf(const char *fmt, ...);
// EXPECT: Pool initialized\nIn use: 0, Allocated: 0, Freed: 0\nAllocated 5 items: handles 0 1 2 3 4\nIn use: 5, Allocated: 5, Freed: 0\nValues: 100 200 300 400 500\nFreed handles 1 and 3\nIn use: 3, Allocated: 5, Freed: 2\nRealloc into freed slot: handle=1 value=600\nIn use: 4, Allocated: 6, Freed: 2\nGet freed handle 1: 600\nFree invalid: -1

#define POOL_SIZE 16

struct PoolItem {
    int data;
    int in_use;
};

struct Pool {
    struct PoolItem items[POOL_SIZE];
    int allocated;
    int freed;
};

void pool_init(struct Pool *p) {
    int i;
    for (i = 0; i < POOL_SIZE; i++) {
        p->items[i].data = 0;
        p->items[i].in_use = 0;
    }
    p->allocated = 0;
    p->freed = 0;
}

int pool_alloc(struct Pool *p, int value) {
    int i;
    for (i = 0; i < POOL_SIZE; i++) {
        if (p->items[i].in_use == 0) {
            p->items[i].data = value;
            p->items[i].in_use = 1;
            p->allocated++;
            return i;
        }
    }
    return -1;
}

int pool_free(struct Pool *p, int handle) {
    if (handle < 0 || handle >= POOL_SIZE) return -1;
    if (p->items[handle].in_use == 0) return -1;
    p->items[handle].in_use = 0;
    p->items[handle].data = 0;
    p->freed++;
    return 0;
}

int pool_get(struct Pool *p, int handle) {
    if (handle < 0 || handle >= POOL_SIZE) return -1;
    if (p->items[handle].in_use == 0) return -1;
    return p->items[handle].data;
}

int pool_count(struct Pool *p) {
    int count = 0;
    int i;
    for (i = 0; i < POOL_SIZE; i++) {
        if (p->items[i].in_use) count++;
    }
    return count;
}

void pool_print_stats(struct Pool *p) {
    printf("In use: %d, Allocated: %d, Freed: %d\n",
           pool_count(p), p->allocated, p->freed);
}

int main(void) {
    struct Pool pool;
    pool_init(&pool);
    printf("Pool initialized\n");
    pool_print_stats(&pool);

    int h1 = pool_alloc(&pool, 100);
    int h2 = pool_alloc(&pool, 200);
    int h3 = pool_alloc(&pool, 300);
    int h4 = pool_alloc(&pool, 400);
    int h5 = pool_alloc(&pool, 500);
    printf("Allocated 5 items: handles %d %d %d %d %d\n", h1, h2, h3, h4, h5);
    pool_print_stats(&pool);

    printf("Values: %d %d %d %d %d\n",
           pool_get(&pool, h1), pool_get(&pool, h2),
           pool_get(&pool, h3), pool_get(&pool, h4),
           pool_get(&pool, h5));

    pool_free(&pool, h2);
    pool_free(&pool, h4);
    printf("Freed handles %d and %d\n", h2, h4);
    pool_print_stats(&pool);

    int h6 = pool_alloc(&pool, 600);
    printf("Realloc into freed slot: handle=%d value=%d\n", h6, pool_get(&pool, h6));
    pool_print_stats(&pool);

    printf("Get freed handle %d: %d\n", h2, pool_get(&pool, h2));
    printf("Free invalid: %d\n", pool_free(&pool, 99));

    return 0;
}
