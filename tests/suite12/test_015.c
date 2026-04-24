int printf(const char *fmt, ...);

/* Test 015: Blur Filter (Box Blur)
   Apply box blur (average of neighbors) to a 6x6 image grid.
   Also test multiple blur passes.
*/

int img[6][6];
int buf[6][6];

void init_image(void) {
    int i;
    int j;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            img[i][j] = 0;
        }
    }
    img[2][2] = 100;
    img[2][3] = 100;
    img[3][2] = 100;
    img[3][3] = 100;
}

void copy_buf_to_img(void) {
    int i;
    int j;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            img[i][j] = buf[i][j];
        }
    }
}

void blur_pass(void) {
    int i;
    int j;
    int ki;
    int kj;
    int sum;
    int count;
    int ni;
    int nj;

    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            sum = 0;
            count = 0;
            for (ki = -1; ki <= 1; ki = ki + 1) {
                for (kj = -1; kj <= 1; kj = kj + 1) {
                    ni = i + ki;
                    nj = j + kj;
                    if (ni >= 0 && ni < 6 && nj >= 0 && nj < 6) {
                        sum = sum + img[ni][nj];
                        count = count + 1;
                    }
                }
            }
            buf[i][j] = sum / count;
        }
    }
    copy_buf_to_img();
}

int sum_image(void) {
    int i;
    int j;
    int s;
    s = 0;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            s = s + img[i][j];
        }
    }
    return s;
}

int max_pixel(void) {
    int i;
    int j;
    int mx;
    mx = 0;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            if (img[i][j] > mx) mx = img[i][j];
        }
    }
    return mx;
}

int count_nonzero(void) {
    int i;
    int j;
    int c;
    c = 0;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            if (img[i][j] > 0) c = c + 1;
        }
    }
    return c;
}

void print_center_region(void) {
    int i;
    int j;
    printf("center region:\n");
    for (i = 1; i < 5; i = i + 1) {
        for (j = 1; j < 5; j = j + 1) {
            if (j > 1) printf(" ");
            printf("%d", img[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int s0;
    int s1;
    int nz;

    init_image();
    s0 = sum_image();
    printf("initial_sum=%d\n", s0);  // EXPECT: initial_sum=400
    printf("initial_max=%d\n", max_pixel());  // EXPECT: initial_max=100
    printf("initial_nonzero=%d\n", count_nonzero());  // EXPECT: initial_nonzero=4

    blur_pass();
    s1 = sum_image();
    printf("blur1_max=%d\n", max_pixel());  // EXPECT: blur1_max=44
    printf("blur1_nonzero=%d\n", count_nonzero());  // EXPECT: blur1_nonzero=16
    printf("blur1_center=%d\n", img[2][2]);  // EXPECT: blur1_center=44

    blur_pass();
    printf("blur2_max=%d\n", max_pixel());  // EXPECT: blur2_max=30
    nz = count_nonzero();
    printf("blur2_nonzero=%d\n", nz);  // EXPECT: blur2_nonzero=36
    printf("blur2_center=%d\n", img[2][2]);  // EXPECT: blur2_center=30

    blur_pass();
    printf("blur3_max=%d\n", max_pixel());  // EXPECT: blur3_max=22
    printf("blur3_center=%d\n", img[2][2]);  // EXPECT: blur3_center=22

    printf("test_015 passed\n");  // EXPECT: test_015 passed
    return 0;
}
