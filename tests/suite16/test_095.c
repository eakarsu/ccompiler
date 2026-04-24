int printf(const char *fmt, ...);

// Test 095: Callback chains and event dispatch

typedef void (*Callback)(int);

struct Handler {
    Callback fn;
    int id;
};

struct EventDispatcher {
    struct Handler handlers[8];
    int count;
};

// Callback functions
int g_log[20];
int g_log_len;

void reset_log(void) {
    g_log_len = 0;
}

void log_value(int x) {
    g_log[g_log_len] = x;
    g_log_len = g_log_len + 1;
}

void log_doubled(int x) {
    g_log[g_log_len] = x * 2;
    g_log_len = g_log_len + 1;
}

void log_squared(int x) {
    g_log[g_log_len] = x * x;
    g_log_len = g_log_len + 1;
}

void log_negated(int x) {
    g_log[g_log_len] = -x;
    g_log_len = g_log_len + 1;
}

void log_plus_ten(int x) {
    g_log[g_log_len] = x + 10;
    g_log_len = g_log_len + 1;
}

void print_log(const char *label) {
    int i;
    printf("%s:", label);
    for (i = 0; i < g_log_len; i++) {
        printf(" %d", g_log[i]);
    }
    printf("\n");
}

void dispatcher_init(struct EventDispatcher *d) {
    d->count = 0;
}

void dispatcher_register(struct EventDispatcher *d, Callback fn, int id) {
    d->handlers[d->count].fn = fn;
    d->handlers[d->count].id = id;
    d->count = d->count + 1;
}

void dispatcher_fire(struct EventDispatcher *d, int value) {
    int i;
    for (i = 0; i < d->count; i++) {
        d->handlers[i].fn(value);
    }
}

void dispatcher_fire_by_id(struct EventDispatcher *d, int id, int value) {
    int i;
    for (i = 0; i < d->count; i++) {
        if (d->handlers[i].id == id) {
            d->handlers[i].fn(value);
        }
    }
}

int dispatcher_count_handlers(struct EventDispatcher *d) {
    return d->count;
}

// Pipeline: chain of callbacks applied in sequence
struct Pipeline {
    Callback steps[8];
    int count;
};

void pipeline_init(struct Pipeline *p) {
    p->count = 0;
}

void pipeline_add(struct Pipeline *p, Callback fn) {
    p->steps[p->count] = fn;
    p->count = p->count + 1;
}

void pipeline_run(struct Pipeline *p, int value) {
    int i;
    for (i = 0; i < p->count; i++) {
        p->steps[i](value);
    }
}

// Callback table: select callback by index
Callback select_callback(int index) {
    if (index == 0) return log_value;
    if (index == 1) return log_doubled;
    if (index == 2) return log_squared;
    if (index == 3) return log_negated;
    return log_plus_ten;
}

int main(void) {
    struct EventDispatcher disp;
    struct Pipeline pipe;
    int i;
    Callback cb;

    // Test dispatcher: all handlers
    dispatcher_init(&disp);
    dispatcher_register(&disp, log_value, 1);
    dispatcher_register(&disp, log_doubled, 2);
    dispatcher_register(&disp, log_squared, 3);
    printf("handlers: %d\n", dispatcher_count_handlers(&disp)); // EXPECT: handlers: 3

    reset_log();
    dispatcher_fire(&disp, 5);
    print_log("fire_5"); // EXPECT: fire_5: 5 10 25

    reset_log();
    dispatcher_fire(&disp, 3);
    print_log("fire_3"); // EXPECT: fire_3: 3 6 9

    // Fire by id
    reset_log();
    dispatcher_fire_by_id(&disp, 2, 7);
    print_log("id2_7"); // EXPECT: id2_7: 14

    reset_log();
    dispatcher_fire_by_id(&disp, 3, 4);
    print_log("id3_4"); // EXPECT: id3_4: 16

    // Test pipeline
    pipeline_init(&pipe);
    pipeline_add(&pipe, log_value);
    pipeline_add(&pipe, log_doubled);
    pipeline_add(&pipe, log_negated);
    pipeline_add(&pipe, log_plus_ten);

    reset_log();
    pipeline_run(&pipe, 8);
    print_log("pipe_8"); // EXPECT: pipe_8: 8 16 -8 18

    reset_log();
    pipeline_run(&pipe, 0);
    print_log("pipe_0"); // EXPECT: pipe_0: 0 0 0 10

    // Callback table selection
    reset_log();
    for (i = 0; i < 5; i++) {
        cb = select_callback(i);
        cb(3);
    }
    print_log("sel_3"); // EXPECT: sel_3: 3 6 9 -3 13

    // Multiple dispatchers
    reset_log();
    dispatcher_init(&disp);
    dispatcher_register(&disp, log_negated, 1);
    dispatcher_register(&disp, log_plus_ten, 1);
    dispatcher_fire(&disp, 5);
    print_log("disp2_5"); // EXPECT: disp2_5: -5 15

    // Fire multiple values through same pipeline
    reset_log();
    pipeline_init(&pipe);
    pipeline_add(&pipe, log_squared);
    pipeline_run(&pipe, 1);
    pipeline_run(&pipe, 2);
    pipeline_run(&pipe, 3);
    pipeline_run(&pipe, 4);
    print_log("sq_1234"); // EXPECT: sq_1234: 1 4 9 16

    printf("done\n"); // EXPECT: done
    return 0;
}
