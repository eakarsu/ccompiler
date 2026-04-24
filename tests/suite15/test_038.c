int printf(const char *fmt, ...);

// Peak detection in signal
// Find local maxima and minima with optional threshold

struct Peak {
    int index;
    int value;
    int is_max;  // 1 for max, 0 for min
};

// Find peaks (local maxima) in signal
// Returns number of peaks found
int find_peaks(int *sig, int len, struct Peak *peaks, int max_peaks) {
    int count = 0;
    int i;
    for (i = 1; i < len - 1; i++) {
        if (count >= max_peaks) break;
        if (sig[i] > sig[i - 1] && sig[i] > sig[i + 1]) {
            peaks[count].index = i;
            peaks[count].value = sig[i];
            peaks[count].is_max = 1;
            count = count + 1;
        }
    }
    return count;
}

// Find valleys (local minima)
int find_valleys(int *sig, int len, struct Peak *valleys, int max_v) {
    int count = 0;
    int i;
    for (i = 1; i < len - 1; i++) {
        if (count >= max_v) break;
        if (sig[i] < sig[i - 1] && sig[i] < sig[i + 1]) {
            valleys[count].index = i;
            valleys[count].value = sig[i];
            valleys[count].is_max = 0;
            count = count + 1;
        }
    }
    return count;
}

// Find peaks above a threshold
int find_peaks_threshold(int *sig, int len, struct Peak *peaks, int max_p, int threshold) {
    int count = 0;
    int i;
    for (i = 1; i < len - 1; i++) {
        if (count >= max_p) break;
        if (sig[i] > sig[i - 1] && sig[i] > sig[i + 1] && sig[i] >= threshold) {
            peaks[count].index = i;
            peaks[count].value = sig[i];
            peaks[count].is_max = 1;
            count = count + 1;
        }
    }
    return count;
}

// Find the maximum peak
int find_max_peak(int *sig, int len) {
    int max_val = sig[0];
    int max_idx = 0;
    int i;
    for (i = 1; i < len; i++) {
        if (sig[i] > max_val) {
            max_val = sig[i];
            max_idx = i;
        }
    }
    return max_idx;
}

// Peak prominence: how much a peak stands out from surrounding valleys
int peak_prominence(int *sig, int len, int peak_idx) {
    // Find lowest valley to the left
    int left_min = sig[peak_idx];
    int i;
    for (i = peak_idx - 1; i >= 0; i--) {
        if (sig[i] < left_min) left_min = sig[i];
    }
    // Find lowest valley to the right
    int right_min = sig[peak_idx];
    for (i = peak_idx + 1; i < len; i++) {
        if (sig[i] < right_min) right_min = sig[i];
    }
    // Prominence = peak - max(left_min, right_min)
    int ref = left_min;
    if (right_min > ref) ref = right_min;
    return sig[peak_idx] - ref;
}

// Count zero crossings in signal
int zero_crossings(int *sig, int len) {
    int count = 0;
    int i;
    for (i = 1; i < len; i++) {
        if ((sig[i - 1] > 0 && sig[i] <= 0) || (sig[i - 1] <= 0 && sig[i] > 0)) {
            count = count + 1;
        }
    }
    return count;
}

// Estimate period from peak positions
int estimate_period(struct Peak *peaks, int n_peaks) {
    if (n_peaks < 2) return 0;
    int total = 0;
    int i;
    for (i = 1; i < n_peaks; i++) {
        total = total + (peaks[i].index - peaks[i - 1].index);
    }
    return total / (n_peaks - 1);
}

