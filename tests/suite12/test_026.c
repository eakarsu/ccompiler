int printf(const char *fmt, ...);

// Beat Detector
// Detects beats in an audio signal by analyzing energy levels
// Uses integer math throughout

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int compute_energy(int *samples, int start, int length) {
    int sum = 0;
    int i;
    for (i = start; i < start + length; i = i + 1) {
        int s = samples[i];
        // Use absolute value to approximate energy
        sum = sum + abs_val(s);
    }
    return sum;
}

int compute_avg_energy(int *samples, int total_len, int window_size) {
    int num_windows = total_len / window_size;
    int total = 0;
    int i;
    for (i = 0; i < num_windows; i = i + 1) {
        total = total + compute_energy(samples, i * window_size, window_size);
    }
    if (num_windows == 0) return 0;
    return total / num_windows;
}

struct BeatInfo {
    int position;
    int energy;
    int is_strong;
};

int detect_beats(int *samples, int total_len, int window_size, struct BeatInfo *beats) {
    int num_windows = total_len / window_size;
    int avg = compute_avg_energy(samples, total_len, window_size);
    int threshold = avg + avg / 2;  // 1.5x average
    int beat_count = 0;
    int i;

    for (i = 0; i < num_windows; i = i + 1) {
        int energy = compute_energy(samples, i * window_size, window_size);
        if (energy > threshold) {
            beats[beat_count].position = i * window_size;
            beats[beat_count].energy = energy;
            if (energy > avg * 2) {
                beats[beat_count].is_strong = 1;
            } else {
                beats[beat_count].is_strong = 0;
            }
            beat_count = beat_count + 1;
        }
    }
    return beat_count;
}

int estimate_bpm(struct BeatInfo *beats, int beat_count, int sample_rate) {
    // Estimate BPM from average distance between beats
    if (beat_count < 2) return 0;
    int total_dist = 0;
    int i;
    for (i = 1; i < beat_count; i = i + 1) {
        total_dist = total_dist + (beats[i].position - beats[i - 1].position);
    }
    int avg_dist = total_dist / (beat_count - 1);
    if (avg_dist == 0) return 0;
    // BPM = 60 * sample_rate / avg_distance
    return (60 * sample_rate) / avg_dist;
}

int peak_energy_position(int *samples, int total_len, int window_size) {
    int num_windows = total_len / window_size;
    int max_energy = 0;
    int max_pos = 0;
    int i;
    for (i = 0; i < num_windows; i = i + 1) {
        int energy = compute_energy(samples, i * window_size, window_size);
        if (energy > max_energy) {
            max_energy = energy;
            max_pos = i * window_size;
        }
    }
    return max_pos;
}

int is_silence(int *samples, int start, int length, int threshold) {
    int energy = compute_energy(samples, start, length);
    return energy < threshold;
}

int main() {
    // Create a signal with periodic beats
    // Simulate a kick drum pattern: loud at beats, quiet between
    int signal[64];
    int i;
    for (i = 0; i < 64; i = i + 1) {
        signal[i] = 5;  // quiet baseline
    }
    // Add beats at positions 0, 16, 32, 48
    signal[0] = 100; signal[1] = 80; signal[2] = 60; signal[3] = 40;
    signal[16] = 90; signal[17] = 70; signal[18] = 50; signal[19] = 30;
    signal[32] = 100; signal[33] = 80; signal[34] = 60; signal[35] = 40;
    signal[48] = 90; signal[49] = 70; signal[50] = 50; signal[51] = 30;

    // Energy of first window (4 samples)
    int e1 = compute_energy(signal, 0, 4);
    printf("Window 0 energy: %d\n", e1);
    // EXPECT: Window 0 energy: 280

    // Energy of a quiet window
    int e2 = compute_energy(signal, 8, 4);
    printf("Window 2 energy: %d\n", e2);
    // EXPECT: Window 2 energy: 20

    // Average energy
    int avg = compute_avg_energy(signal, 64, 4);
    printf("Average energy: %d\n", avg);
    // EXPECT: Average energy: 80

    // Detect beats
    struct BeatInfo beats[16];
    int beat_count = detect_beats(signal, 64, 4, beats);
    printf("Beats detected: %d\n", beat_count);
    // EXPECT: Beats detected: 4

    // Print beat positions
    printf("Beat positions:");
    for (i = 0; i < beat_count; i = i + 1) {
        printf(" %d", beats[i].position);
    }
    printf("\n");
    // EXPECT: Beat positions: 0 16 32 48

    // Strong beats
    int strong = 0;
    for (i = 0; i < beat_count; i = i + 1) {
        if (beats[i].is_strong) strong = strong + 1;
    }
    printf("Strong beats: %d\n", strong);
    // EXPECT: Strong beats: 4

    // BPM estimation (sample_rate = 1000)
    int bpm = estimate_bpm(beats, beat_count, 1000);
    printf("Estimated BPM: %d\n", bpm);
    // EXPECT: Estimated BPM: 3750

    // Peak energy position
    int peak_pos = peak_energy_position(signal, 64, 4);
    printf("Peak at: %d\n", peak_pos);
    // EXPECT: Peak at: 0

    // Silence detection
    int sil1 = is_silence(signal, 8, 4, 50);
    printf("Window 2 silent: %d\n", sil1);
    // EXPECT: Window 2 silent: 1

    int sil2 = is_silence(signal, 0, 4, 50);
    printf("Window 0 silent: %d\n", sil2);
    // EXPECT: Window 0 silent: 0

    // Test with no beats
    int quiet[16];
    for (i = 0; i < 16; i = i + 1) quiet[i] = 1;
    struct BeatInfo no_beats[4];
    int nb = detect_beats(quiet, 16, 4, no_beats);
    printf("Quiet beats: %d\n", nb);
    // EXPECT: Quiet beats: 0

    return 0;
}
