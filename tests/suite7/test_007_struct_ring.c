int printf(const char *fmt, ...);

struct RingBuf {
    int buf[8];
    int head;
    int tail;
    int count;
    int capacity;
};

void ring_init(struct RingBuf *r) {
    int i;
    for (i = 0; i < 8; i = i + 1) {
        r->buf[i] = 0;
    }
    r->head = 0;
    r->tail = 0;
    r->count = 0;
    r->capacity = 8;
}

int ring_is_empty(struct RingBuf *r) {
    return r->count == 0;
}

int ring_is_full(struct RingBuf *r) {
    return r->count == r->capacity;
}

int ring_push(struct RingBuf *r, int val) {
    if (ring_is_full(r)) return 0;
    r->buf[r->tail] = val;
    r->tail = (r->tail + 1) % r->capacity;
    r->count = r->count + 1;
    return 1;
}

int ring_pop(struct RingBuf *r, int *out) {
    if (ring_is_empty(r)) return 0;
    *out = r->buf[r->head];
    r->head = (r->head + 1) % r->capacity;
    r->count = r->count - 1;
    return 1;
}

int ring_peek(struct RingBuf *r) {
    if (ring_is_empty(r)) return -1;
    return r->buf[r->head];
}

int ring_peek_tail(struct RingBuf *r) {
    if (ring_is_empty(r)) return -1;
    int idx = (r->tail - 1 + r->capacity) % r->capacity;
    return r->buf[idx];
}

int ring_sum(struct RingBuf *r) {
    int total = 0;
    int i;
    int idx;
    for (i = 0; i < r->count; i = i + 1) {
        idx = (r->head + i) % r->capacity;
        total = total + r->buf[idx];
    }
    return total;
}

void ring_clear(struct RingBuf *r) {
    r->head = 0;
    r->tail = 0;
    r->count = 0;
}

int main(void) {
    struct RingBuf rb;
    ring_init(&rb);

    // EXPECT: empty=1
    printf("empty=%d\n", ring_is_empty(&rb));
    // EXPECT: full=0
    printf("full=%d\n", ring_is_full(&rb));

    ring_push(&rb, 10);
    ring_push(&rb, 20);
    ring_push(&rb, 30);

    // EXPECT: count=3
    printf("count=%d\n", rb.count);
    // EXPECT: peek=10
    printf("peek=%d\n", ring_peek(&rb));
    // EXPECT: peek_tail=30
    printf("peek_tail=%d\n", ring_peek_tail(&rb));
    // EXPECT: sum=60
    printf("sum=%d\n", ring_sum(&rb));

    int val;
    ring_pop(&rb, &val);
    // EXPECT: popped=10
    printf("popped=%d\n", val);
    // EXPECT: count after pop=2
    printf("count after pop=%d\n", rb.count);
    // EXPECT: new peek=20
    printf("new peek=%d\n", ring_peek(&rb));

    ring_push(&rb, 40);
    ring_push(&rb, 50);
    ring_push(&rb, 60);
    ring_push(&rb, 70);
    ring_push(&rb, 80);
    ring_push(&rb, 90);
    // EXPECT: count=8
    printf("count=%d\n", rb.count);
    // EXPECT: full=1
    printf("full=%d\n", ring_is_full(&rb));

    /* Overflow: push to full buffer should fail */
    int ok = ring_push(&rb, 999);
    // EXPECT: overflow push=0
    printf("overflow push=%d\n", ok);
    // EXPECT: still count=8
    printf("still count=%d\n", rb.count);

    // EXPECT: sum=440
    printf("sum=%d\n", ring_sum(&rb));

    /* Pop all */
    int total = 0;
    while (ring_pop(&rb, &val)) {
        total = total + val;
    }
    // EXPECT: total popped=440
    printf("total popped=%d\n", total);
    // EXPECT: empty after drain=1
    printf("empty after drain=%d\n", ring_is_empty(&rb));

    /* Wraparound test */
    ring_push(&rb, 1);
    ring_push(&rb, 2);
    ring_push(&rb, 3);
    ring_pop(&rb, &val);
    ring_push(&rb, 4);
    // EXPECT: wrap peek=2
    printf("wrap peek=%d\n", ring_peek(&rb));
    // EXPECT: wrap tail=4
    printf("wrap tail=%d\n", ring_peek_tail(&rb));
    // EXPECT: wrap count=3
    printf("wrap count=%d\n", rb.count);
    // EXPECT: wrap sum=9
    printf("wrap sum=%d\n", ring_sum(&rb));

    return 0;
}
