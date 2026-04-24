int printf(const char *fmt, ...);

// Reader-Writer Lock simulation (single-threaded)
// Multiple readers can hold the lock, but writers are exclusive

struct RWLock {
    int readers;
    int writer;
    int writer_id;
    int read_acquires;
    int write_acquires;
    int waiting_writers;
    int waiting_readers;
};

void rwlock_init(struct RWLock *rw) {
    rw->readers = 0;
    rw->writer = 0;
    rw->writer_id = -1;
    rw->read_acquires = 0;
    rw->write_acquires = 0;
    rw->waiting_writers = 0;
    rw->waiting_readers = 0;
}

int rwlock_read_lock(struct RWLock *rw, int tid) {
    if (rw->writer) {
        rw->waiting_readers = rw->waiting_readers + 1;
        return 0;
    }
    rw->readers = rw->readers + 1;
    rw->read_acquires = rw->read_acquires + 1;
    return 1;
}

int rwlock_read_unlock(struct RWLock *rw, int tid) {
    if (rw->readers <= 0) return 0;
    rw->readers = rw->readers - 1;
    return 1;
}

int rwlock_write_lock(struct RWLock *rw, int tid) {
    if (rw->writer || rw->readers > 0) {
        rw->waiting_writers = rw->waiting_writers + 1;
        return 0;
    }
    rw->writer = 1;
    rw->writer_id = tid;
    rw->write_acquires = rw->write_acquires + 1;
    return 1;
}

int rwlock_write_unlock(struct RWLock *rw, int tid) {
    if (!rw->writer || rw->writer_id != tid) return 0;
    rw->writer = 0;
    rw->writer_id = -1;
    return 1;
}

// Shared data protected by RW lock
struct SharedData {
    int values[8];
    int version;
    struct RWLock lock;
};

void shared_init(struct SharedData *sd) {
    int i;
    for (i = 0; i < 8; i = i + 1) {
        sd->values[i] = 0;
    }
    sd->version = 0;
    rwlock_init(&sd->lock);
}

int shared_read(struct SharedData *sd, int idx, int tid) {
    int val;
    if (!rwlock_read_lock(&sd->lock, tid)) return -1;
    val = sd->values[idx];
    rwlock_read_unlock(&sd->lock, tid);
    return val;
}

int shared_write(struct SharedData *sd, int idx, int val, int tid) {
    if (!rwlock_write_lock(&sd->lock, tid)) return 0;
    sd->values[idx] = val;
    sd->version = sd->version + 1;
    rwlock_write_unlock(&sd->lock, tid);
    return 1;
}

int shared_sum(struct SharedData *sd, int tid) {
    int sum;
    int i;
    if (!rwlock_read_lock(&sd->lock, tid)) return -1;
    sum = 0;
    for (i = 0; i < 8; i = i + 1) {
        sum = sum + sd->values[i];
    }
    rwlock_read_unlock(&sd->lock, tid);
    return sum;
}

// Simulate reader-writer scenario step by step
int simulate_step(struct RWLock *rw, int action, int tid) {
    if (action == 0) return rwlock_read_lock(rw, tid);
    if (action == 1) return rwlock_read_unlock(rw, tid);
    if (action == 2) return rwlock_write_lock(rw, tid);
    if (action == 3) return rwlock_write_unlock(rw, tid);
    return -1;
}

int main() {
    struct RWLock rw;
    rwlock_init(&rw);

    // Multiple readers can acquire
    printf("r1 read: %d\n", rwlock_read_lock(&rw, 1)); // EXPECT: r1 read: 1
    printf("r2 read: %d\n", rwlock_read_lock(&rw, 2)); // EXPECT: r2 read: 1
    printf("r3 read: %d\n", rwlock_read_lock(&rw, 3)); // EXPECT: r3 read: 1
    printf("readers: %d\n", rw.readers); // EXPECT: readers: 3

    // Writer blocked while readers hold lock
    printf("w1 write: %d\n", rwlock_write_lock(&rw, 10)); // EXPECT: w1 write: 0
    printf("waiting writers: %d\n", rw.waiting_writers); // EXPECT: waiting writers: 1

    // Release all readers
    rwlock_read_unlock(&rw, 1);
    rwlock_read_unlock(&rw, 2);
    rwlock_read_unlock(&rw, 3);
    printf("readers after unlock: %d\n", rw.readers); // EXPECT: readers after unlock: 0

    // Now writer can acquire
    printf("w1 write: %d\n", rwlock_write_lock(&rw, 10)); // EXPECT: w1 write: 1

    // Reader blocked while writer holds lock
    printf("r1 read: %d\n", rwlock_read_lock(&rw, 1)); // EXPECT: r1 read: 0
    printf("waiting readers: %d\n", rw.waiting_readers); // EXPECT: waiting readers: 1

    // Another writer also blocked
    printf("w2 write: %d\n", rwlock_write_lock(&rw, 20)); // EXPECT: w2 write: 0

    // Release writer
    printf("w1 unlock: %d\n", rwlock_write_unlock(&rw, 10)); // EXPECT: w1 unlock: 1

    // Wrong owner cannot unlock
    rwlock_write_lock(&rw, 30);
    printf("bad unlock: %d\n", rwlock_write_unlock(&rw, 99)); // EXPECT: bad unlock: 0
    rwlock_write_unlock(&rw, 30);

    // SharedData test
    struct SharedData sd;
    shared_init(&sd);

    int i;
    for (i = 0; i < 8; i = i + 1) {
        shared_write(&sd, i, (i + 1) * 5, 0);
    }
    printf("version: %d\n", sd.version); // EXPECT: version: 8

    printf("val[0]: %d\n", shared_read(&sd, 0, 1)); // EXPECT: val[0]: 5
    printf("val[3]: %d\n", shared_read(&sd, 3, 1)); // EXPECT: val[3]: 20
    printf("val[7]: %d\n", shared_read(&sd, 7, 1)); // EXPECT: val[7]: 40

    int s = shared_sum(&sd, 2);
    printf("sum: %d\n", s); // EXPECT: sum: 180

    printf("read acquires: %d\n", sd.lock.read_acquires); // EXPECT: read acquires: 4
    printf("write acquires: %d\n", sd.lock.write_acquires); // EXPECT: write acquires: 8

    // Simulate scenario
    struct RWLock rw2;
    rwlock_init(&rw2);
    printf("sim read: %d\n", simulate_step(&rw2, 0, 1)); // EXPECT: sim read: 1
    printf("sim write: %d\n", simulate_step(&rw2, 2, 2)); // EXPECT: sim write: 0
    printf("sim runlock: %d\n", simulate_step(&rw2, 1, 1)); // EXPECT: sim runlock: 1
    printf("sim write2: %d\n", simulate_step(&rw2, 2, 2)); // EXPECT: sim write2: 1
    printf("sim wunlock: %d\n", simulate_step(&rw2, 3, 2)); // EXPECT: sim wunlock: 1

    return 0;
}
