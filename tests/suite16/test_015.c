int printf(const char *fmt, ...);

// Test 015: Morphological erosion and dilation on a binary 6x6 image

void erode_3x3(int src[36], int dst[36], int w, int h) {
    int i;
    int j;
    int di;
    int dj;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            int min_val;
            min_val = 255;
            for (di = -1; di <= 1; di++) {
                for (dj = -1; dj <= 1; dj++) {
                    int ni;
                    int nj;
                    ni = i + di;
                    nj = j + dj;
                    if (ni >= 0 && ni < h && nj >= 0 && nj < w) {
                        if (src[ni * w + nj] < min_val) {
                            min_val = src[ni * w + nj];
                        }
                    } else {
                        min_val = 0;
                    }
                }
            }
            dst[i * w + j] = min_val;
        }
    }
}

void dilate_3x3(int src[36], int dst[36], int w, int h) {
    int i;
    int j;
    int di;
    int dj;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            int max_val;
            max_val = 0;
            for (di = -1; di <= 1; di++) {
                for (dj = -1; dj <= 1; dj++) {
                    int ni;
                    int nj;
                    ni = i + di;
                    nj = j + dj;
                    if (ni >= 0 && ni < h && nj >= 0 && nj < w) {
                        if (src[ni * w + nj] > max_val) {
                            max_val = src[ni * w + nj];
                        }
                    }
                }
            }
            dst[i * w + j] = max_val;
        }
    }
}

int count_nonzero(int img[36], int size) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < size; i++) {
        if (img[i] > 0) c = c + 1;
    }
    return c;
}

int images_equal(int a[36], int b[36], int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void copy_img(int src[36], int dst[36], int size) {
    int i;
    for (i = 0; i < size; i++) {
        dst[i] = src[i];
    }
}

int main() {
    int img[36];
    int eroded[36];
    int dilated[36];
    int opened[36];
    int closed[36];
    int temp[36];
    int r[6];
    int i;

    // Create a binary image with a cross pattern
    for (i = 0; i < 36; i++) img[i] = 0;
    // Horizontal bar
    img[2 * 6 + 1] = 255; img[2 * 6 + 2] = 255; img[2 * 6 + 3] = 255; img[2 * 6 + 4] = 255;
    img[3 * 6 + 1] = 255; img[3 * 6 + 2] = 255; img[3 * 6 + 3] = 255; img[3 * 6 + 4] = 255;
    // Vertical bar
    img[1 * 6 + 2] = 255; img[1 * 6 + 3] = 255;
    img[4 * 6 + 2] = 255; img[4 * 6 + 3] = 255;

    printf("Input cross:\n"); // EXPECT: Input cross:
    for (i = 0; i < 6; i++) r[i] = img[0 * 6 + i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 0 0 0 0 0
    for (i = 0; i < 6; i++) r[i] = img[1 * 6 + i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 0 255 255 0 0
    for (i = 0; i < 6; i++) r[i] = img[2 * 6 + i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 255 255 255 255 0
    for (i = 0; i < 6; i++) r[i] = img[3 * 6 + i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 255 255 255 255 0
    for (i = 0; i < 6; i++) r[i] = img[4 * 6 + i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 0 255 255 0 0
    for (i = 0; i < 6; i++) r[i] = img[5 * 6 + i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 0 0 0 0 0

    printf("Input nonzero: %d\n", count_nonzero(img, 36)); // EXPECT: Input nonzero: 12

    erode_3x3(img, eroded, 6, 6);
    printf("Eroded nonzero: %d\n", count_nonzero(eroded, 36)); // EXPECT: Eroded nonzero: 0

    printf("Eroded row 2:\n"); // EXPECT: Eroded row 2:
    for (i = 0; i < 6; i++) r[i] = eroded[2 * 6 + i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 0 0 0 0 0

    dilate_3x3(img, dilated, 6, 6);
    printf("Dilated nonzero: %d\n", count_nonzero(dilated, 36)); // EXPECT: Dilated nonzero: 32

    printf("Dilated row 0:\n"); // EXPECT: Dilated row 0:
    for (i = 0; i < 6; i++) r[i] = dilated[0 * 6 + i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 255 255 255 255 0

    printf("Dilated row 2:\n"); // EXPECT: Dilated row 2:
    for (i = 0; i < 6; i++) r[i] = dilated[2 * 6 + i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 255 255 255 255 255 255

    // Opening = erode then dilate
    erode_3x3(img, temp, 6, 6);
    dilate_3x3(temp, opened, 6, 6);
    printf("Opened nonzero: %d\n", count_nonzero(opened, 36)); // EXPECT: Opened nonzero: 0

    // Closing = dilate then erode
    dilate_3x3(img, temp, 6, 6);
    erode_3x3(temp, closed, 6, 6);
    printf("Closed nonzero: %d\n", count_nonzero(closed, 36)); // EXPECT: Closed nonzero: 12

    printf("Closed row 2:\n"); // EXPECT: Closed row 2:
    for (i = 0; i < 6; i++) r[i] = closed[2 * 6 + i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 255 255 255 255 0

    return 0;
}
