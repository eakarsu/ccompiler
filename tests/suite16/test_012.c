int printf(const char *fmt, ...);

// Test 012: Sobel edge detection on a 5x5 image

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int clamp(int val, int lo, int hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

int get_pixel(int img[25], int w, int h, int r, int c) {
    if (r < 0) r = 0;
    if (r >= h) r = h - 1;
    if (c < 0) c = 0;
    if (c >= w) c = w - 1;
    return img[r * w + c];
}

void sobel_x(int src[25], int dst[25], int w, int h) {
    int i;
    int j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            int gx;
            gx = -1 * get_pixel(src, w, h, i - 1, j - 1)
                 + 1 * get_pixel(src, w, h, i - 1, j + 1)
                 - 2 * get_pixel(src, w, h, i, j - 1)
                 + 2 * get_pixel(src, w, h, i, j + 1)
                 - 1 * get_pixel(src, w, h, i + 1, j - 1)
                 + 1 * get_pixel(src, w, h, i + 1, j + 1);
            dst[i * w + j] = gx;
        }
    }
}

void sobel_y(int src[25], int dst[25], int w, int h) {
    int i;
    int j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            int gy;
            gy = -1 * get_pixel(src, w, h, i - 1, j - 1)
                 - 2 * get_pixel(src, w, h, i - 1, j)
                 - 1 * get_pixel(src, w, h, i - 1, j + 1)
                 + 1 * get_pixel(src, w, h, i + 1, j - 1)
                 + 2 * get_pixel(src, w, h, i + 1, j)
                 + 1 * get_pixel(src, w, h, i + 1, j + 1);
            dst[i * w + j] = gy;
        }
    }
}

void edge_magnitude(int gx[25], int gy[25], int dst[25], int size) {
    int i;
    for (i = 0; i < size; i++) {
        dst[i] = abs_val(gx[i]) + abs_val(gy[i]);
    }
}

int compute_sum(int img[25], int size) {
    int i;
    int s;
    s = 0;
    for (i = 0; i < size; i++) {
        s = s + img[i];
    }
    return s;
}

int count_above(int img[25], int size, int thresh) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < size; i++) {
        if (img[i] > thresh) c = c + 1;
    }
    return c;
}

void threshold_image(int src[25], int dst[25], int size, int thresh) {
    int i;
    for (i = 0; i < size; i++) {
        if (src[i] > thresh) {
            dst[i] = 255;
        } else {
            dst[i] = 0;
        }
    }
}

int main() {
    int img[25];
    int gx[25];
    int gy[25];
    int edge[25];
    int binary[25];
    int r[5];
    int i;
    int j;

    // Create a step-edge image: left half dark, right half bright
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (j < 2) {
                img[i * 5 + j] = 20;
            } else if (j == 2) {
                img[i * 5 + j] = 128;
            } else {
                img[i * 5 + j] = 240;
            }
        }
    }

    printf("Input image:\n"); // EXPECT: Input image:
    for (j = 0; j < 5; j++) r[j] = img[0 * 5 + j];
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 20 20 128 240 240
    for (j = 0; j < 5; j++) r[j] = img[1 * 5 + j];
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 20 20 128 240 240
    for (j = 0; j < 5; j++) r[j] = img[2 * 5 + j];
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 20 20 128 240 240
    for (j = 0; j < 5; j++) r[j] = img[3 * 5 + j];
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 20 20 128 240 240
    for (j = 0; j < 5; j++) r[j] = img[4 * 5 + j];
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 20 20 128 240 240

    sobel_x(img, gx, 5, 5);
    sobel_y(img, gy, 5, 5);
    edge_magnitude(gx, gy, edge, 25);

    printf("Sobel Gx row0:\n"); // EXPECT: Sobel Gx row0:
    for (j = 0; j < 5; j++) r[j] = gx[j];
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 0 432 880 448 0

    printf("Sobel Gy row0:\n"); // EXPECT: Sobel Gy row0:
    for (j = 0; j < 5; j++) r[j] = gy[j];
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 0 0 0 0 0

    printf("Edge magnitudes row 0:\n"); // EXPECT: Edge magnitudes row 0:
    for (j = 0; j < 5; j++) r[j] = edge[j];
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 0 432 880 448 0

    printf("Edge magnitudes row 2:\n"); // EXPECT: Edge magnitudes row 2:
    for (j = 0; j < 5; j++) r[j] = edge[2 * 5 + j];
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 0 432 880 448 0

    printf("Edge sum: %d\n", compute_sum(edge, 25)); // EXPECT: Edge sum: 8800
    printf("Pixels above 100: %d\n", count_above(edge, 25, 100)); // EXPECT: Pixels above 100: 15
    printf("Pixels above 200: %d\n", count_above(edge, 25, 200)); // EXPECT: Pixels above 200: 15

    threshold_image(edge, binary, 25, 100);
    printf("Binary edges row 2:\n"); // EXPECT: Binary edges row 2:
    for (j = 0; j < 5; j++) r[j] = binary[2 * 5 + j];
    printf("%d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4]); // EXPECT: 0 255 255 255 0

    printf("Binary sum: %d\n", compute_sum(binary, 25)); // EXPECT: Binary sum: 3825

    return 0;
}
