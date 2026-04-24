int printf(const char *fmt, ...);

struct Connection {
    int id;
    int server_ip;
    int server_port;
    int in_use;
    int request_count;
    int idle_time;
    int created_at;
};

struct ConnPool {
    struct Connection conns[8];
    int pool_size;
    int max_size;
    int min_size;
    int active_count;
    int idle_count;
    int total_acquired;
    int total_released;
    int total_created;
    int total_destroyed;
    int next_id;
    int server_ip;
    int server_port;
    int max_idle_time;
};

int pool_init(struct ConnPool *p, int srv_ip, int srv_port, int min_sz, int max_sz, int max_idle) {
    p->pool_size = 0;
    p->max_size = max_sz;
    p->min_size = min_sz;
    p->active_count = 0;
    p->idle_count = 0;
    p->total_acquired = 0;
    p->total_released = 0;
    p->total_created = 0;
    p->total_destroyed = 0;
    p->next_id = 1;
    p->server_ip = srv_ip;
    p->server_port = srv_port;
    p->max_idle_time = max_idle;
    int i;
    for (i = 0; i < 8; i = i + 1) {
        p->conns[i].id = 0;
        p->conns[i].in_use = 0;
        p->conns[i].request_count = 0;
        p->conns[i].idle_time = 0;
        p->conns[i].created_at = 0;
    }
    return 0;
}

int pool_create_conn(struct ConnPool *p) {
    if (p->pool_size >= p->max_size) {
        printf("Pool full: cannot create (size=%d max=%d)\n", p->pool_size, p->max_size);
        return -1;
    }
    int idx = p->pool_size;
    p->conns[idx].id = p->next_id;
    p->conns[idx].server_ip = p->server_ip;
    p->conns[idx].server_port = p->server_port;
    p->conns[idx].in_use = 0;
    p->conns[idx].request_count = 0;
    p->conns[idx].idle_time = 0;
    p->conns[idx].created_at = p->total_created;
    p->next_id = p->next_id + 1;
    p->pool_size = p->pool_size + 1;
    p->idle_count = p->idle_count + 1;
    p->total_created = p->total_created + 1;
    printf("Created conn %d (pool_size=%d)\n", p->conns[idx].id, p->pool_size);
    return idx;
}

int pool_warmup(struct ConnPool *p) {
    printf("Warming up pool (min=%d)\n", p->min_size);
    int i;
    for (i = 0; i < p->min_size; i = i + 1) {
        pool_create_conn(p);
    }
    return p->pool_size;
}

int pool_acquire(struct ConnPool *p) {
    int i;
    for (i = 0; i < p->pool_size; i = i + 1) {
        if (p->conns[i].in_use == 0 && p->conns[i].id > 0) {
            p->conns[i].in_use = 1;
            p->conns[i].request_count = p->conns[i].request_count + 1;
            p->conns[i].idle_time = 0;
            p->active_count = p->active_count + 1;
            p->idle_count = p->idle_count - 1;
            p->total_acquired = p->total_acquired + 1;
            printf("Acquire conn %d (active=%d idle=%d)\n",
                   p->conns[i].id, p->active_count, p->idle_count);
            return i;
        }
    }
    int idx = pool_create_conn(p);
    if (idx < 0) {
        printf("Acquire FAILED: pool exhausted\n");
        return -1;
    }
    p->conns[idx].in_use = 1;
    p->conns[idx].request_count = 1;
    p->idle_count = p->idle_count - 1;
    p->active_count = p->active_count + 1;
    p->total_acquired = p->total_acquired + 1;
    printf("Acquire conn %d (active=%d idle=%d)\n",
           p->conns[idx].id, p->active_count, p->idle_count);
    return idx;
}

int pool_release(struct ConnPool *p, int idx) {
    if (idx < 0 || idx >= p->pool_size) return -1;
    if (p->conns[idx].in_use == 0) return -1;
    p->conns[idx].in_use = 0;
    p->conns[idx].idle_time = 0;
    p->active_count = p->active_count - 1;
    p->idle_count = p->idle_count + 1;
    p->total_released = p->total_released + 1;
    printf("Release conn %d (active=%d idle=%d)\n",
           p->conns[idx].id, p->active_count, p->idle_count);
    return 0;
}

