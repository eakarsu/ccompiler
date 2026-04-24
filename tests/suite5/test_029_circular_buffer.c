int printf(const char *fmt, ...);

int buf[8];
int head;
int tail;
int count;

int buf_is_empty(void) {
    return count == 0;
}

int buf_is_full(void) {
    return count == 8;
}

int buf_write(int val) {
    if (buf_is_full()) {
        return -1;
    }
    buf[head] = val;
    head = (head + 1) - ((head + 1) / 8) * 8;
    count = count + 1;
    return 0;
}

int buf_read(void) {
    if (buf_is_empty()) {
        return -1;
    }
    int val = buf[tail];
    tail = (tail + 1) - ((tail + 1) / 8) * 8;
    count = count - 1;
    return val;
}

int buf_peek(void) {
    if (buf_is_empty()) {
        return -1;
    }
    return buf[tail];
}

int main(void) {
    head = 0;
    tail = 0;
    count = 0;

    // Initially empty
    // EXPECT: empty: 1
    printf("empty: %d\n", buf_is_empty());
    // EXPECT: full: 0
    printf("full: %d\n", buf_is_full());

    // Write some values
    buf_write(10);
    buf_write(20);
    buf_write(30);

    // EXPECT: count: 3
    printf("count: %d\n", count);
    // EXPECT: empty now: 0
    printf("empty now: %d\n", buf_is_empty());

    // Peek at front
    // EXPECT: peek: 10
    printf("peek: %d\n", buf_peek());

    // Read values (FIFO order)
    // EXPECT: read1: 10
    printf("read1: %d\n", buf_read());
    // EXPECT: read2: 20
    printf("read2: %d\n", buf_read());
    // EXPECT: count after: 1
    printf("count after: %d\n", count);

    // Read last
    // EXPECT: read3: 30
    printf("read3: %d\n", buf_read());
    // EXPECT: empty again: 1
    printf("empty again: %d\n", buf_is_empty());

    // Read from empty returns -1
    // EXPECT: empty read: -1
    printf("empty read: %d\n", buf_read());

    // Fill the buffer completely
    int i = 0;
    while (i < 8) {
        buf_write((i + 1) * 100);
        i = i + 1;
    }
    // EXPECT: full now: 1
    printf("full now: %d\n", buf_is_full());
    // EXPECT: count full: 8
    printf("count full: %d\n", count);

    // Write to full returns -1
    int result = buf_write(999);
    // EXPECT: full write: -1
    printf("full write: %d\n", result);

    // Read all
    // EXPECT: drain1: 100
    printf("drain1: %d\n", buf_read());
    // EXPECT: drain2: 200
    printf("drain2: %d\n", buf_read());

    // Continue draining
    buf_read();
    buf_read();
    buf_read();
    buf_read();
    // EXPECT: drain7: 700
    printf("drain7: %d\n", buf_read());
    // EXPECT: drain8: 800
    printf("drain8: %d\n", buf_read());

    // EXPECT: drained empty: 1
    printf("drained empty: %d\n", buf_is_empty());

    // Test wrap-around: write and read across boundary
    // head and tail have wrapped around at this point
    buf_write(1);
    buf_write(2);
    buf_write(3);
    // EXPECT: wrap read1: 1
    printf("wrap read1: %d\n", buf_read());
    buf_write(4);
    buf_write(5);
    // EXPECT: wrap read2: 2
    printf("wrap read2: %d\n", buf_read());
    // EXPECT: wrap read3: 3
    printf("wrap read3: %d\n", buf_read());
    // EXPECT: wrap read4: 4
    printf("wrap read4: %d\n", buf_read());
    // EXPECT: wrap read5: 5
    printf("wrap read5: %d\n", buf_read());

    // EXPECT: final empty: 1
    printf("final empty: %d\n", buf_is_empty());

    // Stress: alternate write/read many times
    int total = 0;
    i = 0;
    while (i < 20) {
        buf_write(i);
        total = total + buf_read();
        i = i + 1;
    }
    // EXPECT: stress total: 190
    printf("stress total: %d\n", total);

    return 0;
}
