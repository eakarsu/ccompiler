int printf(const char *fmt, ...);

struct Event {
    int type;
    int priority;
    int data;
    int time;
};

struct EventQueue {
    struct Event events[32];
    int count;
};

void eq_init(struct EventQueue *eq) {
    eq->count = 0;
}

void eq_push(struct EventQueue *eq, int type, int pri,
             int data, int time) {
    if (eq->count >= 32) return;
    int idx = eq->count;
    eq->events[idx].type = type;
    eq->events[idx].priority = pri;
    eq->events[idx].data = data;
    eq->events[idx].time = time;
    eq->count = eq->count + 1;
}

int eq_pop_highest(struct EventQueue *eq, struct Event *out) {
    if (eq->count == 0) return 0;
    int best = 0;
    int i = 1;
    while (i < eq->count) {
        if (eq->events[i].priority > eq->events[best].priority) {
            best = i;
        } else if (eq->events[i].priority == eq->events[best].priority) {
            if (eq->events[i].time < eq->events[best].time) {
                best = i;
            }
        }
        i = i + 1;
    }
    out->type = eq->events[best].type;
    out->priority = eq->events[best].priority;
    out->data = eq->events[best].data;
    out->time = eq->events[best].time;

    eq->count = eq->count - 1;
    if (best != eq->count) {
        eq->events[best] = eq->events[eq->count];
    }
    return 1;
}

struct Timer {
    int interval;
    int remaining;
    int event_type;
    int active;
    int fire_count;
};

void timer_init(struct Timer *t, int interval, int etype) {
    t->interval = interval;
    t->remaining = interval;
    t->event_type = etype;
    t->active = 1;
    t->fire_count = 0;
}

int timer_tick(struct Timer *t) {
    if (t->active == 0) return 0;
    t->remaining = t->remaining - 1;
    if (t->remaining <= 0) {
        t->remaining = t->interval;
        t->fire_count = t->fire_count + 1;
        return 1;
    }
    return 0;
}

int handle_event(struct Event *e) {
    switch (e->type) {
        case 1: return e->data * 2;
        case 2: return e->data + 10;
        case 3: return e->data - 5;
        default: return 0;
    }
}

int run_event_loop(struct EventQueue *eq) {
    int total = 0;
    struct Event e;
    while (eq_pop_highest(eq, &e)) {
        total = total + handle_event(&e);
    }
    return total;
}

int main(void) {
    struct EventQueue eq;
    eq_init(&eq);

    eq_push(&eq, 1, 5, 10, 0);
    eq_push(&eq, 2, 10, 20, 1);
    eq_push(&eq, 3, 3, 15, 2);

    struct Event e;
    eq_pop_highest(&eq, &e);
    // EXPECT: first_type=2
    printf("first_type=%d\n", e.type);
    // EXPECT: first_data=20
    printf("first_data=%d\n", e.data);
    // EXPECT: remaining=2
    printf("remaining=%d\n", eq.count);

    eq_pop_highest(&eq, &e);
    // EXPECT: second_type=1
    printf("second_type=%d\n", e.type);

    eq_pop_highest(&eq, &e);
    // EXPECT: third_type=3
    printf("third_type=%d\n", e.type);
    // EXPECT: empty=0
    printf("empty=%d\n", eq.count);

    struct Timer t;
    timer_init(&t, 3, 1);
    int fires = 0;
    int i = 0;
    while (i < 10) {
        if (timer_tick(&t)) fires = fires + 1;
        i = i + 1;
    }
    // EXPECT: fires=3
    printf("fires=%d\n", fires);
    // EXPECT: firecount=3
    printf("firecount=%d\n", t.fire_count);

    struct EventQueue eq2;
    eq_init(&eq2);
    eq_push(&eq2, 1, 1, 5, 0);
    eq_push(&eq2, 2, 1, 3, 1);
    eq_push(&eq2, 1, 2, 7, 2);
    eq_push(&eq2, 3, 1, 8, 3);
    // EXPECT: looptotal=40
    printf("looptotal=%d\n", run_event_loop(&eq2));

    struct EventQueue eq3;
    eq_init(&eq3);
    eq_push(&eq3, 1, 5, 100, 0);
    eq_push(&eq3, 1, 5, 200, 1);
    struct Event e3;
    eq_pop_highest(&eq3, &e3);
    // EXPECT: tiebreak=100
    printf("tiebreak=%d\n", e3.data);

    struct Timer t2;
    timer_init(&t2, 5, 2);
    t2.active = 0;
    int fires2 = 0;
    i = 0;
    while (i < 10) {
        if (timer_tick(&t2)) fires2 = fires2 + 1;
        i = i + 1;
    }
    // EXPECT: inactive=0
    printf("inactive=%d\n", fires2);

    struct Event h1;
    h1.type = 1; h1.priority = 0; h1.data = 10; h1.time = 0;
    // EXPECT: handle1=20
    printf("handle1=%d\n", handle_event(&h1));
    struct Event h2;
    h2.type = 2; h2.priority = 0; h2.data = 20; h2.time = 0;
    // EXPECT: handle2=30
    printf("handle2=%d\n", handle_event(&h2));
    struct Event h3;
    h3.type = 3; h3.priority = 0; h3.data = 30; h3.time = 0;
    // EXPECT: handle3=25
    printf("handle3=%d\n", handle_event(&h3));

    return 0;
}
