int printf(const char *fmt, ...);

enum EventType {
    EVT_NONE,
    EVT_CLICK,
    EVT_KEY,
    EVT_TIMER,
    EVT_RESIZE,
    EVT_CUSTOM
};

struct Event {
    int type;
    int data;
    int priority;
    int handled;
};

struct EventQueue {
    struct Event events[32];
    int head;
    int tail;
    int count;
};

void queue_init(struct EventQueue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

int queue_push(struct EventQueue *q, int type, int data, int priority) {
    if (q->count >= 32) return -1;
    q->events[q->tail].type = type;
    q->events[q->tail].data = data;
    q->events[q->tail].priority = priority;
    q->events[q->tail].handled = 0;
    q->tail = (q->tail + 1) % 32;
    q->count = q->count + 1;
    return 0;
}

int queue_pop(struct EventQueue *q, struct Event *out) {
    if (q->count <= 0) return -1;
    out->type = q->events[q->head].type;
    out->data = q->events[q->head].data;
    out->priority = q->events[q->head].priority;
    out->handled = q->events[q->head].handled;
    q->head = (q->head + 1) % 32;
    q->count = q->count - 1;
    return 0;
}

int queue_peek(struct EventQueue *q, struct Event *out) {
    if (q->count <= 0) return -1;
    out->type = q->events[q->head].type;
    out->data = q->events[q->head].data;
    out->priority = q->events[q->head].priority;
    out->handled = q->events[q->head].handled;
    return 0;
}

int queue_is_empty(struct EventQueue *q) {
    return q->count == 0;
}

/* Handler lookup table: type -> handler_id mapping */
int handler_ids[6];
int handler_call_counts[6];

void init_handlers(void) {
    int i;
    i = 0;
    while (i < 6) {
        handler_ids[i] = 0;
        handler_call_counts[i] = 0;
        i = i + 1;
    }
}

void register_handler(int event_type, int handler_id) {
    if (event_type >= 0 && event_type < 6) {
        handler_ids[event_type] = handler_id;
    }
}

int dispatch_event(struct Event *evt) {
    int type;
    type = evt->type;
    if (type < 0 || type >= 6) return -1;
    if (handler_ids[type] == 0) return -2;
    handler_call_counts[type] = handler_call_counts[type] + 1;
    evt->handled = 1;
    return handler_ids[type];
}

int process_all(struct EventQueue *q) {
    struct Event evt;
    int processed;
    processed = 0;
    while (queue_pop(q, &evt) == 0) {
        dispatch_event(&evt);
        processed = processed + 1;
    }
    return processed;
}

/* Find highest priority event in queue without removing */
int find_highest_priority(struct EventQueue *q) {
    int i;
    int idx;
    int best;
    int best_pri;
    if (q->count == 0) return -1;
    best = -1;
    best_pri = -1;
    i = 0;
    while (i < q->count) {
        idx = (q->head + i) % 32;
        if (q->events[idx].priority > best_pri) {
            best_pri = q->events[idx].priority;
            best = i;
        }
        i = i + 1;
    }
    return best_pri;
}

int count_events_of_type(struct EventQueue *q, int type) {
    int i;
    int idx;
    int c;
    c = 0;
    i = 0;
    while (i < q->count) {
        idx = (q->head + i) % 32;
        if (q->events[idx].type == type) c = c + 1;
        i = i + 1;
    }
    return c;
}

int total_handler_calls(void) {
    int i;
    int total;
    total = 0;
    i = 0;
    while (i < 6) {
        total = total + handler_call_counts[i];
        i = i + 1;
    }
    return total;
}

int main(void) {
    struct EventQueue q;
    struct Event evt;
    int result;

    queue_init(&q);
    init_handlers();

    // EXPECT: 1
    printf("%d\n", queue_is_empty(&q));

    // EXPECT: 0
    printf("%d\n", q.count);

    /* Register handlers */
    register_handler(EVT_CLICK, 10);
    register_handler(EVT_KEY, 20);
    register_handler(EVT_TIMER, 30);
    register_handler(EVT_RESIZE, 40);

    /* Push events */
    queue_push(&q, EVT_CLICK, 100, 1);
    queue_push(&q, EVT_KEY, 65, 2);
    queue_push(&q, EVT_TIMER, 0, 1);
    queue_push(&q, EVT_CLICK, 200, 3);
    queue_push(&q, EVT_RESIZE, 800, 1);

    // EXPECT: 5
    printf("%d\n", q.count);

    // EXPECT: 0
    printf("%d\n", queue_is_empty(&q));

    /* Count click events */
    // EXPECT: 2
    printf("%d\n", count_events_of_type(&q, EVT_CLICK));

    /* Highest priority */
    // EXPECT: 3
    printf("%d\n", find_highest_priority(&q));

    /* Peek */
    queue_peek(&q, &evt);
    // EXPECT: 1
    printf("%d\n", evt.type);
    // EXPECT: 100
    printf("%d\n", evt.data);

    /* Pop first event (CLICK, data=100) */
    queue_pop(&q, &evt);
    result = dispatch_event(&evt);
    // EXPECT: 10
    printf("%d\n", result);
    // EXPECT: 1
    printf("%d\n", evt.handled);

    // EXPECT: 4
    printf("%d\n", q.count);

    /* Pop second (KEY, data=65) */
    queue_pop(&q, &evt);
    result = dispatch_event(&evt);
    // EXPECT: 20
    printf("%d\n", result);

    /* Process remaining 3 events */
    result = process_all(&q);
    // EXPECT: 3
    printf("%d\n", result);

    // EXPECT: 1
    printf("%d\n", queue_is_empty(&q));

    /* Handler call counts: CLICK=2(from dispatch+process), KEY=1+1=2, TIMER=1, RESIZE=1 */
    /* Actually: manual dispatch click=1, key=1, then process: timer, click, resize */
    /* So: click handler called 1+1=2, key=1+0=1, timer=0+1=1, resize=0+1=1 */
    // EXPECT: 2
    printf("%d\n", handler_call_counts[EVT_CLICK]);
    // EXPECT: 1
    printf("%d\n", handler_call_counts[EVT_KEY]);
    // EXPECT: 1
    printf("%d\n", handler_call_counts[EVT_TIMER]);

    /* Total handler calls: 2+1+1+1 = 5 */
    // EXPECT: 5
    printf("%d\n", total_handler_calls());

    /* Dispatch event with no handler */
    evt.type = EVT_CUSTOM;
    evt.data = 999;
    result = dispatch_event(&evt);
    // EXPECT: -2
    printf("%d\n", result);

    /* Pop from empty queue */
    result = queue_pop(&q, &evt);
    // EXPECT: -1
    printf("%d\n", result);

    return 0;
}
