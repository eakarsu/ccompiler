int printf(const char *fmt, ...);
// EXPECT: === Traffic Light FSM ===
// EXPECT: State: RED
// EXPECT: Transition: RED -> GREEN
// EXPECT: State: GREEN
// EXPECT: Transition: GREEN -> YELLOW
// EXPECT: State: YELLOW
// EXPECT: Transition: YELLOW -> RED
// EXPECT: State: RED
// EXPECT: Transition: RED -> GREEN
// EXPECT: State: GREEN
// EXPECT: Full cycle completed: 4 transitions
// EXPECT: === Vending Machine FSM ===
// EXPECT: Vending state: IDLE, balance: 0
// EXPECT: Inserted coin: 25, balance: 25
// EXPECT: Vending state: COINS_INSERTED, balance: 25
// EXPECT: Inserted coin: 25, balance: 50
// EXPECT: Vending state: COINS_INSERTED, balance: 50
// EXPECT: Inserted coin: 25, balance: 75
// EXPECT: Vending state: COINS_INSERTED, balance: 75
// EXPECT: Selected item at price 75
// EXPECT: Vending state: DISPENSING, balance: 0
// EXPECT: Item dispensed!
// EXPECT: Vending state: IDLE, balance: 0
// EXPECT: Inserted coin: 10, balance: 10
// EXPECT: Inserted coin: 25, balance: 35
// EXPECT: Cancel requested, returning 35 cents
// EXPECT: Vending state: IDLE, balance: 0
// EXPECT: === Parser State Machine ===
// EXPECT: Parsing: char 97 -> state TEXT
// EXPECT: Parsing: char 98 -> state TEXT
// EXPECT: Parsing: char 60 -> state TAG_OPEN
// EXPECT: Parsing: char 116 -> state TAG_NAME
// EXPECT: Parsing: char 62 -> state TEXT
// EXPECT: Parsing: char 99 -> state TEXT
// EXPECT: Parsing: char 60 -> state TAG_OPEN
// EXPECT: Parsing: char 47 -> state TAG_CLOSE
// EXPECT: Parsing: char 116 -> state TAG_CLOSE
// EXPECT: Parsing: char 62 -> state TEXT
// EXPECT: Parser finished: 1 tags found
// EXPECT: === Elevator State Machine ===
// EXPECT: Elevator at floor 1, state: IDLE
// EXPECT: Request for floor 5
// EXPECT: Elevator MOVING_UP, at floor 2
// EXPECT: Elevator MOVING_UP, at floor 3
// EXPECT: Elevator MOVING_UP, at floor 4
// EXPECT: Elevator MOVING_UP, at floor 5
// EXPECT: Elevator STOPPED at floor 5
// EXPECT: Elevator IDLE at floor 5
// EXPECT: Request for floor 2
// EXPECT: Elevator MOVING_DOWN, at floor 4
// EXPECT: Elevator MOVING_DOWN, at floor 3
// EXPECT: Elevator MOVING_DOWN, at floor 2
// EXPECT: Elevator STOPPED at floor 2
// EXPECT: Elevator IDLE at floor 2
// EXPECT: Total floors traveled: 7
// EXPECT: === Event Queue Processing ===
// EXPECT: Processing event type 1 with data 10
// EXPECT: Processing event type 2 with data 20
// EXPECT: Processing event type 3 with data 30
// EXPECT: Processing event type 1 with data 40
// EXPECT: Processing event type 2 with data 50
// EXPECT: Events processed: 5
// EXPECT: Event type 1 count: 2
// EXPECT: Event type 2 count: 2
// EXPECT: Event type 3 count: 1
// EXPECT: === Turnstile FSM ===
// EXPECT: Turnstile: LOCKED
// EXPECT: Push -> still LOCKED
// EXPECT: Coin -> UNLOCKED
// EXPECT: Coin -> still UNLOCKED
// EXPECT: Push -> LOCKED (passed through)
// EXPECT: Coin -> UNLOCKED
// EXPECT: Push -> LOCKED (passed through)
// EXPECT: People passed: 2
// EXPECT: Coins collected: 3
// EXPECT: === State Transition Table ===
// EXPECT: From state 0: to 1 on event 0, to 0 on event 1
// EXPECT: From state 1: to 2 on event 0, to 0 on event 1
// EXPECT: From state 2: to 3 on event 0, to 0 on event 1
// EXPECT: From state 3: to 3 on event 0, to 0 on event 1
// EXPECT: Running events: 0 0 1 0 0 0
// EXPECT: Step 0: state 0 + event 0 -> state 1
// EXPECT: Step 1: state 1 + event 0 -> state 2
// EXPECT: Step 2: state 2 + event 1 -> state 0
// EXPECT: Step 3: state 0 + event 0 -> state 1
// EXPECT: Step 4: state 1 + event 0 -> state 2
// EXPECT: Step 5: state 2 + event 0 -> state 3
// EXPECT: Final state: 3
// EXPECT: === All state machine tests passed ===

