int printf(const char *fmt, ...);

// EXPECT: === Peak Detection ===
// Peak Detection in integer signals
// Finds local maxima and minima in a discrete signal

struct Peak {
    int index;
    int value;
    int is_max;
};

int find_peaks(int *signal, int len, struct Peak *peaks, int max_peaks) {
    int count = 0;
    int i;
    for (i = 1; i < len - 1; i++) {
        if (count >= max_peaks) break;
        if (signal[i] > signal[i - 1] && signal[i] > signal[i + 1]) {
            peaks[count].index = i;
            peaks[count].value = signal[i];
            peaks[count].is_max = 1;
            count++;
        } else if (signal[i] < signal[i - 1] && signal[i] < signal[i + 1]) {
            if (count >= max_peaks) break;
            peaks[count].index = i;
            peaks[count].value = signal[i];
            peaks[count].is_max = 0;
            count++;
        }
    }
    return count;
}

int find_peaks_threshold(int *signal, int len, struct Peak *peaks, int max_peaks, int threshold) {
    int count = 0;
    int i;
    for (i = 1; i < len - 1; i++) {
        if (count >= max_peaks) break;
        if (signal[i] > signal[i - 1] && signal[i] > signal[i + 1]) {
            if (signal[i] >= threshold) {
                peaks[count].index = i;
                peaks[count].value = signal[i];
                peaks[count].is_max = 1;
                count++;
            }
        }
    }
    return count;
}

int peak_to_peak(int *signal, int len) {
    int min_val = signal[0];
    int max_val = signal[0];
    int i;
    for (i = 1; i < len; i++) {
        if (signal[i] < min_val) min_val = signal[i];
        if (signal[i] > max_val) max_val = signal[i];
    }
    return max_val - min_val;
}

void test_simple_peaks(void) {
    int signal[10];
    struct Peak peaks[10];
    int count;
    int i;

    signal[0] = 1;
    signal[1] = 3;
    signal[2] = 2;
    signal[3] = 5;
    signal[4] = 1;
    signal[5] = 4;
    signal[6] = 2;
    signal[7] = 6;
    signal[8] = 3;
    signal[9] = 1;

    count = find_peaks(signal, 10, peaks, 10);
    printf("Simple peaks found: %d\n", count); // EXPECT: Simple peaks found: 7
    for (i = 0; i < count; i++) {
        if (peaks[i].is_max) {
            printf("  MAX at [%d] = %d\n", peaks[i].index, peaks[i].value);
        } else {
            printf("  MIN at [%d] = %d\n", peaks[i].index, peaks[i].value);
        }
    }
    // EXPECT:   MAX at [1] = 3
    // EXPECT:   MIN at [2] = 2
    // EXPECT:   MAX at [3] = 5
    // EXPECT:   MIN at [4] = 1
    // EXPECT:   MAX at [5] = 4
    // EXPECT:   MIN at [6] = 2
    // EXPECT:   MAX at [7] = 6
}

void test_threshold_peaks(void) {
    int signal[12];
    struct Peak peaks[10];
    int count;
    int i;

    signal[0] = 10;
    signal[1] = 50;
    signal[2] = 20;
    signal[3] = 80;
    signal[4] = 30;
    signal[5] = 40;
    signal[6] = 15;
    signal[7] = 90;
    signal[8] = 25;
    signal[9] = 60;
    signal[10] = 35;
    signal[11] = 10;

    count = find_peaks_threshold(signal, 12, peaks, 10, 55);
    printf("Peaks above threshold 55: %d\n", count); // EXPECT: Peaks above threshold 55: 3
    for (i = 0; i < count; i++) {
        printf("  Peak at [%d] = %d\n", peaks[i].index, peaks[i].value);
    }
    // EXPECT:   Peak at [3] = 80
    // EXPECT:   Peak at [7] = 90
    // EXPECT:   Peak at [9] = 60
}

void test_peak_to_peak(void) {
    int signal[8];
    int p2p;

    signal[0] = -20;
    signal[1] = 50;
    signal[2] = -10;
    signal[3] = 30;
    signal[4] = -40;
    signal[5] = 60;
    signal[6] = -5;
    signal[7] = 15;

    p2p = peak_to_peak(signal, 8);
    printf("Peak-to-peak amplitude: %d\n", p2p); // EXPECT: Peak-to-peak amplitude: 100
}

void test_monotonic(void) {
    int signal[5];
    struct Peak peaks[5];
    int count;

    signal[0] = 1; signal[1] = 2; signal[2] = 3; signal[3] = 4; signal[4] = 5;
    count = find_peaks(signal, 5, peaks, 5);
    printf("Monotonic signal peaks: %d\n", count); // EXPECT: Monotonic signal peaks: 0
}

int main(void) {
    printf("=== Peak Detection ===\n");
    test_simple_peaks();
    test_threshold_peaks();
    test_peak_to_peak();
    test_monotonic();
    printf("All peak detection tests passed.\n"); // EXPECT: All peak detection tests passed.
    return 0;
}
