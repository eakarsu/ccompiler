int printf(const char *fmt, ...);

// Producer-consumer bounded buffer simulation
// Turn-based (no real threads): producer and consumer alternate

struct BoundedBuffer {
    int data[8];
    int head;     // read position
    int tail;     // write position
    int count;    // items in buffer
    int capacity;
};

void buf_init(struct BoundedBuffer *b, int cap) {
    b->head = 0;
    b->tail = 0;
    b->count = 0;
    b->capacity = cap;
    int i;
    for (i = 0; i < 8; i++) b->data[i] = 0;
}

int buf_is_full(struct BoundedBuffer *b) {
    return b->count == b->capacity;
}

int buf_is_empty(struct BoundedBuffer *b) {
    return b->count == 0;
}

int buf_produce(struct BoundedBuffer *b, int val) {
    if (buf_is_full(b)) return 0;
    b->data[b->tail] = val;
    b->tail = (b->tail + 1) % b->capacity;
    b->count++;
    return 1;
}

int buf_consume(struct BoundedBuffer *b) {
    if (buf_is_empty(b)) return -1;
    int val = b->data[b->head];
    b->head = (b->head + 1) % b->capacity;
    b->count--;
    return val;
}

struct Stats {
    int produced;
    int consumed;
    int produce_blocked;
    int consume_blocked;
};

// Simulate alternating producer/consumer
// Producer produces values: start, start+1, ..., up to 'total' items
// Consumer consumes when available
// Each "tick": producer tries to produce, then consumer tries to consume
void simulate_alternating(struct BoundedBuffer *b, struct Stats *st, int total) {
    int next_val = 1;
    int tick = 0;

    st->produced = 0;
    st->consumed = 0;
    st->produce_blocked = 0;
    st->consume_blocked = 0;

    while (st->consumed < total && tick < 200) {
        // Producer turn
        if (st->produced < total) {
            if (buf_produce(b, next_val)) {
                st->produced++;
                next_val++;
            } else {
                st->produce_blocked++;
            }
        }

        // Consumer turn
        int val = buf_consume(b);
        if (val != -1) {
            st->consumed++;
        } else {
            st->consume_blocked++;
        }

        tick++;
    }
}

// Simulate burst: producer fills buffer, then consumer drains
void simulate_burst(struct BoundedBuffer *b, struct Stats *st, int total) {
    int next_val = 100;

    st->produced = 0;
    st->consumed = 0;
    st->produce_blocked = 0;
    st->consume_blocked = 0;

    // Produce as much as possible
    while (st->produced < total) {
        if (buf_produce(b, next_val)) {
            st->produced++;
            next_val++;
        } else {
            // Buffer full, drain some
            int drain = 0;
            while (!buf_is_empty(b) && drain < 3) {
                int v = buf_consume(b);
                if (v != -1) {
                    st->consumed++;
                    drain++;
                }
            }
        }
    }

    // Drain remaining
    while (!buf_is_empty(b)) {
        int v = buf_consume(b);
        if (v != -1) st->consumed++;
    }
}

int main(void) {
    struct BoundedBuffer buf;
    struct Stats stats;

    // Test 1: Basic produce and consume
    buf_init(&buf, 4);

    printf("=== Basic ops ===\n");
    // EXPECT: === Basic ops ===

    buf_produce(&buf, 10);
    buf_produce(&buf, 20);
    buf_produce(&buf, 30);
    printf("count=%d\n", buf.count);
    // EXPECT: count=3

    int v1 = buf_consume(&buf);
    int v2 = buf_consume(&buf);
    printf("consumed: %d %d\n", v1, v2);
    // EXPECT: consumed: 10 20

    printf("count=%d\n", buf.count);
    // EXPECT: count=1

    // Fill to capacity
    buf_produce(&buf, 40);
    buf_produce(&buf, 50);
    buf_produce(&buf, 60);
    printf("full=%d count=%d\n", buf_is_full(&buf), buf.count);
    // EXPECT: full=1 count=4

    // Try overflow
    int ok = buf_produce(&buf, 70);
    printf("overflow produce: %d\n", ok);
    // EXPECT: overflow produce: 0

    // Drain all
    int v3 = buf_consume(&buf);
    int v4 = buf_consume(&buf);
    int v5 = buf_consume(&buf);
    int v6 = buf_consume(&buf);
    printf("drained: %d %d %d %d\n", v3, v4, v5, v6);
    // EXPECT: drained: 30 40 50 60

    printf("empty=%d\n", buf_is_empty(&buf));
    // EXPECT: empty=1

    // Try underflow
    int uf = buf_consume(&buf);
    printf("underflow consume: %d\n", uf);
    // EXPECT: underflow consume: -1

    // Test 2: Alternating simulation
    printf("=== Alternating sim ===\n");
    // EXPECT: === Alternating sim ===

    buf_init(&buf, 4);
    simulate_alternating(&buf, &stats, 10);

    printf("produced=%d consumed=%d\n", stats.produced, stats.consumed);
    // EXPECT: produced=10 consumed=10

    // With capacity 4 and alternating, producer never blocks
    // because consumer drains 1 each tick
    printf("produce_blocked=%d\n", stats.produce_blocked);
    // EXPECT: produce_blocked=0

    // Consumer blocked on tick 0 (buffer starts empty, consume before first produce settles in same tick?)
    // Actually: tick 0: produce val 1 -> count=1, then consume -> gets 1, count=0
    // tick 1: produce val 2 -> count=1, then consume -> gets 2, count=0
    // ... no blocks at all since produce always happens before consume
    printf("consume_blocked=%d\n", stats.consume_blocked);
    // EXPECT: consume_blocked=0

    // Test 3: Alternating with tiny buffer
    printf("=== Tiny buffer ===\n");
    // EXPECT: === Tiny buffer ===

    buf_init(&buf, 1);
    simulate_alternating(&buf, &stats, 5);
    printf("produced=%d consumed=%d\n", stats.produced, stats.consumed);
    // EXPECT: produced=5 consumed=5
    printf("produce_blocked=%d consume_blocked=%d\n",
           stats.produce_blocked, stats.consume_blocked);
    // EXPECT: produce_blocked=0 consume_blocked=0

    // Test 4: Burst simulation
    printf("=== Burst sim ===\n");
    // EXPECT: === Burst sim ===

    buf_init(&buf, 4);
    simulate_burst(&buf, &stats, 12);

    printf("produced=%d consumed=%d\n", stats.produced, stats.consumed);
    // EXPECT: produced=12 consumed=12

    printf("buffer empty after: %d\n", buf_is_empty(&buf));
    // EXPECT: buffer empty after: 1

    // Test 5: Wrap-around correctness
    printf("=== Wrap-around ===\n");
    // EXPECT: === Wrap-around ===

    buf_init(&buf, 3);
    buf_produce(&buf, 1);
    buf_produce(&buf, 2);
    buf_produce(&buf, 3);
    buf_consume(&buf); // removes 1
    buf_consume(&buf); // removes 2
    buf_produce(&buf, 4);
    buf_produce(&buf, 5);
    // Buffer now: [4, 5, 3] with head at index 2 wrapping
    int w1 = buf_consume(&buf);
    int w2 = buf_consume(&buf);
    int w3 = buf_consume(&buf);
    printf("wrap values: %d %d %d\n", w1, w2, w3);
    // EXPECT: wrap values: 3 4 5

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
