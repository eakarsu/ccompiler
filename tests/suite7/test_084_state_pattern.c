int printf(const char *fmt, ...);

enum State {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_STOPPED,
    STATE_ERROR
};

typedef int (*ActionFunc)(int);

struct StateMachine {
    int state;
    int value;
    int transition_count;
    ActionFunc on_enter;
    ActionFunc on_action;
    ActionFunc on_exit;
};

int idle_enter(int v) { return 0; }
int idle_action(int v) { return v; }
int idle_exit(int v) { return v; }

int running_enter(int v) { return v + 1; }
int running_action(int v) { return v * 2; }
int running_exit(int v) { return v; }

int paused_enter(int v) { return v; }
int paused_action(int v) { return v; }
int paused_exit(int v) { return v; }

int stopped_enter(int v) { return 0; }
int stopped_action(int v) { return 0; }
int stopped_exit(int v) { return 0; }

int error_enter(int v) { return -1; }
int error_action(int v) { return -1; }
int error_exit(int v) { return 0; }

void set_state_funcs(struct StateMachine *sm) {
    if (sm->state == STATE_IDLE) {
        sm->on_enter = idle_enter;
        sm->on_action = idle_action;
        sm->on_exit = idle_exit;
    } else if (sm->state == STATE_RUNNING) {
        sm->on_enter = running_enter;
        sm->on_action = running_action;
        sm->on_exit = running_exit;
    } else if (sm->state == STATE_PAUSED) {
        sm->on_enter = paused_enter;
        sm->on_action = paused_action;
        sm->on_exit = paused_exit;
    } else if (sm->state == STATE_STOPPED) {
        sm->on_enter = stopped_enter;
        sm->on_action = stopped_action;
        sm->on_exit = stopped_exit;
    } else {
        sm->on_enter = error_enter;
        sm->on_action = error_action;
        sm->on_exit = error_exit;
    }
}

void sm_init(struct StateMachine *sm) {
    sm->state = STATE_IDLE;
    sm->value = 0;
    sm->transition_count = 0;
    set_state_funcs(sm);
    sm->value = sm->on_enter(sm->value);
}

int sm_transition(struct StateMachine *sm, int new_state) {
    sm->value = sm->on_exit(sm->value);
    sm->state = new_state;
    set_state_funcs(sm);
    sm->value = sm->on_enter(sm->value);
    sm->transition_count = sm->transition_count + 1;
    return sm->state;
}

int sm_do_action(struct StateMachine *sm) {
    sm->value = sm->on_action(sm->value);
    return sm->value;
}

char *state_name(int s) {
    if (s == STATE_IDLE) return "idle";
    if (s == STATE_RUNNING) return "running";
    if (s == STATE_PAUSED) return "paused";
    if (s == STATE_STOPPED) return "stopped";
    return "error";
}

int can_transition(int from, int to) {
    if (from == STATE_IDLE && to == STATE_RUNNING) return 1;
    if (from == STATE_RUNNING && to == STATE_PAUSED) return 1;
    if (from == STATE_RUNNING && to == STATE_STOPPED) return 1;
    if (from == STATE_PAUSED && to == STATE_RUNNING) return 1;
    if (from == STATE_PAUSED && to == STATE_STOPPED) return 1;
    if (from == STATE_STOPPED && to == STATE_IDLE) return 1;
    return 0;
}

int sm_safe_transition(struct StateMachine *sm, int new_state) {
    if (can_transition(sm->state, new_state)) {
        sm_transition(sm, new_state);
        return 1;
    }
    return 0;
}

int main(void) {
    struct StateMachine sm;
    int ok;

    sm_init(&sm);
    // EXPECT: initial: idle val=0
    printf("initial: %s val=%d\n", state_name(sm.state), sm.value);

    sm_transition(&sm, STATE_RUNNING);
    // EXPECT: after start: running val=1
    printf("after start: %s val=%d\n", state_name(sm.state), sm.value);

    sm_do_action(&sm);
    // EXPECT: after action1: running val=2
    printf("after action1: %s val=%d\n", state_name(sm.state), sm.value);

    sm_do_action(&sm);
    // EXPECT: after action2: running val=4
    printf("after action2: %s val=%d\n", state_name(sm.state), sm.value);

    sm_do_action(&sm);
    // EXPECT: after action3: running val=8
    printf("after action3: %s val=%d\n", state_name(sm.state), sm.value);

    sm_transition(&sm, STATE_PAUSED);
    // EXPECT: paused: paused val=8
    printf("paused: %s val=%d\n", state_name(sm.state), sm.value);

    sm_do_action(&sm);
    // EXPECT: paused action: val=8
    printf("paused action: val=%d\n", sm.value);

    sm_transition(&sm, STATE_RUNNING);
    // EXPECT: resumed: running val=9
    printf("resumed: %s val=%d\n", state_name(sm.state), sm.value);

    sm_do_action(&sm);
    // EXPECT: after resume action: val=18
    printf("after resume action: val=%d\n", sm.value);

    sm_transition(&sm, STATE_STOPPED);
    // EXPECT: stopped: stopped val=0
    printf("stopped: %s val=%d\n", state_name(sm.state), sm.value);

    // EXPECT: transitions: 4
    printf("transitions: %d\n", sm.transition_count);

    sm_init(&sm);
    ok = sm_safe_transition(&sm, STATE_RUNNING);
    // EXPECT: idle->running: 1
    printf("idle->running: %d\n", ok);

    ok = sm_safe_transition(&sm, STATE_IDLE);
    // EXPECT: running->idle: 0
    printf("running->idle: %d\n", ok);

    ok = sm_safe_transition(&sm, STATE_PAUSED);
    // EXPECT: running->paused: 1
    printf("running->paused: %d\n", ok);

    ok = sm_safe_transition(&sm, STATE_STOPPED);
    // EXPECT: paused->stopped: 1
    printf("paused->stopped: %d\n", ok);

    // EXPECT: final state: stopped
    printf("final state: %s\n", state_name(sm.state));

    return 0;
}
