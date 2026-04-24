int printf(const char *fmt, ...);

struct Vec2 {
    int x;
    int y;
};

struct Particle {
    struct Vec2 pos;
    struct Vec2 vel;
    int mass;
    int active;
};

void step_particle(struct Particle *p) {
    if (p->active) {
        p->pos.x = p->pos.x + p->vel.x;
        p->pos.y = p->pos.y + p->vel.y;
    }
}

void apply_gravity(struct Particle *p, int g) {
    if (p->active) {
        p->vel.y = p->vel.y + g;
    }
}

void apply_friction(struct Particle *p, int factor) {
    /* integer friction: reduce vel toward zero */
    if (p->active) {
        if (p->vel.x > 0) {
            p->vel.x = p->vel.x - factor;
            if (p->vel.x < 0) p->vel.x = 0;
        } else if (p->vel.x < 0) {
            p->vel.x = p->vel.x + factor;
            if (p->vel.x > 0) p->vel.x = 0;
        }
    }
}

void bounce_floor(struct Particle *p, int floor_y) {
    if (p->active && p->pos.y >= floor_y) {
        p->pos.y = floor_y;
        p->vel.y = 0 - (p->vel.y / 2);
        if (p->vel.y > -1 && p->vel.y < 1) {
            p->vel.y = 0;
        }
    }
}

int count_active(struct Particle *parts, int n) {
    int count = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (parts[i].active) {
            count = count + 1;
        }
    }
    return count;
}

int total_kinetic(struct Particle *parts, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (parts[i].active) {
            int vx = parts[i].vel.x;
            int vy = parts[i].vel.y;
            total = total + (vx * vx + vy * vy) * parts[i].mass / 2;
        }
    }
    return total;
}

int main(void) {
    struct Particle parts[4];
    int i;
    int step;

    /* Particle 0: moving right and up */
    parts[0].pos.x = 0;
    parts[0].pos.y = 0;
    parts[0].vel.x = 3;
    parts[0].vel.y = -5;
    parts[0].mass = 2;
    parts[0].active = 1;

    /* Particle 1: moving left */
    parts[1].pos.x = 100;
    parts[1].pos.y = 50;
    parts[1].vel.x = -4;
    parts[1].vel.y = 0;
    parts[1].mass = 1;
    parts[1].active = 1;

    /* Particle 2: stationary */
    parts[2].pos.x = 50;
    parts[2].pos.y = 50;
    parts[2].vel.x = 0;
    parts[2].vel.y = 0;
    parts[2].mass = 3;
    parts[2].active = 1;

    /* Particle 3: inactive */
    parts[3].pos.x = 0;
    parts[3].pos.y = 0;
    parts[3].vel.x = 10;
    parts[3].vel.y = 10;
    parts[3].mass = 1;
    parts[3].active = 0;

    // EXPECT: Active: 3
    printf("Active: %d\n", count_active(parts, 4));

    // EXPECT: P0 pos: 0 0
    printf("P0 pos: %d %d\n", parts[0].pos.x, parts[0].pos.y);

    /* Step 1 */
    for (i = 0; i < 4; i = i + 1) {
        step_particle(&parts[i]);
    }
    // EXPECT: P0 step1: 3 -5
    printf("P0 step1: %d %d\n", parts[0].pos.x, parts[0].pos.y);
    // EXPECT: P1 step1: 96 50
    printf("P1 step1: %d %d\n", parts[1].pos.x, parts[1].pos.y);

    /* Apply gravity */
    for (i = 0; i < 4; i = i + 1) {
        apply_gravity(&parts[i], 2);
    }
    // EXPECT: P0 vel after grav: 3 -3
    printf("P0 vel after grav: %d %d\n", parts[0].vel.x, parts[0].vel.y);

    /* Step 2 */
    for (i = 0; i < 4; i = i + 1) {
        step_particle(&parts[i]);
    }
    // EXPECT: P0 step2: 6 -8
    printf("P0 step2: %d %d\n", parts[0].pos.x, parts[0].pos.y);

    /* Apply gravity again */
    for (i = 0; i < 4; i = i + 1) {
        apply_gravity(&parts[i], 2);
    }
    // EXPECT: P0 vel: 3 -1
    printf("P0 vel: %d %d\n", parts[0].vel.x, parts[0].vel.y);

    /* Step 3 */
    for (i = 0; i < 4; i = i + 1) {
        step_particle(&parts[i]);
    }
    // EXPECT: P0 step3: 9 -9
    printf("P0 step3: %d %d\n", parts[0].pos.x, parts[0].pos.y);

    /* Friction test on particle 1 */
    apply_friction(&parts[1], 2);
    // EXPECT: P1 vel after friction: -2 4
    printf("P1 vel after friction: %d %d\n", parts[1].vel.x, parts[1].vel.y);

    apply_friction(&parts[1], 2);
    // EXPECT: P1 vel after friction2: 0 4
    printf("P1 vel after friction2: %d %d\n", parts[1].vel.x, parts[1].vel.y);

    /* Bounce test */
    struct Particle bp;
    bp.pos.x = 10;
    bp.pos.y = 95;
    bp.vel.x = 2;
    bp.vel.y = 10;
    bp.mass = 1;
    bp.active = 1;

    step_particle(&bp);
    // EXPECT: Bounce pre: 12 105
    printf("Bounce pre: %d %d\n", bp.pos.x, bp.pos.y);

    bounce_floor(&bp, 100);
    // EXPECT: Bounce post pos: 12 100
    printf("Bounce post pos: %d %d\n", bp.pos.x, bp.pos.y);
    // EXPECT: Bounce post vel: 2 -5
    printf("Bounce post vel: %d %d\n", bp.vel.x, bp.vel.y);

    /* Kinetic energy test */
    struct Particle ke_parts[2];
    ke_parts[0].pos.x = 0;
    ke_parts[0].pos.y = 0;
    ke_parts[0].vel.x = 4;
    ke_parts[0].vel.y = 3;
    ke_parts[0].mass = 2;
    ke_parts[0].active = 1;
    ke_parts[1].pos.x = 0;
    ke_parts[1].pos.y = 0;
    ke_parts[1].vel.x = 0;
    ke_parts[1].vel.y = 6;
    ke_parts[1].mass = 3;
    ke_parts[1].active = 1;

    // EXPECT: KE total: 79
    printf("KE total: %d\n", total_kinetic(ke_parts, 2));

    /* Inactive particle not affected */
    // EXPECT: P3 unchanged: 0 0
    printf("P3 unchanged: %d %d\n", parts[3].pos.x, parts[3].pos.y);

    return 0;
}
