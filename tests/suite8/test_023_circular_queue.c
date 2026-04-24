int printf(const char *fmt, ...);

/* Queue via circular buffer */

struct Queue {
    int data[16];
    int front;
    int rear;
    int count;
    int capacity;
};

void queue_init(struct Queue *q) {
    int i;
    q->front = 0;
    q->rear = -1;
    q->count = 0;
    q->capacity = 16;
    for (i = 0; i < 16; i++) {
        q->data[i] = 0;
    }
}

int queue_is_empty(struct Queue *q) {
    return q->count == 0;
}

int queue_is_full(struct Queue *q) {
    return q->count == q->capacity;
}

int queue_enqueue(struct Queue *q, int val) {
    if (queue_is_full(q)) return 0;
    q->rear = (q->rear + 1) % q->capacity;
    q->data[q->rear] = val;
    q->count = q->count + 1;
    return 1;
}

int queue_dequeue(struct Queue *q) {
    int val;
    if (queue_is_empty(q)) return -999;
    val = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->count = q->count - 1;
    return val;
}

int queue_peek(struct Queue *q) {
    if (queue_is_empty(q)) return -999;
    return q->data[q->front];
}

void queue_print(struct Queue *q) {
    int i;
    int idx;
    printf("queue[%d]:", q->count);
    for (i = 0; i < q->count; i++) {
        idx = (q->front + i) % q->capacity;
        printf(" %d", q->data[idx]);
    }
    printf("\n");
}

/* Simulate a ticket counter: serve N customers from a queue */
void simulate_ticket_counter(struct Queue *q, int n) {
    int i;
    int ticket;
    for (i = 0; i < n; i++) {
        if (!queue_is_empty(q)) {
            ticket = queue_dequeue(q);
            printf("serving ticket %d\n", ticket);
        }
    }
}

/* Generate N tickets and enqueue them */
void generate_tickets(struct Queue *q, int start, int count) {
    int i;
    for (i = 0; i < count; i++) {
        queue_enqueue(q, start + i);
    }
}

int main() {
    struct Queue q;
    int val;
    int i;

    queue_init(&q);
    printf("empty=%d\n", queue_is_empty(&q));
    // EXPECT: empty=1

    queue_enqueue(&q, 10);
    queue_enqueue(&q, 20);
    queue_enqueue(&q, 30);
    queue_enqueue(&q, 40);
    queue_enqueue(&q, 50);

    // EXPECT: queue[5]: 10 20 30 40 50
    queue_print(&q);

    val = queue_dequeue(&q);
    printf("dequeued=%d\n", val);
    // EXPECT: dequeued=10

    val = queue_dequeue(&q);
    printf("dequeued=%d\n", val);
    // EXPECT: dequeued=20

    val = queue_peek(&q);
    printf("peek=%d\n", val);
    // EXPECT: peek=30

    // EXPECT: queue[3]: 30 40 50
    queue_print(&q);

    /* Test wrap-around behavior */
    queue_enqueue(&q, 60);
    queue_enqueue(&q, 70);
    queue_enqueue(&q, 80);

    // EXPECT: queue[6]: 30 40 50 60 70 80
    queue_print(&q);

    /* Dequeue everything */
    while (!queue_is_empty(&q)) {
        val = queue_dequeue(&q);
    }
    printf("last dequeued=%d\n", val);
    // EXPECT: last dequeued=80

    printf("empty after drain=%d\n", queue_is_empty(&q));
    // EXPECT: empty after drain=1

    /* Test ticket counter simulation */
    generate_tickets(&q, 100, 5);
    // EXPECT: serving ticket 100
    // EXPECT: serving ticket 101
    // EXPECT: serving ticket 102
    simulate_ticket_counter(&q, 3);

    // EXPECT: queue[2]: 103 104
    queue_print(&q);

    printf("remaining=%d\n", q.count);
    // EXPECT: remaining=2

    return 0;
}
