// EXPECT: === Enqueue & Dequeue ===
// EXPECT: enqueued: 10 20 30
// EXPECT: dequeue: 10
// EXPECT: dequeue: 20
// EXPECT: dequeue: 30
// EXPECT: === Front ===
// EXPECT: front: 100
// EXPECT: front after dequeue: 200
// EXPECT: === isEmpty ===
// EXPECT: empty at start: yes
// EXPECT: after enqueue: no
// EXPECT: after dequeue all: yes
// EXPECT: === Queue Size ===
// EXPECT: size: 4
// EXPECT: after 2 dequeues: 2
// EXPECT: === Circular Wrap ===
// EXPECT: after wraparound: 6 7 8 9 10
// EXPECT: === FIFO Order ===
// EXPECT: fifo: 1 2 3 4 5 6 7 8
// EXPECT: === Full Queue ===
// EXPECT: filled 8 items
// EXPECT: drained: 0 1 2 3 4 5 6 7
// EXPECT: === Interleaved Ops ===
// EXPECT: result: 1 2 3 4 5 6
// EXPECT: === Queue Copy ===
// EXPECT: copy: 10 20 30

int printf(const char *fmt, ...);

struct Queue {
    int data[8];
    int head;
    int tail;
    int count;
    int capacity;
};

void queue_init(struct Queue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    q->capacity = 8;
}

int queue_empty(struct Queue *q) {
    return q->count == 0;
}

int queue_full(struct Queue *q) {
    return q->count == q->capacity;
}

int queue_size(struct Queue *q) {
    return q->count;
}

int queue_enqueue(struct Queue *q, int val) {
    if (queue_full(q)) return 0;
    q->data[q->tail] = val;
    q->tail = (q->tail + 1) % q->capacity;
    q->count = q->count + 1;
    return 1;
}

int queue_dequeue(struct Queue *q) {
    int val = q->data[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->count = q->count - 1;
    return val;
}

int queue_front(struct Queue *q) {
    return q->data[q->head];
}

void test_enqueue_dequeue(void) {
    printf("=== Enqueue & Dequeue ===\n");
    struct Queue q;
    queue_init(&q);
    queue_enqueue(&q, 10);
    queue_enqueue(&q, 20);
    queue_enqueue(&q, 30);
    printf("enqueued: 10 20 30\n");
    printf("dequeue: %d\n", queue_dequeue(&q));
    printf("dequeue: %d\n", queue_dequeue(&q));
    printf("dequeue: %d\n", queue_dequeue(&q));
}

void test_front(void) {
    printf("=== Front ===\n");
    struct Queue q;
    queue_init(&q);
    queue_enqueue(&q, 100);
    queue_enqueue(&q, 200);
    printf("front: %d\n", queue_front(&q));
    queue_dequeue(&q);
    printf("front after dequeue: %d\n", queue_front(&q));
}

void test_empty(void) {
    printf("=== isEmpty ===\n");
    struct Queue q;
    queue_init(&q);
    if (queue_empty(&q)) printf("empty at start: yes\n");
    else printf("empty at start: no\n");
    queue_enqueue(&q, 1);
    if (queue_empty(&q)) printf("after enqueue: yes\n");
    else printf("after enqueue: no\n");
    queue_dequeue(&q);
    if (queue_empty(&q)) printf("after dequeue all: yes\n");
    else printf("after dequeue all: no\n");
}

void test_size(void) {
    printf("=== Queue Size ===\n");
    struct Queue q;
    queue_init(&q);
    queue_enqueue(&q, 1);
    queue_enqueue(&q, 2);
    queue_enqueue(&q, 3);
    queue_enqueue(&q, 4);
    printf("size: %d\n", queue_size(&q));
    queue_dequeue(&q);
    queue_dequeue(&q);
    printf("after 2 dequeues: %d\n", queue_size(&q));
}

void test_circular_wrap(void) {
    printf("=== Circular Wrap ===\n");
    struct Queue q;
    queue_init(&q);
    // Fill and drain partially to move head/tail forward
    int i;
    for (i = 0; i < 5; i++) queue_enqueue(&q, i);
    for (i = 0; i < 5; i++) queue_dequeue(&q);
    // Now head and tail are at index 5, enqueue wraps around
    for (i = 6; i <= 10; i++) queue_enqueue(&q, i);
    printf("after wraparound:");
    while (!queue_empty(&q)) {
        printf(" %d", queue_dequeue(&q));
    }
    printf("\n");
}

void test_fifo_order(void) {
    printf("=== FIFO Order ===\n");
    struct Queue q;
    queue_init(&q);
    int i;
    for (i = 1; i <= 8; i++) queue_enqueue(&q, i);
    printf("fifo:");
    while (!queue_empty(&q)) {
        printf(" %d", queue_dequeue(&q));
    }
    printf("\n");
}

void test_full_queue(void) {
    printf("=== Full Queue ===\n");
    struct Queue q;
    queue_init(&q);
    int i;
    int count = 0;
    for (i = 0; i < 10; i++) {
        if (queue_enqueue(&q, i)) count = count + 1;
    }
    printf("filled %d items\n", count);
    printf("drained:");
    while (!queue_empty(&q)) {
        printf(" %d", queue_dequeue(&q));
    }
    printf("\n");
}

void test_interleaved(void) {
    printf("=== Interleaved Ops ===\n");
    struct Queue q;
    queue_init(&q);
    queue_enqueue(&q, 1);
    queue_enqueue(&q, 2);
    queue_enqueue(&q, 3);
    int a = queue_dequeue(&q);
    int b = queue_dequeue(&q);
    queue_enqueue(&q, 4);
    queue_enqueue(&q, 5);
    queue_enqueue(&q, 6);
    // remaining: 3 4 5 6, but we consumed 1 and 2 already
    printf("result: %d %d", a, b);
    while (!queue_empty(&q)) {
        printf(" %d", queue_dequeue(&q));
    }
    printf("\n");
}

void test_copy(void) {
    printf("=== Queue Copy ===\n");
    struct Queue q1;
    queue_init(&q1);
    queue_enqueue(&q1, 10);
    queue_enqueue(&q1, 20);
    queue_enqueue(&q1, 30);

    // Copy by draining and re-enqueuing
    struct Queue q2;
    queue_init(&q2);
    int sz = queue_size(&q1);
    int i;
    for (i = 0; i < sz; i++) {
        int val = queue_dequeue(&q1);
        queue_enqueue(&q2, val);
    }
    printf("copy:");
    while (!queue_empty(&q2)) {
        printf(" %d", queue_dequeue(&q2));
    }
    printf("\n");
}

int main(void) {
    test_enqueue_dequeue();
    test_front();
    test_empty();
    test_size();
    test_circular_wrap();
    test_fifo_order();
    test_full_queue();
    test_interleaved();
    test_copy();
    return 0;
}
