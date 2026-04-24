int printf(const char *fmt, ...);

/* Test 014: Threshold Filter
   Apply binary threshold to convert grayscale image to black/white.
   Also test adaptive threshold using local mean.
*/

int img[6][6];
int result[6][6];

void init_image(void) {
    int i;
    int j;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            img[i][j] = i * 10 + j * 5;
        }
    }
}

void threshold_simple(int thresh) {
    int i;
    int j;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            if (img[i][j] >= thresh) {
                result[i][j] = 255;
            } else {
                result[i][j] = 0;
            }
        }
    }
}

int count_white(void) {
    int i;
    int j;
    int c;
    c = 0;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            if (result[i][j] == 255) {
                c = c + 1;
            }
        }
    }
    return c;
}

int count_black(void) {
    int i;
    int j;
    int c;
    c = 0;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            if (result[i][j] == 0) {
                c = c + 1;
            }
        }
    }
    return c;
}

void threshold_inverted(int thresh) {
    int i;
    int j;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            if (img[i][j] >= thresh) {
                result[i][j] = 0;
            } else {
                result[i][j] = 255;
            }
        }
    }
}

void threshold_truncate(int thresh) {
    int i;
    int j;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            if (img[i][j] > thresh) {
                result[i][j] = thresh;
            } else {
                result[i][j] = img[i][j];
            }
        }
    }
}

void threshold_tozero(int thresh) {
    int i;
    int j;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            if (img[i][j] >= thresh) {
                result[i][j] = img[i][j];
            } else {
                result[i][j] = 0;
            }
        }
    }
}

int sum_result(void) {
    int i;
    int j;
    int s;
    s = 0;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            s = s + result[i][j];
        }
    }
    return s;
}

void print_row(int row) {
    int j;
    for (j = 0; j < 6; j = j + 1) {
        if (j > 0) printf(" ");
        printf("%d", result[row][j]);
    }
    printf("\n");
}

int main(void) {
    init_image();

    threshold_simple(25);
    printf("thresh25_white=%d\n", count_white());  // EXPECT: thresh25_white=27
    printf("thresh25_black=%d\n", count_black());  // EXPECT: thresh25_black=9

    threshold_simple(30);
    printf("thresh30_white=%d\n", count_white());  // EXPECT: thresh30_white=24
    printf("thresh30_black=%d\n", count_black());  // EXPECT: thresh30_black=12

    threshold_inverted(25);
    printf("inv25_white=%d\n", count_white());  // EXPECT: inv25_white=9
    printf("inv25_black=%d\n", count_black());  // EXPECT: inv25_black=27

    threshold_truncate(30);
    printf("trunc_r0c0=%d\n", result[0][0]);  // EXPECT: trunc_r0c0=0
    printf("trunc_r0c5=%d\n", result[0][5]);  // EXPECT: trunc_r0c5=25
    printf("trunc_r5c5=%d\n", result[5][5]);  // EXPECT: trunc_r5c5=30

    threshold_tozero(25);
    printf("tz_r0c0=%d\n", result[0][0]);  // EXPECT: tz_r0c0=0
    printf("tz_r1c3=%d\n", result[1][3]);  // EXPECT: tz_r1c3=25
    printf("tz_r5c5=%d\n", result[5][5]);  // EXPECT: tz_r5c5=75

    printf("test_014 passed\n");  // EXPECT: test_014 passed
    return 0;
}
