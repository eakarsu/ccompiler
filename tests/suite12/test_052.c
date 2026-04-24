int printf(const char *fmt, ...);

// Condition variable simulation (single-threaded)
// Simulates wait/signal/broadcast on a condition variable

struct CondVar {
    int waiters[8];
    int waiter_count;
    int signal_count;
    int broadcast_count;
};

void cond_init(struct CondVar *cv) {
    int i;
    for (i = 0; i < 8; i = i + 1) {
        cv->waiters[i] = -1;
    }
    cv->waiter_count = 0;
    cv->signal_count = 0;
    cv->broadcast_count = 0;
}

int cond_wait(struct CondVar *cv, int thread_id) {
    if (cv->waiter_count >= 8) return 0;
    cv->waiters[cv->waiter_count] = thread_id;
    cv->waiter_count = cv->waiter_count + 1;
    return 1;
}

int cond_signal(struct CondVar *cv) {
    int woken;
    int i;
    if (cv->waiter_count == 0) return -1;
    woken = cv->waiters[0];
    // Shift waiters left
    for (i = 0; i < cv->waiter_count - 1; i = i + 1) {
        cv->waiters[i] = cv->waiters[i + 1];
    }
    cv->waiters[cv->waiter_count - 1] = -1;
    cv->waiter_count = cv->waiter_count - 1;
    cv->signal_count = cv->signal_count + 1;
    return woken;
}

int cond_broadcast(struct CondVar *cv) {
    int count;
    int i;
    count = cv->waiter_count;
    for (i = 0; i < 8; i = i + 1) {
        cv->waiters[i] = -1;
    }
    cv->waiter_count = 0;
    cv->broadcast_count = cv->broadcast_count + 1;
    return count;
}

// Producer-consumer simulation with condvar
struct BoundedBuffer {
    int data[8];
    int head;
    int tail;
    int count;
    int capacity;
    struct CondVar not_full;
    struct CondVar not_empty;
};

void buffer_init(struct BoundedBuffer *b) {
    int i;
    for (i = 0; i < 8; i = i + 1) {
        b->data[i] = 0;
    }
    b->head = 0;
    b->tail = 0;
    b->count = 0;
    b->capacity = 8;
    cond_init(&b->not_full);
    cond_init(&b->not_empty);
}

int buffer_produce(struct BoundedBuffer *b, int val, int tid) {
    if (b->count >= b->capacity) {
        cond_wait(&b->not_full, tid);
        return 0;
    }
    b->data[b->tail] = val;
    b->tail = (b->tail + 1) % b->capacity;
    b->count = b->count + 1;
    cond_signal(&b->not_empty);
    return 1;
}

int buffer_consume(struct BoundedBuffer *b, int tid) {
    int val;
    if (b->count <= 0) {
        cond_wait(&b->not_empty, tid);
        return -1;
    }
    val = b->data[b->head];
    b->head = (b->head + 1) % b->capacity;
    b->count = b->count - 1;
    cond_signal(&b->not_full);
    return val;
}

// Monitor pattern: shared queue with condition
struct Monitor {
    int ready;
    struct CondVar cond;
    int value;
};

void monitor_init(struct Monitor *mon) {
    mon->ready = 0;
    cond_init(&mon->cond);
    mon->value = 0;
}

int monitor_wait_ready(struct Monitor *mon, int tid) {
    if (!mon->ready) {
        cond_wait(&mon->cond, tid);
        return 0;
    }
    return mon->value;
}

void monitor_set_ready(struct Monitor *mon, int val) {
    mon->value = val;
    mon->ready = 1;
    cond_broadcast(&mon->cond);
}

int main() {
    struct CondVar cv;
    cond_init(&cv);

    // Test basic wait/signal
    cond_wait(&cv, 10);
    cond_wait(&cv, 20);
    cond_wait(&cv, 30);
    printf("waiters: %d\n", cv.waiter_count); // EXPECT: waiters: 3

    int woken = cond_signal(&cv);
    printf("woken: %d\n", woken); // EXPECT: woken: 10
    printf("waiters after signal: %d\n", cv.waiter_count); // EXPECT: waiters after signal: 2

    woken = cond_signal(&cv);
    printf("woken: %d\n", woken); // EXPECT: woken: 20

    // Test broadcast
    cond_wait(&cv, 40);
    cond_wait(&cv, 50);
    printf("waiters before broadcast: %d\n", cv.waiter_count); // EXPECT: waiters before broadcast: 3
    int bc = cond_broadcast(&cv);
    printf("broadcast woke: %d\n", bc); // EXPECT: broadcast woke: 3
    printf("waiters after broadcast: %d\n", cv.waiter_count); // EXPECT: waiters after broadcast: 0

    int empty = cond_signal(&cv);
    printf("signal empty: %d\n", empty); // EXPECT: signal empty: -1

    // Producer-consumer test
    struct BoundedBuffer buf;
    buffer_init(&buf);

    int i;
    for (i = 0; i < 5; i = i + 1) {
        buffer_produce(&buf, (i + 1) * 10, 0);
    }
    printf("buf count: %d\n", buf.count); // EXPECT: buf count: 5

    for (i = 0; i < 3; i = i + 1) {
        int v = buffer_consume(&buf, 1);
        printf("consumed: %d\n", v); // EXPECT: consumed: 10
        // EXPECT: consumed: 20
        // EXPECT: consumed: 30
    }
    printf("buf count after: %d\n", buf.count); // EXPECT: buf count after: 2

    // Monitor test
    struct Monitor mon;
    monitor_init(&mon);

    int r = monitor_wait_ready(&mon, 5);
    printf("not ready: %d\n", r); // EXPECT: not ready: 0

    monitor_set_ready(&mon, 42);
    r = monitor_wait_ready(&mon, 5);
    printf("ready val: %d\n", r); // EXPECT: ready val: 42

    printf("signals: %d\n", cv.signal_count); // EXPECT: signals: 2
    printf("broadcasts: %d\n", cv.broadcast_count); // EXPECT: broadcasts: 1

    return 0;
}
