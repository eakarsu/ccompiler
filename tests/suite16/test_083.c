int printf(const char *fmt, ...);

// Test 083: Event system with callback table and event queue

struct Event {
    int type;
    int data1;
    int data2;
};

struct EventQueue {
    struct Event events[10];
    int head;
    int tail;
    int count;
};

void eq_init(struct EventQueue *eq) {
    int i;
    for (i = 0; i < 10; i++) {
        eq->events[i].type = 0;
        eq->events[i].data1 = 0;
        eq->events[i].data2 = 0;
    }
    eq->head = 0;
    eq->tail = 0;
    eq->count = 0;
}

int eq_push(struct EventQueue *eq, int type, int d1, int d2) {
    if (eq->count >= 10) {
        return 0;
    }
    eq->events[eq->tail].type = type;
    eq->events[eq->tail].data1 = d1;
    eq->events[eq->tail].data2 = d2;
    eq->tail = (eq->tail + 1) % 10;
    eq->count = eq->count + 1;
    return 1;
}

int eq_pop(struct EventQueue *eq, struct Event *out) {
    if (eq->count <= 0) {
        return 0;
    }
    out->type = eq->events[eq->head].type;
    out->data1 = eq->events[eq->head].data1;
    out->data2 = eq->events[eq->head].data2;
    eq->head = (eq->head + 1) % 10;
    eq->count = eq->count - 1;
    return 1;
}

int handle_event(struct Event *ev) {
    int result;
    result = 0;
    if (ev->type == 1) {
        result = ev->data1 + ev->data2;
    } else if (ev->type == 2) {
        result = ev->data1 * ev->data2;
    } else if (ev->type == 3) {
        result = ev->data1 - ev->data2;
    } else {
        result = -1;
    }
    return result;
}

int main() {
    struct EventQueue eq;
    struct Event ev;
    int total;
    int r;
    int i;

    eq_init(&eq);
    printf("Queue empty: %d\n", eq.count); // EXPECT: Queue empty: 0

    // Push various event types
    eq_push(&eq, 1, 10, 20);
    eq_push(&eq, 2, 5, 6);
    eq_push(&eq, 3, 100, 40);
    eq_push(&eq, 1, 7, 8);
    eq_push(&eq, 2, 3, 9);
    printf("Count: %d\n", eq.count); // EXPECT: Count: 5

    // Process all events
    total = 0;
    eq_pop(&eq, &ev);
    r = handle_event(&ev);
    printf("Event type=%d result=%d\n", ev.type, r); // EXPECT: Event type=1 result=30
    total = total + r;
    eq_pop(&eq, &ev);
    r = handle_event(&ev);
    printf("Event type=%d result=%d\n", ev.type, r); // EXPECT: Event type=2 result=30
    total = total + r;
    eq_pop(&eq, &ev);
    r = handle_event(&ev);
    printf("Event type=%d result=%d\n", ev.type, r); // EXPECT: Event type=3 result=60
    total = total + r;
    eq_pop(&eq, &ev);
    r = handle_event(&ev);
    printf("Event type=%d result=%d\n", ev.type, r); // EXPECT: Event type=1 result=15
    total = total + r;
    eq_pop(&eq, &ev);
    r = handle_event(&ev);
    printf("Event type=%d result=%d\n", ev.type, r); // EXPECT: Event type=2 result=27
    total = total + r;
    printf("Total: %d\n", total); // EXPECT: Total: 162

    // Fill to capacity
    for (i = 0; i < 10; i++) {
        eq_push(&eq, (i % 3) + 1, i, i + 1);
    }
    printf("Full count: %d\n", eq.count); // EXPECT: Full count: 10

    // Overflow check
    i = eq_push(&eq, 1, 0, 0);
    printf("Overflow: %d\n", i); // EXPECT: Overflow: 0

    // Process half
    eq_pop(&eq, &ev);
    printf("Processed: type=%d d1=%d d2=%d\n", ev.type, ev.data1, ev.data2); // EXPECT: Processed: type=1 d1=0 d2=1
    eq_pop(&eq, &ev);
    printf("Processed: type=%d d1=%d d2=%d\n", ev.type, ev.data1, ev.data2); // EXPECT: Processed: type=2 d1=1 d2=2
    eq_pop(&eq, &ev);
    printf("Processed: type=%d d1=%d d2=%d\n", ev.type, ev.data1, ev.data2); // EXPECT: Processed: type=3 d1=2 d2=3
    eq_pop(&eq, &ev);
    printf("Processed: type=%d d1=%d d2=%d\n", ev.type, ev.data1, ev.data2); // EXPECT: Processed: type=1 d1=3 d2=4
    eq_pop(&eq, &ev);
    printf("Processed: type=%d d1=%d d2=%d\n", ev.type, ev.data1, ev.data2); // EXPECT: Processed: type=2 d1=4 d2=5
    printf("Remaining: %d\n", eq.count); // EXPECT: Remaining: 5

    // Add more after partial drain
    eq_push(&eq, 3, 50, 25);
    eq_push(&eq, 3, 80, 30);
    printf("After add: %d\n", eq.count); // EXPECT: After add: 7

    // Process rest and sum
    total = 0;
    while (eq.count > 0) {
        eq_pop(&eq, &ev);
        r = handle_event(&ev);
        total = total + r;
    }
    printf("Final total: %d\n", total); // EXPECT: Final total: 161

    printf("Queue empty: %d\n", eq.count); // EXPECT: Queue empty: 0

    printf("PASS\n"); // EXPECT: PASS
    return 0;
}
