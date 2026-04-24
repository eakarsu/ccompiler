int printf(const char *fmt, ...);

struct Message {
    int from_id;
    int to_id;
    int type;
    int payload;
};

struct Component {
    int id;
    int inbox[8];
    int inbox_count;
    int total_received;
    int total_sent;
};

struct Mediator {
    struct Component components[8];
    int comp_count;
    struct Message log[32];
    int log_count;
    int total_routed;
};

void mediator_init(struct Mediator *m) {
    m->comp_count = 0;
    m->log_count = 0;
    m->total_routed = 0;
}

int mediator_add_component(struct Mediator *m) {
    int id;
    int i;
    id = m->comp_count;
    m->components[id].id = id;
    m->components[id].inbox_count = 0;
    m->components[id].total_received = 0;
    m->components[id].total_sent = 0;
    for (i = 0; i < 8; i++) {
        m->components[id].inbox[i] = 0;
    }
    m->comp_count = m->comp_count + 1;
    return id;
}

int mediator_send(struct Mediator *m, int from, int to, int type, int payload) {
    struct Component *target;
    struct Message *msg;
    if (to < 0 || to >= m->comp_count) return 0;
    if (from < 0 || from >= m->comp_count) return 0;

    target = &m->components[to];
    if (target->inbox_count >= 8) return 0;

    target->inbox[target->inbox_count] = payload;
    target->inbox_count = target->inbox_count + 1;
    target->total_received = target->total_received + 1;

    m->components[from].total_sent = m->components[from].total_sent + 1;

    if (m->log_count < 32) {
        msg = &m->log[m->log_count];
        msg->from_id = from;
        msg->to_id = to;
        msg->type = type;
        msg->payload = payload;
        m->log_count = m->log_count + 1;
    }
    m->total_routed = m->total_routed + 1;
    return 1;
}

int mediator_broadcast(struct Mediator *m, int from, int type, int payload) {
    int i;
    int sent;
    sent = 0;
    for (i = 0; i < m->comp_count; i++) {
        if (i != from) {
            if (mediator_send(m, from, i, type, payload)) {
                sent = sent + 1;
            }
        }
    }
    return sent;
}

int component_read_inbox(struct Mediator *m, int id) {
    struct Component *c;
    int sum;
    int i;
    c = &m->components[id];
    sum = 0;
    for (i = 0; i < c->inbox_count; i++) {
        sum = sum + c->inbox[i];
    }
    return sum;
}

void component_clear_inbox(struct Mediator *m, int id) {
    m->components[id].inbox_count = 0;
}

int component_inbox_count(struct Mediator *m, int id) {
    return m->components[id].inbox_count;
}

int component_total_received(struct Mediator *m, int id) {
    return m->components[id].total_received;
}

int component_total_sent(struct Mediator *m, int id) {
    return m->components[id].total_sent;
}

int log_count_from(struct Mediator *m, int from) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < m->log_count; i++) {
        if (m->log[i].from_id == from) {
            count = count + 1;
        }
    }
    return count;
}

int log_count_to(struct Mediator *m, int to) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < m->log_count; i++) {
        if (m->log[i].to_id == to) {
            count = count + 1;
        }
    }
    return count;
}

int main(void) {
    struct Mediator med;
    int a;
    int b;
    int c;
    int d;
    int ok;
    int sent;

    mediator_init(&med);
    a = mediator_add_component(&med);
    b = mediator_add_component(&med);
    c = mediator_add_component(&med);
    d = mediator_add_component(&med);

    // EXPECT: components: 4
    printf("components: %d\n", med.comp_count);
    // EXPECT: ids: 0 1 2 3
    printf("ids: %d %d %d %d\n", a, b, c, d);

    ok = mediator_send(&med, a, b, 1, 42);
    // EXPECT: send a->b: 1
    printf("send a->b: %d\n", ok);

    ok = mediator_send(&med, a, c, 1, 100);
    // EXPECT: send a->c: 1
    printf("send a->c: %d\n", ok);

    ok = mediator_send(&med, b, c, 2, 50);
    // EXPECT: send b->c: 1
    printf("send b->c: %d\n", ok);

    // EXPECT: b inbox count: 1
    printf("b inbox count: %d\n", component_inbox_count(&med, b));
    // EXPECT: b inbox sum: 42
    printf("b inbox sum: %d\n", component_read_inbox(&med, b));

    // EXPECT: c inbox count: 2
    printf("c inbox count: %d\n", component_inbox_count(&med, c));
    // EXPECT: c inbox sum: 150
    printf("c inbox sum: %d\n", component_read_inbox(&med, c));

    // EXPECT: a sent: 2
    printf("a sent: %d\n", component_total_sent(&med, a));
    // EXPECT: b received: 1
    printf("b received: %d\n", component_total_received(&med, b));
    // EXPECT: c received: 2
    printf("c received: %d\n", component_total_received(&med, c));

    // EXPECT: total routed: 3
    printf("total routed: %d\n", med.total_routed);
    // EXPECT: log size: 3
    printf("log size: %d\n", med.log_count);

    sent = mediator_broadcast(&med, d, 3, 77);
    // EXPECT: broadcast from d: 3
    printf("broadcast from d: %d\n", sent);

    // EXPECT: a inbox after broadcast: 1
    printf("a inbox after broadcast: %d\n", component_inbox_count(&med, a));
    // EXPECT: b inbox after broadcast: 2
    printf("b inbox after broadcast: %d\n", component_inbox_count(&med, b));

    // EXPECT: total routed now: 6
    printf("total routed now: %d\n", med.total_routed);

    // EXPECT: log from a: 2
    printf("log from a: %d\n", log_count_from(&med, a));
    // EXPECT: log to c: 3
    printf("log to c: %d\n", log_count_to(&med, c));

    component_clear_inbox(&med, c);
    // EXPECT: c inbox after clear: 0
    printf("c inbox after clear: %d\n", component_inbox_count(&med, c));
    // EXPECT: c total received: 3
    printf("c total received: %d\n", component_total_received(&med, c));

    ok = mediator_send(&med, a, 99, 1, 1);
    // EXPECT: invalid target: 0
    printf("invalid target: %d\n", ok);

    // EXPECT: d sent: 3
    printf("d sent: %d\n", component_total_sent(&med, d));

    return 0;
}