int main() {
    // Test 1: Simple peak detection
    int sig1[9];
    sig1[0] = 0; sig1[1] = 3; sig1[2] = 1; sig1[3] = 5;
    sig1[4] = 2; sig1[5] = 4; sig1[6] = 0; sig1[7] = 2; sig1[8] = 0;
    struct Peak peaks[10];
    int np = find_peaks(sig1, 9, peaks, 10);
    printf("Peaks found: %d\n", np);
    // EXPECT: Peaks found: 4
    printf("Peak 0: idx=%d val=%d\n", peaks[0].index, peaks[0].value);
    // EXPECT: Peak 0: idx=1 val=3
    printf("Peak 1: idx=%d val=%d\n", peaks[1].index, peaks[1].value);
    // EXPECT: Peak 1: idx=3 val=5
    printf("Peak 2: idx=%d val=%d\n", peaks[2].index, peaks[2].value);
    // EXPECT: Peak 2: idx=5 val=4

    // Test 2: Valley detection
    struct Peak valleys[10];
    int nv = find_valleys(sig1, 9, valleys, 10);
    printf("Valleys found: %d\n", nv);
    // EXPECT: Valleys found: 3
    printf("Valley 0: idx=%d val=%d\n", valleys[0].index, valleys[0].value);
    // EXPECT: Valley 0: idx=2 val=1
    printf("Valley 1: idx=%d val=%d\n", valleys[1].index, valleys[1].value);
    // EXPECT: Valley 1: idx=4 val=2
    printf("Valley 2: idx=%d val=%d\n", valleys[2].index, valleys[2].value);
    // EXPECT: Valley 2: idx=6 val=0

    // Test 3: Threshold-based peak detection
    struct Peak tp[10];
    int ntp = find_peaks_threshold(sig1, 9, tp, 10, 4);
    printf("Peaks >= 4: %d\n", ntp);
    // EXPECT: Peaks >= 4: 2
    printf("Thresh peak 0: idx=%d val=%d\n", tp[0].index, tp[0].value);
    // EXPECT: Thresh peak 0: idx=3 val=5
    printf("Thresh peak 1: idx=%d val=%d\n", tp[1].index, tp[1].value);
    // EXPECT: Thresh peak 1: idx=5 val=4

    // Test 4: Global maximum
    int gmax = find_max_peak(sig1, 9);
    printf("Global max at: %d (val=%d)\n", gmax, sig1[gmax]);
    // EXPECT: Global max at: 3 (val=5)

    // Test 5: Peak prominence
    // Peak at index 3 (val=5): left_min=0, right_min=0 -> ref=0, prom=5
    int prom3 = peak_prominence(sig1, 9, 3);
    printf("Prom at 3: %d\n", prom3);
    // EXPECT: Prom at 3: 5

    // Peak at index 1 (val=3): left_min=0, right_min=0 -> ref=0, prom=3
    int prom1 = peak_prominence(sig1, 9, 1);
    printf("Prom at 1: %d\n", prom1);
    // EXPECT: Prom at 1: 3

    // Peak at index 5 (val=4): left_min=0, right_min=0 -> ref=0, prom=4
    int prom5 = peak_prominence(sig1, 9, 5);
    printf("Prom at 5: %d\n", prom5);
    // EXPECT: Prom at 5: 4

    // Test 6: Periodic signal peak detection
    int sig2[12];
    sig2[0] = 0; sig2[1] = 10; sig2[2] = 5;
    sig2[3] = 0; sig2[4] = 10; sig2[5] = 5;
    sig2[6] = 0; sig2[7] = 10; sig2[8] = 5;
    sig2[9] = 0; sig2[10] = 10; sig2[11] = 5;
    struct Peak ppeaks[10];
    int npp = find_peaks(sig2, 12, ppeaks, 10);
    printf("Periodic peaks: %d\n", npp);
    // EXPECT: Periodic peaks: 4
    // peaks at indices 1, 4, 7, 10
    printf("Period peaks: %d %d %d %d\n",
           ppeaks[0].index, ppeaks[1].index, ppeaks[2].index, ppeaks[3].index);
    // EXPECT: Period peaks: 1 4 7 10

    // Test 7: Period estimation
    int period = estimate_period(ppeaks, 4);
    printf("Est period: %d\n", period);
    // EXPECT: Est period: 3

    // Test 8: Zero crossings
    int sig3[8];
    sig3[0] = -2; sig3[1] = -1; sig3[2] = 1; sig3[3] = 3;
    sig3[4] = -1; sig3[5] = -2; sig3[6] = 2; sig3[7] = 0;
    int zc = zero_crossings(sig3, 8);
    // Crossings: -1 to 1 (idx 2), 3 to -1 (idx 4), -2 to 2 (idx 6), 2 to 0 (idx 7)
    printf("Zero crossings: %d\n", zc);
    // EXPECT: Zero crossings: 4

    // Test 9: Flat signal (no peaks)
    int flat[5];
    flat[0] = 5; flat[1] = 5; flat[2] = 5; flat[3] = 5; flat[4] = 5;
    struct Peak fp[10];
    int nfp = find_peaks(flat, 5, fp, 10);
    printf("Flat peaks: %d\n", nfp);
    // EXPECT: Flat peaks: 0

    // Test 10: Single peak
    int single[5];
    single[0] = 0; single[1] = 0; single[2] = 100; single[3] = 0; single[4] = 0;
    struct Peak sp[10];
    int nsp = find_peaks(single, 5, sp, 10);
    printf("Single peak: %d at %d\n", nsp, sp[0].index);
    // EXPECT: Single peak: 1 at 2

    printf("All peak detection tests passed\n");
    // EXPECT: All peak detection tests passed

    return 0;
}
