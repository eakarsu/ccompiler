int printf(const char *fmt, ...);

// Ring buffer for producer-consumer pattern
// Fixed-size queue with separate read/write positions

struct RingBuffer {
    int buffer[16];
    int read_pos;
    int write_pos;
    int capacity;
    int used;
    int total_produced;
    int total_consumed;
};

void ring_init(struct RingBuffer *rb) {
    int i;
    rb->read_pos = 0;
    rb->write_pos = 0;
    rb->capacity = 16;
    rb->used = 0;
    rb->total_produced = 0;
    rb->total_consumed = 0;
    for (i = 0; i < 16; i++) {
        rb->buffer[i] = 0;
    }
}

int ring_produce(struct RingBuffer *rb, int val) {
    if (rb->used >= rb->capacity) return 0; // full
    rb->buffer[rb->write_pos] = val;
    rb->write_pos = (rb->write_pos + 1) % rb->capacity;
    rb->used = rb->used + 1;
    rb->total_produced = rb->total_produced + 1;
    return 1;
}

int ring_consume(struct RingBuffer *rb, int *val) {
    if (rb->used <= 0) return 0; // empty
    *val = rb->buffer[rb->read_pos];
    rb->read_pos = (rb->read_pos + 1) % rb->capacity;
    rb->used = rb->used - 1;
    rb->total_consumed = rb->total_consumed + 1;
    return 1;
}

int ring_available(struct RingBuffer *rb) {
    return rb->used;
}

int ring_free_space(struct RingBuffer *rb) {
    return rb->capacity - rb->used;
}

// Produce multiple items, returns how many were actually produced
int ring_produce_batch(struct RingBuffer *rb, int *vals, int n) {
    int i;
    int produced;
    produced = 0;
    for (i = 0; i < n; i++) {
        if (!ring_produce(rb, vals[i])) break;
        produced = produced + 1;
    }
    return produced;
}

// Consume multiple items, returns sum of consumed values
int ring_consume_sum(struct RingBuffer *rb, int n) {
    int sum;
    int val;
    int i;
    sum = 0;
    for (i = 0; i < n; i++) {
        if (!ring_consume(rb, &val)) break;
        sum = sum + val;
    }
    return sum;
}

// Peek at front without consuming
int ring_peek_front(struct RingBuffer *rb) {
    if (rb->used == 0) return -1;
    return rb->buffer[rb->read_pos];
}

// Simulate a producer-consumer pattern: produce n items, consume half
void simulate_round(struct RingBuffer *rb, int start, int n) {
    int i;
    int val;
    for (i = 0; i < n; i++) {
        ring_produce(rb, start + i);
    }
    for (i = 0; i < n / 2; i++) {
        ring_consume(rb, &val);
    }
}

int main() {
    struct RingBuffer rb;
    int val;
    int ok;
    int batch[5];
    int produced;
    int sum;

    ring_init(&rb);

    printf("Initial available: %d\n", ring_available(&rb));
    // EXPECT: Initial available: 0

    printf("Initial free: %d\n", ring_free_space(&rb));
    // EXPECT: Initial free: 16

    // Produce some items
    ok = ring_produce(&rb, 100);
    printf("Produce 100: %d\n", ok);
    // EXPECT: Produce 100: 1

    ring_produce(&rb, 200);
    ring_produce(&rb, 300);

    printf("Available: %d\n", ring_available(&rb));
    // EXPECT: Available: 3

    printf("Peek: %d\n", ring_peek_front(&rb));
    // EXPECT: Peek: 100

    // Consume
    ok = ring_consume(&rb, &val);
    printf("Consumed: %d, val: %d\n", ok, val);
    // EXPECT: Consumed: 1, val: 100

    ring_consume(&rb, &val);
    printf("Next val: %d\n", val);
    // EXPECT: Next val: 200

    printf("Available after 2 consumes: %d\n", ring_available(&rb));
    // EXPECT: Available after 2 consumes: 1

    // Batch produce
    batch[0] = 10;
    batch[1] = 20;
    batch[2] = 30;
    batch[3] = 40;
    batch[4] = 50;
    produced = ring_produce_batch(&rb, batch, 5);
    printf("Batch produced: %d\n", produced);
    // EXPECT: Batch produced: 5

    printf("Available now: %d\n", ring_available(&rb));
    // EXPECT: Available now: 6

    // Consume sum of 3
    sum = ring_consume_sum(&rb, 3);
    printf("Sum of 3 consumed: %d\n", sum);
    // EXPECT: Sum of 3 consumed: 330

    printf("Available after sum consume: %d\n", ring_available(&rb));
    // EXPECT: Available after sum consume: 3

    // Fill to capacity
    ring_init(&rb);
    {
        int i;
        for (i = 0; i < 16; i++) {
            ring_produce(&rb, i * 10);
        }
    }
    printf("Full available: %d\n", ring_available(&rb));
    // EXPECT: Full available: 16

    printf("Free when full: %d\n", ring_free_space(&rb));
    // EXPECT: Free when full: 0

    // Try to produce when full
    ok = ring_produce(&rb, 999);
    printf("Produce when full: %d\n", ok);
    // EXPECT: Produce when full: 0

    // Consume some to make space
    ring_consume(&rb, &val);
    printf("First consumed from full: %d\n", val);
    // EXPECT: First consumed from full: 0

    ring_consume(&rb, &val);
    printf("Second consumed: %d\n", val);
    // EXPECT: Second consumed: 10

    printf("Total produced: %d\n", rb.total_produced);
    // EXPECT: Total produced: 16

    printf("Total consumed: %d\n", rb.total_consumed);
    // EXPECT: Total consumed: 2

    // Simulation round
    ring_init(&rb);
    simulate_round(&rb, 1, 8);
    printf("After round: available: %d\n", ring_available(&rb));
    // EXPECT: After round: available: 4

    printf("After round: peek: %d\n", ring_peek_front(&rb));
    // EXPECT: After round: peek: 5

    simulate_round(&rb, 100, 6);
    printf("After round 2: available: %d\n", ring_available(&rb));
    // EXPECT: After round 2: available: 7

    printf("Total produced: %d\n", rb.total_produced);
    // EXPECT: Total produced: 14

    printf("Total consumed: %d\n", rb.total_consumed);
    // EXPECT: Total consumed: 7

    return 0;
}
