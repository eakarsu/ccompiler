int printf(const char *fmt, ...);

// Spring-mass system simulation using scaled integers
// F = -k*x (Hooke's law), F = m*a
// Using Euler integration with scale factor 1000

struct SpringMass {
    int position;    // scaled by 1000
    int velocity;    // scaled by 1000
    int mass;        // scaled by 1000
    int k;           // spring constant, scaled by 1000
    int damping;     // damping coefficient, scaled by 1000
};

void init_spring(struct SpringMass *s, int pos, int vel, int mass, int k, int damp) {
    s->position = pos;
    s->velocity = vel;
    s->mass = mass;
    s->k = k;
    s->damping = damp;
}

// Step the simulation by dt (in milliseconds)
void step_spring(struct SpringMass *s, int dt) {
    // Force = -k * x - damping * v
    int force_spring = -s->k * s->position / 1000;
    int force_damp = -s->damping * s->velocity / 1000;
    int force = force_spring + force_damp;

    // acceleration = force / mass
    int accel = force * 1000 / s->mass;

    // velocity += accel * dt / 1000
    s->velocity = s->velocity + accel * dt / 1000;

    // position += velocity * dt / 1000
    s->position = s->position + s->velocity * dt / 1000;
}

int compute_energy(struct SpringMass *s) {
    // KE = m*v*v / (2*1000*1000)
    int ke = s->mass * (s->velocity / 100) * (s->velocity / 100) / 2000;
    // PE = k*x*x / (2*1000*1000)
    int pe = s->k * (s->position / 100) * (s->position / 100) / 2000;
    return ke + pe;
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int find_period_steps(struct SpringMass *s, int dt, int max_steps) {
    int initial_pos = s->position;
    int crossed_zero = 0;
    int i;
    int prev_pos;

    for (i = 0; i < max_steps; i = i + 1) {
        prev_pos = s->position;
        step_spring(s, dt);
        // Detect zero crossings (positive to negative)
        if (prev_pos > 0 && s->position <= 0) {
            crossed_zero = crossed_zero + 1;
            if (crossed_zero == 2) {
                return i + 1;
            }
        }
    }
    return -1;
}

void run_oscillation(struct SpringMass *s, int steps, int dt, int print_every) {
    int i;
    int energy;
    for (i = 0; i < steps; i = i + 1) {
        if (i % print_every == 0) {
            energy = compute_energy(s);
            printf("step %d: pos=%d vel=%d energy=%d\n",
                   i, s->position, s->velocity, energy);
        }
        step_spring(s, dt);
    }
}

int main() {
    struct SpringMass spring;
    int i;
    int energy;

    printf("=== Spring-Mass System ===\n");
    // EXPECT: === Spring-Mass System ===

    // Test 1: Simple harmonic motion (no damping)
    // k=1000, m=1000, x0=5000, v0=0
    init_spring(&spring, 5000, 0, 1000, 1000, 0);
    printf("Initial: pos=%d vel=%d\n", spring.position, spring.velocity);
    // EXPECT: Initial: pos=5000 vel=0

    // Run 10 steps at dt=100ms
    for (i = 0; i < 10; i = i + 1) {
        step_spring(&spring, 100);
        printf("t=%d: pos=%d vel=%d\n", (i + 1) * 100, spring.position, spring.velocity);
    }
    // EXPECT: t=100: pos=4950 vel=-500
    // EXPECT: t=200: pos=4851 vel=-995
    // EXPECT: t=300: pos=4703 vel=-1480
    // EXPECT: t=400: pos=4508 vel=-1950
    // EXPECT: t=500: pos=4268 vel=-2400
    // EXPECT: t=600: pos=3986 vel=-2826
    // EXPECT: t=700: pos=3664 vel=-3224
    // EXPECT: t=800: pos=3305 vel=-3590
    // EXPECT: t=900: pos=2913 vel=-3920
    // EXPECT: t=1000: pos=2492 vel=-4211

    // Test 2: Energy conservation (no damping)
    printf("--- Energy Test ---\n");
    // EXPECT: --- Energy Test ---
    init_spring(&spring, 5000, 0, 1000, 1000, 0);
    energy = compute_energy(&spring);
    printf("Initial energy: %d\n", energy);
    // EXPECT: Initial energy: 1250

    for (i = 0; i < 20; i = i + 1) {
        step_spring(&spring, 100);
    }
    energy = compute_energy(&spring);
    printf("Energy after 20 steps: %d\n", energy);
    // EXPECT: Energy after 20 steps: 1276

    // Test 3: Damped oscillation
    printf("--- Damped System ---\n");
    // EXPECT: --- Damped System ---
    init_spring(&spring, 5000, 0, 1000, 1000, 200);

    for (i = 0; i < 10; i = i + 1) {
        step_spring(&spring, 100);
        if (i % 2 == 1) {
            printf("t=%d: pos=%d vel=%d\n", (i + 1) * 100, spring.position, spring.velocity);
        }
    }
    // EXPECT: t=200: pos=4852 vel=-985
    // EXPECT: t=400: pos=4518 vel=-1891
    // EXPECT: t=600: pos=4020 vel=-2687
    // EXPECT: t=800: pos=3383 vel=-3346
    // EXPECT: t=1000: pos=2638 vel=-3846

    // Test 4: Stiff spring (k=4000)
    printf("--- Stiff Spring ---\n");
    // EXPECT: --- Stiff Spring ---
    init_spring(&spring, 3000, 0, 1000, 4000, 0);
    for (i = 0; i < 8; i = i + 1) {
        step_spring(&spring, 50);
        printf("pos=%d\n", spring.position);
    }
    // EXPECT: pos=2970
    // EXPECT: pos=2911
    // EXPECT: pos=2823
    // EXPECT: pos=2706
    // EXPECT: pos=2562
    // EXPECT: pos=2393
    // EXPECT: pos=2200
    // EXPECT: pos=1985

    // Test 5: Multiple coupled springs (two masses, fixed walls)
    printf("--- Two Mass System ---\n");
    // EXPECT: --- Two Mass System ---
    struct SpringMass s1;
    struct SpringMass s2;
    init_spring(&s1, 3000, 0, 1000, 500, 0);
    init_spring(&s2, -2000, 0, 1000, 500, 0);

    for (i = 0; i < 6; i = i + 1) {
        // coupling force between s1 and s2
        int coupling = 500 * (s1.position - s2.position) / 1000;
        step_spring(&s1, 100);
        step_spring(&s2, 100);
        // apply coupling adjustment
        s1.velocity = s1.velocity - coupling * 100 / 1000;
        s2.velocity = s2.velocity + coupling * 100 / 1000;
        printf("s1=%d s2=%d\n", s1.position, s2.position);
    }
    // EXPECT: s1=2985 s2=-1990
    // EXPECT: s1=2931 s2=-1946
    // EXPECT: s1=2837 s2=-1867
    // EXPECT: s1=2705 s2=-1754
    // EXPECT: s1=2536 s2=-1609
    // EXPECT: s1=2332 s2=-1434

    printf("Simulation complete\n");
    // EXPECT: Simulation complete

    return 0;
}
