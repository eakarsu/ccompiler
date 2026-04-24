int printf(const char *fmt, ...);
// EXPECT: === Complex State Machine Test ===\n--- Sequence 1: Normal lifecycle (8 events) ---\n  [0] IDLE + CONNECT -> CONNECTING\n  [1] CONNECTING + CONNECTED -> CONNECTED\n  [2] CONNECTED + SEND -> SENDING\n  [3] SENDING + RECV -> RECEIVING\n  [4] RECEIVING + SEND -> SENDING\n  [5] SENDING + RECV -> RECEIVING\n  [6] RECEIVING + CLOSE -> CLOSING\n  [7] CLOSING + CLOSE -> DONE\n  Result: 8 transitions, final state = DONE\n  History (9): 0 1 2 3 4 3 4 5 7\n--- Sequence 2: Error + recovery (8 events) ---\n  [0] IDLE + CONNECT -> CONNECTING\n  [1] CONNECTING + CONNECTED -> CONNECTED\n  [2] CONNECTED + SEND -> SENDING\n  [3] SENDING + ERROR -> ERROR\n  [4] ERROR + RESET -> IDLE\n  [5] IDLE + CONNECT -> CONNECTING\n  [6] CONNECTING + CONNECTED -> CONNECTED\n  [7] CONNECTED + CLOSE -> CLOSING\n  Result: 8 transitions, final state = CLOSING\n  History (9): 0 1 2 3 6 0 1 2 5\n--- Sequence 3: Timeout + recovery (6 events) ---\n  [0] IDLE + CONNECT -> CONNECTING\n  [1] CONNECTING + TIMEOUT -> ERROR\n  [2] ERROR + RESET -> IDLE\n  [3] IDLE + CONNECT -> CONNECTING\n  [4] CONNECTING + CONNECTED -> CONNECTED\n  [5] CONNECTED + CLOSE -> CLOSING\n  Result: 6 transitions, final state = CLOSING\n  History (7): 0 1 6 0 1 2 5\n--- Aggregate Statistics ---\n  Total transitions across all sequences: 22\n  State visit counts:\n    IDLE: 5\n    CONNECTING: 5\n    CONNECTED: 4\n    SENDING: 3\n    RECEIVING: 2\n    CLOSING: 3\n    ERROR: 2\n    DONE: 1\n  Total visits: 25\n--- Transition matrix (non-zero entries) ---\n  IDLE -> CONNECTING: 5\n  CONNECTING -> CONNECTED: 4\n  CONNECTING -> ERROR: 1\n  CONNECTED -> SENDING: 2\n  CONNECTED -> CLOSING: 2\n  SENDING -> RECEIVING: 2\n  SENDING -> ERROR: 1\n  RECEIVING -> SENDING: 1\n  RECEIVING -> CLOSING: 1\n  CLOSING -> DONE: 1\n  ERROR -> IDLE: 2\n=== Test Complete ===

// ================================================================
// Complex state machine with function pointer dispatch and history
// Tests: enums, function pointers, arrays, dispatch tables, structs
// ================================================================

enum state {
    STATE_IDLE,
    STATE_CONNECTING,
    STATE_CONNECTED,
    STATE_SENDING,
    STATE_RECEIVING,
    STATE_CLOSING,
    STATE_ERROR,
    STATE_DONE
};

enum event {
    EVT_CONNECT,
    EVT_CONNECTED,
    EVT_SEND,
    EVT_RECV,
    EVT_CLOSE,
    EVT_ERROR,
    EVT_RESET,
    EVT_TIMEOUT
};

// State name lookup
const char *state_names[8];
const char *event_names[8];

