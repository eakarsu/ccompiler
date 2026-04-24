int printf(const char *fmt, ...);

struct PoolObj {
    int id;
    int data;
    int active;
    int next_free;
};

struct Pool {
    struct PoolObj objects[16];
    int free_head;
    int capacity;
    int active_count;
};

void pool_init(struct Pool *p) {
    p->capacity = 16;
    p->active_count = 0;
    p->free_head = 0;
    int i = 0;
    while (i < 16) {
        p->objects[i].id = -1;
        p->objects[i].data = 0;
        p->objects[i].active = 0;
        p->objects[i].next_free = i + 1;
        i = i + 1;
    }
    p->objects[15].next_free = -1;
}

int pool_alloc(struct Pool *p, int id, int data) {
    if (p->free_head == -1) {
        return -1;
    }
    int idx = p->free_head;
    p->free_head = p->objects[idx].next_free;
    p->objects[idx].id = id;
    p->objects[idx].data = data;
    p->objects[idx].active = 1;
    p->objects[idx].next_free = -1;
    p->active_count = p->active_count + 1;
    return idx;
}

int pool_free(struct Pool *p, int idx) {
    if (idx < 0 || idx >= p->capacity) return -1;
    if (p->objects[idx].active == 0) return -1;
    p->objects[idx].active = 0;
    p->objects[idx].id = -1;
    p->objects[idx].data = 0;
    p->objects[idx].next_free = p->free_head;
    p->free_head = idx;
    p->active_count = p->active_count - 1;
    return 0;
}

int pool_get_data(struct Pool *p, int idx) {
    if (idx < 0 || idx >= p->capacity) return -1;
    if (p->objects[idx].active == 0) return -1;
    return p->objects[idx].data;
}

int pool_set_data(struct Pool *p, int idx, int data) {
    if (idx < 0 || idx >= p->capacity) return -1;
    if (p->objects[idx].active == 0) return -1;
    p->objects[idx].data = data;
    return 0;
}

int pool_count_active(struct Pool *p) {
    int count = 0;
    int i = 0;
    while (i < p->capacity) {
        if (p->objects[i].active) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int pool_sum_data(struct Pool *p) {
    int sum = 0;
    int i = 0;
    while (i < p->capacity) {
        if (p->objects[i].active) {
            sum = sum + p->objects[i].data;
        }
        i = i + 1;
    }
    return sum;
}

int main(void) {
    struct Pool pool;
    pool_init(&pool);

    // EXPECT: init_active=0
    printf("init_active=%d\n", pool.active_count);
    // EXPECT: init_free=0
    printf("init_free=%d\n", pool.free_head);

    int a = pool_alloc(&pool, 100, 10);
    int b = pool_alloc(&pool, 101, 20);
    int c = pool_alloc(&pool, 102, 30);

    // EXPECT: a=0
    printf("a=%d\n", a);
    // EXPECT: b=1
    printf("b=%d\n", b);
    // EXPECT: c=2
    printf("c=%d\n", c);
    // EXPECT: active=3
    printf("active=%d\n", pool.active_count);

    // EXPECT: data_a=10
    printf("data_a=%d\n", pool_get_data(&pool, a));
    // EXPECT: data_b=20
    printf("data_b=%d\n", pool_get_data(&pool, b));
    // EXPECT: data_c=30
    printf("data_c=%d\n", pool_get_data(&pool, c));

    int ret = pool_free(&pool, b);
    // EXPECT: free_ret=0
    printf("free_ret=%d\n", ret);
    // EXPECT: active_after_free=2
    printf("active_after_free=%d\n", pool.active_count);

    // EXPECT: freed_data=-1
    printf("freed_data=%d\n", pool_get_data(&pool, b));

    int d = pool_alloc(&pool, 103, 40);
    // EXPECT: d_reuses=1
    printf("d_reuses=%d\n", d);
    // EXPECT: data_d=40
    printf("data_d=%d\n", pool_get_data(&pool, d));

    // EXPECT: active_after_reuse=3
    printf("active_after_reuse=%d\n", pool.active_count);

    // EXPECT: count_verify=3
    printf("count_verify=%d\n", pool_count_active(&pool));
    // EXPECT: sum_data=80
    printf("sum_data=%d\n", pool_sum_data(&pool));

    pool_set_data(&pool, a, 99);
    // EXPECT: updated_a=99
    printf("updated_a=%d\n", pool_get_data(&pool, a));

    pool_free(&pool, a);
    pool_free(&pool, c);
    pool_free(&pool, d);
    // EXPECT: all_freed=0
    printf("all_freed=%d\n", pool.active_count);

    int e = pool_alloc(&pool, 200, 50);
    int f = pool_alloc(&pool, 201, 60);
    // EXPECT: e_idx=1
    printf("e_idx=%d\n", e);
    // EXPECT: f_idx=2
    printf("f_idx=%d\n", f);

    // EXPECT: double_free=-1
    printf("double_free=%d\n", pool_free(&pool, 5));

    // EXPECT: final_sum=110
    printf("final_sum=%d\n", pool_sum_data(&pool));

    return 0;
}
