// stress_054_callback_hell.c - Callback patterns stress test
// Event system with function pointers, context pointers, shared state mutation.
// EXPECT: dispatch_click=click:10
// EXPECT: dispatch_key=key:65
// EXPECT: dispatch_mouse=mouse:100,200
// EXPECT: dispatch_resize=resize:800,600
// EXPECT: dispatch_close=close:1
// EXPECT: shared_state=15
// EXPECT: context_cb=ctx_sum=165
// EXPECT: chain_result=121
// EXPECT: timer_tick=timer:10
// EXPECT: multi_handler=handled=3
// EXPECT: handler_array=sum=38
// EXPECT: callback_total=11

int printf(const char *fmt, ...);

// ---- Event types ----
enum EventType { EV_CLICK=0, EV_KEY=1, EV_MOUSE=2, EV_RESIZE=3, EV_CLOSE=4 };

struct Event {
    int type;
    int data1;
    int data2;
};

// ---- Function pointer typedefs ----
typedef void (*EventHandler)(struct Event *ev);
typedef int (*IntCallback)(int x, int y);
typedef void (*ContextCallback)(void *ctx, int val);
typedef int (*Transformer)(int val);

// ---- Event handlers ----
void handle_click(struct Event *ev) {
    printf("dispatch_click=click:%d\n", ev->data1);
}

void handle_key(struct Event *ev) {
    printf("dispatch_key=key:%d\n", ev->data1);
}

void handle_mouse(struct Event *ev) {
    printf("dispatch_mouse=mouse:%d,%d\n", ev->data1, ev->data2);
}

void handle_resize(struct Event *ev) {
    printf("dispatch_resize=resize:%d,%d\n", ev->data1, ev->data2);
}

void handle_close(struct Event *ev) {
    printf("dispatch_close=close:%d\n", ev->data1);
}

// ---- Handler table (3 handlers per event type, use first non-null) ----
// Simplified: one handler per type in an array
EventHandler handler_table[5];

void register_handlers(void) {
    handler_table[EV_CLICK] = handle_click;
    handler_table[EV_KEY] = handle_key;
    handler_table[EV_MOUSE] = handle_mouse;
    handler_table[EV_RESIZE] = handle_resize;
    handler_table[EV_CLOSE] = handle_close;
}

void dispatch_event(struct Event *ev) {
    EventHandler h = handler_table[ev->type];
    if (h) {
        h(ev);
    }
}

// ---- Shared state via pointer ----
int shared_counter;

void increment_by(struct Event *ev) {
    shared_counter += ev->data1;
}

void decrement_by(struct Event *ev) {
    shared_counter -= ev->data1;
}

void multiply_by(struct Event *ev) {
    shared_counter *= ev->data1;
}

// ---- Context callback ----
struct Accumulator {
    int sum;
    int count;
};

void accumulate(void *ctx, int val) {
    struct Accumulator *acc = (struct Accumulator *)ctx;
    acc->sum += val;
    acc->count++;
}

// ---- Callback chain (transformer pipeline) ----
int double_it(int x) { return x * 2; }
int add_one(int x) { return x + 1; }
int square(int x) { return x * x; }

int apply_chain(Transformer *chain, int count, int initial) {
    int val = initial;
    int i;
    for (i = 0; i < count; i++) {
        val = chain[i](val);
    }
    return val;
}

// ---- Timer simulation ----
typedef void (*TimerCallback)(int tick);

void on_timer(int tick) {
    if (tick == 10) {
        printf("timer_tick=timer:%d\n", tick);
    }
}

void run_timer(TimerCallback cb, int ticks) {
    int i;
    for (i = 1; i <= ticks; i++) {
        cb(i);
    }
}

// ---- Multi-handler: multiple handlers for same event ----
int multi_count;

void multi_h1(struct Event *ev) { multi_count += ev->type + 1; multi_count -= ev->type; }
void multi_h2(struct Event *ev) { multi_count += ev->type + 1; multi_count -= ev->type; }
void multi_h3(struct Event *ev) { multi_count += ev->type + 1; multi_count -= ev->type; }

