int printf(const char *fmt, ...);

// Lock-free ring buffer simulation (single-threaded)
// Simulates a SPSC (single-producer single-consumer) ring buffer
// using sequence numbers instead of locks

struct RingBuffer {
    int data[16];
    int sequence[16];
    int capacity;
    int mask;
    int head;     // consumer reads from here
    int tail;     // producer writes here
    int total_writes;
    int total_reads;
    int overflows;
};

void ring_init(struct RingBuffer *rb, int cap) {
    int i;
    rb->capacity = cap;
    rb->mask = cap - 1; // assumes power of 2
    rb->head = 0;
    rb->tail = 0;
    rb->total_writes = 0;
    rb->total_reads = 0;
    rb->overflows = 0;
    for (i = 0; i < 16; i = i + 1) {
        rb->data[i] = 0;
        rb->sequence[i] = i; // initial sequence
    }
}

int ring_size(struct RingBuffer *rb) {
    int diff;
    diff = rb->tail - rb->head;
    if (diff < 0) diff = -diff;
    return diff;
}

int ring_is_empty(struct RingBuffer *rb) {
    return rb->head == rb->tail;
}

int ring_is_full(struct RingBuffer *rb) {
    return ring_size(rb) >= rb->capacity;
}

int ring_push(struct RingBuffer *rb, int val) {
    int slot;
    if (ring_is_full(rb)) {
        rb->overflows = rb->overflows + 1;
        return 0;
    }
    slot = rb->tail & rb->mask;
    rb->data[slot] = val;
    rb->sequence[slot] = rb->tail;
    rb->tail = rb->tail + 1;
    rb->total_writes = rb->total_writes + 1;
    return 1;
}

int ring_pop(struct RingBuffer *rb) {
    int slot;
    int val;
    if (ring_is_empty(rb)) return -999;
    slot = rb->head & rb->mask;
    val = rb->data[slot];
    rb->head = rb->head + 1;
    rb->total_reads = rb->total_reads + 1;
    return val;
}

int ring_peek(struct RingBuffer *rb) {
    int slot;
    if (ring_is_empty(rb)) return -999;
    slot = rb->head & rb->mask;
    return rb->data[slot];
}

// Batch operations
int ring_push_batch(struct RingBuffer *rb, int *vals, int n) {
    int pushed;
    int i;
    pushed = 0;
    for (i = 0; i < n; i = i + 1) {
        if (ring_push(rb, vals[i])) {
            pushed = pushed + 1;
        } else {
            break;
        }
    }
    return pushed;
}

int ring_pop_batch(struct RingBuffer *rb, int *out, int n) {
    int popped;
    int val;
    int i;
    popped = 0;
    for (i = 0; i < n; i = i + 1) {
        val = ring_pop(rb);
        if (val == -999) break;
        out[i] = val;
        popped = popped + 1;
    }
    return popped;
}

// Simulate producer-consumer with ring buffer
int simulate_produce_consume(struct RingBuffer *rb, int rounds) {
    int produced;
    int consumed;
    int val;
    int round;
    int i;
    produced = 0;
    consumed = 0;
    for (round = 0; round < rounds; round = round + 1) {
        // Producer: push 3 items
        for (i = 0; i < 3; i = i + 1) {
            if (ring_push(rb, produced + 1)) {
                produced = produced + 1;
            }
        }
        // Consumer: pop 2 items
        for (i = 0; i < 2; i = i + 1) {
            val = ring_pop(rb);
            if (val != -999) {
                consumed = consumed + 1;
            }
        }
    }
    return consumed;
}

// Check ring buffer integrity
int ring_verify(struct RingBuffer *rb) {
    if (rb->head > rb->tail) return 0;
    if (ring_size(rb) > rb->capacity) return 0;
    if (rb->total_writes - rb->total_reads != ring_size(rb)) return 0;
    return 1;
}

int main() {
    struct RingBuffer rb;
    ring_init(&rb, 8);

    printf("empty: %d\n", ring_is_empty(&rb)); // EXPECT: empty: 1
    printf("full: %d\n", ring_is_full(&rb)); // EXPECT: full: 0
    printf("size: %d\n", ring_size(&rb)); // EXPECT: size: 0

    // Push items
    printf("push 10: %d\n", ring_push(&rb, 10)); // EXPECT: push 10: 1
    printf("push 20: %d\n", ring_push(&rb, 20)); // EXPECT: push 20: 1
    printf("push 30: %d\n", ring_push(&rb, 30)); // EXPECT: push 30: 1
    printf("size: %d\n", ring_size(&rb)); // EXPECT: size: 3
    printf("peek: %d\n", ring_peek(&rb)); // EXPECT: peek: 10

    // Pop items (FIFO)
    printf("pop: %d\n", ring_pop(&rb)); // EXPECT: pop: 10
    printf("pop: %d\n", ring_pop(&rb)); // EXPECT: pop: 20
    printf("size: %d\n", ring_size(&rb)); // EXPECT: size: 1

    // Fill to capacity
    int i;
    for (i = 0; i < 7; i = i + 1) {
        ring_push(&rb, (i + 1) * 100);
    }
    printf("size: %d\n", ring_size(&rb)); // EXPECT: size: 8
    printf("full: %d\n", ring_is_full(&rb)); // EXPECT: full: 1

    // Overflow
    printf("overflow: %d\n", ring_push(&rb, 999)); // EXPECT: overflow: 0
    printf("overflows: %d\n", rb.overflows); // EXPECT: overflows: 1

    // Pop all
    printf("first: %d\n", ring_pop(&rb)); // EXPECT: first: 30
    int j;
    for (j = 0; j < 7; j = j + 1) {
        ring_pop(&rb);
    }
    printf("empty: %d\n", ring_is_empty(&rb)); // EXPECT: empty: 1
    printf("pop empty: %d\n", ring_pop(&rb)); // EXPECT: pop empty: -999

    // Batch operations
    int vals[4];
    vals[0] = 11;
    vals[1] = 22;
    vals[2] = 33;
    vals[3] = 44;
    int pushed = ring_push_batch(&rb, vals, 4);
    printf("batch push: %d\n", pushed); // EXPECT: batch push: 4

    int out[4];
    int popped = ring_pop_batch(&rb, out, 4);
    printf("batch pop: %d\n", popped); // EXPECT: batch pop: 4
    printf("b0: %d\n", out[0]); // EXPECT: b0: 11
    printf("b1: %d\n", out[1]); // EXPECT: b1: 22
    printf("b2: %d\n", out[2]); // EXPECT: b2: 33
    printf("b3: %d\n", out[3]); // EXPECT: b3: 44

    // Verify integrity
    printf("verify: %d\n", ring_verify(&rb)); // EXPECT: verify: 1

    // Producer-consumer simulation
    struct RingBuffer rb2;
    ring_init(&rb2, 8);
    int consumed = simulate_produce_consume(&rb2, 5);
    printf("consumed: %d\n", consumed); // EXPECT: consumed: 10
    printf("rb2 size: %d\n", ring_size(&rb2)); // EXPECT: rb2 size: 5
    printf("rb2 writes: %d\n", rb2.total_writes); // EXPECT: rb2 writes: 15
    printf("rb2 reads: %d\n", rb2.total_reads); // EXPECT: rb2 reads: 10
    printf("rb2 verify: %d\n", ring_verify(&rb2)); // EXPECT: rb2 verify: 1

    printf("total writes: %d\n", rb.total_writes); // EXPECT: total writes: 14
    printf("total reads: %d\n", rb.total_reads); // EXPECT: total reads: 14

    return 0;
}
