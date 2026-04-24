int printf(const char *fmt, ...);

/* State machine with enum states */
enum State {
    STATE_IDLE = 0,
    STATE_RUNNING = 1,
    STATE_PAUSED = 2,
    STATE_STOPPED = 3,
    STATE_ERROR = 4,
    STATE_COUNT = 5
};

enum Event {
    EVT_START = 0,
    EVT_PAUSE = 1,
    EVT_RESUME = 2,
    EVT_STOP = 3,
    EVT_RESET = 4,
    EVT_FAIL = 5,
    EVT_COUNT = 6
};

/* Transition table: transition[state][event] = next_state, -1 means invalid */
int transition[5][6];

void init_transitions(void) {
    int i;
    int j;
    for (i = 0; i < 5; i++)
        for (j = 0; j < 6; j++)
            transition[i][j] = -1;

    /* IDLE transitions */
    transition[STATE_IDLE][EVT_START] = STATE_RUNNING;
    transition[STATE_IDLE][EVT_FAIL] = STATE_ERROR;

    /* RUNNING transitions */
    transition[STATE_RUNNING][EVT_PAUSE] = STATE_PAUSED;
    transition[STATE_RUNNING][EVT_STOP] = STATE_STOPPED;
    transition[STATE_RUNNING][EVT_FAIL] = STATE_ERROR;

    /* PAUSED transitions */
    transition[STATE_PAUSED][EVT_RESUME] = STATE_RUNNING;
    transition[STATE_PAUSED][EVT_STOP] = STATE_STOPPED;
    transition[STATE_PAUSED][EVT_FAIL] = STATE_ERROR;

    /* STOPPED transitions */
    transition[STATE_STOPPED][EVT_RESET] = STATE_IDLE;

    /* ERROR transitions */
    transition[STATE_ERROR][EVT_RESET] = STATE_IDLE;
}

char *state_name(int s) {
    if (s == STATE_IDLE) return "IDLE";
    if (s == STATE_RUNNING) return "RUNNING";
    if (s == STATE_PAUSED) return "PAUSED";
    if (s == STATE_STOPPED) return "STOPPED";
    if (s == STATE_ERROR) return "ERROR";
    return "UNKNOWN";
}

char *event_name(int e) {
    if (e == EVT_START) return "START";
    if (e == EVT_PAUSE) return "PAUSE";
    if (e == EVT_RESUME) return "RESUME";
    if (e == EVT_STOP) return "STOP";
    if (e == EVT_RESET) return "RESET";
    if (e == EVT_FAIL) return "FAIL";
    return "UNKNOWN";
}

int process_event(int current, int event) {
    int next = transition[current][event];
    if (next == -1) return current;
    return next;
}

int is_terminal(int s) {
    return s == STATE_STOPPED || s == STATE_ERROR;
}

int count_valid_transitions(int s) {
    int count = 0;
    int e;
    for (e = 0; e < EVT_COUNT; e++) {
        if (transition[s][e] != -1) count++;
    }
    return count;
}

int main(void) {
    init_transitions();

    /* Test state names */
    // EXPECT: State: IDLE
    printf("State: %s\n", state_name(STATE_IDLE));
    // EXPECT: State: RUNNING
    printf("State: %s\n", state_name(STATE_RUNNING));
    // EXPECT: State: ERROR
    printf("State: %s\n", state_name(STATE_ERROR));

    /* Test event names */
    // EXPECT: Event: START
    printf("Event: %s\n", event_name(EVT_START));
    // EXPECT: Event: PAUSE
    printf("Event: %s\n", event_name(EVT_PAUSE));

    /* Test basic transitions */
    int state = STATE_IDLE;
    state = process_event(state, EVT_START);
    // EXPECT: After START: RUNNING
    printf("After START: %s\n", state_name(state));

    state = process_event(state, EVT_PAUSE);
    // EXPECT: After PAUSE: PAUSED
    printf("After PAUSE: %s\n", state_name(state));

    state = process_event(state, EVT_RESUME);
    // EXPECT: After RESUME: RUNNING
    printf("After RESUME: %s\n", state_name(state));

    state = process_event(state, EVT_STOP);
    // EXPECT: After STOP: STOPPED
    printf("After STOP: %s\n", state_name(state));

    state = process_event(state, EVT_RESET);
    // EXPECT: After RESET: IDLE
    printf("After RESET: %s\n", state_name(state));

    /* Test invalid transition (IDLE + PAUSE = stays IDLE) */
    state = STATE_IDLE;
    state = process_event(state, EVT_PAUSE);
    // EXPECT: Invalid: IDLE
    printf("Invalid: %s\n", state_name(state));

    /* Test error path */
    state = STATE_IDLE;
    state = process_event(state, EVT_START);
    state = process_event(state, EVT_FAIL);
    // EXPECT: Error path: ERROR
    printf("Error path: %s\n", state_name(state));

    /* Test terminal state check */
    // EXPECT: IDLE terminal: 0
    printf("IDLE terminal: %d\n", is_terminal(STATE_IDLE));
    // EXPECT: STOPPED terminal: 1
    printf("STOPPED terminal: %d\n", is_terminal(STATE_STOPPED));
    // EXPECT: ERROR terminal: 1
    printf("ERROR terminal: %d\n", is_terminal(STATE_ERROR));

    /* Count valid transitions from each state */
    // EXPECT: IDLE transitions: 2
    printf("IDLE transitions: %d\n", count_valid_transitions(STATE_IDLE));
    // EXPECT: RUNNING transitions: 3
    printf("RUNNING transitions: %d\n", count_valid_transitions(STATE_RUNNING));
    // EXPECT: PAUSED transitions: 3
    printf("PAUSED transitions: %d\n", count_valid_transitions(STATE_PAUSED));
    // EXPECT: STOPPED transitions: 1
    printf("STOPPED transitions: %d\n", count_valid_transitions(STATE_STOPPED));

    /* Process a full event sequence */
    int events[6];
    events[0] = EVT_START;
    events[1] = EVT_PAUSE;
    events[2] = EVT_RESUME;
    events[3] = EVT_PAUSE;
    events[4] = EVT_STOP;
    events[5] = EVT_RESET;
    state = STATE_IDLE;
    int i;
    for (i = 0; i < 6; i++) {
        state = process_event(state, events[i]);
    }
    // EXPECT: Final: IDLE
    printf("Final: %s\n", state_name(state));

    /* Count state visits in sequence */
    int visit_count[5];
    int k;
    for (k = 0; k < 5; k++) visit_count[k] = 0;
    state = STATE_IDLE;
    visit_count[state]++;
    for (i = 0; i < 6; i++) {
        state = process_event(state, events[i]);
        visit_count[state]++;
    }
    // EXPECT: IDLE visited: 2
    printf("IDLE visited: %d\n", visit_count[STATE_IDLE]);
    // EXPECT: RUNNING visited: 2
    printf("RUNNING visited: %d\n", visit_count[STATE_RUNNING]);
    // EXPECT: PAUSED visited: 2
    printf("PAUSED visited: %d\n", visit_count[STATE_PAUSED]);
    // EXPECT: STOPPED visited: 1
    printf("STOPPED visited: %d\n", visit_count[STATE_STOPPED]);

    return 0;
}
