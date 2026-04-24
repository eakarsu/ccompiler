int printf(const char *fmt, ...);

struct RingBuffer {
    int data[16];
    int head;
    int tail;
    int count;
    int capacity;
};

void rb_init(struct RingBuffer *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    rb->capacity = 16;
    int i;
    for (i = 0; i < 16; i = i + 1) {
        rb->data[i] = 0;
    }
}

int rb_is_empty(struct RingBuffer *rb) {
    return rb->count == 0;
}

int rb_is_full(struct RingBuffer *rb) {
    return rb->count == rb->capacity;
}

int rb_size(struct RingBuffer *rb) {
    return rb->count;
}

int rb_write(struct RingBuffer *rb, int value) {
    if (rb_is_full(rb)) {
        return 0;  /* failure */
    }
    rb->data[rb->tail] = value;
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count = rb->count + 1;
    return 1;  /* success */
}

int rb_read(struct RingBuffer *rb, int *value) {
    if (rb_is_empty(rb)) {
        return 0;  /* failure */
    }
    *value = rb->data[rb->head];
    rb->head = (rb->head + 1) % rb->capacity;
    rb->count = rb->count - 1;
    return 1;  /* success */
}

int rb_peek(struct RingBuffer *rb) {
    if (rb_is_empty(rb)) {
        return -1;
    }
    return rb->data[rb->head];
}

void rb_clear(struct RingBuffer *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

int main(void) {
    struct RingBuffer rb;
    rb_init(&rb);

    // EXPECT: Empty: 1
    printf("Empty: %d\n", rb_is_empty(&rb));
    // EXPECT: Full: 0
    printf("Full: %d\n", rb_is_full(&rb));
    // EXPECT: Size: 0
    printf("Size: %d\n", rb_size(&rb));

    /* Write some values */
    int ok;
    ok = rb_write(&rb, 10);
    // EXPECT: Write 10: 1
    printf("Write 10: %d\n", ok);

    ok = rb_write(&rb, 20);
    // EXPECT: Write 20: 1
    printf("Write 20: %d\n", ok);

    ok = rb_write(&rb, 30);
    // EXPECT: Write 30: 1
    printf("Write 30: %d\n", ok);

    // EXPECT: Size after 3 writes: 3
    printf("Size after 3 writes: %d\n", rb_size(&rb));
    // EXPECT: Empty: 0
    printf("Empty: %d\n", rb_is_empty(&rb));

    /* Peek at front */
    // EXPECT: Peek: 10
    printf("Peek: %d\n", rb_peek(&rb));

    /* Read values - should be FIFO */
    int val;
    rb_read(&rb, &val);
    // EXPECT: Read: 10
    printf("Read: %d\n", val);

    rb_read(&rb, &val);
    // EXPECT: Read: 20
    printf("Read: %d\n", val);

    // EXPECT: Size after 2 reads: 1
    printf("Size after 2 reads: %d\n", rb_size(&rb));

    rb_read(&rb, &val);
    // EXPECT: Read: 30
    printf("Read: %d\n", val);

    // EXPECT: Empty again: 1
    printf("Empty again: %d\n", rb_is_empty(&rb));

    /* Test read from empty */
    ok = rb_read(&rb, &val);
    // EXPECT: Read empty: 0
    printf("Read empty: %d\n", ok);

    /* Fill the buffer completely */
    int i;
    for (i = 0; i < 16; i = i + 1) {
        rb_write(&rb, i * 10);
    }
    // EXPECT: Full: 1
    printf("Full: %d\n", rb_is_full(&rb));
    // EXPECT: Size: 16
    printf("Size: %d\n", rb_size(&rb));

    /* Try to write when full */
    ok = rb_write(&rb, 999);
    // EXPECT: Write full: 0
    printf("Write full: %d\n", ok);

    /* Read all and sum */
    int sum = 0;
    int count = 0;
    while (!rb_is_empty(&rb)) {
        rb_read(&rb, &val);
        sum = sum + val;
        count = count + 1;
    }
    /* Sum = 0+10+20+...+150 = 10*(0+1+2+...+15) = 10*120 = 1200 */
    // EXPECT: Sum: 1200
    printf("Sum: %d\n", sum);
    // EXPECT: Count: 16
    printf("Count: %d\n", count);

    /* Test wrap-around: write 5, read 3, write 5, read 7 */
    rb_clear(&rb);
    for (i = 0; i < 5; i = i + 1) {
        rb_write(&rb, 100 + i);
    }
    /* Read 3 */
    for (i = 0; i < 3; i = i + 1) {
        rb_read(&rb, &val);
    }
    /* val should be 102 (last read) */
    // EXPECT: Last read in wrap test: 102
    printf("Last read in wrap test: %d\n", val);

    /* Write 5 more - these wrap around */
    for (i = 0; i < 5; i = i + 1) {
        rb_write(&rb, 200 + i);
    }
    // EXPECT: Size after wrap: 7
    printf("Size after wrap: %d\n", rb_size(&rb));

    /* Read all 7: should be 103, 104, 200, 201, 202, 203, 204 */
    rb_read(&rb, &val);
    // EXPECT: Wrap read 1: 103
    printf("Wrap read 1: %d\n", val);
    rb_read(&rb, &val);
    // EXPECT: Wrap read 2: 104
    printf("Wrap read 2: %d\n", val);
    rb_read(&rb, &val);
    // EXPECT: Wrap read 3: 200
    printf("Wrap read 3: %d\n", val);

    /* Read remaining 4 and sum them */
    sum = 0;
    for (i = 0; i < 4; i = i + 1) {
        rb_read(&rb, &val);
        sum = sum + val;
    }
    /* 201+202+203+204 = 810 */
    // EXPECT: Remaining sum: 810
    printf("Remaining sum: %d\n", sum);

    // EXPECT: Final empty: 1
    printf("Final empty: %d\n", rb_is_empty(&rb));

    return 0;
}
