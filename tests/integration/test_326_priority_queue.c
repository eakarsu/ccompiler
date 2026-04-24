int printf(const char *fmt, ...);
// EXPECT: pq[5]: (100,p1), (200,p5), (300,p3), (400,p7), (500,p2)\nhighest priority: 7\nextract: 400\nextract: 200\nextract: 300\npq[2]: (100,p1), (500,p2)\nextract: 500\nextract: 100\nempty: 1\nafter insert: pq[1]: (999,p10)\nextract: 999
// Test: priority queue with structs

struct PQItem {
    int value;
    int priority;
};

struct PriorityQueue {
    struct PQItem items[16];
    int count;
};

void pq_init(struct PriorityQueue *pq) {
    pq->count = 0;
}

int pq_empty(struct PriorityQueue *pq) {
    return pq->count == 0;
}

void pq_insert(struct PriorityQueue *pq, int value, int priority) {
    if (pq->count >= 16) return;
    int i = pq->count;
    pq->items[i].value = value;
    pq->items[i].priority = priority;
    pq->count = pq->count + 1;
}

int pq_find_highest(struct PriorityQueue *pq) {
    if (pq->count == 0) return -1;
    int best = 0;
    int i;
    for (i = 1; i < pq->count; i++) {
        if (pq->items[i].priority > pq->items[best].priority) {
            best = i;
        }
    }
    return best;
}

int pq_extract(struct PriorityQueue *pq) {
    if (pq->count == 0) return -1;
    int idx = pq_find_highest(pq);
    int val = pq->items[idx].value;
    int i;
    for (i = idx; i < pq->count - 1; i++) {
        pq->items[i] = pq->items[i + 1];
    }
    pq->count = pq->count - 1;
    return val;
}

int pq_peek_priority(struct PriorityQueue *pq) {
    if (pq->count == 0) return -1;
    int idx = pq_find_highest(pq);
    return pq->items[idx].priority;
}

void pq_print(struct PriorityQueue *pq) {
    printf("pq[%d]: ", pq->count);
    int i;
    for (i = 0; i < pq->count; i++) {
        if (i > 0) printf(", ");
        printf("(%d,p%d)", pq->items[i].value, pq->items[i].priority);
    }
    printf("\n");
}

int main(void) {
    struct PriorityQueue pq;
    pq_init(&pq);

    pq_insert(&pq, 100, 1);
    pq_insert(&pq, 200, 5);
    pq_insert(&pq, 300, 3);
    pq_insert(&pq, 400, 7);
    pq_insert(&pq, 500, 2);

    pq_print(&pq);
    printf("highest priority: %d\n", pq_peek_priority(&pq));

    printf("extract: %d\n", pq_extract(&pq));
    printf("extract: %d\n", pq_extract(&pq));
    printf("extract: %d\n", pq_extract(&pq));
    pq_print(&pq);

    printf("extract: %d\n", pq_extract(&pq));
    printf("extract: %d\n", pq_extract(&pq));
    printf("empty: %d\n", pq_empty(&pq));

    pq_insert(&pq, 999, 10);
    printf("after insert: ");
    pq_print(&pq);
    printf("extract: %d\n", pq_extract(&pq));

    return 0;
}
