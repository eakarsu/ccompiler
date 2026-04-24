int printf(const char *fmt, ...);

// Obstacle Avoidance Simulation
// Robot navigates a grid avoiding obstacles using simple reactive control

struct Robot {
    int x;
    int y;
    int dir;       // 0=north,1=east,2=south,3=west
    int steps;
    int collisions;
};

int arena[10][10];

void arena_init() {
    int i;
    int j;
    for (i = 0; i < 10; i = i + 1) {
        for (j = 0; j < 10; j = j + 1) {
            arena[i][j] = 0;
        }
    }
}

void arena_add_wall(int x, int y) {
    if (x >= 0 && x < 10 && y >= 0 && y < 10) {
        arena[x][y] = 1;
    }
}

int arena_blocked(int x, int y) {
    if (x < 0 || x >= 10 || y < 0 || y >= 10) return 1;
    return arena[x][y];
}

void robot_init(struct Robot *r, int x, int y, int dir) {
    r->x = x;
    r->y = y;
    r->dir = dir;
    r->steps = 0;
    r->collisions = 0;
}

int dir_dx(int d) {
    if (d == 0) return -1;
    if (d == 2) return 1;
    return 0;
}

int dir_dy(int d) {
    if (d == 1) return 1;
    if (d == 3) return -1;
    return 0;
}

char *dir_name(int d) {
    if (d == 0) return "N";
    if (d == 1) return "E";
    if (d == 2) return "S";
    return "W";
}

void robot_turn_right(struct Robot *r) {
    r->dir = (r->dir + 1) % 4;
}

void robot_turn_left(struct Robot *r) {
    r->dir = (r->dir + 3) % 4;
}

int robot_sensor_front(struct Robot *r) {
    int nx;
    int ny;
    nx = r->x + dir_dx(r->dir);
    ny = r->y + dir_dy(r->dir);
    return arena_blocked(nx, ny);
}

int robot_sensor_right(struct Robot *r) {
    int rd;
    int nx;
    int ny;
    rd = (r->dir + 1) % 4;
    nx = r->x + dir_dx(rd);
    ny = r->y + dir_dy(rd);
    return arena_blocked(nx, ny);
}

int robot_sensor_left(struct Robot *r) {
    int ld;
    int nx;
    int ny;
    ld = (r->dir + 3) % 4;
    nx = r->x + dir_dx(ld);
    ny = r->y + dir_dy(ld);
    return arena_blocked(nx, ny);
}

int robot_move(struct Robot *r) {
    int nx;
    int ny;
    nx = r->x + dir_dx(r->dir);
    ny = r->y + dir_dy(r->dir);
    if (arena_blocked(nx, ny)) {
        r->collisions = r->collisions + 1;
        return 0;
    }
    r->x = nx;
    r->y = ny;
    r->steps = r->steps + 1;
    return 1;
}

// Wall-following algorithm: keep wall on right side
void robot_step_wall_follow(struct Robot *r) {
    int rf;
    int ff;

    rf = robot_sensor_right(r);
    ff = robot_sensor_front(r);

    if (!rf) {
        // No wall on right, turn right and move
        robot_turn_right(r);
        robot_move(r);
    } else if (!ff) {
        // Wall on right, clear ahead: go forward
        robot_move(r);
    } else {
        // Blocked ahead and right, turn left
        robot_turn_left(r);
    }
}

int manhattan_dist(int x1, int y1, int x2, int y2) {
    int dx;
    int dy;
    dx = x1 - x2;
    if (dx < 0) dx = -dx;
    dy = y1 - y2;
    if (dy < 0) dy = -dy;
    return dx + dy;
}

int main() {
    struct Robot bot;
    int i;
    int dist;

    printf("=== Obstacle Avoidance Test ===\n");
    // EXPECT: === Obstacle Avoidance Test ===

    // Test 1: Open field movement
    arena_init();
    robot_init(&bot, 5, 5, 0);
    robot_move(&bot);
    robot_move(&bot);
    robot_move(&bot);
    printf("Open move: pos=(%d,%d), steps=%d\n", bot.x, bot.y, bot.steps);
    // EXPECT: Open move: pos=(2,5), steps=3

    // Test 2: Wall collision
    arena_init();
    arena_add_wall(3, 5);
    robot_init(&bot, 5, 5, 0);
    robot_move(&bot);
    i = robot_move(&bot);
    printf("Wall hit: moved=%d, pos=(%d,%d), collisions=%d\n", i, bot.x, bot.y, bot.collisions);
    // EXPECT: Wall hit: moved=0, pos=(4,5), collisions=1

    // Test 3: Sensor readings
    arena_init();
    arena_add_wall(4, 5);
    robot_init(&bot, 5, 5, 0);
    printf("Sensors: front=%d, right=%d, left=%d\n",
           robot_sensor_front(&bot), robot_sensor_right(&bot), robot_sensor_left(&bot));
    // EXPECT: Sensors: front=1, right=0, left=0

    // Test 4: Turning
    robot_init(&bot, 5, 5, 0);
    printf("Start dir: %s\n", dir_name(bot.dir));
    // EXPECT: Start dir: N
    robot_turn_right(&bot);
    printf("After right: %s\n", dir_name(bot.dir));
    // EXPECT: After right: E
    robot_turn_right(&bot);
    printf("After right: %s\n", dir_name(bot.dir));
    // EXPECT: After right: S
    robot_turn_left(&bot);
    printf("After left: %s\n", dir_name(bot.dir));
    // EXPECT: After left: E

    // Test 5: Wall following around obstacle
    arena_init();
    arena_add_wall(3, 3);
    arena_add_wall(3, 4);
    arena_add_wall(3, 5);
    arena_add_wall(4, 3);
    arena_add_wall(4, 5);
    arena_add_wall(5, 3);

    robot_init(&bot, 6, 4, 0);
    printf("Wall follow start: (%d,%d) dir=%s\n", bot.x, bot.y, dir_name(bot.dir));
    // EXPECT: Wall follow start: (6,4) dir=N

    for (i = 0; i < 12; i = i + 1) {
        robot_step_wall_follow(&bot);
    }
    printf("After 12 steps: (%d,%d) dir=%s, steps=%d\n", bot.x, bot.y, dir_name(bot.dir), bot.steps);
    // EXPECT: After 12 steps: (6,4) dir=N, steps=12

    // Test 6: Boundary detection
    arena_init();
    robot_init(&bot, 0, 0, 0);
    printf("Boundary N: %d\n", robot_sensor_front(&bot));
    // EXPECT: Boundary N: 1
    robot_init(&bot, 0, 0, 3);
    printf("Boundary W: %d\n", robot_sensor_front(&bot));
    // EXPECT: Boundary W: 1

    // Test 7: Manhattan distance
    dist = manhattan_dist(0, 0, 5, 7);
    printf("Manhattan (0,0)-(5,7): %d\n", dist);
    // EXPECT: Manhattan (0,0)-(5,7): 12

    printf("Obstacle avoidance done\n");
    // EXPECT: Obstacle avoidance done

    return 0;
}
