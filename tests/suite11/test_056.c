int printf(const char *fmt, ...);

// Bouncing ball simulation with multiple balls in a box
// Coordinates in integer pixels, velocities in pixels per step

struct Ball {
    int x;
    int y;
    int vx;
    int vy;
    int radius;
    int id;
};

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

void init_ball(struct Ball *b, int id, int x, int y, int vx, int vy, int r) {
    b->id = id;
    b->x = x;
    b->y = y;
    b->vx = vx;
    b->vy = vy;
    b->radius = r;
}

void bounce_walls(struct Ball *b, int width, int height) {
    if (b->x - b->radius < 0) {
        b->x = b->radius;
        b->vx = -b->vx;
    }
    if (b->x + b->radius > width) {
        b->x = width - b->radius;
        b->vx = -b->vx;
    }
    if (b->y - b->radius < 0) {
        b->y = b->radius;
        b->vy = -b->vy;
    }
    if (b->y + b->radius > height) {
        b->y = height - b->radius;
        b->vy = -b->vy;
    }
}

void apply_gravity_ball(struct Ball *b, int g) {
    b->vy = b->vy + g;
}

void move_ball(struct Ball *b) {
    b->x = b->x + b->vx;
    b->y = b->y + b->vy;
}

int balls_collide(struct Ball *a, struct Ball *b) {
    int dx = a->x - b->x;
    int dy = a->y - b->y;
    int dist2 = dx * dx + dy * dy;
    int min_dist = a->radius + b->radius;
    return dist2 <= min_dist * min_dist;
}

void resolve_ball_collision(struct Ball *a, struct Ball *b) {
    int tmp;
    tmp = a->vx; a->vx = b->vx; b->vx = tmp;
    tmp = a->vy; a->vy = b->vy; b->vy = tmp;
}

int count_wall_contacts(struct Ball *b, int width, int height) {
    int count = 0;
    if (b->x - b->radius <= 0 || b->x + b->radius >= width) count = count + 1;
    if (b->y - b->radius <= 0 || b->y + b->radius >= height) count = count + 1;
    return count;
}

