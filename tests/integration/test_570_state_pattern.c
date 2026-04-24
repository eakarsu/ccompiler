int printf(const char *fmt, ...);
// EXPECT: Initial: IDLE\nNormal lifecycle:\n  IDLE + START -> RUNNING\n  RUNNING + PAUSE -> PAUSED\n  PAUSED + RESUME -> RUNNING\n  RUNNING + STOP -> STOPPED\n  STOPPED + RESET -> IDLE\nError path:\n  IDLE + START -> RUNNING\n  RUNNING + ERROR -> ERROR\n  ERROR + RESET -> IDLE\nInvalid transitions:\n  [INVALID] IDLE + PAUSE -> stays IDLE\n  [INVALID] IDLE + STOP -> stays IDLE\nStats: 8 transitions, 2 invalid

enum State {
    STATE_IDLE = 0,
    STATE_RUNNING = 1,
    STATE_PAUSED = 2,
    STATE_STOPPED = 3,
    STATE_ERROR = 4
};

enum Event {
    EVT_START = 0,
    EVT_PAUSE = 1,
    EVT_RESUME = 2,
    EVT_STOP = 3,
    EVT_ERROR = 4,
    EVT_RESET = 5
};

typedef int (*TransitionFn)(int current_state, int event);

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
    if (e == EVT_ERROR) return "ERROR";
    if (e == EVT_RESET) return "RESET";
    return "UNKNOWN";
}

int transition(int state, int event) {
    if (state == STATE_IDLE) {
        if (event == EVT_START) return STATE_RUNNING;
        return state;
    }
    if (state == STATE_RUNNING) {
        if (event == EVT_PAUSE) return STATE_PAUSED;
        if (event == EVT_STOP) return STATE_STOPPED;
        if (event == EVT_ERROR) return STATE_ERROR;
        return state;
    }
    if (state == STATE_PAUSED) {
        if (event == EVT_RESUME) return STATE_RUNNING;
        if (event == EVT_STOP) return STATE_STOPPED;
        return state;
    }
    if (state == STATE_STOPPED) {
        if (event == EVT_RESET) return STATE_IDLE;
        return state;
    }
    if (state == STATE_ERROR) {
        if (event == EVT_RESET) return STATE_IDLE;
        return state;
    }
    return state;
}

struct StateMachine {
    int state;
    int transitions;
    int invalid_attempts;
    TransitionFn trans_fn;
};

void sm_init(struct StateMachine *sm) {
    sm->state = STATE_IDLE;
    sm->transitions = 0;
    sm->invalid_attempts = 0;
    sm->trans_fn = transition;
}

void sm_send(struct StateMachine *sm, int event) {
    int old = sm->state;
    int new_state = sm->trans_fn(old, event);
    if (new_state == old && event != EVT_RESET) {
        sm->invalid_attempts++;
        printf("  [INVALID] %s + %s -> stays %s\n",
               state_name(old), event_name(event), state_name(old));
    } else {
        sm->state = new_state;
        sm->transitions++;
        printf("  %s + %s -> %s\n",
               state_name(old), event_name(event), state_name(new_state));
    }
}

int main(void) {
    struct StateMachine sm;
    sm_init(&sm);
    printf("Initial: %s\n", state_name(sm.state));

    printf("Normal lifecycle:\n");
    sm_send(&sm, EVT_START);
    sm_send(&sm, EVT_PAUSE);
    sm_send(&sm, EVT_RESUME);
    sm_send(&sm, EVT_STOP);
    sm_send(&sm, EVT_RESET);

    printf("Error path:\n");
    sm_send(&sm, EVT_START);
    sm_send(&sm, EVT_ERROR);
    sm_send(&sm, EVT_RESET);

    printf("Invalid transitions:\n");
    sm_send(&sm, EVT_PAUSE);
    sm_send(&sm, EVT_STOP);

    printf("Stats: %d transitions, %d invalid\n",
           sm.transitions, sm.invalid_attempts);

    return 0;
}
