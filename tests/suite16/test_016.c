int printf(const char *fmt, ...);

// Test 016: Median filter for salt-and-pepper noise removal

void sort_array(int arr[9], int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp;
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int get_pixel_clamp(int img[36], int w, int h, int r, int c) {
    if (r < 0) r = 0;
    if (r >= h) r = h - 1;
    if (c < 0) c = 0;
    if (c >= w) c = w - 1;
    return img[r * w + c];
}

void median_filter_3x3(int src[36], int dst[36], int w, int h) {
    int i;
    int j;
    int di;
    int dj;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            int window[9];
            int k;
            k = 0;
            for (di = -1; di <= 1; di++) {
                for (dj = -1; dj <= 1; dj++) {
                    window[k] = get_pixel_clamp(src, w, h, i + di, j + dj);
                    k = k + 1;
                }
            }
            sort_array(window, 9);
            dst[i * w + j] = window[4];
        }
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

int count_noisy(int img[36], int clean[36], int size, int tol) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < size; i++) {
        int d;
        d = img[i] - clean[i];
        if (d < 0) d = -d;
        if (d > tol) c = c + 1;
    }
    return c;
}

int psnr_approx(int orig[36], int filt[36], int size) {
    int i;
    int mse;
    mse = 0;
    for (i = 0; i < size; i++) {
        int d;
        d = orig[i] - filt[i];
        mse = mse + d * d;
    }
    mse = mse / size;
    if (mse == 0) return 9999;
    // Return 10*log10(255^2/mse) approximated as 255*255*10/mse
    return 255 * 255 * 10 / mse;
}

int main() {
    int clean[36];
    int noisy[36];
    int filtered[36];
    int r[6];
    int i;

    // Create clean gradient image
    for (i = 0; i < 36; i++) {
        clean[i] = 100 + (i / 6) * 20;
    }

    // Add salt-and-pepper noise to some pixels
    for (i = 0; i < 36; i++) {
        noisy[i] = clean[i];
    }
    noisy[0] = 255;   // salt
    noisy[7] = 0;     // pepper
    noisy[14] = 255;  // salt
    noisy[21] = 0;    // pepper
    noisy[28] = 255;  // salt
    noisy[35] = 0;    // pepper

    printf("Clean row 0:\n"); // EXPECT: Clean row 0:
    for (i = 0; i < 6; i++) r[i] = clean[i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 100 100 100 100 100 100

    printf("Noisy row 0:\n"); // EXPECT: Noisy row 0:
    for (i = 0; i < 6; i++) r[i] = noisy[i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 255 100 100 100 100 100

    printf("Noisy row 1:\n"); // EXPECT: Noisy row 1:
    for (i = 0; i < 6; i++) r[i] = noisy[1 * 6 + i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 120 0 120 120 120 120

    printf("Clean mean: %d\n", compute_mean(clean, 36)); // EXPECT: Clean mean: 150
    printf("Noisy mean: %d\n", compute_mean(noisy, 36)); // EXPECT: Noisy mean: 146
    printf("Noisy pixels: %d\n", count_noisy(noisy, clean, 36, 10)); // EXPECT: Noisy pixels: 6

    median_filter_3x3(noisy, filtered, 6, 6);

    printf("Filtered row 0:\n"); // EXPECT: Filtered row 0:
    for (i = 0; i < 6; i++) r[i] = filtered[i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 120 100 100 100 100 100

    printf("Filtered row 1:\n"); // EXPECT: Filtered row 1:
    for (i = 0; i < 6; i++) r[i] = filtered[1 * 6 + i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 140 120 120 120 120 120

    printf("Filtered row 3:\n"); // EXPECT: Filtered row 3:
    for (i = 0; i < 6; i++) r[i] = filtered[3 * 6 + i];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 160 160 160 160 160 160

    printf("Filtered mean: %d\n", compute_mean(filtered, 36)); // EXPECT: Filtered mean: 150
    printf("Remaining noisy: %d\n", count_noisy(filtered, clean, 36, 10)); // EXPECT: Remaining noisy: 4
    printf("PSNR noisy: %d\n", psnr_approx(clean, noisy, 36)); // EXPECT: PSNR noisy: 190
    printf("PSNR filtered: %d\n", psnr_approx(clean, filtered, 36)); // EXPECT: PSNR filtered: 14778

    return 0;
}
