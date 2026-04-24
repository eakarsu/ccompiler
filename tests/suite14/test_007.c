int printf(const char *fmt, ...);

/* Elevator controller state machine.
   States: IDLE=0, MOVING_UP=1, MOVING_DOWN=2, DOOR_OPEN=3, DOOR_CLOSING=4
   Handles requests for floors 0-4.
   Uses a request queue (bitmask of floor requests). */

typedef struct {
    int state;
    int current_floor;
    int target_floor;
    int requests;       /* bitmask: bit i = floor i requested */
    int direction;      /* 1=up, -1=down, 0=none */
    int door_timer;
    int floors_visited;
    int total_moves;
} Elevator;

void elev_init(Elevator *e) {
    e->state = 0;
    e->current_floor = 0;
    e->target_floor = 0;
    e->requests = 0;
    e->direction = 0;
    e->door_timer = 0;
    e->floors_visited = 0;
    e->total_moves = 0;
}

void elev_request(Elevator *e, int floor) {
    if (floor >= 0 && floor <= 4) {
        e->requests = e->requests | (1 << floor);
    }
}

int elev_has_request(Elevator *e) {
    return e->requests != 0;
}

int find_next_target_up(Elevator *e) {
    int f;
    for (f = e->current_floor; f <= 4; f++) {
        if (e->requests & (1 << f)) return f;
    }
    /* Nothing above, check below */
    for (f = e->current_floor - 1; f >= 0; f--) {
        if (e->requests & (1 << f)) return f;
    }
    return e->current_floor;
}

int find_next_target_down(Elevator *e) {
    int f;
    for (f = e->current_floor; f >= 0; f--) {
        if (e->requests & (1 << f)) return f;
    }
    /* Nothing below, check above */
    for (f = e->current_floor + 1; f <= 4; f++) {
        if (e->requests & (1 << f)) return f;
    }
    return e->current_floor;
}

int find_next_target(Elevator *e) {
    if (e->direction >= 0) {
        return find_next_target_up(e);
    } else {
        return find_next_target_down(e);
    }
}

void elev_step(Elevator *e) {
    int s = e->state;

    if (s == 0) {
        /* IDLE */
        if (elev_has_request(e)) {
            e->target_floor = find_next_target(e);
            if (e->target_floor == e->current_floor) {
                /* Already here, open door */
                e->state = 3; /* DOOR_OPEN */
                e->door_timer = 0;
                e->requests = e->requests & ~(1 << e->current_floor);
                e->floors_visited++;
            } else if (e->target_floor > e->current_floor) {
                e->state = 1; /* MOVING_UP */
                e->direction = 1;
            } else {
                e->state = 2; /* MOVING_DOWN */
                e->direction = -1;
            }
        }
    } else if (s == 1) {
        /* MOVING_UP */
        e->current_floor++;
        e->total_moves++;
        if (e->requests & (1 << e->current_floor)) {
            /* Stop at this floor */
            e->state = 3; /* DOOR_OPEN */
            e->door_timer = 0;
            e->requests = e->requests & ~(1 << e->current_floor);
            e->floors_visited++;
        } else if (e->current_floor >= e->target_floor) {
            /* Reached target without matching request (shouldn't happen normally) */
            e->state = 0; /* IDLE */
        }
    } else if (s == 2) {
        /* MOVING_DOWN */
        e->current_floor--;
        e->total_moves++;
        if (e->requests & (1 << e->current_floor)) {
            e->state = 3; /* DOOR_OPEN */
            e->door_timer = 0;
            e->requests = e->requests & ~(1 << e->current_floor);
            e->floors_visited++;
        } else if (e->current_floor <= e->target_floor) {
            e->state = 0; /* IDLE */
        }
    } else if (s == 3) {
        /* DOOR_OPEN */
        e->door_timer++;
        if (e->door_timer >= 2) {
            e->state = 4; /* DOOR_CLOSING */
        }
    } else if (s == 4) {
        /* DOOR_CLOSING */
        if (elev_has_request(e)) {
            e->target_floor = find_next_target(e);
            if (e->target_floor > e->current_floor) {
                e->state = 1;
                e->direction = 1;
            } else if (e->target_floor < e->current_floor) {
                e->state = 2;
                e->direction = -1;
            } else {
                e->state = 3;
                e->door_timer = 0;
                e->requests = e->requests & ~(1 << e->current_floor);
                e->floors_visited++;
            }
        } else {
            e->state = 0; /* IDLE */
            e->direction = 0;
        }
    }
}

