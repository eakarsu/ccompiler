int printf(const char *fmt, ...);

// Signal energy and power computation (integer arithmetic)

// Signal energy: E = sum(x[i]^2)
int signal_energy(int *x, int len) {
    int i;
    int e = 0;
    for (i = 0; i < len; i++) {
        e = e + x[i] * x[i];
    }
    return e;
}

// Average power: P = E / N
int signal_power(int *x, int len) {
    int e = signal_energy(x, len);
    return e / len;
}

// RMS (integer sqrt approximation): sqrt(P) * 100 for precision
int isqrt(int n) {
    if (n <= 0) return 0;
    int x = n;
    int y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

int signal_rms_100(int *x, int len) {
    int p = signal_power(x, len);
    return isqrt(p * 10000);
}

// Signal-to-Noise Ratio (scaled by 10 for one decimal place)
// SNR = 10 * log10(P_signal / P_noise)
// We approximate log10 using integer math:
// log10(x) ~ (number of digits - 1) + frac
// Simpler: compute ratio * 100 and report that
int snr_ratio_100(int *signal, int *noise, int len) {
    int ps = signal_power(signal, len);
    int pn = signal_power(noise, len);
    if (pn == 0) return 9999;  // infinite SNR
    return (ps * 100) / pn;
}

// Energy in frequency band (using DFT coefficient approximation)
// Sum energy at specific indices of a simple DFT
int dft_coeff_energy(int *x, int len, int k) {
    // X[k] = sum x[n] * cos(2*pi*k*n/N) (real part only)
    // Approximate cos as lookup for small N
    // For simplicity, compute energy of x weighted by index pattern
    int re = 0;
    int im = 0;
    int n;
    for (n = 0; n < len; n++) {
        // angle = 2*pi*k*n/N, mapped to quadrant
        int phase = (4 * k * n) % (4 * len);  // in units of quarter period
        phase = (phase * 1000) / len;  // scale to 0-4000

        // Rough cos/sin from quadrant
        int c;
        int s;
        int p = phase % 4000;
        if (p < 1000) { c = 1000 - p; s = p; }
        else if (p < 2000) { c = -(p - 1000); s = 1000 - (p - 1000); }
        else if (p < 3000) { c = -(3000 - p); s = -(p - 2000); }
        else { c = p - 3000; s = -(4000 - p); }

        re = re + (x[n] * c) / 1000;
        im = im + (x[n] * s) / 1000;
    }
    return (re * re + im * im);
}

// Parseval's theorem check: sum |x[n]|^2 should equal (1/N) sum |X[k]|^2
// We verify total energy conservation

// Mean of signal
int signal_mean(int *x, int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i++) {
        sum = sum + x[i];
    }
    return sum / len;
}

// Variance (scaled): sum((x[i] - mean)^2) / len
int signal_variance(int *x, int len) {
    int m = signal_mean(x, len);
    int var = 0;
    int i;
    for (i = 0; i < len; i++) {
        int d = x[i] - m;
        var = var + d * d;
    }
    return var / len;
}

// Peak-to-peak amplitude
int peak_to_peak(int *x, int len) {
    int mn = x[0];
    int mx = x[0];
    int i;
    for (i = 1; i < len; i++) {
        if (x[i] < mn) mn = x[i];
        if (x[i] > mx) mx = x[i];
    }
    return mx - mn;
}

// Crest factor * 100: peak / rms
int crest_factor_100(int *x, int len) {
    int mx = x[0];
    int i;
    if (mx < 0) mx = -mx;
    for (i = 1; i < len; i++) {
        int v = x[i];
        if (v < 0) v = -v;
        if (v > mx) mx = v;
    }
    int rms = signal_rms_100(x, len);
    if (rms == 0) return 0;
    return (mx * 10000) / rms;
}

