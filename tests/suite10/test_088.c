int printf(const char *fmt, ...);

// Chain of Responsibility: handlers try to process requests in order

enum RequestType { REQ_LOW, REQ_MED, REQ_HIGH, REQ_CRITICAL };

struct Request {
    enum RequestType type;
    int id;
    char *description;
    int handled;
};

typedef int (*HandlerFunc)(struct Request *);

struct Handler {
    HandlerFunc handle;
    char *name;
    int next_idx;
    int active;
};

struct Chain {
    struct Handler handlers[8];
    int count;
    int head;
};

void chain_init(struct Chain *ch) {
    ch->count = 0;
    ch->head = -1;
}

void chain_add(struct Chain *ch, HandlerFunc func, char *name) {
    int idx;
    idx = ch->count;
    ch->handlers[idx].handle = func;
    ch->handlers[idx].name = name;
    ch->handlers[idx].next_idx = -1;
    ch->handlers[idx].active = 1;

    if (ch->count > 0) {
        ch->handlers[ch->count - 1].next_idx = idx;
    } else {
        ch->head = idx;
    }
    ch->count = ch->count + 1;
}

int chain_process(struct Chain *ch, struct Request *req) {
    int cur;
    cur = ch->head;
    while (cur >= 0) {
        if (ch->handlers[cur].active) {
            int result;
            result = ch->handlers[cur].handle(req);
            if (result) {
                printf("Handled by: %s\n", ch->handlers[cur].name);
                req->handled = 1;
                return 1;
            }
        }
        cur = ch->handlers[cur].next_idx;
    }
    return 0;
}

int handle_low(struct Request *req) {
    if (req->type == REQ_LOW) {
        printf("Low handler processing: %s\n", req->description);
        return 1;
    }
    return 0;
}

int handle_med(struct Request *req) {
    if (req->type == REQ_MED) {
        printf("Med handler processing: %s\n", req->description);
        return 1;
    }
    return 0;
}

int handle_high(struct Request *req) {
    if (req->type == REQ_HIGH) {
        printf("High handler processing: %s\n", req->description);
        return 1;
    }
    return 0;
}

int handle_critical(struct Request *req) {
    if (req->type == REQ_CRITICAL) {
        printf("CRITICAL handler processing: %s\n", req->description);
        return 1;
    }
    return 0;
}

int handle_catchall(struct Request *req) {
    printf("Catchall handler processing: %s\n", req->description);
    return 1;
}

int main() {
    struct Chain chain;
    chain_init(&chain);

    chain_add(&chain, handle_low, "LowHandler");
    chain_add(&chain, handle_med, "MedHandler");
    chain_add(&chain, handle_high, "HighHandler");
    chain_add(&chain, handle_critical, "CriticalHandler");

    printf("Chain length: %d\n", chain.count); // EXPECT: Chain length: 4

    struct Request r1;
    r1.type = REQ_LOW;
    r1.id = 1;
    r1.description = "minor fix";
    r1.handled = 0;

    int ok;
    ok = chain_process(&chain, &r1); // EXPECT: Low handler processing: minor fix
    // EXPECT: Handled by: LowHandler
    printf("Request %d handled: %d\n", r1.id, ok); // EXPECT: Request 1 handled: 1

    struct Request r2;
    r2.type = REQ_HIGH;
    r2.id = 2;
    r2.description = "urgent issue";
    r2.handled = 0;

    ok = chain_process(&chain, &r2); // EXPECT: High handler processing: urgent issue
    // EXPECT: Handled by: HighHandler
    printf("Request %d handled: %d\n", r2.id, ok); // EXPECT: Request 2 handled: 1

    struct Request r3;
    r3.type = REQ_CRITICAL;
    r3.id = 3;
    r3.description = "system down";
    r3.handled = 0;

    ok = chain_process(&chain, &r3); // EXPECT: CRITICAL handler processing: system down
    // EXPECT: Handled by: CriticalHandler
    printf("Request %d handled: %d\n", r3.id, ok); // EXPECT: Request 3 handled: 1

    printf("--- Disable high handler ---\n"); // EXPECT: --- Disable high handler ---
    chain.handlers[2].active = 0;

    struct Request r4;
    r4.type = REQ_HIGH;
    r4.id = 4;
    r4.description = "another urgent";
    r4.handled = 0;

    ok = chain_process(&chain, &r4);
    printf("Request %d handled: %d\n", r4.id, ok); // EXPECT: Request 4 handled: 0

    printf("--- Add catchall ---\n"); // EXPECT: --- Add catchall ---
    chain_add(&chain, handle_catchall, "CatchAll");

    ok = chain_process(&chain, &r4); // EXPECT: Catchall handler processing: another urgent
    // EXPECT: Handled by: CatchAll
    printf("Request %d now handled: %d\n", r4.id, ok); // EXPECT: Request 4 now handled: 1

    struct Request r5;
    r5.type = REQ_MED;
    r5.id = 5;
    r5.description = "medium task";
    r5.handled = 0;

    ok = chain_process(&chain, &r5); // EXPECT: Med handler processing: medium task
    // EXPECT: Handled by: MedHandler
    printf("Request %d handled: %d\n", r5.id, ok); // EXPECT: Request 5 handled: 1

    printf("Total handlers: %d\n", chain.count); // EXPECT: Total handlers: 5
    printf("Done\n"); // EXPECT: Done
    return 0;
}
