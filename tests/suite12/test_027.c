int printf(const char *fmt, ...);

// Echo / Delay Effect
// Implements a simple echo effect using a circular delay buffer
// All integer math, no floating point

struct EchoState {
    int buffer[64];
    int write_pos;
    int delay_samples;
    int feedback;     // 0..100 (percent)
    int wet_mix;      // 0..100 (percent)
    int dry_mix;      // 0..100 (percent)
    int buf_size;
};

void echo_init(struct EchoState *e, int delay, int feedback, int wet, int dry) {
    int i;
    for (i = 0; i < 64; i = i + 1) {
        e->buffer[i] = 0;
    }
    e->write_pos = 0;
    e->delay_samples = delay;
    e->feedback = feedback;
    e->wet_mix = wet;
    e->dry_mix = dry;
    e->buf_size = 64;
}

int echo_process(struct EchoState *e, int input) {
    // Read delayed sample
    int read_pos = e->write_pos - e->delay_samples;
    if (read_pos < 0) read_pos = read_pos + e->buf_size;

    int delayed = e->buffer[read_pos];

    // Mix: output = dry*input + wet*delayed
    int output = (e->dry_mix * input) / 100 + (e->wet_mix * delayed) / 100;

    // Write to buffer: input + feedback*delayed
    e->buffer[e->write_pos] = input + (e->feedback * delayed) / 100;

    // Advance write position
    e->write_pos = (e->write_pos + 1) % e->buf_size;

    return output;
}

void process_block(struct EchoState *e, int *input, int *output, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        output[i] = echo_process(e, input[i]);
    }
}

int compute_decay_time(int feedback, int threshold) {
    // How many echo repetitions until signal drops below threshold
    // Starting from amplitude 1000
    int amplitude = 1000;
    int count = 0;
    while (amplitude > threshold && count < 100) {
        amplitude = (amplitude * feedback) / 100;
        count = count + 1;
    }
    return count;
}

int sum_buffer(int *buf, int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        sum = sum + buf[i];
    }
    return sum;
}

int max_abs(int *buf, int len) {
    int m = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        int v = buf[i];
        if (v < 0) v = -v;
        if (v > m) m = v;
    }
    return m;
}

int main() {
    struct EchoState echo;
    echo_init(&echo, 4, 50, 50, 100);

    // Process an impulse (single sample = 1000, rest = 0)
    int input[16];
    int output[16];
    int i;

    input[0] = 1000;
    for (i = 1; i < 16; i = i + 1) {
        input[i] = 0;
    }

    process_block(&echo, input, output, 16);

    printf("Echo impulse response:\n");
    // EXPECT: Echo impulse response:
    for (i = 0; i < 16; i = i + 1) {
        printf("  [%d] = %d\n", i, output[i]);
    }
    // EXPECT:   [0] = 1000
    // EXPECT:   [1] = 0
    // EXPECT:   [2] = 0
    // EXPECT:   [3] = 0
    // EXPECT:   [4] = 500
    // EXPECT:   [5] = 0
    // EXPECT:   [6] = 0
    // EXPECT:   [7] = 0
    // EXPECT:   [8] = 250
    // EXPECT:   [9] = 0
    // EXPECT:   [10] = 0
    // EXPECT:   [11] = 0
    // EXPECT:   [12] = 125
    // EXPECT:   [13] = 0
    // EXPECT:   [14] = 0
    // EXPECT:   [15] = 0

    // Max absolute value
    int peak = max_abs(output, 16);
    printf("Peak: %d\n", peak);
    // EXPECT: Peak: 1000

    // Sum of output
    int total = sum_buffer(output, 16);
    printf("Total energy: %d\n", total);
    // EXPECT: Total energy: 1875

    // Decay time calculation
    int dt1 = compute_decay_time(50, 10);
    printf("Decay at 50%% fb: %d repeats\n", dt1);
    // EXPECT: Decay at 50% fb: 7 repeats

    int dt2 = compute_decay_time(75, 10);
    printf("Decay at 75%% fb: %d repeats\n", dt2);
    // EXPECT: Decay at 75% fb: 16 repeats

    int dt3 = compute_decay_time(90, 10);
    printf("Decay at 90%% fb: %d repeats\n", dt3);
    // EXPECT: Decay at 90% fb: 40 repeats

    // Process a step function (constant signal)
    struct EchoState echo2;
    echo_init(&echo2, 3, 40, 30, 70);

    int step_in[12];
    int step_out[12];
    for (i = 0; i < 12; i = i + 1) {
        step_in[i] = 100;
    }
    process_block(&echo2, step_in, step_out, 12);

    printf("Step response: ");
    for (i = 0; i < 12; i = i + 1) {
        printf("%d", step_out[i]);
        if (i < 11) printf(" ");
    }
    printf("\n");
    // EXPECT: Step response: 70 70 70 100 100 100 112 112 112 116 116 116

    return 0;
}
