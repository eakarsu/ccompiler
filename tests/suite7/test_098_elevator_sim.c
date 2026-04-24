int printf(const char *fmt, ...);

// Elevator system simulation: request queue, floor tracking, direction, scheduling

enum {
    DIR_UP = 1,
    DIR_DOWN = -1,
    DIR_IDLE = 0
};

struct Request {
    int from_floor;
    int to_floor;
    int served;
};

struct Elevator {
    int current_floor;
    int direction;
    int doors_open;
    int stops_made;
    int total_distance;
    int passengers;
    int capacity;
};

struct Building {
    struct Elevator elev;
    struct Request requests[16];
    int req_count;
    int num_floors;
};

void building_init(struct Building *b, int floors, int capacity) {
    b->num_floors = floors;
    b->req_count = 0;
    b->elev.current_floor = 1;
    b->elev.direction = DIR_IDLE;
    b->elev.doors_open = 0;
    b->elev.stops_made = 0;
    b->elev.total_distance = 0;
    b->elev.passengers = 0;
    b->elev.capacity = capacity;
}

void add_request(struct Building *b, int from, int to) {
    int idx = b->req_count;
    b->requests[idx].from_floor = from;
    b->requests[idx].to_floor = to;
    b->requests[idx].served = 0;
    b->req_count = b->req_count + 1;
}

int has_pending(struct Building *b) {
    int i;
    for (i = 0; i < b->req_count; i++) {
        if (b->requests[i].served == 0) return 1;
    }
    return 0;
}

// Find nearest unserved request
int find_nearest(struct Building *b) {
    int best = -1;
    int best_dist = 999;
    int i;
    for (i = 0; i < b->req_count; i++) {
        if (b->requests[i].served) continue;
        int dist = b->requests[i].from_floor - b->elev.current_floor;
        if (dist < 0) dist = -dist;
        if (dist < best_dist) {
            best_dist = dist;
            best = i;
        }
    }
    return best;
}

// Move elevator one floor toward target
int move_toward(struct Building *b, int target) {
    if (b->elev.current_floor < target) {
        b->elev.current_floor = b->elev.current_floor + 1;
        b->elev.direction = DIR_UP;
        b->elev.total_distance = b->elev.total_distance + 1;
        return 1;
    } else if (b->elev.current_floor > target) {
        b->elev.current_floor = b->elev.current_floor - 1;
        b->elev.direction = DIR_DOWN;
        b->elev.total_distance = b->elev.total_distance + 1;
        return 1;
    }
    return 0; // already there
}

// Serve a single request completely
void serve_request(struct Building *b, int req_idx) {
    // Move to pickup floor
    while (b->elev.current_floor != b->requests[req_idx].from_floor) {
        move_toward(b, b->requests[req_idx].from_floor);
    }
    // Pick up
    b->elev.doors_open = 1;
    b->elev.passengers = b->elev.passengers + 1;
    b->elev.stops_made = b->elev.stops_made + 1;
    b->elev.doors_open = 0;

    // Move to destination
    while (b->elev.current_floor != b->requests[req_idx].to_floor) {
        move_toward(b, b->requests[req_idx].to_floor);
    }
    // Drop off
    b->elev.doors_open = 1;
    b->elev.passengers = b->elev.passengers - 1;
    b->elev.stops_made = b->elev.stops_made + 1;
    b->elev.doors_open = 0;

    b->requests[req_idx].served = 1;
}

// Run nearest-first scheduling
void run_nearest_first(struct Building *b) {
    while (has_pending(b)) {
        int next = find_nearest(b);
        if (next == -1) break;
        serve_request(b, next);
    }
    b->elev.direction = DIR_IDLE;
}

