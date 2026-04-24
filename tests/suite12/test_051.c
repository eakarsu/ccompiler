int printf(const char *fmt, ...);

// Mutex simulation: single-threaded mutex with lock/unlock/trylock

struct Mutex {
    int locked;
    int owner;
    int lock_count;
    int contention_count;
};

void mutex_init(struct Mutex *m) {
    m->locked = 0;
    m->owner = -1;
    m->lock_count = 0;
    m->contention_count = 0;
}

int mutex_trylock(struct Mutex *m, int thread_id) {
    if (m->locked) {
        m->contention_count = m->contention_count + 1;
        return 0;
    }
    m->locked = 1;
    m->owner = thread_id;
    m->lock_count = m->lock_count + 1;
    return 1;
}

int mutex_lock(struct Mutex *m, int thread_id) {
    if (m->locked) {
        m->contention_count = m->contention_count + 1;
        return 0;
    }
    m->locked = 1;
    m->owner = thread_id;
    m->lock_count = m->lock_count + 1;
    return 1;
}

int mutex_unlock(struct Mutex *m, int thread_id) {
    if (!m->locked) {
        return 0;
    }
    if (m->owner != thread_id) {
        return -1;
    }
    m->locked = 0;
    m->owner = -1;
    return 1;
}

int mutex_is_locked(struct Mutex *m) {
    return m->locked;
}

// Simulate a shared counter protected by mutex
struct SharedCounter {
    int value;
    struct Mutex mtx;
};

void counter_init(struct SharedCounter *c) {
    c->value = 0;
    mutex_init(&c->mtx);
}

int counter_increment(struct SharedCounter *c, int tid) {
    if (mutex_lock(&c->mtx, tid)) {
        c->value = c->value + 1;
        mutex_unlock(&c->mtx, tid);
        return 1;
    }
    return 0;
}

int counter_decrement(struct SharedCounter *c, int tid) {
    if (mutex_lock(&c->mtx, tid)) {
        c->value = c->value - 1;
        mutex_unlock(&c->mtx, tid);
        return 1;
    }
    return 0;
}

int counter_get(struct SharedCounter *c, int tid) {
    int val;
    if (mutex_lock(&c->mtx, tid)) {
        val = c->value;
        mutex_unlock(&c->mtx, tid);
        return val;
    }
    return -999;
}

// Simulate recursive-style mutex (reentrant)
struct RecursiveMutex {
    int locked;
    int owner;
    int depth;
};

void rmutex_init(struct RecursiveMutex *m) {
    m->locked = 0;
    m->owner = -1;
    m->depth = 0;
}

int rmutex_lock(struct RecursiveMutex *m, int tid) {
    if (!m->locked) {
        m->locked = 1;
        m->owner = tid;
        m->depth = 1;
        return 1;
    }
    if (m->owner == tid) {
        m->depth = m->depth + 1;
        return 1;
    }
    return 0;
}

int rmutex_unlock(struct RecursiveMutex *m, int tid) {
    if (!m->locked || m->owner != tid) return 0;
    m->depth = m->depth - 1;
    if (m->depth == 0) {
        m->locked = 0;
        m->owner = -1;
    }
    return 1;
}

int main() {
    struct Mutex m;
    mutex_init(&m);

    printf("mutex locked: %d\n", mutex_is_locked(&m)); // EXPECT: mutex locked: 0

    int r1 = mutex_lock(&m, 1);
    printf("lock by t1: %d\n", r1); // EXPECT: lock by t1: 1
    printf("mutex locked: %d\n", mutex_is_locked(&m)); // EXPECT: mutex locked: 1
    printf("owner: %d\n", m.owner); // EXPECT: owner: 1

    int r2 = mutex_trylock(&m, 2);
    printf("trylock by t2: %d\n", r2); // EXPECT: trylock by t2: 0
    printf("contention: %d\n", m.contention_count); // EXPECT: contention: 1

    int r3 = mutex_unlock(&m, 2);
    printf("unlock by t2: %d\n", r3); // EXPECT: unlock by t2: -1

    int r4 = mutex_unlock(&m, 1);
    printf("unlock by t1: %d\n", r4); // EXPECT: unlock by t1: 1
    printf("mutex locked: %d\n", mutex_is_locked(&m)); // EXPECT: mutex locked: 0

    // Shared counter test
    struct SharedCounter cnt;
    counter_init(&cnt);

    int i;
    for (i = 0; i < 5; i = i + 1) {
        counter_increment(&cnt, 0);
    }
    printf("counter after 5 inc: %d\n", counter_get(&cnt, 0)); // EXPECT: counter after 5 inc: 5

    counter_decrement(&cnt, 0);
    counter_decrement(&cnt, 0);
    printf("counter after 2 dec: %d\n", counter_get(&cnt, 0)); // EXPECT: counter after 2 dec: 3

    printf("total locks: %d\n", cnt.mtx.lock_count); // EXPECT: total locks: 9

    // Recursive mutex test
    struct RecursiveMutex rm;
    rmutex_init(&rm);

    printf("rlock1: %d\n", rmutex_lock(&rm, 1)); // EXPECT: rlock1: 1
    printf("rlock2: %d\n", rmutex_lock(&rm, 1)); // EXPECT: rlock2: 1
    printf("rlock3: %d\n", rmutex_lock(&rm, 1)); // EXPECT: rlock3: 1
    printf("depth: %d\n", rm.depth); // EXPECT: depth: 3

    printf("rlock other: %d\n", rmutex_lock(&rm, 2)); // EXPECT: rlock other: 0

    printf("runlock1: %d\n", rmutex_unlock(&rm, 1)); // EXPECT: runlock1: 1
    printf("still locked: %d\n", rm.locked); // EXPECT: still locked: 1
    printf("runlock2: %d\n", rmutex_unlock(&rm, 1)); // EXPECT: runlock2: 1
    printf("runlock3: %d\n", rmutex_unlock(&rm, 1)); // EXPECT: runlock3: 1
    printf("final locked: %d\n", rm.locked); // EXPECT: final locked: 0

    return 0;
}
