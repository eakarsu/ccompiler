int printf(const char *fmt, ...);

// Test 088: Bounded buffer with multiple producers and consumers simulation

struct Task {
    int id;
    int producer;
    int work;
};

struct BoundedBuf {
    struct Task tasks[5];
    int head;
    int tail;
    int count;
    int total_produced;
    int total_consumed;
};

void bb_init(struct BoundedBuf *bb) {
    int i;
    for (i = 0; i < 5; i++) {
        bb->tasks[i].id = 0;
        bb->tasks[i].producer = 0;
        bb->tasks[i].work = 0;
    }
    bb->head = 0;
    bb->tail = 0;
    bb->count = 0;
    bb->total_produced = 0;
    bb->total_consumed = 0;
}

int bb_produce(struct BoundedBuf *bb, int prod_id, int work) {
    if (bb->count >= 5) return 0;
    bb->total_produced = bb->total_produced + 1;
    bb->tasks[bb->tail].id = bb->total_produced;
    bb->tasks[bb->tail].producer = prod_id;
    bb->tasks[bb->tail].work = work;
    bb->tail = (bb->tail + 1) % 5;
    bb->count = bb->count + 1;
    return bb->total_produced;
}

int bb_consume(struct BoundedBuf *bb, struct Task *out) {
    if (bb->count <= 0) return 0;
    out->id = bb->tasks[bb->head].id;
    out->producer = bb->tasks[bb->head].producer;
    out->work = bb->tasks[bb->head].work;
    bb->head = (bb->head + 1) % 5;
    bb->count = bb->count - 1;
    bb->total_consumed = bb->total_consumed + 1;
    return 1;
}

int process_task(struct Task *t) {
    // Simulate work: returns work * id
    return t->work * t->id;
}

int main() {
    struct BoundedBuf bb;
    struct Task t;
    int result;
    int total_work;
    int i;
    int id;

    bb_init(&bb);
    printf("Init: produced=%d consumed=%d\n", bb.total_produced, bb.total_consumed); // EXPECT: Init: produced=0 consumed=0

    // Producer 1 produces 3 tasks
    id = bb_produce(&bb, 1, 10);
    printf("Produced task %d\n", id); // EXPECT: Produced task 1
    id = bb_produce(&bb, 1, 20);
    printf("Produced task %d\n", id); // EXPECT: Produced task 2
    id = bb_produce(&bb, 1, 30);
    printf("Produced task %d\n", id); // EXPECT: Produced task 3

    // Producer 2 produces 2 tasks
    id = bb_produce(&bb, 2, 15);
    printf("Produced task %d\n", id); // EXPECT: Produced task 4
    id = bb_produce(&bb, 2, 25);
    printf("Produced task %d\n", id); // EXPECT: Produced task 5

    printf("Buffer full: count=%d\n", bb.count); // EXPECT: Buffer full: count=5

    // Try to produce when full
    id = bb_produce(&bb, 1, 99);
    printf("Produce when full: %d\n", id); // EXPECT: Produce when full: 0

    // Consumer 1 consumes 2 tasks
    bb_consume(&bb, &t);
    result = process_task(&t);
    printf("C1 got task %d from P%d: result=%d\n", t.id, t.producer, result); // EXPECT: C1 got task 1 from P1: result=10
    bb_consume(&bb, &t);
    result = process_task(&t);
    printf("C1 got task %d from P%d: result=%d\n", t.id, t.producer, result); // EXPECT: C1 got task 2 from P1: result=40

    printf("After C1: count=%d consumed=%d\n", bb.count, bb.total_consumed); // EXPECT: After C1: count=3 consumed=2

    // Producer 1 adds 2 more (wraps around)
    bb_produce(&bb, 1, 40);
    bb_produce(&bb, 1, 50);
    printf("After more produce: count=%d\n", bb.count); // EXPECT: After more produce: count=5

    // Consumer 2 drains everything
    total_work = 0;
    while (bb.count > 0) {
        bb_consume(&bb, &t);
        total_work = total_work + process_task(&t);
    }
    printf("C2 total work: %d\n", total_work); // EXPECT: C2 total work: 865

    printf("Stats: produced=%d consumed=%d\n", bb.total_produced, bb.total_consumed); // EXPECT: Stats: produced=7 consumed=7

    // Simulation: 3 producers take turns, consumer drains periodically
    total_work = 0;
    for (i = 0; i < 15; i++) {
        bb_produce(&bb, (i % 3) + 1, (i + 1) * 2);
        if (bb.count >= 3) {
            bb_consume(&bb, &t);
            total_work = total_work + t.work;
        }
    }
    // Drain remaining
    while (bb.count > 0) {
        bb_consume(&bb, &t);
        total_work = total_work + t.work;
    }
    printf("Simulation total: %d\n", total_work); // EXPECT: Simulation total: 240

    printf("Final: produced=%d consumed=%d\n", bb.total_produced, bb.total_consumed); // EXPECT: Final: produced=22 consumed=22

    printf("PASS\n"); // EXPECT: PASS
    return 0;
}
