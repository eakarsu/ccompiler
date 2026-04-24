int printf(const char *fmt, ...);

struct Queue {
    int data[32];
    int front;
    int rear;
    int count;
};

void queue_init(struct Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

int queue_is_empty(struct Queue *q) {
    return q->count == 0;
}

int queue_is_full(struct Queue *q) {
    return q->count == 32;
}

int queue_size(struct Queue *q) {
    return q->count;
}

void queue_enqueue(struct Queue *q, int val) {
    if (q->count < 32) {
        q->rear = (q->rear + 1) % 32;
        q->data[q->rear] = val;
        q->count = q->count + 1;
    }
}

int queue_dequeue(struct Queue *q) {
    int val;
    if (q->count > 0) {
        val = q->data[q->front];
        q->front = (q->front + 1) % 32;
        q->count = q->count - 1;
        return val;
    }
    return -1;
}

int queue_front(struct Queue *q) {
    if (q->count > 0) {
        return q->data[q->front];
    }
    return -1;
}

int queue_rear_val(struct Queue *q) {
    if (q->count > 0) {
        return q->data[q->rear];
    }
    return -1;
}

void queue_print(struct Queue *q) {
    int i;
    int idx;
    for (i = 0; i < q->count; i = i + 1) {
        if (i > 0) {
            printf(" ");
        }
        idx = (q->front + i) % 32;
        printf("%d", q->data[idx]);
    }
}

int queue_contains(struct Queue *q, int val) {
    int i;
    int idx;
    for (i = 0; i < q->count; i = i + 1) {
        idx = (q->front + i) % 32;
        if (q->data[idx] == val) {
            return 1;
        }
    }
    return 0;
}

int main(void) {
    struct Queue q;
    int val;
    int i;

    queue_init(&q);

    // EXPECT: empty: 1
    printf("empty: %d\n", queue_is_empty(&q));
    // EXPECT: size: 0
    printf("size: %d\n", queue_size(&q));

    queue_enqueue(&q, 10);
    queue_enqueue(&q, 20);
    queue_enqueue(&q, 30);

    // EXPECT: empty: 0
    printf("empty: %d\n", queue_is_empty(&q));
    // EXPECT: size: 3
    printf("size: %d\n", queue_size(&q));

    // EXPECT: front: 10
    printf("front: %d\n", queue_front(&q));
    // EXPECT: rear: 30
    printf("rear: %d\n", queue_rear_val(&q));

    // EXPECT: queue: 10 20 30
    printf("queue: ");
    queue_print(&q);
    printf("\n");

    val = queue_dequeue(&q);
    // EXPECT: dequeued: 10
    printf("dequeued: %d\n", val);
    // EXPECT: front: 20
    printf("front: %d\n", queue_front(&q));
    // EXPECT: size: 2
    printf("size: %d\n", queue_size(&q));

    val = queue_dequeue(&q);
    // EXPECT: dequeued: 20
    printf("dequeued: %d\n", val);

    val = queue_dequeue(&q);
    // EXPECT: dequeued: 30
    printf("dequeued: %d\n", val);

    // EXPECT: empty: 1
    printf("empty: %d\n", queue_is_empty(&q));

    queue_enqueue(&q, 100);
    queue_enqueue(&q, 200);
    queue_enqueue(&q, 300);
    queue_enqueue(&q, 400);
    queue_enqueue(&q, 500);

    // EXPECT: five: 100 200 300 400 500
    printf("five: ");
    queue_print(&q);
    printf("\n");

    // EXPECT: contains 300: 1
    printf("contains 300: %d\n", queue_contains(&q, 300));
    // EXPECT: contains 999: 0
    printf("contains 999: %d\n", queue_contains(&q, 999));

    queue_dequeue(&q);
    queue_dequeue(&q);
    queue_enqueue(&q, 600);
    queue_enqueue(&q, 700);

    // EXPECT: wrapped: 300 400 500 600 700
    printf("wrapped: ");
    queue_print(&q);
    printf("\n");

    // EXPECT: wrapped size: 5
    printf("wrapped size: %d\n", queue_size(&q));
    // EXPECT: wrapped front: 300
    printf("wrapped front: %d\n", queue_front(&q));
    // EXPECT: wrapped rear: 700
    printf("wrapped rear: %d\n", queue_rear_val(&q));

    // EXPECT: done
    printf("done\n");

    return 0;
}