// ============================================================
// Traffic Light FSM
// ============================================================
enum TrafficLight { TL_RED, TL_GREEN, TL_YELLOW };

void print_traffic_state(int state) {
    if (state == TL_RED) printf("State: RED\n");
    else if (state == TL_GREEN) printf("State: GREEN\n");
    else if (state == TL_YELLOW) printf("State: YELLOW\n");
}

int traffic_next_state(int state) {
    if (state == TL_RED) return TL_GREEN;
    if (state == TL_GREEN) return TL_YELLOW;
    if (state == TL_YELLOW) return TL_RED;
    return TL_RED;
}

void print_traffic_transition(int from, int to) {
    printf("Transition: ");
    if (from == TL_RED) printf("RED");
    else if (from == TL_GREEN) printf("GREEN");
    else if (from == TL_YELLOW) printf("YELLOW");
    printf(" -> ");
    if (to == TL_RED) printf("RED");
    else if (to == TL_GREEN) printf("GREEN");
    else if (to == TL_YELLOW) printf("YELLOW");
    printf("\n");
}

void test_traffic_light(void) {
    int state;
    int transitions;
    int i;
    int next;

    printf("=== Traffic Light FSM ===\n");
    state = TL_RED;
    transitions = 0;

    print_traffic_state(state);

    for (i = 0; i < 4; i++) {
        next = traffic_next_state(state);
        print_traffic_transition(state, next);
        state = next;
        transitions++;
        print_traffic_state(state);
    }
    printf("Full cycle completed: %d transitions\n", transitions);
}

// ============================================================
// Vending Machine FSM
// ============================================================
enum VendState { VS_IDLE, VS_COINS_INSERTED, VS_DISPENSING };

struct VendingMachine {
    int state;
    int balance;
};

void vend_print_state(struct VendingMachine *vm) {
    printf("Vending state: ");
    if (vm->state == VS_IDLE) printf("IDLE");
    else if (vm->state == VS_COINS_INSERTED) printf("COINS_INSERTED");
    else if (vm->state == VS_DISPENSING) printf("DISPENSING");
    printf(", balance: %d\n", vm->balance);
}

void vend_insert_coin(struct VendingMachine *vm, int amount) {
    if (vm->state == VS_IDLE || vm->state == VS_COINS_INSERTED) {
        vm->balance = vm->balance + amount;
        printf("Inserted coin: %d, balance: %d\n", amount, vm->balance);
        vm->state = VS_COINS_INSERTED;
    }
}

void vend_select_item(struct VendingMachine *vm, int price) {
    if (vm->state == VS_COINS_INSERTED && vm->balance >= price) {
        printf("Selected item at price %d\n", price);
        vm->balance = vm->balance - price;
        vm->state = VS_DISPENSING;
    }
}

void vend_dispense(struct VendingMachine *vm) {
    if (vm->state == VS_DISPENSING) {
        printf("Item dispensed!\n");
        vm->state = VS_IDLE;
    }
}

void vend_cancel(struct VendingMachine *vm) {
    if (vm->state == VS_COINS_INSERTED) {
        printf("Cancel requested, returning %d cents\n", vm->balance);
        vm->balance = 0;
        vm->state = VS_IDLE;
    }
}

