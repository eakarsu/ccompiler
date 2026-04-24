int printf(const char *fmt, ...);

// Test 018: Image downscale (6x6 -> 3x3) and upscale (3x3 -> 6x6) using nearest neighbor and averaging

void downscale_avg(int src[36], int dst[9], int sw, int sh) {
    // Each 2x2 block in src maps to one pixel in dst (3x3)
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            int si;
            int sj;
            int sum;
            si = i * 2;
            sj = j * 2;
            sum = src[si * sw + sj]
                + src[si * sw + sj + 1]
                + src[(si + 1) * sw + sj]
                + src[(si + 1) * sw + sj + 1];
            dst[i * 3 + j] = sum / 4;
        }
    }
}

void upscale_nearest(int src[9], int dst[36], int dw, int dh) {
    // Each pixel in src maps to a 2x2 block in dst
    int i;
    int j;
    for (i = 0; i < dh; i++) {
        for (j = 0; j < dw; j++) {
            int si;
            int sj;
            si = i / 2;
            sj = j / 2;
            dst[i * dw + j] = src[si * 3 + sj];
        }
    }
}

void upscale_bilinear_int(int src[9], int dst[36], int dw, int dh) {
    // Integer approximation of bilinear interpolation
    // Map destination pixel to source position using fixed-point (x256)
    int i;
    int j;
    for (i = 0; i < dh; i++) {
        for (j = 0; j < dw; j++) {
            int sx256;
            int sy256;
            int sx;
            int sy;
            int fx;
            int fy;
            int v00;
            int v01;
            int v10;
            int v11;
            // Map dest coords to source coords (fixed point x256)
            sy256 = i * 256 * 2 / (dh - 1);
            sx256 = j * 256 * 2 / (dw - 1);
            sy = sy256 / 256;
            sx = sx256 / 256;
            fy = sy256 - sy * 256;
            fx = sx256 - sx * 256;
            if (sy > 2) sy = 2;
            if (sx > 2) sx = 2;
            v00 = src[sy * 3 + sx];
            v01 = (sx + 1 < 3) ? src[sy * 3 + sx + 1] : v00;
            v10 = (sy + 1 < 3) ? src[(sy + 1) * 3 + sx] : v00;
            v11 = (sy + 1 < 3 && sx + 1 < 3) ? src[(sy + 1) * 3 + sx + 1] : v00;
            // Bilinear: (1-fy)(1-fx)*v00 + (1-fy)*fx*v01 + fy*(1-fx)*v10 + fy*fx*v11
            // In fixed point /256
            dst[i * dw + j] = ((256 - fy) * (256 - fx) * v00
                             + (256 - fy) * fx * v01
                             + fy * (256 - fx) * v10
                             + fy * fx * v11) / (256 * 256);
        }
    }
}

int compute_sum(int img[36], int size) {
    int i;
    int s;
    s = 0;
    for (i = 0; i < size; i++) s = s + img[i];
    return s;
}

int compute_mean(int img[36], int size) {
    return compute_sum(img, size) / size;
}

int max_abs_diff(int a[36], int b[36], int size) {
    int i;
    int mx;
    mx = 0;
    for (i = 0; i < size; i++) {
        int d;
        d = a[i] - b[i];
        if (d < 0) d = -d;
        if (d > mx) mx = d;
    }
    return mx;
}

int main() {
    int img[36];
    int small[9];
    int up_nn[36];
    int up_bl[36];
    int r6[6];
    int r3[3];
    int i;
    int j;

    // Create 6x6 image with a gradient pattern
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            img[i * 6 + j] = (i * 40) + (j * 10);
        }
    }

    printf("Input row 0:\n"); // EXPECT: Input row 0:
    for (j = 0; j < 6; j++) r6[j] = img[j];
    printf("%d %d %d %d %d %d\n", r6[0], r6[1], r6[2], r6[3], r6[4], r6[5]); // EXPECT: 0 10 20 30 40 50

    printf("Input row 5:\n"); // EXPECT: Input row 5:
    for (j = 0; j < 6; j++) r6[j] = img[5*6+j];
    printf("%d %d %d %d %d %d\n", r6[0], r6[1], r6[2], r6[3], r6[4], r6[5]); // EXPECT: 200 210 220 230 240 250

    printf("Input mean: %d\n", compute_mean(img, 36)); // EXPECT: Input mean: 125

    downscale_avg(img, small, 6, 6);

    printf("Downscaled 3x3:\n"); // EXPECT: Downscaled 3x3:
    for (j = 0; j < 3; j++) r3[j] = small[j];
    printf("%d %d %d\n", r3[0], r3[1], r3[2]); // EXPECT: 25 45 65
    for (j = 0; j < 3; j++) r3[j] = small[3+j];
    printf("%d %d %d\n", r3[0], r3[1], r3[2]); // EXPECT: 105 125 145
    for (j = 0; j < 3; j++) r3[j] = small[6+j];
    printf("%d %d %d\n", r3[0], r3[1], r3[2]); // EXPECT: 185 205 225

    upscale_nearest(small, up_nn, 6, 6);

    printf("Upscaled NN row 0:\n"); // EXPECT: Upscaled NN row 0:
    for (j = 0; j < 6; j++) r6[j] = up_nn[j];
    printf("%d %d %d %d %d %d\n", r6[0], r6[1], r6[2], r6[3], r6[4], r6[5]); // EXPECT: 25 25 45 45 65 65

    printf("Upscaled NN row 5:\n"); // EXPECT: Upscaled NN row 5:
    for (j = 0; j < 6; j++) r6[j] = up_nn[5*6+j];
    printf("%d %d %d %d %d %d\n", r6[0], r6[1], r6[2], r6[3], r6[4], r6[5]); // EXPECT: 185 185 205 205 225 225

    upscale_bilinear_int(small, up_bl, 6, 6);

    printf("Upscaled BL row 0:\n"); // EXPECT: Upscaled BL row 0:
    for (j = 0; j < 6; j++) r6[j] = up_bl[j];
    printf("%d %d %d %d %d %d\n", r6[0], r6[1], r6[2], r6[3], r6[4], r6[5]); // EXPECT: 25 32 40 48 56 65

    printf("Upscaled BL row 5:\n"); // EXPECT: Upscaled BL row 5:
    for (j = 0; j < 6; j++) r6[j] = up_bl[5*6+j];
    printf("%d %d %d %d %d %d\n", r6[0], r6[1], r6[2], r6[3], r6[4], r6[5]); // EXPECT: 185 192 200 208 216 225

    printf("NN mean: %d\n", compute_mean(up_nn, 36)); // EXPECT: NN mean: 125
    printf("BL mean: %d\n", compute_mean(up_bl, 36)); // EXPECT: BL mean: 124
    printf("Max diff NN vs orig: %d\n", max_abs_diff(img, up_nn, 36)); // EXPECT: Max diff NN vs orig: 25
    printf("Max diff BL vs orig: %d\n", max_abs_diff(img, up_bl, 36)); // EXPECT: Max diff BL vs orig: 25

    return 0;
}
