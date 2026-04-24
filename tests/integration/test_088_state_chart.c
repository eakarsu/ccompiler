int printf(const char *fmt, ...);
// EXPECT: === Simple State Machine Tests ===\nSM initial: 0\nSM after power_on: 1\nSM after timer1: 3\nSM after timer2: 2\nSM after timer3: 1\nSM after timer4: 3\nSM after power_off: 0\nSM transitions: 6\nSM actions: 6\nSM action_log: 100 101 102 103 101 104\n=== Guarded Transition Tests ===\nGuarded after start (counter=0): 1\nGuarded after start (counter=1): 1\nGuarded after pause: 2\nGuarded after resume: 1\nGuarded finish attempt (counter=1): took=0 state=1\nGuarded finish (counter=5): took=1 state=3\nGuarded actions: 4\nGuarded action_log: 200 202 203 204\n=== Hierarchical State Machine Tests ===\nHSM parent of 10: 1\nHSM parent of 11: 1\nHSM parent of 0: -1\nHSM is_composite 1: 1\nHSM is_composite 10: 0\nHSM depth 10: 1\nHSM depth 1: 0\nHSM depth 0: 0\nHSM is_in 10/1: 1\nHSM is_in 10/0: 0\nHSM is_in 11/1: 1\nHSM ancestor 10,11: 1\nHSM ancestor 10,12: 1\nHSM initial: 0\nHSM after power_on: 10\nHSM after start: 11\nHSM after wait: 12\nHSM after resume: 11\nHSM after emergency: 2\nHSM after reset: 0\nHSM action_log: 300 301 302 303 304 305\n=== Event Queue Tests ===\nEQ processed: 7\nEQ final_state: 1\nEQ transitions: 7\nEQ action_count: 7\nEQ action_log: 100 101 102 103 101 102 103\n=== Complex Scenario Tests ===\nElevator processed: 10\nElevator final: 0\nElevator transitions: 10\nElevator actions: 400 402 404 406 404 405 401 403 404 405\n=== State Property Tests ===\nProp depth 10: 2\nProp depth 1: 1\nProp depth 0: 0\nProp depth 21: 2\nProp ancestor 10,11: 1\nProp ancestor 10,20: 0\nProp ancestor 10,21: 0\nProp ancestor 1,2: 0\nProp is_in 10/1: 1\nProp is_in 10/0: 1\nProp is_in 10/2: 0\nProp is_in 21/2: 1\nProp is_in 21/0: 1
// ============================================================
// test_088_state_chart.c
// State machine implementations:
//   1. Hierarchical state machine
//   2. State transition tables
//   3. Event processing
//   4. Guard conditions
//   5. Action logging
// ============================================================

// ============================================================
// State and Event definitions (using integer constants)
// ============================================================

// States for a traffic light controller
// 0 = OFF, 1 = RED, 2 = YELLOW, 3 = GREEN
// Sub-states for RED: 10 = RED_WAITING, 11 = RED_PEDESTRIAN
// Sub-states for GREEN: 30 = GREEN_NORMAL, 31 = GREEN_EXTENDED

// Events
// 0 = TIMER, 1 = BUTTON_PRESS, 2 = EMERGENCY, 3 = RESET, 4 = POWER_ON, 5 = POWER_OFF

// ============================================================
// Action Log
// ============================================================
int action_log[200];
int action_log_count;

void log_init() {
    action_log_count = 0;
}

void log_action(int action_code) {
    if (action_log_count < 200) {
        action_log[action_log_count] = action_code;
        action_log_count++;
    }
}

int log_get_count() {
    return action_log_count;
}

int log_get_action(int index) {
    if (index < 0 || index >= action_log_count) return -1;
    return action_log[index];
}

void log_print() {
    int i;
    for (i = 0; i < action_log_count; i++) {
        printf("%d", action_log[i]);
        if (i < action_log_count - 1) printf(" ");
    }
    printf("\n");
}

