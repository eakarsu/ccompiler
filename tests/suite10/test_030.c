int printf(const char *fmt, ...);

// Pipe Buffer Simulation: circular buffer IPC between producer/consumer

struct PipeBuffer {
    int data[16];
    int read_pos;
    int write_pos;
    int count;
    int capacity;
    int closed;
    int total_written;
    int total_read;
};

void pipe_init(struct PipeBuffer *p, int cap) {
    int i;
    p->read_pos = 0;
    p->write_pos = 0;
    p->count = 0;
    p->capacity = cap;
    p->closed = 0;
    p->total_written = 0;
    p->total_read = 0;
    for (i = 0; i < cap; i = i + 1) {
        p->data[i] = 0;
    }
}

int pipe_write(struct PipeBuffer *p, int value) {
    if (p->closed) {
        return -1; // pipe closed
    }
    if (p->count >= p->capacity) {
        return 0; // buffer full
    }
    p->data[p->write_pos] = value;
    p->write_pos = p->write_pos + 1;
    if (p->write_pos >= p->capacity) {
        p->write_pos = 0;
    }
    p->count = p->count + 1;
    p->total_written = p->total_written + 1;
    return 1;
}

int pipe_read(struct PipeBuffer *p, int *value) {
    if (p->count <= 0) {
        if (p->closed) return -1; // EOF
        return 0; // would block
    }
    *value = p->data[p->read_pos];
    p->read_pos = p->read_pos + 1;
    if (p->read_pos >= p->capacity) {
        p->read_pos = 0;
    }
    p->count = p->count - 1;
    p->total_read = p->total_read + 1;
    return 1;
}

void pipe_close(struct PipeBuffer *p) {
    p->closed = 1;
}

int pipe_is_empty(struct PipeBuffer *p) {
    return p->count == 0;
}

int pipe_is_full(struct PipeBuffer *p) {
    return p->count >= p->capacity;
}

// Multi-pipe simulation: chain of pipes
struct PipeChain {
    struct PipeBuffer pipes[4];
    int num_pipes;
};

void chain_init(struct PipeChain *c, int npipes, int cap) {
    int i;
    c->num_pipes = npipes;
    for (i = 0; i < npipes; i = i + 1) {
        pipe_init(&c->pipes[i], cap);
    }
}

// Write to first pipe, each stage doubles the value
void chain_process(struct PipeChain *c) {
    int val;
    int result;
    int i;
    int stage;

    for (stage = 0; stage < c->num_pipes - 1; stage = stage + 1) {
        // Read from pipes[stage], transform, write to pipes[stage+1]
        while (1) {
            result = pipe_read(&c->pipes[stage], &val);
            if (result <= 0) break;
            val = val * 2;
            pipe_write(&c->pipes[stage + 1], val);
        }
    }
}

int main() {
    struct PipeBuffer pipe;
    struct PipeChain chain;
    int val;
    int result;
    int i;
    int sum;

    printf("=== Basic Pipe Operations ===\n"); // EXPECT: === Basic Pipe Operations ===
    pipe_init(&pipe, 4);
    printf("Empty: %d\n", pipe_is_empty(&pipe)); // EXPECT: Empty: 1
    printf("Full: %d\n", pipe_is_full(&pipe)); // EXPECT: Full: 0

    // Write 4 items (fill buffer)
    result = pipe_write(&pipe, 10);
    printf("Write 10: %d\n", result); // EXPECT: Write 10: 1
    result = pipe_write(&pipe, 20);
    printf("Write 20: %d\n", result); // EXPECT: Write 20: 1
    result = pipe_write(&pipe, 30);
    printf("Write 30: %d\n", result); // EXPECT: Write 30: 1
    result = pipe_write(&pipe, 40);
    printf("Write 40: %d\n", result); // EXPECT: Write 40: 1

    printf("Full: %d\n", pipe_is_full(&pipe)); // EXPECT: Full: 1

    // Try to write when full
    result = pipe_write(&pipe, 50);
    printf("Write when full: %d\n", result); // EXPECT: Write when full: 0

    // Read all items
    result = pipe_read(&pipe, &val);
    printf("Read: %d (status=%d)\n", val, result); // EXPECT: Read: 10 (status=1)
    result = pipe_read(&pipe, &val);
    printf("Read: %d (status=%d)\n", val, result); // EXPECT: Read: 20 (status=1)
    result = pipe_read(&pipe, &val);
    printf("Read: %d (status=%d)\n", val, result); // EXPECT: Read: 30 (status=1)
    result = pipe_read(&pipe, &val);
    printf("Read: %d (status=%d)\n", val, result); // EXPECT: Read: 40 (status=1)

    printf("Empty: %d\n", pipe_is_empty(&pipe)); // EXPECT: Empty: 1

    // Read from empty pipe (not closed)
    result = pipe_read(&pipe, &val);
    printf("Read empty: %d\n", result); // EXPECT: Read empty: 0

    // Close and read
    pipe_close(&pipe);
    result = pipe_read(&pipe, &val);
    printf("Read closed empty: %d\n", result); // EXPECT: Read closed empty: -1
    result = pipe_write(&pipe, 99);
    printf("Write closed: %d\n", result); // EXPECT: Write closed: -1

    printf("Stats: written=%d read=%d\n", pipe.total_written, pipe.total_read); // EXPECT: Stats: written=4 read=4

    printf("=== Circular Wrap-around ===\n"); // EXPECT: === Circular Wrap-around ===
    pipe_init(&pipe, 4);
    // Write 3, read 3, write 3 more (wraps around)
    pipe_write(&pipe, 1);
    pipe_write(&pipe, 2);
    pipe_write(&pipe, 3);
    pipe_read(&pipe, &val); // read 1
    pipe_read(&pipe, &val); // read 2
    pipe_read(&pipe, &val); // read 3
    // Now read_pos=3, write_pos=3
    pipe_write(&pipe, 4);
    pipe_write(&pipe, 5);
    pipe_write(&pipe, 6);
    // write_pos should have wrapped: 3->0->1->2... but capacity is 4
    pipe_read(&pipe, &val);
    printf("Wrap read: %d\n", val); // EXPECT: Wrap read: 4
    pipe_read(&pipe, &val);
    printf("Wrap read: %d\n", val); // EXPECT: Wrap read: 5
    pipe_read(&pipe, &val);
    printf("Wrap read: %d\n", val); // EXPECT: Wrap read: 6
    printf("After wrap: count=%d\n", pipe.count); // EXPECT: After wrap: count=0

    printf("=== Pipe Chain ===\n"); // EXPECT: === Pipe Chain ===
    chain_init(&chain, 3, 8);

    // Write values 1-5 into first pipe
    for (i = 1; i <= 5; i = i + 1) {
        pipe_write(&chain.pipes[0], i);
    }
    printf("Chain input: 1 2 3 4 5\n"); // EXPECT: Chain input: 1 2 3 4 5

    // Process through chain (each stage doubles)
    chain_process(&chain);

    // Read from last pipe
    printf("Chain output:");
    sum = 0;
    while (1) {
        result = pipe_read(&chain.pipes[2], &val);
        if (result <= 0) break;
        printf(" %d", val);
        sum = sum + val;
    }
    printf("\n"); // EXPECT: Chain output: 4 8 12 16 20
    printf("Sum of output: %d\n", sum); // EXPECT: Sum of output: 60

    printf("Pipe buffer simulation complete\n"); // EXPECT: Pipe buffer simulation complete
    return 0;
}
