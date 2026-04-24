int printf(const char *fmt, ...);

// Random walk simulation using a small LCG (no overflow)
// LCG: next = (a * state + c) % m
// a=1021, c=13, m=32749 (all fit in int without overflow)

typedef struct {
    int state;
} RNG;

void rng_init(RNG *r, int seed) {
    r->state = seed % 32749;
    if (r->state < 0) r->state = r->state + 32749;
}

int rng_next(RNG *r) {
    r->state = (1021 * r->state + 13) % 32749;
    return r->state;
}

// Return -1 or +1
int random_step(RNG *r) {
    int v = rng_next(r);
    if (v % 2 == 0) return 1;
    return -1;
}

// Return 0..3 for 2D directions
int random_dir(RNG *r) {
    int v = rng_next(r);
    return v % 4;
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

// Simulate a 1D random walk
int walk_1d(RNG *r, int steps) {
    int pos = 0;
    int i;
    for (i = 0; i < steps; i++) {
        pos = pos + random_step(r);
    }
    return pos;
}

typedef struct {
    int x;
    int y;
} Point;

void walk_2d(RNG *r, int steps, Point *result) {
    int x = 0;
    int y = 0;
    int i;
    for (i = 0; i < steps; i++) {
        int dir = random_dir(r);
        if (dir == 0) x++;
        else if (dir == 1) y++;
        else if (dir == 2) x--;
        else y--;
    }
    result->x = x;
    result->y = y;
}

int count_returns(RNG *r, int steps) {
    int pos = 0;
    int returns = 0;
    int i;
    for (i = 0; i < steps; i++) {
        pos = pos + random_step(r);
        if (pos == 0) returns++;
    }
    return returns;
}

int max_displacement(RNG *r, int steps) {
    int pos = 0;
    int max_disp = 0;
    int i;
    for (i = 0; i < steps; i++) {
        pos = pos + random_step(r);
        int d = abs_val(pos);
        if (d > max_disp) max_disp = d;
    }
    return max_disp;
}

void build_histogram(int *positions, int n, int *hist, int size) {
    int i;
    int offset = size / 2;
    for (i = 0; i < size; i++) hist[i] = 0;
    for (i = 0; i < n; i++) {
        int bin = positions[i] + offset;
        if (bin >= 0 && bin < size) hist[bin]++;
    }
}

int main(void) {
    RNG rng;

    // Test 1: 1D walk seed=42
    rng_init(&rng, 42);
    int pos = walk_1d(&rng, 10);
    printf("Walk1D(10,42): %d\n", pos);
    // EXPECT: Walk1D(10,42): 4

    // Test 2: 1D walk seed=123
    rng_init(&rng, 123);
    pos = walk_1d(&rng, 10);
    printf("Walk1D(10,123): %d\n", pos);
    // EXPECT: Walk1D(10,123): 2

    // Test 3: Longer walk
    rng_init(&rng, 42);
    pos = walk_1d(&rng, 100);
    printf("Walk1D(100,42): %d\n", pos);
    // EXPECT: Walk1D(100,42): 20

    // Test 4: 2D walk
    Point pt;
    rng_init(&rng, 42);
    walk_2d(&rng, 20, &pt);
    printf("Walk2D(20): x=%d y=%d\n", pt.x, pt.y);
    // EXPECT: Walk2D(20): x=-4 y=-4

    // Test 5: Returns to origin
    rng_init(&rng, 42);
    int ret = count_returns(&rng, 50);
    printf("Returns(50): %d\n", ret);
    // EXPECT: Returns(50): 2

    // Test 6: Max displacement
    rng_init(&rng, 42);
    int maxd = max_displacement(&rng, 50);
    printf("MaxDisp(50): %d\n", maxd);
    // EXPECT: MaxDisp(50): 7

    // Test 7: Multiple walks
    int finals[10];
    int i;
    for (i = 0; i < 10; i++) {
        rng_init(&rng, 100 + i);
        finals[i] = walk_1d(&rng, 20);
    }
    printf("Finals:");
    for (i = 0; i < 10; i++) {
        printf(" %d", finals[i]);
    }
    printf("\n");
    // EXPECT: Finals: 0 -2 -4 2 6 0 4 2 -2 -6

    int sum = 0;
    for (i = 0; i < 10; i++) sum = sum + finals[i];
    printf("Sum: %d\n", sum);
    // EXPECT: Sum: 0

    int msd = 0;
    for (i = 0; i < 10; i++) msd = msd + finals[i] * finals[i];
    printf("SumSq: %d\n", msd);
    // EXPECT: SumSq: 120

    // Test 8: Trajectory histogram
    int traj[21];
    rng_init(&rng, 42);
    traj[0] = 0;
    for (i = 1; i <= 20; i++) {
        traj[i] = traj[i - 1] + random_step(&rng);
    }
    printf("TrajEnd: %d\n", traj[20]);
    // EXPECT: TrajEnd: 4

    int hist[15]; // positions -7 to +7
    build_histogram(traj, 21, hist, 15);
    printf("Hist:");
    for (i = 0; i < 15; i++) {
        printf(" %d", hist[i]);
    }
    printf("\n");
    // EXPECT: Hist: 0 0 0 0 0 0 0 3 4 4 6 4 0 0 0

    return 0;
}