void init_names(void) {
    state_names[0] = "IDLE";
    state_names[1] = "CONNECTING";
    state_names[2] = "CONNECTED";
    state_names[3] = "SENDING";
    state_names[4] = "RECEIVING";
    state_names[5] = "CLOSING";
    state_names[6] = "ERROR";
    state_names[7] = "DONE";

    event_names[0] = "CONNECT";
    event_names[1] = "CONNECTED";
    event_names[2] = "SEND";
    event_names[3] = "RECV";
    event_names[4] = "CLOSE";
    event_names[5] = "ERROR";
    event_names[6] = "RESET";
    event_names[7] = "TIMEOUT";
}

// --- State handler functions ---
// Each takes an event and returns the next state

int handle_idle(int event) {
    if (event == EVT_CONNECT) return STATE_CONNECTING;
    return STATE_IDLE;
}

int handle_connecting(int event) {
    if (event == EVT_CONNECTED) return STATE_CONNECTED;
    if (event == EVT_TIMEOUT)   return STATE_ERROR;
    if (event == EVT_ERROR)     return STATE_ERROR;
    return STATE_CONNECTING;
}

int handle_connected(int event) {
    if (event == EVT_SEND)  return STATE_SENDING;
    if (event == EVT_RECV)  return STATE_RECEIVING;
    if (event == EVT_CLOSE) return STATE_CLOSING;
    if (event == EVT_ERROR) return STATE_ERROR;
    return STATE_CONNECTED;
}

int handle_sending(int event) {
    if (event == EVT_RECV)    return STATE_RECEIVING;
    if (event == EVT_CLOSE)   return STATE_CLOSING;
    if (event == EVT_TIMEOUT) return STATE_ERROR;
    if (event == EVT_ERROR)   return STATE_ERROR;
    return STATE_SENDING;
}

int handle_receiving(int event) {
    if (event == EVT_SEND)    return STATE_SENDING;
    if (event == EVT_CLOSE)   return STATE_CLOSING;
    if (event == EVT_TIMEOUT) return STATE_ERROR;
    if (event == EVT_ERROR)   return STATE_ERROR;
    return STATE_RECEIVING;
}

int handle_closing(int event) {
    if (event == EVT_CLOSE)   return STATE_DONE;
    if (event == EVT_TIMEOUT) return STATE_ERROR;
    if (event == EVT_ERROR)   return STATE_ERROR;
    return STATE_CLOSING;
}

int handle_error(int event) {
    if (event == EVT_RESET) return STATE_IDLE;
    return STATE_ERROR;
}

int handle_done(int event) {
    if (event == EVT_RESET) return STATE_IDLE;
    return STATE_DONE;
}

// --- State machine runner ---
// Returns number of transitions processed

int run_machine(int (*handlers[8])(int), int *events, int num_events,
                int *history, int *hist_len) {
    int state = STATE_IDLE;
    int transitions = 0;
    int i;

    // Record initial state
    history[0] = state;
    *hist_len = 1;

    for (i = 0; i < num_events; i++) {
        int evt = events[i];
        int next = handlers[state](evt);
        printf("  [%d] %s + %s -> %s\n", i,
               state_names[state], event_names[evt], state_names[next]);
        state = next;
        history[*hist_len] = state;
        *hist_len = *hist_len + 1;
        transitions++;
    }

    printf("  Result: %d transitions, final state = %s\n",
           transitions, state_names[state]);

    // Print history
    printf("  History (%d):", *hist_len);
    for (i = 0; i < *hist_len; i++) {
        printf(" %d", history[i]);
    }
    printf("\n");

    return transitions;
}