int main() {
    struct Ball balls[4];
    int i;
    int j;
    int step;
    int width;
    int height;

    width = 200;
    height = 150;

    printf("=== Bouncing Ball Simulation ===\n");
    // EXPECT: === Bouncing Ball Simulation ===

    // Test 1: Single ball linear motion (no gravity, no walls hit)
    init_ball(&balls[0], 0, 50, 50, 7, 3, 5);
    printf("Ball at (%d,%d) vel=(%d,%d)\n", balls[0].x, balls[0].y, balls[0].vx, balls[0].vy);
    // EXPECT: Ball at (50,50) vel=(7,3)

    for (step = 0; step < 10; step = step + 1) {
        move_ball(&balls[0]);
        bounce_walls(&balls[0], width, height);
        printf("Step %d: (%d,%d)\n", step, balls[0].x, balls[0].y);
    }
    // EXPECT: Step 0: (57,53)
    // EXPECT: Step 1: (64,56)
    // EXPECT: Step 2: (71,59)
    // EXPECT: Step 3: (78,62)
    // EXPECT: Step 4: (85,65)
    // EXPECT: Step 5: (92,68)
    // EXPECT: Step 6: (99,71)
    // EXPECT: Step 7: (106,74)
    // EXPECT: Step 8: (113,77)
    // EXPECT: Step 9: (120,80)

    // Test 2: Ball with gravity falling and bouncing off floor
    printf("--- Gravity Drop ---\n");
    // EXPECT: --- Gravity Drop ---
    init_ball(&balls[0], 0, 100, 10, 3, 0, 5);
    for (step = 0; step < 10; step = step + 1) {
        apply_gravity_ball(&balls[0], 2);
        move_ball(&balls[0]);
        bounce_walls(&balls[0], width, height);
        printf("y=%d vy=%d\n", balls[0].y, balls[0].vy);
    }
    // EXPECT: y=12 vy=2
    // EXPECT: y=16 vy=4
    // EXPECT: y=22 vy=6
    // EXPECT: y=30 vy=8
    // EXPECT: y=40 vy=10
    // EXPECT: y=52 vy=12
    // EXPECT: y=66 vy=14
    // EXPECT: y=82 vy=16
    // EXPECT: y=100 vy=18
    // EXPECT: y=120 vy=20

    // Test 3: Ball bouncing off right wall
    printf("--- Wall Bounce ---\n");
    // EXPECT: --- Wall Bounce ---
    init_ball(&balls[0], 0, 180, 75, 10, 0, 5);
    for (step = 0; step < 6; step = step + 1) {
        move_ball(&balls[0]);
        bounce_walls(&balls[0], width, height);
        printf("x=%d vx=%d\n", balls[0].x, balls[0].vx);
    }
    // EXPECT: x=190 vx=10
    // EXPECT: x=195 vx=-10
    // EXPECT: x=185 vx=-10
    // EXPECT: x=175 vx=-10
    // EXPECT: x=165 vx=-10
    // EXPECT: x=155 vx=-10

    // Test 4: Corner bounce
    printf("--- Corner ---\n");
    // EXPECT: --- Corner ---
    init_ball(&balls[0], 0, 190, 140, 8, 6, 5);
    for (step = 0; step < 4; step = step + 1) {
        move_ball(&balls[0]);
        bounce_walls(&balls[0], width, height);
        printf("pos=(%d,%d) vel=(%d,%d)\n",
               balls[0].x, balls[0].y, balls[0].vx, balls[0].vy);
    }
    // EXPECT: pos=(195,145) vel=(-8,-6)
    // EXPECT: pos=(187,139) vel=(-8,-6)
    // EXPECT: pos=(179,133) vel=(-8,-6)
    // EXPECT: pos=(171,127) vel=(-8,-6)

    // Test 5: Two balls head-on collision
    printf("--- Head On ---\n");
    // EXPECT: --- Head On ---
    init_ball(&balls[0], 0, 40, 50, 5, 0, 8);
    init_ball(&balls[1], 1, 80, 50, -5, 0, 8);

    int collided = 0;
    for (step = 0; step < 8; step = step + 1) {
        move_ball(&balls[0]);
        move_ball(&balls[1]);
        if (!collided && balls_collide(&balls[0], &balls[1])) {
            printf("Collision at step %d: b0=(%d,%d) b1=(%d,%d)\n",
                   step, balls[0].x, balls[0].y, balls[1].x, balls[1].y);
            resolve_ball_collision(&balls[0], &balls[1]);
            collided = 1;
        }
        bounce_walls(&balls[0], width, height);
        bounce_walls(&balls[1], width, height);
    }
    // EXPECT: Collision at step 2: b0=(55,50) b1=(65,50)

    printf("Final: b0=(%d,%d) b1=(%d,%d)\n",
           balls[0].x, balls[0].y, balls[1].x, balls[1].y);
    // EXPECT: Final: b0=(30,50) b1=(90,50)

    // Test 6: Diagonal bouncing in box
    printf("--- Diagonal ---\n");
    // EXPECT: --- Diagonal ---
    init_ball(&balls[0], 0, 100, 75, 15, 12, 5);
    int bounce_count = 0;
    for (step = 0; step < 20; step = step + 1) {
        int ox = balls[0].vx;
        int oy = balls[0].vy;
        move_ball(&balls[0]);
        bounce_walls(&balls[0], width, height);
        if (balls[0].vx != ox || balls[0].vy != oy) {
            bounce_count = bounce_count + 1;
        }
    }
    printf("Bounces in 20 steps: %d\n", bounce_count);
    // EXPECT: Bounces in 20 steps: 4
    printf("Final pos: (%d,%d)\n", balls[0].x, balls[0].y);
    // EXPECT: Final pos: (5,29)

    printf("Bouncing ball simulation done\n");
    // EXPECT: Bouncing ball simulation done

    return 0;
}
