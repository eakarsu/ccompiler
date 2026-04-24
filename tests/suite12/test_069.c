int printf(const char *fmt, ...);

// Odometry simulation: tracking robot position from wheel encoder ticks
// Uses integer math scaled by 1000 for position (millimeters)
// Heading in degrees (0-359)

// Integer sine/cosine tables for 0-90 degrees, scaled by 1000
int sin_tab[91];

void build_sin_table(void) {
    int vals[10];
    vals[0] = 0;
    vals[1] = 174;
    vals[2] = 342;
    vals[3] = 500;
    vals[4] = 643;
    vals[5] = 766;
    vals[6] = 866;
    vals[7] = 940;
    vals[8] = 985;
    vals[9] = 1000;

    int i;
    for (i = 0; i <= 90; i++) {
        int b = i / 10;
        int f = i - b * 10;
        int v0 = vals[b];
        int v1;
        if (b < 9) {
            v1 = vals[b + 1];
        } else {
            v1 = 1000;
        }
        sin_tab[i] = v0 + (v1 - v0) * f / 10;
    }
}

int get_sin(int deg) {
    while (deg < 0) deg = deg + 360;
    while (deg >= 360) deg = deg - 360;
    if (deg <= 90) return sin_tab[deg];
    if (deg <= 180) return sin_tab[180 - deg];
    if (deg <= 270) return -sin_tab[deg - 180];
    return -sin_tab[360 - deg];
}

int get_cos(int deg) {
    return get_sin(deg + 90);
}

struct RobotState {
    int x;          // position x in mm (scaled by 1000)
    int y;          // position y in mm (scaled by 1000)
    int heading;    // heading in degrees (0 = east, 90 = north)
};

// Wheel parameters
// wheel_base: distance between wheels in mm
// ticks_per_rev: encoder ticks per wheel revolution
// wheel_circumference: in mm

struct OdometryConfig {
    int wheel_base;
    int ticks_per_rev;
    int wheel_circ;   // wheel circumference in mm
};

// Update robot state given left and right encoder tick deltas
struct RobotState update_odometry(struct RobotState state, struct OdometryConfig *cfg, int left_ticks, int right_ticks) {
    // Distance traveled by each wheel (mm, scaled by 1000 for precision)
    int left_dist = left_ticks * cfg->wheel_circ * 1000 / cfg->ticks_per_rev;
    int right_dist = right_ticks * cfg->wheel_circ * 1000 / cfg->ticks_per_rev;

    // Center distance and turn
    int center_dist = (left_dist + right_dist) / 2;

    // Change in heading (degrees, scaled by 1000)
    // delta_theta = (right - left) / wheel_base * (180/pi)
    // Approximate 180/pi ~ 57
    int delta_heading_scaled = (right_dist - left_dist) * 57 / cfg->wheel_base;

    // Update heading (unscale from 1000)
    int new_heading = state.heading + delta_heading_scaled / 1000;
    while (new_heading < 0) new_heading = new_heading + 360;
    while (new_heading >= 360) new_heading = new_heading - 360;

    // Average heading for position update
    int avg_heading = state.heading + delta_heading_scaled / 2000;

    // Update position
    // dx = center_dist * cos(heading), dy = center_dist * sin(heading)
    int dx = center_dist * get_cos(avg_heading) / 1000;
    int dy = center_dist * get_sin(avg_heading) / 1000;

    struct RobotState result;
    result.x = state.x + dx / 1000;
    result.y = state.y + dy / 1000;
    result.heading = new_heading;
    return result;
}

// Compute distance between two points
int distance(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int dsq = dx * dx + dy * dy;
    // Integer sqrt
    if (dsq <= 0) return 0;
    int r = dsq;
    int s = (r + 1) / 2;
    while (s < r) {
        r = s;
        s = (r + dsq / r) / 2;
    }
    return r;
}

// Record path as array of positions
struct PathPoint {
    int x;
    int y;
    int heading;
};

