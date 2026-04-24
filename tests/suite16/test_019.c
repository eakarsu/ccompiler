int printf(const char *fmt, ...);

// Test 019: Image rotation (90/180/270 degrees) and flip operations

void rotate_90cw(int src[36], int dst[36], int w, int h) {
    // 90 clockwise: dst[j][h-1-i] = src[i][j]
    int i;
    int j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            dst[j * h + (h - 1 - i)] = src[i * w + j];
        }
    }
}

void rotate_180(int src[36], int dst[36], int w, int h) {
    int i;
    int j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            dst[(h - 1 - i) * w + (w - 1 - j)] = src[i * w + j];
        }
    }
}

void rotate_270cw(int src[36], int dst[36], int w, int h) {
    // 270 clockwise = 90 counter-clockwise: dst[w-1-j][i] = src[i][j]
    int i;
    int j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            dst[(w - 1 - j) * h + i] = src[i * w + j];
        }
    }
}

void flip_horizontal(int src[36], int dst[36], int w, int h) {
    int i;
    int j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            dst[i * w + (w - 1 - j)] = src[i * w + j];
        }
    }
}

void flip_vertical(int src[36], int dst[36], int w, int h) {
    int i;
    int j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            dst[(h - 1 - i) * w + j] = src[i * w + j];
        }
    }
}

int images_equal(int a[36], int b[36], int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int compute_sum(int img[36], int size) {
    int i;
    int s;
    s = 0;
    for (i = 0; i < size; i++) s = s + img[i];
    return s;
}

int main() {
    int img[36];
    int rot90[36];
    int rot180[36];
    int rot270[36];
    int fliph[36];
    int flipv[36];
    int temp[36];
    int r[6];
    int i;
    int j;

    // Create 6x6 image with unique values
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            img[i * 6 + j] = i * 10 + j;
        }
    }

    printf("Input row 0:\n"); // EXPECT: Input row 0:
    for (j = 0; j < 6; j++) r[j] = img[j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 1 2 3 4 5

    printf("Input row 5:\n"); // EXPECT: Input row 5:
    for (j = 0; j < 6; j++) r[j] = img[5*6+j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 50 51 52 53 54 55

    rotate_90cw(img, rot90, 6, 6);
    printf("Rot90 row 0:\n"); // EXPECT: Rot90 row 0:
    for (j = 0; j < 6; j++) r[j] = rot90[j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 50 40 30 20 10 0

    printf("Rot90 row 5:\n"); // EXPECT: Rot90 row 5:
    for (j = 0; j < 6; j++) r[j] = rot90[5*6+j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 55 45 35 25 15 5

    rotate_180(img, rot180, 6, 6);
    printf("Rot180 row 0:\n"); // EXPECT: Rot180 row 0:
    for (j = 0; j < 6; j++) r[j] = rot180[j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 55 54 53 52 51 50

    rotate_270cw(img, rot270, 6, 6);
    printf("Rot270 row 0:\n"); // EXPECT: Rot270 row 0:
    for (j = 0; j < 6; j++) r[j] = rot270[j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 5 15 25 35 45 55

    // Verify: rot90 applied 4 times == identity
    rotate_90cw(img, rot90, 6, 6);
    rotate_90cw(rot90, temp, 6, 6);
    rotate_90cw(temp, rot90, 6, 6);
    rotate_90cw(rot90, temp, 6, 6);
    printf("4x rot90 equals orig: %d\n", images_equal(img, temp, 36)); // EXPECT: 4x rot90 equals orig: 1

    // Verify: rot180 applied 2 times == identity
    rotate_180(img, rot180, 6, 6);
    rotate_180(rot180, temp, 6, 6);
    printf("2x rot180 equals orig: %d\n", images_equal(img, temp, 36)); // EXPECT: 2x rot180 equals orig: 1

    flip_horizontal(img, fliph, 6, 6);
    printf("FlipH row 0:\n"); // EXPECT: FlipH row 0:
    for (j = 0; j < 6; j++) r[j] = fliph[j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 5 4 3 2 1 0

    flip_vertical(img, flipv, 6, 6);
    printf("FlipV row 0:\n"); // EXPECT: FlipV row 0:
    for (j = 0; j < 6; j++) r[j] = flipv[j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 50 51 52 53 54 55

    // Double flip == identity
    flip_horizontal(fliph, temp, 6, 6);
    printf("2x flipH equals orig: %d\n", images_equal(img, temp, 36)); // EXPECT: 2x flipH equals orig: 1

    flip_vertical(flipv, temp, 6, 6);
    printf("2x flipV equals orig: %d\n", images_equal(img, temp, 36)); // EXPECT: 2x flipV equals orig: 1

    // Sum is invariant under rotation/flip
    printf("Sum orig: %d\n", compute_sum(img, 36)); // EXPECT: Sum orig: 990
    printf("Sum rot90: %d\n", compute_sum(rot90, 36)); // EXPECT: Sum rot90: 990
    printf("Sum flipH: %d\n", compute_sum(fliph, 36)); // EXPECT: Sum flipH: 990

    return 0;
}
