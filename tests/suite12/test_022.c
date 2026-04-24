int printf(const char *fmt, ...);

// ADSR Envelope Generator
// Attack-Decay-Sustain-Release envelope using integer math
// Amplitude scale: 0..1000 (representing 0.0 to 1.0)

struct Envelope {
    int attack;      // attack time in samples
    int decay;       // decay time in samples
    int sustain;     // sustain level 0..1000
    int release_time; // release time in samples
    int gate_on;     // total time gate is held
};

int envelope_value(struct Envelope env, int t) {
    int level;
    int release_start;
    int release_t;

    if (t < 0) return 0;

    // Gate is on for env.gate_on samples
    if (t < env.gate_on) {
        // Attack phase
        if (t < env.attack) {
            if (env.attack == 0) return 1000;
            level = (1000 * t) / env.attack;
            return level;
        }
        // Decay phase
        if (t < env.attack + env.decay) {
            int dt = t - env.attack;
            if (env.decay == 0) return env.sustain;
            level = 1000 - ((1000 - env.sustain) * dt) / env.decay;
            return level;
        }
        // Sustain phase
        return env.sustain;
    }

    // Release phase
    release_start = env.gate_on;
    release_t = t - release_start;
    if (release_t >= env.release_time) return 0;
    if (env.release_time == 0) return 0;

    // Find level at gate-off point
    if (env.gate_on <= env.attack) {
        if (env.attack == 0) level = 1000;
        else level = (1000 * env.gate_on) / env.attack;
    } else if (env.gate_on <= env.attack + env.decay) {
        int dt = env.gate_on - env.attack;
        if (env.decay == 0) level = env.sustain;
        else level = 1000 - ((1000 - env.sustain) * dt) / env.decay;
    } else {
        level = env.sustain;
    }

    // Fade from that level to 0
    return level - (level * release_t) / env.release_time;
}

int apply_envelope(int sample, int env_level) {
    // Scale sample by envelope level (0..1000)
    return (sample * env_level) / 1000;
}

void print_envelope_curve(struct Envelope env, int total_time) {
    int t;
    for (t = 0; t < total_time; t = t + 1) {
        int val = envelope_value(env, t);
        printf("%d", val);
        if (t < total_time - 1) printf(" ");
    }
    printf("\n");
}

int find_peak(struct Envelope env, int total_time) {
    int t;
    int max_val = 0;
    for (t = 0; t < total_time; t = t + 1) {
        int val = envelope_value(env, t);
        if (val > max_val) max_val = val;
    }
    return max_val;
}

int find_peak_time(struct Envelope env, int total_time) {
    int t;
    int max_val = 0;
    int max_t = 0;
    for (t = 0; t < total_time; t = t + 1) {
        int val = envelope_value(env, t);
        if (val > max_val) {
            max_val = val;
            max_t = t;
        }
    }
    return max_t;
}

int compute_average(struct Envelope env, int total_time) {
    int t;
    int sum = 0;
    for (t = 0; t < total_time; t = t + 1) {
        sum = sum + envelope_value(env, t);
    }
    return sum / total_time;
}

int main() {
    struct Envelope env1;
    env1.attack = 4;
    env1.decay = 4;
    env1.sustain = 500;
    env1.release_time = 4;
    env1.gate_on = 12;

    // Print envelope curve for 16 samples
    printf("ADSR curve: ");
    print_envelope_curve(env1, 16);
    // EXPECT: ADSR curve: 0 250 500 750 1000 875 750 625 500 500 500 500 500 375 250 125

    int peak = find_peak(env1, 16);
    printf("Peak level: %d\n", peak);
    // EXPECT: Peak level: 1000

    int peak_t = find_peak_time(env1, 16);
    printf("Peak at sample: %d\n", peak_t);
    // EXPECT: Peak at sample: 4

    int avg = compute_average(env1, 16);
    printf("Average level: %d\n", avg);
    // EXPECT: Average level: 500

    // Apply envelope to a constant signal of 200
    printf("Applied: ");
    int t;
    for (t = 0; t < 16; t = t + 1) {
        int env_val = envelope_value(env1, t);
        int out = apply_envelope(200, env_val);
        printf("%d", out);
        if (t < 15) printf(" ");
    }
    printf("\n");
    // EXPECT: Applied: 0 50 100 150 200 175 150 125 100 100 100 100 100 75 50 25

    // Short envelope - no sustain phase
    struct Envelope env2;
    env2.attack = 2;
    env2.decay = 2;
    env2.sustain = 300;
    env2.release_time = 2;
    env2.gate_on = 4;

    printf("Short ADSR: ");
    print_envelope_curve(env2, 8);
    // EXPECT: Short ADSR: 0 500 1000 650 300 150 0 0

    // Instant attack
    struct Envelope env3;
    env3.attack = 0;
    env3.decay = 3;
    env3.sustain = 400;
    env3.release_time = 3;
    env3.gate_on = 6;

    printf("Instant attack: ");
    print_envelope_curve(env3, 10);
    // EXPECT: Instant attack: 1000 800 600 400 400 400 400 267 134 0

    int peak3 = find_peak(env3, 10);
    printf("Peak3: %d\n", peak3);
    // EXPECT: Peak3: 1000

    return 0;
}
