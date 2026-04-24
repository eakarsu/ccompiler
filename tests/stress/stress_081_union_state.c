// stress_081_union_state.c - Tagged union state machine with 5 struct types
// EXPECT: init_tag=0
// EXPECT: idle_val=0
// EXPECT: trans_to_loading=1
// EXPECT: loading_progress=50
// EXPECT: trans_to_processing=2
// EXPECT: proc_items=10 total=250
// EXPECT: trans_to_error=3
// EXPECT: err_code=404 msg_first=N
// EXPECT: trans_to_done=4
// EXPECT: done_result=9999 success=1
// EXPECT: chain_final_tag=4
// EXPECT: chain_final_result=42
// EXPECT: array_tags=0 1 2 3 4
// EXPECT: array_process_0=idle:0
// EXPECT: array_process_1=loading:75
// EXPECT: array_process_2=processing:5x20
// EXPECT: array_process_3=error:500
// EXPECT: array_process_4=done:1234
// EXPECT: machine_run=done:7777
// EXPECT: multi_chain_0=4
// EXPECT: multi_chain_1=4
// EXPECT: multi_chain_2=4
// EXPECT: sizeof_state_positive=1
// EXPECT: pass_done=1

int printf(const char *fmt, ...);

enum StateTag {
    TAG_IDLE = 0,
    TAG_LOADING = 1,
    TAG_PROCESSING = 2,
    TAG_ERROR = 3,
    TAG_DONE = 4
};

struct IdleState {
    int placeholder;
};

struct LoadingState {
    int progress;
    int bytes_loaded;
};

struct ProcessingState {
    int items_processed;
    int total_value;
    int batch_size;
};

struct ErrorState {
    int error_code;
    char message[32];
};

struct DoneState {
    int result;
    int success;
};

union StateData {
    struct IdleState idle;
    struct LoadingState loading;
    struct ProcessingState processing;
    struct ErrorState error;
    struct DoneState done;
};

struct State {
    enum StateTag tag;
    union StateData data;
};

struct State make_idle(void) {
    struct State s;
    s.tag = TAG_IDLE;
    s.data.idle.placeholder = 0;
    return s;
}

struct State make_loading(int progress, int bytes) {
    struct State s;
    s.tag = TAG_LOADING;
    s.data.loading.progress = progress;
    s.data.loading.bytes_loaded = bytes;
    return s;
}

struct State make_processing(int items, int total, int batch) {
    struct State s;
    s.tag = TAG_PROCESSING;
    s.data.processing.items_processed = items;
    s.data.processing.total_value = total;
    s.data.processing.batch_size = batch;
    return s;
}

struct State make_error(int code, char first_char) {
    struct State s;
    s.tag = TAG_ERROR;
    s.data.error.error_code = code;
    s.data.error.message[0] = first_char;
    s.data.error.message[1] = 0;
    return s;
}

struct State make_done(int result, int success) {
    struct State s;
    s.tag = TAG_DONE;
    s.data.done.result = result;
    s.data.done.success = success;
    return s;
}

// Process state and return description via printf
void process_state(struct State *s) {
    switch (s->tag) {
        case TAG_IDLE:
            printf("idle:%d\n", s->data.idle.placeholder);
            break;
        case TAG_LOADING:
            printf("loading:%d\n", s->data.loading.progress);
            break;
        case TAG_PROCESSING:
            printf("processing:%dx%d\n",
                   s->data.processing.items_processed,
                   s->data.processing.total_value / s->data.processing.items_processed);
            break;
        case TAG_ERROR:
            printf("error:%d\n", s->data.error.error_code);
            break;
        case TAG_DONE:
            printf("done:%d\n", s->data.done.result);
            break;
    }
}

// Transition function: advance state machine
struct State transition(struct State current) {
    switch (current.tag) {
        case TAG_IDLE:
            return make_loading(0, 0);
        case TAG_LOADING:
            if (current.data.loading.progress >= 100)
                return make_processing(0, 0, 10);
            else {
                return make_loading(current.data.loading.progress + 25, current.data.loading.bytes_loaded + 1024);
            }
        case TAG_PROCESSING:
            if (current.data.processing.items_processed >= 10)
                return make_done(current.data.processing.total_value, 1);
            else {
                return make_processing(
                    current.data.processing.items_processed + 1,
                    current.data.processing.total_value + 42,
                    current.data.processing.batch_size
                );
            }
        case TAG_ERROR:
            return make_done(-1, 0);
        case TAG_DONE:
            return current;
    }
    return current;
}

// Run state machine to completion
struct State run_machine(struct State s) {
    int steps = 0;
    while (s.tag != TAG_DONE && steps < 100) {
        s = transition(s);
        steps++;
    }
    return s;
}

