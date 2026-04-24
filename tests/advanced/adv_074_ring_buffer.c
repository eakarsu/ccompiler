// EXPECT: === Ring Buffer ===
// EXPECT: Empty: yes
// EXPECT: Full: no
// EXPECT: Enqueued: 10 20 30 40 50 60 70 80
// EXPECT: Full: yes
// EXPECT: Size: 8
// EXPECT: Peek: 10
// EXPECT: Dequeued: 10 20 30
// EXPECT: Size: 5
// EXPECT: Enqueued more: 90 100 110
// EXPECT: Full: yes
// EXPECT: All items: 40 50 60 70 80 90 100 110
// EXPECT: Empty: yes
// EXPECT: Dequeue from empty: failed (expected)
// EXPECT: Wrap test enqueue: 1 2 3 4 5 6 7 8
// EXPECT: Wrap dequeue 4: 1 2 3 4
// EXPECT: Wrap enqueue 4 more: 9 10 11 12
// EXPECT: Wrap all: 5 6 7 8 9 10 11 12
// EXPECT: Bulk write 5: ok
// EXPECT: Bulk read 3: 100 200 300
// EXPECT: Remaining: 400 500
// EXPECT: All ring buffer tests passed

int printf(const char *fmt, ...);

typedef struct {
    int data[8];
    int head;
    int tail;
    int count;
    int capacity;
} RingBuffer;

void rb_init(RingBuffer *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    rb->capacity = 8;
    int i;
    for (i = 0; i < 8; i++) rb->data[i] = 0;
}

int rb_empty(RingBuffer *rb) {
    return rb->count == 0;
}

int rb_full(RingBuffer *rb) {
    return rb->count == rb->capacity;
}

int rb_size(RingBuffer *rb) {
    return rb->count;
}

int rb_enqueue(RingBuffer *rb, int val) {
    if (rb_full(rb)) return -1;
    rb->data[rb->tail] = val;
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count++;
    return 0;
}

int rb_dequeue(RingBuffer *rb, int *val) {
    if (rb_empty(rb)) return -1;
    *val = rb->data[rb->head];
    rb->head = (rb->head + 1) % rb->capacity;
    rb->count--;
    return 0;
}

int rb_peek(RingBuffer *rb, int *val) {
    if (rb_empty(rb)) return -1;
    *val = rb->data[rb->head];
    return 0;
}

void test_basic(void) {
    RingBuffer rb;
    rb_init(&rb);

    printf("Empty: %s\n", rb_empty(&rb) ? "yes" : "no");
    printf("Full: %s\n", rb_full(&rb) ? "yes" : "no");

    int vals[8];
    vals[0] = 10; vals[1] = 20; vals[2] = 30; vals[3] = 40;
    vals[4] = 50; vals[5] = 60; vals[6] = 70; vals[7] = 80;

    printf("Enqueued:");
    int i;
    for (i = 0; i < 8; i++) {
        rb_enqueue(&rb, vals[i]);
        printf(" %d", vals[i]);
    }
    printf("\n");

    printf("Full: %s\n", rb_full(&rb) ? "yes" : "no");
    printf("Size: %d\n", rb_size(&rb));

    int v;
    rb_peek(&rb, &v);
    printf("Peek: %d\n", v);

    printf("Dequeued:");
    for (i = 0; i < 3; i++) {
        rb_dequeue(&rb, &v);
        printf(" %d", v);
    }
    printf("\n");
    printf("Size: %d\n", rb_size(&rb));

    printf("Enqueued more:");
    int more[3];
    more[0] = 90; more[1] = 100; more[2] = 110;
    for (i = 0; i < 3; i++) {
        rb_enqueue(&rb, more[i]);
        printf(" %d", more[i]);
    }
    printf("\n");
    printf("Full: %s\n", rb_full(&rb) ? "yes" : "no");

    printf("All items:");
    while (!rb_empty(&rb)) {
        rb_dequeue(&rb, &v);
        printf(" %d", v);
    }
    printf("\n");
    printf("Empty: %s\n", rb_empty(&rb) ? "yes" : "no");

    if (rb_dequeue(&rb, &v) == -1) {
        printf("Dequeue from empty: failed (expected)\n");
    }
}

void test_wraparound(void) {
    RingBuffer rb;
    rb_init(&rb);
    int i;
    int v;

    printf("Wrap test enqueue:");
    for (i = 1; i <= 8; i++) {
        rb_enqueue(&rb, i);
        printf(" %d", i);
    }
    printf("\n");

    printf("Wrap dequeue 4:");
    for (i = 0; i < 4; i++) {
        rb_dequeue(&rb, &v);
        printf(" %d", v);
    }
    printf("\n");

    printf("Wrap enqueue 4 more:");
    for (i = 9; i <= 12; i++) {
        rb_enqueue(&rb, i);
        printf(" %d", i);
    }
    printf("\n");

    printf("Wrap all:");
    while (!rb_empty(&rb)) {
        rb_dequeue(&rb, &v);
        printf(" %d", v);
    }
    printf("\n");
}

int rb_bulk_write(RingBuffer *rb, int *vals, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (rb_enqueue(rb, vals[i]) != 0) return i;
    }
    return n;
}

int rb_bulk_read(RingBuffer *rb, int *out, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (rb_dequeue(rb, &out[i]) != 0) return i;
    }
    return n;
}

void test_bulk(void) {
    RingBuffer rb;
    rb_init(&rb);

    int vals[5];
    vals[0] = 100; vals[1] = 200; vals[2] = 300; vals[3] = 400; vals[4] = 500;

    int written = rb_bulk_write(&rb, vals, 5);
    printf("Bulk write %d: ok\n", written);

    int out[3];
    int readn = rb_bulk_read(&rb, out, 3);
    printf("Bulk read %d:", readn);
    int i;
    for (i = 0; i < readn; i++) printf(" %d", out[i]);
    printf("\n");

    printf("Remaining:");
    int v;
    while (!rb_empty(&rb)) {
        rb_dequeue(&rb, &v);
        printf(" %d", v);
    }
    printf("\n");
}

int main(void) {
    printf("=== Ring Buffer ===\n");
    test_basic();
    test_wraparound();
    test_bulk();
    printf("All ring buffer tests passed\n");
    return 0;
}
