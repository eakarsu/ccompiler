int printf(const char *fmt, ...);

// Test 014: Floyd-Steinberg dithering on a 6x6 grayscale image

int clamp(int val, int lo, int hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

void copy_image(int src[36], int dst[36], int size) {
    int i;
    for (i = 0; i < size; i++) {
        dst[i] = src[i];
    }
}

void floyd_steinberg(int img[36], int w, int h) {
    int y;
    int x;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            int old_val;
            int new_val;
            int err;
            old_val = img[y * w + x];
            if (old_val > 127) {
                new_val = 255;
            } else {
                new_val = 0;
            }
            err = old_val - new_val;
            img[y * w + x] = new_val;

            if (x + 1 < w) {
                img[y * w + x + 1] = clamp(img[y * w + x + 1] + err * 7 / 16, 0, 255);
            }
            if (y + 1 < h && x > 0) {
                img[(y + 1) * w + x - 1] = clamp(img[(y + 1) * w + x - 1] + err * 3 / 16, 0, 255);
            }
            if (y + 1 < h) {
                img[(y + 1) * w + x] = clamp(img[(y + 1) * w + x] + err * 5 / 16, 0, 255);
            }
            if (y + 1 < h && x + 1 < w) {
                img[(y + 1) * w + x + 1] = clamp(img[(y + 1) * w + x + 1] + err * 1 / 16, 0, 255);
            }
        }
    }
}

int count_value(int img[36], int size, int val) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < size; i++) {
        if (img[i] == val) c = c + 1;
    }
    return c;
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

int mean_abs_error(int a[36], int b[36], int size) {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < size; i++) {
        int d;
        d = a[i] - b[i];
        if (d < 0) d = -d;
        sum = sum + d;
    }
    return sum / size;
}

int main() {
    int img[36];
    int orig[36];
    int i;
    int r[6];

    // Create gradient image 0..250
    for (i = 0; i < 36; i++) {
        img[i] = i * 250 / 35;
    }

    copy_image(img, orig, 36);

    printf("Original row 0:\n"); // EXPECT: Original row 0:
    for (i = 0; i < 6; i++) r[i] = orig[i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 7 14 21 28 35

    printf("Original row 5:\n"); // EXPECT: Original row 5:
    for (i = 0; i < 6; i++) r[i] = orig[5 * 6 + i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 214 221 228 235 242 250

    printf("Original mean: %d\n", compute_mean(orig, 36)); // EXPECT: Original mean: 124

    floyd_steinberg(img, 6, 6);

    printf("Dithered row 0:\n"); // EXPECT: Dithered row 0:
    for (i = 0; i < 6; i++) r[i] = img[i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 0 0 0 0 0

    printf("Dithered row 3:\n"); // EXPECT: Dithered row 3:
    for (i = 0; i < 6; i++) r[i] = img[3 * 6 + i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 255 0 255 0 255 0

    printf("Dithered row 5:\n"); // EXPECT: Dithered row 5:
    for (i = 0; i < 6; i++) r[i] = img[5 * 6 + i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 255 0 255 255 255 255

    printf("White pixels: %d\n", count_value(img, 36, 255)); // EXPECT: White pixels: 18
    printf("Black pixels: %d\n", count_value(img, 36, 0)); // EXPECT: Black pixels: 18
    printf("Dithered mean: %d\n", compute_mean(img, 36)); // EXPECT: Dithered mean: 127
    printf("Mean abs error: %d\n", mean_abs_error(orig, img, 36)); // EXPECT: Mean abs error: 78

    // Verify dithering only produces 0 or 255
    int all_binary;
    all_binary = 1;
    for (i = 0; i < 36; i++) {
        if (img[i] != 0 && img[i] != 255) {
            all_binary = 0;
        }
    }
    printf("All binary: %d\n", all_binary); // EXPECT: All binary: 1

    return 0;
}
