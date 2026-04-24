int printf(const char *fmt, ...);
// EXPECT: === Fourier Transform Simulation ===\nInput signal (square): 100 100 100 100 -100 -100 -100 -100 \nDFT coefficients:\n  k=0: real=0 imag=0 mag2=0\n  k=1: real=200 imag=-484 mag2=274256\n  k=2: real=0 imag=0 mag2=0\n  k=3: real=200 imag=-84 mag2=47056\n  k=4: real=0 imag=0 mag2=0\n  k=5: real=200 imag=84 mag2=47056\n  k=6: real=0 imag=0 mag2=0\n  k=7: real=200 imag=484 mag2=274256\nDominant frequency: k=1\nTotal power: 80328\nCosine signal: 100 0 -100 0 100 0 -100 0 \nDFT of cosine:\n  k=0: real=0 imag=0\n  k=1: real=0 imag=0\n  k=2: real=400 imag=0\n  k=3: real=0 imag=0\n  k=4: real=0 imag=0\n  k=5: real=0 imag=0\n  k=6: real=400 imag=0\n  k=7: real=0 imag=0\nDominant: k=2

struct FourierCoeff {
    int real;
    int imag;
};

struct Signal {
    int samples[16];
    int length;
};

int cos_table[16];
int sin_table[16];

void init_trig_tables(void) {
    cos_table[0] = 100;  cos_table[1] = 92;   cos_table[2] = 71;   cos_table[3] = 38;
    cos_table[4] = 0;    cos_table[5] = -38;   cos_table[6] = -71;  cos_table[7] = -92;
    cos_table[8] = -100; cos_table[9] = -92;   cos_table[10] = -71; cos_table[11] = -38;
    cos_table[12] = 0;   cos_table[13] = 38;   cos_table[14] = 71;  cos_table[15] = 92;

    sin_table[0] = 0;    sin_table[1] = 38;    sin_table[2] = 71;   sin_table[3] = 92;
    sin_table[4] = 100;  sin_table[5] = 92;    sin_table[6] = 71;   sin_table[7] = 38;
    sin_table[8] = 0;    sin_table[9] = -38;   sin_table[10] = -71; sin_table[11] = -92;
    sin_table[12] = -100;sin_table[13] = -92;  sin_table[14] = -71; sin_table[15] = -38;
}

int lookup_cos(int index, int n) {
    int idx = (index % n + n) % n;
    idx = idx * 16 / n;
    if (idx >= 16) idx = idx % 16;
    return cos_table[idx];
}

int lookup_sin(int index, int n) {
    int idx = (index % n + n) % n;
    idx = idx * 16 / n;
    if (idx >= 16) idx = idx % 16;
    return sin_table[idx];
}

void dft(struct Signal *s, struct FourierCoeff *coeffs) {
    int k, n;
    for (k = 0; k < s->length; k++) {
        int sum_real = 0;
        int sum_imag = 0;
        for (n = 0; n < s->length; n++) {
            int angle_idx = k * n;
            sum_real = sum_real + s->samples[n] * lookup_cos(angle_idx, s->length);
            sum_imag = sum_imag - s->samples[n] * lookup_sin(angle_idx, s->length);
        }
        coeffs[k].real = sum_real / 100;
        coeffs[k].imag = sum_imag / 100;
    }
}

int magnitude_sq(struct FourierCoeff *c) {
    return c->real * c->real + c->imag * c->imag;
}

int find_dominant_freq(struct FourierCoeff *coeffs, int n) {
    int max_mag = 0;
    int max_k = 0;
    int k;
    for (k = 1; k < n / 2; k++) {
        int mag = magnitude_sq(&coeffs[k]);
        if (mag > max_mag) {
            max_mag = mag;
            max_k = k;
        }
    }
    return max_k;
}

int compute_power(struct FourierCoeff *coeffs, int n) {
    int power = 0;
    int k;
    for (k = 0; k < n; k++) {
        power = power + magnitude_sq(&coeffs[k]);
    }
    return power / n;
}

int main(void) {
    init_trig_tables();

    printf("=== Fourier Transform Simulation ===\n");

    struct Signal s1;
    s1.length = 8;
    int i;
    for (i = 0; i < 8; i++) {
        s1.samples[i] = (i < 4) ? 100 : -100;
    }

    printf("Input signal (square): ");
    for (i = 0; i < s1.length; i++) {
        printf("%d ", s1.samples[i]);
    }
    printf("\n");

    struct FourierCoeff coeffs1[16];
    dft(&s1, coeffs1);

    printf("DFT coefficients:\n");
    for (i = 0; i < s1.length; i++) {
        printf("  k=%d: real=%d imag=%d mag2=%d\n",
               i, coeffs1[i].real, coeffs1[i].imag, magnitude_sq(&coeffs1[i]));
    }

    printf("Dominant frequency: k=%d\n", find_dominant_freq(coeffs1, s1.length));
    printf("Total power: %d\n", compute_power(coeffs1, s1.length));

    struct Signal s2;
    s2.length = 8;
    for (i = 0; i < 8; i++) {
        s2.samples[i] = lookup_cos(i * 2, 8);
    }

    printf("Cosine signal: ");
    for (i = 0; i < s2.length; i++) {
        printf("%d ", s2.samples[i]);
    }
    printf("\n");

    struct FourierCoeff coeffs2[16];
    dft(&s2, coeffs2);

    printf("DFT of cosine:\n");
    for (i = 0; i < s2.length; i++) {
        printf("  k=%d: real=%d imag=%d\n", i, coeffs2[i].real, coeffs2[i].imag);
    }
    printf("Dominant: k=%d\n", find_dominant_freq(coeffs2, s2.length));

    return 0;
}
