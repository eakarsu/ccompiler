int printf(const char *fmt, ...);
// EXPECT: === scenario 1 ===\nInitial: IDLE\n  START: IDLE -> RUNNING\n  PAUSE: RUNNING -> PAUSED\n  RESUME: PAUSED -> RUNNING\n  STOP: RUNNING -> STOPPED\n  RESET: STOPPED -> IDLE\nFinal: IDLE\n=== scenario 2 (invalid transitions) ===\nInitial: IDLE\n  PAUSE: IDLE -> IDLE\n  STOP: IDLE -> IDLE\n  START: IDLE -> RUNNING\n  STOP: RUNNING -> STOPPED\nFinal: STOPPED\n=== scenario 3 ===\nInitial: IDLE\n  START: IDLE -> RUNNING\n  PAUSE: RUNNING -> PAUSED\n  RESUME: PAUSED -> RUNNING\n  PAUSE: RUNNING -> PAUSED\n  RESUME: PAUSED -> RUNNING\n  STOP: RUNNING -> STOPPED\n  RESET: STOPPED -> IDLE\nFinal: IDLE\n=== counter ===\ninitial=0\nafter 7 incs=5\nafter 10 decs=-3\nafter reset=0
// Test 244: State machine using function dispatch

enum State { IDLE, RUNNING, PAUSED, STOPPED };
enum Event { START, PAUSE, RESUME, STOP, RESET };

int transition(int state, int event) {
    if (state == 0) { // IDLE
        if (event == 0) return 1; // START -> RUNNING
        if (event == 4) return 0; // RESET -> IDLE
    }
    if (state == 1) { // RUNNING
        if (event == 1) return 2; // PAUSE -> PAUSED
        if (event == 3) return 3; // STOP -> STOPPED
    }
    if (state == 2) { // PAUSED
        if (event == 2) return 1; // RESUME -> RUNNING
        if (event == 3) return 3; // STOP -> STOPPED
    }
    if (state == 3) { // STOPPED
        if (event == 4) return 0; // RESET -> IDLE
    }
    return state;
}

void state_name(int state) {
    if (state == 0) printf("IDLE");
    else if (state == 1) printf("RUNNING");
    else if (state == 2) printf("PAUSED");
    else if (state == 3) printf("STOPPED");
    else printf("UNKNOWN");
}

void event_name(int event) {
    if (event == 0) printf("START");
    else if (event == 1) printf("PAUSE");
    else if (event == 2) printf("RESUME");
    else if (event == 3) printf("STOP");
    else if (event == 4) printf("RESET");
    else printf("UNKNOWN");
}

void run_machine(int *events, int n_events) {
    int state = 0;
    printf("Initial: ");
    state_name(state);
    printf("\n");
    int i = 0;
    while (i < n_events) {
        int new_state = transition(state, events[i]);
        printf("  ");
        event_name(events[i]);
        printf(": ");
        state_name(state);
        printf(" -> ");
        state_name(new_state);
        printf("\n");
        state = new_state;
        i = i + 1;
    }
    printf("Final: ");
    state_name(state);
    printf("\n");
}

// Simple counter state machine
struct Counter {
    int value;
    int min_val;
    int max_val;
};

void counter_init(struct Counter *c, int min_v, int max_v) {
    c->value = 0;
    c->min_val = min_v;
    c->max_val = max_v;
}

void counter_inc(struct Counter *c) {
    if (c->value < c->max_val) c->value = c->value + 1;
}

void counter_dec(struct Counter *c) {
    if (c->value > c->min_val) c->value = c->value - 1;
}

void counter_reset(struct Counter *c) {
    c->value = 0;
}

int main(void) {
    printf("=== scenario 1 ===\n");
    int events1[5];
    events1[0] = 0; events1[1] = 1; events1[2] = 2; events1[3] = 3; events1[4] = 4;
    run_machine(events1, 5);

    printf("=== scenario 2 (invalid transitions) ===\n");
    int events2[4];
    events2[0] = 1; events2[1] = 3; events2[2] = 0; events2[3] = 3;
    run_machine(events2, 4);

    printf("=== scenario 3 ===\n");
    int events3[7];
    events3[0] = 0; events3[1] = 1; events3[2] = 2;
    events3[3] = 1; events3[4] = 2; events3[5] = 3; events3[6] = 4;
    run_machine(events3, 7);

    printf("=== counter ===\n");
    struct Counter cnt;
    counter_init(&cnt, -3, 5);
    printf("initial=%d\n", cnt.value);
    int i = 0;
    while (i < 7) { counter_inc(&cnt); i = i + 1; }
    printf("after 7 incs=%d\n", cnt.value);
    i = 0;
    while (i < 10) { counter_dec(&cnt); i = i + 1; }
    printf("after 10 decs=%d\n", cnt.value);
    counter_reset(&cnt);
    printf("after reset=%d\n", cnt.value);

    return 0;
}
