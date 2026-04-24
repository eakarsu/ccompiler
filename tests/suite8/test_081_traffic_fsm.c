int printf(const char *fmt, ...);

enum TrafficState { RED, GREEN, YELLOW };

struct TrafficLight {
    int state;
    int timer;
    int cycle_count;
};

void init_light(struct TrafficLight *tl) {
    tl->state = RED;
    tl->timer = 0;
    tl->cycle_count = 0;
}

const char *state_name(int state) {
    if (state == RED) return "RED";
    if (state == GREEN) return "GREEN";
    if (state == YELLOW) return "YELLOW";
    return "UNKNOWN";
}

int get_duration(int state) {
    if (state == RED) return 5;
    if (state == GREEN) return 4;
    if (state == YELLOW) return 2;
    return 0;
}

int next_state(int state) {
    if (state == RED) return GREEN;
    if (state == GREEN) return YELLOW;
    if (state == YELLOW) return RED;
    return RED;
}

void tick(struct TrafficLight *tl) {
    tl->timer = tl->timer + 1;
    int duration = get_duration(tl->state);
    if (tl->timer >= duration) {
        int old_state = tl->state;
        tl->state = next_state(tl->state);
        tl->timer = 0;
        if (tl->state == RED) {
            tl->cycle_count = tl->cycle_count + 1;
        }
    }
}

void print_state(struct TrafficLight *tl, int time) {
    printf("T=%d: %s (timer=%d)\n", time, state_name(tl->state), tl->timer);
}

void run_intersection(struct TrafficLight *north, struct TrafficLight *east, int steps) {
    int i;
    for (i = 0; i < steps; i = i + 1) {
        printf("Step %d: North=%s East=%s\n", i, state_name(north->state), state_name(east->state));
        // EXPECT: Step 0: North=RED East=GREEN
        // EXPECT: Step 1: North=RED East=GREEN
        // EXPECT: Step 2: North=RED East=GREEN
        // EXPECT: Step 3: North=RED East=GREEN
        // EXPECT: Step 4: North=RED East=YELLOW
        // EXPECT: Step 5: North=GREEN East=YELLOW
        // EXPECT: Step 6: North=GREEN East=RED
        // EXPECT: Step 7: North=GREEN East=RED
        // EXPECT: Step 8: North=GREEN East=RED
        // EXPECT: Step 9: North=YELLOW East=RED
        // EXPECT: Step 10: North=YELLOW East=RED
        // EXPECT: Step 11: North=RED East=GREEN
        tick(north);
        tick(east);
    }
}

int can_cross(int light_state) {
    return light_state == GREEN;
}

int is_caution(int light_state) {
    return light_state == YELLOW;
}

int is_stopped(int light_state) {
    return light_state == RED;
}

int main() {
    struct TrafficLight north;
    struct TrafficLight east;
    init_light(&north);
    init_light(&east);

    east.state = GREEN;
    east.timer = 0;

    run_intersection(&north, &east, 12);

    printf("North cycles: %d\n", north.cycle_count);
    // EXPECT: North cycles: 1
    printf("East cycles: %d\n", east.cycle_count);
    // EXPECT: East cycles: 1

    printf("Can cross north: %d\n", can_cross(north.state));
    // EXPECT: Can cross north: 0
    printf("Is north red: %d\n", is_stopped(north.state));
    // EXPECT: Is north red: 1
    printf("Can cross east: %d\n", can_cross(east.state));
    // EXPECT: Can cross east: 1

    struct TrafficLight solo;
    init_light(&solo);
    int t;
    for (t = 0; t < 22; t = t + 1) {
        tick(&solo);
    }
    printf("Solo cycles after 22 ticks: %d\n", solo.cycle_count);
    // EXPECT: Solo cycles after 22 ticks: 2
    printf("Solo state: %s\n", state_name(solo.state));
    // EXPECT: Solo state: RED

    return 0;
}
