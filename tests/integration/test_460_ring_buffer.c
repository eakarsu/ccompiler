int printf(const char *fmt, ...);
// EXPECT: After pushing 10-50: [10,20,30,40,50] (count=5)\nPopped: 10\nPopped: 20\nAfter 2 pops: [30,40,50] (count=3)\nAfter filling: [30,40,50,60,70,80,90,100] (count=8)\nPush when full: 0\nPeek: 30\nOverwrite push 110: 1\nAfter overwrite: [40,50,60,70,80,90,100,110] (count=8)\nOverwrite push 120: 1\nAfter overwrite: [50,60,70,80,90,100,110,120] (count=8)
// Ring buffer implementation

struct RingBuffer {
    int data[8];
    int head;
    int tail;
    int count;
    int capacity;
};

void rb_init(struct RingBuffer *rb) {
    int i;
    for (i = 0; i < 8; i++) rb->data[i] = 0;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    rb->capacity = 8;
}

int rb_is_full(struct RingBuffer *rb) {
    return rb->count == rb->capacity;
}

int rb_is_empty(struct RingBuffer *rb) {
    return rb->count == 0;
}

int rb_push(struct RingBuffer *rb, int val) {
    if (rb_is_full(rb)) return 0;
    rb->data[rb->tail] = val;
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count++;
    return 1;
}

int rb_pop(struct RingBuffer *rb, int *val) {
    if (rb_is_empty(rb)) return 0;
    *val = rb->data[rb->head];
    rb->head = (rb->head + 1) % rb->capacity;
    rb->count--;
    return 1;
}

int rb_peek(struct RingBuffer *rb) {
    if (rb_is_empty(rb)) return -1;
    return rb->data[rb->head];
}

void rb_print(struct RingBuffer *rb) {
    int i;
    int idx;
    printf("[");
    for (i = 0; i < rb->count; i++) {
        idx = (rb->head + i) % rb->capacity;
        printf("%d", rb->data[idx]);
        if (i < rb->count - 1) printf(",");
    }
    printf("] (count=%d)\n", rb->count);
}

int rb_push_overwrite(struct RingBuffer *rb, int val) {
    int overwritten = 0;
    if (rb_is_full(rb)) {
        rb->head = (rb->head + 1) % rb->capacity;
        rb->count--;
        overwritten = 1;
    }
    rb->data[rb->tail] = val;
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count++;
    return overwritten;
}

int main(void) {
    struct RingBuffer rb;
    int val;
    int i;
    rb_init(&rb);

    for (i = 1; i <= 5; i++) {
        rb_push(&rb, i * 10);
    }
    printf("After pushing 10-50: ");
    rb_print(&rb);

    rb_pop(&rb, &val);
    printf("Popped: %d\n", val);
    rb_pop(&rb, &val);
    printf("Popped: %d\n", val);
    printf("After 2 pops: ");
    rb_print(&rb);

    rb_push(&rb, 60);
    rb_push(&rb, 70);
    rb_push(&rb, 80);
    rb_push(&rb, 90);
    rb_push(&rb, 100);
    printf("After filling: ");
    rb_print(&rb);

    printf("Push when full: %d\n", rb_push(&rb, 999));

    printf("Peek: %d\n", rb_peek(&rb));

    printf("Overwrite push 110: %d\n", rb_push_overwrite(&rb, 110));
    printf("After overwrite: ");
    rb_print(&rb);

    printf("Overwrite push 120: %d\n", rb_push_overwrite(&rb, 120));
    printf("After overwrite: ");
    rb_print(&rb);

    return 0;
}
