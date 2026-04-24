int printf(const char *fmt, ...);

// Semaphore Simulation: counting semaphore with process queue

struct SemProcess {
    int pid;
    int state;  // 0=ready, 1=running, 2=blocked, 3=done
};

struct Semaphore {
    int value;
    int queue[10];  // blocked pids
    int queue_size;
};

void sem_init(struct Semaphore *sem, int val) {
    sem->value = val;
    sem->queue_size = 0;
}

int sem_wait(struct Semaphore *sem, int pid) {
    sem->value = sem->value - 1;
    if (sem->value < 0) {
        sem->queue[sem->queue_size] = pid;
        sem->queue_size = sem->queue_size + 1;
        printf("P%d: BLOCKED (sem=%d)\n", pid, sem->value);
        return 0; // blocked
    }
    printf("P%d: acquired (sem=%d)\n", pid, sem->value);
    return 1; // acquired
}

int sem_signal(struct Semaphore *sem, int pid) {
    int woken_pid;
    int i;
    sem->value = sem->value + 1;
    if (sem->value <= 0 && sem->queue_size > 0) {
        woken_pid = sem->queue[0];
        // Remove from queue
        for (i = 0; i < sem->queue_size - 1; i = i + 1) {
            sem->queue[i] = sem->queue[i + 1];
        }
        sem->queue_size = sem->queue_size - 1;
        printf("P%d: released, woke P%d (sem=%d)\n", pid, woken_pid, sem->value);
        return woken_pid;
    }
    printf("P%d: released (sem=%d)\n", pid, sem->value);
    return -1;
}

// Producer-Consumer simulation
struct Buffer {
    int data[5];
    int count;
    int in_idx;
    int out_idx;
    int capacity;
};

void buf_init(struct Buffer *buf, int cap) {
    int i;
    buf->count = 0;
    buf->in_idx = 0;
    buf->out_idx = 0;
    buf->capacity = cap;
    for (i = 0; i < cap; i = i + 1) {
        buf->data[i] = 0;
    }
}

int buf_produce(struct Buffer *buf, int item) {
    if (buf->count >= buf->capacity) return 0;
    buf->data[buf->in_idx] = item;
    buf->in_idx = (buf->in_idx + 1);
    if (buf->in_idx >= buf->capacity) buf->in_idx = 0;
    buf->count = buf->count + 1;
    return 1;
}

int buf_consume(struct Buffer *buf) {
    int item;
    if (buf->count <= 0) return -1;
    item = buf->data[buf->out_idx];
    buf->out_idx = (buf->out_idx + 1);
    if (buf->out_idx >= buf->capacity) buf->out_idx = 0;
    buf->count = buf->count - 1;
    return item;
}

int main() {
    struct Semaphore mutex;
    struct Semaphore empty_sem;
    struct Semaphore full_sem;
    struct Buffer buf;
    int item;
    int woken;

    printf("=== Semaphore Operations ===\n"); // EXPECT: === Semaphore Operations ===
    sem_init(&mutex, 1);

    sem_wait(&mutex, 1); // EXPECT: P1: acquired (sem=0)
    sem_wait(&mutex, 2); // EXPECT: P2: BLOCKED (sem=-1)
    sem_wait(&mutex, 3); // EXPECT: P3: BLOCKED (sem=-2)
    sem_signal(&mutex, 1); // EXPECT: P1: released, woke P2 (sem=-1)
    sem_signal(&mutex, 2); // EXPECT: P2: released, woke P3 (sem=0)
    sem_signal(&mutex, 3); // EXPECT: P3: released (sem=1)

    printf("=== Counting Semaphore ===\n"); // EXPECT: === Counting Semaphore ===
    sem_init(&mutex, 3);
    sem_wait(&mutex, 10); // EXPECT: P10: acquired (sem=2)
    sem_wait(&mutex, 11); // EXPECT: P11: acquired (sem=1)
    sem_wait(&mutex, 12); // EXPECT: P12: acquired (sem=0)
    sem_wait(&mutex, 13); // EXPECT: P13: BLOCKED (sem=-1)
    sem_signal(&mutex, 10); // EXPECT: P10: released, woke P13 (sem=0)

    printf("=== Producer-Consumer ===\n"); // EXPECT: === Producer-Consumer ===
    buf_init(&buf, 3);
    sem_init(&empty_sem, 3);  // 3 empty slots
    sem_init(&full_sem, 0);   // 0 full slots

    // Producer produces 3 items
    sem_wait(&empty_sem, 1); // EXPECT: P1: acquired (sem=2)
    buf_produce(&buf, 100);
    printf("Produced: 100, buf_count=%d\n", buf.count); // EXPECT: Produced: 100, buf_count=1
    sem_signal(&full_sem, 1); // EXPECT: P1: released (sem=1)

    sem_wait(&empty_sem, 1); // EXPECT: P1: acquired (sem=1)
    buf_produce(&buf, 200);
    printf("Produced: 200, buf_count=%d\n", buf.count); // EXPECT: Produced: 200, buf_count=2
    sem_signal(&full_sem, 1); // EXPECT: P1: released (sem=2)

    sem_wait(&empty_sem, 1); // EXPECT: P1: acquired (sem=0)
    buf_produce(&buf, 300);
    printf("Produced: 300, buf_count=%d\n", buf.count); // EXPECT: Produced: 300, buf_count=3
    sem_signal(&full_sem, 1); // EXPECT: P1: released (sem=3)

    // Consumer consumes 2 items
    sem_wait(&full_sem, 2); // EXPECT: P2: acquired (sem=2)
    item = buf_consume(&buf);
    printf("Consumed: %d, buf_count=%d\n", item, buf.count); // EXPECT: Consumed: 100, buf_count=2
    sem_signal(&empty_sem, 2); // EXPECT: P2: released (sem=1)

    sem_wait(&full_sem, 2); // EXPECT: P2: acquired (sem=1)
    item = buf_consume(&buf);
    printf("Consumed: %d, buf_count=%d\n", item, buf.count); // EXPECT: Consumed: 200, buf_count=1
    sem_signal(&empty_sem, 2); // EXPECT: P2: released (sem=2)

    printf("Semaphore simulation complete\n"); // EXPECT: Semaphore simulation complete
    return 0;
}
