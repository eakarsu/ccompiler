int printf(const char *fmt, ...);

// Gravity simulation: freefall, bouncing, projectiles, two-body
// Integer-only physics

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

struct Faller {
    int y;
    int vy;
    int g;
    int ground;
    int bounce_pct;
};

void init_faller(struct Faller *f, int y, int vy, int g, int ground, int bounce) {
    f->y = y;
    f->vy = vy;
    f->g = g;
    f->ground = ground;
    f->bounce_pct = bounce;
}

void step_faller(struct Faller *f) {
    f->vy = f->vy - f->g;
    f->y = f->y + f->vy;
    if (f->y <= f->ground) {
        f->y = f->ground;
        if (f->vy < 0) {
            f->vy = -f->vy * f->bounce_pct / 100;
        }
    }
}

struct GravBody {
    int x;
    int y;
    int vx;
    int vy;
    int gm;
};

void init_gbody(struct GravBody *b, int x, int y, int vx, int vy, int gm) {
    b->x = x;
    b->y = y;
    b->vx = vx;
    b->vy = vy;
    b->gm = gm;
}

void step_two_bodies(struct GravBody *a, struct GravBody *b) {
    int dx = b->x - a->x;
    int dy = b->y - a->y;
    int r2 = dx * dx + dy * dy;
    if (r2 < 100) r2 = 100;
    int r = isqrt(r2);
    if (r < 10) r = 10;

    int acc_a = b->gm * 100 / r2;
    int acc_b = a->gm * 100 / r2;

    a->vx = a->vx + acc_a * dx / r;
    a->vy = a->vy + acc_a * dy / r;
    b->vx = b->vx - acc_b * dx / r;
    b->vy = b->vy - acc_b * dy / r;

    a->x = a->x + a->vx;
    a->y = a->y + a->vy;
    b->x = b->x + b->vx;
    b->y = b->y + b->vy;
}

int main() {
    int step;
    int i;

    printf("=== Gravity Simulation ===\n");
    // EXPECT: === Gravity Simulation ===

    // Test 1: Simple freefall from height 1000, g=10
    printf("--- Freefall ---\n");
    // EXPECT: --- Freefall ---
    struct Faller f;
    init_faller(&f, 1000, 0, 10, 0, 0);

    for (step = 0; step < 10; step = step + 1) {
        step_faller(&f);
        printf("y=%d vy=%d\n", f.y, f.vy);
    }
    // EXPECT: y=990 vy=-10
    // EXPECT: y=970 vy=-20
    // EXPECT: y=940 vy=-30
    // EXPECT: y=900 vy=-40
    // EXPECT: y=850 vy=-50
    // EXPECT: y=790 vy=-60
    // EXPECT: y=720 vy=-70
    // EXPECT: y=640 vy=-80
    // EXPECT: y=550 vy=-90
    // EXPECT: y=450 vy=-100

    // Test 2: Bouncing ball with restitution 70%
    printf("--- Bounce ---\n");
    // EXPECT: --- Bounce ---
    init_faller(&f, 200, 0, 10, 0, 70);

    int bounce_count = 0;
    int prev_vy;
    for (step = 0; step < 30; step = step + 1) {
        prev_vy = f.vy;
        step_faller(&f);
        if (prev_vy < 0 && f.vy > 0) {
            bounce_count = bounce_count + 1;
            printf("Bounce %d at step %d: vy=%d y=%d\n",
                   bounce_count, step, f.vy, f.y);
        }
    }

    // EXPECT: Bounce 1 at step 5: vy=42 y=0
    // EXPECT: Bounce 2 at step 13: vy=26 y=0
    // EXPECT: Bounce 3 at step 18: vy=16 y=0
    // EXPECT: Bounce 4 at step 21: vy=9 y=0

    printf("Total bounces: %d\n", bounce_count);
    // EXPECT: Total bounces: 4

    // Test 3: Thrown upward
    printf("--- Throw Up ---\n");
    // EXPECT: --- Throw Up ---
    init_faller(&f, 0, 100, 10, 0, 0);

    int max_height = 0;
    for (step = 0; step < 22; step = step + 1) {
        step_faller(&f);
        if (f.y > max_height) max_height = f.y;
        if (step % 4 == 3) {
            printf("t=%d: y=%d vy=%d\n", step + 1, f.y, f.vy);
        }
    }

    // EXPECT: t=4: y=300 vy=60
    // EXPECT: t=8: y=440 vy=20
    // EXPECT: t=12: y=420 vy=-20
    // EXPECT: t=16: y=240 vy=-60
    // EXPECT: t=20: y=0 vy=0

    printf("Max height: %d\n", max_height);
    // EXPECT: Max height: 450

    // Test 4: Two-body gravity
    printf("--- Two Body ---\n");
    // EXPECT: --- Two Body ---
    struct GravBody g1;
    struct GravBody g2;
    init_gbody(&g1, 0, 0, 0, 0, 500);
    init_gbody(&g2, 200, 0, 0, 5, 500);

    for (step = 0; step < 10; step = step + 1) {
        step_two_bodies(&g1, &g2);
        printf("g1=(%d,%d) g2=(%d,%d)\n", g1.x, g1.y, g2.x, g2.y);
    }
    // EXPECT: g1=(1,0) g2=(199,5)
    // EXPECT: g1=(3,0) g2=(197,10)
    // EXPECT: g1=(6,0) g2=(194,15)
    // EXPECT: g1=(10,0) g2=(190,20)
    // EXPECT: g1=(14,0) g2=(186,25)
    // EXPECT: g1=(18,0) g2=(182,30)
    // EXPECT: g1=(22,0) g2=(178,35)
    // EXPECT: g1=(26,0) g2=(174,40)
    // EXPECT: g1=(31,0) g2=(169,45)
    // EXPECT: g1=(37,0) g2=(163,50)

    // Test 5: Multi-faller race (different starting heights)
    printf("--- Race ---\n");
    // EXPECT: --- Race ---
    struct Faller fallers[3];
    init_faller(&fallers[0], 500, 0, 10, 0, 0);
    init_faller(&fallers[1], 300, 0, 10, 0, 0);
    init_faller(&fallers[2], 100, 0, 10, 0, 0);

    for (step = 0; step < 12; step = step + 1) {
        for (i = 0; i < 3; i = i + 1) {
            step_faller(&fallers[i]);
        }
        if (step % 3 == 2) {
            printf("step %d: f0=%d f1=%d f2=%d\n", step,
                   fallers[0].y, fallers[1].y, fallers[2].y);
        }
    }
    // EXPECT: step 2: f0=440 f1=240 f2=40
    // EXPECT: step 5: f0=290 f1=90 f2=0
    // EXPECT: step 8: f0=50 f1=0 f2=0
    // EXPECT: step 11: f0=0 f1=0 f2=0

    // Test 6: Energy conservation check in freefall
    printf("--- Energy ---\n");
    // EXPECT: --- Energy ---
    init_faller(&f, 500, 0, 10, 0, 100);
    for (step = 0; step < 5; step = step + 1) {
        int ke = f.vy * f.vy / 2;
        int pe = f.y * 10;
        printf("KE=%d PE=%d Total=%d\n", ke, pe, ke + pe);
        step_faller(&f);
    }
    // EXPECT: KE=0 PE=5000 Total=5000
    // EXPECT: KE=50 PE=4900 Total=4950
    // EXPECT: KE=200 PE=4700 Total=4900
    // EXPECT: KE=450 PE=4400 Total=4850
    // EXPECT: KE=800 PE=4000 Total=4800

    printf("Gravity simulation done\n");
    // EXPECT: Gravity simulation done

    return 0;
}
