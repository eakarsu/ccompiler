int printf(const char *fmt, ...);

// Test 013: Histogram computation and equalization on integer pixel grid

void compute_histogram(int img[36], int size, int hist[8]) {
    int i;
    for (i = 0; i < 8; i++) hist[i] = 0;
    for (i = 0; i < size; i++) {
        int bin;
        bin = img[i] / 32;
        if (bin > 7) bin = 7;
        if (bin < 0) bin = 0;
        hist[bin] = hist[bin] + 1;
    }
}

void compute_cdf(int hist[8], int cdf[8], int nbins) {
    int i;
    cdf[0] = hist[0];
    for (i = 1; i < nbins; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }
}

int find_cdf_min(int cdf[8], int nbins) {
    int i;
    for (i = 0; i < nbins; i++) {
        if (cdf[i] > 0) return cdf[i];
    }
    return 0;
}

void equalize(int img[36], int out[36], int size, int cdf[8]) {
    int cdf_min;
    int i;
    cdf_min = find_cdf_min(cdf, 8);
    for (i = 0; i < size; i++) {
        int bin;
        int val;
        bin = img[i] / 32;
        if (bin > 7) bin = 7;
        if (bin < 0) bin = 0;
        if (size - cdf_min == 0) {
            val = img[i];
        } else {
            val = (cdf[bin] - cdf_min) * 255 / (size - cdf_min);
        }
        if (val < 0) val = 0;
        if (val > 255) val = 255;
        out[i] = val;
    }
}

int compute_mean(int img[36], int size) {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < size; i++) {
        sum = sum + img[i];
    }
    return sum / size;
}

int compute_variance(int img[36], int size) {
    int mean;
    int i;
    int var;
    mean = compute_mean(img, size);
    var = 0;
    for (i = 0; i < size; i++) {
        int d;
        d = img[i] - mean;
        var = var + d * d;
    }
    return var / size;
}

int compute_max(int img[36], int size) {
    int i;
    int mx;
    mx = img[0];
    for (i = 1; i < size; i++) {
        if (img[i] > mx) mx = img[i];
    }
    return mx;
}

int compute_min(int img[36], int size) {
    int i;
    int mn;
    mn = img[0];
    for (i = 1; i < size; i++) {
        if (img[i] < mn) mn = img[i];
    }
    return mn;
}

int main() {
    int img[36];
    int out[36];
    int hist[8];
    int cdf[8];
    int i;

    // Create a low-contrast image: values in range 40-120
    img[0] = 40; img[1] = 45; img[2] = 50; img[3] = 55; img[4] = 60; img[5] = 65;
    img[6] = 70; img[7] = 75; img[8] = 80; img[9] = 85; img[10] = 90; img[11] = 95;
    img[12] = 100; img[13] = 105; img[14] = 110; img[15] = 115; img[16] = 120; img[17] = 80;
    img[18] = 50; img[19] = 60; img[20] = 70; img[21] = 80; img[22] = 90; img[23] = 100;
    img[24] = 55; img[25] = 65; img[26] = 75; img[27] = 85; img[28] = 95; img[29] = 105;
    img[30] = 45; img[31] = 55; img[32] = 65; img[33] = 75; img[34] = 85; img[35] = 95;

    printf("Input mean: %d\n", compute_mean(img, 36)); // EXPECT: Input mean: 77
    printf("Input variance: %d\n", compute_variance(img, 36)); // EXPECT: Input variance: 444
    printf("Input min: %d\n", compute_min(img, 36)); // EXPECT: Input min: 40
    printf("Input max: %d\n", compute_max(img, 36)); // EXPECT: Input max: 120

    compute_histogram(img, 36, hist);
    printf("Histogram:\n"); // EXPECT: Histogram:
    printf("Bin 0: %d\n", hist[0]); // EXPECT: Bin 0: 0
    printf("Bin 1: %d\n", hist[1]); // EXPECT: Bin 1: 10
    printf("Bin 2: %d\n", hist[2]); // EXPECT: Bin 2: 19
    printf("Bin 3: %d\n", hist[3]); // EXPECT: Bin 3: 7
    printf("Bin 4: %d\n", hist[4]); // EXPECT: Bin 4: 0
    printf("Bin 5: %d\n", hist[5]); // EXPECT: Bin 5: 0
    printf("Bin 6: %d\n", hist[6]); // EXPECT: Bin 6: 0
    printf("Bin 7: %d\n", hist[7]); // EXPECT: Bin 7: 0

    compute_cdf(hist, cdf, 8);
    printf("CDF:\n"); // EXPECT: CDF:
    printf("CDF 0: %d\n", cdf[0]); // EXPECT: CDF 0: 0
    printf("CDF 1: %d\n", cdf[1]); // EXPECT: CDF 1: 10
    printf("CDF 2: %d\n", cdf[2]); // EXPECT: CDF 2: 29
    printf("CDF 3: %d\n", cdf[3]); // EXPECT: CDF 3: 36
    printf("CDF 7: %d\n", cdf[7]); // EXPECT: CDF 7: 36

    equalize(img, out, 36, cdf);

    printf("Equalized mean: %d\n", compute_mean(out, 36)); // EXPECT: Equalized mean: 147
    printf("Equalized min: %d\n", compute_min(out, 36)); // EXPECT: Equalized min: 0
    printf("Equalized max: %d\n", compute_max(out, 36)); // EXPECT: Equalized max: 255

    printf("Sample equalized values:\n"); // EXPECT: Sample equalized values:
    printf("out[0]: %d\n", out[0]); // EXPECT: out[0]: 0
    printf("out[6]: %d\n", out[6]); // EXPECT: out[6]: 186
    printf("out[12]: %d\n", out[12]); // EXPECT: out[12]: 255
    printf("out[35]: %d\n", out[35]); // EXPECT: out[35]: 186

    // Verify histogram of equalized image is more spread out
    compute_histogram(out, 36, hist);
    printf("Equalized histogram:\n"); // EXPECT: Equalized histogram:
    printf("EqBin 0: %d\n", hist[0]); // EXPECT: EqBin 0: 10
    printf("EqBin 3: %d\n", hist[3]); // EXPECT: EqBin 3: 0
    printf("EqBin 7: %d\n", hist[7]); // EXPECT: EqBin 7: 7

    return 0;
}
