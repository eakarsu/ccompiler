int printf(const char *fmt, ...);

// Pendulum simulation using small-angle approximation (integer math)
// sin(theta) ~ theta for small angles (in milliradians)
// theta'' = -(g/L) * theta
// Angles in milliradians, g=9800 (mm/s^2), L in mm

struct Pendulum {
    int theta;       // angle in milliradians
    int omega;       // angular velocity in millirad/s
    int length;      // length in mm
    int g;           // gravity in mm/s^2
    int damping;     // damping coefficient *1000
};

void init_pendulum(struct Pendulum *p, int theta, int length) {
    p->theta = theta;
    p->omega = 0;
    p->length = length;
    p->g = 9800;
    p->damping = 0;
}

void set_damping(struct Pendulum *p, int d) {
    p->damping = d;
}

void step_pendulum(struct Pendulum *p, int dt) {
    int alpha = -p->g * p->theta / p->length;

    if (p->damping > 0) {
        alpha = alpha - p->damping * p->omega / 1000;
    }

    p->omega = p->omega + alpha * dt / 1000;
    p->theta = p->theta + p->omega * dt / 1000;
}

int pendulum_energy(struct Pendulum *p) {
    int ke = p->length * (p->omega / 100) * (p->omega / 100) / 2000;
    int pe = p->g * (p->theta / 10) * (p->theta / 10) / (2000 * p->length / 1000);
    return ke + pe;
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

// Find period by measuring time between same-direction zero crossings
int measure_period(struct Pendulum *p, int dt) {
    int t = 0;
    int crossings = 0;
    int prev_theta;
    int max_t = 100000;

    while (t < max_t) {
        prev_theta = p->theta;
        step_pendulum(p, dt);
        t = t + dt;
        if (prev_theta > 0 && p->theta <= 0) {
            crossings = crossings + 1;
            if (crossings == 2) return t;
        }
    }
    return -1;
}

int main() {
    struct Pendulum pend;
    int step;
    int i;

    printf("=== Pendulum Simulation ===\n");
    // EXPECT: === Pendulum Simulation ===

    // Test 1: Basic oscillation (L=1000mm, theta0=100 millirad ~ 5.7 deg)
    init_pendulum(&pend, 100, 1000);
    printf("Initial: theta=%d omega=%d\n", pend.theta, pend.omega);
    // EXPECT: Initial: theta=100 omega=0

    for (step = 0; step < 10; step = step + 1) {
        step_pendulum(&pend, 50);
        printf("t=%d: theta=%d omega=%d\n", (step + 1) * 50,
               pend.theta, pend.omega);
    }
    // EXPECT: t=50: theta=98 omega=-49
    // EXPECT: t=100: theta=94 omega=-97
    // EXPECT: t=150: theta=87 omega=-143
    // EXPECT: t=200: theta=78 omega=-185
    // EXPECT: t=250: theta=67 omega=-223
    // EXPECT: t=300: theta=55 omega=-255
    // EXPECT: t=350: theta=41 omega=-281
    // EXPECT: t=400: theta=26 omega=-301
    // EXPECT: t=450: theta=11 omega=-313
    // EXPECT: t=500: theta=-4 omega=-318

    // Test 2: Continue to see full oscillation
    printf("--- Full Swing ---\n");
    // EXPECT: --- Full Swing ---
    for (step = 0; step < 10; step = step + 1) {
        step_pendulum(&pend, 50);
        printf("t=%d: theta=%d omega=%d\n", 500 + (step + 1) * 50,
               pend.theta, pend.omega);
    }
    // EXPECT: t=550: theta=-19 omega=-317
    // EXPECT: t=600: theta=-34 omega=-308
    // EXPECT: t=650: theta=-48 omega=-292
    // EXPECT: t=700: theta=-61 omega=-269
    // EXPECT: t=750: theta=-73 omega=-240
    // EXPECT: t=800: theta=-83 omega=-205
    // EXPECT: t=850: theta=-91 omega=-165
    // EXPECT: t=900: theta=-97 omega=-121
    // EXPECT: t=950: theta=-100 omega=-74
    // EXPECT: t=1000: theta=-101 omega=-25

    // Test 3: Measure period
    printf("--- Period ---\n");
    // EXPECT: --- Period ---
    init_pendulum(&pend, 100, 1000);
    int period = measure_period(&pend, 10);
    printf("Period (L=1000mm): %d ms\n", period);
    // EXPECT: Period (L=1000mm): 2850 ms

    // Test 4: Different lengths affect period
    printf("--- Length vs Period ---\n");
    // EXPECT: --- Length vs Period ---
    int lengths[4];
    lengths[0] = 250;
    lengths[1] = 500;
    lengths[2] = 1000;
    lengths[3] = 2000;
    for (i = 0; i < 4; i = i + 1) {
        init_pendulum(&pend, 50, lengths[i]);
        int p = measure_period(&pend, 10);
        printf("L=%d: T=%d ms\n", lengths[i], p);
    }
    // EXPECT: L=250: T=1390 ms
    // EXPECT: L=500: T=2120 ms
    // EXPECT: L=1000: T=3720 ms
    // EXPECT: L=2000: T=4970 ms

    // Test 5: Damped pendulum - amplitude decay
    printf("--- Damped ---\n");
    // EXPECT: --- Damped ---
    init_pendulum(&pend, 500, 1000);
    set_damping(&pend, 50);

    // Track amplitude peaks
    int prev_abs = abs_val(pend.theta);
    int peak_count = 0;
    int peaks[10];
    for (step = 0; step < 2000; step = step + 1) {
        step_pendulum(&pend, 10);
        int cur_abs = abs_val(pend.theta);
        if (prev_abs > cur_abs && prev_abs > 10 && peak_count < 6) {
            // Check if this is truly a peak (not just noise)
            if (peak_count == 0 || prev_abs < peaks[peak_count - 1]) {
                peaks[peak_count] = prev_abs;
                peak_count = peak_count + 1;
            }
        }
        prev_abs = cur_abs;
    }

    for (i = 0; i < peak_count; i = i + 1) {
        printf("Peak %d: amplitude=%d\n", i, peaks[i]);
    }
    // EXPECT: Peak 0: amplitude=500
    // EXPECT: Peak 1: amplitude=499
    // EXPECT: Peak 2: amplitude=498
    // EXPECT: Peak 3: amplitude=496
    // EXPECT: Peak 4: amplitude=494
    // EXPECT: Peak 5: amplitude=491

    // Test 6: Energy check
    printf("--- Energy ---\n");
    // EXPECT: --- Energy ---
    init_pendulum(&pend, 100, 1000);
    int e0 = pendulum_energy(&pend);
    printf("Initial energy: %d\n", e0);
    // EXPECT: Initial energy: 490

    for (step = 0; step < 100; step = step + 1) {
        step_pendulum(&pend, 10);
    }
    int e1 = pendulum_energy(&pend);
    printf("Energy after 1s: %d\n", e1);
    // EXPECT: Energy after 1s: 396

    // Test 7: Large vs small amplitude comparison
    printf("--- Amplitude ---\n");
    // EXPECT: --- Amplitude ---
    init_pendulum(&pend, 50, 1000);
    int p1 = measure_period(&pend, 10);
    init_pendulum(&pend, 200, 1000);
    int p2 = measure_period(&pend, 10);
    printf("Small angle period: %d ms\n", p1);
    // EXPECT: Small angle period: 3720 ms
    printf("Large angle period: %d ms\n", p2);
    // EXPECT: Large angle period: 2680 ms

    printf("Pendulum simulation done\n");
    // EXPECT: Pendulum simulation done

    return 0;
}
