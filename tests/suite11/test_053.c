int printf(const char *fmt, ...);

// AABB Collision Detection System
// All coordinates are integers (pixels or grid units)

struct AABB {
    int x;
    int y;
    int w;
    int h;
};

struct Entity {
    struct AABB box;
    int vx;
    int vy;
    int id;
    int active;
};

void init_aabb(struct AABB *b, int x, int y, int w, int h) {
    b->x = x;
    b->y = y;
    b->w = w;
    b->h = h;
}

void init_entity(struct Entity *e, int id, int x, int y, int w, int h, int vx, int vy) {
    e->id = id;
    e->box.x = x;
    e->box.y = y;
    e->box.w = w;
    e->box.h = h;
    e->vx = vx;
    e->vy = vy;
    e->active = 1;
}

int check_collision(struct AABB *a, struct AABB *b) {
    // AABB overlap test
    if (a->x + a->w <= b->x) return 0;
    if (b->x + b->w <= a->x) return 0;
    if (a->y + a->h <= b->y) return 0;
    if (b->y + b->h <= a->y) return 0;
    return 1;
}

int overlap_area(struct AABB *a, struct AABB *b) {
    int x_left;
    int x_right;
    int y_top;
    int y_bottom;
    int ow;
    int oh;

    x_left = a->x;
    if (b->x > x_left) x_left = b->x;

    x_right = a->x + a->w;
    if (b->x + b->w < x_right) x_right = b->x + b->w;

    y_top = a->y;
    if (b->y > y_top) y_top = b->y;

    y_bottom = a->y + a->h;
    if (b->y + b->h < y_bottom) y_bottom = b->y + b->h;

    ow = x_right - x_left;
    oh = y_bottom - y_top;

    if (ow <= 0 || oh <= 0) return 0;
    return ow * oh;
}

int point_in_aabb(struct AABB *b, int px, int py) {
    if (px >= b->x && px < b->x + b->w && py >= b->y && py < b->y + b->h)
        return 1;
    return 0;
}

void move_entity(struct Entity *e) {
    e->box.x = e->box.x + e->vx;
    e->box.y = e->box.y + e->vy;
}

void resolve_collision(struct Entity *a, struct Entity *b) {
    // Simple elastic: swap velocities
    int tmp;
    tmp = a->vx; a->vx = b->vx; b->vx = tmp;
    tmp = a->vy; a->vy = b->vy; b->vy = tmp;
}

