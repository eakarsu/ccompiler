int printf(const char *fmt, ...);

// Test 087: Double-ended queue (deque) with circular buffer

struct Deque {
    int data[8];
    int head;
    int tail;
    int count;
};

void dq_init(struct Deque *dq) {
    int i;
    for (i = 0; i < 8; i++) {
        dq->data[i] = 0;
    }
    dq->head = 0;
    dq->tail = 0;
    dq->count = 0;
}

int dq_push_back(struct Deque *dq, int val) {
    if (dq->count >= 8) return 0;
    dq->data[dq->tail] = val;
    dq->tail = (dq->tail + 1) % 8;
    dq->count = dq->count + 1;
    return 1;
}

int dq_push_front(struct Deque *dq, int val) {
    if (dq->count >= 8) return 0;
    dq->head = (dq->head + 7) % 8;  // head - 1, wrapping
    dq->data[dq->head] = val;
    dq->count = dq->count + 1;
    return 1;
}

int dq_pop_front(struct Deque *dq, int *out) {
    if (dq->count <= 0) return 0;
    *out = dq->data[dq->head];
    dq->head = (dq->head + 1) % 8;
    dq->count = dq->count - 1;
    return 1;
}

int dq_pop_back(struct Deque *dq, int *out) {
    if (dq->count <= 0) return 0;
    dq->tail = (dq->tail + 7) % 8;  // tail - 1, wrapping
    *out = dq->data[dq->tail];
    dq->count = dq->count - 1;
    return 1;
}

int dq_front(struct Deque *dq) {
    if (dq->count <= 0) return -1;
    return dq->data[dq->head];
}

int dq_back(struct Deque *dq) {
    int idx;
    if (dq->count <= 0) return -1;
    idx = (dq->tail + 7) % 8;
    return dq->data[idx];
}

int main() {
    struct Deque dq;
    int val;
    int i;

    dq_init(&dq);

    // Push back 1,2,3
    dq_push_back(&dq, 1);
    dq_push_back(&dq, 2);
    dq_push_back(&dq, 3);
    printf("Count: %d\n", dq.count); // EXPECT: Count: 3
    printf("Front: %d Back: %d\n", dq_front(&dq), dq_back(&dq)); // EXPECT: Front: 1 Back: 3

    // Push front 10,20
    dq_push_front(&dq, 10);
    dq_push_front(&dq, 20);
    printf("Count: %d\n", dq.count); // EXPECT: Count: 5
    printf("Front: %d Back: %d\n", dq_front(&dq), dq_back(&dq)); // EXPECT: Front: 20 Back: 3

    // Pop front
    dq_pop_front(&dq, &val);
    printf("Pop front: %d\n", val); // EXPECT: Pop front: 20
    dq_pop_front(&dq, &val);
    printf("Pop front: %d\n", val); // EXPECT: Pop front: 10

    // Pop back
    dq_pop_back(&dq, &val);
    printf("Pop back: %d\n", val); // EXPECT: Pop back: 3
    printf("Count: %d\n", dq.count); // EXPECT: Count: 2

    // Remaining: [1, 2]
    printf("Front: %d Back: %d\n", dq_front(&dq), dq_back(&dq)); // EXPECT: Front: 1 Back: 2

    // Clear
    while (dq.count > 0) {
        dq_pop_front(&dq, &val);
    }
    printf("Empty: %d\n", dq.count); // EXPECT: Empty: 0

    // Stress: alternating push front and push back
    for (i = 0; i < 4; i++) {
        dq_push_front(&dq, i * 10);
        dq_push_back(&dq, i * 10 + 1);
    }
    // Deque order from front: 30, 20, 10, 0, 1, 11, 21, 31
    printf("Full: %d\n", dq.count); // EXPECT: Full: 8

    dq_pop_front(&dq, &val);
    printf("F: %d\n", val); // EXPECT: F: 30
    dq_pop_front(&dq, &val);
    printf("F: %d\n", val); // EXPECT: F: 20
    dq_pop_back(&dq, &val);
    printf("B: %d\n", val); // EXPECT: B: 31
    dq_pop_back(&dq, &val);
    printf("B: %d\n", val); // EXPECT: B: 21

    // Remaining: 10, 0, 1, 11
    printf("Count: %d\n", dq.count); // EXPECT: Count: 4
    printf("Front: %d Back: %d\n", dq_front(&dq), dq_back(&dq)); // EXPECT: Front: 10 Back: 11

    // Use deque as palindrome checker
    // Push: a b c b a -> check if front==back repeatedly
    while (dq.count > 0) dq_pop_front(&dq, &val);
    dq_push_back(&dq, 5);
    dq_push_back(&dq, 3);
    dq_push_back(&dq, 7);
    dq_push_back(&dq, 3);
    dq_push_back(&dq, 5);
    i = 1; // assume palindrome
    while (dq.count > 1) {
        dq_pop_front(&dq, &val);
        int back;
        dq_pop_back(&dq, &back);
        if (val != back) i = 0;
    }
    printf("Palindrome: %d\n", i); // EXPECT: Palindrome: 1

    // Non-palindrome
    while (dq.count > 0) dq_pop_front(&dq, &val);
    dq_push_back(&dq, 1);
    dq_push_back(&dq, 2);
    dq_push_back(&dq, 3);
    dq_push_back(&dq, 4);
    i = 1;
    while (dq.count > 1) {
        dq_pop_front(&dq, &val);
        int back2;
        dq_pop_back(&dq, &back2);
        if (val != back2) i = 0;
    }
    printf("Palindrome: %d\n", i); // EXPECT: Palindrome: 0

    printf("PASS\n"); // EXPECT: PASS
    return 0;
}
