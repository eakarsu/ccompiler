int printf(const char *fmt, ...);

// Ring Modulator
// Multiplies two signals together (amplitude modulation)
// Produces sum and difference frequencies
// All integer math with scale factor 1000

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

// Simple integer sine approximation using a lookup table (one period = 16 samples)
// Values scaled to -1000..1000
int sine_table[16];

void init_sine_table() {
    // Approximate sine wave with 16 samples per period
    // sin(0), sin(22.5), sin(45), ... scaled by 1000
    sine_table[0] = 0;
    sine_table[1] = 383;
    sine_table[2] = 707;
    sine_table[3] = 924;
    sine_table[4] = 1000;
    sine_table[5] = 924;
    sine_table[6] = 707;
    sine_table[7] = 383;
    sine_table[8] = 0;
    sine_table[9] = -383;
    sine_table[10] = -707;
    sine_table[11] = -924;
    sine_table[12] = -1000;
    sine_table[13] = -924;
    sine_table[14] = -707;
    sine_table[15] = -383;
}

int get_sine(int phase) {
    // phase is in units where 16 = full period
    int idx = phase % 16;
    if (idx < 0) idx = idx + 16;
    return sine_table[idx];
}

int ring_mod(int sample_a, int sample_b) {
    // Multiply two samples, both in range -1000..1000
    // Result scaled back to -1000..1000
    return (sample_a * sample_b) / 1000;
}

void generate_tone(int *output, int period_samples, int amplitude, int num_samples) {
    int i;
    for (i = 0; i < num_samples; i = i + 1) {
        // Map i to phase within the sine table
        int phase = (i * 16) / period_samples;
        output[i] = (get_sine(phase % 16) * amplitude) / 1000;
    }
}

void ring_modulate(int *sig_a, int *sig_b, int *output, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        output[i] = (sig_a[i] * sig_b[i]) / 1000;
    }
}

int compute_dc_offset(int *signal, int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        sum = sum + signal[i];
    }
    return sum / len;
}

int compute_power(int *signal, int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        // Avoid overflow by dividing first
        sum = sum + (signal[i] * signal[i]) / len;
    }
    return sum;
}

int count_sign_changes(int *signal, int len) {
    int changes = 0;
    int i;
    for (i = 1; i < len; i = i + 1) {
        if ((signal[i] >= 0 && signal[i - 1] < 0) ||
            (signal[i] < 0 && signal[i - 1] >= 0)) {
            changes = changes + 1;
        }
    }
    return changes;
}

int main() {
    init_sine_table();

    // Generate carrier: period 16 samples, amplitude 1000
    int carrier[32];
    generate_tone(carrier, 16, 1000, 32);

    printf("Carrier: ");
    int i;
    for (i = 0; i < 16; i = i + 1) {
        printf("%d", carrier[i]);
        if (i < 15) printf(" ");
    }
    printf("\n");
    // EXPECT: Carrier: 0 383 707 924 1000 924 707 383 0 -383 -707 -924 -1000 -924 -707 -383

    // Generate modulator: period 32 samples (half frequency), amplitude 1000
    int modulator[32];
    generate_tone(modulator, 32, 1000, 32);

    printf("Modulator: ");
    for (i = 0; i < 16; i = i + 1) {
        printf("%d", modulator[i]);
        if (i < 15) printf(" ");
    }
    printf("\n");
    // EXPECT: Modulator: 0 0 383 383 707 707 924 924 1000 1000 924 924 707 707 383 383

    // Ring modulate
    int rm_out[32];
    ring_modulate(carrier, modulator, rm_out, 32);

    printf("Ring mod: ");
    for (i = 0; i < 16; i = i + 1) {
        printf("%d", rm_out[i]);
        if (i < 15) printf(" ");
    }
    printf("\n");
    // EXPECT: Ring mod: 0 0 270 353 707 653 653 353 0 -383 -653 -853 -707 -653 -270 -146

    // DC offset of ring modulated signal (should be near zero)
    int dc = compute_dc_offset(rm_out, 32);
    printf("RM DC offset: %d\n", dc);
    // EXPECT: RM DC offset: 0

    // Power of carrier
    int pow_c = compute_power(carrier, 16);
    printf("Carrier power: %d\n", pow_c);
    // EXPECT: Carrier power: 500076

    // Power of ring mod output
    int pow_rm = compute_power(rm_out, 16);
    printf("Ring mod power: %d\n", pow_rm);
    // EXPECT: Ring mod power: 249743

    // Sign changes in carrier (frequency indicator)
    int sc_carrier = count_sign_changes(carrier, 32);
    printf("Carrier sign changes: %d\n", sc_carrier);
    // EXPECT: Carrier sign changes: 3

    // Sign changes in ring mod output
    int sc_rm = count_sign_changes(rm_out, 32);
    printf("Ring mod sign changes: %d\n", sc_rm);
    // EXPECT: Ring mod sign changes: 4

    // Simple ring mod test: constant * sine
    int constant[16];
    for (i = 0; i < 16; i = i + 1) constant[i] = 500;
    int const_rm[16];
    ring_modulate(constant, carrier, const_rm, 16);

    printf("Const*sine: ");
    for (i = 0; i < 16; i = i + 1) {
        printf("%d", const_rm[i]);
        if (i < 15) printf(" ");
    }
    printf("\n");
    // EXPECT: Const*sine: 0 191 353 462 500 462 353 191 0 -191 -353 -462 -500 -462 -353 -191

    // Ring mod of signal with itself (squaring)
    int squared[16];
    ring_modulate(carrier, carrier, squared, 16);
    printf("Self-mod: ");
    for (i = 0; i < 8; i = i + 1) {
        printf("%d", squared[i]);
        if (i < 7) printf(" ");
    }
    printf("\n");
    // EXPECT: Self-mod: 0 146 499 853 1000 853 499 146

    return 0;
}
