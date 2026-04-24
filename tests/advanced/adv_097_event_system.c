// EXPECT: === Event System Tests ===
// EXPECT: Registered handler for event 0
// EXPECT: Registered handler for event 0
// EXPECT: Registered handler for event 1
// EXPECT: Registered handler for event 2
// EXPECT: === Fire Event: CLICK ===
// EXPECT: [Click Handler A] x=100, y=200
// EXPECT: [Click Handler B] x=100, y=200
// EXPECT: === Fire Event: KEYPRESS ===
// EXPECT: [Key Handler] key=65
// EXPECT: === Fire Event: TIMER ===
// EXPECT: [Timer Handler] ms=5000
// EXPECT: === Multiple Fires ===
// EXPECT: [Click Handler A] x=10, y=20
// EXPECT: [Click Handler B] x=10, y=20
// EXPECT: [Click Handler A] x=30, y=40
// EXPECT: [Click Handler B] x=30, y=40
// EXPECT: === Event Counting ===
// EXPECT: CLICK fired 3 times
// EXPECT: KEYPRESS fired 1 times
// EXPECT: TIMER fired 1 times
// EXPECT: Total handlers invoked: 8
// EXPECT: === Chain Test ===
// EXPECT: Registered handler for event 3
// EXPECT: Registered handler for event 3
// EXPECT: Registered handler for event 3
// EXPECT: Registered handler for event 3
// EXPECT: [Chain 1] val=42
// EXPECT: [Chain 2] val=42
// EXPECT: [Chain 3] val=42
// EXPECT: [Chain 4] val=42
// EXPECT: All event system tests passed!

int printf(const char *fmt, ...);

// Event types
enum EventType {
    EVT_CLICK,
    EVT_KEYPRESS,
    EVT_TIMER,
    EVT_CUSTOM,
    EVT_COUNT
};

// Event data structures
struct ClickData {
    int x;
    int y;
};

struct KeyData {
    int key_code;
};

struct TimerData {
    int milliseconds;
};

struct IntData {
    int value;
};

// Handler function pointer type: takes event type and void* data
typedef void (*EventHandler)(int event_type, void *data);

// Event system: up to 4 handlers per event type
struct EventSystem {
    EventHandler handlers[4][8];  // [event_type][handler_index]
    int handler_count[4];
    int fire_count[4];
    int total_invocations;
};

void event_init(struct EventSystem *es) {
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        es->handler_count[i] = 0;
        es->fire_count[i] = 0;
        for (j = 0; j < 8; j++) {
            es->handlers[i][j] = 0;
        }
    }
    es->total_invocations = 0;
}

int event_register(struct EventSystem *es, int event_type, EventHandler handler) {
    if (event_type < 0 || event_type >= 4) return -1;
    int idx = es->handler_count[event_type];
    if (idx >= 8) return -1;
    es->handlers[event_type][idx] = handler;
    es->handler_count[event_type] = idx + 1;
    printf("Registered handler for event %d\n", event_type);
    return 0;
}

void event_fire(struct EventSystem *es, int event_type, void *data) {
    int i;
    if (event_type < 0 || event_type >= 4) return;
    es->fire_count[event_type] = es->fire_count[event_type] + 1;
    int count = es->handler_count[event_type];
    for (i = 0; i < count; i++) {
        es->handlers[event_type][i](event_type, data);
        es->total_invocations = es->total_invocations + 1;
    }
}

// Handler implementations
void click_handler_a(int evt, void *data) {
    struct ClickData *cd = (struct ClickData *)data;
    printf("[Click Handler A] x=%d, y=%d\n", cd->x, cd->y);
}

void click_handler_b(int evt, void *data) {
    struct ClickData *cd = (struct ClickData *)data;
    printf("[Click Handler B] x=%d, y=%d\n", cd->x, cd->y);
}

void key_handler(int evt, void *data) {
    struct KeyData *kd = (struct KeyData *)data;
    printf("[Key Handler] key=%d\n", kd->key_code);
}

void timer_handler(int evt, void *data) {
    struct TimerData *td = (struct TimerData *)data;
    printf("[Timer Handler] ms=%d\n", td->milliseconds);
}

void chain_handler_1(int evt, void *data) {
    struct IntData *id = (struct IntData *)data;
    printf("[Chain 1] val=%d\n", id->value);
}

void chain_handler_2(int evt, void *data) {
    struct IntData *id = (struct IntData *)data;
    printf("[Chain 2] val=%d\n", id->value);
}

void chain_handler_3(int evt, void *data) {
    struct IntData *id = (struct IntData *)data;
    printf("[Chain 3] val=%d\n", id->value);
}

void chain_handler_4(int evt, void *data) {
    struct IntData *id = (struct IntData *)data;
    printf("[Chain 4] val=%d\n", id->value);
}

int main(void) {
    struct EventSystem es;
    struct ClickData click;
    struct KeyData key;
    struct TimerData timer;
    struct IntData custom;

    printf("=== Event System Tests ===\n");
    event_init(&es);

    // Register handlers
    event_register(&es, EVT_CLICK, click_handler_a);
    event_register(&es, EVT_CLICK, click_handler_b);
    event_register(&es, EVT_KEYPRESS, key_handler);
    event_register(&es, EVT_TIMER, timer_handler);

    // Fire click event
    printf("=== Fire Event: CLICK ===\n");
    click.x = 100;
    click.y = 200;
    event_fire(&es, EVT_CLICK, &click);

    // Fire keypress event
    printf("=== Fire Event: KEYPRESS ===\n");
    key.key_code = 65;
    event_fire(&es, EVT_KEYPRESS, &key);

    // Fire timer event
    printf("=== Fire Event: TIMER ===\n");
    timer.milliseconds = 5000;
    event_fire(&es, EVT_TIMER, &timer);

    // Multiple fires
    printf("=== Multiple Fires ===\n");
    click.x = 10;
    click.y = 20;
    event_fire(&es, EVT_CLICK, &click);
    click.x = 30;
    click.y = 40;
    event_fire(&es, EVT_CLICK, &click);

    // Event counting
    printf("=== Event Counting ===\n");
    printf("CLICK fired %d times\n", es.fire_count[EVT_CLICK]);
    printf("KEYPRESS fired %d times\n", es.fire_count[EVT_KEYPRESS]);
    printf("TIMER fired %d times\n", es.fire_count[EVT_TIMER]);
    printf("Total handlers invoked: %d\n", es.total_invocations);

    // Chain test: 4 handlers on one event
    printf("=== Chain Test ===\n");
    event_init(&es);
    event_register(&es, EVT_CUSTOM, chain_handler_1);
    event_register(&es, EVT_CUSTOM, chain_handler_2);
    event_register(&es, EVT_CUSTOM, chain_handler_3);
    event_register(&es, EVT_CUSTOM, chain_handler_4);
    custom.value = 42;
    event_fire(&es, EVT_CUSTOM, &custom);

    printf("All event system tests passed!\n");
    return 0;
}
