int printf(const char *fmt, ...);

enum State {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_ERROR,
    STATE_DONE
};

enum Event {
    EVT_START,
    EVT_PAUSE,
    EVT_RESUME,
    EVT_ERROR,
    EVT_RESET,
    EVT_FINISH
};

struct StateMachine {
    int current;
    int transition_count;
    int error_count;
    int history[20];
    int history_len;
};

void sm_init(struct StateMachine *sm) {
    sm->current = STATE_IDLE;
    sm->transition_count = 0;
    sm->error_count = 0;
    sm->history_len = 0;
}

void sm_record(struct StateMachine *sm, int state) {
    if (sm->history_len < 20) {
        sm->history[sm->history_len] = state;
        sm->history_len = sm->history_len + 1;
    }
}

void sm_process(struct StateMachine *sm, int event) {
    int prev = sm->current;
    switch (sm->current) {
        case STATE_IDLE:
            if (event == EVT_START) {
                sm->current = STATE_RUNNING;
            } else if (event == EVT_ERROR) {
                sm->current = STATE_ERROR;
                sm->error_count = sm->error_count + 1;
            }
            break;
        case STATE_RUNNING:
            if (event == EVT_PAUSE) {
                sm->current = STATE_PAUSED;
            } else if (event == EVT_FINISH) {
                sm->current = STATE_DONE;
            } else if (event == EVT_ERROR) {
                sm->current = STATE_ERROR;
                sm->error_count = sm->error_count + 1;
            }
            break;
        case STATE_PAUSED:
            if (event == EVT_RESUME) {
                sm->current = STATE_RUNNING;
            } else if (event == EVT_ERROR) {
                sm->current = STATE_ERROR;
                sm->error_count = sm->error_count + 1;
            }
            break;
        case STATE_ERROR:
            if (event == EVT_RESET) {
                sm->current = STATE_IDLE;
            }
            break;
        case STATE_DONE:
            if (event == EVT_RESET) {
                sm->current = STATE_IDLE;
            }
            break;
    }
    if (sm->current != prev) {
        sm->transition_count = sm->transition_count + 1;
        sm_record(sm, sm->current);
    }
}

int state_name(int s) {
    switch (s) {
        case STATE_IDLE: return 0;
        case STATE_RUNNING: return 1;
        case STATE_PAUSED: return 2;
        case STATE_ERROR: return 3;
        case STATE_DONE: return 4;
    }
    return -1;
}

int main(void) {
    struct StateMachine sm;
    sm_init(&sm);

    // EXPECT: Initial state: 0
    printf("Initial state: %d\n", state_name(sm.current));

    sm_process(&sm, EVT_START);
    // EXPECT: After START: 1
    printf("After START: %d\n", state_name(sm.current));

    sm_process(&sm, EVT_PAUSE);
    // EXPECT: After PAUSE: 2
    printf("After PAUSE: %d\n", state_name(sm.current));

    sm_process(&sm, EVT_RESUME);
    // EXPECT: After RESUME: 1
    printf("After RESUME: %d\n", state_name(sm.current));

    sm_process(&sm, EVT_FINISH);
    // EXPECT: After FINISH: 4
    printf("After FINISH: %d\n", state_name(sm.current));

    // EXPECT: Transitions so far: 4
    printf("Transitions so far: %d\n", sm.transition_count);

    sm_process(&sm, EVT_RESET);
    // EXPECT: After RESET from DONE: 0
    printf("After RESET from DONE: %d\n", state_name(sm.current));

    sm_process(&sm, EVT_START);
    sm_process(&sm, EVT_ERROR);
    // EXPECT: After ERROR: 3
    printf("After ERROR: %d\n", state_name(sm.current));

    // EXPECT: Error count: 1
    printf("Error count: %d\n", sm.error_count);

    sm_process(&sm, EVT_RESET);
    // EXPECT: After RESET from ERROR: 0
    printf("After RESET from ERROR: %d\n", state_name(sm.current));

    // EXPECT: Total transitions: 8
    printf("Total transitions: %d\n", sm.transition_count);

    // EXPECT: History length: 8
    printf("History length: %d\n", sm.history_len);

    // Check history: RUNNING, PAUSED, RUNNING, DONE, IDLE, RUNNING, ERROR, IDLE
    // EXPECT: History[0]: 1
    printf("History[0]: %d\n", state_name(sm.history[0]));
    // EXPECT: History[1]: 2
    printf("History[1]: %d\n", state_name(sm.history[1]));
    // EXPECT: History[2]: 1
    printf("History[2]: %d\n", state_name(sm.history[2]));
    // EXPECT: History[3]: 4
    printf("History[3]: %d\n", state_name(sm.history[3]));

    // Test: invalid event in IDLE should not transition
    sm_process(&sm, EVT_PAUSE);
    // EXPECT: Still IDLE after invalid event: 0
    printf("Still IDLE after invalid event: %d\n", state_name(sm.current));

    // EXPECT: Transitions unchanged: 8
    printf("Transitions unchanged: %d\n", sm.transition_count);

    // Run a second machine
    struct StateMachine sm2;
    sm_init(&sm2);
    int events[6];
    events[0] = EVT_START;
    events[1] = EVT_ERROR;
    events[2] = EVT_RESET;
    events[3] = EVT_START;
    events[4] = EVT_PAUSE;
    events[5] = EVT_RESUME;
    int i;
    for (i = 0; i < 6; i = i + 1) {
        sm_process(&sm2, events[i]);
    }
    // EXPECT: SM2 final state: 1
    printf("SM2 final state: %d\n", state_name(sm2.current));
    // EXPECT: SM2 transitions: 6
    printf("SM2 transitions: %d\n", sm2.transition_count);
    // EXPECT: SM2 errors: 1
    printf("SM2 errors: %d\n", sm2.error_count);

    return 0;
}
