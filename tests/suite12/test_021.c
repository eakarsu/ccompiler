int printf(const char *fmt, ...);

// Waveform Generator (square, triangle, sawtooth)
// Uses integer math with fixed-point scaling (scale factor 1000)

int square_wave(int phase, int period, int amplitude) {
    // phase is 0..period-1
    // output: +amplitude for first half, -amplitude for second half
    if (phase < period / 2) {
        return amplitude;
    }
    return -amplitude;
}

int triangle_wave(int phase, int period, int amplitude) {
    // Rising from -amplitude to +amplitude in first half
    // Falling from +amplitude to -amplitude in second half
    int half = period / 2;
    int value;
    if (half == 0) return 0;
    if (phase < half) {
        // rising: -amplitude + 2*amplitude*phase/half
        value = -amplitude + (2 * amplitude * phase) / half;
    } else {
        // falling: amplitude - 2*amplitude*(phase-half)/half
        value = amplitude - (2 * amplitude * (phase - half)) / half;
    }
    return value;
}

int sawtooth_wave(int phase, int period, int amplitude) {
    // Linear ramp from -amplitude to +amplitude over the period
    if (period == 0) return 0;
    return -amplitude + (2 * amplitude * phase) / period;
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

void generate_waveform(int wave_type, int period, int amplitude, int num_samples) {
    int i;
    int phase;
    int sample;
    for (i = 0; i < num_samples; i = i + 1) {
        phase = i % period;
        if (wave_type == 0) {
            sample = square_wave(phase, period, amplitude);
        } else if (wave_type == 1) {
            sample = triangle_wave(phase, period, amplitude);
        } else {
            sample = sawtooth_wave(phase, period, amplitude);
        }
        printf("%d", sample);
        if (i < num_samples - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int compute_rms(int wave_type, int period, int amplitude, int num_samples) {
    // Approximate RMS using sum of squares / N, return sum_sq/N
    int i;
    int phase;
    int sample;
    int sum_sq = 0;
    for (i = 0; i < num_samples; i = i + 1) {
        phase = i % period;
        if (wave_type == 0) {
            sample = square_wave(phase, period, amplitude);
        } else if (wave_type == 1) {
            sample = triangle_wave(phase, period, amplitude);
        } else {
            sample = sawtooth_wave(phase, period, amplitude);
        }
        sum_sq = sum_sq + (sample * sample) / num_samples;
    }
    return sum_sq;
}

int count_zero_crossings(int wave_type, int period, int amplitude, int num_samples) {
    int i;
    int phase;
    int prev = 0;
    int curr;
    int crossings = 0;
    for (i = 0; i < num_samples; i = i + 1) {
        phase = i % period;
        if (wave_type == 0) {
            curr = square_wave(phase, period, amplitude);
        } else if (wave_type == 1) {
            curr = triangle_wave(phase, period, amplitude);
        } else {
            curr = sawtooth_wave(phase, period, amplitude);
        }
        if (i > 0) {
            if ((prev >= 0 && curr < 0) || (prev < 0 && curr >= 0)) {
                crossings = crossings + 1;
            }
        }
        prev = curr;
    }
    return crossings;
}

int main() {
    // Test square wave with period=8, amplitude=100, 8 samples
    printf("Square wave: ");
    // EXPECT: Square wave: 100 100 100 100 -100 -100 -100 -100
    generate_waveform(0, 8, 100, 8);

    // Test triangle wave with period=8, amplitude=100, 8 samples
    printf("Triangle wave: ");
    // EXPECT: Triangle wave: -100 -50 0 50 100 50 0 -50
    generate_waveform(1, 8, 100, 8);

    // Test sawtooth wave with period=8, amplitude=100, 8 samples
    printf("Sawtooth wave: ");
    // EXPECT: Sawtooth wave: -100 -75 -50 -25 0 25 50 75
    generate_waveform(2, 8, 100, 8);

    // RMS energy of square wave (should be amplitude^2 = 10000)
    int rms_sq = compute_rms(0, 8, 100, 8);
    printf("Square RMS energy: %d\n", rms_sq);
    // EXPECT: Square RMS energy: 10000

    // RMS energy of triangle wave
    int rms_tri = compute_rms(1, 8, 100, 8);
    printf("Triangle RMS energy: %d\n", rms_tri);
    // EXPECT: Triangle RMS energy: 3748

    // RMS energy of sawtooth wave
    int rms_saw = compute_rms(2, 8, 100, 8);
    printf("Sawtooth RMS energy: %d\n", rms_saw);
    // EXPECT: Sawtooth RMS energy: 3436

    // Zero crossings for square wave over 16 samples (period=8)
    int zc_sq = count_zero_crossings(0, 8, 100, 16);
    printf("Square zero crossings: %d\n", zc_sq);
    // EXPECT: Square zero crossings: 3

    // Zero crossings for triangle wave over 16 samples
    int zc_tri = count_zero_crossings(1, 8, 100, 16);
    printf("Triangle zero crossings: %d\n", zc_tri);
    // EXPECT: Triangle zero crossings: 4

    // Two periods of square wave
    printf("Two periods: ");
    // EXPECT: Two periods: 100 100 -100 -100 100 100 -100 -100
    generate_waveform(0, 4, 100, 8);

    return 0;
}
