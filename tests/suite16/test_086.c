int printf(const char *fmt, ...);

// Test 086: Priority queue using circular buffer with insertion sort

struct PrioItem {
    int priority;
    int value;
};

struct PrioQueue {
    struct PrioItem items[8];
    int count;
};

void pq_init(struct PrioQueue *pq) {
    int i;
    for (i = 0; i < 8; i++) {
        pq->items[i].priority = 0;
        pq->items[i].value = 0;
    }
    pq->count = 0;
}

int pq_insert(struct PrioQueue *pq, int prio, int val) {
    int i;
    int j;
    if (pq->count >= 8) return 0;

    // Find insertion point (higher priority = lower number = front)
    i = pq->count - 1;
    while (i >= 0 && pq->items[i].priority > prio) {
        pq->items[i + 1].priority = pq->items[i].priority;
        pq->items[i + 1].value = pq->items[i].value;
        i = i - 1;
    }
    pq->items[i + 1].priority = prio;
    pq->items[i + 1].value = val;
    pq->count = pq->count + 1;
    return 1;
}

int pq_remove_front(struct PrioQueue *pq, struct PrioItem *out) {
    int i;
    if (pq->count <= 0) return 0;
    out->priority = pq->items[0].priority;
    out->value = pq->items[0].value;
    for (i = 1; i < pq->count; i++) {
        pq->items[i - 1].priority = pq->items[i].priority;
        pq->items[i - 1].value = pq->items[i].value;
    }
    pq->count = pq->count - 1;
    return 1;
}

int pq_peek_prio(struct PrioQueue *pq) {
    if (pq->count <= 0) return -1;
    return pq->items[0].priority;
}

int main() {
    struct PrioQueue pq;
    struct PrioItem item;
    int i;
    int sum;

    pq_init(&pq);
    printf("Empty: %d\n", pq.count); // EXPECT: Empty: 0

    // Insert with various priorities (lower number = higher priority)
    pq_insert(&pq, 5, 500);
    pq_insert(&pq, 1, 100);
    pq_insert(&pq, 3, 300);
    pq_insert(&pq, 2, 200);
    pq_insert(&pq, 4, 400);
    printf("Count: %d\n", pq.count); // EXPECT: Count: 5
    printf("Front prio: %d\n", pq_peek_prio(&pq)); // EXPECT: Front prio: 1

    // Remove in priority order
    pq_remove_front(&pq, &item);
    printf("Got: prio=%d val=%d\n", item.priority, item.value); // EXPECT: Got: prio=1 val=100
    pq_remove_front(&pq, &item);
    printf("Got: prio=%d val=%d\n", item.priority, item.value); // EXPECT: Got: prio=2 val=200
    pq_remove_front(&pq, &item);
    printf("Got: prio=%d val=%d\n", item.priority, item.value); // EXPECT: Got: prio=3 val=300
    printf("Remaining: %d\n", pq.count); // EXPECT: Remaining: 2

    // Insert more, interleaved with removes
    pq_insert(&pq, 1, 10);
    pq_remove_front(&pq, &item);
    printf("Got: prio=%d val=%d\n", item.priority, item.value); // EXPECT: Got: prio=1 val=10
    printf("Count: %d\n", pq.count); // EXPECT: Count: 2

    // Fill up
    pq_insert(&pq, 7, 700);
    pq_insert(&pq, 6, 600);
    pq_insert(&pq, 8, 800);
    pq_insert(&pq, 0, 1);
    pq_insert(&pq, 9, 900);
    pq_insert(&pq, 3, 333);
    printf("Count: %d\n", pq.count); // EXPECT: Count: 8

    // Overflow
    i = pq_insert(&pq, 1, 999);
    printf("Overflow: %d\n", i); // EXPECT: Overflow: 0

    // Drain and verify order
    sum = 0;
    while (pq.count > 0) {
        pq_remove_front(&pq, &item);
        sum = sum + item.value;
    }
    printf("Drain sum=%d\n", sum); // EXPECT: Drain sum=4234

    // Task scheduling simulation: add tasks, process highest prio
    pq_insert(&pq, 2, 50);
    pq_insert(&pq, 1, 99);
    pq_insert(&pq, 3, 25);
    pq_remove_front(&pq, &item);
    printf("Highest prio task: %d\n", item.value); // EXPECT: Highest prio task: 99

    pq_insert(&pq, 1, 88);
    pq_remove_front(&pq, &item);
    printf("Next task: %d\n", item.value); // EXPECT: Next task: 88

    pq_remove_front(&pq, &item);
    printf("Next task: %d\n", item.value); // EXPECT: Next task: 50

    pq_remove_front(&pq, &item);
    printf("Next task: %d\n", item.value); // EXPECT: Next task: 25

    printf("Final count: %d\n", pq.count); // EXPECT: Final count: 0

    printf("PASS\n"); // EXPECT: PASS
    return 0;
}