int main(void) {
    build_sin_table();

    struct OdometryConfig cfg;
    cfg.wheel_base = 200;       // 200mm between wheels
    cfg.ticks_per_rev = 100;    // 100 ticks per revolution
    cfg.wheel_circ = 314;       // ~100mm diameter wheel, circumference 314mm

    struct RobotState robot;
    robot.x = 0;
    robot.y = 0;
    robot.heading = 0;  // facing east

    printf("=== Initial State ===\n");                            // EXPECT: === Initial State ===
    printf("pos=(%d,%d) heading=%d\n", robot.x, robot.y, robot.heading); // EXPECT: pos=(0,0) heading=0

    // Test 1: Drive straight forward (equal ticks on both wheels)
    printf("=== Drive Straight ===\n");                           // EXPECT: === Drive Straight ===
    robot = update_odometry(robot, &cfg, 100, 100);
    printf("pos=(%d,%d) heading=%d\n", robot.x, robot.y, robot.heading); // EXPECT: pos=(314,0) heading=0

    // Another straight segment
    robot = update_odometry(robot, &cfg, 100, 100);
    printf("pos=(%d,%d) heading=%d\n", robot.x, robot.y, robot.heading); // EXPECT: pos=(628,0) heading=0

    // Test 2: Turn in place (opposite ticks)
    printf("=== Turn In Place ===\n");                            // EXPECT: === Turn In Place ===
    struct RobotState turner;
    turner.x = 0;
    turner.y = 0;
    turner.heading = 0;

    // Right wheel forward, left wheel backward -> turn left (CCW)
    turner = update_odometry(turner, &cfg, -10, 10);
    printf("turn heading=%d\n", turner.heading);                  // EXPECT: turn heading=17
    printf("turn pos=(%d,%d)\n", turner.x, turner.y);             // EXPECT: turn pos=(0,0)

    // Test 3: Arc motion (different speeds)
    printf("=== Arc Motion ===\n");                               // EXPECT: === Arc Motion ===
    struct RobotState arcer;
    arcer.x = 0;
    arcer.y = 0;
    arcer.heading = 0;

    // Left slower than right -> curves left
    arcer = update_odometry(arcer, &cfg, 50, 80);
    printf("arc heading=%d\n", arcer.heading);                    // EXPECT: arc heading=26
    printf("arc x_positive=%d\n", arcer.x > 0 ? 1 : 0);         // EXPECT: arc x_positive=1
    printf("arc y_positive=%d\n", arcer.y > 0 ? 1 : 0);          // EXPECT: arc y_positive=1

    // Test 4: Drive a square path
    printf("=== Square Path ===\n");                              // EXPECT: === Square Path ===
    struct RobotState sq;
    sq.x = 0;
    sq.y = 0;
    sq.heading = 0;

    struct PathPoint path[5];
    path[0].x = sq.x;
    path[0].y = sq.y;
    path[0].heading = sq.heading;

    int side;
    for (side = 0; side < 4; side++) {
        // Drive forward 1 revolution
        sq = update_odometry(sq, &cfg, 100, 100);

        // Turn ~90 degrees: need delta = 90 degrees
        // delta_heading = (right-left)*57*circ / (ticks_per_rev * wheel_base * 1000) degrees
        // We need ~90 degrees: experimentally calibrate
        // (right-left) * 314 * 57 / (100 * 200) = 90 -> right-left ~ 503/17.898 ~ 50
        int turn_steps;
        for (turn_steps = 0; turn_steps < 5; turn_steps++) {
            sq = update_odometry(sq, &cfg, -10, 10);
        }
        path[side + 1].x = sq.x;
        path[side + 1].y = sq.y;
        path[side + 1].heading = sq.heading;
    }

    printf("square end heading=%d\n", sq.heading);                // EXPECT: square end heading=340
    printf("square traveled=%d sides\n", 4);                      // EXPECT: square traveled=4 sides

    // Test 5: Encoder accumulation test
    printf("=== Encoder Accumulation ===\n");                     // EXPECT: === Encoder Accumulation ===
    struct RobotState accum;
    accum.x = 0;
    accum.y = 0;
    accum.heading = 0;

    int total_left = 0;
    int total_right = 0;
    int step;
    for (step = 0; step < 10; step++) {
        accum = update_odometry(accum, &cfg, 10, 10);
        total_left = total_left + 10;
        total_right = total_right + 10;
    }
    printf("total_ticks: L=%d R=%d\n", total_left, total_right);  // EXPECT: total_ticks: L=100 R=100

    // Should be same as doing 100 ticks at once
    struct RobotState once;
    once.x = 0;
    once.y = 0;
    once.heading = 0;
    once = update_odometry(once, &cfg, 100, 100);
    printf("incremental x=%d\n", accum.x);                       // EXPECT: incremental x=310
    printf("single x=%d\n", once.x);                              // EXPECT: single x=314
    printf("match=%d\n", accum.x == once.x ? 1 : 0);             // EXPECT: match=0

    // Test 6: Heading wrapping
    printf("=== Heading Wrap ===\n");                             // EXPECT: === Heading Wrap ===
    struct RobotState wrap;
    wrap.x = 0;
    wrap.y = 0;
    wrap.heading = 350;
    wrap = update_odometry(wrap, &cfg, -10, 10);
    printf("wrapped heading=%d\n", wrap.heading);                 // EXPECT: wrapped heading=7
    int heading_valid = (wrap.heading >= 0 && wrap.heading < 360) ? 1 : 0;
    printf("heading_valid=%d\n", heading_valid);                  // EXPECT: heading_valid=1

    // Test 7: Distance calculation
    printf("=== Distance ===\n");                                 // EXPECT: === Distance ===
    printf("dist(0,0,3,4)=%d\n", distance(0, 0, 3, 4));          // EXPECT: dist(0,0,3,4)=5
    printf("dist(0,0,0,0)=%d\n", distance(0, 0, 0, 0));          // EXPECT: dist(0,0,0,0)=0
    printf("dist(0,0,100,0)=%d\n", distance(0, 0, 100, 0));      // EXPECT: dist(0,0,100,0)=100

    printf("PASS\n");                                             // EXPECT: PASS
    return 0;
}