int main(void) {
    // Basic state creation and inspection
    struct State s = make_idle();
    printf("init_tag=%d\n", s.tag);
    printf("idle_val=%d\n", s.data.idle.placeholder);

    // Transition chain: idle -> loading
    s = transition(s);
    printf("trans_to_loading=%d\n", s.tag);
    // loading with 50%
    s = make_loading(50, 2048);
    printf("loading_progress=%d\n", s.data.loading.progress);

    // loading -> processing (set progress to 100 to trigger transition)
    s = make_loading(100, 4096);
    s = transition(s);
    printf("trans_to_processing=%d\n", s.tag);

    // processing with items
    s = make_processing(10, 250, 5);
    printf("proc_items=%d total=%d\n", s.data.processing.items_processed, s.data.processing.total_value);

    // error state
    s = make_error(404, 'N');
    printf("trans_to_error=%d\n", s.tag);
    printf("err_code=%d msg_first=%c\n", s.data.error.error_code, s.data.error.message[0]);

    // error -> done
    s = transition(s);
    printf("trans_to_done=%d\n", s.tag);

    // done state with values
    s = make_done(9999, 1);
    printf("done_result=%d success=%d\n", s.data.done.result, s.data.done.success);

    // Full chain from idle to done
    s = make_idle();
    s = run_machine(s);
    printf("chain_final_tag=%d\n", s.tag);
    // idle->loading(0)->loading(25)->loading(50)->loading(75)->loading(100)->processing(0)->...->processing(10)->done(420)
    // actually: processing goes 0,1,2,...,10 with total 0,42,84,...,420
    // But transition checks >= 10, so at items=10 we get done with total
    // items: 0+1=1, 1+1=2, ..., 9+1=10 (10 transitions in processing)
    // total: 0+42=42, 42+42=84, ..., 42*1=42 (first step)
    // Wait: starts at 0, then +1 and +42 each step.
    // After 1 step: items=1, total=42
    // After 10 steps: items=10, total=420
    // Then items >= 10 -> done(420)
    // But the transition at items=10 goes to done(total_value=420, success=1)
    // Hmm let me retrace: make_processing(0,0,10) then transition:
    // items=0 < 10 -> make_processing(1, 42, 10)
    // items=1 < 10 -> make_processing(2, 84, 10)
    // ...
    // items=9 < 10 -> make_processing(10, 420, 10)
    // items=10 >= 10 -> make_done(420, 1)
    // But loading: starts at progress=0. 0<100 -> 25. 25<100 -> 50. 50<100 -> 75. 75<100 -> 100. 100>=100 -> processing
    // So: idle -> loading(0) -> loading(25) -> loading(50) -> loading(75) -> loading(100) -> processing(0,0) -> ... -> processing(10, 420) -> done(420)
    // Hmm, but I printed chain_final_result=42 above. Let me fix.
    // Actually for the "chain" test, let me just use a custom chain.
    // Let me use a simpler chain that gives result=42
    s = make_processing(9, 0, 10);
    s = transition(s); // items=10, total=42
    s = transition(s); // done(42)
    printf("chain_final_result=%d\n", s.data.done.result);

    // Array of states
    struct State arr[5];
    arr[0] = make_idle();
    arr[1] = make_loading(75, 3000);
    arr[2] = make_processing(5, 100, 20);
    arr[3] = make_error(500, 'E');
    arr[4] = make_done(1234, 1);

    printf("array_tags=%d %d %d %d %d\n", arr[0].tag, arr[1].tag, arr[2].tag, arr[3].tag, arr[4].tag);

    // Process each state in array
    int i;
    for (i = 0; i < 5; i++) {
        printf("array_process_%d=", i);
        process_state(&arr[i]);
    }

    // Full machine run from idle
    s = make_idle();
    s = run_machine(s);
    // idle->loading(0)->loading(25)->loading(50)->loading(75)->loading(100)->processing(0,0)->...->processing(10,420)->done(420)
    // Hmm that gives 420. Let me set up a specific test.
    // Actually, let me make a custom start:
    // Start from processing(9, 7735, 10): transition -> processing(10, 7777, 10) -> done(7777, 1)
    s = make_processing(9, 7735, 10);
    s = run_machine(s);
    printf("machine_run=done:%d\n", s.data.done.result);

    // Multiple independent chains
    struct State chains[3];
    chains[0] = make_error(100, 'A');
    chains[1] = make_error(200, 'B');
    chains[2] = make_error(300, 'C');
    for (i = 0; i < 3; i++) {
        chains[i] = run_machine(chains[i]);
        printf("multi_chain_%d=%d\n", i, chains[i].tag);
    }

    // Sizeof check
    int sz = (int)(sizeof(struct State));
    printf("sizeof_state_positive=%d\n", sz > 0 ? 1 : 0);

    printf("pass_done=1\n");
    return 0;
}
