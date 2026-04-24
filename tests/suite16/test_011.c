int printf(const char *fmt, ...);

// Test 011: 2D Convolution with 3x3 box blur on a 5x5 image

int clamp(int val, int lo, int hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

void fill_image(int img[25], int w, int h) {
    int i;
    int j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            img[i * w + j] = (i + 1) * (j + 1) * 10;
        }
    }
}

void box_blur_3x3(int src[25], int dst[25], int w, int h) {
    int i;
    int j;
    int di;
    int dj;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            int sum;
            int count;
            sum = 0;
            count = 0;
            for (di = -1; di <= 1; di++) {
                for (dj = -1; dj <= 1; dj++) {
                    int ni;
                    int nj;
                    ni = i + di;
                    nj = j + dj;
                    if (ni >= 0 && ni < h && nj >= 0 && nj < w) {
                        sum = sum + src[ni * w + nj];
                        count = count + 1;
                    }
                }
            }
            dst[i * w + j] = sum / count;
        }
    }
}

void apply_sharpen(int src[25], int blur[25], int dst[25], int w, int h, int strength) {
    int i;
    for (i = 0; i < w * h; i++) {
        int val;
        val = src[i] + strength * (src[i] - blur[i]);
        dst[i] = clamp(val, 0, 255);
    }
}

int compute_sum(int img[25], int size) {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < size; i++) {
        sum = sum + img[i];
    }
    return sum;
}

int compute_max(int img[25], int size) {
    int i;
    int mx;
    mx = img[0];
    for (i = 1; i < size; i++) {
        if (img[i] > mx) mx = img[i];
    }
    return mx;
}

int compute_min(int img[25], int size) {
    int i;
    int mn;
    mn = img[0];
    for (i = 1; i < size; i++) {
        if (img[i] < mn) mn = img[i];
    }
    return mn;
}

int row_to_str(int img[25], int w, int row, int buf[5]) {
    int j;
    for (j = 0; j < w; j++) {
        buf[j] = img[row * w + j];
    }
    return 0;
}

int main() {
    int img[25];
    int blurred[25];
    int sharp[25];
    int r[5];

    fill_image(img, 5, 5);

    printf("Original:\n"); // EXPECT: Original:
    row_to_str(img, 5, 0, r);
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 10 20 30 40 50
    row_to_str(img, 5, 1, r);
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 20 40 60 80 100
    row_to_str(img, 5, 2, r);
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 30 60 90 120 150
    row_to_str(img, 5, 3, r);
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 40 80 120 160 200
    row_to_str(img, 5, 4, r);
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 50 100 150 200 250

    box_blur_3x3(img, blurred, 5, 5);

    printf("Blurred:\n"); // EXPECT: Blurred:
    row_to_str(blurred, 5, 0, r);
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 22 30 45 60 67
    row_to_str(blurred, 5, 1, r);
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 30 40 60 80 90
    row_to_str(blurred, 5, 2, r);
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 45 60 90 120 135
    row_to_str(blurred, 5, 3, r);
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 60 80 120 160 180
    row_to_str(blurred, 5, 4, r);
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 67 90 135 180 202

    apply_sharpen(img, blurred, sharp, 5, 5, 1);

    printf("Sharpened:\n"); // EXPECT: Sharpened:
    row_to_str(sharp, 5, 0, r);
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 0 10 15 20 33
    row_to_str(sharp, 5, 1, r);
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 10 40 60 80 110
    row_to_str(sharp, 5, 2, r);
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 15 60 90 120 165
    row_to_str(sharp, 5, 3, r);
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 20 80 120 160 220
    row_to_str(sharp, 5, 4, r);
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 33 110 165 220 255

    printf("Sum orig: %d\n", compute_sum(img, 25)); // EXPECT: Sum orig: 2250
    printf("Sum blur: %d\n", compute_sum(blurred, 25)); // EXPECT: Sum blur: 2248
    printf("Max orig: %d\n", compute_max(img, 25)); // EXPECT: Max orig: 250
    printf("Min orig: %d\n", compute_min(img, 25)); // EXPECT: Min orig: 10
    printf("Max blur: %d\n", compute_max(blurred, 25)); // EXPECT: Max blur: 202
    printf("Min blur: %d\n", compute_min(blurred, 25)); // EXPECT: Min blur: 22

    return 0;
}
