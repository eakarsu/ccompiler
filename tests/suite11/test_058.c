int printf(const char *fmt, ...);

// Orbital mechanics (simplified, 2D, integer)
// Circular and elliptical orbit approximations

int isqrt(int n) {
    if (n <= 0) return 0;
    int s = 1;
    while (s * s <= n) s = s + 1;
    return s - 1;
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

struct Orbit {
    int x;
    int y;
    int vx;
    int vy;
    int cx;
    int cy;
    int gm;
};

void init_orbit(struct Orbit *o, int x, int y, int vx, int vy, int gm) {
    o->x = x;
    o->y = y;
    o->vx = vx;
    o->vy = vy;
    o->cx = 0;
    o->cy = 0;
    o->gm = gm;
}

void step_orbit(struct Orbit *o) {
    int dx = o->cx - o->x;
    int dy = o->cy - o->y;
    int r2 = dx * dx + dy * dy;
    if (r2 < 100) r2 = 100;
    int r = isqrt(r2);
    if (r < 10) r = 10;

    int accel = o->gm / r2;

    o->vx = o->vx + accel * dx / r;
    o->vy = o->vy + accel * dy / r;

    o->x = o->x + o->vx;
    o->y = o->y + o->vy;
}

int orbital_radius(struct Orbit *o) {
    int dx = o->x - o->cx;
    int dy = o->y - o->cy;
    return isqrt(dx * dx + dy * dy);
}

int orbital_speed(struct Orbit *o) {
    return isqrt(o->vx * o->vx + o->vy * o->vy);
}

int specific_energy(struct Orbit *o) {
    int v2 = o->vx * o->vx + o->vy * o->vy;
    int r = orbital_radius(o);
    if (r < 1) r = 1;
    return v2 / 2 - o->gm / r;
}

int angular_momentum(struct Orbit *o) {
    return o->x * o->vy - o->y * o->vx;
}

int main() {
    struct Orbit orb;
    int step;
    int i;

    printf("=== Orbital Mechanics ===\n");
    // EXPECT: === Orbital Mechanics ===

    // Test 1: Near-circular orbit
    init_orbit(&orb, 100, 0, 0, 10, 10000);
    printf("Start: (%d,%d) v=(%d,%d)\n", orb.x, orb.y, orb.vx, orb.vy);
    // EXPECT: Start: (100,0) v=(0,10)

    int r0 = orbital_radius(&orb);
    printf("Initial radius: %d\n", r0);
    // EXPECT: Initial radius: 100

    int L0 = angular_momentum(&orb);
    printf("Angular momentum: %d\n", L0);
    // EXPECT: Angular momentum: 1000

    for (step = 0; step < 12; step = step + 1) {
        step_orbit(&orb);
        int r = orbital_radius(&orb);
        printf("Step %d: (%d,%d) r=%d\n", step, orb.x, orb.y, r);
    }
    // EXPECT: Step 0: (99,10) r=99
    // EXPECT: Step 1: (97,20) r=99
    // EXPECT: Step 2: (95,30) r=99
    // EXPECT: Step 3: (93,40) r=101
    // EXPECT: Step 4: (91,50) r=103
    // EXPECT: Step 5: (89,60) r=107
    // EXPECT: Step 6: (87,70) r=111
    // EXPECT: Step 7: (85,80) r=116
    // EXPECT: Step 8: (83,90) r=122
    // EXPECT: Step 9: (81,100) r=128
    // EXPECT: Step 10: (79,110) r=135
    // EXPECT: Step 11: (77,120) r=142

    // Test 2: Angular momentum conservation
    printf("--- Conservation ---\n");
    // EXPECT: --- Conservation ---
    init_orbit(&orb, 100, 0, 0, 10, 10000);
    int L_vals[5];
    for (step = 0; step < 50; step = step + 1) {
        step_orbit(&orb);
        if (step % 10 == 9) {
            L_vals[step / 10] = angular_momentum(&orb);
        }
    }
    for (i = 0; i < 5; i = i + 1) {
        printf("L at step %d: %d\n", (i + 1) * 10, L_vals[i]);
    }
    // EXPECT: L at step 10: 1010
    // EXPECT: L at step 20: 1010
    // EXPECT: L at step 30: 1010
    // EXPECT: L at step 40: 1010
    // EXPECT: L at step 50: 1010

    // Test 3: Escape velocity (hyperbolic trajectory)
    printf("--- Escape ---\n");
    // EXPECT: --- Escape ---
    init_orbit(&orb, 100, 0, 0, 20, 10000);
    for (step = 0; step < 8; step = step + 1) {
        step_orbit(&orb);
        int r = orbital_radius(&orb);
        printf("r=%d\n", r);
    }
    // EXPECT: r=101
    // EXPECT: r=105
    // EXPECT: r=114
    // EXPECT: r=124
    // EXPECT: r=137
    // EXPECT: r=152
    // EXPECT: r=168
    // EXPECT: r=184

    // Test 4: Different GM values affect orbit shape
    printf("--- GM comparison ---\n");
    // EXPECT: --- GM comparison ---
    int gms[3];
    gms[0] = 5000;
    gms[1] = 10000;
    gms[2] = 20000;

    for (i = 0; i < 3; i = i + 1) {
        init_orbit(&orb, 100, 0, 0, 10, gms[i]);
        int j;
        for (j = 0; j < 20; j = j + 1) {
            step_orbit(&orb);
        }
        int r = orbital_radius(&orb);
        int e = specific_energy(&orb);
        printf("GM=%d: r=%d E=%d\n", gms[i], r, e);
    }
    // EXPECT: GM=5000: r=223 E=28
    // EXPECT: GM=10000: r=209 E=5
    // EXPECT: GM=20000: r=100 E=-144

    // Test 5: Orbit shape tracking (min/max radius over time)
    printf("--- Shape ---\n");
    // EXPECT: --- Shape ---
    init_orbit(&orb, 100, 0, 0, 8, 10000);
    int r_min = 10000;
    int r_max = 0;
    for (step = 0; step < 200; step = step + 1) {
        step_orbit(&orb);
        int r = orbital_radius(&orb);
        if (r < r_min) r_min = r;
        if (r > r_max) r_max = r;
    }
    printf("Elliptical orbit: rmin=%d rmax=%d\n", r_min, r_max);
    // EXPECT: Elliptical orbit: rmin=97 rmax=1627

    // Test 6: Speed at different orbital positions
    printf("--- Speed ---\n");
    // EXPECT: --- Speed ---
    init_orbit(&orb, 100, 0, 0, 10, 10000);
    for (step = 0; step < 6; step = step + 1) {
        step_orbit(&orb);
        int sp = orbital_speed(&orb);
        printf("speed=%d\n", sp);
    }
    // EXPECT: speed=10
    // EXPECT: speed=10
    // EXPECT: speed=10
    // EXPECT: speed=10
    // EXPECT: speed=10
    // EXPECT: speed=10

    printf("Orbital mechanics done\n");
    // EXPECT: Orbital mechanics done

    return 0;
}
