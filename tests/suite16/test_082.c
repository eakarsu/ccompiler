int printf(const char *fmt, ...);

// Test 082: Circular queue with producer-consumer simulation

struct Message {
    int sender;
    int type;
    int payload;
};

struct CircQueue {
    struct Message msgs[6];
    int head;
    int tail;
    int count;
};

void cq_init(struct CircQueue *q) {
    int i;
    for (i = 0; i < 6; i++) {
        q->msgs[i].sender = 0;
        q->msgs[i].type = 0;
        q->msgs[i].payload = 0;
    }
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

int cq_enqueue(struct CircQueue *q, int sender, int type, int payload) {
    if (q->count >= 6) {
        return 0;
    }
    q->msgs[q->tail].sender = sender;
    q->msgs[q->tail].type = type;
    q->msgs[q->tail].payload = payload;
    q->tail = (q->tail + 1) % 6;
    q->count = q->count + 1;
    return 1;
}

int cq_dequeue(struct CircQueue *q, struct Message *out) {
    if (q->count <= 0) {
        return 0;
    }
    out->sender = q->msgs[q->head].sender;
    out->type = q->msgs[q->head].type;
    out->payload = q->msgs[q->head].payload;
    q->head = (q->head + 1) % 6;
    q->count = q->count - 1;
    return 1;
}

void produce(struct CircQueue *q, int producer_id, int round) {
    int val;
    val = producer_id * 100 + round;
    cq_enqueue(q, producer_id, 1, val);
}

int consume(struct CircQueue *q) {
    struct Message msg;
    if (cq_dequeue(q, &msg)) {
        printf("Consumed: sender=%d type=%d val=%d\n", msg.sender, msg.type, msg.payload);
        return msg.payload;
    }
    return -1;
}

int main() {
    struct CircQueue q;
    struct Message msg;
    int i;
    int total;

    cq_init(&q);

    // Producer 1 sends 3 messages
    produce(&q, 1, 0);
    produce(&q, 1, 1);
    produce(&q, 1, 2);
    printf("After P1: count=%d\n", q.count); // EXPECT: After P1: count=3

    // Producer 2 sends 2 messages
    produce(&q, 2, 0);
    produce(&q, 2, 1);
    printf("After P2: count=%d\n", q.count); // EXPECT: After P2: count=5

    // Consumer reads first 3 (consume prints Consumed: lines)
    consume(&q); // EXPECT: Consumed: sender=1 type=1 val=100
    printf("---\n"); // EXPECT: ---
    consume(&q); // EXPECT: Consumed: sender=1 type=1 val=101
    printf("---\n"); // EXPECT: ---
    consume(&q); // EXPECT: Consumed: sender=1 type=1 val=102
    printf("After consume 3: count=%d\n", q.count); // EXPECT: After consume 3: count=2

    // Producer 3 fills remaining slots
    produce(&q, 3, 0);
    produce(&q, 3, 1);
    produce(&q, 3, 2);
    produce(&q, 3, 3);
    printf("After P3: count=%d\n", q.count); // EXPECT: After P3: count=6

    // Try to overflow
    i = cq_enqueue(&q, 9, 9, 999);
    printf("Overflow attempt: %d\n", i); // EXPECT: Overflow attempt: 0

    // Drain all
    total = 0;
    while (q.count > 0) {
        cq_dequeue(&q, &msg);
        total = total + msg.payload;
    }
    printf("Total drained: %d\n", total); // EXPECT: Total drained: 1607

    // Interleave produce/consume, simulating real-time
    for (i = 0; i < 10; i++) {
        cq_enqueue(&q, i % 3, 2, i * 10);
        if (i % 2 == 1) {
            cq_dequeue(&q, &msg);
        }
    }
    printf("After interleave: count=%d\n", q.count); // EXPECT: After interleave: count=5

    // Drain and sum
    total = 0;
    while (q.count > 0) {
        cq_dequeue(&q, &msg);
        total = total + msg.payload;
    }
    printf("Interleave sum: %d\n", total); // EXPECT: Interleave sum: 350

    printf("PASS\n"); // EXPECT: PASS
    return 0;
}