void test_single_request(void) {
    Elevator e;

    printf("=== Single Request ===\n");
    // EXPECT: === Single Request ===

    elev_init(&e);
    elev_request(&e, 3);

    printf("floor=%d target req=3\n", e.current_floor);
    // EXPECT: floor=0 target req=3

    elev_step(&e); /* IDLE -> MOVING_UP, target=3 */
    printf("step1: state=%d floor=%d\n", e.state, e.current_floor);
    // EXPECT: step1: state=1 floor=0

    elev_step(&e); /* move to 1 */
    printf("step2: floor=%d\n", e.current_floor);
    // EXPECT: step2: floor=1

    elev_step(&e); /* move to 2 */
    printf("step3: floor=%d\n", e.current_floor);
    // EXPECT: step3: floor=2

    elev_step(&e); /* move to 3 -> DOOR_OPEN */
    printf("step4: state=%d floor=%d\n", e.state, e.current_floor);
    // EXPECT: step4: state=3 floor=3

    elev_step(&e); /* door open tick 1 */
    elev_step(&e); /* door open tick 2 -> DOOR_CLOSING */
    printf("door closing: state=%d\n", e.state);
    // EXPECT: door closing: state=4

    elev_step(&e); /* no more requests -> IDLE */
    printf("idle: state=%d floor=%d\n", e.state, e.current_floor);
    // EXPECT: idle: state=0 floor=3
}

void test_multiple_floors(void) {
    Elevator e;
    int i;

    printf("=== Multiple Floors ===\n");
    // EXPECT: === Multiple Floors ===

    elev_init(&e);
    elev_request(&e, 1);
    elev_request(&e, 3);

    /* Run until idle */
    for (i = 0; i < 20; i++) {
        elev_step(&e);
        if (e.state == 0 && !elev_has_request(&e)) break;
    }

    printf("visited=%d floor=%d moves=%d\n",
           e.floors_visited, e.current_floor, e.total_moves);
    // EXPECT: visited=2 floor=3 moves=3
}

void test_down_request(void) {
    Elevator e;
    int i;

    printf("=== Down Request ===\n");
    // EXPECT: === Down Request ===

    elev_init(&e);
    /* Move to floor 4 first */
    elev_request(&e, 4);
    for (i = 0; i < 20; i++) {
        elev_step(&e);
        if (e.state == 0 && !elev_has_request(&e)) break;
    }
    printf("at floor: %d\n", e.current_floor);
    // EXPECT: at floor: 4

    /* Now request floor 1 */
    elev_request(&e, 1);
    for (i = 0; i < 20; i++) {
        elev_step(&e);
        if (e.state == 0 && !elev_has_request(&e)) break;
    }
    printf("went to: %d moves=%d\n", e.current_floor, e.total_moves);
    // EXPECT: went to: 1 moves=7

    printf("total visited: %d\n", e.floors_visited);
    // EXPECT: total visited: 2
}

void test_same_floor(void) {
    Elevator e;

    printf("=== Same Floor ===\n");
    // EXPECT: === Same Floor ===

    elev_init(&e);
    elev_request(&e, 0); /* request current floor */

    elev_step(&e); /* should open door immediately */
    printf("state=%d floor=%d\n", e.state, e.current_floor);
    // EXPECT: state=3 floor=0
    printf("moves=%d visited=%d\n", e.total_moves, e.floors_visited);
    // EXPECT: moves=0 visited=1
}

void test_request_during_move(void) {
    Elevator e;
    int i;

    printf("=== Request During Move ===\n");
    // EXPECT: === Request During Move ===

    elev_init(&e);
    elev_request(&e, 4);

    elev_step(&e); /* start moving up */
    elev_step(&e); /* at floor 1 */

    /* Add request for floor 2 while moving */
    elev_request(&e, 2);
    elev_step(&e); /* at floor 2 -> should stop here (request exists) */
    printf("stopped at: %d state=%d\n", e.current_floor, e.state);
    // EXPECT: stopped at: 2 state=3

    /* Continue to floor 4 after door closes */
    for (i = 0; i < 20; i++) {
        elev_step(&e);
        if (e.state == 0 && !elev_has_request(&e)) break;
    }
    printf("final: floor=%d visited=%d\n", e.current_floor, e.floors_visited);
    // EXPECT: final: floor=4 visited=2
}

int main(void) {
    test_single_request();
    test_multiple_floors();
    test_down_request();
    test_same_floor();
    test_request_during_move();

    printf("Elevator done.\n");
    // EXPECT: Elevator done.
    return 0;
}