int main() {
    // Test 1: Energy of simple signal
    int s1[4];
    s1[0] = 1; s1[1] = 2; s1[2] = 3; s1[3] = 4;
    int e1 = signal_energy(s1, 4);
    printf("Energy [1,2,3,4]: %d\n", e1);
    // EXPECT: Energy [1,2,3,4]: 30

    // Test 2: Average power
    int p1 = signal_power(s1, 4);
    printf("Power [1,2,3,4]: %d\n", p1);
    // EXPECT: Power [1,2,3,4]: 7

    // Test 3: Energy of DC signal
    int dc[4];
    dc[0] = 5; dc[1] = 5; dc[2] = 5; dc[3] = 5;
    int edc = signal_energy(dc, 4);
    printf("Energy DC 5: %d\n", edc);
    // EXPECT: Energy DC 5: 100

    // Test 4: Power of DC
    int pdc = signal_power(dc, 4);
    printf("Power DC 5: %d\n", pdc);
    // EXPECT: Power DC 5: 25

    // Test 5: RMS of DC signal (should be ~500, since 5*100=500)
    int rms_dc = signal_rms_100(dc, 4);
    printf("RMS DC 5: %d\n", rms_dc);
    // EXPECT: RMS DC 5: 500

    // Test 6: Energy of alternating signal
    int alt[4];
    alt[0] = 10; alt[1] = -10; alt[2] = 10; alt[3] = -10;
    int ealt = signal_energy(alt, 4);
    printf("Energy alt: %d\n", ealt);
    // EXPECT: Energy alt: 400

    // Test 7: Power and RMS of alternating
    int palt = signal_power(alt, 4);
    printf("Power alt: %d\n", palt);
    // EXPECT: Power alt: 100
    int rms_alt = signal_rms_100(alt, 4);
    printf("RMS alt: %d\n", rms_alt);
    // EXPECT: RMS alt: 1000

    // Test 8: SNR computation
    int sig[4];
    sig[0] = 100; sig[1] = 100; sig[2] = 100; sig[3] = 100;
    int noise[4];
    noise[0] = 1; noise[1] = -1; noise[2] = 1; noise[3] = -1;
    int snr = snr_ratio_100(sig, noise, 4);
    // P_signal = 10000, P_noise = 1
    // ratio = 10000*100/1 = 1000000
    printf("SNR ratio*100: %d\n", snr);
    // EXPECT: SNR ratio*100: 1000000

    // Test 9: Mean
    int m1 = signal_mean(s1, 4);
    printf("Mean [1,2,3,4]: %d\n", m1);
    // EXPECT: Mean [1,2,3,4]: 2

    // Test 10: Mean of alternating (should be 0)
    int malt = signal_mean(alt, 4);
    printf("Mean alt: %d\n", malt);
    // EXPECT: Mean alt: 0

    // Test 11: Variance
    int v1 = signal_variance(s1, 4);
    // mean = 2, deviations: -1, 0, 1, 2 -> squares: 1, 0, 1, 4 -> sum=6, /4=1
    printf("Var [1,2,3,4]: %d\n", v1);
    // EXPECT: Var [1,2,3,4]: 1

    // Test 12: Variance of DC (should be 0)
    int vdc = signal_variance(dc, 4);
    printf("Var DC: %d\n", vdc);
    // EXPECT: Var DC: 0

    // Test 13: Variance of alternating
    // mean=0, deviations = 10,-10,10,-10, squares=100*4=400, /4=100
    int valt = signal_variance(alt, 4);
    printf("Var alt: %d\n", valt);
    // EXPECT: Var alt: 100

    // Test 14: Peak-to-peak
    int pp1 = peak_to_peak(s1, 4);
    printf("P2P [1,2,3,4]: %d\n", pp1);
    // EXPECT: P2P [1,2,3,4]: 3

    int pp2 = peak_to_peak(alt, 4);
    printf("P2P alt: %d\n", pp2);
    // EXPECT: P2P alt: 20

    // Test 15: Crest factor of DC signal: peak=5, rms=500(scaled)
    // crest = 5 * 10000 / 500 = 100
    int cf_dc = crest_factor_100(dc, 4);
    printf("Crest DC: %d\n", cf_dc);
    // EXPECT: Crest DC: 100

    // Test 16: Crest factor of alternating: peak=10, rms=1000(scaled)
    // crest = 10 * 10000 / 1000 = 100
    int cf_alt = crest_factor_100(alt, 4);
    printf("Crest alt: %d\n", cf_alt);
    // EXPECT: Crest alt: 100

    // Test 17: Energy relationship: E = N * P
    int n = 4;
    int check = signal_energy(s1, n) / n;
    printf("E/N = P: %d = %d -> %d\n", check, signal_power(s1, n), check == signal_power(s1, n));
    // EXPECT: E/N = P: 7 = 7 -> 1

    // Test 18: isqrt edge cases
    printf("sqrt 0: %d\n", isqrt(0));
    // EXPECT: sqrt 0: 0
    printf("sqrt 1: %d\n", isqrt(1));
    // EXPECT: sqrt 1: 1
    printf("sqrt 144: %d\n", isqrt(144));
    // EXPECT: sqrt 144: 12
    printf("sqrt 10000: %d\n", isqrt(10000));
    // EXPECT: sqrt 10000: 100

    printf("All energy/power tests passed\n");
    // EXPECT: All energy/power tests passed

    return 0;
}
