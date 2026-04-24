int printf(const char *fmt, ...);

/* Test 012: Edge Detection (Sobel-like)
   Apply Sobel-like horizontal and vertical gradient operators
   to detect edges in a 5x5 integer grid.
*/

int img[5][5];
int gx[5][5];
int gy[5][5];
int mag[5][5];

void init_image(void) {
    int i;
    int j;
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            if (j < 3) {
                img[i][j] = 10;
            } else {
                img[i][j] = 100;
            }
        }
    }
}

int abs_val(int x) {
    if (x < 0) return 0 - x;
    return x;
}

void compute_sobel(void) {
    int sx_k[3][3];
    int sy_k[3][3];
    int i;
    int j;
    int ki;
    int kj;
    int sum_x;
    int sum_y;

    sx_k[0][0] = -1; sx_k[0][1] = 0; sx_k[0][2] = 1;
    sx_k[1][0] = -2; sx_k[1][1] = 0; sx_k[1][2] = 2;
    sx_k[2][0] = -1; sx_k[2][1] = 0; sx_k[2][2] = 1;

    sy_k[0][0] = -1; sy_k[0][1] = -2; sy_k[0][2] = -1;
    sy_k[1][0] = 0;  sy_k[1][1] = 0;  sy_k[1][2] = 0;
    sy_k[2][0] = 1;  sy_k[2][1] = 2;  sy_k[2][2] = 1;

    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            gx[i][j] = 0;
            gy[i][j] = 0;
            mag[i][j] = 0;
        }
    }

    for (i = 1; i < 4; i = i + 1) {
        for (j = 1; j < 4; j = j + 1) {
            sum_x = 0;
            sum_y = 0;
            for (ki = -1; ki <= 1; ki = ki + 1) {
                for (kj = -1; kj <= 1; kj = kj + 1) {
                    sum_x = sum_x + img[i + ki][j + kj] * sx_k[ki + 1][kj + 1];
                    sum_y = sum_y + img[i + ki][j + kj] * sy_k[ki + 1][kj + 1];
                }
            }
            gx[i][j] = sum_x;
            gy[i][j] = sum_y;
            mag[i][j] = abs_val(sum_x) + abs_val(sum_y);
        }
    }
}

// EXPECT: sobel done
void print_gradient_x(void) {
    printf("gx[1][1]=%d\n", gx[1][1]);  // EXPECT: gx[1][1]=0
    printf("gx[1][2]=%d\n", gx[1][2]);  // EXPECT: gx[1][2]=360
    printf("gx[1][3]=%d\n", gx[1][3]);  // EXPECT: gx[1][3]=360
    printf("gx[2][2]=%d\n", gx[2][2]);  // EXPECT: gx[2][2]=360
}

void print_gradient_y(void) {
    printf("gy[1][2]=%d\n", gy[1][2]);  // EXPECT: gy[1][2]=0
    printf("gy[2][2]=%d\n", gy[2][2]);  // EXPECT: gy[2][2]=0
}

void print_magnitude(void) {
    printf("mag[1][2]=%d\n", mag[1][2]);  // EXPECT: mag[1][2]=360
    printf("mag[2][2]=%d\n", mag[2][2]);  // EXPECT: mag[2][2]=360
    printf("mag[2][1]=%d\n", mag[2][1]);  // EXPECT: mag[2][1]=0
}

void find_max_edge(void) {
    int i;
    int j;
    int max;
    int mi;
    int mj;
    max = 0;
    mi = 0;
    mj = 0;
    for (i = 1; i < 4; i = i + 1) {
        for (j = 1; j < 4; j = j + 1) {
            if (mag[i][j] > max) {
                max = mag[i][j];
                mi = i;
                mj = j;
            }
        }
    }
    printf("max_edge=%d at %d,%d\n", max, mi, mj);  // EXPECT: max_edge=360 at 1,2
}

void count_edges(int threshold) {
    int i;
    int j;
    int count;
    count = 0;
    for (i = 1; i < 4; i = i + 1) {
        for (j = 1; j < 4; j = j + 1) {
            if (mag[i][j] > threshold) {
                count = count + 1;
            }
        }
    }
    printf("edges_above_%d=%d\n", threshold, count);  // EXPECT: edges_above_100=6
}

int main(void) {
    init_image();
    compute_sobel();
    printf("sobel done\n");
    print_gradient_x();
    print_gradient_y();
    print_magnitude();
    find_max_edge();
    count_edges(100);
    printf("test_012 passed\n");  // EXPECT: test_012 passed
    return 0;
}
