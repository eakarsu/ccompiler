int printf(const char *fmt, ...);

enum State {
    ST_IDLE,
    ST_RUNNING,
    ST_PAUSED,
    ST_ERROR,
    ST_DONE,
    ST_COUNT
};

enum Event {
    EV_START,
    EV_PAUSE,
    EV_RESUME,
    EV_ERROR,
    EV_RESET,
    EV_TICK,
    EV_COUNT
};

typedef int (*StateHandler)(int event, int *data);

int g_transitions;

int state_idle(int event, int *data) {
    g_transitions = g_transitions + 1;
    if (event == EV_START) {
        *data = 0;
        return ST_RUNNING;
    }
    return ST_IDLE;
}

int state_running(int event, int *data) {
    g_transitions = g_transitions + 1;
    if (event == EV_PAUSE) {
        return ST_PAUSED;
    }
    if (event == EV_ERROR) {
        return ST_ERROR;
    }
    if (event == EV_TICK) {
        *data = *data + 1;
        if (*data >= 10) {
            return ST_DONE;
        }
        return ST_RUNNING;
    }
    return ST_RUNNING;
}

int state_paused(int event, int *data) {
    g_transitions = g_transitions + 1;
    if (event == EV_RESUME) {
        return ST_RUNNING;
    }
    if (event == EV_RESET) {
        *data = 0;
        return ST_IDLE;
    }
    return ST_PAUSED;
}

int state_error(int event, int *data) {
    g_transitions = g_transitions + 1;
    if (event == EV_RESET) {
        *data = 0;
        return ST_IDLE;
    }
    return ST_ERROR;
}

int state_done(int event, int *data) {
    g_transitions = g_transitions + 1;
    if (event == EV_RESET) {
        *data = 0;
        return ST_IDLE;
    }
    return ST_DONE;
}

struct StateMachine {
    StateHandler handlers[5];
    int current;
    int data;
};

void sm_init(struct StateMachine *sm) {
    sm->handlers[ST_IDLE] = state_idle;
    sm->handlers[ST_RUNNING] = state_running;
    sm->handlers[ST_PAUSED] = state_paused;
    sm->handlers[ST_ERROR] = state_error;
    sm->handlers[ST_DONE] = state_done;
    sm->current = ST_IDLE;
    sm->data = 0;
}

int sm_process(struct StateMachine *sm, int event) {
    int next;
    next = sm->handlers[sm->current](event, &sm->data);
    sm->current = next;
    return next;
}

int sm_run_events(struct StateMachine *sm, int events[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        sm_process(sm, events[i]);
    }
    return sm->current;
}

int state_name(int st) {
    if (st == ST_IDLE) return 0;
    if (st == ST_RUNNING) return 1;
    if (st == ST_PAUSED) return 2;
    if (st == ST_ERROR) return 3;
    if (st == ST_DONE) return 4;
    return -1;
}

int main(void) {
    struct StateMachine sm;
    int events[20];
    int st;
    int i;

    g_transitions = 0;
    sm_init(&sm);
    // EXPECT: initial: 0
    printf("initial: %d\n", sm.current);

    st = sm_process(&sm, EV_START);
    // EXPECT: after start: 1
    printf("after start: %d\n", st);
    // EXPECT: data: 0
    printf("data: %d\n", sm.data);

    st = sm_process(&sm, EV_TICK);
    // EXPECT: after tick: 1
    printf("after tick: %d\n", st);
    // EXPECT: data: 1
    printf("data: %d\n", sm.data);

    for (i = 0; i < 5; i++) {
        sm_process(&sm, EV_TICK);
    }
    // EXPECT: after 5 ticks: data=6
    printf("after 5 ticks: data=%d\n", sm.data);

    st = sm_process(&sm, EV_PAUSE);
    // EXPECT: paused: 2
    printf("paused: %d\n", st);

    st = sm_process(&sm, EV_TICK);
    // EXPECT: tick while paused: 2
    printf("tick while paused: %d\n", st);
    // EXPECT: data unchanged: 6
    printf("data unchanged: %d\n", sm.data);

    st = sm_process(&sm, EV_RESUME);
    // EXPECT: resumed: 1
    printf("resumed: %d\n", st);

    for (i = 0; i < 4; i++) {
        st = sm_process(&sm, EV_TICK);
    }
    // EXPECT: after 4 more: state=4 data=10
    printf("after 4 more: state=%d data=%d\n", st, sm.data);

    st = sm_process(&sm, EV_TICK);
    // EXPECT: tick when done: 4
    printf("tick when done: %d\n", st);

    st = sm_process(&sm, EV_RESET);
    // EXPECT: reset: 0
    printf("reset: %d\n", st);
    // EXPECT: data reset: 0
    printf("data reset: %d\n", sm.data);

    // EXPECT: transitions: 16
    printf("transitions: %d\n", g_transitions);

    sm_init(&sm);
    g_transitions = 0;
    events[0] = EV_START;
    events[1] = EV_ERROR;
    st = sm_run_events(&sm, events, 2);
    // EXPECT: error path: 3
    printf("error path: %d\n", st);

    st = sm_process(&sm, EV_TICK);
    // EXPECT: tick in error: 3
    printf("tick in error: %d\n", st);

    st = sm_process(&sm, EV_RESET);
    // EXPECT: error reset: 0
    printf("error reset: %d\n", st);

    return 0;
}