// ============================================================
// Transition Table
// ============================================================
// Each transition: from_state, event, to_state, guard_id, action_id
// guard_id: -1 = no guard, otherwise index into guard functions
// action_id: -1 = no action, otherwise logged action code

int trans_from[50];
int trans_event[50];
int trans_to[50];
int trans_guard[50];
int trans_action[50];
int trans_count;

void trans_init() {
    trans_count = 0;
}

void trans_add(int from, int event, int to, int guard, int action) {
    trans_from[trans_count] = from;
    trans_event[trans_count] = event;
    trans_to[trans_count] = to;
    trans_guard[trans_count] = guard;
    trans_action[trans_count] = action;
    trans_count++;
}

// ============================================================
// Guard Conditions
// ============================================================
// Guard variables (simulate conditions)
int guard_timer_expired;
int guard_pedestrian_waiting;
int guard_emergency_active;
int guard_night_mode;
int guard_counter;

void guards_init() {
    guard_timer_expired = 0;
    guard_pedestrian_waiting = 0;
    guard_emergency_active = 0;
    guard_night_mode = 0;
    guard_counter = 0;
}

// Evaluate a guard condition by ID
// Returns 1 if guard passes, 0 otherwise
int evaluate_guard(int guard_id) {
    if (guard_id == -1) return 1; // No guard, always pass
    if (guard_id == 0) return guard_timer_expired;
    if (guard_id == 1) return guard_pedestrian_waiting;
    if (guard_id == 2) return guard_emergency_active;
    if (guard_id == 3) return guard_night_mode;
    if (guard_id == 4) return guard_counter > 3;
    if (guard_id == 5) return guard_counter > 0;
    return 0;
}

// ============================================================
// Simple State Machine
// ============================================================
int sm_current_state;
int sm_transition_count;

void sm_init(int initial_state) {
    sm_current_state = initial_state;
    sm_transition_count = 0;
}

// Process an event using the transition table
// Returns 1 if a transition was taken, 0 otherwise
int sm_process_event(int event) {
    int i;
    for (i = 0; i < trans_count; i++) {
        if (trans_from[i] == sm_current_state && trans_event[i] == event) {
            // Check guard
            if (evaluate_guard(trans_guard[i])) {
                // Execute action
                if (trans_action[i] != -1) {
                    log_action(trans_action[i]);
                }
                // Transition
                sm_current_state = trans_to[i];
                sm_transition_count++;
                return 1;
            }
        }
    }
    return 0; // No valid transition found
}

int sm_get_state() {
    return sm_current_state;
}

int sm_get_transition_count() {
    return sm_transition_count;
}

// ============================================================
// Hierarchical State Machine
// ============================================================
// Parent states contain child states
// parent_of[state] = parent state, -1 if top-level

int parent_of[40];
int is_composite[40]; // 1 if state has children
int default_child[40]; // Default child when entering a composite state

void hierarchy_init() {
    int i;
    for (i = 0; i < 40; i++) {
        parent_of[i] = -1;
        is_composite[i] = 0;
        default_child[i] = -1;
    }
}

void set_parent(int child, int parent) {
    parent_of[child] = parent;
    is_composite[parent] = 1;
    if (default_child[parent] == -1) {
        default_child[parent] = child;
    }
}

void set_default_child(int parent, int child) {
    default_child[parent] = child;
}

// Check if state is in a given parent state (or is the parent itself)
int is_in_state(int current, int target) {
    if (current == target) return 1;
    int s = current;
    while (parent_of[s] != -1) {
        s = parent_of[s];
        if (s == target) return 1;
    }
    return 0;
}

// Get the depth of a state in the hierarchy
int state_depth(int state) {
    int depth = 0;
    int s = state;
    while (parent_of[s] != -1) {
        s = parent_of[s];
        depth++;
    }
    return depth;
}

