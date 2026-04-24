int printf(const char *fmt, ...);

// Test 017: Gaussian-like weighted blur and Laplacian edge detection

int get_px(int img[36], int w, int h, int r, int c) {
    if (r < 0) r = 0;
    if (r >= h) r = h - 1;
    if (c < 0) c = 0;
    if (c >= w) c = w - 1;
    return img[r * w + c];
}

int clamp(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

// Gaussian-like 3x3 kernel: [1 2 1; 2 4 2; 1 2 1] / 16
void gaussian_blur(int src[36], int dst[36], int w, int h) {
    int i;
    int j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            int sum;
            sum = 1 * get_px(src, w, h, i-1, j-1)
                + 2 * get_px(src, w, h, i-1, j)
                + 1 * get_px(src, w, h, i-1, j+1)
                + 2 * get_px(src, w, h, i, j-1)
                + 4 * get_px(src, w, h, i, j)
                + 2 * get_px(src, w, h, i, j+1)
                + 1 * get_px(src, w, h, i+1, j-1)
                + 2 * get_px(src, w, h, i+1, j)
                + 1 * get_px(src, w, h, i+1, j+1);
            dst[i * w + j] = sum / 16;
        }
    }
}

// Laplacian kernel: [0 -1 0; -1 4 -1; 0 -1 0]
void laplacian(int src[36], int dst[36], int w, int h) {
    int i;
    int j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            int val;
            val = -1 * get_px(src, w, h, i-1, j)
                + -1 * get_px(src, w, h, i, j-1)
                +  4 * get_px(src, w, h, i, j)
                + -1 * get_px(src, w, h, i, j+1)
                + -1 * get_px(src, w, h, i+1, j);
            dst[i * w + j] = val;
        }
    }
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int compute_sum(int img[36], int size) {
    int i;
    int s;
    s = 0;
    for (i = 0; i < size; i++) {
        s = s + img[i];
    }
    return s;
}

int compute_abs_sum(int img[36], int size) {
    int i;
    int s;
    s = 0;
    for (i = 0; i < size; i++) {
        s = s + abs_val(img[i]);
    }
    return s;
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
    int blurred[36];
    int lap[36];
    int r[6];
    int i;
    int j;

    // Create image with a bright square in center
    for (i = 0; i < 36; i++) img[i] = 30;
    img[1*6+2] = 200; img[1*6+3] = 200;
    img[2*6+2] = 200; img[2*6+3] = 200;
    img[3*6+2] = 200; img[3*6+3] = 200;
    img[4*6+2] = 200; img[4*6+3] = 200;

    printf("Input row 0:\n"); // EXPECT: Input row 0:
    for (j = 0; j < 6; j++) r[j] = img[j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 30 30 30 30 30 30

    printf("Input row 2:\n"); // EXPECT: Input row 2:
    for (j = 0; j < 6; j++) r[j] = img[2*6+j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 30 30 200 200 30 30

    gaussian_blur(img, blurred, 6, 6);

    printf("Blurred row 0:\n"); // EXPECT: Blurred row 0:
    for (j = 0; j < 6; j++) r[j] = blurred[j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 30 40 61 61 40 30

    printf("Blurred row 2:\n"); // EXPECT: Blurred row 2:
    for (j = 0; j < 6; j++) r[j] = blurred[2*6+j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 30 72 157 157 72 30

    printf("Blurred row 5:\n"); // EXPECT: Blurred row 5:
    for (j = 0; j < 6; j++) r[j] = blurred[5*6+j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 30 40 61 61 40 30

    printf("Blur sum: %d\n", compute_sum(blurred, 36)); // EXPECT: Blur sum: 2424
    printf("Blur max: %d\n", compute_max(blurred, 36)); // EXPECT: Blur max: 157
    printf("Blur min: %d\n", compute_min(blurred, 36)); // EXPECT: Blur min: 30

    laplacian(img, lap, 6, 6);

    printf("Laplacian row 1:\n"); // EXPECT: Laplacian row 1:
    for (j = 0; j < 6; j++) r[j] = lap[1*6+j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 -170 340 340 -170 0

    printf("Laplacian row 2:\n"); // EXPECT: Laplacian row 2:
    for (j = 0; j < 6; j++) r[j] = lap[2*6+j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 -170 170 170 -170 0

    printf("Lap abs sum: %d\n", compute_abs_sum(lap, 36)); // EXPECT: Lap abs sum: 4080
    printf("Lap max: %d\n", compute_max(lap, 36)); // EXPECT: Lap max: 340
    printf("Lap min: %d\n", compute_min(lap, 36)); // EXPECT: Lap min: -170

    return 0;
}
