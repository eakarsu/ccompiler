int printf(const char *fmt, ...);

enum ElevState { ELEV_IDLE, ELEV_MOVING_UP, ELEV_MOVING_DOWN, ELEV_DOOR_OPEN };

struct Elevator {
    int state;
    int current_floor;
    int target_floor;
    int passengers;
    int max_passengers;
    int trips_completed;
    int request_queue[10];
    int queue_size;
};

void init_elevator(struct Elevator *e, int max_p) {
    e->state = ELEV_IDLE;
    e->current_floor = 1;
    e->target_floor = 1;
    e->passengers = 0;
    e->max_passengers = max_p;
    e->trips_completed = 0;
    e->queue_size = 0;
}

void add_request(struct Elevator *e, int floor) {
    if (e->queue_size < 10) {
        int i;
        for (i = 0; i < e->queue_size; i = i + 1) {
            if (e->request_queue[i] == floor) return;
        }
        e->request_queue[e->queue_size] = floor;
        e->queue_size = e->queue_size + 1;
    }
}

void remove_front_request(struct Elevator *e) {
    if (e->queue_size > 0) {
        int i;
        for (i = 0; i < e->queue_size - 1; i = i + 1) {
            e->request_queue[i] = e->request_queue[i + 1];
        }
        e->queue_size = e->queue_size - 1;
    }
}

int get_next_target(struct Elevator *e) {
    if (e->queue_size > 0) return e->request_queue[0];
    return -1;
}

const char *elev_state_name(int s) {
    if (s == ELEV_IDLE) return "IDLE";
    if (s == ELEV_MOVING_UP) return "UP";
    if (s == ELEV_MOVING_DOWN) return "DOWN";
    if (s == ELEV_DOOR_OPEN) return "OPEN";
    return "???";
}

void step_elevator(struct Elevator *e) {
    int target;
    switch (e->state) {
    case ELEV_IDLE:
        target = get_next_target(e);
        if (target >= 0) {
            e->target_floor = target;
            if (target > e->current_floor)
                e->state = ELEV_MOVING_UP;
            else if (target < e->current_floor)
                e->state = ELEV_MOVING_DOWN;
            else {
                e->state = ELEV_DOOR_OPEN;
            }
        }
        break;
    case ELEV_MOVING_UP:
        e->current_floor = e->current_floor + 1;
        if (e->current_floor == e->target_floor) {
            e->state = ELEV_DOOR_OPEN;
        }
        break;
    case ELEV_MOVING_DOWN:
        e->current_floor = e->current_floor - 1;
        if (e->current_floor == e->target_floor) {
            e->state = ELEV_DOOR_OPEN;
        }
        break;
    case ELEV_DOOR_OPEN:
        remove_front_request(e);
        e->trips_completed = e->trips_completed + 1;
        e->state = ELEV_IDLE;
        break;
    }
}

void board_passengers(struct Elevator *e, int count) {
    int space = e->max_passengers - e->passengers;
    int boarded;
    if (count < space) boarded = count;
    else boarded = space;
    e->passengers = e->passengers + boarded;
    printf("Boarded %d, total: %d\n", boarded, e->passengers);
}

void exit_passengers(struct Elevator *e, int count) {
    int exited;
    if (count < e->passengers) exited = count;
    else exited = e->passengers;
    e->passengers = e->passengers - exited;
    printf("Exited %d, remaining: %d\n", exited, e->passengers);
}

int main() {
    struct Elevator elev;
    init_elevator(&elev, 6);

    printf("Initial: floor=%d state=%s\n", elev.current_floor, elev_state_name(elev.state));
    // EXPECT: Initial: floor=1 state=IDLE

    add_request(&elev, 5);
    add_request(&elev, 3);

    int step;
    for (step = 0; step < 10; step = step + 1) {
        step_elevator(&elev);
        printf("Step %d: floor=%d state=%s\n", step, elev.current_floor, elev_state_name(elev.state));
    }
    // EXPECT: Step 0: floor=1 state=UP
    // EXPECT: Step 1: floor=2 state=UP
    // EXPECT: Step 2: floor=3 state=UP
    // EXPECT: Step 3: floor=4 state=UP
    // EXPECT: Step 4: floor=5 state=OPEN
    // EXPECT: Step 5: floor=5 state=IDLE
    // EXPECT: Step 6: floor=5 state=DOWN
    // EXPECT: Step 7: floor=4 state=DOWN
    // EXPECT: Step 8: floor=3 state=OPEN
    // EXPECT: Step 9: floor=3 state=IDLE

    printf("Trips: %d\n", elev.trips_completed);
    // EXPECT: Trips: 2

    board_passengers(&elev, 4);
    // EXPECT: Boarded 4, total: 4
    board_passengers(&elev, 3);
    // EXPECT: Boarded 2, total: 6
    exit_passengers(&elev, 2);
    // EXPECT: Exited 2, remaining: 4

    add_request(&elev, 1);
    int s2;
    for (s2 = 0; s2 < 4; s2 = s2 + 1) {
        step_elevator(&elev);
        printf("S2 %d: floor=%d state=%s\n", s2, elev.current_floor, elev_state_name(elev.state));
    }
    // EXPECT: S2 0: floor=3 state=DOWN
    // EXPECT: S2 1: floor=2 state=DOWN
    // EXPECT: S2 2: floor=1 state=OPEN
    // EXPECT: S2 3: floor=1 state=IDLE

    printf("Queue empty: %d\n", elev.queue_size == 0);
    // EXPECT: Queue empty: 1
    printf("Total trips: %d\n", elev.trips_completed);
    // EXPECT: Total trips: 3

    return 0;
}
