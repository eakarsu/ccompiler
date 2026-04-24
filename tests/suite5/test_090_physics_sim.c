int printf(const char *fmt, ...);

struct Particle {
    int x;
    int y;
    int vx;
    int vy;
    int ax;
    int ay;
    int mass;
    int id;
};

int init_particle(struct Particle *p, int id, int x, int y, int vx, int vy, int ax, int ay, int mass) {
    p->id = id;
    p->x = x;
    p->y = y;
    p->vx = vx;
    p->vy = vy;
    p->ax = ax;
    p->ay = ay;
    p->mass = mass;
    return 0;
}

int step(struct Particle *p) {
    /* Update velocity from acceleration */
    p->vx = p->vx + p->ax;
    p->vy = p->vy + p->ay;
    /* Update position from velocity */
    p->x = p->x + p->vx;
    p->y = p->y + p->vy;
    return 0;
}

int step_n(struct Particle *p, int n) {
    int i;
    i = 0;
    while (i < n) {
        step(p);
        i = i + 1;
    }
    return 0;
}

int abs_val(int v) {
    if (v < 0) {
        return 0 - v;
    }
    return v;
}

int distance_sq(struct Particle *a, struct Particle *b) {
    int dx;
    int dy;
    dx = a->x - b->x;
    dy = a->y - b->y;
    return dx * dx + dy * dy;
}

int manhattan_dist(struct Particle *a, struct Particle *b) {
    return abs_val(a->x - b->x) + abs_val(a->y - b->y);
}

int check_collision(struct Particle *a, struct Particle *b, int radius) {
    int dist2;
    int min_dist;
    dist2 = distance_sq(a, b);
    min_dist = radius * 2;
    if (dist2 <= min_dist * min_dist) {
        return 1;
    }
    return 0;
}

int bounce_walls(struct Particle *p, int min_x, int min_y, int max_x, int max_y) {
    int bounced;
    bounced = 0;
    if (p->x <= min_x) {
        p->x = min_x;
        p->vx = abs_val(p->vx);
        bounced = 1;
    }
    if (p->x >= max_x) {
        p->x = max_x;
        p->vx = 0 - abs_val(p->vx);
        bounced = 1;
    }
    if (p->y <= min_y) {
        p->y = min_y;
        p->vy = abs_val(p->vy);
        bounced = 1;
    }
    if (p->y >= max_y) {
        p->y = max_y;
        p->vy = 0 - abs_val(p->vy);
        bounced = 1;
    }
    return bounced;
}

int kinetic_energy(struct Particle *p) {
    /* KE = 0.5 * m * v^2, use integer approx: m * (vx^2 + vy^2) / 2 */
    return p->mass * (p->vx * p->vx + p->vy * p->vy) / 2;
}

int momentum_x(struct Particle *p) {
    return p->mass * p->vx;
}

int momentum_y(struct Particle *p) {
    return p->mass * p->vy;
}

int total_momentum_x(struct Particle *parts, int n) {
    int i;
    int total;
    total = 0;
    i = 0;
    while (i < n) {
        total = total + momentum_x(&parts[i]);
        i = i + 1;
    }
    return total;
}

int total_momentum_y(struct Particle *parts, int n) {
    int i;
    int total;
    total = 0;
    i = 0;
    while (i < n) {
        total = total + momentum_y(&parts[i]);
        i = i + 1;
    }
    return total;
}

int total_kinetic_energy(struct Particle *parts, int n) {
    int i;
    int total;
    total = 0;
    i = 0;
    while (i < n) {
        total = total + kinetic_energy(&parts[i]);
        i = i + 1;
    }
    return total;
}

int elastic_collision_1d(struct Particle *a, struct Particle *b) {
    /* 1D elastic collision: swap velocities for equal mass */
    int tmp_vx;
    int tmp_vy;
    if (a->mass == b->mass) {
        tmp_vx = a->vx;
        tmp_vy = a->vy;
        a->vx = b->vx;
        a->vy = b->vy;
        b->vx = tmp_vx;
        b->vy = tmp_vy;
    }
    return 0;
}