// Find the lowest common ancestor of two states
int common_ancestor(int s1, int s2) {
    // Bring both to same depth
    int d1 = state_depth(s1);
    int d2 = state_depth(s2);

    int a = s1;
    int b = s2;

    while (d1 > d2) { a = parent_of[a]; d1--; }
    while (d2 > d1) { b = parent_of[b]; d2--; }

    while (a != b) {
        a = parent_of[a];
        b = parent_of[b];
    }
    return a; // Could be -1 if no common ancestor
}

// ============================================================
// Hierarchical State Machine with transitions
// ============================================================
int hsm_current_state;

void hsm_init(int initial_state) {
    // If initial state is composite, enter its default child
    int s = initial_state;
    while (is_composite[s]) {
        s = default_child[s];
    }
    hsm_current_state = s;
}

// Enter a state, diving into composite states
int hsm_enter_state(int state) {
    int s = state;
    while (is_composite[s] && default_child[s] != -1) {
        s = default_child[s];
    }
    hsm_current_state = s;
    return s;
}

// Process event in HSM: check current state and ancestors for matching transitions
int hsm_process_event(int event) {
    int check_state = hsm_current_state;

    while (check_state != -1) {
        // Search transition table for this state
        int i;
        for (i = 0; i < trans_count; i++) {
            if (trans_from[i] == check_state && trans_event[i] == event) {
                if (evaluate_guard(trans_guard[i])) {
                    if (trans_action[i] != -1) {
                        log_action(trans_action[i]);
                    }
                    // Enter new state (possibly composite)
                    hsm_enter_state(trans_to[i]);
                    return 1;
                }
            }
        }
        // Check parent state
        check_state = parent_of[check_state];
    }
    return 0; // No transition found
}

// ============================================================
// Event Queue for processing multiple events
// ============================================================
int event_queue[50];
int eq_front;
int eq_rear;
int eq_count;

void eq_init() {
    eq_front = 0;
    eq_rear = -1;
    eq_count = 0;
}

void eq_push(int event) {
    eq_rear++;
    event_queue[eq_rear] = event;
    eq_count++;
}

int eq_pop() {
    if (eq_count == 0) return -1;
    int ev = event_queue[eq_front];
    eq_front++;
    eq_count--;
    return ev;
}

int eq_is_empty() {
    return eq_count == 0;
}

// Process all events in queue using simple state machine
int process_all_events_sm() {
    int processed = 0;
    while (!eq_is_empty()) {
        int ev = eq_pop();
        sm_process_event(ev);
        processed++;
    }
    return processed;
}

// Process all events in queue using hierarchical state machine
int process_all_events_hsm() {
    int processed = 0;
    while (!eq_is_empty()) {
        int ev = eq_pop();
        hsm_process_event(ev);
        processed++;
    }
    return processed;
}

// ============================================================
// Test functions
// ============================================================

void test_simple_state_machine() {
    printf("=== Simple State Machine Tests ===\n");

    // Build a traffic light state machine
    // States: 0=OFF, 1=RED, 2=YELLOW, 3=GREEN
    // Events: 0=TIMER, 1=BUTTON_PRESS, 4=POWER_ON, 5=POWER_OFF
    trans_init();
    log_init();
    guards_init();

    // OFF -> RED on POWER_ON (action 100: "powered on")
    trans_add(0, 4, 1, -1, 100);
    // RED -> GREEN on TIMER (action 101: "go green")
    trans_add(1, 0, 3, -1, 101);
    // GREEN -> YELLOW on TIMER (action 102: "go yellow")
    trans_add(3, 0, 2, -1, 102);
    // YELLOW -> RED on TIMER (action 103: "go red")
    trans_add(2, 0, 1, -1, 103);
    // Any state -> OFF on POWER_OFF (action 104: "powered off")
    trans_add(1, 5, 0, -1, 104);
    trans_add(2, 5, 0, -1, 104);
    trans_add(3, 5, 0, -1, 104);

    sm_init(0); // Start OFF
    printf("SM initial: %d\n", sm_get_state());

    // Power on
    sm_process_event(4);
    printf("SM after power_on: %d\n", sm_get_state());

    // Timer -> GREEN
    sm_process_event(0);
    printf("SM after timer1: %d\n", sm_get_state());

    // Timer -> YELLOW
    sm_process_event(0);
    printf("SM after timer2: %d\n", sm_get_state());

    // Timer -> RED
    sm_process_event(0);
    printf("SM after timer3: %d\n", sm_get_state());

    // Timer -> GREEN
    sm_process_event(0);
    printf("SM after timer4: %d\n", sm_get_state());

    // Power off
    sm_process_event(5);
    printf("SM after power_off: %d\n", sm_get_state());

    printf("SM transitions: %d\n", sm_get_transition_count());
    printf("SM actions: %d\n", log_get_count());
    printf("SM action_log: ");
    log_print();
}