void test_vending_machine(void) {
    struct VendingMachine vm;

    printf("=== Vending Machine FSM ===\n");
    vm.state = VS_IDLE;
    vm.balance = 0;

    vend_print_state(&vm);

    vend_insert_coin(&vm, 25);
    vend_print_state(&vm);
    vend_insert_coin(&vm, 25);
    vend_print_state(&vm);
    vend_insert_coin(&vm, 25);
    vend_print_state(&vm);

    vend_select_item(&vm, 75);
    vend_print_state(&vm);
    vend_dispense(&vm);
    vend_print_state(&vm);

    vend_insert_coin(&vm, 10);
    vend_insert_coin(&vm, 25);
    vend_cancel(&vm);
    vend_print_state(&vm);
}

// ============================================================
// Parser State Machine
// ============================================================
enum ParserState { PS_TEXT, PS_TAG_OPEN, PS_TAG_NAME, PS_TAG_CLOSE };

struct Parser {
    int state;
    int tag_count;
};

void parser_process_char(struct Parser *p, int ch) {
    if (p->state == PS_TEXT) {
        if (ch == 60) {
            p->state = PS_TAG_OPEN;
        }
    } else if (p->state == PS_TAG_OPEN) {
        if (ch == 47) {
            p->state = PS_TAG_CLOSE;
        } else {
            p->state = PS_TAG_NAME;
            p->tag_count++;
        }
    } else if (p->state == PS_TAG_NAME) {
        if (ch == 62) {
            p->state = PS_TEXT;
        }
    } else if (p->state == PS_TAG_CLOSE) {
        if (ch == 62) {
            p->state = PS_TEXT;
        }
    }

    printf("Parsing: char %d -> state ", ch);
    if (p->state == PS_TEXT) printf("TEXT");
    else if (p->state == PS_TAG_OPEN) printf("TAG_OPEN");
    else if (p->state == PS_TAG_NAME) printf("TAG_NAME");
    else if (p->state == PS_TAG_CLOSE) printf("TAG_CLOSE");
    printf("\n");
}

void test_parser_fsm(void) {
    struct Parser p;
    int input[10];
    int i;

    printf("=== Parser State Machine ===\n");
    p.state = PS_TEXT;
    p.tag_count = 0;

    input[0] = 97;
    input[1] = 98;
    input[2] = 60;
    input[3] = 116;
    input[4] = 62;
    input[5] = 99;
    input[6] = 60;
    input[7] = 47;
    input[8] = 116;
    input[9] = 62;

    for (i = 0; i < 10; i++) {
        parser_process_char(&p, input[i]);
    }

    printf("Parser finished: %d tags found\n", p.tag_count);
}

// ============================================================
// Elevator State Machine
// ============================================================
enum ElevState { ES_IDLE, ES_MOVING_UP, ES_MOVING_DOWN, ES_STOPPED };

struct Elevator {
    int state;
    int current_floor;
    int target_floor;
    int total_traveled;
};

void elevator_request(struct Elevator *e, int floor) {
    printf("Request for floor %d\n", floor);
    e->target_floor = floor;
    if (floor > e->current_floor) {
        e->state = ES_MOVING_UP;
    } else if (floor < e->current_floor) {
        e->state = ES_MOVING_DOWN;
    }
}

int elevator_step(struct Elevator *e) {
    if (e->state == ES_MOVING_UP) {
        e->current_floor++;
        e->total_traveled++;
        printf("Elevator MOVING_UP, at floor %d\n", e->current_floor);
        if (e->current_floor == e->target_floor) {
            e->state = ES_STOPPED;
            printf("Elevator STOPPED at floor %d\n", e->current_floor);
            return 1;
        }
    } else if (e->state == ES_MOVING_DOWN) {
        e->current_floor--;
        e->total_traveled++;
        printf("Elevator MOVING_DOWN, at floor %d\n", e->current_floor);
        if (e->current_floor == e->target_floor) {
            e->state = ES_STOPPED;
            printf("Elevator STOPPED at floor %d\n", e->current_floor);
            return 1;
        }
    }
    return 0;
}

