int printf(const char *fmt, ...);
// EXPECT: === Signal Processing ===\nSquare wave: 100 100 100 100 -100 -100 -100 -100 100 100 100 100 -100 -100 -100 -100 \nSawtooth wave: -50 -38 -25 -13 0 12 25 37 -50 -38 -25 -13 0 12 25 37 \nSquare: peak=100 valley=-100 energy=160000 rms=100\nSawtooth: peak=37 valley=-50 energy=13752 rms=29\nFiltered square (MA-3): 100 100 100 33 -33 -100 -100 -33 33 100 100 33 -33 -100 -100 -100 \nCross-correlation (sq x saw):\n  lag=-3: 12600\n  lag=-2: -6200\n  lag=-1: -23700\n  lag=0: -40000\n  lag=1: -25000\n  lag=2: -8800\n  lag=3: 8700

struct Signal {
    int samples[32];
    int length;
};

void init_signal(struct Signal *s, int len) {
    s->length = len;
    int i;
    for (i = 0; i < len; i++) {
        s->samples[i] = 0;
    }
}

void generate_square_wave(struct Signal *s, int period, int amplitude) {
    int i;
    for (i = 0; i < s->length; i++) {
        if ((i / (period / 2)) % 2 == 0) {
            s->samples[i] = amplitude;
        } else {
            s->samples[i] = -amplitude;
        }
    }
}

void generate_sawtooth(struct Signal *s, int period, int amplitude) {
    int i;
    for (i = 0; i < s->length; i++) {
        s->samples[i] = (i % period) * amplitude / period - amplitude / 2;
    }
}

void moving_average(struct Signal *input, struct Signal *output, int window) {
    output->length = input->length;
    int i, j;
    for (i = 0; i < input->length; i++) {
        int sum = 0;
        int count = 0;
        for (j = i - window / 2; j <= i + window / 2; j++) {
            if (j >= 0 && j < input->length) {
                sum = sum + input->samples[j];
                count++;
            }
        }
        output->samples[i] = sum / count;
    }
}

int find_peak(struct Signal *s) {
    int peak = s->samples[0];
    int i;
    for (i = 1; i < s->length; i++) {
        if (s->samples[i] > peak) peak = s->samples[i];
    }
    return peak;
}

int find_valley(struct Signal *s) {
    int valley = s->samples[0];
    int i;
    for (i = 1; i < s->length; i++) {
        if (s->samples[i] < valley) valley = s->samples[i];
    }
    return valley;
}

int compute_energy(struct Signal *s) {
    int energy = 0;
    int i;
    for (i = 0; i < s->length; i++) {
        energy = energy + s->samples[i] * s->samples[i];
    }
    return energy;
}

int compute_rms(struct Signal *s) {
    int energy = compute_energy(s);
    int avg = energy / s->length;
    int rms = 1;
    while (rms * rms < avg) rms++;
    if (rms * rms - avg > avg - (rms - 1) * (rms - 1)) rms--;
    return rms;
}

int cross_correlate(struct Signal *a, struct Signal *b, int lag) {
    int sum = 0;
    int i;
    for (i = 0; i < a->length; i++) {
        int j = i + lag;
        if (j >= 0 && j < b->length) {
            sum = sum + a->samples[i] * b->samples[j];
        }
    }
    return sum;
}

int main(void) {
    struct Signal sq;
    struct Signal saw;
    struct Signal filtered;

    init_signal(&sq, 16);
    init_signal(&saw, 16);
    init_signal(&filtered, 16);

    generate_square_wave(&sq, 8, 100);
    generate_sawtooth(&saw, 8, 100);

    printf("=== Signal Processing ===\n");

    printf("Square wave: ");
    int i;
    for (i = 0; i < sq.length; i++) {
        printf("%d ", sq.samples[i]);
    }
    printf("\n");

    printf("Sawtooth wave: ");
    for (i = 0; i < saw.length; i++) {
        printf("%d ", saw.samples[i]);
    }
    printf("\n");

    printf("Square: peak=%d valley=%d energy=%d rms=%d\n",
           find_peak(&sq), find_valley(&sq), compute_energy(&sq), compute_rms(&sq));
    printf("Sawtooth: peak=%d valley=%d energy=%d rms=%d\n",
           find_peak(&saw), find_valley(&saw), compute_energy(&saw), compute_rms(&saw));

    moving_average(&sq, &filtered, 3);
    printf("Filtered square (MA-3): ");
    for (i = 0; i < filtered.length; i++) {
        printf("%d ", filtered.samples[i]);
    }
    printf("\n");

    printf("Cross-correlation (sq x saw):\n");
    int lag;
    for (lag = -3; lag <= 3; lag++) {
        printf("  lag=%d: %d\n", lag, cross_correlate(&sq, &saw, lag));
    }

    return 0;
}