void test_guarded_transitions() {
    printf("=== Guarded Transition Tests ===\n");

    trans_init();
    log_init();
    guards_init();

    // States: 0=IDLE, 1=ACTIVE, 2=PAUSED, 3=DONE
    // Events: 0=START, 1=PAUSE, 2=RESUME, 3=FINISH, 4=TICK

    // IDLE -> ACTIVE on START if counter > 0 (guard 5, action 200)
    trans_add(0, 0, 1, 5, 200);
    // IDLE -> ACTIVE on START without guard (lower priority, action 201)
    trans_add(0, 0, 1, -1, 201);
    // ACTIVE -> PAUSED on PAUSE (action 202)
    trans_add(1, 1, 2, -1, 202);
    // PAUSED -> ACTIVE on RESUME (action 203)
    trans_add(2, 2, 1, -1, 203);
    // ACTIVE -> DONE on FINISH if counter > 3 (guard 4, action 204)
    trans_add(1, 3, 3, 4, 204);
    // ACTIVE -> ACTIVE on TICK (action 205)
    trans_add(1, 4, 1, -1, 205);

    sm_init(0); // Start IDLE

    // Try START with counter = 0 (guard 5 fails: counter > 0)
    // But second transition has no guard, so it matches
    guard_counter = 0;
    sm_process_event(0);
    printf("Guarded after start (counter=0): %d\n", sm_get_state());

    // Reset and try with counter = 1
    sm_init(0);
    log_init();
    guard_counter = 1;
    sm_process_event(0);
    printf("Guarded after start (counter=1): %d\n", sm_get_state());

    // Pause
    sm_process_event(1);
    printf("Guarded after pause: %d\n", sm_get_state());

    // Resume
    sm_process_event(2);
    printf("Guarded after resume: %d\n", sm_get_state());

    // Try finish with counter = 1 (guard 4 fails: counter > 3)
    int took = sm_process_event(3);
    printf("Guarded finish attempt (counter=1): took=%d state=%d\n", took, sm_get_state());

    // Set counter > 3 and finish
    guard_counter = 5;
    took = sm_process_event(3);
    printf("Guarded finish (counter=5): took=%d state=%d\n", took, sm_get_state());

    printf("Guarded actions: %d\n", log_get_count());
    printf("Guarded action_log: ");
    log_print();
}

