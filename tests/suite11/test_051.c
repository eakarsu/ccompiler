int printf(const char *fmt, ...);

// Projectile motion using scaled integers (scale factor = 1000)
// Physics: x = v0x * t, y = v0y * t - g*t*t/2
// All values multiplied by 1000 for fixed-point arithmetic

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

struct Vec2 {
    int x;
    int y;
};

struct Projectile {
    struct Vec2 pos;
    struct Vec2 vel;
    int time_ms;
    int active;
};

void init_projectile(struct Projectile *p, int vx, int vy) {
    p->pos.x = 0;
    p->pos.y = 0;
    p->vel.x = vx;
    p->vel.y = vy;
    p->time_ms = 0;
    p->active = 1;
}

// Update position using scaled integer math
// dt = 100ms = 0.1s, scale=1000, g=9800 (9.8 * 1000)
void step_projectile(struct Projectile *p) {
    int dt = 100; // 100 ms
    int g = 9800; // gravity * 1000

    // x += vx * dt / 1000
    p->pos.x = p->pos.x + p->vel.x * dt / 1000;

    // vy -= g * dt / 1000
    p->vel.y = p->vel.y - g * dt / 1000;

    // y += vy * dt / 1000
    p->pos.y = p->pos.y + p->vel.y * dt / 1000;

    p->time_ms = p->time_ms + dt;

    if (p->pos.y < 0) {
        p->pos.y = 0;
        p->active = 0;
    }
}

int compute_range(int vx, int vy) {
    // Range = 2 * vx * vy / g (all scaled by 1000)
    // R = 2 * vx * vy / 9800
    int range = 2 * vx * vy / 9800;
    return range;
}

void simulate_trajectory(struct Projectile *p, int max_steps) {
    int step;
    for (step = 0; step < max_steps; step = step + 1) {
        if (!p->active) break;
        step_projectile(p);
    }
}

int find_max_height(int vy) {
    // max height = vy^2 / (2*g) = vy*vy / (2*9800)
    int h = (vy / 10) * (vy / 10) / (2 * 98);
    return h;
}

int main() {
    struct Projectile p;
    int i;
    int max_h;
    int range;

    // Test 1: Simple projectile (vx=10000, vy=10000) => 10 m/s at 45 degrees
    init_projectile(&p, 10000, 10000);
    printf("=== Projectile Motion Simulator ===\n");
    // EXPECT: === Projectile Motion Simulator ===

    printf("Initial vel: vx=%d vy=%d\n", p.vel.x, p.vel.y);
    // EXPECT: Initial vel: vx=10000 vy=10000

    // Simulate and print first 5 steps
    for (i = 0; i < 5; i = i + 1) {
        step_projectile(&p);
        printf("t=%d pos=(%d,%d) vel=(%d,%d)\n",
               p.time_ms, p.pos.x, p.pos.y, p.vel.x, p.vel.y);
    }
    // EXPECT: t=100 pos=(1000,902) vel=(10000,9020)
    // EXPECT: t=200 pos=(2000,1706) vel=(10000,8040)
    // EXPECT: t=300 pos=(3000,2412) vel=(10000,7060)
    // EXPECT: t=400 pos=(4000,3020) vel=(10000,6080)
    // EXPECT: t=500 pos=(5000,3530) vel=(10000,5100)

    // Continue until landing
    simulate_trajectory(&p, 100);
    printf("Landed at x=%d after %d ms\n", p.pos.x, p.time_ms);
    // EXPECT: Landed at x=20000 after 2000 ms

    // Test 2: Higher angle (vx=5000, vy=15000)
    init_projectile(&p, 5000, 15000);
    simulate_trajectory(&p, 100);
    printf("High angle landed at x=%d after %d ms\n", p.pos.x, p.time_ms);
    // EXPECT: High angle landed at x=15000 after 3000 ms

    // Test 3: Compute theoretical range
    range = compute_range(10000, 10000);
    printf("Theoretical range (45 deg): %d\n", range);
    // EXPECT: Theoretical range (45 deg): 20408

    range = compute_range(5000, 15000);
    printf("Theoretical range (high): %d\n", range);
    // EXPECT: Theoretical range (high): 15306

    // Test 4: Max height calculation
    max_h = find_max_height(10000);
    printf("Max height (vy=10000): %d\n", max_h);
    // EXPECT: Max height (vy=10000): 5102

    max_h = find_max_height(15000);
    printf("Max height (vy=15000): %d\n", max_h);
    // EXPECT: Max height (vy=15000): 11479

    // Test 5: Multiple projectiles comparison
    printf("--- Angle comparison ---\n");
    // EXPECT: --- Angle comparison ---
    int angles[5];
    int vxs[5];
    int vys[5];
    angles[0] = 15; vxs[0] = 9659; vys[0] = 2588;
    angles[1] = 30; vxs[1] = 8660; vys[1] = 5000;
    angles[2] = 45; vxs[2] = 7071; vys[2] = 7071;
    angles[3] = 60; vxs[3] = 5000; vys[3] = 8660;
    angles[4] = 75; vxs[4] = 2588; vys[4] = 9659;

    for (i = 0; i < 5; i = i + 1) {
        range = compute_range(vxs[i], vys[i]);
        printf("Angle %d: range=%d\n", angles[i], range);
    }
    // EXPECT: Angle 15: range=5101
    // EXPECT: Angle 30: range=8836
    // EXPECT: Angle 45: range=10203
    // EXPECT: Angle 60: range=8836
    // EXPECT: Angle 75: range=5101

    printf("Max range at 45 degrees as expected\n");
    // EXPECT: Max range at 45 degrees as expected

    return 0;
}