int main(void) {
    init_names();

    // Set up dispatch table
    int (*handlers[8])(int);
    handlers[STATE_IDLE]       = handle_idle;
    handlers[STATE_CONNECTING] = handle_connecting;
    handlers[STATE_CONNECTED]  = handle_connected;
    handlers[STATE_SENDING]    = handle_sending;
    handlers[STATE_RECEIVING]  = handle_receiving;
    handlers[STATE_CLOSING]    = handle_closing;
    handlers[STATE_ERROR]      = handle_error;
    handlers[STATE_DONE]       = handle_done;

    // History buffers
    int history1[64];
    int hist_len1 = 0;
    int history2[64];
    int hist_len2 = 0;
    int history3[64];
    int hist_len3 = 0;

    int total_transitions = 0;

    printf("=== Complex State Machine Test ===\n");

    // --- Sequence 1: Normal lifecycle ---
    {
        int events[8];
        events[0] = EVT_CONNECT;
        events[1] = EVT_CONNECTED;
        events[2] = EVT_SEND;
        events[3] = EVT_RECV;
        events[4] = EVT_SEND;
        events[5] = EVT_RECV;
        events[6] = EVT_CLOSE;
        events[7] = EVT_CLOSE;

        printf("--- Sequence 1: Normal lifecycle (8 events) ---\n");
        total_transitions = total_transitions +
            run_machine(handlers, events, 8, history1, &hist_len1);
    }

    // --- Sequence 2: Error + recovery ---
    {
        int events[8];
        events[0] = EVT_CONNECT;
        events[1] = EVT_CONNECTED;
        events[2] = EVT_SEND;
        events[3] = EVT_ERROR;
        events[4] = EVT_RESET;
        events[5] = EVT_CONNECT;
        events[6] = EVT_CONNECTED;
        events[7] = EVT_CLOSE;

        printf("--- Sequence 2: Error + recovery (8 events) ---\n");
        total_transitions = total_transitions +
            run_machine(handlers, events, 8, history2, &hist_len2);
    }

    // --- Sequence 3: Timeout + recovery ---
    {
        int events[6];
        events[0] = EVT_CONNECT;
        events[1] = EVT_TIMEOUT;
        events[2] = EVT_RESET;
        events[3] = EVT_CONNECT;
        events[4] = EVT_CONNECTED;
        events[5] = EVT_CLOSE;

        printf("--- Sequence 3: Timeout + recovery (6 events) ---\n");
        total_transitions = total_transitions +
            run_machine(handlers, events, 6, history3, &hist_len3);
    }

    // --- Aggregate statistics ---
    printf("--- Aggregate Statistics ---\n");
    printf("  Total transitions across all sequences: %d\n", total_transitions);

    // Count state visits across all histories
    int visit_counts[8];
    int s;
    int i;
    for (s = 0; s < 8; s++) visit_counts[s] = 0;

    for (i = 0; i < hist_len1; i++) visit_counts[history1[i]]++;
    for (i = 0; i < hist_len2; i++) visit_counts[history2[i]]++;
    for (i = 0; i < hist_len3; i++) visit_counts[history3[i]]++;

    printf("  State visit counts:\n");
    int total_visits = 0;
    for (s = 0; s < 8; s++) {
        printf("    %s: %d\n", state_names[s], visit_counts[s]);
        total_visits = total_visits + visit_counts[s];
    }
    printf("  Total visits: %d\n", total_visits);

    // --- Build transition matrix ---
    // trans[from][to] counts
    int trans[8][8];
    int from;
    int to;
    for (from = 0; from < 8; from++)
        for (to = 0; to < 8; to++)
            trans[from][to] = 0;

    // Count transitions from each history
    for (i = 0; i < hist_len1 - 1; i++)
        trans[history1[i]][history1[i + 1]]++;
    for (i = 0; i < hist_len2 - 1; i++)
        trans[history2[i]][history2[i + 1]]++;
    for (i = 0; i < hist_len3 - 1; i++)
        trans[history3[i]][history3[i + 1]]++;

    printf("--- Transition matrix (non-zero entries) ---\n");
    for (from = 0; from < 8; from++) {
        for (to = 0; to < 8; to++) {
            if (trans[from][to] > 0) {
                printf("  %s -> %s: %d\n",
                       state_names[from], state_names[to], trans[from][to]);
            }
        }
    }

    printf("=== Test Complete ===\n");
    return 0;
}