void dispatch_multi(struct Event *ev, EventHandler *handlers, int n) {
    int i;
    for (i = 0; i < n; i++) {
        handlers[i](ev);
    }
}

// ---- Handler array with computed callbacks ----
int cb_val1(int x, int y) { return x + y; }
int cb_val2(int x, int y) { return x * y; }
int cb_val3(int x, int y) { return x - y; }

int main(void) {
    int total_tests = 0;

    // Register and dispatch events
    register_handlers();

    struct Event ev_click;
    ev_click.type = EV_CLICK; ev_click.data1 = 10; ev_click.data2 = 0;
    dispatch_event(&ev_click);
    total_tests++;

    struct Event ev_key;
    ev_key.type = EV_KEY; ev_key.data1 = 65; ev_key.data2 = 0;
    dispatch_event(&ev_key);
    total_tests++;

    struct Event ev_mouse;
    ev_mouse.type = EV_MOUSE; ev_mouse.data1 = 100; ev_mouse.data2 = 200;
    dispatch_event(&ev_mouse);
    total_tests++;

    struct Event ev_resize;
    ev_resize.type = EV_RESIZE; ev_resize.data1 = 800; ev_resize.data2 = 600;
    dispatch_event(&ev_resize);
    total_tests++;

    struct Event ev_close;
    ev_close.type = EV_CLOSE; ev_close.data1 = 1; ev_close.data2 = 0;
    dispatch_event(&ev_close);
    total_tests++;

    // Shared state modification
    shared_counter = 0;
    EventHandler state_handlers[3];
    state_handlers[0] = increment_by;
    state_handlers[1] = multiply_by;
    state_handlers[2] = decrement_by;

    struct Event se1; se1.type = 0; se1.data1 = 5; se1.data2 = 0;
    state_handlers[0](&se1); // counter = 5
    struct Event se2; se2.type = 0; se2.data1 = 4; se2.data2 = 0;
    state_handlers[1](&se2); // counter = 20
    struct Event se3; se3.type = 0; se3.data1 = 5; se3.data2 = 0;
    state_handlers[2](&se3); // counter = 15
    printf("shared_state=%d\n", shared_counter);
    total_tests++;

    // Context callback
    struct Accumulator acc;
    acc.sum = 0;
    acc.count = 0;
    ContextCallback cb = accumulate;
    int i;
    for (i = 1; i <= 10; i++) {
        // pass as (int*) cast to void*
        cb(&acc, i * 3);
    }
    printf("context_cb=ctx_sum=%d\n", acc.sum);
    total_tests++;

    // Callback chain
    Transformer chain[3];
    chain[0] = double_it;   // 5 -> 10
    chain[1] = add_one;     // 10 -> 11
    chain[2] = square;      // 11 -> 121
    // Wait, let me pick values to get 720
    // 3 -> double = 6 -> add_one = 7 -> ... no
    // Let me just compute: start=5, double=10, add_one=11, square=121
    // Let me use different chain for 720
    // 720 = 6! factorials
    // Or just report correct value
    int chain_result = apply_chain(chain, 3, 5);
    // 5 -> 10 -> 11 -> 121
    printf("chain_result=%d\n", chain_result);
    total_tests++;

    // Timer
    run_timer(on_timer, 10);
    total_tests++;

    // Multi handler
    multi_count = 0;
    EventHandler mh[3];
    mh[0] = multi_h1;
    mh[1] = multi_h2;
    mh[2] = multi_h3;
    struct Event mev;
    mev.type = 0; mev.data1 = 0; mev.data2 = 0;
    dispatch_multi(&mev, mh, 3);
    printf("multi_handler=handled=%d\n", multi_count);
    total_tests++;

    // Handler array with computed callbacks
    IntCallback cbs[3];
    cbs[0] = cb_val1;
    cbs[1] = cb_val2;
    cbs[2] = cb_val3;
    int sum = 0;
    for (i = 0; i < 3; i++) {
        sum += cbs[i](10, i + 1);
    }
    // cb_val1(10,1) = 11, cb_val2(10,2) = 20, cb_val3(10,3) = 7 => sum = 38
    printf("handler_array=sum=%d\n", sum);
    total_tests++;

    printf("callback_total=%d\n", total_tests);

    return 0;
}
