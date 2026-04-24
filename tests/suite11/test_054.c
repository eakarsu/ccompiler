int printf(const char *fmt, ...);

// Rigid body velocity and rotation simulation (2D, integer scaled)
// Scale factor: 1000

struct RigidBody {
    int px;         // position x, *1000
    int py;         // position y, *1000
    int vx;         // velocity x, *1000
    int vy;         // velocity y, *1000
    int angle;      // angle in milliradians (0-6283)
    int omega;      // angular velocity in millirad/s
    int mass;       // mass * 1000
    int inertia;    // moment of inertia * 1000
};

void init_body(struct RigidBody *b, int px, int py, int mass) {
    b->px = px;
    b->py = py;
    b->vx = 0;
    b->vy = 0;
    b->angle = 0;
    b->omega = 0;
    b->mass = mass;
    // For a uniform disk: I = m*r^2/2, simplified
    b->inertia = mass / 2;
}

void apply_force(struct RigidBody *b, int fx, int fy, int dt) {
    // a = F/m, v += a*dt
    int ax = fx * 1000 / b->mass;
    int ay = fy * 1000 / b->mass;
    b->vx = b->vx + ax * dt / 1000;
    b->vy = b->vy + ay * dt / 1000;
}

void apply_torque(struct RigidBody *b, int torque, int dt) {
    // alpha = torque / I
    int alpha = torque * 1000 / b->inertia;
    b->omega = b->omega + alpha * dt / 1000;
}

void update_body(struct RigidBody *b, int dt) {
    b->px = b->px + b->vx * dt / 1000;
    b->py = b->py + b->vy * dt / 1000;
    b->angle = b->angle + b->omega * dt / 1000;
    // Wrap angle to 0..6283
    while (b->angle >= 6283) b->angle = b->angle - 6283;
    while (b->angle < 0) b->angle = b->angle + 6283;
}

int kinetic_energy(struct RigidBody *b) {
    // KE = 0.5*m*v^2 + 0.5*I*w^2
    int v2 = (b->vx / 100) * (b->vx / 100) + (b->vy / 100) * (b->vy / 100);
    int ke_lin = b->mass * v2 / 2000;
    int ke_rot = b->inertia * (b->omega / 100) * (b->omega / 100) / 2000;
    return ke_lin + ke_rot;
}

int momentum_x(struct RigidBody *b) {
    return b->mass * b->vx / 1000;
}

int momentum_y(struct RigidBody *b) {
    return b->mass * b->vy / 1000;
}

int speed(struct RigidBody *b) {
    // integer sqrt approximation
    int v2 = (b->vx / 10) * (b->vx / 10) + (b->vy / 10) * (b->vy / 10);
    int s = 0;
    while (s * s < v2) s = s + 1;
    if (s > 0 && (s * s - v2) > (v2 - (s-1)*(s-1))) s = s - 1;
    return s * 10;
}

void apply_friction(struct RigidBody *b, int coeff) {
    // friction reduces velocity: v *= (1000 - coeff) / 1000
    b->vx = b->vx * (1000 - coeff) / 1000;
    b->vy = b->vy * (1000 - coeff) / 1000;
    b->omega = b->omega * (1000 - coeff) / 1000;
}

int main() {
    struct RigidBody body;
    struct RigidBody b1;
    struct RigidBody b2;
    int i;
    int ke;
    int spd;

    printf("=== Rigid Body Simulation ===\n");
    // EXPECT: === Rigid Body Simulation ===

    // Test 1: Linear motion under force
    init_body(&body, 0, 0, 2000);
    apply_force(&body, 1000, 500, 100);
    printf("After force: vx=%d vy=%d\n", body.vx, body.vy);
    // EXPECT: After force: vx=50 vy=25

    for (i = 0; i < 5; i = i + 1) {
        apply_force(&body, 1000, 500, 100);
        update_body(&body, 100);
        printf("Step %d: pos=(%d,%d) vel=(%d,%d)\n",
               i, body.px, body.py, body.vx, body.vy);
    }
    // EXPECT: Step 0: pos=(10,5) vel=(100,50)
    // EXPECT: Step 1: pos=(25,12) vel=(150,75)
    // EXPECT: Step 2: pos=(45,22) vel=(200,100)
    // EXPECT: Step 3: pos=(70,34) vel=(250,125)
    // EXPECT: Step 4: pos=(100,49) vel=(300,150)

    // Test 2: Rotation
    printf("--- Rotation ---\n");
    // EXPECT: --- Rotation ---
    init_body(&body, 0, 0, 1000);
    apply_torque(&body, 500, 100);
    printf("Angular vel: %d\n", body.omega);
    // EXPECT: Angular vel: 100

    for (i = 0; i < 5; i = i + 1) {
        update_body(&body, 100);
        printf("angle=%d omega=%d\n", body.angle, body.omega);
    }
    // EXPECT: angle=10 omega=100
    // EXPECT: angle=20 omega=100
    // EXPECT: angle=30 omega=100
    // EXPECT: angle=40 omega=100
    // EXPECT: angle=50 omega=100

    // Test 3: Energy computation
    printf("--- Energy ---\n");
    // EXPECT: --- Energy ---
    init_body(&body, 0, 0, 2000);
    body.vx = 3000;
    body.vy = 4000;
    body.omega = 1000;
    ke = kinetic_energy(&body);
    printf("KE: %d\n", ke);
    // EXPECT: KE: 2550

    spd = speed(&body);
    printf("Speed: %d\n", spd);
    // EXPECT: Speed: 5000

    // Test 4: Momentum conservation (collision)
    printf("--- Collision ---\n");
    // EXPECT: --- Collision ---
    init_body(&b1, 0, 0, 1000);
    init_body(&b2, 10000, 0, 1000);
    b1.vx = 2000;
    b2.vx = -1000;

    int p_before = momentum_x(&b1) + momentum_x(&b2);
    printf("Momentum before: %d\n", p_before);
    // EXPECT: Momentum before: 1000

    // Elastic collision: swap velocities for equal mass
    int tmp = b1.vx;
    b1.vx = b2.vx;
    b2.vx = tmp;

    int p_after = momentum_x(&b1) + momentum_x(&b2);
    printf("Momentum after: %d\n", p_after);
    // EXPECT: Momentum after: 1000
    printf("b1.vx=%d b2.vx=%d\n", b1.vx, b2.vx);
    // EXPECT: b1.vx=-1000 b2.vx=2000

    // Test 5: Friction slowdown
    printf("--- Friction ---\n");
    // EXPECT: --- Friction ---
    init_body(&body, 0, 0, 1000);
    body.vx = 5000;
    body.vy = 3000;
    body.omega = 2000;

    for (i = 0; i < 8; i = i + 1) {
        apply_friction(&body, 100);
        printf("vx=%d vy=%d omega=%d\n", body.vx, body.vy, body.omega);
    }
    // EXPECT: vx=4500 vy=2700 omega=1800
    // EXPECT: vx=4050 vy=2430 omega=1620
    // EXPECT: vx=3645 vy=2187 omega=1458
    // EXPECT: vx=3280 vy=1968 omega=1312
    // EXPECT: vx=2952 vy=1771 omega=1180
    // EXPECT: vx=2656 vy=1593 omega=1062
    // EXPECT: vx=2390 vy=1433 omega=955
    // EXPECT: vx=2151 vy=1289 omega=859

    printf("Rigid body simulation complete\n");
    // EXPECT: Rigid body simulation complete

    return 0;
}
