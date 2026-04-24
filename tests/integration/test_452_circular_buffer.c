int printf(const char *fmt, ...);
// EXPECT: Empty: 1\nFull: 1\nCount: 8\nOverflow: -1\nPop: 10\nPop: 20\nPop: 30\nAfter wrap: [40, 50, 60, 70, 80, 90, 100, 110]\nCount: 8\nPeek: 40\nDrain: 40 50 60 70 80 90 100 110\nEmpty again: 1\nDone

// Circular buffer with wrap-around

struct CircBuf {
    int data[8];
    int head;
    int tail;
    int count;
    int capacity;
};

void cb_init(struct CircBuf *cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
    cb->capacity = 8;
    int i = 0;
    while (i < 8) {
        cb->data[i] = 0;
        i = i + 1;
    }
}

int cb_is_full(struct CircBuf *cb) {
    return cb->count == cb->capacity;
}

int cb_is_empty(struct CircBuf *cb) {
    return cb->count == 0;
}

int cb_push(struct CircBuf *cb, int val) {
    if (cb_is_full(cb)) {
        return -1;
    }
    cb->data[cb->tail] = val;
    cb->tail = (cb->tail + 1) % cb->capacity;
    cb->count = cb->count + 1;
    return 0;
}

int cb_pop(struct CircBuf *cb) {
    if (cb_is_empty(cb)) {
        return -1;
    }
    int val = cb->data[cb->head];
    cb->head = (cb->head + 1) % cb->capacity;
    cb->count = cb->count - 1;
    return val;
}

int cb_peek(struct CircBuf *cb) {
    return cb->data[cb->head];
}

void cb_print(struct CircBuf *cb) {
    printf("[");
    int i = 0;
    int idx = cb->head;
    while (i < cb->count) {
        if (i > 0) printf(", ");
        printf("%d", cb->data[idx]);
        idx = (idx + 1) % cb->capacity;
        i = i + 1;
    }
    printf("]");
}

int main(void) {
    struct CircBuf cb;
    cb_init(&cb);

    printf("Empty: %d\n", cb_is_empty(&cb));

    // Fill buffer
    int i = 1;
    while (i <= 8) {
        cb_push(&cb, i * 10);
        i = i + 1;
    }
    printf("Full: %d\n", cb_is_full(&cb));
    printf("Count: %d\n", cb.count);

    // Overflow test
    int res = cb_push(&cb, 99);
    printf("Overflow: %d\n", res);

    // Pop some and push more (wrap-around)
    printf("Pop: %d\n", cb_pop(&cb));
    printf("Pop: %d\n", cb_pop(&cb));
    printf("Pop: %d\n", cb_pop(&cb));

    cb_push(&cb, 90);
    cb_push(&cb, 100);
    cb_push(&cb, 110);

    printf("After wrap: ");
    cb_print(&cb);
    printf("\n");

    printf("Count: %d\n", cb.count);
    printf("Peek: %d\n", cb_peek(&cb));

    // Drain all
    printf("Drain:");
    while (!cb_is_empty(&cb)) {
        printf(" %d", cb_pop(&cb));
    }
    printf("\n");

    printf("Empty again: %d\n", cb_is_empty(&cb));
    printf("Done\n");
    return 0;
}