void test_hierarchical_sm() {
    printf("=== Hierarchical State Machine Tests ===\n");

    // Build hierarchy:
    // 0: OFF (top-level)
    // 1: RUNNING (top-level, composite)
    //   10: RUNNING.IDLE (child of 1)
    //   11: RUNNING.PROCESSING (child of 1)
    //   12: RUNNING.WAITING (child of 1)
    // 2: ERROR (top-level)

    hierarchy_init();
    set_parent(10, 1);
    set_parent(11, 1);
    set_parent(12, 1);
    set_default_child(1, 10); // Default child of RUNNING is IDLE

    printf("HSM parent of 10: %d\n", parent_of[10]);
    printf("HSM parent of 11: %d\n", parent_of[11]);
    printf("HSM parent of 0: %d\n", parent_of[0]);
    printf("HSM is_composite 1: %d\n", is_composite[1]);
    printf("HSM is_composite 10: %d\n", is_composite[10]);
    printf("HSM depth 10: %d\n", state_depth(10));
    printf("HSM depth 1: %d\n", state_depth(1));
    printf("HSM depth 0: %d\n", state_depth(0));
    printf("HSM is_in 10/1: %d\n", is_in_state(10, 1));
    printf("HSM is_in 10/0: %d\n", is_in_state(10, 0));
    printf("HSM is_in 11/1: %d\n", is_in_state(11, 1));
    printf("HSM ancestor 10,11: %d\n", common_ancestor(10, 11));
    printf("HSM ancestor 10,12: %d\n", common_ancestor(10, 12));

    // Build transitions
    trans_init();
    log_init();
    guards_init();

    // OFF -> RUNNING on POWER_ON (enters default child = IDLE)
    trans_add(0, 4, 1, -1, 300);
    // RUNNING.IDLE -> RUNNING.PROCESSING on START
    trans_add(10, 0, 11, -1, 301);
    // RUNNING.PROCESSING -> RUNNING.WAITING on WAIT
    trans_add(11, 1, 12, -1, 302);
    // RUNNING.WAITING -> RUNNING.PROCESSING on RESUME
    trans_add(12, 2, 11, -1, 303);
    // RUNNING (any substate) -> ERROR on EMERGENCY (event 2)
    trans_add(1, 2, 2, -1, 304);
    // ERROR -> OFF on RESET (event 3)
    trans_add(2, 3, 0, -1, 305);

    // Initialize HSM
    hsm_init(0);
    printf("HSM initial: %d\n", hsm_current_state);

    // Power on -> should enter RUNNING -> RUNNING.IDLE
    hsm_process_event(4);
    printf("HSM after power_on: %d\n", hsm_current_state);

    // START -> RUNNING.PROCESSING
    hsm_process_event(0);
    printf("HSM after start: %d\n", hsm_current_state);

    // WAIT -> RUNNING.WAITING
    hsm_process_event(1);
    printf("HSM after wait: %d\n", hsm_current_state);

    // RESUME -> RUNNING.PROCESSING
    hsm_process_event(2);
    printf("HSM after resume: %d\n", hsm_current_state);

    // EMERGENCY while in RUNNING.PROCESSING
    // Should match RUNNING's transition (parent)
    hsm_process_event(2);
    printf("HSM after emergency: %d\n", hsm_current_state);

    // RESET -> OFF
    hsm_process_event(3);
    printf("HSM after reset: %d\n", hsm_current_state);

    printf("HSM action_log: ");
    log_print();
}

void test_event_queue() {
    printf("=== Event Queue Tests ===\n");

    // Setup traffic light SM again
    trans_init();
    log_init();
    guards_init();

    trans_add(0, 4, 1, -1, 100); // OFF -> RED on POWER_ON
    trans_add(1, 0, 3, -1, 101); // RED -> GREEN on TIMER
    trans_add(3, 0, 2, -1, 102); // GREEN -> YELLOW on TIMER
    trans_add(2, 0, 1, -1, 103); // YELLOW -> RED on TIMER

    sm_init(0);

    // Queue up events
    eq_init();
    eq_push(4); // POWER_ON
    eq_push(0); // TIMER
    eq_push(0); // TIMER
    eq_push(0); // TIMER
    eq_push(0); // TIMER
    eq_push(0); // TIMER
    eq_push(0); // TIMER

    int processed = process_all_events_sm();
    printf("EQ processed: %d\n", processed);
    printf("EQ final_state: %d\n", sm_get_state());
    printf("EQ transitions: %d\n", sm_get_transition_count());
    printf("EQ action_count: %d\n", log_get_count());
    printf("EQ action_log: ");
    log_print();
}

