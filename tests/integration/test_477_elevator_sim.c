int printf(const char *fmt, ...);
// EXPECT: === Elevator Simulation ===\nGoing to pick up at floor 1\n  Picked up at floor 1, passengers: 1\n  Dropped off at floor 5, trips done: 1\nGoing to pick up at floor 3\n  Picked up at floor 3, passengers: 1\n  Dropped off at floor 1, trips done: 2\nGoing to pick up at floor 2\n  Picked up at floor 2, passengers: 1\n  Dropped off at floor 7, trips done: 3\nGoing to pick up at floor 7\n  Picked up at floor 7, passengers: 1\n  Dropped off at floor 2, trips done: 4\nGoing to pick up at floor 4\n  Picked up at floor 4, passengers: 1\n  Dropped off at floor 6, trips done: 5\n--- Results ---\nTotal trips: 5\nTotal distance: 23 floors\nFinal floor: 6\nAll served: yes

enum Direction { DIR_UP, DIR_DOWN, DIR_IDLE };

struct Elevator {
    int current_floor;
    int target_floor;
    enum Direction dir;
    int passengers;
    int max_capacity;
    int trips_completed;
};

struct Request {
    int from_floor;
    int to_floor;
    int served;
};

void init_elevator(struct Elevator *e) {
    e->current_floor = 1;
    e->target_floor = 1;
    e->dir = DIR_IDLE;
    e->passengers = 0;
    e->max_capacity = 4;
    e->trips_completed = 0;
}

void move_elevator(struct Elevator *e) {
    if (e->current_floor < e->target_floor) {
        e->current_floor++;
        e->dir = DIR_UP;
    } else if (e->current_floor > e->target_floor) {
        e->current_floor--;
        e->dir = DIR_DOWN;
    } else {
        e->dir = DIR_IDLE;
    }
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int main(void) {
    struct Elevator elev;
    init_elevator(&elev);

    struct Request reqs[5];
    reqs[0].from_floor = 1; reqs[0].to_floor = 5; reqs[0].served = 0;
    reqs[1].from_floor = 3; reqs[1].to_floor = 1; reqs[1].served = 0;
    reqs[2].from_floor = 2; reqs[2].to_floor = 7; reqs[2].served = 0;
    reqs[3].from_floor = 7; reqs[3].to_floor = 2; reqs[3].served = 0;
    reqs[4].from_floor = 4; reqs[4].to_floor = 6; reqs[4].served = 0;

    printf("=== Elevator Simulation ===\n");

    int step;
    int total_distance = 0;
    int req_idx;

    for (req_idx = 0; req_idx < 5; req_idx++) {
        elev.target_floor = reqs[req_idx].from_floor;
        printf("Going to pick up at floor %d\n", reqs[req_idx].from_floor);

        while (elev.current_floor != elev.target_floor) {
            move_elevator(&elev);
            total_distance++;
        }

        elev.passengers++;
        printf("  Picked up at floor %d, passengers: %d\n",
               elev.current_floor, elev.passengers);

        elev.target_floor = reqs[req_idx].to_floor;
        while (elev.current_floor != elev.target_floor) {
            move_elevator(&elev);
            total_distance++;
        }

        elev.passengers--;
        elev.trips_completed++;
        reqs[req_idx].served = 1;
        printf("  Dropped off at floor %d, trips done: %d\n",
               elev.current_floor, elev.trips_completed);
    }

    printf("--- Results ---\n");
    printf("Total trips: %d\n", elev.trips_completed);
    printf("Total distance: %d floors\n", total_distance);
    printf("Final floor: %d\n", elev.current_floor);

    int all_served = 1;
    int i;
    for (i = 0; i < 5; i++) {
        if (!reqs[i].served) all_served = 0;
    }
    printf("All served: %s\n", all_served ? "yes" : "no");

    return 0;
}
