int printf(const char *fmt, ...);

// Test 081: Basic ring buffer with integer storage

struct RingBuffer {
    int data[8];
    int head;
    int tail;
    int count;
    int capacity;
};

void rb_init(struct RingBuffer *rb) {
    int i;
    for (i = 0; i < 8; i++) {
        rb->data[i] = 0;
    }
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    rb->capacity = 8;
}

int rb_push(struct RingBuffer *rb, int value) {
    if (rb->count >= rb->capacity) {
        return 0;
    }
    rb->data[rb->tail] = value;
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count = rb->count + 1;
    return 1;
}

int rb_pop(struct RingBuffer *rb, int *out) {
    if (rb->count <= 0) {
        return 0;
    }
    *out = rb->data[rb->head];
    rb->head = (rb->head + 1) % rb->capacity;
    rb->count = rb->count - 1;
    return 1;
}

int rb_peek(struct RingBuffer *rb) {
    if (rb->count <= 0) {
        return -1;
    }
    return rb->data[rb->head];
}

int rb_is_empty(struct RingBuffer *rb) {
    return rb->count == 0;
}

int rb_is_full(struct RingBuffer *rb) {
    return rb->count >= rb->capacity;
}

int main() {
    struct RingBuffer rb;
    int val;
    int ok;
    int i;

    rb_init(&rb);

    printf("Empty: %d\n", rb_is_empty(&rb)); // EXPECT: Empty: 1
    printf("Full: %d\n", rb_is_full(&rb)); // EXPECT: Full: 0

    // Push values 10-17
    for (i = 0; i < 8; i++) {
        ok = rb_push(&rb, 10 + i);
    }
    printf("Count after fill: %d\n", rb.count); // EXPECT: Count after fill: 8
    printf("Full: %d\n", rb_is_full(&rb)); // EXPECT: Full: 1

    // Try push when full
    ok = rb_push(&rb, 99);
    printf("Push when full: %d\n", ok); // EXPECT: Push when full: 0

    // Pop 3 values
    rb_pop(&rb, &val);
    printf("Popped: %d\n", val); // EXPECT: Popped: 10
    rb_pop(&rb, &val);
    printf("Popped: %d\n", val); // EXPECT: Popped: 11
    rb_pop(&rb, &val);
    printf("Popped: %d\n", val); // EXPECT: Popped: 12

    // Push 3 more (wrap around)
    for (i = 0; i < 3; i++) {
        rb_push(&rb, 20 + i);
    }
    printf("Count: %d\n", rb.count); // EXPECT: Count: 8
    printf("Head: %d Tail: %d\n", rb.head, rb.tail); // EXPECT: Head: 3 Tail: 3

    // Pop all and print
    rb_pop(&rb, &val);
    printf("Got: %d\n", val); // EXPECT: Got: 13
    rb_pop(&rb, &val);
    printf("Got: %d\n", val); // EXPECT: Got: 14
    rb_pop(&rb, &val);
    printf("Got: %d\n", val); // EXPECT: Got: 15
    rb_pop(&rb, &val);
    printf("Got: %d\n", val); // EXPECT: Got: 16
    rb_pop(&rb, &val);
    printf("Got: %d\n", val); // EXPECT: Got: 17
    rb_pop(&rb, &val);
    printf("Got: %d\n", val); // EXPECT: Got: 20
    rb_pop(&rb, &val);
    printf("Got: %d\n", val); // EXPECT: Got: 21
    rb_pop(&rb, &val);
    printf("Got: %d\n", val); // EXPECT: Got: 22

    // Pop from empty
    ok = rb_pop(&rb, &val);
    printf("Pop empty: %d\n", ok); // EXPECT: Pop empty: 0

    // Peek on empty
    printf("Peek empty: %d\n", rb_peek(&rb)); // EXPECT: Peek empty: -1

    // Stress: push and pop alternately
    for (i = 0; i < 20; i++) {
        rb_push(&rb, i * 3);
        rb_pop(&rb, &val);
    }
    printf("After stress: count=%d\n", rb.count); // EXPECT: After stress: count=0
    printf("Head: %d Tail: %d\n", rb.head, rb.tail); // EXPECT: Head: 7 Tail: 7

    printf("PASS\n"); // EXPECT: PASS
    return 0;
}
