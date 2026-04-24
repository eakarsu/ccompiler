int printf(const char *fmt, ...);

/* Circular queue (ring buffer) using a fixed-size array */

#define QUEUE_CAP 8

typedef struct {
    int data[QUEUE_CAP];
    int head;   /* index of front element */
    int tail;   /* index of next empty slot */
    int count;
} Queue;

void queue_init(Queue *q) {
    q->head  = 0;
    q->tail  = 0;
    q->count = 0;
}

int queue_is_empty(Queue *q) {
    return q->count == 0;
}

int queue_is_full(Queue *q) {
    return q->count == QUEUE_CAP;
}

int queue_size(Queue *q) {
    return q->count;
}

int queue_enqueue(Queue *q, int val) {
    if (queue_is_full(q)) return 0;
    q->data[q->tail] = val;
    q->tail = (q->tail + 1) % QUEUE_CAP;
    q->count++;
    return 1;
}

int queue_dequeue(Queue *q, int *out) {
    if (queue_is_empty(q)) return 0;
    *out = q->data[q->head];
    q->head = (q->head + 1) % QUEUE_CAP;
    q->count--;
    return 1;
}

int queue_peek(Queue *q, int *out) {
    if (queue_is_empty(q)) return 0;
    *out = q->data[q->head];
    return 1;
}

/* Peek at back (last enqueued element) */
int queue_peek_back(Queue *q, int *out) {
    if (queue_is_empty(q)) return 0;
    int back = (q->tail - 1 + QUEUE_CAP) % QUEUE_CAP;
    *out = q->data[back];
    return 1;
}

/* Print all elements front-to-back */
void queue_print(Queue *q) {
    int i;
    for (i = 0; i < q->count; i++) {
        int idx = (q->head + i) % QUEUE_CAP;
        printf("%d", q->data[idx]);
        if (i < q->count - 1) printf(" ");
    }
    printf("\n");
}

int main(void) {
    Queue q;
    queue_init(&q);

    /* Empty checks */
    printf("%d\n", queue_is_empty(&q));    /* 1 */
    // EXPECT: 1
    printf("%d\n", queue_size(&q));        /* 0 */
    // EXPECT: 0

    /* Enqueue 5 elements */
    queue_enqueue(&q, 10);
    queue_enqueue(&q, 20);
    queue_enqueue(&q, 30);
    queue_enqueue(&q, 40);
    queue_enqueue(&q, 50);

    printf("%d\n", queue_size(&q));        /* 5 */
    // EXPECT: 5

    /* Peek front and back */
    int val;
    queue_peek(&q, &val);
    printf("%d\n", val);                   /* 10 */
    // EXPECT: 10

    queue_peek_back(&q, &val);
    printf("%d\n", val);                   /* 50 */
    // EXPECT: 50

    /* Dequeue 2 */
    queue_dequeue(&q, &val);
    printf("%d\n", val);                   /* 10 */
    // EXPECT: 10

    queue_dequeue(&q, &val);
    printf("%d\n", val);                   /* 20 */
    // EXPECT: 20

    printf("%d\n", queue_size(&q));        /* 3 */
    // EXPECT: 3

    /* Print remaining: 30 40 50 */
    queue_print(&q);
    // EXPECT: 30 40 50

    /* Fill up to capacity (currently 3 items, cap=8, need 5 more) */
    queue_enqueue(&q, 60);
    queue_enqueue(&q, 70);
    queue_enqueue(&q, 80);
    queue_enqueue(&q, 90);
    queue_enqueue(&q, 100);

    printf("%d\n", queue_is_full(&q));     /* 1 */
    // EXPECT: 1

    /* Enqueue onto full returns 0 */
    int en = queue_enqueue(&q, 999);
    printf("%d\n", en);                    /* 0 */
    // EXPECT: 0

    /* Print all 8: 30 40 50 60 70 80 90 100 */
    queue_print(&q);
    // EXPECT: 30 40 50 60 70 80 90 100

    /* Wrap-around test: dequeue 4, then enqueue 4 more */
    queue_dequeue(&q, &val);  /* 30 */
    queue_dequeue(&q, &val);  /* 40 */
    queue_dequeue(&q, &val);  /* 50 */
    queue_dequeue(&q, &val);  /* 60 */

    queue_enqueue(&q, 110);
    queue_enqueue(&q, 120);
    queue_enqueue(&q, 130);
    queue_enqueue(&q, 140);

    /* Now queue contains: 70 80 90 100 110 120 130 140 */
    /* head has wrapped around in the array */
    printf("%d\n", queue_size(&q));        /* 8 */
    // EXPECT: 8

    queue_print(&q);
    // EXPECT: 70 80 90 100 110 120 130 140

    /* Dequeue all and sum */
    int sum = 0;
    while (!queue_is_empty(&q)) {
        queue_dequeue(&q, &val);
        sum += val;
    }
    /* 70+80+90+100+110+120+130+140 = 840 */
    printf("%d\n", sum);                   /* 840 */
    // EXPECT: 840

    printf("%d\n", queue_is_empty(&q));    /* 1 */
    // EXPECT: 1

    /* Dequeue from empty returns 0 */
    int dq = queue_dequeue(&q, &val);
    printf("%d\n", dq);                    /* 0 */
    // EXPECT: 0

    /* FIFO order test */
    int i;
    for (i = 1; i <= 6; i++) {
        queue_enqueue(&q, i * i);
    }
    /* queue: 1 4 9 16 25 36 */
    queue_print(&q);
    // EXPECT: 1 4 9 16 25 36

    /* Dequeue 3 and print front */
    queue_dequeue(&q, &val); /* 1 */
    queue_dequeue(&q, &val); /* 4 */
    queue_dequeue(&q, &val); /* 9 */
    queue_peek(&q, &val);
    printf("%d\n", val);                   /* 16 */
    // EXPECT: 16

    return 0;
}