// SCAN algorithm (elevator algorithm): go up serving requests, then down
void run_scan(struct Building *b) {
    int floor;
    int i;
    int served_any;

    // Go up from current to top
    for (floor = b->elev.current_floor; floor <= b->num_floors; floor++) {
        served_any = 0;
        for (i = 0; i < b->req_count; i++) {
            if (b->requests[i].served == 0 && b->requests[i].from_floor == floor) {
                while (b->elev.current_floor != floor) move_toward(b, floor);
                serve_request(b, i);
                served_any = 1;
            }
        }
    }
    // Go down from top to bottom
    for (floor = b->num_floors; floor >= 1; floor--) {
        for (i = 0; i < b->req_count; i++) {
            if (b->requests[i].served == 0 && b->requests[i].from_floor == floor) {
                while (b->elev.current_floor != floor) move_toward(b, floor);
                serve_request(b, i);
            }
        }
    }
    b->elev.direction = DIR_IDLE;
}

int main(void) {
    struct Building b;

    // Initialize 10-floor building, capacity 5
    building_init(&b, 10, 5);
    // EXPECT: Init: floor=1 dir=0 capacity=5
    printf("Init: floor=%d dir=%d capacity=%d\n",
           b.elev.current_floor, b.elev.direction, b.elev.capacity);

    // Single request: floor 1 -> floor 5
    add_request(&b, 1, 5);
    // EXPECT: Requests: 1
    printf("Requests: %d\n", b.req_count);

    run_nearest_first(&b);
    // EXPECT: After serve: floor=5 stops=2 dist=4
    printf("After serve: floor=%d stops=%d dist=%d\n",
           b.elev.current_floor, b.elev.stops_made, b.elev.total_distance);

    // EXPECT: Passengers=0 direction=0
    printf("Passengers=%d direction=%d\n", b.elev.passengers, b.elev.direction);

    // EXPECT: Request 0 served=1
    printf("Request 0 served=%d\n", b.requests[0].served);

    // Multiple requests with nearest-first
    building_init(&b, 10, 5);
    add_request(&b, 3, 7);
    add_request(&b, 2, 5);
    add_request(&b, 8, 1);
    // EXPECT: Pending: 1
    printf("Pending: %d\n", has_pending(&b));

    run_nearest_first(&b);
    // EXPECT: All served: pending=0
    printf("All served: pending=%d\n", has_pending(&b));

    // EXPECT: Nearest-first stops=6
    printf("Nearest-first stops=%d\n", b.elev.stops_made);

    // 1->2=1, 2->5=3, 5->3=2, 3->7=4, 7->8=1, 8->1=7 = 18
    // EXPECT: Nearest-first distance=18
    printf("Nearest-first distance=%d\n", b.elev.total_distance);

    // SCAN algorithm test
    building_init(&b, 10, 5);
    add_request(&b, 3, 7);
    add_request(&b, 8, 2);
    add_request(&b, 5, 1);

    run_scan(&b);
    // EXPECT: SCAN all served: pending=0
    printf("SCAN all served: pending=%d\n", has_pending(&b));

    // EXPECT: SCAN stops=6
    printf("SCAN stops=%d\n", b.elev.stops_made);

    // Test direction tracking
    building_init(&b, 10, 5);
    move_toward(&b, 5);
    // EXPECT: Move up: dir=1
    printf("Move up: dir=%d\n", b.elev.direction);

    b.elev.current_floor = 5;
    move_toward(&b, 3);
    // EXPECT: Move down: dir=-1
    printf("Move down: dir=%d\n", b.elev.direction);

    // Test find nearest
    building_init(&b, 10, 5);
    b.elev.current_floor = 5;
    add_request(&b, 2, 8);
    add_request(&b, 6, 1);
    add_request(&b, 9, 3);
    {
        int nearest = find_nearest(&b);
        // EXPECT: Nearest to 5: req=1 floor=6
        printf("Nearest to 5: req=%d floor=%d\n", nearest, b.requests[nearest].from_floor);
    }

    // Edge case: already on floor
    building_init(&b, 10, 5);
    b.elev.current_floor = 3;
    add_request(&b, 3, 7);
    run_nearest_first(&b);
    // EXPECT: Same floor: dist=4 stops=2
    printf("Same floor: dist=%d stops=%d\n", b.elev.total_distance, b.elev.stops_made);

    // EXPECT: Elevator sim complete
    printf("Elevator sim complete\n");

    return 0;
}
