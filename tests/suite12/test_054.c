int printf(const char *fmt, ...);

// Barrier synchronization simulation (single-threaded)
// Simulates threads arriving at a barrier and proceeding together

struct Barrier {
    int threshold;
    int count;
    int generation;
    int total_waits;
};

void barrier_init(struct Barrier *b, int n) {
    b->threshold = n;
    b->count = 0;
    b->generation = 0;
    b->total_waits = 0;
}

// Returns 1 if this thread is the last to arrive (releases barrier)
// Returns 0 if still waiting for others
int barrier_wait(struct Barrier *b, int tid) {
    b->count = b->count + 1;
    b->total_waits = b->total_waits + 1;
    if (b->count >= b->threshold) {
        b->count = 0;
        b->generation = b->generation + 1;
        return 1;
    }
    return 0;
}

void barrier_reset(struct Barrier *b) {
    b->count = 0;
}

// Phased barrier: tracks which phase we are in
struct PhasedBarrier {
    struct Barrier barriers[4];
    int current_phase;
    int num_phases;
    int thread_count;
};

void phased_init(struct PhasedBarrier *pb, int threads, int phases) {
    int i;
    pb->thread_count = threads;
    pb->num_phases = phases;
    pb->current_phase = 0;
    for (i = 0; i < 4; i = i + 1) {
        barrier_init(&pb->barriers[i], threads);
    }
}

int phased_arrive(struct PhasedBarrier *pb, int tid) {
    int phase;
    int result;
    phase = pb->current_phase;
    if (phase >= pb->num_phases) return -1;
    result = barrier_wait(&pb->barriers[phase], tid);
    if (result) {
        pb->current_phase = pb->current_phase + 1;
    }
    return result;
}

// Tournament barrier simulation
struct TournamentBarrier {
    int arrived[8];
    int num_threads;
    int rounds_completed;
};

void tourney_init(struct TournamentBarrier *tb, int n) {
    int i;
    tb->num_threads = n;
    tb->rounds_completed = 0;
    for (i = 0; i < 8; i = i + 1) {
        tb->arrived[i] = 0;
    }
}

int tourney_arrive(struct TournamentBarrier *tb, int tid) {
    int all_arrived;
    int i;
    if (tid >= tb->num_threads) return -1;
    tb->arrived[tid] = 1;
    all_arrived = 1;
    for (i = 0; i < tb->num_threads; i = i + 1) {
        if (!tb->arrived[i]) {
            all_arrived = 0;
        }
    }
    if (all_arrived) {
        for (i = 0; i < tb->num_threads; i = i + 1) {
            tb->arrived[i] = 0;
        }
        tb->rounds_completed = tb->rounds_completed + 1;
        return 1;
    }
    return 0;
}

// Simulate parallel computation with barrier sync
int compute_partial(int tid, int phase) {
    return (tid + 1) * (phase + 1) * 10;
}

int main() {
    struct Barrier b;
    barrier_init(&b, 3);

    // Three "threads" arrive at barrier
    printf("t0 arrive: %d\n", barrier_wait(&b, 0)); // EXPECT: t0 arrive: 0
    printf("t1 arrive: %d\n", barrier_wait(&b, 1)); // EXPECT: t1 arrive: 0
    printf("t2 arrive: %d\n", barrier_wait(&b, 2)); // EXPECT: t2 arrive: 1
    printf("generation: %d\n", b.generation); // EXPECT: generation: 1
    printf("count reset: %d\n", b.count); // EXPECT: count reset: 0

    // Second round
    printf("t0 arrive2: %d\n", barrier_wait(&b, 0)); // EXPECT: t0 arrive2: 0
    printf("t1 arrive2: %d\n", barrier_wait(&b, 1)); // EXPECT: t1 arrive2: 0
    printf("t2 arrive2: %d\n", barrier_wait(&b, 2)); // EXPECT: t2 arrive2: 1
    printf("generation: %d\n", b.generation); // EXPECT: generation: 2
    printf("total waits: %d\n", b.total_waits); // EXPECT: total waits: 6

    // Phased barrier test
    struct PhasedBarrier pb;
    phased_init(&pb, 2, 3);

    printf("phase: %d\n", pb.current_phase); // EXPECT: phase: 0
    printf("p0t0: %d\n", phased_arrive(&pb, 0)); // EXPECT: p0t0: 0
    printf("p0t1: %d\n", phased_arrive(&pb, 1)); // EXPECT: p0t1: 1
    printf("phase: %d\n", pb.current_phase); // EXPECT: phase: 1

    printf("p1t0: %d\n", phased_arrive(&pb, 0)); // EXPECT: p1t0: 0
    printf("p1t1: %d\n", phased_arrive(&pb, 1)); // EXPECT: p1t1: 1
    printf("phase: %d\n", pb.current_phase); // EXPECT: phase: 2

    printf("p2t0: %d\n", phased_arrive(&pb, 0)); // EXPECT: p2t0: 0
    printf("p2t1: %d\n", phased_arrive(&pb, 1)); // EXPECT: p2t1: 1
    printf("phase: %d\n", pb.current_phase); // EXPECT: phase: 3

    printf("done: %d\n", phased_arrive(&pb, 0)); // EXPECT: done: -1

    // Tournament barrier
    struct TournamentBarrier tb;
    tourney_init(&tb, 4);

    printf("tt0: %d\n", tourney_arrive(&tb, 0)); // EXPECT: tt0: 0
    printf("tt1: %d\n", tourney_arrive(&tb, 1)); // EXPECT: tt1: 0
    printf("tt2: %d\n", tourney_arrive(&tb, 2)); // EXPECT: tt2: 0
    printf("tt3: %d\n", tourney_arrive(&tb, 3)); // EXPECT: tt3: 1
    printf("rounds: %d\n", tb.rounds_completed); // EXPECT: rounds: 1

    // Invalid thread id
    printf("invalid: %d\n", tourney_arrive(&tb, 10)); // EXPECT: invalid: -1

    // Compute with barrier sync
    int results[4];
    int total;
    int i;
    int phase;

    total = 0;
    for (phase = 0; phase < 2; phase = phase + 1) {
        for (i = 0; i < 4; i = i + 1) {
            results[i] = compute_partial(i, phase);
        }
        for (i = 0; i < 4; i = i + 1) {
            total = total + results[i];
        }
    }
    printf("total compute: %d\n", total); // EXPECT: total compute: 300

    return 0;
}
