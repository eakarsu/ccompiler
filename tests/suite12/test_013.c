int printf(const char *fmt, ...);

/* Test 013: Image Histogram
   Compute histogram of pixel intensities in an integer grid.
   Pixel values range 0-9 for simplicity.
*/

int img[6][6];
int hist[10];

void init_image(void) {
    int vals[36];
    int i;
    int j;
    int k;
    vals[0] = 0; vals[1] = 1; vals[2] = 2; vals[3] = 3; vals[4] = 4; vals[5] = 5;
    vals[6] = 1; vals[7] = 2; vals[8] = 3; vals[9] = 4; vals[10] = 5; vals[11] = 6;
    vals[12] = 2; vals[13] = 3; vals[14] = 4; vals[15] = 5; vals[16] = 6; vals[17] = 7;
    vals[18] = 3; vals[19] = 4; vals[20] = 5; vals[21] = 6; vals[22] = 7; vals[23] = 8;
    vals[24] = 4; vals[25] = 5; vals[26] = 6; vals[27] = 7; vals[28] = 8; vals[29] = 9;
    vals[30] = 5; vals[31] = 6; vals[32] = 7; vals[33] = 8; vals[34] = 9; vals[35] = 9;

    k = 0;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            img[i][j] = vals[k];
            k = k + 1;
        }
    }
}

void clear_hist(void) {
    int i;
    for (i = 0; i < 10; i = i + 1) {
        hist[i] = 0;
    }
}

void compute_histogram(void) {
    int i;
    int j;
    int v;
    clear_hist();
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            v = img[i][j];
            if (v >= 0 && v < 10) {
                hist[v] = hist[v] + 1;
            }
        }
    }
}

void print_histogram(void) {
    int i;
    for (i = 0; i < 10; i = i + 1) {
        printf("hist[%d]=%d\n", i, hist[i]);
    }
    // EXPECT: hist[0]=1
    // EXPECT: hist[1]=2
    // EXPECT: hist[2]=3
    // EXPECT: hist[3]=4
    // EXPECT: hist[4]=5
    // EXPECT: hist[5]=6
    // EXPECT: hist[6]=5
    // EXPECT: hist[7]=4
    // EXPECT: hist[8]=3
    // EXPECT: hist[9]=3
}

int find_mode(void) {
    int i;
    int max;
    int mode;
    max = 0;
    mode = 0;
    for (i = 0; i < 10; i = i + 1) {
        if (hist[i] > max) {
            max = hist[i];
            mode = i;
        }
    }
    return mode;
}

int compute_mean(void) {
    int i;
    int j;
    int sum;
    sum = 0;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            sum = sum + img[i][j];
        }
    }
    return sum / 36;
}

int compute_min(void) {
    int i;
    int j;
    int mn;
    mn = 999;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            if (img[i][j] < mn) mn = img[i][j];
        }
    }
    return mn;
}

int compute_max(void) {
    int i;
    int j;
    int mx;
    mx = -1;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            if (img[i][j] > mx) mx = img[i][j];
        }
    }
    return mx;
}

int main(void) {
    int mode;
    int mean;
    init_image();
    compute_histogram();
    print_histogram();
    mode = find_mode();
    printf("mode=%d\n", mode);  // EXPECT: mode=5
    mean = compute_mean();
    printf("mean=%d\n", mean);  // EXPECT: mean=4
    printf("min=%d\n", compute_min());  // EXPECT: min=0
    printf("max=%d\n", compute_max());  // EXPECT: max=9
    printf("test_013 passed\n");  // EXPECT: test_013 passed
    return 0;
}