int count_collisions(struct Particle *parts, int n, int radius) {
    int i;
    int j;
    int count;
    count = 0;
    i = 0;
    while (i < n) {
        j = i + 1;
        while (j < n) {
            if (check_collision(&parts[i], &parts[j], radius)) {
                count = count + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return count;
}

int simulate_with_walls(struct Particle *parts, int n, int steps, int min_x, int min_y, int max_x, int max_y) {
    int s;
    int i;
    int bounces;
    bounces = 0;
    s = 0;
    while (s < steps) {
        i = 0;
        while (i < n) {
            step(&parts[i]);
            bounces = bounces + bounce_walls(&parts[i], min_x, min_y, max_x, max_y);
            i = i + 1;
        }
        s = s + 1;
    }
    return bounces;
}

int main(void) {
    struct Particle parts[8];
    int result;

    /* ===== Test 1: Basic motion ===== */
    init_particle(&parts[0], 1, 0, 0, 3, 4, 0, 0, 10);

    step(&parts[0]);

    // EXPECT: 3
    printf("%d\n", parts[0].x);

    // EXPECT: 4
    printf("%d\n", parts[0].y);

    step(&parts[0]);

    // EXPECT: 6
    printf("%d\n", parts[0].x);

    // EXPECT: 8
    printf("%d\n", parts[0].y);

    /* ===== Test 2: Acceleration ===== */
    init_particle(&parts[0], 2, 0, 0, 0, 0, 2, 3, 5);

    step(&parts[0]);
    /* v=(2,3), pos=(2,3) */

    // EXPECT: 2
    printf("%d\n", parts[0].x);

    // EXPECT: 3
    printf("%d\n", parts[0].y);

    step(&parts[0]);
    /* v=(4,6), pos=(6,9) */

    // EXPECT: 6
    printf("%d\n", parts[0].x);

    // EXPECT: 9
    printf("%d\n", parts[0].y);

    step(&parts[0]);
    /* v=(6,9), pos=(12,18) */

    // EXPECT: 12
    printf("%d\n", parts[0].x);

    // EXPECT: 18
    printf("%d\n", parts[0].y);

    /* ===== Test 3: Distance and collision ===== */
    init_particle(&parts[0], 3, 0, 0, 0, 0, 0, 0, 1);
    init_particle(&parts[1], 4, 3, 4, 0, 0, 0, 0, 1);

    /* distance_sq = 9 + 16 = 25 */
    // EXPECT: 25
    printf("%d\n", distance_sq(&parts[0], &parts[1]));

    // EXPECT: 7
    printf("%d\n", manhattan_dist(&parts[0], &parts[1]));

    /* radius=3: min_dist=6, 6^2=36 > 25, so collision */
    // EXPECT: 1
    printf("%d\n", check_collision(&parts[0], &parts[1], 3));

    /* radius=2: min_dist=4, 4^2=16 < 25, no collision */
    // EXPECT: 0
    printf("%d\n", check_collision(&parts[0], &parts[1], 2));

    /* ===== Test 4: Wall bouncing ===== */
    init_particle(&parts[0], 5, 95, 50, 10, 0, 0, 0, 1);

    /* Step: pos=(105,50), should bounce off max_x=100 */
    step(&parts[0]);
    result = bounce_walls(&parts[0], 0, 0, 100, 100);

    // EXPECT: 1
    printf("%d\n", result);

    // EXPECT: 100
    printf("%d\n", parts[0].x);

    /* vx should be negative now */
    // EXPECT: -10
    printf("%d\n", parts[0].vx);

    /* ===== Test 5: Kinetic energy and momentum ===== */
    init_particle(&parts[0], 6, 0, 0, 3, 4, 0, 0, 10);
    init_particle(&parts[1], 7, 0, 0, -2, 1, 0, 0, 5);

    /* KE of part 0: 10 * (9 + 16) / 2 = 125 */
    // EXPECT: 125
    printf("%d\n", kinetic_energy(&parts[0]));

    /* KE of part 1: 5 * (4 + 1) / 2 = 12 */
    // EXPECT: 12
    printf("%d\n", kinetic_energy(&parts[1]));

    /* Total momentum X: 10*3 + 5*(-2) = 20 */
    // EXPECT: 20
    printf("%d\n", total_momentum_x(parts, 2));

    /* Total momentum Y: 10*4 + 5*1 = 45 */
    // EXPECT: 45
    printf("%d\n", total_momentum_y(parts, 2));

    /* ===== Test 6: Elastic collision (equal mass) ===== */
    init_particle(&parts[0], 8, 5, 5, 4, 0, 0, 0, 10);
    init_particle(&parts[1], 9, 5, 5, -3, 0, 0, 0, 10);

    elastic_collision_1d(&parts[0], &parts[1]);

    /* Velocities swapped */
    // EXPECT: -3
    printf("%d\n", parts[0].vx);

    // EXPECT: 4
    printf("%d\n", parts[1].vx);

    /* ===== Test 7: Multiple particles with wall simulation ===== */
    init_particle(&parts[0], 10, 10, 10, 5, 5, 0, 0, 1);
    init_particle(&parts[1], 11, 90, 90, -5, -5, 0, 0, 1);

    /* Simulate 25 steps in 100x100 box */
    result = simulate_with_walls(parts, 2, 25, 0, 0, 100, 100);

    /* Some bounces should have occurred */
    // EXPECT: 1
    printf("%d\n", result > 0 ? 1 : 0);

    /* ===== Test 8: step_n ===== */
    init_particle(&parts[0], 12, 0, 0, 1, 2, 0, 0, 1);

    step_n(&parts[0], 5);

    // EXPECT: 5
    printf("%d\n", parts[0].x);

    // EXPECT: 10
    printf("%d\n", parts[0].y);

    /* ===== Test 9: Multiple collision count ===== */
    init_particle(&parts[0], 13, 0, 0, 0, 0, 0, 0, 1);
    init_particle(&parts[1], 14, 1, 1, 0, 0, 0, 0, 1);
    init_particle(&parts[2], 15, 100, 100, 0, 0, 0, 0, 1);
    init_particle(&parts[3], 16, 101, 101, 0, 0, 0, 0, 1);

    /* With radius 5: (0,1) collide, (2,3) collide, others too far apart */
    /* dist_sq(0,1) = 2, min=10, 10^2=100 > 2, collide */
    /* dist_sq(0,2) = 10000+10000=20000, 100 < 20000, no */
    /* dist_sq(0,3) = 101^2+101^2=20402, no */
    /* dist_sq(1,2) = 99^2+99^2=19602, no */
    /* dist_sq(1,3) = 100^2+100^2=20000, no */
    /* dist_sq(2,3) = 1+1=2, 100 > 2, collide */
    // EXPECT: 2
    printf("%d\n", count_collisions(parts, 4, 5));

    /* ===== Test 10: Gravity simulation ===== */
    init_particle(&parts[0], 17, 0, 100, 5, 0, 0, -2, 1);

    step_n(&parts[0], 10);

    /* After 10 steps with gravity=-2:
       vx stays 5, x = 0 + 5*10 = 50
       vy: 0, -2, -4, -6, -8, -10, -12, -14, -16, -18 (after each acc)
       y: 100, 100-2=98, 98-4=94, 94-6=88, 88-8=80, 80-10=70, 70-12=58, 58-14=44, 44-16=28, 28-18=10 */
    // EXPECT: 50
    printf("%d\n", parts[0].x);

    // EXPECT: -10
    printf("%d\n", parts[0].y);

    // EXPECT: -20
    printf("%d\n", parts[0].vy);

    return 0;
}
