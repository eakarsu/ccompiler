int printf(const char *fmt, ...);

// Traffic flow simulation: cellular automaton (Nagel-Schreckenberg-like)
// Road is a 1D array of cells. -1=empty, 0..max_speed=car with velocity
// Rules: accelerate, brake to avoid collision, random slowdown, move

void init_road(int *road, int n) {
    int i;
    for (i = 0; i < n; i++) {
        road[i] = -1;
    }
}

int count_cars(int *road, int n) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (road[i] >= 0) count++;
    }
    return count;
}

int gap_ahead(int *road, int n, int pos) {
    int gap = 0;
    int i;
    for (i = 1; i < n; i++) {
        int next = (pos + i) % n;
        if (road[next] >= 0) return gap;
        gap++;
    }
    return gap;
}

int total_velocity(int *road, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (road[i] >= 0) total = total + road[i];
    }
    return total;
}

typedef struct {
    int state;
} RNG;

void rng_init(RNG *r, int seed) {
    r->state = seed % 32749;
}

int rng_next(RNG *r) {
    r->state = (1021 * r->state + 13) % 32749;
    return r->state;
}

void traffic_step(int *road, int *new_road, int n, int max_speed, RNG *rng) {
    int i;
    for (i = 0; i < n; i++) new_road[i] = -1;

    for (i = 0; i < n; i++) {
        if (road[i] < 0) continue;
        int v = road[i];

        // Accelerate
        if (v < max_speed) v = v + 1;

        // Brake
        int gap = gap_ahead(road, n, i);
        if (v > gap) v = gap;

        // Random slowdown (30%)
        if (v > 0) {
            int r = rng_next(rng) % 10;
            if (r < 3) v = v - 1;
        }

        // Move
        int new_pos = (i + v) % n;
        new_road[new_pos] = v;
    }
}

void copy_road(int *dst, int *src, int n) {
    int i;
    for (i = 0; i < n; i++) dst[i] = src[i];
}

// Convert road to char array for printing
void road_to_str(int *road, int n, char *buf) {
    int i;
    for (i = 0; i < n; i++) {
        if (road[i] < 0) buf[i] = '.';
        else buf[i] = '0' + road[i];
    }
    buf[n] = 0;
}

int main(void) {
    int n = 20;
    int road[20];
    int new_road[20];
    int max_speed = 3;
    RNG rng;
    char buf[21];

    rng_init(&rng, 42);
    init_road(road, n);

    // Place 4 cars evenly spaced
    road[0] = 0;
    road[5] = 0;
    road[10] = 0;
    road[15] = 0;

    int cars = count_cars(road, n);
    printf("Cars: %d\n", cars);
    // EXPECT: Cars: 4

    road_to_str(road, n, buf);
    printf("Init: %s\n", buf);
    // EXPECT: Init: 0....0....0....0....

    // Step 1
    traffic_step(road, new_road, n, max_speed, &rng);
    copy_road(road, new_road, n);
    road_to_str(road, n, buf);
    printf("S1: %s\n", buf);
    // EXPECT: S1: .1....1...0.....1...

    // Step 2
    traffic_step(road, new_road, n, max_speed, &rng);
    copy_road(road, new_road, n);
    road_to_str(road, n, buf);
    printf("S2: %s\n", buf);
    // EXPECT: S2: ...2...1...1.....1..

    // Step 3
    traffic_step(road, new_road, n, max_speed, &rng);
    copy_road(road, new_road, n);
    road_to_str(road, n, buf);
    printf("S3: %s\n", buf);
    // EXPECT: S3: .....2...2..1......2

    int tv3 = total_velocity(road, n);
    printf("Velocity S3: %d\n", tv3);
    // EXPECT: Velocity S3: 7

    // Run to step 10
    int step;
    for (step = 4; step <= 10; step++) {
        traffic_step(road, new_road, n, max_speed, &rng);
        copy_road(road, new_road, n);
    }
    road_to_str(road, n, buf);
    printf("S10: %s\n", buf);
    // EXPECT: S10: ....2.....3..1.....2

    int tv10 = total_velocity(road, n);
    printf("Velocity S10: %d\n", tv10);
    // EXPECT: Velocity S10: 8

    int avg100 = tv10 * 100 / count_cars(road, n);
    printf("Avg speed x100: %d\n", avg100);
    // EXPECT: Avg speed x100: 200

    // Run to step 20
    for (step = 11; step <= 20; step++) {
        traffic_step(road, new_road, n, max_speed, &rng);
        copy_road(road, new_road, n);
    }
    road_to_str(road, n, buf);
    printf("S20: %s\n", buf);
    // EXPECT: S20: ..2.....3...3....3..

    int tv20 = total_velocity(road, n);
    printf("Velocity S20: %d\n", tv20);
    // EXPECT: Velocity S20: 11

    printf("Flow S20: %d\n", tv20);
    // EXPECT: Flow S20: 11

    int cars20 = count_cars(road, n);
    printf("Cars S20: %d\n", cars20);
    // EXPECT: Cars S20: 4

    // Density = cars * 100 / road_length
    int density = cars20 * 100 / n;
    printf("Density: %d\n", density);
    // EXPECT: Density: 20

    return 0;
}