int pool_evict_idle(struct ConnPool *p, int time_elapsed) {
    int evicted = 0;
    int i;
    for (i = 0; i < p->pool_size; i = i + 1) {
        if (p->conns[i].in_use == 0 && p->conns[i].id > 0) {
            p->conns[i].idle_time = p->conns[i].idle_time + time_elapsed;
            if (p->conns[i].idle_time > p->max_idle_time) {
                int active_total = p->active_count + p->idle_count;
                if (active_total > p->min_size) {
                    printf("Evict conn %d (idle=%d)\n", p->conns[i].id, p->conns[i].idle_time);
                    p->conns[i].id = 0;
                    p->idle_count = p->idle_count - 1;
                    p->total_destroyed = p->total_destroyed + 1;
                    evicted = evicted + 1;
                }
            }
        }
    }
    return evicted;
}

int pool_stats(struct ConnPool *p) {
    printf("Pool: size=%d active=%d idle=%d\n", p->pool_size, p->active_count, p->idle_count);
    printf("Totals: created=%d destroyed=%d acquired=%d released=%d\n",
           p->total_created, p->total_destroyed, p->total_acquired, p->total_released);
    return 0;
}

int main() {
    struct ConnPool pool;
    pool_init(&pool, 192, 5432, 2, 6, 60);

    printf("=== Connection Pooling ===\n");
    // EXPECT: === Connection Pooling ===
    printf("Server: %d:%d min=%d max=%d\n", pool.server_ip, pool.server_port, pool.min_size, pool.max_size);
    // EXPECT: Server: 192:5432 min=2 max=6

    printf("--- Warmup ---\n");
    // EXPECT: --- Warmup ---
    pool_warmup(&pool);
    // EXPECT: Warming up pool (min=2)
    // EXPECT: Created conn 1 (pool_size=1)
    // EXPECT: Created conn 2 (pool_size=2)

    printf("--- Acquire/Release ---\n");
    // EXPECT: --- Acquire/Release ---
    int c1 = pool_acquire(&pool);
    // EXPECT: Acquire conn 1 (active=1 idle=1)
    int c2 = pool_acquire(&pool);
    // EXPECT: Acquire conn 2 (active=2 idle=0)

    printf("--- Grow on demand ---\n");
    // EXPECT: --- Grow on demand ---
    int c3 = pool_acquire(&pool);
    // EXPECT: Created conn 3 (pool_size=3)
    // EXPECT: Acquire conn 3 (active=3 idle=0)

    pool_release(&pool, c1);
    // EXPECT: Release conn 1 (active=2 idle=1)
    pool_release(&pool, c2);
    // EXPECT: Release conn 2 (active=1 idle=2)

    printf("--- Reuse released ---\n");
    // EXPECT: --- Reuse released ---
    int c4 = pool_acquire(&pool);
    // EXPECT: Acquire conn 1 (active=2 idle=1)

    pool_release(&pool, c3);
    // EXPECT: Release conn 3 (active=1 idle=2)
    pool_release(&pool, c4);
    // EXPECT: Release conn 1 (active=0 idle=3)

    printf("--- Idle eviction ---\n");
    // EXPECT: --- Idle eviction ---
    int ev = pool_evict_idle(&pool, 70);
    // EXPECT: Evict conn 1 (idle=70)
    printf("Evicted: %d\n", ev);
    // EXPECT: Evicted: 1

    pool_stats(&pool);
    // EXPECT: Pool: size=3 active=0 idle=2
    // EXPECT: Totals: created=3 destroyed=1 acquired=4 released=4

    printf("--- Max pool test ---\n");
    // EXPECT: --- Max pool test ---
    int ci;
    int handles[6];
    for (ci = 0; ci < 6; ci = ci + 1) {
        handles[ci] = pool_acquire(&pool);
    }
    // EXPECT: Acquire conn 2 (active=1 idle=1)
    // EXPECT: Acquire conn 3 (active=2 idle=0)
    // EXPECT: Created conn 4 (pool_size=4)
    // EXPECT: Acquire conn 4 (active=3 idle=0)
    // EXPECT: Created conn 5 (pool_size=5)
    // EXPECT: Acquire conn 5 (active=4 idle=0)
    // EXPECT: Created conn 6 (pool_size=6)
    // EXPECT: Acquire conn 6 (active=5 idle=0)
    // EXPECT: Pool full: cannot create (size=6 max=6)
    // EXPECT: Acquire FAILED: pool exhausted

    pool_stats(&pool);
    // EXPECT: Pool: size=6 active=5 idle=0
    // EXPECT: Totals: created=6 destroyed=1 acquired=9 released=4

    printf("--- Request counts ---\n");
    // EXPECT: --- Request counts ---
    printf("Conn 2 requests: %d\n", pool.conns[1].request_count);
    // EXPECT: Conn 2 requests: 2
    printf("Conn 3 requests: %d\n", pool.conns[2].request_count);
    // EXPECT: Conn 3 requests: 2

    return 0;
}