void test_complex_scenario() {
    printf("=== Complex Scenario Tests ===\n");

    // Elevator state machine
    // States: 0=IDLE, 1=MOVING_UP, 2=MOVING_DOWN, 3=DOOR_OPEN, 4=DOOR_CLOSING
    // Events: 0=CALL_UP, 1=CALL_DOWN, 2=ARRIVED, 3=DOOR_TIMER, 4=OBSTRUCTION

    trans_init();
    log_init();
    guards_init();

    // IDLE -> MOVING_UP on CALL_UP
    trans_add(0, 0, 1, -1, 400);
    // IDLE -> MOVING_DOWN on CALL_DOWN
    trans_add(0, 1, 2, -1, 401);
    // MOVING_UP -> DOOR_OPEN on ARRIVED
    trans_add(1, 2, 3, -1, 402);
    // MOVING_DOWN -> DOOR_OPEN on ARRIVED
    trans_add(2, 2, 3, -1, 403);
    // DOOR_OPEN -> DOOR_CLOSING on DOOR_TIMER
    trans_add(3, 3, 4, -1, 404);
    // DOOR_CLOSING -> IDLE on DOOR_TIMER (door closed)
    trans_add(4, 3, 0, -1, 405);
    // DOOR_CLOSING -> DOOR_OPEN on OBSTRUCTION
    trans_add(4, 4, 3, -1, 406);

    sm_init(0);

    // Scenario: Call up, arrive, door timer, door timer (close), call down, arrive
    eq_init();
    eq_push(0); // CALL_UP
    eq_push(2); // ARRIVED
    eq_push(3); // DOOR_TIMER (start closing)
    eq_push(4); // OBSTRUCTION (reopen)
    eq_push(3); // DOOR_TIMER (start closing again)
    eq_push(3); // DOOR_TIMER (fully closed)
    eq_push(1); // CALL_DOWN
    eq_push(2); // ARRIVED
    eq_push(3); // DOOR_TIMER
    eq_push(3); // DOOR_TIMER

    int processed = process_all_events_sm();
    printf("Elevator processed: %d\n", processed);
    printf("Elevator final: %d\n", sm_get_state());
    printf("Elevator transitions: %d\n", sm_get_transition_count());
    printf("Elevator actions: ");
    log_print();
}

void test_state_properties() {
    printf("=== State Property Tests ===\n");

    // Test various hierarchy properties
    hierarchy_init();

    // Build a 3-level hierarchy:
    // 0: ROOT
    //   1: CHILD_A
    //     10: GRANDCHILD_A1
    //     11: GRANDCHILD_A2
    //   2: CHILD_B
    //     20: GRANDCHILD_B1
    //     21: GRANDCHILD_B2

    set_parent(1, 0);
    set_parent(2, 0);
    set_parent(10, 1);
    set_parent(11, 1);
    set_parent(20, 2);
    set_parent(21, 2);

    printf("Prop depth 10: %d\n", state_depth(10));
    printf("Prop depth 1: %d\n", state_depth(1));
    printf("Prop depth 0: %d\n", state_depth(0));
    printf("Prop depth 21: %d\n", state_depth(21));

    printf("Prop ancestor 10,11: %d\n", common_ancestor(10, 11));
    printf("Prop ancestor 10,20: %d\n", common_ancestor(10, 20));
    printf("Prop ancestor 10,21: %d\n", common_ancestor(10, 21));
    printf("Prop ancestor 1,2: %d\n", common_ancestor(1, 2));

    printf("Prop is_in 10/1: %d\n", is_in_state(10, 1));
    printf("Prop is_in 10/0: %d\n", is_in_state(10, 0));
    printf("Prop is_in 10/2: %d\n", is_in_state(10, 2));
    printf("Prop is_in 21/2: %d\n", is_in_state(21, 2));
    printf("Prop is_in 21/0: %d\n", is_in_state(21, 0));
}


int main() {
    test_simple_state_machine();
    test_guarded_transitions();
    test_hierarchical_sm();
    test_event_queue();
    test_complex_scenario();
    test_state_properties();
    return 0;
}