void test_elevator(void) {
    struct Elevator e;
    int arrived;

    printf("=== Elevator State Machine ===\n");
    e.state = ES_IDLE;
    e.current_floor = 1;
    e.target_floor = 1;
    e.total_traveled = 0;

    printf("Elevator at floor %d, state: IDLE\n", e.current_floor);

    elevator_request(&e, 5);
    arrived = 0;
    while (arrived == 0) {
        arrived = elevator_step(&e);
    }
    e.state = ES_IDLE;
    printf("Elevator IDLE at floor %d\n", e.current_floor);

    elevator_request(&e, 2);
    arrived = 0;
    while (arrived == 0) {
        arrived = elevator_step(&e);
    }
    e.state = ES_IDLE;
    printf("Elevator IDLE at floor %d\n", e.current_floor);

    printf("Total floors traveled: %d\n", e.total_traveled);
}

// ============================================================
// Event Queue Processing
// ============================================================
struct Event {
    int type;
    int data;
};

struct EventQueue {
    struct Event events[20];
    int head;
    int tail;
    int count;
};

void eq_init(struct EventQueue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

void eq_push(struct EventQueue *q, int type, int data) {
    if (q->count < 20) {
        q->events[q->tail].type = type;
        q->events[q->tail].data = data;
        q->tail = (q->tail + 1) % 20;
        q->count++;
    }
}

int eq_pop(struct EventQueue *q, struct Event *out) {
    if (q->count > 0) {
        out->type = q->events[q->head].type;
        out->data = q->events[q->head].data;
        q->head = (q->head + 1) % 20;
        q->count--;
        return 1;
    }
    return 0;
}

void test_event_queue(void) {
    struct EventQueue q;
    struct Event ev;
    int processed;
    int type_counts[4];
    int i;

    printf("=== Event Queue Processing ===\n");
    eq_init(&q);

    eq_push(&q, 1, 10);
    eq_push(&q, 2, 20);
    eq_push(&q, 3, 30);
    eq_push(&q, 1, 40);
    eq_push(&q, 2, 50);

    processed = 0;
    for (i = 0; i < 4; i++) type_counts[i] = 0;

    while (eq_pop(&q, &ev)) {
        printf("Processing event type %d with data %d\n", ev.type, ev.data);
        if (ev.type >= 1 && ev.type <= 3) {
            type_counts[ev.type]++;
        }
        processed++;
    }

    printf("Events processed: %d\n", processed);
    printf("Event type 1 count: %d\n", type_counts[1]);
    printf("Event type 2 count: %d\n", type_counts[2]);
    printf("Event type 3 count: %d\n", type_counts[3]);
}

// ============================================================
// Turnstile FSM
// ============================================================
enum TurnstileState { TS_LOCKED, TS_UNLOCKED };
enum TurnstileEvent { TE_COIN, TE_PUSH };

struct Turnstile {
    int state;
    int people_passed;
    int coins_collected;
};

void turnstile_process(struct Turnstile *t, int event) {
    if (t->state == TS_LOCKED) {
        if (event == TE_COIN) {
            t->coins_collected++;
            t->state = TS_UNLOCKED;
            printf("Coin -> UNLOCKED\n");
        } else {
            printf("Push -> still LOCKED\n");
        }
    } else {
        if (event == TE_COIN) {
            t->coins_collected++;
            printf("Coin -> still UNLOCKED\n");
        } else {
            t->people_passed++;
            t->state = TS_LOCKED;
            printf("Push -> LOCKED (passed through)\n");
        }
    }
}

void test_turnstile(void) {
    struct Turnstile t;

    printf("=== Turnstile FSM ===\n");
    t.state = TS_LOCKED;
    t.people_passed = 0;
    t.coins_collected = 0;

    printf("Turnstile: LOCKED\n");
    turnstile_process(&t, TE_PUSH);
    turnstile_process(&t, TE_COIN);
    turnstile_process(&t, TE_COIN);
    turnstile_process(&t, TE_PUSH);
    turnstile_process(&t, TE_COIN);
    turnstile_process(&t, TE_PUSH);

    printf("People passed: %d\n", t.people_passed);
    printf("Coins collected: %d\n", t.coins_collected);
}

// ============================================================
// State Transition Table
// ============================================================
int transition_table[4][2];

void init_transition_table(void) {
    transition_table[0][0] = 1;
    transition_table[0][1] = 0;
    transition_table[1][0] = 2;
    transition_table[1][1] = 0;
    transition_table[2][0] = 3;
    transition_table[2][1] = 0;
    transition_table[3][0] = 3;
    transition_table[3][1] = 0;
}

void test_transition_table(void) {
    int events[6];
    int state;
    int i;

    printf("=== State Transition Table ===\n");
    init_transition_table();

    for (i = 0; i < 4; i++) {
        printf("From state %d: to %d on event 0, to %d on event 1\n",
               i, transition_table[i][0], transition_table[i][1]);
    }

    events[0] = 0;
    events[1] = 0;
    events[2] = 1;
    events[3] = 0;
    events[4] = 0;
    events[5] = 0;

    printf("Running events:");
    for (i = 0; i < 6; i++) {
        printf(" %d", events[i]);
    }
    printf("\n");

    state = 0;
    for (i = 0; i < 6; i++) {
        int old_state;
        int new_state;
        old_state = state;
        new_state = transition_table[state][events[i]];
        printf("Step %d: state %d + event %d -> state %d\n",
               i, old_state, events[i], new_state);
        state = new_state;
    }
    printf("Final state: %d\n", state);
}

// ============================================================
// Additional silent FSMs for code volume
// ============================================================
struct DoorFSM {
    int state;
    int timer;
    int open_count;
};

void door_init(struct DoorFSM *d) {
    d->state = 0;
    d->timer = 0;
    d->open_count = 0;
}

int door_tick(struct DoorFSM *d, int button_pressed) {
    if (d->state == 0) {
        if (button_pressed) {
            d->state = 1;
            d->timer = 3;
        }
    } else if (d->state == 1) {
        d->timer--;
        if (d->timer <= 0) {
            d->state = 2;
            d->timer = 5;
            d->open_count++;
        }
    } else if (d->state == 2) {
        d->timer--;
        if (d->timer <= 0) {
            d->state = 3;
            d->timer = 3;
        }
    } else if (d->state == 3) {
        d->timer--;
        if (d->timer <= 0) {
            d->state = 0;
        }
    }
    return d->state;
}

enum WashState { WS_OFF, WS_FILL, WS_WASH, WS_RINSE, WS_SPIN, WS_DONE };

struct WashMachine {
    int state;
    int timer;
    int cycles_done;
};

void wash_init(struct WashMachine *w) {
    w->state = WS_OFF;
    w->timer = 0;
    w->cycles_done = 0;
}

int wash_tick(struct WashMachine *w) {
    if (w->state == WS_OFF) {
        return 0;
    } else if (w->state == WS_FILL) {
        w->timer--;
        if (w->timer <= 0) {
            w->state = WS_WASH;
            w->timer = 4;
        }
    } else if (w->state == WS_WASH) {
        w->timer--;
        if (w->timer <= 0) {
            w->state = WS_RINSE;
            w->timer = 3;
        }
    } else if (w->state == WS_RINSE) {
        w->timer--;
        if (w->timer <= 0) {
            w->state = WS_SPIN;
            w->timer = 2;
        }
    } else if (w->state == WS_SPIN) {
        w->timer--;
        if (w->timer <= 0) {
            w->state = WS_DONE;
            w->cycles_done++;
        }
    }
    return w->state;
}

void wash_start(struct WashMachine *w) {
    if (w->state == WS_OFF || w->state == WS_DONE) {
        w->state = WS_FILL;
        w->timer = 2;
    }
}

enum ConnState { CS_DISCONNECTED, CS_CONNECTING, CS_CONNECTED, CS_DISCONNECTING };

struct Connection {
    int state;
    int retry_count;
    int max_retries;
    int connected_time;
};

void conn_init(struct Connection *c) {
    c->state = CS_DISCONNECTED;
    c->retry_count = 0;
    c->max_retries = 3;
    c->connected_time = 0;
}

int conn_connect(struct Connection *c) {
    if (c->state == CS_DISCONNECTED) {
        c->state = CS_CONNECTING;
        c->retry_count = 0;
        return 1;
    }
    return 0;
}

int conn_tick(struct Connection *c, int success) {
    if (c->state == CS_CONNECTING) {
        if (success) {
            c->state = CS_CONNECTED;
            c->connected_time = 0;
            return 1;
        } else {
            c->retry_count++;
            if (c->retry_count >= c->max_retries) {
                c->state = CS_DISCONNECTED;
                return -1;
            }
            return 0;
        }
    } else if (c->state == CS_CONNECTED) {
        c->connected_time++;
        return 1;
    } else if (c->state == CS_DISCONNECTING) {
        c->state = CS_DISCONNECTED;
        return 0;
    }
    return 0;
}

int conn_disconnect(struct Connection *c) {
    if (c->state == CS_CONNECTED) {
        c->state = CS_DISCONNECTING;
        return 1;
    }
    return 0;
}

enum OrderState { OS_PENDING, OS_CONFIRMED, OS_SHIPPED, OS_DELIVERED, OS_CANCELLED };

struct Order {
    int state;
    int id;
    int amount;
};

int order_confirm(struct Order *o) {
    if (o->state == OS_PENDING) {
        o->state = OS_CONFIRMED;
        return 1;
    }
    return 0;
}

int order_ship(struct Order *o) {
    if (o->state == OS_CONFIRMED) {
        o->state = OS_SHIPPED;
        return 1;
    }
    return 0;
}

int order_deliver(struct Order *o) {
    if (o->state == OS_SHIPPED) {
        o->state = OS_DELIVERED;
        return 1;
    }
    return 0;
}

int order_cancel(struct Order *o) {
    if (o->state == OS_PENDING || o->state == OS_CONFIRMED) {
        o->state = OS_CANCELLED;
        return 1;
    }
    return 0;
}

void run_door_simulation(void) {
    struct DoorFSM d;
    int i;
    int buttons[15];
    door_init(&d);
    for (i = 0; i < 15; i++) buttons[i] = 0;
    buttons[0] = 1;
    buttons[10] = 1;
    for (i = 0; i < 15; i++) {
        door_tick(&d, buttons[i]);
    }
}

void run_wash_simulation(void) {
    struct WashMachine w;
    int i;
    wash_init(&w);
    wash_start(&w);
    for (i = 0; i < 20; i++) {
        wash_tick(&w);
        if (w.state == WS_DONE) break;
    }
}

void run_conn_simulation(void) {
    struct Connection c;
    int results[5];
    int i;
    conn_init(&c);
    conn_connect(&c);
    results[0] = 0;
    results[1] = 0;
    results[2] = 1;
    for (i = 0; i < 3; i++) {
        conn_tick(&c, results[i]);
    }
    conn_tick(&c, 1);
    conn_disconnect(&c);
    conn_tick(&c, 0);
}

void run_order_simulation(void) {
    struct Order orders[3];
    int i;
    for (i = 0; i < 3; i++) {
        orders[i].state = OS_PENDING;
        orders[i].id = i + 1;
        orders[i].amount = (i + 1) * 100;
    }
    order_confirm(&orders[0]);
    order_ship(&orders[0]);
    order_deliver(&orders[0]);
    order_confirm(&orders[1]);
    order_cancel(&orders[1]);
}

int main(void) {
    test_traffic_light();
    test_vending_machine();
    test_parser_fsm();
    test_elevator();
    test_event_queue();
    test_turnstile();
    test_transition_table();
    run_door_simulation();
    run_wash_simulation();
    run_conn_simulation();
    run_order_simulation();
    printf("=== All state machine tests passed ===\n");
    return 0;
}
