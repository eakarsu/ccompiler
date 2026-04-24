int printf(const char *fmt, ...);

enum State {
    STATE_IDLE = 0,
    STATE_RUNNING = 1,
    STATE_PAUSED = 2,
    STATE_ERROR = 3,
    STATE_DONE = 4
};

enum Event {
    EVT_START = 0,
    EVT_PAUSE = 1,
    EVT_RESUME = 2,
    EVT_ERROR = 3,
    EVT_RESET = 4,
    EVT_FINISH = 5
};

char *state_name(enum State s) {
    switch (s) {
        case STATE_IDLE: return "IDLE";
        case STATE_RUNNING: return "RUNNING";
        case STATE_PAUSED: return "PAUSED";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

char *event_name(enum Event e) {
    switch (e) {
        case EVT_START: return "START";
        case EVT_PAUSE: return "PAUSE";
        case EVT_RESUME: return "RESUME";
        case EVT_ERROR: return "ERROR";
        case EVT_RESET: return "RESET";
        case EVT_FINISH: return "FINISH";
        default: return "UNKNOWN";
    }
}

enum State transition(enum State current, enum Event event) {
    switch (current) {
        case STATE_IDLE:
            if (event == EVT_START) return STATE_RUNNING;
            break;
        case STATE_RUNNING:
            if (event == EVT_PAUSE) return STATE_PAUSED;
            if (event == EVT_ERROR) return STATE_ERROR;
            if (event == EVT_FINISH) return STATE_DONE;
            break;
        case STATE_PAUSED:
            if (event == EVT_RESUME) return STATE_RUNNING;
            if (event == EVT_ERROR) return STATE_ERROR;
            break;
        case STATE_ERROR:
            if (event == EVT_RESET) return STATE_IDLE;
            break;
        case STATE_DONE:
            if (event == EVT_RESET) return STATE_IDLE;
            break;
        default:
            break;
    }
    return current;
}

void test_basic_transitions() {
    enum State s = STATE_IDLE;
    // EXPECT: state=IDLE
    printf("state=%s\n", state_name(s));

    s = transition(s, EVT_START);
    // EXPECT: state=RUNNING
    printf("state=%s\n", state_name(s));

    s = transition(s, EVT_PAUSE);
    // EXPECT: state=PAUSED
    printf("state=%s\n", state_name(s));

    s = transition(s, EVT_RESUME);
    // EXPECT: state=RUNNING
    printf("state=%s\n", state_name(s));

    s = transition(s, EVT_FINISH);
    // EXPECT: state=DONE
    printf("state=%s\n", state_name(s));
}

void test_error_recovery() {
    enum State s = STATE_IDLE;
    s = transition(s, EVT_START);
    s = transition(s, EVT_ERROR);
    // EXPECT: err_state=ERROR
    printf("err_state=%s\n", state_name(s));

    s = transition(s, EVT_START);
    // EXPECT: err_no_start=ERROR
    printf("err_no_start=%s\n", state_name(s));

    s = transition(s, EVT_RESET);
    // EXPECT: after_reset=IDLE
    printf("after_reset=%s\n", state_name(s));

    s = transition(s, EVT_START);
    // EXPECT: recovered=RUNNING
    printf("recovered=%s\n", state_name(s));
}

void test_invalid_transitions() {
    enum State s = STATE_IDLE;

    s = transition(s, EVT_PAUSE);
    // EXPECT: idle_pause=IDLE
    printf("idle_pause=%s\n", state_name(s));

    s = transition(s, EVT_FINISH);
    // EXPECT: idle_finish=IDLE
    printf("idle_finish=%s\n", state_name(s));

    s = transition(s, EVT_RESUME);
    // EXPECT: idle_resume=IDLE
    printf("idle_resume=%s\n", state_name(s));
}

void test_event_sequence() {
    enum State s = STATE_IDLE;
    enum Event events[8];
    events[0] = EVT_START;
    events[1] = EVT_PAUSE;
    events[2] = EVT_RESUME;
    events[3] = EVT_PAUSE;
    events[4] = EVT_RESUME;
    events[5] = EVT_ERROR;
    events[6] = EVT_RESET;
    events[7] = EVT_START;

    int i;
    for (i = 0; i < 8; i++) {
        enum State prev = s;
        s = transition(s, events[i]);
        // EXPECT: IDLE + START -> RUNNING
        // EXPECT: RUNNING + PAUSE -> PAUSED
        // EXPECT: PAUSED + RESUME -> RUNNING
        // EXPECT: RUNNING + PAUSE -> PAUSED
        // EXPECT: PAUSED + RESUME -> RUNNING
        // EXPECT: RUNNING + ERROR -> ERROR
        // EXPECT: ERROR + RESET -> IDLE
        // EXPECT: IDLE + START -> RUNNING
        printf("%s + %s -> %s\n", state_name(prev), event_name(events[i]), state_name(s));
    }
}

void test_state_counter() {
    enum State s = STATE_IDLE;
    int counts[5];
    int j;
    for (j = 0; j < 5; j++) counts[j] = 0;

    enum Event script[10];
    script[0] = EVT_START;
    script[1] = EVT_PAUSE;
    script[2] = EVT_RESUME;
    script[3] = EVT_PAUSE;
    script[4] = EVT_RESUME;
    script[5] = EVT_PAUSE;
    script[6] = EVT_RESUME;
    script[7] = EVT_FINISH;
    script[8] = EVT_RESET;
    script[9] = EVT_START;

    counts[s] = counts[s] + 1;
    int i;
    for (i = 0; i < 10; i++) {
        s = transition(s, script[i]);
        counts[s] = counts[s] + 1;
    }

    // EXPECT: visits: IDLE=2 RUNNING=5 PAUSED=3 ERROR=0 DONE=1
    printf("visits: IDLE=%d RUNNING=%d PAUSED=%d ERROR=%d DONE=%d\n",
        counts[0], counts[1], counts[2], counts[3], counts[4]);
}

int main() {
    test_basic_transitions();
    test_error_recovery();
    test_invalid_transitions();
    test_event_sequence();
    test_state_counter();
    // EXPECT: done
    printf("done\n");
    return 0;
}