int main() {
    struct AABB a;
    struct AABB b;
    struct AABB c;
    int i;
    int j;
    int col;
    int area;

    printf("=== AABB Collision Detection ===\n");
    // EXPECT: === AABB Collision Detection ===

    // Test 1: Basic overlap test
    init_aabb(&a, 0, 0, 10, 10);
    init_aabb(&b, 5, 5, 10, 10);
    col = check_collision(&a, &b);
    printf("Overlap (0,0,10,10) vs (5,5,10,10): %d\n", col);
    // EXPECT: Overlap (0,0,10,10) vs (5,5,10,10): 1

    init_aabb(&c, 20, 20, 5, 5);
    col = check_collision(&a, &c);
    printf("Overlap (0,0,10,10) vs (20,20,5,5): %d\n", col);
    // EXPECT: Overlap (0,0,10,10) vs (20,20,5,5): 0

    // Test 2: Edge touching (no overlap)
    init_aabb(&b, 10, 0, 10, 10);
    col = check_collision(&a, &b);
    printf("Edge touch: %d\n", col);
    // EXPECT: Edge touch: 0

    // Test 3: Overlap area calculation
    init_aabb(&a, 0, 0, 10, 10);
    init_aabb(&b, 5, 5, 10, 10);
    area = overlap_area(&a, &b);
    printf("Overlap area: %d\n", area);
    // EXPECT: Overlap area: 25

    init_aabb(&b, 3, 3, 4, 4);
    area = overlap_area(&a, &b);
    printf("Contained overlap: %d\n", area);
    // EXPECT: Contained overlap: 16

    // Test 4: Point in AABB
    init_aabb(&a, 10, 10, 20, 20);
    printf("Point (15,15) in box: %d\n", point_in_aabb(&a, 15, 15));
    // EXPECT: Point (15,15) in box: 1
    printf("Point (5,5) in box: %d\n", point_in_aabb(&a, 5, 5));
    // EXPECT: Point (5,5) in box: 0
    printf("Point (10,10) in box: %d\n", point_in_aabb(&a, 10, 10));
    // EXPECT: Point (10,10) in box: 1
    printf("Point (30,30) in box: %d\n", point_in_aabb(&a, 30, 30));
    // EXPECT: Point (30,30) in box: 0

    // Test 5: Moving entities with collision detection
    printf("--- Entity Simulation ---\n");
    // EXPECT: --- Entity Simulation ---

    struct Entity entities[4];
    init_entity(&entities[0], 0, 0, 0, 4, 4, 3, 1);
    init_entity(&entities[1], 1, 30, 0, 4, 4, -3, 1);
    init_entity(&entities[2], 2, 0, 30, 4, 4, 3, -3);
    init_entity(&entities[3], 3, 30, 30, 4, 4, -3, -3);

    int step;
    int collision_count = 0;
    for (step = 0; step < 10; step = step + 1) {
        // Move all entities
        for (i = 0; i < 4; i = i + 1) {
            move_entity(&entities[i]);
        }

        // Check all pairs
        for (i = 0; i < 4; i = i + 1) {
            for (j = i + 1; j < 4; j = j + 1) {
                if (check_collision(&entities[i].box, &entities[j].box)) {
                    collision_count = collision_count + 1;
                    printf("Step %d: collision %d-%d\n", step, entities[i].id, entities[j].id);
                    resolve_collision(&entities[i], &entities[j]);
                }
            }
        }
    }
    // EXPECT: Step 4: collision 0-1
    // EXPECT: Step 4: collision 2-3
    // EXPECT: Step 6: collision 0-2
    // EXPECT: Step 6: collision 1-3

    printf("Total collisions: %d\n", collision_count);
    // EXPECT: Total collisions: 4
    printf("Final positions:\n");
    // EXPECT: Final positions:
    for (i = 0; i < 4; i = i + 1) {
        printf("  Entity %d: (%d,%d) vel=(%d,%d)\n", entities[i].id,
               entities[i].box.x, entities[i].box.y,
               entities[i].vx, entities[i].vy);
    }
    // EXPECT:   Entity 0: (0,-2) vel=(-3,-3)
    // EXPECT:   Entity 1: (30,-2) vel=(3,-3)
    // EXPECT:   Entity 2: (0,12) vel=(-3,1)
    // EXPECT:   Entity 3: (30,12) vel=(3,1)

    // Test 6: Sweep test - two entities approaching each other
    printf("--- Sweep Test ---\n");
    // EXPECT: --- Sweep Test ---
    struct Entity ea;
    struct Entity eb;
    init_entity(&ea, 10, 0, 0, 5, 5, 2, 0);
    init_entity(&eb, 11, 30, 0, 5, 5, -2, 0);

    int collision_step = -1;
    for (step = 0; step < 20; step = step + 1) {
        move_entity(&ea);
        move_entity(&eb);
        if (check_collision(&ea.box, &eb.box)) {
            collision_step = step;
            break;
        }
    }
    printf("Sweep collision at step: %d\n", collision_step);
    // EXPECT: Sweep collision at step: 6
    printf("EA=(%d,%d) EB=(%d,%d)\n", ea.box.x, ea.box.y, eb.box.x, eb.box.y);
    // EXPECT: EA=(14,0) EB=(16,0)

    // Test 7: Containment test
    printf("--- Containment ---\n");
    // EXPECT: --- Containment ---
    init_aabb(&a, 0, 0, 100, 100);
    init_aabb(&b, 10, 10, 20, 20);
    col = check_collision(&a, &b);
    area = overlap_area(&a, &b);
    printf("Large contains small: col=%d area=%d\n", col, area);
    // EXPECT: Large contains small: col=1 area=400

    printf("Collision detection complete\n");
    // EXPECT: Collision detection complete

    return 0;
}
