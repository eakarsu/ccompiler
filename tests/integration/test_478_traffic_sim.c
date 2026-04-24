int printf(const char *fmt, ...);
// EXPECT: === Traffic Light Simulation ===\nTick 0: NS=GREEN EW=RED (passed NS:2 EW:0)\nTick 4: NS=RED EW=GREEN (passed NS:6 EW:2)\nTick 8: NS=GREEN EW=RED (passed NS:7 EW:6)\nTick 12: NS=RED EW=GREEN (passed NS:10 EW:8)\nTick 16: NS=GREEN EW=RED (passed NS:11 EW:12)\n--- Results ---\nTotal ticks: 20\nCars passed NS: 15\nCars passed EW: 12\nTotal cars: 27

enum LightColor { RED, YELLOW, GREEN };

struct TrafficLight {
    enum LightColor color;
    int timer;
    int green_dur;
    int yellow_dur;
    int red_dur;
};

struct Intersection {
    struct TrafficLight ns;
    struct TrafficLight ew;
    int cars_passed_ns;
    int cars_passed_ew;
    int total_ticks;
};

void init_light(struct TrafficLight *l, int g, int y, int r, enum LightColor start) {
    l->color = start;
    l->timer = 0;
    l->green_dur = g;
    l->yellow_dur = y;
    l->red_dur = r;
}

const char *color_name(enum LightColor c) {
    if (c == RED) return "RED";
    if (c == YELLOW) return "YELLOW";
    return "GREEN";
}

void tick_light(struct TrafficLight *l) {
    l->timer++;
    if (l->color == GREEN && l->timer >= l->green_dur) {
        l->color = YELLOW;
        l->timer = 0;
    } else if (l->color == YELLOW && l->timer >= l->yellow_dur) {
        l->color = RED;
        l->timer = 0;
    } else if (l->color == RED && l->timer >= l->red_dur) {
        l->color = GREEN;
        l->timer = 0;
    }
}

int main(void) {
    struct Intersection inter;
    init_light(&inter.ns, 3, 1, 4, GREEN);
    init_light(&inter.ew, 3, 1, 4, RED);
    inter.cars_passed_ns = 0;
    inter.cars_passed_ew = 0;
    inter.total_ticks = 0;

    int queue_ns[10];
    int queue_ew[10];
    int i;
    for (i = 0; i < 10; i++) {
        queue_ns[i] = (i * 3 + 1) % 5 + 1;
        queue_ew[i] = (i * 2 + 3) % 4 + 1;
    }

    printf("=== Traffic Light Simulation ===\n");

    int tick;
    int ns_idx = 0;
    int ew_idx = 0;

    for (tick = 0; tick < 20; tick++) {
        inter.total_ticks++;

        if (inter.ns.color == GREEN && ns_idx < 10) {
            int cars = queue_ns[ns_idx];
            if (cars > 2) cars = 2;
            inter.cars_passed_ns = inter.cars_passed_ns + cars;
            queue_ns[ns_idx] = queue_ns[ns_idx] - cars;
            if (queue_ns[ns_idx] <= 0) ns_idx++;
        }

        if (inter.ew.color == GREEN && ew_idx < 10) {
            int cars = queue_ew[ew_idx];
            if (cars > 2) cars = 2;
            inter.cars_passed_ew = inter.cars_passed_ew + cars;
            queue_ew[ew_idx] = queue_ew[ew_idx] - cars;
            if (queue_ew[ew_idx] <= 0) ew_idx++;
        }

        if (tick % 4 == 0) {
            printf("Tick %d: NS=%s EW=%s (passed NS:%d EW:%d)\n",
                   tick, color_name(inter.ns.color), color_name(inter.ew.color),
                   inter.cars_passed_ns, inter.cars_passed_ew);
        }

        tick_light(&inter.ns);
        tick_light(&inter.ew);
    }

    printf("--- Results ---\n");
    printf("Total ticks: %d\n", inter.total_ticks);
    printf("Cars passed NS: %d\n", inter.cars_passed_ns);
    printf("Cars passed EW: %d\n", inter.cars_passed_ew);
    printf("Total cars: %d\n", inter.cars_passed_ns + inter.cars_passed_ew);

    return 0;
}
