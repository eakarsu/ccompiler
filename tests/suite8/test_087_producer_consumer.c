int printf(const char *fmt, ...);

struct Buffer {
    int data[8];
    int head;
    int tail;
    int count;
    int capacity;
};

struct Producer {
    int id;
    int items_produced;
    int next_item;
};

struct Consumer {
    int id;
    int items_consumed;
    int sum_consumed;
};

struct Simulation {
    struct Buffer buf;
    struct Producer producers[3];
    struct Consumer consumers[3];
    int num_producers;
    int num_consumers;
    int total_produced;
    int total_consumed;
    int tick;
};

void init_buffer(struct Buffer *b, int capacity) {
    int i;
    b->head = 0;
    b->tail = 0;
    b->count = 0;
    b->capacity = capacity;
    for (i = 0; i < 8; i = i + 1) {
        b->data[i] = 0;
    }
}

int buffer_full(struct Buffer *b) {
    return b->count >= b->capacity;
}

int buffer_empty(struct Buffer *b) {
    return b->count <= 0;
}

int buffer_push(struct Buffer *b, int item) {
    if (buffer_full(b)) return 0;
    b->data[b->tail] = item;
    b->tail = (b->tail + 1) % b->capacity;
    b->count = b->count + 1;
    return 1;
}

int buffer_pop(struct Buffer *b, int *item) {
    if (buffer_empty(b)) return 0;
    *item = b->data[b->head];
    b->head = (b->head + 1) % b->capacity;
    b->count = b->count - 1;
    return 1;
}

void init_simulation(struct Simulation *sim, int np, int nc, int cap) {
    int i;
    init_buffer(&sim->buf, cap);
    sim->num_producers = np;
    sim->num_consumers = nc;
    sim->total_produced = 0;
    sim->total_consumed = 0;
    sim->tick = 0;
    for (i = 0; i < np; i = i + 1) {
        sim->producers[i].id = i;
        sim->producers[i].items_produced = 0;
        sim->producers[i].next_item = (i + 1) * 100;
    }
    for (i = 0; i < nc; i = i + 1) {
        sim->consumers[i].id = i;
        sim->consumers[i].items_consumed = 0;
        sim->consumers[i].sum_consumed = 0;
    }
}

void produce_step(struct Simulation *sim) {
    int i;
    for (i = 0; i < sim->num_producers; i = i + 1) {
        int item = sim->producers[i].next_item;
        if (buffer_push(&sim->buf, item)) {
            printf("P%d produced %d\n", i, item);
            sim->producers[i].items_produced = sim->producers[i].items_produced + 1;
            sim->producers[i].next_item = sim->producers[i].next_item + 1;
            sim->total_produced = sim->total_produced + 1;
        }
    }
}

void consume_step(struct Simulation *sim) {
    int i;
    for (i = 0; i < sim->num_consumers; i = i + 1) {
        int item;
        if (buffer_pop(&sim->buf, &item)) {
            printf("C%d consumed %d\n", i, item);
            sim->consumers[i].items_consumed = sim->consumers[i].items_consumed + 1;
            sim->consumers[i].sum_consumed = sim->consumers[i].sum_consumed + item;
            sim->total_consumed = sim->total_consumed + 1;
        }
    }
}

void run_tick(struct Simulation *sim) {
    printf("--- Tick %d (buf=%d) ---\n", sim->tick, sim->buf.count);
    produce_step(sim);
    consume_step(sim);
    sim->tick = sim->tick + 1;
}

int main() {
    struct Simulation sim;
    init_simulation(&sim, 2, 2, 4);

    run_tick(&sim);
    // EXPECT: --- Tick 0 (buf=0) ---
    // EXPECT: P0 produced 100
    // EXPECT: P1 produced 200
    // EXPECT: C0 consumed 100
    // EXPECT: C1 consumed 200

    run_tick(&sim);
    // EXPECT: --- Tick 1 (buf=0) ---
    // EXPECT: P0 produced 101
    // EXPECT: P1 produced 201
    // EXPECT: C0 consumed 101
    // EXPECT: C1 consumed 201

    run_tick(&sim);
    // EXPECT: --- Tick 2 (buf=0) ---
    // EXPECT: P0 produced 102
    // EXPECT: P1 produced 202
    // EXPECT: C0 consumed 102
    // EXPECT: C1 consumed 202

    printf("Total produced: %d\n", sim.total_produced);
    // EXPECT: Total produced: 6
    printf("Total consumed: %d\n", sim.total_consumed);
    // EXPECT: Total consumed: 6
    printf("Buffer count: %d\n", sim.buf.count);
    // EXPECT: Buffer count: 0

    printf("P0: %d items, P1: %d items\n",
           sim.producers[0].items_produced, sim.producers[1].items_produced);
    // EXPECT: P0: 3 items, P1: 3 items
    printf("C0: %d items sum=%d\n", sim.consumers[0].items_consumed, sim.consumers[0].sum_consumed);
    // EXPECT: C0: 3 items sum=303
    printf("C1: %d items sum=%d\n", sim.consumers[1].items_consumed, sim.consumers[1].sum_consumed);
    // EXPECT: C1: 3 items sum=603

    printf("=== Buffer stress ===\n");
    // EXPECT: === Buffer stress ===
    struct Buffer b;
    init_buffer(&b, 4);
    int pushed = 0;
    int i;
    for (i = 0; i < 6; i = i + 1) {
        pushed = pushed + buffer_push(&b, i * 10);
    }
    printf("Pushed %d of 6\n", pushed);
    // EXPECT: Pushed 4 of 6
    printf("Full: %d\n", buffer_full(&b));
    // EXPECT: Full: 1
    int val;
    buffer_pop(&b, &val);
    printf("Popped: %d\n", val);
    // EXPECT: Popped: 0
    buffer_pop(&b, &val);
    printf("Popped: %d\n", val);
    // EXPECT: Popped: 10
    printf("Count: %d\n", b.count);
    // EXPECT: Count: 2

    return 0;
}
