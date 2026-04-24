int printf(const char *fmt, ...);

// Circular buffer with overwrite semantics
// When full, new writes overwrite the oldest data

struct CircBuf {
    int data[8];
    int head;    // read position
    int tail;    // write position
    int count;   // number of elements
    int capacity;
};

void cb_init(struct CircBuf *cb) {
    int i;
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
    cb->capacity = 8;
    for (i = 0; i < 8; i++) {
        cb->data[i] = 0;
    }
}

int cb_is_full(struct CircBuf *cb) {
    return cb->count == cb->capacity;
}

int cb_is_empty(struct CircBuf *cb) {
    return cb->count == 0;
}

int cb_size(struct CircBuf *cb) {
    return cb->count;
}

// Write with overwrite: if full, overwrite oldest
void cb_write(struct CircBuf *cb, int val) {
    cb->data[cb->tail] = val;
    if (cb->count == cb->capacity) {
        // Overwriting: advance head too
        cb->head = (cb->head + 1) % cb->capacity;
    } else {
        cb->count = cb->count + 1;
    }
    cb->tail = (cb->tail + 1) % cb->capacity;
}

// Read oldest element, returns -1 if empty
int cb_read(struct CircBuf *cb) {
    int val;
    if (cb->count == 0) return -1;
    val = cb->data[cb->head];
    cb->head = (cb->head + 1) % cb->capacity;
    cb->count = cb->count - 1;
    return val;
}

// Peek at oldest without removing
int cb_peek(struct CircBuf *cb) {
    if (cb->count == 0) return -1;
    return cb->data[cb->head];
}

// Peek at element at index from head
int cb_peek_at(struct CircBuf *cb, int idx) {
    int pos;
    if (idx < 0 || idx >= cb->count) return -1;
    pos = (cb->head + idx) % cb->capacity;
    return cb->data[pos];
}

// Clear the buffer
void cb_clear(struct CircBuf *cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

// Sum all elements in buffer
int cb_sum(struct CircBuf *cb) {
    int sum;
    int i;
    int pos;
    sum = 0;
    for (i = 0; i < cb->count; i++) {
        pos = (cb->head + i) % cb->capacity;
        sum = sum + cb->data[pos];
    }
    return sum;
}

int main() {
    struct CircBuf cb;
    int val;
    int i;

    cb_init(&cb);
    printf("Empty: %d\n", cb_is_empty(&cb));
    // EXPECT: Empty: 1

    printf("Size: %d\n", cb_size(&cb));
    // EXPECT: Size: 0

    // Write some values
    cb_write(&cb, 10);
    cb_write(&cb, 20);
    cb_write(&cb, 30);

    printf("Size after 3 writes: %d\n", cb_size(&cb));
    // EXPECT: Size after 3 writes: 3

    printf("Peek: %d\n", cb_peek(&cb));
    // EXPECT: Peek: 10

    printf("Peek at 1: %d\n", cb_peek_at(&cb, 1));
    // EXPECT: Peek at 1: 20

    printf("Peek at 2: %d\n", cb_peek_at(&cb, 2));
    // EXPECT: Peek at 2: 30

    // Read one
    val = cb_read(&cb);
    printf("Read: %d\n", val);
    // EXPECT: Read: 10

    printf("Size after read: %d\n", cb_size(&cb));
    // EXPECT: Size after read: 2

    printf("New peek: %d\n", cb_peek(&cb));
    // EXPECT: New peek: 20

    // Fill buffer to capacity (already has 20, 30; add 6 more)
    cb_write(&cb, 40);
    cb_write(&cb, 50);
    cb_write(&cb, 60);
    cb_write(&cb, 70);
    cb_write(&cb, 80);
    cb_write(&cb, 90);

    printf("Full: %d\n", cb_is_full(&cb));
    // EXPECT: Full: 1

    printf("Size: %d\n", cb_size(&cb));
    // EXPECT: Size: 8

    printf("Sum: %d\n", cb_sum(&cb));
    // EXPECT: Sum: 440

    // Now overwrite: writing 100 should overwrite 20 (oldest)
    cb_write(&cb, 100);
    printf("After overwrite, size: %d\n", cb_size(&cb));
    // EXPECT: After overwrite, size: 8

    printf("After overwrite, peek: %d\n", cb_peek(&cb));
    // EXPECT: After overwrite, peek: 30

    // Write another, overwrites 30
    cb_write(&cb, 110);
    printf("Peek now: %d\n", cb_peek(&cb));
    // EXPECT: Peek now: 40

    printf("Sum now: %d\n", cb_sum(&cb));
    // EXPECT: Sum now: 600

    // Read all
    val = cb_read(&cb);
    printf("Read 1: %d\n", val);
    // EXPECT: Read 1: 40

    val = cb_read(&cb);
    printf("Read 2: %d\n", val);
    // EXPECT: Read 2: 50

    // Clear and test
    cb_clear(&cb);
    printf("After clear, empty: %d\n", cb_is_empty(&cb));
    // EXPECT: After clear, empty: 1

    printf("Read empty: %d\n", cb_read(&cb));
    // EXPECT: Read empty: -1

    // Write after clear
    cb_write(&cb, 999);
    printf("After re-write: %d\n", cb_peek(&cb));
    // EXPECT: After re-write: 999

    printf("Final size: %d\n", cb_size(&cb));
    // EXPECT: Final size: 1

    return 0;
}
