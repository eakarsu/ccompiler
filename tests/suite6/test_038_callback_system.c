int printf(const char *fmt, ...);

typedef void (*EventHandler)(int event_id, int data);
typedef int (*Filter)(int value);
typedef int (*Transform)(int value);

int g_log[32];
int g_log_count;

void log_reset(void) {
    g_log_count = 0;
}

void log_add(int val) {
    if (g_log_count < 32) {
        g_log[g_log_count] = val;
        g_log_count = g_log_count + 1;
    }
}

int log_get(int idx) {
    if (idx < 0 || idx >= g_log_count) return -1;
    return g_log[idx];
}

typedef struct {
    EventHandler handlers[8];
    int handler_count;
} EventSystem;

void event_system_init(EventSystem *sys) {
    sys->handler_count = 0;
}

int event_register(EventSystem *sys, EventHandler handler) {
    if (sys->handler_count >= 8) return 0;
    sys->handlers[sys->handler_count] = handler;
    sys->handler_count = sys->handler_count + 1;
    return 1;
}

void event_notify_all(EventSystem *sys, int event_id, int data) {
    int i;
    for (i = 0; i < sys->handler_count; i = i + 1) {
        sys->handlers[i](event_id, data);
    }
}

void handler_log(int event_id, int data) {
    log_add(event_id * 100 + data);
}

void handler_double(int event_id, int data) {
    log_add(data * 2);
}

void handler_negate(int event_id, int data) {
    log_add(-data);
}

typedef struct {
    Filter filters[4];
    int filter_count;
} FilterChain;

void filter_chain_init(FilterChain *fc) {
    fc->filter_count = 0;
}

int filter_chain_add(FilterChain *fc, Filter f) {
    if (fc->filter_count >= 4) return 0;
    fc->filters[fc->filter_count] = f;
    fc->filter_count = fc->filter_count + 1;
    return 1;
}

int filter_chain_apply(FilterChain *fc, int value) {
    int i;
    for (i = 0; i < fc->filter_count; i = i + 1) {
        if (!fc->filters[i](value)) return 0;
    }
    return 1;
}

int filter_count_passing(FilterChain *fc, int arr[], int len) {
    int count = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (filter_chain_apply(fc, arr[i])) count = count + 1;
    }
    return count;
}

int is_positive(int v) { return v > 0; }
int is_less_than_100(int v) { return v < 100; }
int is_even(int v) { return (v % 2) == 0; }
int is_nonzero(int v) { return v != 0; }

typedef struct {
    Transform transforms[4];
    int transform_count;
} TransformPipeline;

void pipeline_init(TransformPipeline *tp) {
    tp->transform_count = 0;
}

int pipeline_add(TransformPipeline *tp, Transform t) {
    if (tp->transform_count >= 4) return 0;
    tp->transforms[tp->transform_count] = t;
    tp->transform_count = tp->transform_count + 1;
    return 1;
}

int pipeline_run(TransformPipeline *tp, int value) {
    int result = value;
    int i;
    for (i = 0; i < tp->transform_count; i = i + 1) {
        result = tp->transforms[i](result);
    }
    return result;
}

int double_val(int v) { return v * 2; }
int add_ten(int v) { return v + 10; }
int square_val(int v) { return v * v; }
int negate_val(int v) { return -v; }

int main(void) {
    log_reset();

    EventSystem es;
    event_system_init(&es);
    event_register(&es, handler_log);
    event_register(&es, handler_double);
    event_register(&es, handler_negate);

    // EXPECT: handler count: 3
    printf("handler count: %d\n", es.handler_count);

    event_notify_all(&es, 1, 5);
    // EXPECT: log[0]: 105
    printf("log[0]: %d\n", log_get(0));

    // EXPECT: log[1]: 10
    printf("log[1]: %d\n", log_get(1));

    // EXPECT: log[2]: -5
    printf("log[2]: %d\n", log_get(2));

    // EXPECT: log count: 3
    printf("log count: %d\n", g_log_count);

    event_notify_all(&es, 2, 7);
    // EXPECT: log[3]: 207
    printf("log[3]: %d\n", log_get(3));

    // EXPECT: log[4]: 14
    printf("log[4]: %d\n", log_get(4));

    // EXPECT: log[5]: -7
    printf("log[5]: %d\n", log_get(5));

    // EXPECT: log count after 2: 6
    printf("log count after 2: %d\n", g_log_count);

    FilterChain fc;
    filter_chain_init(&fc);
    filter_chain_add(&fc, is_positive);
    filter_chain_add(&fc, is_less_than_100);

    // EXPECT: filter 50: 1
    printf("filter 50: %d\n", filter_chain_apply(&fc, 50));

    // EXPECT: filter -5: 0
    printf("filter -5: %d\n", filter_chain_apply(&fc, -5));

    // EXPECT: filter 200: 0
    printf("filter 200: %d\n", filter_chain_apply(&fc, 200));

    int data[8];
    data[0] = 5; data[1] = -3; data[2] = 50; data[3] = 200;
    data[4] = 0; data[5] = 99; data[6] = -10; data[7] = 1;
    // EXPECT: passing count: 4
    printf("passing count: %d\n", filter_count_passing(&fc, data, 8));

    filter_chain_add(&fc, is_even);
    // EXPECT: passing with even: 1
    printf("passing with even: %d\n", filter_count_passing(&fc, data, 8));

    TransformPipeline tp;
    pipeline_init(&tp);
    pipeline_add(&tp, double_val);
    pipeline_add(&tp, add_ten);
    // EXPECT: pipe 5: 20
    printf("pipe 5: %d\n", pipeline_run(&tp, 5));

    // EXPECT: pipe 0: 10
    printf("pipe 0: %d\n", pipeline_run(&tp, 0));

    TransformPipeline tp2;
    pipeline_init(&tp2);
    pipeline_add(&tp2, add_ten);
    pipeline_add(&tp2, double_val);
    // EXPECT: pipe2 5: 30
    printf("pipe2 5: %d\n", pipeline_run(&tp2, 5));

    TransformPipeline tp3;
    pipeline_init(&tp3);
    pipeline_add(&tp3, double_val);
    pipeline_add(&tp3, double_val);
    pipeline_add(&tp3, double_val);
    // EXPECT: triple double 3: 24
    printf("triple double 3: %d\n", pipeline_run(&tp3, 3));

    // EXPECT: triple double 1: 8
    printf("triple double 1: %d\n", pipeline_run(&tp3, 1));

    TransformPipeline tp4;
    pipeline_init(&tp4);
    pipeline_add(&tp4, negate_val);
    pipeline_add(&tp4, add_ten);
    // EXPECT: negate+add10 3: 7
    printf("negate+add10 3: %d\n", pipeline_run(&tp4, 3));

    return 0;
}
