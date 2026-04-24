int printf(const char *fmt, ...);
// EXPECT: state=IDLE ticks=0 errors=0\nstart: 1\nstate=RUNNING ticks=0 errors=0\nstate=RUNNING ticks=3 errors=0\npause: 1\nstate=PAUSED ticks=3 errors=0\nresume: 1\nstate=RUNNING ticks=5 errors=0\nerror: 1\nstate=ERROR ticks=5 errors=1\nstart from error: 0\nreset: 1\nstate=IDLE ticks=5 errors=1\nstart again: 1\nstop: 1\nstate=STOPPED ticks=5 errors=1
// Test: enum-based state machine

enum State {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_STOPPED,
    STATE_ERROR
};

enum Event {
    EVT_START,
    EVT_PAUSE,
    EVT_RESUME,
    EVT_STOP,
    EVT_ERROR,
    EVT_RESET
};

struct Machine {
    enum State state;
    int ticks;
    int errors;
};

void machine_init(struct Machine *m) {
    m->state = STATE_IDLE;
    m->ticks = 0;
    m->errors = 0;
}

void print_state(enum State s) {
    switch (s) {
        case STATE_IDLE:    printf("IDLE"); break;
        case STATE_RUNNING: printf("RUNNING"); break;
        case STATE_PAUSED:  printf("PAUSED"); break;
        case STATE_STOPPED: printf("STOPPED"); break;
        case STATE_ERROR:   printf("ERROR"); break;
    }
}

int machine_transition(struct Machine *m, enum Event evt) {
    switch (m->state) {
        case STATE_IDLE:
            if (evt == EVT_START) { m->state = STATE_RUNNING; return 1; }
            break;
        case STATE_RUNNING:
            if (evt == EVT_PAUSE) { m->state = STATE_PAUSED; return 1; }
            if (evt == EVT_STOP) { m->state = STATE_STOPPED; return 1; }
            if (evt == EVT_ERROR) { m->state = STATE_ERROR; m->errors++; return 1; }
            break;
        case STATE_PAUSED:
            if (evt == EVT_RESUME) { m->state = STATE_RUNNING; return 1; }
            if (evt == EVT_STOP) { m->state = STATE_STOPPED; return 1; }
            break;
        case STATE_ERROR:
            if (evt == EVT_RESET) { m->state = STATE_IDLE; return 1; }
            break;
        case STATE_STOPPED:
            if (evt == EVT_RESET) { m->state = STATE_IDLE; return 1; }
            break;
    }
    return 0;
}

void machine_tick(struct Machine *m) {
    if (m->state == STATE_RUNNING) {
        m->ticks = m->ticks + 1;
    }
}

void machine_print(struct Machine *m) {
    printf("state=");
    print_state(m->state);
    printf(" ticks=%d errors=%d\n", m->ticks, m->errors);
}

int main(void) {
    struct Machine m;
    machine_init(&m);
    machine_print(&m);

    printf("start: %d\n", machine_transition(&m, EVT_START));
    machine_print(&m);

    machine_tick(&m);
    machine_tick(&m);
    machine_tick(&m);
    machine_print(&m);

    printf("pause: %d\n", machine_transition(&m, EVT_PAUSE));
    machine_tick(&m);
    machine_print(&m);

    printf("resume: %d\n", machine_transition(&m, EVT_RESUME));
    machine_tick(&m);
    machine_tick(&m);
    machine_print(&m);

    printf("error: %d\n", machine_transition(&m, EVT_ERROR));
    machine_print(&m);

    printf("start from error: %d\n", machine_transition(&m, EVT_START));
    printf("reset: %d\n", machine_transition(&m, EVT_RESET));
    machine_print(&m);

    printf("start again: %d\n", machine_transition(&m, EVT_START));
    printf("stop: %d\n", machine_transition(&m, EVT_STOP));
    machine_print(&m);

    return 0;
}
