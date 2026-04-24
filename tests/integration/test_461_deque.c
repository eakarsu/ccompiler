int printf(const char *fmt, ...);
// EXPECT: Deque: [1,5,10,20,30]\nFront: 1\nBack: 30\nPop front: 1\nPop back: 30\nAfter pops: [5,10,20]\nAfter pushes: [100,5,10,20,200]\nCount: 5\nEmpty: 0\nAfter clear, empty: 1
// Deque (double-ended queue) with circular array

struct Deque {
    int data[16];
    int front;
    int back;
    int count;
    int capacity;
};

void dq_init(struct Deque *dq) {
    int i;
    for (i = 0; i < 16; i++) dq->data[i] = 0;
    dq->front = 0;
    dq->back = 0;
    dq->count = 0;
    dq->capacity = 16;
}

int dq_is_empty(struct Deque *dq) { return dq->count == 0; }
int dq_is_full(struct Deque *dq) { return dq->count == dq->capacity; }

int dq_push_front(struct Deque *dq, int val) {
    if (dq_is_full(dq)) return 0;
    dq->front = (dq->front - 1 + dq->capacity) % dq->capacity;
    dq->data[dq->front] = val;
    dq->count++;
    return 1;
}

int dq_push_back(struct Deque *dq, int val) {
    if (dq_is_full(dq)) return 0;
    dq->data[dq->back] = val;
    dq->back = (dq->back + 1) % dq->capacity;
    dq->count++;
    return 1;
}

int dq_pop_front(struct Deque *dq) {
    int val;
    if (dq_is_empty(dq)) return -1;
    val = dq->data[dq->front];
    dq->front = (dq->front + 1) % dq->capacity;
    dq->count--;
    return val;
}

int dq_pop_back(struct Deque *dq) {
    int val;
    if (dq_is_empty(dq)) return -1;
    dq->back = (dq->back - 1 + dq->capacity) % dq->capacity;
    val = dq->data[dq->back];
    dq->count--;
    return val;
}

int dq_peek_front(struct Deque *dq) {
    if (dq_is_empty(dq)) return -1;
    return dq->data[dq->front];
}

int dq_peek_back(struct Deque *dq) {
    if (dq_is_empty(dq)) return -1;
    return dq->data[(dq->back - 1 + dq->capacity) % dq->capacity];
}

void dq_print(struct Deque *dq) {
    int i;
    int idx;
    printf("[");
    for (i = 0; i < dq->count; i++) {
        idx = (dq->front + i) % dq->capacity;
        printf("%d", dq->data[idx]);
        if (i < dq->count - 1) printf(",");
    }
    printf("]\n");
}

int main(void) {
    struct Deque dq;
    dq_init(&dq);

    dq_push_back(&dq, 10);
    dq_push_back(&dq, 20);
    dq_push_back(&dq, 30);
    dq_push_front(&dq, 5);
    dq_push_front(&dq, 1);
    printf("Deque: ");
    dq_print(&dq);

    printf("Front: %d\n", dq_peek_front(&dq));
    printf("Back: %d\n", dq_peek_back(&dq));

    printf("Pop front: %d\n", dq_pop_front(&dq));
    printf("Pop back: %d\n", dq_pop_back(&dq));
    printf("After pops: ");
    dq_print(&dq);

    dq_push_front(&dq, 100);
    dq_push_back(&dq, 200);
    printf("After pushes: ");
    dq_print(&dq);
    printf("Count: %d\n", dq.count);

    printf("Empty: %d\n", dq_is_empty(&dq));
    while (!dq_is_empty(&dq)) dq_pop_front(&dq);
    printf("After clear, empty: %d\n", dq_is_empty(&dq));

    return 0;
}
