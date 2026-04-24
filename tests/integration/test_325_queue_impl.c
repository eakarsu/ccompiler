int printf(const char *fmt, ...);
// EXPECT: empty: 1\nqueue[3]: 10 20 30\npeek: 10\nsize: 3\ndequeued: 10\nqueue[2]: 20 30\nqueue[4]: 20 30 40 50\ndequeue: 20\ndequeue: 30\nqueue[2]: 40 50\ntotal processed: 690\nempty after process: 1
// Test: struct for queue implementation

struct Queue {
    int data[20];
    int front;
    int rear;
    int count;
};

void queue_init(struct Queue *q) {
    q->front = 0;
    q->rear = 0;
    q->count = 0;
}

int queue_empty(struct Queue *q) {
    return q->count == 0;
}

int queue_full(struct Queue *q) {
    return q->count == 20;
}

int queue_size(struct Queue *q) {
    return q->count;
}

void queue_enqueue(struct Queue *q, int val) {
    if (!queue_full(q)) {
        q->data[q->rear] = val;
        q->rear = (q->rear + 1) % 20;
        q->count = q->count + 1;
    }
}

int queue_dequeue(struct Queue *q) {
    if (!queue_empty(q)) {
        int val = q->data[q->front];
        q->front = (q->front + 1) % 20;
        q->count = q->count - 1;
        return val;
    }
    return -1;
}

int queue_peek(struct Queue *q) {
    if (!queue_empty(q)) {
        return q->data[q->front];
    }
    return -1;
}

void queue_print(struct Queue *q) {
    printf("queue[%d]: ", q->count);
    int i;
    int idx = q->front;
    for (i = 0; i < q->count; i++) {
        if (i > 0) printf(" ");
        printf("%d", q->data[idx]);
        idx = (idx + 1) % 20;
    }
    printf("\n");
}

int process_tasks(struct Queue *q) {
    int total = 0;
    while (!queue_empty(q)) {
        total = total + queue_dequeue(q);
    }
    return total;
}

int main(void) {
    struct Queue q;
    queue_init(&q);

    printf("empty: %d\n", queue_empty(&q));
    queue_enqueue(&q, 10);
    queue_enqueue(&q, 20);
    queue_enqueue(&q, 30);
    queue_print(&q);
    printf("peek: %d\n", queue_peek(&q));
    printf("size: %d\n", queue_size(&q));

    int val = queue_dequeue(&q);
    printf("dequeued: %d\n", val);
    queue_print(&q);

    queue_enqueue(&q, 40);
    queue_enqueue(&q, 50);
    queue_print(&q);

    printf("dequeue: %d\n", queue_dequeue(&q));
    printf("dequeue: %d\n", queue_dequeue(&q));
    queue_print(&q);

    queue_enqueue(&q, 100);
    queue_enqueue(&q, 200);
    queue_enqueue(&q, 300);
    printf("total processed: %d\n", process_tasks(&q));
    printf("empty after process: %d\n", queue_empty(&q));

    return 0;
}
